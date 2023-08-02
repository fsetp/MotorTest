////////////////////////////////////////////////////////////////////////////////
// Furicha's Os Realtime Tiny Embedded
//
//	2007.11.18	new generated
//	2007.11.19	change priority value
//	2007.11.21	suspend, resume追加
//	2007.11.22	割り込み内でのイベント解除追加
//	2007.11.25	いくつかを__inlineに
//				同期オブジェクトのロック・アンロックにバグがあった
//				スタックチェック追加
//				グローバルエラー追加
//	2007.11.27	割り込み中フラグを追加
//				SetEvent()に割り込み中処理を追加
//	2007.11.28	イベントのLock()で、抜ける時にリセットするようにした
//				run()で自タスクにも切り替わってた（自タスクはスルー）
//	2007.11.29	イベントのLock()でタイムアウトや自動リセットの事を考えていなかった
//	2007.12.01	CreateTaskとGetScheduledTaskをシステムコールとして割込みへ移行
//					結果としてスタック消費量を改善した
//				システム側のスタックと割込みのスタックを新設
//					結果としてスタック消費量を改善した
//				DestroyTaskのバグを修正
//				DestroyTaskにおいて最後のタスクの場合EnterNewTaskの文脈に返る
//					用に修正
//				マクロENTER_INTERRUPTとLEAVE_INTERRUPTを追加
//	2007.12.02	レビジン追加
//				システムと割込みのスタックオーバーフローチェック追加
//	2007.12.05	CTaskのコンストラクタの動作を変更
//	2007.12.06	派生クラスの基底クラスのデストラクタをvirutalにするの忘れてた
//	2007.12.08	システムタスクの概念を創り始める
//	2007.12.15	deleteで配列記述をしていなかった
//	2007.12.16	システムタスクのIDを-1とする
//				無効タスクIDをINT_MAXとする
//				タスクに対する操作をシステムタスクへのシステムコールとする
//				タスク切替時は一旦システムタスクへ切り替える
//				WaitUs()追加
//	2007.12.17	インターバルタイマ追加
//	2007.12.22	CTaskのCreateTask()にバグあり（スタック足りない場合、中途半端な生成だった）
//	2008.01.02	Sleepを()イベントと同等にした
//	2008.01.27	同期オブジェクトをシステムオブジェクトに変更
//	2008.01.27	GetTickCount()追加
//	2008.02.03	GetLastError()をCTaskCtrlの関数に変更
//	2008.03.29	CThread::CreateThread()の戻り値判定を修正
//	2008.04.29	SR_SLEEP削除
//	2008.04.29	g_nIntModeへ割込みモードを格納
//	2008.05.10	WaitForMultipleObjects()対応
//				WaitForSingleObject()を内部的にはWaitForMultipleObject()とした
//				タスクの優先順位をFIFOの管理とした（m_schedulefifo）
//				伴って、タスク優先順位変更関数からタスク切替を行うようにした
//	2008.05.17	Sleep(INFINITE)なら、タスク停止とする
//				EnableDebugMode(bool bEnable)追加
//				タスク切替追跡キューを追加
//				GetTaskChangeQueue()、GetTaskChangeQueueSize()追加
//	2008.05.18	forte16lib.hを作るに伴い、できるだけcppへ移行した
//	2008.05.22	WaitForMultipleObject()とGetScheduledTask()にイベント処理上のバグがあった
//	2008.05.25	タスクステートをRUNだけでなく、WAITなどを追加
//				CheckWaitTask()を追加して同期オブジェクトによるタスク開放を整理した
//	2008.05.31	CheckWaitTask()でBLOCKOUTを返すところ、TIMEOUTを返していた
//	2008.06.07	WaitForMultipleObjects()でタスクをWaitにしていなかった
//				SysTaskFunc()でSR_*フラグの設定が適切でなかった
//	2008.06.09	CheckWaitTask()で待ちタスクでない場合、SR_NOPにしていた
//	2008.06.22	CPUクロックをInitialize()の引数とした
//				GetCpuName()、GetMaxSyncObjects()追加
//	2008.07.05	CThread::SignleLock()追加
//				CTaskのメンバにm_nStackSizeを追加
//	2008.07.06	スタックチェックサインを0xccに変更
//				スタックの中を生成時にスタックチェックサインで埋めるようにした
//				CTaskのに使用中のスタックサイズを返すGetUsingStackSize()追加
//				CTaskCtrlにGetUsingStackSize(int task)追加
//				CThreadにGetUsingStackSize()追加
//				変数がforte使用側の変数と衝突しないように、_を付加した
//	2008.07.27	GetScheduledTask()の中を割り込み禁止とした
//

