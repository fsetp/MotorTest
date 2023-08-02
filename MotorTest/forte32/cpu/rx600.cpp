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
//

#include "fortedef.h"
#include "forte32.h"
#include "clock_ctrl.h"
#include "cmt_ctrl.h"

int				g_nNextTask = INVALID_TASK_ID;	// 切替先タスク番号
void*			g_pTaskReg  = 0;				// 初めての起動時は0になっている必要がある
unsigned long	g_org_stack = 0;				// 非タスクのスタックポインタ保存先
int				g_nWaitUsLoop = 0;				//

extern	CTaskCtrl	g_TaskCtrl;					// OSオブジェクト

//#define	SIMULATOR_USE	true
#define	SIMULATOR_USE	false

////////////////////////////////////////
// タスクの生成
//		非タスクあるいはシステムタスクから呼ばれる
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
	//		-8	PC
	//		-4	PSW
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
	m_dwReg[1]  = (unsigned long)pVoid;	// タスク関数引数にパラメータを設定
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
	m_dwReg[16] = (unsigned long)pFunc;	// PC
	m_dwReg[17] = 0x00130000;			// PSW
	m_dwReg[18] = 0;					// ACCL
	m_dwReg[19] = 0;					// ACCH
	m_dwReg[20] = 1;					// virgin mark

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

	CClockCtrl::SelectClockMultiply(CClockCtrl::ICK, ICLK_MULTIPLY);
	CClockCtrl::SelectClockMultiply(CClockCtrl::BCK, BCLK_MULTIPLY);
	CClockCtrl::SelectClockMultiply(CClockCtrl::PCK, PCLK_MULTIPLY);

//	CClockCtrl::SetClockPLL(CClockCtrl::ICK, CClockCtrl::x8);
//	CClockCtrl::SetClockPLL(CClockCtrl::BCK, CClockCtrl::x4);
//	CClockCtrl::SetClockPLL(CClockCtrl::PCK, CClockCtrl::x4);

//	CClockCtrl::EnableClockOut(CClockCtrl::SDCLK, false);
//	CClockCtrl::EnableClockOut(CClockCtrl::BCLK, false);
	CClockCtrl::SetBclkDivision(false);

#endif
}

////////////////////////////////////////
// インターバルタイマの初期化
//		CMTを使用
//
void CTaskCtrl::InitializeInterval()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	// Enable CMT0
	//
	MSTP_CMT0 = 0;

	// enable interrupt
	//
	IEN(CMT0, CMI0) = 0;
	IR(CMT0, CMI0)  = 0;
	IPR(CMT0, CMI0) = 1;
	IEN(CMT0, CMI0) = 1;

	// set 1 ms interval
	//
	CCompareMatchTimer::StartCounter(0, false);
#if OSC_EXTERNAL == true
	CCompareMatchTimer::SetIntervalTimer(0, EXTERNAL_INPUT_MHZ * PCLK_MULTIPLY, 1);
#else
	CCompareMatchTimer::SetIntervalTimer(0, INTERNAL_OSC_MHZ * PCLK_DIVISER, 1);
#endif
	CCompareMatchTimer::StartCounter(0, true);
	CCompareMatchTimer::EnableInterrupt(0, true);
}

////////////////////////////////////////
// ウォッチドッグタイマの初期化
//		IWDTTを使用
//
void CTaskCtrl::InitializeWatchDogTimer()
{
//	WDT.WRITE.WINA = 0xa522;
}

////////////////////////////////////////
// ウォッチドッグタイマのリフレッシュ
//
void CTaskCtrl::RefreshWatchDogTimer()
{
	// リフレッシュ
	//
	IWDT.IWDTRR = 0;
	IWDT.IWDTRR = 0xff;
}

////////////////////////////////////////
// μSの待ち
//		割込み禁止にして、サイクル数を消費する
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
	int_exception(VECT_TASK_SWITCH);	// タスク切替
}

void CTaskCtrl::ChangeSupervisor()
{
	int_exception(VECT_SUPERVISOR);		// スーパーバイザーへ切替
}

void CTaskCtrl::ChangeUser()
{
	chg_pmusr();			//
}

void CTaskCtrl::EnterSystemCritical()
{
	int_exception(VECT_ENTER_CRITICAL);	//
}

void CTaskCtrl::LeaveSystemCritical()
{
	int_exception(VECT_LEAVE_CRITICAL);	//
}

void CTaskCtrl::DisableInterrupt()
{
	int_exception(VECT_DISABLE_INTERRUPT);	//
}

void CTaskCtrl::EnableInterrupt()
{
	int_exception(VECT_ENABLE_INTERRUPT);	//
}

////////////////////////////////////////
// タスクレジスタを切り替える
//		上にあるSwitchTask()から呼ばれる
//		切替たいタスクのレジスタ配列アドレスを取得する
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
// インターバル割込み(WDT)
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
