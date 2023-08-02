////////////////////////////////////////////////////////////////////////////////
// Furicha's Os Realtime Tiny Embedded
//
//	履歴はヘッダ参照
//
//

#include <assert.h>
#include "forte32.h"

////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////
// コンパイル制限
//#define	__TERM_OF_VALIDITY__	"Jun 9 2008"

////////////////////////////////////////
// Global Implementation
//
CTaskCtrl				g_TaskCtrl;					// OSオブジェクト
int						g_nLastError  = ERROR_NONE;	// 最終のエラー番号
int						g_nErrorTask  = -1;			//
bool					g_bInterrupt = false;		// 割り込み中フラグ
extern int				g_nNextTask;				// 切替先タスク番号
extern void*			g_pTaskReg;					// 初めての起動時は0になっている必要がある
static bool				g_bDebugMode  = false;		// デバッグモード
static CTaskChgQueue	g_TaskChgQueue;				// タスク切替情報キュー
////////////////////////////////////////
// HANDLEの種別
//
//	0x00000000
//	  ||  |__|_ index
//	  ||_______ type
//
#define	SYNC_EVENT		0x01000000	// type event
#define	SYNC_SEMAPHORE	0x02000000	// type semapho
#define	SYNC_MUTEX		0x04000000	// type mutex

#define	INDEX_OF_HANDLE(h)	((unsigned short)h & 0xffff)
#define	IS_HANDLE(t, h)		((h & 0xff000000) == t)
#define	MAKE_HANDLE(t, idx)	(t | (unsigned long)idx)

////////////////////////////////////////
// 現タスクの終了処理
//
void _DestroyCurrentTask()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	g_TaskCtrl.DestroyCurrentTask();
}

////////////////////////////////////////
// システムタスクの終了処理
//
void _DestroySystemTask()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	g_nLastError = ERROR_NO_TASK_EXIST;
	CTaskCtrl::RestorSystem();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////
//
bool CSyncObjectCore::DecrementLockCount()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_nLockCount > 0)
		m_nLockCount--;
	return (m_nLockCount == 0);
}

////////////////////////////////////////
//
void CSyncObjectCore::IncrementLockCount()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_nLockCount < m_nMaximumCount)
		m_nLockCount++;
}

////////////////////////////////////////
//
bool CSemaphoreCore::Lock()
{
	DecrementLockCount();
	return true;
}

////////////////////////////////////////
//
bool CSemaphoreCore::Unlock()
{
	IncrementLockCount();
	return true;
}

////////////////////////////////////////
//
bool CMutexCore::Lock()
{
	DecrementLockCount();
	return true;
}

////////////////////////////////////////
//
bool CMutexCore::Unlock()
{
	IncrementLockCount();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////
//
CTask::CTask()
{
	m_pStackFrame        = NULL;					// スタックフレームポインタ
	m_nLockMs            = 0;						// 待ちms
	m_TaskFlag.nState    = TS_NO_CREATE;			// デフォルト停止
	m_TaskFlag.nPriority = THREAD_PRIORITY_NORMAL;	// デフォルトプライオリティ

	m_nSyncObjectCount     = 0;						// 同期待ちオブジェクト数
	m_phSyncObjects        = NULL;					// 同期待ちオブジェクト配列ポインタ
	m_bSyncObjectWaitAll   = false;					// 同期フラグ
	m_nSignalSyncObjectIdx = -1;					// 同期インデックス

	// 退避レジスタクリア
	//
	for (int i = 0; i < REG_NUM; i++)
		m_dwReg[i] = 0;

	m_bValid = false;
}

////////////////////////////////////////
//
CTask::~CTask()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	// スタックフレームがあったら削除
	if (m_pStackFrame) {
		delete[] m_pStackFrame;
		m_pStackFrame = NULL;
	}
}
/*
////////////////////////////////////////
//
CTaskCtrl& CTaskCtrl::GetTaskCtrl()
{
	return g_TaskCtrl;
}
*/
////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////
//
CIntervalTimer::CIntervalTimer()
{
	// コールバック構造体ポインタ配列の初期化
	//
	for (int i = 0; i < MAX_INTERVAL_CALLBACK; i++)
		m_IntervalCalllback[i].bValid = false;
}

////////////////////////////////////////
//
CIntervalTimer::~CIntervalTimer()
{
}

////////////////////////////////////////
// コールバック関数の登録
//
int CIntervalTimer::CreateCallback(unsigned int ms, ITVL_CALLBACK_FUNC pFunc, void* pVoid)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	int nRet = -1;

	// 割込み禁止
	CTaskCtrl::EnterSystemCritical();

	// 空いているエントリを探して生成
	//
	for (int i = 0; i < MAX_INTERVAL_CALLBACK; i++) {

		if (!m_IntervalCalllback[i].bValid) {
			// パラメータ設定
			m_IntervalCalllback[i].pFunc   = pFunc;
			m_IntervalCalllback[i].pVoid   = pVoid;
			m_IntervalCalllback[i].timer   = ms;
			m_IntervalCalllback[i].counter = ms;
			m_IntervalCalllback[i].bValid    = true;

			nRet = i;	// 登録IDを返す
			break;
		}
	}

	// 割込み許可
	CTaskCtrl::LeaveSystemCritical();
	return nRet;
}

////////////////////////////////////////
// コールバック関数の削除
//
void CIntervalTimer::DeleteCallback(int id)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	// 割込み禁止
	CTaskCtrl::EnterSystemCritical();

	// 有効なIDなら、削除
	if (id >= 0 && id < MAX_INTERVAL_CALLBACK) {

		if (m_IntervalCalllback[id].bValid)
			m_IntervalCalllback[id].bValid = false;
	}

	// 割込み許可
	CTaskCtrl::LeaveSystemCritical();
}