#ifndef	_FORTE32_H_
#define	_FORTE32_H_

#include <machine.h>
#include <limits.h>
#include "fortedef.h"

#ifdef __cplusplus
extern "C" {
#endif
void abort(void);
#ifdef __cplusplus
}
#endif

////////////////////////////////////////
//
//#define	DEBUG
//#define	STATIC_SYSTEM_OBJECT
//#define	USE_SYS_HEAP

#ifdef	USE_SYS_HEAP
#ifdef __cplusplus
extern "C" {
void* sys_malloc(size_t allocsz);
void sys_free(void *ptr);
}
#endif
#endif

////////////////////////////////////////
// Global Description
//
#ifndef	NULL
#ifdef __cplusplus
    #define NULL 0
#else
    #define NULL ((void *)0)
#endif
#endif

// wait time infinite
//
#ifndef	INFINITE
#define	INFINITE	0xffffffff
#endif

// version
//
#define	FORTE_RELEASE	"15.03.26"

////////////////////////////////////////
// Priority Description
//
#define THREAD_PRIORITY_TIME_CRITICAL	0
#define THREAD_PRIORITY_HIGHEST			1
#define THREAD_PRIORITY_ABOVE_NORMAL	2
#define THREAD_PRIORITY_NORMAL			3
#define THREAD_PRIORITY_BELOW_NORMAL	4
#define THREAD_PRIORITY_LOWEST			5
#define THREAD_PRIORITY_ABOVE_IDLE		6
#define THREAD_PRIORITY_IDLE			7
#define	THREAD_PRIORITY_MAX_VALUE		127

////////////////////////////////////////
// Task Control
//
#define	WAIT_TIMEOUT		0xfffffffe
#define	WAIT_OBJECT_0		0x00000000
#define	WAIT_ABANDONED_0	0x80000000
#define	WAIT_ABANDONED		WAIT_ABANDONED_0
#define	WAIT_FAILED			0xffffffff

////////////////////////////////////////
// Global Error Code
//
#define	ERROR_NONE						0	//
#define	ERROR_TASK_STACK_OVERFLOW		1	// タスクスタックを超えて上書きされた
#define	ERROR_SYSTEM_STACK_OVERFLOW		2	// システムスタックを超えて上書きされた
#define	ERROR_INTERRUPT_STACK_OVERFLOW	3	// 割込みスタックを超えて上書きされた
#define	ERROR_NO_STACK_MEMORY			4	// タスクのためのメモリが足りない
#define	ERROR_NO_TASK_EXIST				5	// 全てのタスクが終了した
#define	ERROR_TERM_EXPIRED				6	// 有効期限が満了した
#define	ERROR_INSIDE_INTERRUPT			7	// 割り込み中に呼ばれてはいけない
#define	ERROR_OUTSIDE_INTERRUPT			8	// 割り込み中に呼ばれる必要がある

extern	int	g_nLastError;
extern	int	g_nErrorTask;

////////////////////////////////////////
// Interrupt Macro Definition
//
extern bool g_bInterrupt;
#define	ENTER_INTERRUPT()	g_bInterrupt = true
//#define	LEAVE_INTERRUPT()	g_bInterrupt = false; CHECK_STACK_SIGN2(_g_IntStack, ERROR_INTERRUPT_STACK_OVERFLOW)
#define	LEAVE_INTERRUPT()	g_bInterrupt = false

#define	IS_INTERRUPT()		g_bInterrupt

#define	ENABLE_INTERRUPT(e)	if (!g_bInterrupt){ if(e) setpsw_i(); else clrpsw_i();}

#define	OUTSIDE_INTERRUPT_CHECK()			\
if (g_bInterrupt)	{						\
	g_nLastError = ERROR_INSIDE_INTERRUPT;	\
	abort();								\
}
#define	INSIDE_INTERRUPT_CHECK()			\
if (!g_bInterrupt)	{						\
	g_nLastError = ERROR_OUTSIDE_INTERRUPT;	\
	abort();								\
}

