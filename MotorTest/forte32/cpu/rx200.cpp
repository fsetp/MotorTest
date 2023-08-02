////////////////////////////////////////////////////////////////////////////////
//
//	2007.11.25	スタックチェック追加
//				H8S/2000系として扱う
//	2007.12.01	割り込み中フラグを追加
//				RestoreSystem関数追加
//	2007.12.16	WaitUs()追加
//	2007.12.22	SwitchTask()がスタックを消費するため、小分けした
//	2007.12.24	SwitchTask()でER0を保存していなかった
//	2008.04.29	g_nPrevTask追加
//	2008.04.29	割込みモードによって、割込みスタックの解釈を行う様にした
//				g_nIntMode追加
//	2008.05.17	g_nPrevTask削除（FIFO導入で使わない）
//	2008.07.06	スタックチェックの方法を変更
//				変数がforte使用側の変数と衝突しないように、_を付加した
//	2023.06.17	void CTaskCtrl::InitializeClock()
//				内蔵クロック設定を追加
//	2023.06.18	void CTaskCtrl::InitializeInterval()
//				タイマ割り込み許可をsysobjを使用するように変更
//	2023.08.02	void CTaskCtrl::InitializeClock()
//				void CTaskCtrl::InitializeInterval()
//				内蔵クロック設定追加
//

#include "fortedef.h"
#include "forte32.h"
#include "sysobj.h"

int				g_nNextTask   = INVALID_TASK_ID;	// 切替先タスク番号
void*			g_pTaskReg    = 0;					// 初めての起動時は0になっている必要がある
unsigned long	g_org_stack   = 0;					// 非タスクのスタックポインタ保存先
int				g_nWaitUsLoop = 0;					//

extern	CTaskCtrl	g_TaskCtrl;						// OSオブジェクト

//#define	SIMULATOR_USE	true
#define	SIMULATOR_USE	false

////////////////////////////////////////
// タスクの生成
//	非タスクあるいはシステムタスクから呼ばれる
//
//	引数	pFunc		タスク関数ポインタ
//			nPriority	タスクプライオリティ	THREAD_PRIORITY_*
//			nStackSize	タスク固有スタックサイズ
//			pVoid		タスク関数に渡すパラメータ
//			pDestroy	タスク終了時に呼び出される関数ポインタ
//
//	戻値	true		成功
//			false		失敗（ヒープメモリが足りない）
//
bool CTask::CreateTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	// TCBの生成
	m_TaskFlag.nPriority = nPriority;
	m_TaskFlag.nState    = TS_SUSPEND;
	m_nLockMs            = 0;
	m_pStackFrame        = new char[nStackSize];
	m_nStackSize         = nStackSize;

	// スタックの確保ができなかった
	if (m_pStackFrame == NULL) {
		g_nLastError = ERROR_NO_STACK_MEMORY;
		return false;
	}

	// スタックチェックシグネチャセット
	FILL_STACK_SIGN(m_pStackFrame, m_nStackSize);

	// スタックフレームの初期化
	//	タスク関数終了時の処理アドレスをスタックに積んでおく
	//
	//	-8	PC
	//	-4	PSW
	//
	unsigned long* pStack;
	pStack  = (unsigned long*)(&m_pStackFrame[nStackSize - 4]);
	*pStack = 0x00130000;
	pStack  = (unsigned long*)(&m_pStackFrame[nStackSize - 8]);
	*pStack = (unsigned long)pDestroy;

	// レジスタの初期値
	//	ER0はタスクに渡すパラメータ
	//	ER1～ER6まで0に
	//	ER7はスタックポインタ
	//
	m_dwReg[0]  = (unsigned long)pStack;	// SP
	m_dwReg[1]  = (unsigned long)pVoid;		// タスク関数引数にパラメータを設定
	m_dwReg[2]  = 0;
	m_dwReg[3]  = 0;
	m_dwReg[4]  = 0;
	m_dwReg[5]  = 0;
	m_dwReg[6]  = 0;
	m_dwReg[7]  = 0;
	m_dwReg[8]  = 0;
	m_dwReg[9]  = 0;
	m_dwReg[10] = 0;
	m_dwReg[11] = 0;
	m_dwReg[12] = 0;
	m_dwReg[13] = 0;
	m_dwReg[14] = 0;
	m_dwReg[15] = 0;
	m_dwReg[16] = (unsigned long)pFunc;		// PC
	m_dwReg[17] = 0x00130000;				// PSW
	m_dwReg[18] = 0;						// ACCL
	m_dwReg[19] = 0;						// ACCH
	m_dwReg[20] = 1;						// virgin mark

	return true;
}

