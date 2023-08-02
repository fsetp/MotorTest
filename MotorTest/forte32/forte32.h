////////////////////////////////////////////////////////////////////////////////
// Furicha's Os Realtime Tiny Embedded
//
//	2007.11.18	new generated
//	2007.11.19	change priority value
//	2007.11.21	suspend, resume�ǉ�
//	2007.11.22	���荞�ݓ��ł̃C�x���g�����ǉ�
//	2007.11.25	��������__inline��
//				�����I�u�W�F�N�g�̃��b�N�E�A�����b�N�Ƀo�O��������
//				�X�^�b�N�`�F�b�N�ǉ�
//				�O���[�o���G���[�ǉ�
//	2007.11.27	���荞�ݒ��t���O��ǉ�
//				SetEvent()�Ɋ��荞�ݒ�������ǉ�
//	2007.11.28	�C�x���g��Lock()�ŁA�����鎞�Ƀ��Z�b�g����悤�ɂ���
//				run()�Ŏ��^�X�N�ɂ��؂�ւ���Ă��i���^�X�N�̓X���[�j
//	2007.11.29	�C�x���g��Lock()�Ń^�C���A�E�g�⎩�����Z�b�g�̎����l���Ă��Ȃ�����
//	2007.12.01	CreateTask��GetScheduledTask���V�X�e���R�[���Ƃ��Ċ����݂ֈڍs
//					���ʂƂ��ăX�^�b�N����ʂ����P����
//				�V�X�e�����̃X�^�b�N�Ɗ����݂̃X�^�b�N��V��
//					���ʂƂ��ăX�^�b�N����ʂ����P����
//				DestroyTask�̃o�O���C��
//				DestroyTask�ɂ����čŌ�̃^�X�N�̏ꍇEnterNewTask�̕����ɕԂ�
//					�p�ɏC��
//				�}�N��ENTER_INTERRUPT��LEAVE_INTERRUPT��ǉ�
//	2007.12.02	���r�W���ǉ�
//				�V�X�e���Ɗ����݂̃X�^�b�N�I�[�o�[�t���[�`�F�b�N�ǉ�
//	2007.12.05	CTask�̃R���X�g���N�^�̓����ύX
//	2007.12.06	�h���N���X�̊��N���X�̃f�X�g���N�^��virutal�ɂ���̖Y��Ă�
//	2007.12.08	�V�X�e���^�X�N�̊T�O��n��n�߂�
//	2007.12.15	delete�Ŕz��L�q�����Ă��Ȃ�����
//	2007.12.16	�V�X�e���^�X�N��ID��-1�Ƃ���
//				�����^�X�NID��INT_MAX�Ƃ���
//				�^�X�N�ɑ΂��鑀����V�X�e���^�X�N�ւ̃V�X�e���R�[���Ƃ���
//				�^�X�N�ؑ֎��͈�U�V�X�e���^�X�N�֐؂�ւ���
//				WaitUs()�ǉ�
//	2007.12.17	�C���^�[�o���^�C�}�ǉ�
//	2007.12.22	CTask��CreateTask()�Ƀo�O����i�X�^�b�N����Ȃ��ꍇ�A���r���[�Ȑ����������j
//	2008.01.02	Sleep��()�C�x���g�Ɠ����ɂ���
//	2008.01.27	�����I�u�W�F�N�g���V�X�e���I�u�W�F�N�g�ɕύX
//	2008.01.27	GetTickCount()�ǉ�
//	2008.02.03	GetLastError()��CTaskCtrl�̊֐��ɕύX
//	2008.03.29	CThread::CreateThread()�̖߂�l������C��
//	2008.04.29	SR_SLEEP�폜
//	2008.04.29	g_nIntMode�֊����݃��[�h���i�[
//	2008.05.10	WaitForMultipleObjects()�Ή�
//				WaitForSingleObject()������I�ɂ�WaitForMultipleObject()�Ƃ���
//				�^�X�N�̗D�揇�ʂ�FIFO�̊Ǘ��Ƃ����im_schedulefifo�j
//				�����āA�^�X�N�D�揇�ʕύX�֐�����^�X�N�ؑւ��s���悤�ɂ���
//	2008.05.17	Sleep(INFINITE)�Ȃ�A�^�X�N��~�Ƃ���
//				EnableDebugMode(bool bEnable)�ǉ�
//				�^�X�N�ؑ֒ǐՃL���[��ǉ�
//				GetTaskChangeQueue()�AGetTaskChangeQueueSize()�ǉ�
//	2008.05.18	forte16lib.h�����ɔ����A�ł��邾��cpp�ֈڍs����
//	2008.05.22	WaitForMultipleObject()��GetScheduledTask()�ɃC�x���g������̃o�O��������
//	2008.05.25	�^�X�N�X�e�[�g��RUN�����łȂ��AWAIT�Ȃǂ�ǉ�
//				CheckWaitTask()��ǉ����ē����I�u�W�F�N�g�ɂ��^�X�N�J���𐮗�����
//	2008.05.31	CheckWaitTask()��BLOCKOUT��Ԃ��Ƃ���ATIMEOUT��Ԃ��Ă���
//	2008.06.07	WaitForMultipleObjects()�Ń^�X�N��Wait�ɂ��Ă��Ȃ�����
//				SysTaskFunc()��SR_*�t���O�̐ݒ肪�K�؂łȂ�����
//	2008.06.09	CheckWaitTask()�ő҂��^�X�N�łȂ��ꍇ�ASR_NOP�ɂ��Ă���
//	2008.06.22	CPU�N���b�N��Initialize()�̈����Ƃ���
//				GetCpuName()�AGetMaxSyncObjects()�ǉ�
//	2008.07.05	CThread::SignleLock()�ǉ�
//				CTask�̃����o��m_nStackSize��ǉ�
//	2008.07.06	�X�^�b�N�`�F�b�N�T�C����0xcc�ɕύX
//				�X�^�b�N�̒��𐶐����ɃX�^�b�N�`�F�b�N�T�C���Ŗ��߂�悤�ɂ���
//				CTask�̂Ɏg�p���̃X�^�b�N�T�C�Y��Ԃ�GetUsingStackSize()�ǉ�
//				CTaskCtrl��GetUsingStackSize(int task)�ǉ�
//				CThread��GetUsingStackSize()�ǉ�
//				�ϐ���forte�g�p���̕ϐ��ƏՓ˂��Ȃ��悤�ɁA_��t������
//	2008.07.27	GetScheduledTask()�̒������荞�݋֎~�Ƃ���
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
#define	ERROR_TASK_STACK_OVERFLOW		1	// �^�X�N�X�^�b�N�𒴂��ď㏑�����ꂽ
#define	ERROR_SYSTEM_STACK_OVERFLOW		2	// �V�X�e���X�^�b�N�𒴂��ď㏑�����ꂽ
#define	ERROR_INTERRUPT_STACK_OVERFLOW	3	// �����݃X�^�b�N�𒴂��ď㏑�����ꂽ
#define	ERROR_NO_STACK_MEMORY			4	// �^�X�N�̂��߂̃�����������Ȃ�
#define	ERROR_NO_TASK_EXIST				5	// �S�Ẵ^�X�N���I������
#define	ERROR_TERM_EXPIRED				6	// �L����������������
#define	ERROR_INSIDE_INTERRUPT			7	// ���荞�ݒ��ɌĂ΂�Ă͂����Ȃ�
#define	ERROR_OUTSIDE_INTERRUPT			8	// ���荞�ݒ��ɌĂ΂��K�v������

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
#define	INVALID_TASK_ID		INT_MAX		// �����ȃ^�X�NID
#define	SYSTEM_TASK_ID		-1			// �V�X�e���^�X�NID