////////////////////////////////////////
// Stack Check
//
#define	STACK_SIGN	0xcc
#define	FILL_STACK_SIGN(p, s)			\
for (int _i = 0; _i < s; _i++)			\
	((char*)p)[_i] = STACK_SIGN
#define	CHECK_STACK_SIGN(p, t, e)			\
if (p) {									\
	if ((*(char*)p) != (char)STACK_SIGN) {	\
		g_nErrorTask = t;					\
		g_nLastError = e;					\
		abort();							\
	}										\
}

#define	CHECK_STACK_SIGN2(p, t, e)		\
if ((*(char*)p) != (char)STACK_SIGN) {	\
	g_nErrorTask = t;					\
	g_nLastError = e;					\
	abort();							\
}

////////////////////////////////////////
//
#define	INVALID_TASK_ID		INT_MAX		// 無効なタスクID
#define	SYSTEM_TASK_ID		-1			// システムタスクID

////////////////////////////////////////
// オブジェクトハンドル
typedef unsigned long HANDLE;
#define	INVALID_HANDLE	0xffffffff		// 無効ハンドル

////////////////////////////////////////////////////////////////////////////////
// 同期オブジェクトプリミティブ
class CSyncObjectCore
{
public:
	CSyncObjectCore()
	: m_nLockCount(0)
	, m_nMaximumCount(0)
	{
#ifdef	STATIC_SYSTEM_OBJECT
	m_bValid = false;
#endif

#ifdef	DEBUG
	m_sign[0] = '-';
	m_sign[1] = '-';
	m_task = -1;
#endif
	}

	virtual ~CSyncObjectCore()	{}

protected:
	int	m_nLockCount;		// 0:non signal / >0 signal
	int	m_nMaximumCount;	//

#ifdef	STATIC_SYSTEM_OBJECT
public:
	bool	m_bValid;
#endif

#ifdef	DEBUG
protected:
	char	m_sign[2];
	short	m_task;
public:
	void SetTaskId(int id)
	{
		m_task = (short)id;
	}
#endif

public:
	virtual bool DecrementLockCount();
	virtual void IncrementLockCount();

	virtual bool Lock()		{return false;}
	virtual bool Unlock()	{return false;}

#ifdef	STATIC_SYSTEM_OBJECT
	bool IsSignal()		{return (m_nLockCount > 0);}
#else
	virtual	bool IsSignal() = 0;
#endif

#ifdef	USE_SYS_HEAP
	//
	static void* operator new(size_t size)
	{
		return sys_malloc(size);
	}

	static void operator delete(void* p)
	{
		sys_free(p);
	}
#endif
};

////////////////////////////////////////
// イベントプリミティブ
class CEventCore : public CSyncObjectCore
{
public:
	CEventCore(bool bManualReset = false, bool bInitialState = false)
	{
		m_nLockCount    = bInitialState ? 1 : 0;
		m_nMaximumCount = bManualReset ? 1 : 0;

#ifdef	DEBUG
		m_sign[0] = 'E';
		m_sign[1] = 'V';
#endif
	}

	~CEventCore()	{}

protected:

public:
	bool IsManualReset()	{return (m_nMaximumCount == 1);}

	bool SetEvent()		{m_nLockCount = 1; return true;}
	bool ResetEvent()	{m_nLockCount = 0; return true;}
	bool IsSignal()		{return (m_nLockCount > 0);}
};

////////////////////////////////////////
// セマフォプリミティブ
class CSemaphoreCore : public CSyncObjectCore
{
public:
	CSemaphoreCore(int nInitialCount, int nMaximumCount)
	{
		m_nLockCount    = nInitialCount;
		m_nMaximumCount = nMaximumCount;

#ifdef	DEBUG
		m_sign[0] = 'S';
		m_sign[1] = 'E';
#endif
	}

	~CSemaphoreCore()	{}

protected:

public:
	bool IsSignal()				{return (m_nLockCount > 0);}
	bool Lock();
	bool Unlock();

};

////////////////////////////////////////
// ミューテックスプリミティブ
class CMutexCore : public CSyncObjectCore
{
public:
	CMutexCore()
	{
		m_nLockCount    = 0;
		m_nMaximumCount = 1;

#ifdef	DEBUG
		m_sign[0] = 'M';
		m_sign[1] = 'U';
#endif
	}

