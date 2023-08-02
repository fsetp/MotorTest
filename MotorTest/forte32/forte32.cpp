////////////////////////////////////////////////////////////////////////////////
// Furicha's Os Realtime Tiny Embedded
//
//	�����̓w�b�_�Q��
//
//

#include <assert.h>
#include "forte32.h"

////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////
// �R���p�C������
//#define	__TERM_OF_VALIDITY__	"Jun 9 2008"

////////////////////////////////////////
// Global Implementation
//
CTaskCtrl				g_TaskCtrl;					// OS�I�u�W�F�N�g
int						g_nLastError  = ERROR_NONE;	// �ŏI�̃G���[�ԍ�
int						g_nErrorTask  = -1;			//
bool					g_bInterrupt = false;		// ���荞�ݒ��t���O
extern int				g_nNextTask;				// �ؑ֐�^�X�N�ԍ�
extern void*			g_pTaskReg;					// ���߂Ă̋N������0�ɂȂ��Ă���K�v������
static bool				g_bDebugMode  = false;		// �f�o�b�O���[�h
static CTaskChgQueue	g_TaskChgQueue;				// �^�X�N�ؑ֏��L���[
////////////////////////////////////////
// HANDLE�̎��
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
// ���^�X�N�̏I������
//
void _DestroyCurrentTask()
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	g_TaskCtrl.DestroyCurrentTask();
}

////////////////////////////////////////
// �V�X�e���^�X�N�̏I������
//
void _DestroySystemTask()
{
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	m_pStackFrame        = NULL;					// �X�^�b�N�t���[���|�C���^
	m_nLockMs            = 0;						// �҂�ms
	m_TaskFlag.nState    = TS_NO_CREATE;			// �f�t�H���g��~
	m_TaskFlag.nPriority = THREAD_PRIORITY_NORMAL;	// �f�t�H���g�v���C�I���e�B

	m_nSyncObjectCount     = 0;						// �����҂��I�u�W�F�N�g��
	m_phSyncObjects        = NULL;					// �����҂��I�u�W�F�N�g�z��|�C���^
	m_bSyncObjectWaitAll   = false;					// �����t���O
	m_nSignalSyncObjectIdx = -1;					// �����C���f�b�N�X

	// �ޔ����W�X�^�N���A
	//
	for (int i = 0; i < REG_NUM; i++)
		m_dwReg[i] = 0;

	m_bValid = false;
}

////////////////////////////////////////
//
CTask::~CTask()
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	// �X�^�b�N�t���[������������폜
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
	// �R�[���o�b�N�\���̃|�C���^�z��̏�����
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
// �R�[���o�b�N�֐��̓o�^
//
int CIntervalTimer::CreateCallback(unsigned int ms, ITVL_CALLBACK_FUNC pFunc, void* pVoid)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	int nRet = -1;

	// �����݋֎~
	CTaskCtrl::EnterSystemCritical();

	// �󂢂Ă���G���g����T���Đ���
	//
	for (int i = 0; i < MAX_INTERVAL_CALLBACK; i++) {

		if (!m_IntervalCalllback[i].bValid) {
			// �p�����[�^�ݒ�
			m_IntervalCalllback[i].pFunc   = pFunc;
			m_IntervalCalllback[i].pVoid   = pVoid;
			m_IntervalCalllback[i].timer   = ms;
			m_IntervalCalllback[i].counter = ms;
			m_IntervalCalllback[i].bValid    = true;

			nRet = i;	// �o�^ID��Ԃ�
			break;
		}
	}

	// �����݋���
	CTaskCtrl::LeaveSystemCritical();
	return nRet;
}

////////////////////////////////////////
// �R�[���o�b�N�֐��̍폜
//
void CIntervalTimer::DeleteCallback(int id)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	// �����݋֎~
	CTaskCtrl::EnterSystemCritical();

	// �L����ID�Ȃ�A�폜
	if (id >= 0 && id < MAX_INTERVAL_CALLBACK) {

		if (m_IntervalCalllback[id].bValid)
			m_IntervalCalllback[id].bValid = false;
	}

	// �����݋���
	CTaskCtrl::LeaveSystemCritical();
}