////////////////////////////////////////
//
void CTaskCtrl::InitializeClock()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

#if	SIMULATOR_USE == false

// 外部クロック
//
#if OSC_EXTERNAL == true

	CCpuObject::EnableWriteProtectRegister0(true);
	CCpuObject::EnableWriteProtectRegister1(true);

	CCpuObject::SelectClockSource(CCpuObject::CLKSRC_MAINOSC);
	CCpuObject::SelectPllInputDivider(CCpuObject::PLL_DIV2);		// PLL入力分周比： 2分周
	CCpuObject::SelectPllOutputMultiplier(CCpuObject::PLL_x8);		// 周波数逓倍率 ： 8逓倍

	CCpuObject::SelectSystemClock(CCpuObject::ICLK_DIV1);			// ICLK ： 48MHz
	CCpuObject::SelectPeripheralClockA(CCpuObject::PCLK_DIV1);		// PCLKA： 48MHz
	CCpuObject::SelectPeripheralClockB(CCpuObject::PCLK_DIV2);		// PCLKB： 24MHz
	CCpuObject::SelectPeripheralClockD(CCpuObject::PCLK_DIV1);		// PCLKD： 48MHz
	CCpuObject::SelectBusClock(CCpuObject::BCLK_DIV2);				// BCLK ： 24MHz
	CCpuObject::SelectFlashIFClock(CCpuObject::FCLK_DIV2);			// FCLK ： 24MHz

	CCpuObject::StopMainClockOscillator(true);

	// メインクロック発振中?
	//
	while (CCpuObject::IsStabilizeMainClock())
		;

	CCpuObject::EnableExternalOscillationInput(true);
	CCpuObject::EnableMemoryWait(true);
	CCpuObject::SelectMainClockOscillatorWaitTime(CCpuObject::CYCLE_32768);

	// PLL動作
	//
	CCpuObject::StopPll(false);
	while (!CCpuObject::IsStabilizePll())
		;

	// メインクロック発振
	CCpuObject::StopMainClockOscillator(false);
	while (!CCpuObject::IsStabilizeMainClock())
		;

// 内蔵クロック
//
#else

	CCpuObject::EnableWriteProtectRegister0(true);
	CCpuObject::EnableWriteProtectRegister1(true);

	CCpuObject::SelectClockSource(CCpuObject::CLKSRC_HOCO);

	CCpuObject::SelectSystemClock(CCpuObject::ICLK_DIV1);			// ICLK ： 54MHz
	CCpuObject::SelectPeripheralClockA(CCpuObject::PCLK_DIV1);		// PCLKA： 54MHz
	CCpuObject::SelectPeripheralClockB(CCpuObject::PCLK_DIV2);		// PCLKB： 27MHz
	CCpuObject::SelectPeripheralClockD(CCpuObject::PCLK_DIV1);		// PCLKD： 54MHz
	CCpuObject::SelectBusClock(CCpuObject::BCLK_DIV2);				// BCLK ： 27MHz
	CCpuObject::SelectFlashIFClock(CCpuObject::FCLK_DIV2);			// FCLK ： 27MHz

	CCpuObject::EnableExternalOscillationInput(false);
	CCpuObject::EnableMemoryWait(true);
	CCpuObject::SelectMainClockOscillatorWaitTime(CCpuObject::CYCLE_32768);

	// 高速オンチップオシレータ
	//
	CCpuObject::EnableMemoryWait(true);
	CCpuObject::SelectHOCOFrequency(CCpuObject::HOCO_54MHZ);
	CCpuObject::StopHOCO(false);
	while (!CCpuObject::IsStabilizeHOCO())
		;