	~CMutexCore()	{}

protected:

public:
	bool IsSignal()				{return (m_nLockCount > 0);}
	bool Lock();
	bool Unlock();

};

////////////////////////////////////////
// タスクスケジューリングFIFO
//
typedef struct tagPRIO_INFO {
	int	task;			// タスク番号
	int	priority;		// 優先順位
} PRIO_INFO;

class CScheduleFifo
{
public:
	CScheduleFifo()
	: m_read(0)
	, m_write(0)
	, m_cnt(0)
	{
	}

	~CScheduleFifo()
	{
	}

protected:
	PRIO_INFO	m_fifo[MAX_TASKS];	// タスク管理FIFO
	int			m_read;
	int			m_write;
	int			m_cnt;

public:
	// キューの終端に追加
	void AppendTail(int task, int priority)
	{
		PRIO_INFO info = {task, priority};
		AppendTail(info);
	}

	// キューの終端に追加
	void AppendTail(PRIO_INFO info)
	{
		m_fifo[m_write++] = info;
		if (m_write >= MAX_TASKS)
			m_write = 0;
		m_cnt++;
	}

	// プライオリティ付き追加
	void AppendPriority(int task, int priority)
	{
		PRIO_INFO info = {task, priority};
		AppendPriority(info);
	}

	// プライオリティ付き追加
	void AppendPriority(PRIO_INFO info)
	{
		bool inserted = false;
		int size = m_cnt;
		for (int i = 0; i < size; i++) {
			PRIO_INFO temp = GetHead();
			if (temp.priority > info.priority) {
				AppendTail(info);
				inserted = true;
			}
			AppendTail(temp);
		}
		if (!inserted)
			AppendTail(info);
	}

	// キューの先頭から取得
	PRIO_INFO GetHead()
	{
		PRIO_INFO info = {-1, -1};

		if (m_cnt > 0) {
			info = m_fifo[m_read++];
			if (m_read >= MAX_TASKS)
			m_read = 0;
			m_cnt--;
		}
		return info;
	}

	// タスクID指定のキューからの削除
	void DeleteTask(int task)
	{
		int size = m_cnt;
		for (int i = 0; i < size; i++) {
			PRIO_INFO info = GetHead();
			if (info.task != task)
				AppendTail(info);
		}
	}

	// キューサイズ
	int GetSize()
	{
		return m_cnt;
	}

protected:

	PRIO_INFO operator[](int idx)
	{
		PRIO_INFO info = {-1, -1};

		if (idx < m_cnt) {
			int pos = m_read + idx;
			if (pos > MAX_TASKS)
				pos -= MAX_TASKS;
			info = m_fifo[pos];
		}
		return info;
	}
};

////////////////////////////////////////
// タスク変更確認デバッグキュー
class CTaskChgQueue
{
public:
	CTaskChgQueue()
	{
		for (int i = 0; i < NUM; i++)
			m_queue[i] = INVALID_TASK_ID;
	}

	~CTaskChgQueue()	{}

protected:
	enum	{NUM = 16};
	int	m_queue[NUM];

public:
	void put(int num)
	{
		for (int i = NUM - 1; i > 0; i--)
			m_queue[i] = m_queue[i - 1];
		m_queue[0] = num;
	}

	const int* getQueue()	{return m_queue;}
	int getSize()			{return NUM;}
};

////////////////////////////////////////
// タスク関数の型
typedef	int (*TASK_FUNC)(void* pVoid);

////////////////////////////////////////
// タスク終了関数の型
typedef void (*TASK_DESTROY)(void);

////////////////////////////////////////
// タスク状態フラグ
//
typedef struct tagTASK_FLAG {
	unsigned char	nState		: 2;	// タスク実行ステータス
	unsigned char	nPriority	: 6;	// タスクプライオリティ
} TASK_FLAG;

// タスクステート
enum	{	TS_NO_CREATE,	// タスク生成前
			TS_SUSPEND,		// 停止状態
			TS_RUN,			// 実行状態
			TS_WAIT			// 待ち状態
		};