////////////////////////////////////////
// �I�u�W�F�N�g�n���h��
typedef unsigned long HANDLE;
#define	INVALID_HANDLE	0xffffffff		// �����n���h��

////////////////////////////////////////////////////////////////////////////////
// �����I�u�W�F�N�g�v���~�e�B�u
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
// �C�x���g�v���~�e�B�u
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
// �Z�}�t�H�v���~�e�B�u
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
// �~���[�e�b�N�X�v���~�e�B�u
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
// �^�X�N�X�P�W���[�����OFIFO
//
typedef struct tagPRIO_INFO {
	int	task;			// �^�X�N�ԍ�
	int	priority;		// �D�揇��
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
	PRIO_INFO	m_fifo[MAX_TASKS];	// �^�X�N�Ǘ�FIFO
	int			m_read;
	int			m_write;
	int			m_cnt;

public:
	// �L���[�̏I�[�ɒǉ�
	void AppendTail(int task, int priority)
	{
		PRIO_INFO info = {task, priority};
		AppendTail(info);
	}

	// �L���[�̏I�[�ɒǉ�
	void AppendTail(PRIO_INFO info)
	{
		m_fifo[m_write++] = info;
		if (m_write >= MAX_TASKS)
			m_write = 0;
		m_cnt++;
	}

	// �v���C�I���e�B�t���ǉ�
	void AppendPriority(int task, int priority)
	{
		PRIO_INFO info = {task, priority};
		AppendPriority(info);
	}

	// �v���C�I���e�B�t���ǉ�
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

	// �L���[�̐擪����擾
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

	// �^�X�NID�w��̃L���[����̍폜
	void DeleteTask(int task)
	{
		int size = m_cnt;
		for (int i = 0; i < size; i++) {
			PRIO_INFO info = GetHead();
			if (info.task != task)
				AppendTail(info);
		}
	}

	// �L���[�T�C�Y
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
// �^�X�N�ύX�m�F�f�o�b�O�L���[
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
// �^�X�N�֐��̌^
typedef	int (*TASK_FUNC)(void* pVoid);

////////////////////////////////////////
// �^�X�N�I���֐��̌^
typedef void (*TASK_DESTROY)(void);

////////////////////////////////////////
// �^�X�N��ԃt���O
//
typedef struct tagTASK_FLAG {
	unsigned char	nState		: 2;	// �^�X�N���s�X�e�[�^�X
	unsigned char	nPriority	: 6;	// �^�X�N�v���C�I���e�B
} TASK_FLAG;

// �^�X�N�X�e�[�g
enum	{	TS_NO_CREATE,	// �^�X�N�����O
			TS_SUSPEND,		// ��~���
			TS_RUN,			// ���s���
			TS_WAIT			// �҂����
		};

////////////////////////////////////////
// �^�X�N�N���X
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
	// �^�X�N�̐���
	//
	bool CreateTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy);