////////////////////////////////////////
// コールバック処理
//		インターバル割り込みから呼ばれる
//
void CIntervalTimer::ScanAndCall()
{
	// 割り込み中でなければ、エラーとする
	//
	INSIDE_INTERRUPT_CHECK();

	// 有効なコールバックについて
	for (int i = 0; i < MAX_INTERVAL_CALLBACK; i++) {

		if (m_IntervalCalllback[i].bValid) {

			// カウンタがタイムアップしたら
			if (--m_IntervalCalllback[i].counter == 0) {

				// カウンタを初期値に
				m_IntervalCalllback[i].counter = m_IntervalCalllback[i].timer;

				// falseを返したら、コールバックを削除
				if (!m_IntervalCalllback[i].pFunc(m_IntervalCalllback[i].pVoid))
					m_IntervalCalllback[i].bValid = false;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////
//
CTaskCtrl::CTaskCtrl()
: m_seqSystem(SYS_NOP)
, m_nTaskID(INVALID_TASK_ID)
, m_bInitialized(false)
, m_hCritical(INVALID_HANDLE)
, m_dwTickCount(0)
, m_nCpuClockFreq(0)
{
	// タスク管理配列の初期化
	//
	for (int i = 0; i < MAX_TASKS; i++)
		m_Task[i].m_bValid = false;

	// エラーコード初期値
	g_nLastError = ERROR_NONE;

	//
	for (int i = 0; i < MAX_SYNC_OBJECTS; i++) {
#ifdef	STATIC_SYSTEM_OBJECT
		m_SyncObject[i].m_bValid = false;
#else
		m_pSyncObject[i] = NULL;
#endif
	}
}

////////////////////////////////////////
//
CTaskCtrl::~CTaskCtrl()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	// 有効なタスクを削除
	for (int i = 0; i < MAX_TASKS; i++)
		m_Task[i].m_bValid = false;

	// 同期待ちオブジェクトの後処理
	for (int i = 0; i < MAX_SYNC_OBJECTS; i++) {
#ifdef	STATIC_SYSTEM_OBJECT
		m_SyncObject[i].m_bValid = false;
#else
		if (m_pSyncObject[i] != NULL) {
			delete m_pSyncObject[i];
			m_pSyncObject[i] = NULL;
		}
#endif
	}
}
////////////////////////////////////////

// デバッグモードを有効にする
void CTaskCtrl::EnableDebugMode(bool bEnable)
{
	g_bDebugMode = bEnable;
}

////////////////////////////////////////
// タスク切替キューのポインタを得る
const int* CTaskCtrl::GetTaskChangeQueue()
{
	if (g_bDebugMode)
		return g_TaskChgQueue.getQueue();

	return NULL;
}

////////////////////////////////////////
// タスク切替キューサイズを得る
int CTaskCtrl::GetTaskChangeQueueSize()
{
	if (g_bDebugMode)
		return g_TaskChgQueue.getSize();

	return 0;
}

////////////////////////////////////////
// エラー番号の取得
//
int CTaskCtrl::GetLastError()
{
	return g_nLastError;
}

////////////////////////////////////////
// タスクIDの取得
int CTaskCtrl::GetLastTaskID()
{
	return m_nTaskID;
}

static const char* pProduct   = "Forte32 Realtime Multitask Operating System for Renesas 32 bits MPU.";

////////////////////////////////////////
//
const char* CTaskCtrl::GetProductString()
{
	return pProduct;
}

static const char* pCopyright = "Chikuma Seiki 2007-2015. all rights reserved.";

////////////////////////////////////////
//
const char* CTaskCtrl::GetCopyrightString()
{
	return pCopyright;
}

////////////////////////////////////////
//
const char* CTaskCtrl::GetVersionString()
{
	return FORTE_RELEASE;
}

int CTaskCtrl::GetMaxTasks()					{return MAX_TASKS;				}
int CTaskCtrl::GetMaxSyncObjects()				{return MAX_SYNC_OBJECTS;		}
int CTaskCtrl::GetMaxIntervalCallbacks()		{return MAX_INTERVAL_CALLBACK;	}
int CTaskCtrl::GetClockFrequency()				{return m_nCpuClockFreq;		}
short CTaskCtrl::GetSystemStackSize()			{return SYS_STACK_SIZE;			}
const char* CTaskCtrl::GetCpuName()				{return CPU_NAME;				}

////////////////////////////////////////
// 使用中のスタックサイズを返す
unsigned int CTaskCtrl::GetUsingStackSize(int task)
{
	CTask* pTask = GetTask(task);

	// 有効なタスク
	if (pTask != NULL)
		return pTask->GetUsingStackSize();

	return 0;
}

////////////////////////////////////////
//
CTask* CTaskCtrl::GetTask(int task)
{
	// システムタスクIDなら、システムタスク
	if (task == SYSTEM_TASK_ID)
		return &m_sysTask;

	// 有効なタスクIDなら
	else if (task < MAX_TASKS)
		return &m_Task[task];

	// 無効なタスクIDなら
	else
		return NULL;
}

////////////////////////////////////////
//
bool CTaskCtrl::IsValidTask(int task)
{
	// システムタスクIDか有効タスクIDなら
	if (task == SYSTEM_TASK_ID || (task < MAX_TASKS && m_Task[task].m_bValid))
		return true;

	return false;
}

////////////////////////////////////////
//
CTask* CTaskCtrl::GetCurrentTask()
{
	// 有効なタスクIDなら
	if (IsValidTask(m_nTaskID))
		return GetTask(m_nTaskID);

	return NULL;
}

////////////////////////////////////////
//
void CTaskCtrl::DeleteTCB(int task)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (IsValidTask(task)) {

		m_Task[task].m_bValid = false;

		m_schedulefifo.DeleteTask(task);
	}
}

////////////////////////////////////////
//
int CTaskCtrl::CreateIntervalCallback(int ms, ITVL_CALLBACK_FUNC pFunc, void* pVoid)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	return m_IntervalTimer.CreateCallback(ms, pFunc, pVoid);
}

////////////////////////////////////////
//
void CTaskCtrl::DeleteIntervalCallback(int id)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	m_IntervalTimer.DeleteCallback(id);
}