////////////////////////////////////////
// タスククラス
//
class CSyncObject;
class CEvent;
class CSemaphore;
class CMutex;
class CCriticalSection;
class CTaskCtrl;
class CTask
{
	friend CSyncObject;
	friend CEvent;
	friend CSemaphore;
	friend CMutex;
	friend CCriticalSection;
	friend CTaskCtrl;
private:
	CTask();
	~CTask();

private:
	// タスクの生成
	//
	bool CreateTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy);

protected:
	unsigned long	m_dwReg[REG_NUM + 1];	// 退避レジスタ
	char*			m_pStackFrame;			// スタックフレームポインタ
	unsigned int	m_nStackSize;			// スタックバイトサイズ
	unsigned int	m_nLockMs;				// スリープ、イベントなどの減算カウンタ
	TASK_FLAG		m_TaskFlag;				// タスクフラグ

	const HANDLE*	m_phSyncObjects;		// 同期オブジェクト配列ポインタ
	int				m_nSyncObjectCount;		// 待ち同期オブジェクト数
	bool			m_bSyncObjectWaitAll;	// 全ての同期待ちかのフラグ
	int				m_nSignalSyncObjectIdx;	// 全てでない場合のシグナルオブジェクトインデックス（戻り）
	bool			m_bValid;

	// タスクに切り替わった理由
	enum {	SR_NOP,
			SR_SWITCH,		// 通常タスク切替
			SR_RUN,			// タスク実行
			SR_SUSPEND,		// タスク停止
			SR_BLOCKOUT,	// 同期解除
			SR_TIMEOUT		// 同期タイムアウト
		};
	int	m_nSwitchReson;

private:
	// タスク切替時の理由コード
	__inline void SetSwitchReson(int sr)				{m_nSwitchReson = sr;}
	__inline int GetSwitchReson()						{return m_nSwitchReson;}

	// タスクのスタックフレーム先頭ポインタ
	__inline char* GetStackFrame()						{return m_pStackFrame;}

	// タスクのレジスタ保存配列
	__inline void* GetTaskReg()							{return m_dwReg;}

	// タスクの実行状態
	__inline void SetTaskRun(bool bRun)					{m_TaskFlag.nState = bRun ? TS_RUN : TS_SUSPEND;}
	__inline void SetTaskWait(bool bWait)				{m_TaskFlag.nState = bWait ? TS_WAIT : TS_RUN;}
	__inline bool IsTaskRun()							{return (m_TaskFlag.nState == TS_RUN);}
	__inline bool IsTaskWait()							{return (m_TaskFlag.nState == TS_WAIT);}

	// タスクプライオリティ
	__inline int GetPriority()							{return m_TaskFlag.nPriority;}
	__inline void SetPriority(int nPriority)			{m_TaskFlag.nPriority = nPriority;}

	// 同期ロックタイムアウト
	__inline void SetLockMs(unsigned int ms)			{m_nLockMs = ms;}
	__inline unsigned int GetLockMs()					{return m_nLockMs;}

	// 同期オブジェクト
	__inline void SetSyncObjectHandlePt(const HANDLE* phSyncObjects)	{m_phSyncObjects = phSyncObjects;}
	__inline const HANDLE* GetSyncObjectHandlePt()		{return m_phSyncObjects;}
	__inline int GetSyncObjectCount()					{return m_nSyncObjectCount;}
	__inline bool IsSyncObjectWaitAll()					{return m_bSyncObjectWaitAll;}
	__inline void SetSyncObjectIndex(int idx)			{m_nSignalSyncObjectIdx = idx;}

	// ウェイトオブジェクトハンドル配列をセットする
	__inline void SetSyncObjectHandles(int nCount, const HANDLE* phSyncObjects, bool bWaitAll)
	{
		m_nSyncObjectCount   = nCount;
		m_phSyncObjects      = phSyncObjects;
		m_bSyncObjectWaitAll = bWaitAll;
	}

	// 同期ロック時間の減算（インターバルから呼び出される）
	__inline void DecrementLockMs()
	{
		if (m_nLockMs != INFINITE) {
			if (m_nLockMs > 0)
				m_nLockMs--;
		}
	}

	// 使用中のスタックサイズを返す
	unsigned int GetUsingStackSize()
	{
		unsigned int nUse = m_nStackSize;
		const char *p = (const char*)m_pStackFrame;
		for (int i = 2; i < m_nStackSize; i++) {
			if (p[i] == (char)STACK_SIGN)
				nUse--;
			else
				break;
		}
		return nUse;
	}