protected:
	unsigned long	m_dwReg[REG_NUM + 1];	// �ޔ����W�X�^
	char*			m_pStackFrame;			// �X�^�b�N�t���[���|�C���^
	unsigned int	m_nStackSize;			// �X�^�b�N�o�C�g�T�C�Y
	unsigned int	m_nLockMs;				// �X���[�v�A�C�x���g�Ȃǂ̌��Z�J�E���^
	TASK_FLAG		m_TaskFlag;				// �^�X�N�t���O

	const HANDLE*	m_phSyncObjects;		// �����I�u�W�F�N�g�z��|�C���^
	int				m_nSyncObjectCount;		// �҂������I�u�W�F�N�g��
	bool			m_bSyncObjectWaitAll;	// �S�Ă̓����҂����̃t���O
	int				m_nSignalSyncObjectIdx;	// �S�ĂłȂ��ꍇ�̃V�O�i���I�u�W�F�N�g�C���f�b�N�X�i�߂�j
	bool			m_bValid;

	// �^�X�N�ɐ؂�ւ�������R
	enum {	SR_NOP,
			SR_SWITCH,		// �ʏ�^�X�N�ؑ�
			SR_RUN,			// �^�X�N���s
			SR_SUSPEND,		// �^�X�N��~
			SR_BLOCKOUT,	// ��������
			SR_TIMEOUT		// �����^�C���A�E�g
		};
	int	m_nSwitchReson;