////////////////////////////////////////
//
CSyncObjectCore* CTaskCtrl::_FromHandle(HANDLE hSyncObjet)
{
	int idx = INDEX_OF_HANDLE(hSyncObjet);
	if (idx >= 0 && idx < MAX_SYNC_OBJECTS) {
#ifdef	STATIC_SYSTEM_OBJECT
		return &m_SyncObject[idx];
#else
		return m_pSyncObject[idx];
#endif
	}
	return NULL;
}

////////////////////////////////////////
// システムタスク関数をクラス関数として実行するため
//		キックするための静的関数
//
int CTaskCtrl::_SysTaskKick(void* pVoid)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	CTaskCtrl* pTaskCtrl = (CTaskCtrl*)pVoid;
	return pTaskCtrl->SysTaskFunc();
}

////////////////////////////////////////
// インターバルコールバックサービス
//		インターバル割り込みから呼ばれる
//
void CTaskCtrl::IntervalTimerScan()
{
	// 割り込み中でなければ、エラーとする
	//
	INSIDE_INTERRUPT_CHECK();

	m_IntervalTimer.ScanAndCall();
}

////////////////////////////////////////
// タスクロックカウントの減算
//		インターバル割り込みから呼ばれる
//
void CTaskCtrl::DecrementWaitMs()
{
	// 割り込み中でなければ、エラーとする
	//
	INSIDE_INTERRUPT_CHECK();

	// タスク個数分の繰り返し
	for (int i = 0; i < MAX_TASKS; i++) {

		// 有効なタスク
		if (m_Task[i].m_bValid)
			m_Task[i].DecrementLockMs();
	}
}

#ifdef	__TERM_OF_VALIDITY__

#include <string.h>
#include <stdlib.h>

////////////////////////////////////////
// ユリウス日＝[Year * 365.25] + [Year / 400] - [Year / 100] + [30.59 * (Month - 2)] + Day - 678912
//
const char* g_pMonth[] = {	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
unsigned long _getJulianDay(const char* pDate)
{
	int nYear  = -1;
	int nMonth = -1;
	int nDay   = -1;

	// 01234567890
	// May 17 2008
	char date[12];
	if (strlen(pDate) >= 12)
		return 0;
	strcpy(date, pDate);

	// 文字列抜き出し
	char* pMonth = strtok(date, " ");
	char* pDay   = strtok(NULL, " ");
	char* pYear  = strtok(NULL, " ");

	// 数値変換
	if (pYear) nYear = atoi(pYear);
	for (int i = 0; i < sizeof (g_pMonth) / sizeof (const char*); i++) {
		if (strcmp(pMonth, g_pMonth[i]) == 0) {
			nMonth = i + 1;
			break;
		}
	}
	if (pDay)  nDay = atoi(pDay);

	// 1月、2月は、前年の13月、14月とする
	if (nMonth <= 2) {
		nMonth += 12;
		nYear--;
	}

	// ユリウス日＝[Year * 365.25] + [Year / 400] - [Year / 100] + [30.59 * (Month - 2)] + Day - 678912
	unsigned long ulJulianDay = (float)nYear * 365.25 + (float)nYear / 400 - (float)nYear / 100 + 30.59 * ((float)nMonth - 2) + nDay - 678912;

	return ulJulianDay;
}
#endif

////////////////////////////////////////
// タスク管理の初期化
//
bool CTaskCtrl::Initialize(int nFreq)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

// コンパイルの有効期限
#ifdef	__TERM_OF_VALIDITY__

	// コンパイル時のユリウス日
	unsigned long ulCompiled = _getJulianDay(__DATE__);

	// 有効期限のユリウス日（この日付以降は使用不可）
	unsigned long ulTermOfValidity = _getJulianDay(__TERM_OF_VALIDITY__);

	// 有効期限を超えていたら
	if (ulCompiled > ulTermOfValidity) {
		g_nLastError = ERROR_TERM_EXPIRED;
		abort();
	}
#endif

	// 既に初期化されている場合、何もしない
	if (m_bInitialized)
		return true;

	// CPUクロックの設定
	m_nCpuClockFreq = nFreq;

	//
	InitializeClock();

	// インターバルの初期化
	InitializeInterval();

	// システムタスクの生成と実行予約
	if (m_sysTask.CreateTask(CTaskCtrl::_SysTaskKick, THREAD_PRIORITY_NORMAL, SYS_STACK_SIZE, this, _DestroySystemTask))
		m_sysTask.SetTaskRun(true);

	// 初期化済みフラグ
	m_bInitialized = true;

	return true;
}

////////////////////////////////////////
// 初期化済みか返す
bool CTaskCtrl::IsInitialized()
{
	return m_bInitialized;
}