#ifdef	USE_SYS_HEAP
	//
	static void* operator new(size_t size)
	{
		return sys_malloc(size);
	}

	static void operator delete(void* p)
	{
		sys_free(p);
	}
#endif
};

////////////////////////////////////////
// インターバルコールバック管理
//
typedef	bool (*ITVL_CALLBACK_FUNC)(void* pVoid);
typedef struct tagINTERVAL_CALLBACK {
	ITVL_CALLBACK_FUNC	pFunc;		// コールバック関数	bool foo(void* pVoid)の形式
	void*				pVoid;		// コールバック関数に渡すパラメータ
	unsigned int		timer;		// ms初期値
	unsigned int		counter;	// msのカウンタ
	bool				bValid;		//
} INTERVAL_CALLBACK;

////////////////////////////////////////
//
class CIntervalTimer
{
friend CTaskCtrl;
private:
	CIntervalTimer();
	~CIntervalTimer();

protected:

	INTERVAL_CALLBACK	m_IntervalCalllback[MAX_INTERVAL_CALLBACK];

private:
	// コールバック関数の登録
	//
	//	引数	ms		コールバック周期
	//			pFunc	コールバック関数ポインタ	bool foo(void* pVoid)の形式
	//			pVoid	コールバック関数の引数
	//
	//	戻値	>=0		コールバックID
	//			-1		エラー
	//
	int CreateCallback(unsigned int ms, ITVL_CALLBACK_FUNC pFunc, void* pVoid);

	// コールバック関数の削除
	//
	//	引数	id		コールバックID
	//
	void DeleteCallback(int id);

	// コールバック処理
	//	インターバルタイマから呼ばれる
	//
	void ScanAndCall();
};

////////////////////////////////////////////////////////////////////////////////
// CTaskCtrl

////////////////////////////////////////
// タスク制御クラス
//
class CSyncObject;
class CEvent;
class CSemaphore;
class CMutex;
class CCriticalSection;
class CTaskCtrl
{
	friend CSyncObject;
	friend CEvent;
	friend CSemaphore;
	friend CMutex;
	friend CCriticalSection;

public:
	CTaskCtrl();
	~CTaskCtrl();

protected:
	int		m_nCpuClockFreq;	// CPUクロック
	CTask	m_Task[MAX_TASKS];	// タスク管理配列
	int		m_nTaskID;			// 現在実行中のタスク番号
	bool	m_bInitialized;		// 初期化済みフラグ

	CTask	m_sysTask;			// システムタスク

	// システムコールシーケンス
	enum	{	SYS_NOP,		// 何もしない
				SYS_CREATE,		// タスク生成
				SYS_RUN,		// タスク実行
				SYS_SUSPEND,	// タスク停止
				SYS_LOCK,		// タスク同期待ち
				SYS_DESTROY		// タスク終了
			};
	int		m_seqSystem;

	// システムコールパラメータ
	int				m_nOrderTask;
	TASK_FUNC		m_pFunc;
	int				m_nPriority;
	unsigned int	m_nStackSize;
	void*			m_pVoid;
	TASK_DESTROY	m_pDestroy;

	// インターバルクラスポインタ
	CIntervalTimer m_IntervalTimer;

	// 同期オブジェクト配列
#ifdef	STATIC_SYSTEM_OBJECT
	CSyncObjectCore m_SyncObject[MAX_SYNC_OBJECTS];
#else
	CSyncObjectCore* m_pSyncObject[MAX_SYNC_OBJECTS];
#endif

	HANDLE			m_hCritical;	// クリティカルセクションハンドル

	// タスクスケジュールFIFO
	CScheduleFifo	m_schedulefifo;

	// チックカウント
	unsigned long	m_dwTickCount;

private:
	// タスクIDからタスククラスポインタを得る
	//
	CTask* GetTask(int task);

	// 有効なタスクIDかを返す
	//
	bool IsValidTask(int task);

	// 現在実行中のタスククラスポインタを返す
	CTask* GetCurrentTask();

public:
	// 製造者名
	static const char* GetProductString();