#endif

	// PLL回路の選択
//	CCpuObject::SelectClockSource(CCpuObject::CLKSRC_PLL);

	CCpuObject::EnableWriteProtectRegister0(false);
	CCpuObject::EnableWriteProtectRegister1(false);

#endif
}

////////////////////////////////////////
// インターバルタイマの初期化
//	CMTを使用
//
//	クロックソース	PCLKB	54 -> 27
//
void CTaskCtrl::InitializeInterval()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	// Enable CMT0
	//
	CCpuObject::EnableWriteProtectRegister1(true);
	CCmtObject::RunModule(CCmtObject::CMT_U0, true);

#if	OSC_EXTERNAL == true
	// Main Clock         = OSC / 2 * 8    = 48MHz
	// Peripheral Clock B = Main Clock / 2 = 24MHz
	// CMT Clock          = PCLKB / 8      = 3MHz
	// 1000 * 1000 / (1000 / CMT Clock)
	//
	unsigned int nDefCnt = (((m_nCpuClockFreq * 1000 / 2 * 8) / 2) / 8) / 1;

#else
	// Main Clock                          = 54MHz
	// Peripheral Clock B = Main Clock / 2 = 27MHz
	// CMT Clock          = PCLKB / 8      = 3.375MHz
	// 1000 * 1000 / (1000 / CMT Clock)
	//
	unsigned int nDefCnt = (((m_nCpuClockFreq * 1000) / PCLK_DIVISER) / 8);

#endif

	//
//	IEN(CMT0, CMI0) = 0;
//	IR(CMT0, CMI0)  = 0;
//	IPR(CMT0, CMI0) = 1;
//	IEN(CMT0, CMI0) = 1;

	CIcuObject::EnableInterrupt(28, false);				// 割込み禁止
	CIcuObject::CleatInterruptRequest(28);				// 割込み要求のクリア
	CIcuObject::SetPriority(28, 1);						// 割込み優先順位の設定
	CIcuObject::EnableInterrupt(28, true);				// 割込み許可

	CCmtObject::StartCounter(0, false);					// 停止
	CCmtObject::SelectClock(0, CCmtObject::PCLK_1_8);	// 8分周
	CCmtObject::SetCounter(0, 0);						// カウンタのクリア
	CCmtObject::SetInterval(0, nDefCnt);				// インターバルの設定
	CCmtObject::StartCounter(0, true);					// 開始
	CCmtObject::EnableInterrupt(0, true);				// コンペアマッチ割込み許可
}

////////////////////////////////////////
// ウォッチドッグタイマの初期化
//	IWDTTを使用
//
void CTaskCtrl::InitializeWatchDogTimer()
{
	CIwdtObject::SetTimeoutPeriod(CIwdtObject::CYCLE_512);				// 512 Cycle
	CIwdtObject::SelectClock(CIwdtObject::IWDTCLK_1_256);				// IWDTTCLK / 256
	CIwdtObject::SetWindowEndPosition(CIwdtObject::END_POS_25);			// 終了25%
	CIwdtObject::SetWindowStartPosition(CIwdtObject::START_POS_100);	// 開始100%

	CIwdtObject::EnableNonMaskableInterrupt(true);		// ノンマスカブル割込み許可
	CIwdtObject::StopSleepModeCount(false);				// カウント停止無効
}

////////////////////////////////////////
// ウォッチドッグタイマのリフレッシュ
//
void CTaskCtrl::RefreshWatchDogTimer()
{
	CIwdtObject::RefreshCounter();
}