////////////////////////////////////////
// システムタスク関数
//
//		ユーザータスクからタスク切替が起こると、
//		システムタスクに移行する
//
int CTaskCtrl::SysTaskFunc()
{
	CTask* pTask;
	int nTask;

	while (1) {
		switch(m_seqSystem) {
			// 何もしない
			//
			case SYS_NOP:
				break;

			// タスクの生成
			//	引数	m_nOrderTask	呼び出し元タスク番号
			//
			//	戻値	m_nOrderTask	生成されたタスク番号
			//
			case SYS_CREATE:
				// 空いているIDのタスクを生成
				nTask = m_nOrderTask;
				m_nOrderTask = INVALID_TASK_ID;

				for (int i = 0; i < MAX_TASKS; i++) {
					if (!m_Task[i].m_bValid) {

						// タスクの生成できた
						if (m_Task[i].CreateTask(m_pFunc, m_nPriority, m_nStackSize, m_pVoid, m_pDestroy)) {
							m_Task[i].m_bValid = true;
							m_schedulefifo.AppendPriority(i, m_nPriority);
							m_nOrderTask = i;
							break;

						} else {
							// 無効ID
							m_nOrderTask = INVALID_TASK_ID;
							break;
						}
					}
				}

				// 呼び出しタスクに戻る
				pTask = GetTask(nTask);
				pTask->SetSwitchReson(CTask::SR_SWITCH);

				g_nNextTask = nTask;
				if (g_bDebugMode)
					g_TaskChgQueue.put(nTask);
				SwitchTask();
				break;

			// タスクの実行と停止
			//	引数	m_nOrderTask	実行、停止するタスク
			//
			case SYS_RUN:
			case SYS_SUSPEND:
				pTask = GetTask(m_nOrderTask);
				if (pTask) {
					pTask->SetTaskRun((m_seqSystem == SYS_RUN) ? true : false);
					pTask->SetSwitchReson((m_seqSystem == SYS_RUN) ? CTask::SR_RUN : CTask::SR_SUSPEND);
				}

				// 有効タスクなら、タスク切替
				while (!GetScheduledTask(&nTask))
					;

				// タスク切替
				pTask = GetTask(nTask);
				pTask->SetSwitchReson(CTask::SR_SWITCH);
				g_nNextTask = nTask;
				if (g_bDebugMode)
					g_TaskChgQueue.put(nTask);
				SwitchTask();
				break;

			// タスクの同期待ち
			//
			case SYS_LOCK:
				// 有効タスクなら、タスク切替
				while (!GetScheduledTask(&nTask))
					;

				// タスク切替
				g_nNextTask = nTask;
				if (g_bDebugMode)
					g_TaskChgQueue.put(nTask);
				SwitchTask();
				break;

			// タスク破棄
			//		m_nOrderTask
			//
			case SYS_DESTROY:
				// TCBを破棄
				DeleteTCB(m_nOrderTask);

				// 有効タスクなら、タスク切替
				while (!GetScheduledTask(&nTask))
					;

				// タスク切替
				pTask = GetTask(nTask);
				pTask->SetSwitchReson(CTask::SR_SWITCH);
				g_nNextTask = nTask;
				if (g_bDebugMode)
					g_TaskChgQueue.put(nTask);
				SwitchTask();
				break;
		}
	}
	return 0;
}

////////////////////////////////////////
// スリープ
//
void CTaskCtrl::Sleep(unsigned int ms)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	// 永遠の眠りでない
	if (ms != INFINITE) {
		// スリープは解除されないイベントと同じ
		CEvent event;
		event.Lock(ms);

	// 永遠の眠りなら、タスク停止
	} else
		RunTask(m_nTaskID, false);
}

////////////////////////////////////////
// タスク実行
//
void CTaskCtrl::RunTask(int task, bool bRun)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	m_nOrderTask = task;
	m_seqSystem  = bRun ? SYS_RUN : SYS_SUSPEND;
	SwitchTask();
}

////////////////////////////////////////
// タスク関数から抜ける時の処理
//
void CTaskCtrl::DestroyCurrentTask()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	m_nOrderTask = m_nTaskID;
	m_seqSystem  = SYS_DESTROY;
	SwitchTask();
}

////////////////////////////////////////
// タスクの生成
//
//	引数	pFunc		タスク関数ポインタ
//			nPriority	タスクプライオリティ	THREAD_PRIORITY_*
//			nStackSize	タスク固有スタックサイズ
//			pVoid		タスク関数に渡すパラメータ
//			pDestroy	タスク終了時に呼び出される関数ポインタ
//
//	戻値	!= INVALID_TASK_ID	タスクID
//			INVALID_TASK_ID		失敗
//
int CTaskCtrl::CreateTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	//
	if (nPriority >= THREAD_PRIORITY_MAX_VALUE)
		nPriority = THREAD_PRIORITY_MAX_VALUE;

	// 現在タスクIDが無効タスクID（まだタスクが走っていない）なら
	if (m_nTaskID == INVALID_TASK_ID) {

		// 直接生成
		return CreateTask_OnNoTask(pFunc, nPriority, nStackSize, pVoid, pDestroy);

	// 既にタスクが走っているなら
	} else {

		// システムタスクにて生成
		m_pFunc      = pFunc;
		m_nPriority  = nPriority;
		m_nStackSize = nStackSize;
		m_pVoid      = pVoid;
		m_pDestroy   = pDestroy;
		m_nOrderTask = m_nTaskID;	// 今のタスク番号に戻ってくるため
		m_seqSystem  = SYS_CREATE;
		SwitchTask();

		return m_nOrderTask;	// 生成されたタスクID
	}
}

////////////////////////////////////////
// 非タスクでのタスク生成
//
//	引数	pFunc		タスク関数ポインタ
//			nPriority	タスクプライオリティ	THREAD_PRIORITY_*
//			nStackSize	タスク固有スタックサイズ
//			pVoid		タスク関数に渡すパラメータ
//			pDestroy	タスク終了時に呼び出される関数ポインタ
//
//	戻値	!= INVALID_TASK_ID	タスクID
//			INVALID_TASK_ID		失敗
//
int CTaskCtrl::CreateTask_OnNoTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	//
	if (nPriority >= THREAD_PRIORITY_MAX_VALUE)
		nPriority = THREAD_PRIORITY_MAX_VALUE;

	// 空いているIDのタスクを生成
	for (int i = 0; i < MAX_TASKS; i++) {
		if (!m_Task[i].m_bValid) {

			// タスクの生成できた
			if (m_Task[i].CreateTask(pFunc, nPriority, nStackSize, pVoid, pDestroy)) {
				m_Task[i].m_bValid = true;
				m_schedulefifo.AppendPriority(i, nPriority);
				return i;
			}
		}
		return INVALID_TASK_ID;
	}

	return INVALID_TASK_ID;
}
////////////////////////////////////////
// タスクプライオリティの変更
//
void CTaskCtrl::ChangeTaskPriority(int task, int priority)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	//
	if (priority >= THREAD_PRIORITY_MAX_VALUE)
		priority = THREAD_PRIORITY_MAX_VALUE;

	CTask* pTask = GetTask(task);

	// 有効なタスク
	if (pTask != NULL) {
		// FIFO並べ替え
		m_schedulefifo.DeleteTask(task);
		m_schedulefifo.AppendPriority(task, priority);

		// タスクの情報を変えておく
		pTask->SetPriority(priority);

		// タスク切替
		m_seqSystem = SYS_LOCK;
		SwitchTask();
	}
}