////////////////////////////////////////
// �R�[���o�b�N����
//		�C���^�[�o�����荞�݂���Ă΂��
//
void CIntervalTimer::ScanAndCall()
{
	// ���荞�ݒ��łȂ���΁A�G���[�Ƃ���
	//
	INSIDE_INTERRUPT_CHECK();

	// �L���ȃR�[���o�b�N�ɂ���
	for (int i = 0; i < MAX_INTERVAL_CALLBACK; i++) {

		if (m_IntervalCalllback[i].bValid) {

			// �J�E���^���^�C���A�b�v������
			if (--m_IntervalCalllback[i].counter == 0) {

				// �J�E���^�������l��
				m_IntervalCalllback[i].counter = m_IntervalCalllback[i].timer;

				// false��Ԃ�����A�R�[���o�b�N���폜
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
	// �^�X�N�Ǘ��z��̏�����
	//
	for (int i = 0; i < MAX_TASKS; i++)
		m_Task[i].m_bValid = false;

	// �G���[�R�[�h�����l
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
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	// �L���ȃ^�X�N���폜
	for (int i = 0; i < MAX_TASKS; i++)
		m_Task[i].m_bValid = false;

	// �����҂��I�u�W�F�N�g�̌㏈��
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

// �f�o�b�O���[�h��L���ɂ���
void CTaskCtrl::EnableDebugMode(bool bEnable)
{
	g_bDebugMode = bEnable;
}

////////////////////////////////////////
// �^�X�N�ؑփL���[�̃|�C���^�𓾂�
const int* CTaskCtrl::GetTaskChangeQueue()
{
	if (g_bDebugMode)
		return g_TaskChgQueue.getQueue();

	return NULL;
}

////////////////////////////////////////
// �^�X�N�ؑփL���[�T�C�Y�𓾂�
int CTaskCtrl::GetTaskChangeQueueSize()
{
	if (g_bDebugMode)
		return g_TaskChgQueue.getSize();

	return 0;
}

////////////////////////////////////////
// �G���[�ԍ��̎擾
//
int CTaskCtrl::GetLastError()
{
	return g_nLastError;
}

////////////////////////////////////////
// �^�X�NID�̎擾
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
// �g�p���̃X�^�b�N�T�C�Y��Ԃ�
unsigned int CTaskCtrl::GetUsingStackSize(int task)
{
	CTask* pTask = GetTask(task);

	// �L���ȃ^�X�N
	if (pTask != NULL)
		return pTask->GetUsingStackSize();

	return 0;
}

////////////////////////////////////////
//
CTask* CTaskCtrl::GetTask(int task)
{
	// �V�X�e���^�X�NID�Ȃ�A�V�X�e���^�X�N
	if (task == SYSTEM_TASK_ID)
		return &m_sysTask;

	// �L���ȃ^�X�NID�Ȃ�
	else if (task < MAX_TASKS)
		return &m_Task[task];

	// �����ȃ^�X�NID�Ȃ�
	else
		return NULL;
}

////////////////////////////////////////
//
bool CTaskCtrl::IsValidTask(int task)
{
	// �V�X�e���^�X�NID���L���^�X�NID�Ȃ�
	if (task == SYSTEM_TASK_ID || (task < MAX_TASKS && m_Task[task].m_bValid))
		return true;

	return false;
}

////////////////////////////////////////
//
CTask* CTaskCtrl::GetCurrentTask()
{
	// �L���ȃ^�X�NID�Ȃ�
	if (IsValidTask(m_nTaskID))
		return GetTask(m_nTaskID);

	return NULL;
}

////////////////////////////////////////
//
void CTaskCtrl::DeleteTCB(int task)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	return m_IntervalTimer.CreateCallback(ms, pFunc, pVoid);
}

////////////////////////////////////////
//
void CTaskCtrl::DeleteIntervalCallback(int id)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
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
// �V�X�e���^�X�N�֐����N���X�֐��Ƃ��Ď��s���邽��
//		�L�b�N���邽�߂̐ÓI�֐�
//
int CTaskCtrl::_SysTaskKick(void* pVoid)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	CTaskCtrl* pTaskCtrl = (CTaskCtrl*)pVoid;
	return pTaskCtrl->SysTaskFunc();
}

////////////////////////////////////////
// �C���^�[�o���R�[���o�b�N�T�[�r�X
//		�C���^�[�o�����荞�݂���Ă΂��
//
void CTaskCtrl::IntervalTimerScan()
{
	// ���荞�ݒ��łȂ���΁A�G���[�Ƃ���
	//
	INSIDE_INTERRUPT_CHECK();

	m_IntervalTimer.ScanAndCall();
}

////////////////////////////////////////
// �^�X�N���b�N�J�E���g�̌��Z
//		�C���^�[�o�����荞�݂���Ă΂��
//
void CTaskCtrl::DecrementWaitMs()
{
	// ���荞�ݒ��łȂ���΁A�G���[�Ƃ���
	//
	INSIDE_INTERRUPT_CHECK();

	// �^�X�N�����̌J��Ԃ�
	for (int i = 0; i < MAX_TASKS; i++) {

		// �L���ȃ^�X�N
		if (m_Task[i].m_bValid)
			m_Task[i].DecrementLockMs();
	}
}

#ifdef	__TERM_OF_VALIDITY__

#include <string.h>
#include <stdlib.h>

////////////////////////////////////////
// �����E�X����[Year * 365.25] + [Year / 400] - [Year / 100] + [30.59 * (Month - 2)] + Day - 678912
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

	// �����񔲂��o��
	char* pMonth = strtok(date, " ");
	char* pDay   = strtok(NULL, " ");
	char* pYear  = strtok(NULL, " ");

	// ���l�ϊ�
	if (pYear) nYear = atoi(pYear);
	for (int i = 0; i < sizeof (g_pMonth) / sizeof (const char*); i++) {
		if (strcmp(pMonth, g_pMonth[i]) == 0) {
			nMonth = i + 1;
			break;
		}
	}
	if (pDay)  nDay = atoi(pDay);

	// 1���A2���́A�O�N��13���A14���Ƃ���
	if (nMonth <= 2) {
		nMonth += 12;
		nYear--;
	}

	// �����E�X����[Year * 365.25] + [Year / 400] - [Year / 100] + [30.59 * (Month - 2)] + Day - 678912
	unsigned long ulJulianDay = (float)nYear * 365.25 + (float)nYear / 400 - (float)nYear / 100 + 30.59 * ((float)nMonth - 2) + nDay - 678912;

	return ulJulianDay;
}
#endif

////////////////////////////////////////
// �^�X�N�Ǘ��̏�����
//
bool CTaskCtrl::Initialize(int nFreq)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

// �R���p�C���̗L������
#ifdef	__TERM_OF_VALIDITY__

	// �R���p�C�����̃����E�X��
	unsigned long ulCompiled = _getJulianDay(__DATE__);

	// �L�������̃����E�X���i���̓��t�ȍ~�͎g�p�s�j
	unsigned long ulTermOfValidity = _getJulianDay(__TERM_OF_VALIDITY__);

	// �L�������𒴂��Ă�����
	if (ulCompiled > ulTermOfValidity) {
		g_nLastError = ERROR_TERM_EXPIRED;
		abort();
	}
#endif

	// ���ɏ���������Ă���ꍇ�A�������Ȃ�
	if (m_bInitialized)
		return true;

	// CPU�N���b�N�̐ݒ�
	m_nCpuClockFreq = nFreq;

	//
	InitializeClock();

	// �C���^�[�o���̏�����
	InitializeInterval();

	// �V�X�e���^�X�N�̐����Ǝ��s�\��
	if (m_sysTask.CreateTask(CTaskCtrl::_SysTaskKick, THREAD_PRIORITY_NORMAL, SYS_STACK_SIZE, this, _DestroySystemTask))
		m_sysTask.SetTaskRun(true);

	// �������ς݃t���O
	m_bInitialized = true;

	return true;
}