////////////////////////////////////////
// μSの待ち
//	割込み禁止にして、サイクル数を消費する
//
//	引数	us	μ秒
//
#define	PLL_DIV			2
#define	PLL_MUL			8
#define	ICLK_DIV		2
#define	WU_FIXED_STATE	120		// 100 + 6(int) + 6(rte) + 5 + 1
#define	WU_LOOP_STATE	10		// 10
void CTaskCtrl::WaitUs(int us)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	//	固定ステート	事前計算と呼出、戻りステート
	//	ループステート	
	//

	
	// システムクロック
	//	外部クロック→PLLクロック→ICLK
	//
	int nIclk = m_nCpuClockFreq / PLL_DIV * PLL_MUL / ICLK_DIV;

	// ステート数
	//	固定ステート + ループステート x n = トータルステート
	//
	int nTotalState = us * 1000 / nIclk;

	// ループ回数
	//
	int nLoopTimes = (nTotalState - WU_FIXED_STATE) / WU_LOOP_STATE;

	g_nWaitUsLoop = nLoopTimes;

	int_exception(VECT_WAIT_US);
}

////////////////////////////////////////
// 初めてSwitchTaskが実行されたときの文脈へ戻す
//
void CTaskCtrl::RestorSystem()
{
	int_exception(VECT_RESTORE_SYSTEM);
}

////////////////////////////////////////
//
void CTaskCtrl::SwitchTask()
{
	int_exception(VECT_TASK_SWITCH);		// タスク切替
}

////////////////////////////////////////
//
void CTaskCtrl::ChangeSupervisor()
{
	int_exception(VECT_SUPERVISOR);			// スーパーバイザーへ切替
}

////////////////////////////////////////
//
void CTaskCtrl::ChangeUser()
{
	chg_pmusr();							//
}

////////////////////////////////////////
//
void CTaskCtrl::EnterSystemCritical()
{
	int_exception(VECT_ENTER_CRITICAL);		//
}

////////////////////////////////////////
//
void CTaskCtrl::LeaveSystemCritical()
{
	int_exception(VECT_LEAVE_CRITICAL);		//
}

////////////////////////////////////////
//
void CTaskCtrl::DisableInterrupt()
{
	int_exception(VECT_DISABLE_INTERRUPT);	//
}

////////////////////////////////////////
//
void CTaskCtrl::EnableInterrupt()
{
	int_exception(VECT_ENABLE_INTERRUPT);	//
}

////////////////////////////////////////
// タスクレジスタを切り替える
//	上にあるSwitchTask()から呼ばれる
//	切り替えたいタスクのレジスタ配列アドレスを取得する
//
void CTaskCtrl::SetupTaskParam()
{
	// 割り込み中でなければ、エラーとする
	//
	INSIDE_INTERRUPT_CHECK();

	// スタックフレームチェック
	CTask* pTask = g_TaskCtrl.GetCurrentTask();
	if (pTask)
		CHECK_STACK_SIGN(pTask->m_pStackFrame, g_TaskCtrl.m_nTaskID, ERROR_TASK_STACK_OVERFLOW);

	// ユーザータスクからの切替なら、システムタスクへ切り替える
	//	あわせて、切替前のタスクIDの保存
	if (g_TaskCtrl.m_nTaskID != SYSTEM_TASK_ID)
		g_nNextTask = SYSTEM_TASK_ID;

	// カレントタスクに設定
	g_TaskCtrl.m_nTaskID = g_nNextTask;

	// レジスタエリア
	pTask = g_TaskCtrl.GetTask(g_nNextTask);
	g_pTaskReg = pTask->GetTaskReg();
}

////////////////////////////////////////
// インターバル割込み(IWDT)
//
void CTaskCtrl::IntervalIntProc()
{
	ENTER_INTERRUPT();

	// SLEEPなどで使用するmsカウンタの減算
	//
	g_TaskCtrl.DecrementWaitMs();

	// インターバルコールバックサービス
	//
	g_TaskCtrl.IntervalTimerScan();

	// チックカウントのインクリメント
	//
	g_TaskCtrl.m_dwTickCount++;

	LEAVE_INTERRUPT();
}