////////////////////////////////////////
// チックカウント
unsigned long CTaskCtrl::GetTickCount()
{
	return m_dwTickCount;
}

////////////////////////////////////////
// スケジュールされたタスクを返す
//
bool CTaskCtrl::GetScheduledTask(int* pnTask)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	// 割込み禁止
	EnterSystemCritical();

	// 全タスクの同期解除を確認
	for (int i = 0; i < MAX_TASKS; i++)
		CheckWaitTask(i);

	// 同期オブジェクトによるスケジューリング
	*pnTask = INVALID_TASK_ID;

	// FIFOにあるタスク数繰り返し
	int tasks = m_schedulefifo.GetSize();
	for (int i = 0; i < tasks; i++) {

		// FIFOからタスクを取り出し、また追加しておく
		PRIO_INFO info = m_schedulefifo.GetHead();
		int nTask = info.task;
		m_schedulefifo.AppendTail(info);
		CTask* pTask = GetTask(nTask);

		// 有効なタスク
		if (pTask != NULL) {

			// 実行許可?
			if (pTask->IsTaskRun()) {

				// 必要なら、自動リセットイベントをリセット
				if (pTask->GetSwitchReson() == CTask::SR_BLOCKOUT)
					ResetAutoEvent(nTask);

				// タスクID確定
				*pnTask = nTask;

				// 割込み許可
				LeaveSystemCritical();
				return true;
			}
		}
	}

	// 割込み許可
	LeaveSystemCritical();

	// 実行するタスクがない
	return false;
}

////////////////////////////////////////
// 同期待ちタスクの解除処理
bool CTaskCtrl::CheckWaitTask(int nTaskID)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	bool bSignal = false;

	// 有効なタスクで同期待ち?
	CTask* pTask = GetTask(nTaskID);
	if (pTask == NULL || !pTask->IsTaskWait())
		return false;

	// あらかじめインデックスを0にしておく
	pTask->m_nSignalSyncObjectIdx = 0;

	// タイムアウトした
	if (pTask->GetLockMs() == 0) {

		// 同期待ち解除
		pTask->SetTaskWait(false);
		pTask->SetSwitchReson(CTask::SR_TIMEOUT);
		return true;

	// 同期チェック
	} else {

		// 同期オブジェクトポインタ
		const HANDLE* phSyncObjects = pTask->GetSyncObjectHandlePt();
		if (phSyncObjects == NULL) {
			pTask->SetSwitchReson(CTask::SR_NOP);
			return false;
		}

		// 同期オブジェクト数、スキャンする
		bool bAllSignal = true;
		int nIdxObject  =  -1;
		for (int j = 0; j < pTask->GetSyncObjectCount(); j++) {

			// 有効なハンドル
			if (phSyncObjects[j] != INVALID_HANDLE) {
				CSyncObjectCore* pSyncObject = _FromHandle(phSyncObjects[j]);
				assert(pSyncObject);

				// 待ってる同期オブジェクトがブロック解除になっている
				if (pSyncObject->IsSignal()) {

					// 全部待ちでない
					if (!pTask->IsSyncObjectWaitAll()) {

						// 同期待ち解除
						pTask->SetTaskWait(false);

						// 最初に解除したインデックス
						if (nIdxObject == -1)
							nIdxObject = j;
					}
					bSignal = true;

				// 一つでもシグナルでない
				} else
					bAllSignal = false;
			}
		}

		// 全部待ちでなくて、最初に解除されたインデックスが有効なら
		if (!pTask->IsSyncObjectWaitAll() && bSignal) {
			pTask->m_nSignalSyncObjectIdx = nIdxObject;
			pTask->SetSwitchReson(CTask::SR_BLOCKOUT);
			return true;
		}

		// 全部待ちで、全部シグナルになった
		if (pTask->IsSyncObjectWaitAll() && bAllSignal) {

			// 同期待ち解除
			pTask->SetTaskWait(false);
			pTask->SetSwitchReson(CTask::SR_BLOCKOUT);
			return true;
		}
	}

	//
	pTask->SetSwitchReson(CTask::SR_NOP);
	return false;
}

////////////////////////////////////////
// 必要なら、自動リセットイベントをリセットする
bool CTaskCtrl::ResetAutoEvent(int nTaskID)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	// 有効なタスクで同期待ち?
	CTask* pTask = GetTask(nTaskID);
	if (pTask != NULL) {

		// 同期オブジェクトポインタ
		const HANDLE* phSyncObjects = pTask->GetSyncObjectHandlePt();

		// 同期オブジェクト数繰り返し
		for (int j = 0; j < pTask->GetSyncObjectCount(); j++) {

			// 有効なハンドル
			if (phSyncObjects[j] != INVALID_HANDLE) {
				CSyncObjectCore* pSyncObject = _FromHandle(phSyncObjects[j]);
				assert(pSyncObject);

				// 待ってる同期オブジェクトがブロック解除になっている
				if (pSyncObject->IsSignal()) {

					// イベントハンドルだったら、必要に応じて自動解除処理
					if (IS_HANDLE(SYNC_EVENT, phSyncObjects[j])) {
							CEventCore* pEvent = (CEventCore*)pSyncObject;
						if (pEvent != NULL && !pEvent->IsManualReset())
							pEvent->ResetEvent();
					}
				}
			}
		}
		return true;
	}

	return false;
}