////////////////////////////////////////
// �������ς݂��Ԃ�
bool CTaskCtrl::IsInitialized()
{
	return m_bInitialized;
}

////////////////////////////////////////
// �V�X�e���^�X�N�֐�
//
//		���[�U�[�^�X�N����^�X�N�ؑւ��N����ƁA
//		�V�X�e���^�X�N�Ɉڍs����
//
int CTaskCtrl::SysTaskFunc()
{
	CTask* pTask;
	int nTask;

	while (1) {
		switch(m_seqSystem) {
			// �������Ȃ�
			//
			case SYS_NOP:
				break;

			// �^�X�N�̐���
			//	����	m_nOrderTask	�Ăяo�����^�X�N�ԍ�
			//
			//	�ߒl	m_nOrderTask	�������ꂽ�^�X�N�ԍ�
			//
			case SYS_CREATE:
				// �󂢂Ă���ID�̃^�X�N�𐶐�
				nTask = m_nOrderTask;
				m_nOrderTask = INVALID_TASK_ID;

				for (int i = 0; i < MAX_TASKS; i++) {
					if (!m_Task[i].m_bValid) {

						// �^�X�N�̐����ł���
						if (m_Task[i].CreateTask(m_pFunc, m_nPriority, m_nStackSize, m_pVoid, m_pDestroy)) {
							m_Task[i].m_bValid = true;
							m_schedulefifo.AppendPriority(i, m_nPriority);
							m_nOrderTask = i;
							break;

						} else {
							// ����ID
							m_nOrderTask = INVALID_TASK_ID;
							break;
						}
					}
				}

				// �Ăяo���^�X�N�ɖ߂�
				pTask = GetTask(nTask);
				pTask->SetSwitchReson(CTask::SR_SWITCH);

				g_nNextTask = nTask;
				if (g_bDebugMode)
					g_TaskChgQueue.put(nTask);
				SwitchTask();
				break;

			// �^�X�N�̎��s�ƒ�~
			//	����	m_nOrderTask	���s�A��~����^�X�N
			//
			case SYS_RUN:
			case SYS_SUSPEND:
				pTask = GetTask(m_nOrderTask);
				if (pTask) {
					pTask->SetTaskRun((m_seqSystem == SYS_RUN) ? true : false);
					pTask->SetSwitchReson((m_seqSystem == SYS_RUN) ? CTask::SR_RUN : CTask::SR_SUSPEND);
				}

				// �L���^�X�N�Ȃ�A�^�X�N�ؑ�
				while (!GetScheduledTask(&nTask))
					;

				// �^�X�N�ؑ�
				pTask = GetTask(nTask);
				pTask->SetSwitchReson(CTask::SR_SWITCH);
				g_nNextTask = nTask;
				if (g_bDebugMode)
					g_TaskChgQueue.put(nTask);
				SwitchTask();
				break;

			// �^�X�N�̓����҂�
			//
			case SYS_LOCK:
				// �L���^�X�N�Ȃ�A�^�X�N�ؑ�
				while (!GetScheduledTask(&nTask))
					;

				// �^�X�N�ؑ�
				g_nNextTask = nTask;
				if (g_bDebugMode)
					g_TaskChgQueue.put(nTask);
				SwitchTask();
				break;

			// �^�X�N�j��
			//		m_nOrderTask
			//
			case SYS_DESTROY:
				// TCB��j��
				DeleteTCB(m_nOrderTask);

				// �L���^�X�N�Ȃ�A�^�X�N�ؑ�
				while (!GetScheduledTask(&nTask))
					;

				// �^�X�N�ؑ�
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
// �X���[�v
//
void CTaskCtrl::Sleep(unsigned int ms)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	// �i���̖���łȂ�
	if (ms != INFINITE) {
		// �X���[�v�͉�������Ȃ��C�x���g�Ɠ���
		CEvent event;
		event.Lock(ms);

	// �i���̖���Ȃ�A�^�X�N��~
	} else
		RunTask(m_nTaskID, false);
}

////////////////////////////////////////
// �^�X�N���s
//
void CTaskCtrl::RunTask(int task, bool bRun)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	m_nOrderTask = task;
	m_seqSystem  = bRun ? SYS_RUN : SYS_SUSPEND;
	SwitchTask();
}

////////////////////////////////////////
// �^�X�N�֐����甲���鎞�̏���
//
void CTaskCtrl::DestroyCurrentTask()
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	m_nOrderTask = m_nTaskID;
	m_seqSystem  = SYS_DESTROY;
	SwitchTask();
}