private:
	// �^�X�N�ؑ֎��̗��R�R�[�h
	__inline void SetSwitchReson(int sr)				{m_nSwitchReson = sr;}
	__inline int GetSwitchReson()						{return m_nSwitchReson;}

	// �^�X�N�̃X�^�b�N�t���[���擪�|�C���^
	__inline char* GetStackFrame()						{return m_pStackFrame;}

	// �^�X�N�̃��W�X�^�ۑ��z��
	__inline void* GetTaskReg()							{return m_dwReg;}

	// �^�X�N�̎��s���
	__inline void SetTaskRun(bool bRun)					{m_TaskFlag.nState = bRun ? TS_RUN : TS_SUSPEND;}
	__inline void SetTaskWait(bool bWait)				{m_TaskFlag.nState = bWait ? TS_WAIT : TS_RUN;}
	__inline bool IsTaskRun()							{return (m_TaskFlag.nState == TS_RUN);}
	__inline bool IsTaskWait()							{return (m_TaskFlag.nState == TS_WAIT);}

	// �^�X�N�v���C�I���e�B
	__inline int GetPriority()							{return m_TaskFlag.nPriority;}
	__inline void SetPriority(int nPriority)			{m_TaskFlag.nPriority = nPriority;}

	// �������b�N�^�C���A�E�g
	__inline void SetLockMs(unsigned int ms)			{m_nLockMs = ms;}
	__inline unsigned int GetLockMs()					{return m_nLockMs;}

	// �����I�u�W�F�N�g
	__inline void SetSyncObjectHandlePt(const HANDLE* phSyncObjects)	{m_phSyncObjects = phSyncObjects;}
	__inline const HANDLE* GetSyncObjectHandlePt()		{return m_phSyncObjects;}
	__inline int GetSyncObjectCount()					{return m_nSyncObjectCount;}
	__inline bool IsSyncObjectWaitAll()					{return m_bSyncObjectWaitAll;}
	__inline void SetSyncObjectIndex(int idx)			{m_nSignalSyncObjectIdx = idx;}

	// �E�F�C�g�I�u�W�F�N�g�n���h���z����Z�b�g����
	__inline void SetSyncObjectHandles(int nCount, const HANDLE* phSyncObjects, bool bWaitAll)
	{
		m_nSyncObjectCount   = nCount;
		m_phSyncObjects      = phSyncObjects;
		m_bSyncObjectWaitAll = bWaitAll;
	}

	// �������b�N���Ԃ̌��Z�i�C���^�[�o������Ăяo�����j
	__inline void DecrementLockMs()
	{
		if (m_nLockMs != INFINITE) {
			if (m_nLockMs > 0)
				m_nLockMs--;
		}
	}

	// �g�p���̃X�^�b�N�T�C�Y��Ԃ�
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
// �C���^�[�o���R�[���o�b�N�Ǘ�
//
typedef	bool (*ITVL_CALLBACK_FUNC)(void* pVoid);
typedef struct tagINTERVAL_CALLBACK {
	ITVL_CALLBACK_FUNC	pFunc;		// �R�[���o�b�N�֐�	bool foo(void* pVoid)�̌`��
	void*				pVoid;		// �R�[���o�b�N�֐��ɓn���p�����[�^
	unsigned int		timer;		// ms�����l
	unsigned int		counter;	// ms�̃J�E���^
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
	// �R�[���o�b�N�֐��̓o�^
	//
	//	����	ms		�R�[���o�b�N����
	//			pFunc	�R�[���o�b�N�֐��|�C���^	bool foo(void* pVoid)�̌`��
	//			pVoid	�R�[���o�b�N�֐��̈���
	//
	//	�ߒl	>=0		�R�[���o�b�NID
	//			-1		�G���[
	//
	int CreateCallback(unsigned int ms, ITVL_CALLBACK_FUNC pFunc, void* pVoid);

	// �R�[���o�b�N�֐��̍폜
	//
	//	����	id		�R�[���o�b�NID
	//
	void DeleteCallback(int id);

	// �R�[���o�b�N����
	//	�C���^�[�o���^�C�}����Ă΂��
	//
	void ScanAndCall();
};

////////////////////////////////////////////////////////////////////////////////
// CTaskCtrl