////////////////////////////////////////
//
int CTaskCtrl::WaitForSingleObject(HANDLE hSyncObject, unsigned int ms)
{
	// 同期オブジェクトが１個のWaitForMultipleObjects()として実行する
	//
	HANDLE handle = hSyncObject;
	return WaitForMultipleObjects(1, &handle, false, ms);
}

////////////////////////////////////////
//
int CTaskCtrl::WaitForMultipleObjects(int nCount, const HANDLE* phSyncObjects, bool bWaitAll, unsigned int ms)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	// 指定された同期オブジェクトハンドルが有効なら
	if (phSyncObjects == NULL || nCount <= 0)
		return WAIT_FAILED;

	// 有効な同期オブジェクトならロックする
	for (int i = 0; i < nCount; i++) {
		HANDLE hSyncObject = phSyncObjects[i];
		if (hSyncObject == INVALID_HANDLE)
			return WAIT_FAILED;

		// 指定された同期オブジェクトハンドルが無効なら
		int idx = INDEX_OF_HANDLE(hSyncObject);
		if (idx >= 0 && idx < MAX_SYNC_OBJECTS) {
#ifdef	STATIC_SYSTEM_OBJECT
			if (!m_SyncObject[idx].m_bValid)
				return WAIT_FAILED;
#else
			if (m_pSyncObject[idx] == NULL)
				return WAIT_FAILED;
#endif
		}

		// ロック
#ifdef	STATIC_SYSTEM_OBJECT
		m_SyncObject[idx].Lock();
#else
		m_pSyncObject[idx]->Lock();
#endif
	}

	// 待ち
	CTask* pTask = GetCurrentTask();
	if (pTask) {
		pTask->SetLockMs(ms);
		pTask->SetTaskWait(true);
		pTask->SetSyncObjectHandles(nCount, phSyncObjects, bWaitAll);
		m_seqSystem = SYS_LOCK;
		SwitchTask();
		if (pTask->GetSwitchReson() == CTask::SR_TIMEOUT)
			return WAIT_TIMEOUT;

		// 全て待ちだった
		if (bWaitAll)
			return WAIT_OBJECT_0;

		// どれかひとつでもの場合
		else
			return WAIT_OBJECT_0 + pTask->m_nSignalSyncObjectIdx;
	}

	return WAIT_FAILED;
}

////////////////////////////////////////
//
HANDLE CTaskCtrl::CreateEvent(bool bManualReset, bool bInitialState)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

#ifdef	STATIC_SYSTEM_OBJECT
	//
	for (int i = 0; i < MAX_SYNC_OBJECTS; i++) {
		if (!m_SyncObject[i].m_bValid) {
			m_SyncObject[i].m_bValid = true;
#ifdef	DEBUG
			m_SyncObject[i].SetTaskId(i);
#endif
			return MAKE_HANDLE(SYNC_EVENT, i);
		}
	}

#else
	CEventCore* pEvent = new CEventCore(bManualReset, bInitialState);
	if (pEvent) {
		//
		for (int i = 0; i < MAX_SYNC_OBJECTS; i++) {
			if (m_pSyncObject[i] == NULL) {
				m_pSyncObject[i] = pEvent;
#ifdef	DEBUG
				pEvent->SetTaskId(i);
#endif
				return MAKE_HANDLE(SYNC_EVENT, i);
			}
		}
	}
#endif
	return INVALID_HANDLE;
}

////////////////////////////////////////
//
void CTaskCtrl::DeleteSyncObject(HANDLE hSyncObject)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	int idx = INDEX_OF_HANDLE(hSyncObject);

#ifdef	STATIC_SYSTEM_OBJECT
	if (m_SyncObject[idx].m_bValid)
		m_SyncObject[idx].m_bValid = false;

#else
	if (m_pSyncObject[idx] != NULL) {
		delete m_pSyncObject[idx];
		m_pSyncObject[idx] = NULL;
	}
#endif
}

////////////////////////////////////////
//
bool CTaskCtrl::SetEvent(HANDLE hEvent)
{
	if (!IS_HANDLE(SYNC_EVENT, hEvent))
		return false;

	CEventCore* pEvent = (CEventCore*)_FromHandle(hEvent);
	if (pEvent != NULL) {
		bool bSignal = pEvent->IsSignal();
		pEvent->SetEvent();

		// 非割り込み時は、タスクを切り替える
		//
		if (!bSignal && pEvent->IsSignal() && !IS_INTERRUPT()) {
			m_seqSystem = SYS_LOCK;
			SwitchTask();
		}
		return true;
	}

	return false;
}

////////////////////////////////////////
//
bool CTaskCtrl::PulseEvent(HANDLE hEvent)
{
	if (SetEvent(hEvent))
		return ResetEvent(hEvent);

	return false;
}

////////////////////////////////////////
//
bool CTaskCtrl::ResetEvent(HANDLE hEvent)
{
	if (!IS_HANDLE(SYNC_EVENT, hEvent))
		return false;

	CEventCore* pEvent = (CEventCore*)_FromHandle(hEvent);
	if (pEvent != NULL) {
		pEvent->ResetEvent();
		return true;
	}

	return false;
}

////////////////////////////////////////
//
HANDLE CTaskCtrl::CreateSemaphore(int nInitialCount, int nMaximumCount)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