	// 著作権
	static const char* GetCopyrightString();

	// バージョン
	static const char* GetVersionString();

	// エラー番号の取得
	static int GetLastError();

	// タスクIDの取得
	int GetLastTaskID();

	// デバッグモードを有効にする
	static void EnableDebugMode(bool bEnable);

	// タスク切替キューのポインタを得る
	static const int* GetTaskChangeQueue();

	// タスク切替キューサイズを得る
	static int GetTaskChangeQueueSize();

	// 最大タスク数
	static int GetMaxTasks();

	// 最大同期オブジェクト数
	static int GetMaxSyncObjects();

	// 最大インターバルコールバック数
	static int GetMaxIntervalCallbacks();

	// クロック周波数
	int GetClockFrequency();

	// システムタスクのスタックサイズ
	static short GetSystemStackSize();

	// CPU名称を返す
	static const char* GetCpuName();

	// 使用中のスタックサイズを返す
	unsigned int GetUsingStackSize(int task);

public:
	// 初期化
	bool Initialize(int nFreq);

	// 初期化済みか返す
	bool IsInitialized();

	// タスクの生成
	//
	int CreateTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy);
	int CreateTask_OnNoTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy);

	// タスクの破棄
	//	m_nTaskIDがtaskであった場合、無効となるので注意する
	//
	void DeleteTCB(int task);

	// タスク実行
	void RunTask(int task, bool bRun = true);

	// タスクの破棄
	void DestroyCurrentTask();

	// スリープ
	void Sleep(unsigned int ms);

	// μ秒待ち
	void WaitUs(int us);

	// 指定タスクの優先順位
	void ChangeTaskPriority(int task, int priority);

	// チックカウント
	unsigned long GetTickCount();

	// コールバック関数の登録
	//
	//	引数	ms		コールバック周期
	//			pFunc	コールバック関数ポインタ	bool foo(void* pVoid)の形式
	//			pVoid	コールバック関数の引数
	//
	//	戻値	>=0		コールバックID
	//			-1		エラー
	//
	int CreateIntervalCallback(int ms, ITVL_CALLBACK_FUNC pFunc, void* pVoid);

	// コールバック関数の削除
	//
	//	引数	id		コールバックID
	//
	void DeleteIntervalCallback(int id);

	// 単一同期待ち
	int WaitForSingleObject(HANDLE hSyncObject, unsigned int ms);

	// 複数同期待ち
	int WaitForMultipleObjects(int nCount, const HANDLE* phSyncObjects, bool bWaitAll, unsigned int ms);

	// 同期オブジェクトの削除
	void DeleteSyncObject(HANDLE hSyncObject);

	// イベント関数
	HANDLE CreateEvent(bool bManualReset = false, bool bInitialState = false);
	bool SetEvent(HANDLE hEvent);
	bool PulseEvent(HANDLE hEvent);
	bool ResetEvent(HANDLE hEvent);

	// セマフォ関数
	HANDLE CreateSemaphore(int nInitialCount = 1, int nMaximumCount = 1);
	bool ReleaseSemaphore(HANDLE hSemaphore);

	// ミューテックス関数
	HANDLE CreateMutex();

	// クリテイカルセクション関数
	void EnterCriticalSection();
	void LeaveCriticalSection();

protected:
	// 同期ハンドルから同期プリミティブオブジェクトを得る
	CSyncObjectCore* _FromHandle(HANDLE hSyncObjet);

	// システムタスク関数をクラス関数として実行するため
	//		キックするための静的関数
	static int _SysTaskKick(void* pVoid);

	// システムタスク関数
	int SysTaskFunc();

	//
	void InitializeClock();

	// インターバルタイマの初期化
	void InitializeInterval();

	// ウォッチドッグタイマの初期化
	void InitializeWatchDogTimer();

	// ウォッチドッグタイマのリフレッシュ
	void RefreshWatchDogTimer();

	// インターバルコールバックの実際
	void IntervalTimerScan();

	// 同期待ちのmsタイマのデクリメント
	void DecrementWaitMs();

	// スケジュールされたタスクを返す
	bool GetScheduledTask(int* pnTask);

	// 同期待ちタスクの解除処理
	bool CheckWaitTask(int nTaskID);

	// 必要なら、自動リセットイベントをリセットする
	bool ResetAutoEvent(int nTaskID);