////////////////////////////////////////
// �^�X�N�̐���
//
//	����	pFunc		�^�X�N�֐��|�C���^
//			nPriority	�^�X�N�v���C�I���e�B	THREAD_PRIORITY_*
//			nStackSize	�^�X�N�ŗL�X�^�b�N�T�C�Y
//			pVoid		�^�X�N�֐��ɓn���p�����[�^
//			pDestroy	�^�X�N�I�����ɌĂяo�����֐��|�C���^
//
//	�ߒl	!= INVALID_TASK_ID	�^�X�NID
//			INVALID_TASK_ID		���s
//
int CTaskCtrl::CreateTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	//
	if (nPriority >= THREAD_PRIORITY_MAX_VALUE)
		nPriority = THREAD_PRIORITY_MAX_VALUE;

	// ���݃^�X�NID�������^�X�NID�i�܂��^�X�N�������Ă��Ȃ��j�Ȃ�
	if (m_nTaskID == INVALID_TASK_ID) {

		// ���ڐ���
		return CreateTask_OnNoTask(pFunc, nPriority, nStackSize, pVoid, pDestroy);

	// ���Ƀ^�X�N�������Ă���Ȃ�
	} else {

		// �V�X�e���^�X�N�ɂĐ���
		m_pFunc      = pFunc;
		m_nPriority  = nPriority;
		m_nStackSize = nStackSize;
		m_pVoid      = pVoid;
		m_pDestroy   = pDestroy;
		m_nOrderTask = m_nTaskID;	// ���̃^�X�N�ԍ��ɖ߂��Ă��邽��
		m_seqSystem  = SYS_CREATE;
		SwitchTask();

		return m_nOrderTask;	// �������ꂽ�^�X�NID
	}
}