#ifdef	STATIC_SYSTEM_OBJECT
	//
	for (int i = 0; i < MAX_SYNC_OBJECTS; i++) {
		if (!m_SyncObject[i].m_bValid) {
			m_SyncObject[i].m_bValid = true;
#ifdef	DEBUG
			m_SyncObject[i].SetTaskId(i);
#endif
			return MAKE_HANDLE(SYNC_SEMAPHORE, i);
		}
	}
#else
	CSemaphoreCore* pSemaphore = new CSemaphoreCore(nInitialCount, nMaximumCount);
	if (pSemaphore) {
		//
		for (int i = 0; i < MAX_SYNC_OBJECTS; i++) {
			if (m_pSyncObject[i] == NULL) {
				m_pSyncObject[i] = pSemaphore;
#ifdef	DEBUG
				pSemaphore->SetTaskId(i);
#endif
				return MAKE_HANDLE(SYNC_SEMAPHORE, i);
			}
		}
	}
#endif
	return INVALID_HANDLE;
}

////////////////////////////////////////
//
bool CTaskCtrl::ReleaseSemaphore(HANDLE hSemaphore)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (!IS_HANDLE(SYNC_SEMAPHORE, hSemaphore))
		return false;

	CSemaphoreCore* pSemaphore = (CSemaphoreCore*)_FromHandle(hSemaphore);
	if (pSemaphore != NULL) {
		pSemaphore->Unlock();
		return true;
	}

	return false;
}

////////////////////////////////////////
//
HANDLE CTaskCtrl::CreateMutex()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

#ifdef	STATIC_SYSTEM_OBJECT
	//
	for (int i = 0; i < MAX_SYNC_OBJECTS; i++) {
		if (!m_SyncObject[i].m_bValid) {
			m_SyncObject[i].m_bValid = true;
#ifdef	DEBUG
			m_SyncObject[i].SetTaskId(i);
#endif
			return MAKE_HANDLE(SYNC_MUTEX, i);
		}
	}
#else

	CMutexCore* pMutex = new CMutexCore();
	if (pMutex) {
		//
		for (int i = 0; i < MAX_SYNC_OBJECTS; i++) {
			if (m_pSyncObject[i] == NULL) {
				m_pSyncObject[i] = pMutex;
#ifdef	DEBUG
				pMutex->SetTaskId(i);
#endif
				return MAKE_HANDLE(SYNC_MUTEX, i);
			}
		}
	}
#endif
	return INVALID_HANDLE;
}

////////////////////////////////////////
//
void CTaskCtrl::EnterCriticalSection()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_hCritical == INVALID_HANDLE)
		m_hCritical = CreateMutex();

	WaitForSingleObject(m_hCritical, INFINITE);
}

////////////////////////////////////////
//
void CTaskCtrl::LeaveCriticalSection()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_hCritical == INVALID_HANDLE)
		return;

	CMutexCore* pMutex = (CMutexCore*)_FromHandle(m_hCritical);
	if (pMutex != NULL)
		pMutex->Unlock();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////
//
CSyncObject::CSyncObject()
: m_hSyncObject(INVALID_HANDLE)
{
}

////////////////////////////////////////
//
CSyncObject::~CSyncObject()
{
	if (m_hSyncObject != INVALID_HANDLE) {
		GetTaskCtrl().DeleteSyncObject(m_hSyncObject);
		m_hSyncObject = INVALID_HANDLE;
	}
}

////////////////////////////////////////
//
CEvent::CEvent(bool bManualReset, bool bInitialState)
: m_bManualReset(bManualReset)
, m_bInitialState(bInitialState)
{
}

////////////////////////////////////////
//
CEvent::~CEvent()
{
//	CSyncObject::~CSyncObject();
}

////////////////////////////////////////
//
HANDLE CEvent::Create()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_hSyncObject == INVALID_HANDLE)
		m_hSyncObject = GetTaskCtrl().CreateEvent(m_bManualReset, m_bInitialState);

	return m_hSyncObject;
}

////////////////////////////////////////
//
bool CEvent::SetEvent()
{
	// 割り込み中
	//
	if (IS_INTERRUPT()) {
		if (m_hSyncObject == INVALID_HANDLE)
			return false;
	}

	if (m_hSyncObject == INVALID_HANDLE)
		m_hSyncObject = GetTaskCtrl().CreateEvent(m_bManualReset, m_bInitialState);

	return GetTaskCtrl().SetEvent(m_hSyncObject);
}

////////////////////////////////////////
//
bool CEvent::ResetEvent()
{
	// 割り込み中
	//
	if (IS_INTERRUPT()) {
		if (m_hSyncObject == INVALID_HANDLE)
			return false;
	}

	if (m_hSyncObject == INVALID_HANDLE)
		m_hSyncObject = GetTaskCtrl().CreateEvent(m_bManualReset, m_bInitialState);

	return GetTaskCtrl().ResetEvent(m_hSyncObject);
}

////////////////////////////////////////
//
bool CEvent::PulseEvent()
{
	// 割り込み中
	//
	if (IS_INTERRUPT()) {
		if (m_hSyncObject == INVALID_HANDLE)
			return false;
	}

	if (m_hSyncObject == INVALID_HANDLE)
		m_hSyncObject = GetTaskCtrl().CreateEvent(m_bManualReset, m_bInitialState);

	return GetTaskCtrl().PulseEvent(m_hSyncObject);
}

////////////////////////////////////////
//
bool CEvent::Lock(unsigned int ms)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_hSyncObject == INVALID_HANDLE)
		m_hSyncObject = GetTaskCtrl().CreateEvent(m_bManualReset, m_bInitialState);

	int nRet = GetTaskCtrl().WaitForSingleObject(m_hSyncObject, ms);
	if (nRet == WAIT_OBJECT_0)
		return true;

	return false;
}