////////////////////////////////////////
// �^�X�N����N���X
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
	int		m_nCpuClockFreq;	// CPU�N���b�N
	CTask	m_Task[MAX_TASKS];	// �^�X�N�Ǘ��z��
	int		m_nTaskID;			// ���ݎ��s���̃^�X�N�ԍ�
	bool	m_bInitialized;		// �������ς݃t���O

	CTask	m_sysTask;			// �V�X�e���^�X�N

	// �V�X�e���R�[���V�[�P���X
	enum	{	SYS_NOP,		// �������Ȃ�
				SYS_CREATE,		// �^�X�N����
				SYS_RUN,		// �^�X�N���s
				SYS_SUSPEND,	// �^�X�N��~
				SYS_LOCK,		// �^�X�N�����҂�
				SYS_DESTROY		// �^�X�N�I��
			};
	int		m_seqSystem;

	// �V�X�e���R�[���p�����[�^
	int				m_nOrderTask;
	TASK_FUNC		m_pFunc;
	int				m_nPriority;
	unsigned int	m_nStackSize;
	void*			m_pVoid;
	TASK_DESTROY	m_pDestroy;

	// �C���^�[�o���N���X�|�C���^
	CIntervalTimer m_IntervalTimer;

	// �����I�u�W�F�N�g�z��
#ifdef	STATIC_SYSTEM_OBJECT
	CSyncObjectCore m_SyncObject[MAX_SYNC_OBJECTS];
#else
	CSyncObjectCore* m_pSyncObject[MAX_SYNC_OBJECTS];
#endif

	HANDLE			m_hCritical;	// �N���e�B�J���Z�N�V�����n���h��

	// �^�X�N�X�P�W���[��FIFO
	CScheduleFifo	m_schedulefifo;

	// �`�b�N�J�E���g
	unsigned long	m_dwTickCount;

private:
	// �^�X�NID����^�X�N�N���X�|�C���^�𓾂�
	//
	CTask* GetTask(int task);

	// �L���ȃ^�X�NID����Ԃ�
	//
	bool IsValidTask(int task);

	// ���ݎ��s���̃^�X�N�N���X�|�C���^��Ԃ�
	CTask* GetCurrentTask();

public:
	// �����Җ�
	static const char* GetProductString();

	// ���쌠
	static const char* GetCopyrightString();

	// �o�[�W����
	static const char* GetVersionString();

	// �G���[�ԍ��̎擾
	static int GetLastError();

	// �^�X�NID�̎擾
	int GetLastTaskID();

	// �f�o�b�O���[�h��L���ɂ���
	static void EnableDebugMode(bool bEnable);

	// �^�X�N�ؑփL���[�̃|�C���^�𓾂�
	static const int* GetTaskChangeQueue();

	// �^�X�N�ؑփL���[�T�C�Y�𓾂�
	static int GetTaskChangeQueueSize();

	// �ő�^�X�N��
	static int GetMaxTasks();

	// �ő哯���I�u�W�F�N�g��
	static int GetMaxSyncObjects();

	// �ő�C���^�[�o���R�[���o�b�N��
	static int GetMaxIntervalCallbacks();

	// �N���b�N���g��
	int GetClockFrequency();

	// �V�X�e���^�X�N�̃X�^�b�N�T�C�Y
	static short GetSystemStackSize();

	// CPU���̂�Ԃ�
	static const char* GetCpuName();

	// �g�p���̃X�^�b�N�T�C�Y��Ԃ�
	unsigned int GetUsingStackSize(int task);