public:
	// 初めてTaskSwitcherが実行されたときの文脈へ戻す
	__inline static void RestorSystem();

	__inline static void SwitchTask();

	__inline static void ChangeSupervisor();

	__inline static void ChangeUser();

	__inline static void EnterSystemCritical();
	__inline static void LeaveSystemCritical();

	__inline static void DisableInterrupt();
	__inline static void EnableInterrupt();


protected:
	// タスクレジスタを切り替える（スタックの関係で、TaskSwitcher()にまとめられない）
	static void SetupTaskParam();

//#pragma	interrupt CTaskCtrl::IntervalIntProc(vect = 28)
	static void IntervalIntProc();

//public:
//	static CTaskCtrl& GetTaskCtrl();

};

extern CTaskCtrl		g_TaskCtrl;
#define	GetTaskCtrl()	g_TaskCtrl
////////////////////////////////////////////////////////////////////////////////
//
#include <stdarg.h>
////////////////////////////////////////
// スレッドプリミティブ関数
//
class CThread
{
public:
	CThread();

	virtual ~CThread();

protected:
	int	m_nTaskId;

public:
	bool Initialize(int nFreq);

	//
	bool CreateThread(int nPriority, unsigned int nStackSize);

	__inline void Run()				{_run(true);}
	__inline void Stop()			{_run(false);}
	__inline void Suspend()			{_run(false);}
	__inline void Resume()			{_run(true);}

protected:
	void Sleep(unsigned int ms);
	void WaitUs(int us);
	unsigned long GetTickCount();
	bool SignleLock(CSyncObject* pObject, unsigned int ms = INFINITE);
	int MultiLock(bool bWaitAll, unsigned int ms, int nCount, ...);
	unsigned int GetUsingStackSize();

//protected:
//	HANDLE	m_WaitHandles[MAX_WAIT_HANDLES];

protected:
	void _run(bool bRun);

	//
	static int ThreadKick(void* pVoid);

	virtual int ThreadFunc() = 0;
};

////////////////////////////////////////
// 同期オブジェクトの基底クラス
//	単独生成は不可（派生元）
//
class CTask;
class CTaskCtrl;
class CSyncObject
{
	friend CTask;
	friend CTaskCtrl;

protected:
	CSyncObject();
	virtual ~CSyncObject();

public:
	virtual HANDLE Create() = 0;
	operator HANDLE()	{return m_hSyncObject;}

protected:
	HANDLE	m_hSyncObject;
};

////////////////////////////////////////
// CEvent
//
class CTaskCtrl;
class CEvent : public CSyncObject
{
	friend CTaskCtrl;
public:
	CEvent(bool bManualReset = false, bool bInitialState = false);
	~CEvent();

protected:
	bool m_bManualReset;
	bool m_bInitialState;

public:
	HANDLE Create();
	bool SetEvent();
	bool ResetEvent();
	bool PulseEvent();

	bool Lock(unsigned int ms);
};

////////////////////////////////////////
// CSemaphore
//
class CTaskCtrl;
class CSemaphore : public CSyncObject
{
	friend CTaskCtrl;

public:
	CSemaphore(int nInitialCount = 1, int nMaximumCount = 1);
	~CSemaphore();

protected:
	int m_nInitialCount;
	int m_nMaximumCount;

public:
	HANDLE Create();

	// 使用 or 待ち
	bool Lock(unsigned int ms);

	// 使用解除
	bool Unlock();
};

////////////////////////////////////////
// CMutex
//
class CTaskCtrl;
class CMutex : public CSyncObject
{
	friend CTaskCtrl;

public:
	CMutex();
	~CMutex();

public:
	HANDLE Create();

	// 使用 or 待ち
	bool Lock(unsigned int ms);

	// 使用解除
	bool Unlock();
};

////////////////////////////////////////
// CCriticalSection
//
class CTaskCtrl;
class CCriticalSection : public CSyncObject
{
	friend CTaskCtrl;
public:
	CCriticalSection();
	~CCriticalSection();

public:
	HANDLE Create();

	// 領域に入る
	bool Lock();

	// 領域から出る
	bool Unlock();
};

////////////////////////////////////////////////////////////////////////////////
#endif	//	_FORTE16_H_