////////////////////////////////////////
// ��^�X�N�ł̃^�X�N����
//
//	����	pFunc		�^�X�N�֐��|�C���^
//			nPriority	�^�X�N�v���C�I���e�B	THREAD_PRIORITY_*
//			nStackSize	�^�X�N�ŗL�X�^�b�N�T�C�Y
//			pVoid		�^�X�N�֐��ɓn���p�����[�^
//			pDestroy	�^�X�N�I�����ɌĂяo�����֐��|�C���^
//
//	�ߒl	!= INVALID_TASK_ID	�^�X�NID
//			INVALID_TASK_ID		���s
//
int CTaskCtrl::CreateTask_OnNoTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	//
	if (nPriority >= THREAD_PRIORITY_MAX_VALUE)
		nPriority = THREAD_PRIORITY_MAX_VALUE;

	// �󂢂Ă���ID�̃^�X�N�𐶐�
	for (int i = 0; i < MAX_TASKS; i++) {
		if (!m_Task[i].m_bValid) {

			// �^�X�N�̐����ł���
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
// �^�X�N�v���C�I���e�B�̕ύX
//
void CTaskCtrl::ChangeTaskPriority(int task, int priority)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	//
	if (priority >= THREAD_PRIORITY_MAX_VALUE)
		priority = THREAD_PRIORITY_MAX_VALUE;

	CTask* pTask = GetTask(task);

	// �L���ȃ^�X�N
	if (pTask != NULL) {
		// FIFO���בւ�
		m_schedulefifo.DeleteTask(task);
		m_schedulefifo.AppendPriority(task, priority);

		// �^�X�N�̏���ς��Ă���
		pTask->SetPriority(priority);

		// �^�X�N�ؑ�
		m_seqSystem = SYS_LOCK;
		SwitchTask();
	}
}

////////////////////////////////////////
// �`�b�N�J�E���g
unsigned long CTaskCtrl::GetTickCount()
{
	return m_dwTickCount;
}

////////////////////////////////////////
// �X�P�W���[�����ꂽ�^�X�N��Ԃ�
//
bool CTaskCtrl::GetScheduledTask(int* pnTask)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	// �����݋֎~
	EnterSystemCritical();

	// �S�^�X�N�̓����������m�F
	for (int i = 0; i < MAX_TASKS; i++)
		CheckWaitTask(i);

	// �����I�u�W�F�N�g�ɂ��X�P�W���[�����O
	*pnTask = INVALID_TASK_ID;

	// FIFO�ɂ���^�X�N���J��Ԃ�
	int tasks = m_schedulefifo.GetSize();
	for (int i = 0; i < tasks; i++) {

		// FIFO����^�X�N�����o���A�܂��ǉ����Ă���
		PRIO_INFO info = m_schedulefifo.GetHead();
		int nTask = info.task;
		m_schedulefifo.AppendTail(info);
		CTask* pTask = GetTask(nTask);

		// �L���ȃ^�X�N
		if (pTask != NULL) {

			// ���s����?
			if (pTask->IsTaskRun()) {

				// �K�v�Ȃ�A�������Z�b�g�C�x���g�����Z�b�g
				if (pTask->GetSwitchReson() == CTask::SR_BLOCKOUT)
					ResetAutoEvent(nTask);

				// �^�X�NID�m��
				*pnTask = nTask;

				// �����݋���
				LeaveSystemCritical();
				return true;
			}
		}
	}

	// �����݋���
	LeaveSystemCritical();

	// ���s����^�X�N���Ȃ�
	return false;
}