public:
	// ������
	bool Initialize(int nFreq);

	// �������ς݂��Ԃ�
	bool IsInitialized();

	// �^�X�N�̐���
	//
	int CreateTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy);
	int CreateTask_OnNoTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy);

	// �^�X�N�̔j��
	//	m_nTaskID��task�ł������ꍇ�A�����ƂȂ�̂Œ��ӂ���
	//
	void DeleteTCB(int task);

	// �^�X�N���s
	void RunTask(int task, bool bRun = true);

	// �^�X�N�̔j��
	void DestroyCurrentTask();

	// �X���[�v
	void Sleep(unsigned int ms);

	// �ʕb�҂�
	void WaitUs(int us);

	// �w��^�X�N�̗D�揇��
	void ChangeTaskPriority(int task, int priority);

	// �`�b�N�J�E���g
	unsigned long GetTickCount();

	// �R�[���o�b�N�֐��̓o�^
	//
	//	����	ms		�R�[���o�b�N����
	//			pFunc	�R�[���o�b�N�֐��|�C���^	bool foo(void* pVoid)�̌`��
	//			pVoid	�R�[���o�b�N�֐��̈���
	//
	//	�ߒl	>=0		�R�[���o�b�NID
	//			-1		�G���[
	//
	int CreateIntervalCallback(int ms, ITVL_CALLBACK_FUNC pFunc, void* pVoid);

	// �R�[���o�b�N�֐��̍폜
	//
	//	����	id		�R�[���o�b�NID
	//
	void DeleteIntervalCallback(int id);

	// �P�ꓯ���҂�
	int WaitForSingleObject(HANDLE hSyncObject, unsigned int ms);

	// ���������҂�
	int WaitForMultipleObjects(int nCount, const HANDLE* phSyncObjects, bool bWaitAll, unsigned int ms);

	// �����I�u�W�F�N�g�̍폜
	void DeleteSyncObject(HANDLE hSyncObject);

	// �C�x���g�֐�
	HANDLE CreateEvent(bool bManualReset = false, bool bInitialState = false);
	bool SetEvent(HANDLE hEvent);
	bool PulseEvent(HANDLE hEvent);
	bool ResetEvent(HANDLE hEvent);

	// �Z�}�t�H�֐�
	HANDLE CreateSemaphore(int nInitialCount = 1, int nMaximumCount = 1);
	bool ReleaseSemaphore(HANDLE hSemaphore);

	// �~���[�e�b�N�X�֐�
	HANDLE CreateMutex();

	// �N���e�C�J���Z�N�V�����֐�
	void EnterCriticalSection();
	void LeaveCriticalSection();

protected:
	// �����n���h�����瓯���v���~�e�B�u�I�u�W�F�N�g�𓾂�
	CSyncObjectCore* _FromHandle(HANDLE hSyncObjet);

	// �V�X�e���^�X�N�֐����N���X�֐��Ƃ��Ď��s���邽��
	//		�L�b�N���邽�߂̐ÓI�֐�
	static int _SysTaskKick(void* pVoid);

	// �V�X�e���^�X�N�֐�
	int SysTaskFunc();

	//
	void InitializeClock();

	// �C���^�[�o���^�C�}�̏�����
	void InitializeInterval();

	// �E�H�b�`�h�b�O�^�C�}�̏�����
	void InitializeWatchDogTimer();

	// �E�H�b�`�h�b�O�^�C�}�̃��t���b�V��
	void RefreshWatchDogTimer();

	// �C���^�[�o���R�[���o�b�N�̎���
	void IntervalTimerScan();

	// �����҂���ms�^�C�}�̃f�N�������g
	void DecrementWaitMs();

	// �X�P�W���[�����ꂽ�^�X�N��Ԃ�
	bool GetScheduledTask(int* pnTask);

	// �����҂��^�X�N�̉�������
	bool CheckWaitTask(int nTaskID);

	// �K�v�Ȃ�A�������Z�b�g�C�x���g�����Z�b�g����
	bool ResetAutoEvent(int nTaskID);

public:
	// ���߂�TaskSwitcher�����s���ꂽ�Ƃ��̕����֖߂�
	__inline static void RestorSystem();

	__inline static void SwitchTask();

	__inline static void ChangeSupervisor();

	__inline static void ChangeUser();

	__inline static void EnterSystemCritical();
	__inline static void LeaveSystemCritical();

	__inline static void DisableInterrupt();
	__inline static void EnableInterrupt();


protected:
	// �^�X�N���W�X�^��؂�ւ���i�X�^�b�N�̊֌W�ŁATaskSwitcher()�ɂ܂Ƃ߂��Ȃ��j
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
// �X���b�h�v���~�e�B�u�֐�
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
// �����I�u�W�F�N�g�̊��N���X
//	�P�Ɛ����͕s�i�h�����j
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

	// �g�p or �҂�
	bool Lock(unsigned int ms);

	// �g�p����
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

	// �g�p or �҂�
	bool Lock(unsigned int ms);

	// �g�p����
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

	// �̈�ɓ���
	bool Lock();

	// �̈悩��o��
	bool Unlock();
};

////////////////////////////////////////////////////////////////////////////////
#endif	//	_FORTE16_H_