////////////////////////////////////////
//
CSemaphore::CSemaphore(int nInitialCount, int nMaximumCount)
: m_nInitialCount(nInitialCount)
, m_nMaximumCount(nMaximumCount)
{
	// Free Count must be under Max Count
	//
	if (m_nInitialCount >= m_nMaximumCount)
		m_nInitialCount = m_nMaximumCount;
}

////////////////////////////////////////
//
CSemaphore::~CSemaphore()
{
}

////////////////////////////////////////
//
HANDLE CSemaphore::Create()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_hSyncObject == INVALID_HANDLE)
		m_hSyncObject = GetTaskCtrl().CreateSemaphore(m_nInitialCount, m_nMaximumCount);

	return m_hSyncObject;
}

////////////////////////////////////////
//
bool CSemaphore::Lock(unsigned int ms)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_hSyncObject == INVALID_HANDLE)
		m_hSyncObject = GetTaskCtrl().CreateSemaphore(m_nInitialCount, m_nMaximumCount);

	int nRet = GetTaskCtrl().WaitForSingleObject(m_hSyncObject, ms);
	if (nRet == WAIT_OBJECT_0)
		return true;

	return false;
}

////////////////////////////////////////
//
bool CSemaphore::Unlock()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_hSyncObject == INVALID_HANDLE)
		return false;

	return GetTaskCtrl().ReleaseSemaphore(m_hSyncObject);
}

////////////////////////////////////////
//
CMutex::CMutex()
{
}

////////////////////////////////////////
//
CMutex::~CMutex()
{
}

////////////////////////////////////////
//
HANDLE CMutex::Create()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_hSyncObject == INVALID_HANDLE)
		m_hSyncObject = GetTaskCtrl().CreateMutex();

	return m_hSyncObject;
}

////////////////////////////////////////
// 使用 or 待ち
bool CMutex::Lock(unsigned int ms)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_hSyncObject == INVALID_HANDLE)
		m_hSyncObject = GetTaskCtrl().CreateMutex();

	int nRet = GetTaskCtrl().WaitForSingleObject(m_hSyncObject, ms);
	if (nRet == WAIT_OBJECT_0)
		return true;

	return false;
}

////////////////////////////////////////
//
CCriticalSection::CCriticalSection()
{
}

////////////////////////////////////////
//
CCriticalSection::~CCriticalSection()
{
}

////////////////////////////////////////
//
HANDLE CCriticalSection::Create()
{
	return INVALID_HANDLE;
}

////////////////////////////////////////
//
bool CCriticalSection::Lock()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	GetTaskCtrl().EnterCriticalSection();
	return true;
}

////////////////////////////////////////
//
bool CCriticalSection::Unlock()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	GetTaskCtrl().LeaveCriticalSection();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////
//
CThread::CThread()
: m_nTaskId(INVALID_TASK_ID)
{
}

////////////////////////////////////////
//
CThread::~CThread()
{
	if (m_nTaskId != INVALID_TASK_ID) {
		GetTaskCtrl().DestroyCurrentTask();
		m_nTaskId = INVALID_TASK_ID;
	}
}

////////////////////////////////////////
//
bool CThread::Initialize(int nFreq)
{
	return GetTaskCtrl().Initialize(nFreq);
}

////////////////////////////////////////
// タスクの生成
//	非割り込み
//
bool CThread::CreateThread(int nPriority, unsigned int nStackSize)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	m_nTaskId = GetTaskCtrl().CreateTask(ThreadKick, nPriority, nStackSize, this, _DestroyCurrentTask);
	return (m_nTaskId != INVALID_HANDLE);
}

////////////////////////////////////////
//
bool CThread::SignleLock(CSyncObject* pObject, unsigned int ms)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (pObject) {
		pObject->Create();
		HANDLE hSyncObject = (HANDLE)*pObject;

		int nRet = GetTaskCtrl().WaitForSingleObject(hSyncObject, ms);
		if (nRet == WAIT_OBJECT_0)
			return true;
	}
	return false;
}

////////////////////////////////////////
//
int CThread::MultiLock(bool bWaitAll, unsigned int ms, int nCount, ...)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	va_list	ap;
	va_start(ap, nCount);

	assert(nCount <= MAX_WAIT_HANDLES);

	HANDLE	WaitHandles[MAX_WAIT_HANDLES];

	for (int i = 0; i < MAX_WAIT_HANDLES; i++) {
		if (i < nCount) {
			CSyncObject* pSync = va_arg(ap, CSyncObject*);
			if (pSync) {
				pSync->Create();
				WaitHandles[i] = (HANDLE)*pSync;

			} else
				WaitHandles[i] = INVALID_HANDLE;
		} else
			WaitHandles[i] = INVALID_HANDLE;
	}

	int nRet = GetTaskCtrl().WaitForMultipleObjects(nCount, WaitHandles, bWaitAll, ms);

	va_end(ap);

	return nRet;
}

////////////////////////////////////////
//
unsigned int CThread::GetUsingStackSize()
{
	return GetTaskCtrl().GetUsingStackSize(m_nTaskId);
}

////////////////////////////////////////
//
void CThread::Sleep(unsigned int ms)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	GetTaskCtrl().Sleep(ms);
}

////////////////////////////////////////
//
void CThread::WaitUs(int us)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	GetTaskCtrl().WaitUs(us);
}

////////////////////////////////////////
//
unsigned long CThread::GetTickCount()
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	return GetTaskCtrl().GetTickCount();
}

////////////////////////////////////////
//
void CThread::_run(bool bRun)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_nTaskId != INVALID_TASK_ID)
		GetTaskCtrl().RunTask(m_nTaskId, bRun);
}

////////////////////////////////////////
//
int CThread::ThreadKick(void* pVoid)
{
	// 割り込み中は、エラーとする
	//
	OUTSIDE_INTERRUPT_CHECK();

	CThread* pThread = (CThread*)pVoid;
	return pThread->ThreadFunc();
}