////////////////////////////////////////
// �����҂��^�X�N�̉�������
bool CTaskCtrl::CheckWaitTask(int nTaskID)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	bool bSignal = false;

	// �L���ȃ^�X�N�œ����҂�?
	CTask* pTask = GetTask(nTaskID);
	if (pTask == NULL || !pTask->IsTaskWait())
		return false;

	// ���炩���߃C���f�b�N�X��0�ɂ��Ă���
	pTask->m_nSignalSyncObjectIdx = 0;

	// �^�C���A�E�g����
	if (pTask->GetLockMs() == 0) {

		// �����҂�����
		pTask->SetTaskWait(false);
		pTask->SetSwitchReson(CTask::SR_TIMEOUT);
		return true;

	// �����`�F�b�N
	} else {

		// �����I�u�W�F�N�g�|�C���^
		const HANDLE* phSyncObjects = pTask->GetSyncObjectHandlePt();
		if (phSyncObjects == NULL) {
			pTask->SetSwitchReson(CTask::SR_NOP);
			return false;
		}

		// �����I�u�W�F�N�g���A�X�L��������
		bool bAllSignal = true;
		int nIdxObject  =  -1;
		for (int j = 0; j < pTask->GetSyncObjectCount(); j++) {

			// �L���ȃn���h��
			if (phSyncObjects[j] != INVALID_HANDLE) {
				CSyncObjectCore* pSyncObject = _FromHandle(phSyncObjects[j]);
				assert(pSyncObject);

				// �҂��Ă铯���I�u�W�F�N�g���u���b�N�����ɂȂ��Ă���
				if (pSyncObject->IsSignal()) {

					// �S���҂��łȂ�
					if (!pTask->IsSyncObjectWaitAll()) {

						// �����҂�����
						pTask->SetTaskWait(false);

						// �ŏ��ɉ��������C���f�b�N�X
						if (nIdxObject == -1)
							nIdxObject = j;
					}
					bSignal = true;

				// ��ł��V�O�i���łȂ�
				} else
					bAllSignal = false;
			}
		}

		// �S���҂��łȂ��āA�ŏ��ɉ������ꂽ�C���f�b�N�X���L���Ȃ�
		if (!pTask->IsSyncObjectWaitAll() && bSignal) {
			pTask->m_nSignalSyncObjectIdx = nIdxObject;
			pTask->SetSwitchReson(CTask::SR_BLOCKOUT);
			return true;
		}

		// �S���҂��ŁA�S���V�O�i���ɂȂ���
		if (pTask->IsSyncObjectWaitAll() && bAllSignal) {

			// �����҂�����
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
// �K�v�Ȃ�A�������Z�b�g�C�x���g�����Z�b�g����
bool CTaskCtrl::ResetAutoEvent(int nTaskID)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	// �L���ȃ^�X�N�œ����҂�?
	CTask* pTask = GetTask(nTaskID);
	if (pTask != NULL) {

		// �����I�u�W�F�N�g�|�C���^
		const HANDLE* phSyncObjects = pTask->GetSyncObjectHandlePt();

		// �����I�u�W�F�N�g���J��Ԃ�
		for (int j = 0; j < pTask->GetSyncObjectCount(); j++) {

			// �L���ȃn���h��
			if (phSyncObjects[j] != INVALID_HANDLE) {
				CSyncObjectCore* pSyncObject = _FromHandle(phSyncObjects[j]);
				assert(pSyncObject);

				// �҂��Ă铯���I�u�W�F�N�g���u���b�N�����ɂȂ��Ă���
				if (pSyncObject->IsSignal()) {

					// �C�x���g�n���h����������A�K�v�ɉ����Ď�����������
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
	// �����I�u�W�F�N�g���P��WaitForMultipleObjects()�Ƃ��Ď��s����
	//
	HANDLE handle = hSyncObject;
	return WaitForMultipleObjects(1, &handle, false, ms);
}

////////////////////////////////////////
//
int CTaskCtrl::WaitForMultipleObjects(int nCount, const HANDLE* phSyncObjects, bool bWaitAll, unsigned int ms)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	// �w�肳�ꂽ�����I�u�W�F�N�g�n���h�����L���Ȃ�
	if (phSyncObjects == NULL || nCount <= 0)
		return WAIT_FAILED;

	// �L���ȓ����I�u�W�F�N�g�Ȃ烍�b�N����
	for (int i = 0; i < nCount; i++) {
		HANDLE hSyncObject = phSyncObjects[i];
		if (hSyncObject == INVALID_HANDLE)
			return WAIT_FAILED;

		// �w�肳�ꂽ�����I�u�W�F�N�g�n���h���������Ȃ�
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

		// ���b�N
#ifdef	STATIC_SYSTEM_OBJECT
		m_SyncObject[idx].Lock();
#else
		m_pSyncObject[idx]->Lock();
#endif
	}

	// �҂�
	CTask* pTask = GetCurrentTask();
	if (pTask) {
		pTask->SetLockMs(ms);
		pTask->SetTaskWait(true);
		pTask->SetSyncObjectHandles(nCount, phSyncObjects, bWaitAll);
		m_seqSystem = SYS_LOCK;
		SwitchTask();
		if (pTask->GetSwitchReson() == CTask::SR_TIMEOUT)
			return WAIT_TIMEOUT;

		// �S�đ҂�������
		if (bWaitAll)
			return WAIT_OBJECT_0;

		// �ǂꂩ�ЂƂł��̏ꍇ
		else
			return WAIT_OBJECT_0 + pTask->m_nSignalSyncObjectIdx;
	}

	return WAIT_FAILED;
}

////////////////////////////////////////
//
HANDLE CTaskCtrl::CreateEvent(bool bManualReset, bool bInitialState)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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

		// �񊄂荞�ݎ��́A�^�X�N��؂�ւ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ�
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
	// ���荞�ݒ�
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
	// ���荞�ݒ�
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_hSyncObject == INVALID_HANDLE)
		m_hSyncObject = GetTaskCtrl().CreateMutex();

	return m_hSyncObject;
}

////////////////////////////////////////
// �g�p or �҂�
bool CMutex::Lock(unsigned int ms)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	GetTaskCtrl().EnterCriticalSection();
	return true;
}

////////////////////////////////////////
//
bool CCriticalSection::Unlock()
{
	// ���荞�ݒ��́A�G���[�Ƃ���
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
// �^�X�N�̐���
//	�񊄂荞��
//
bool CThread::CreateThread(int nPriority, unsigned int nStackSize)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	m_nTaskId = GetTaskCtrl().CreateTask(ThreadKick, nPriority, nStackSize, this, _DestroyCurrentTask);
	return (m_nTaskId != INVALID_HANDLE);
}

////////////////////////////////////////
//
bool CThread::SignleLock(CSyncObject* pObject, unsigned int ms)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	GetTaskCtrl().Sleep(ms);
}

////////////////////////////////////////
//
void CThread::WaitUs(int us)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	GetTaskCtrl().WaitUs(us);
}

////////////////////////////////////////
//
unsigned long CThread::GetTickCount()
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	return GetTaskCtrl().GetTickCount();
}

////////////////////////////////////////
//
void CThread::_run(bool bRun)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	if (m_nTaskId != INVALID_TASK_ID)
		GetTaskCtrl().RunTask(m_nTaskId, bRun);
}

////////////////////////////////////////
//
int CThread::ThreadKick(void* pVoid)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	CThread* pThread = (CThread*)pVoid;
	return pThread->ThreadFunc();
}
