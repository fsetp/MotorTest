////////////////////////////////////////////////////////////////////////////////
//
//	2007.11.25	�X�^�b�N�`�F�b�N�ǉ�
//				H8S/2000�n�Ƃ��Ĉ���
//	2007.12.01	���荞�ݒ��t���O��ǉ�
//				RestoreSystem�֐��ǉ�
//	2007.12.16	WaitUs()�ǉ�
//	2007.12.22	SwitchTask()���X�^�b�N������邽�߁A����������
//	2007.12.24	SwitchTask()��ER0��ۑ����Ă��Ȃ�����
//	2008.04.29	g_nPrevTask�ǉ�
//	2008.04.29	�����݃��[�h�ɂ���āA�����݃X�^�b�N�̉��߂��s���l�ɂ���
//				g_nIntMode�ǉ�
//	2008.05.17	g_nPrevTask�폜�iFIFO�����Ŏg��Ȃ��j
//	2008.07.06	�X�^�b�N�`�F�b�N�̕��@��ύX
//				�ϐ���forte�g�p���̕ϐ��ƏՓ˂��Ȃ��悤�ɁA_��t������
//

#include "fortedef.h"
#include "forte32.h"
#include "clock_ctrl.h"
#include "cmt_ctrl.h"

int				g_nNextTask = INVALID_TASK_ID;	// �ؑ֐�^�X�N�ԍ�
void*			g_pTaskReg  = 0;				// ���߂Ă̋N������0�ɂȂ��Ă���K�v������
unsigned long	g_org_stack = 0;				// ��^�X�N�̃X�^�b�N�|�C���^�ۑ���
int				g_nWaitUsLoop = 0;				//

extern	CTaskCtrl	g_TaskCtrl;					// OS�I�u�W�F�N�g

//#define	SIMULATOR_USE	true
#define	SIMULATOR_USE	false

////////////////////////////////////////
// �^�X�N�̐���
//		��^�X�N���邢�̓V�X�e���^�X�N����Ă΂��
//
//	����	pFunc		�^�X�N�֐��|�C���^
//			nPriority	�^�X�N�v���C�I���e�B	THREAD_PRIORITY_*
//			nStackSize	�^�X�N�ŗL�X�^�b�N�T�C�Y
//			pVoid		�^�X�N�֐��ɓn���p�����[�^
//			pDestroy	�^�X�N�I�����ɌĂяo�����֐��|�C���^
//
//	�ߒl	true		����
//			false		���s�i�q�[�v������������Ȃ��j
//
bool CTask::CreateTask(TASK_FUNC pFunc, int nPriority, unsigned int nStackSize, void* pVoid, TASK_DESTROY pDestroy)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	// TCB�̐���
	m_TaskFlag.nPriority = nPriority;
	m_TaskFlag.nState    = TS_SUSPEND;
	m_nLockMs            = 0;
	m_pStackFrame        = new char[nStackSize];
	m_nStackSize         = nStackSize;

	// �X�^�b�N�̊m�ۂ��ł��Ȃ�����
	if (m_pStackFrame == NULL) {
		g_nLastError = ERROR_NO_STACK_MEMORY;
		return false;
	}

	// �X�^�b�N�`�F�b�N�V�O�l�`���Z�b�g
	FILL_STACK_SIGN(m_pStackFrame, m_nStackSize);

	// �X�^�b�N�t���[���̏�����
	//	�^�X�N�֐��I�����̏����A�h���X���X�^�b�N�ɐς�ł���
	//
	//		-8	PC
	//		-4	PSW
	//
	unsigned long* pStack;
	pStack  = (unsigned long*)(&m_pStackFrame[nStackSize - 4]);
	*pStack = 0x00130000;
	pStack  = (unsigned long*)(&m_pStackFrame[nStackSize - 8]);
	*pStack = (unsigned long)pDestroy;

	// ���W�X�^�̏����l
	//	ER0�̓^�X�N�ɓn���p�����[�^
	//	ER1�`ER6�܂�0��
	//	ER7�̓X�^�b�N�|�C���^
	//
	m_dwReg[0]  = (unsigned long)pStack;	// SP
	m_dwReg[1]  = (unsigned long)pVoid;	// �^�X�N�֐������Ƀp�����[�^��ݒ�
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
	// ���荞�ݒ��́A�G���[�Ƃ���
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
// �C���^�[�o���^�C�}�̏�����
//		CMT���g�p
//
void CTaskCtrl::InitializeInterval()
{
	// ���荞�ݒ��́A�G���[�Ƃ���
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
// �E�H�b�`�h�b�O�^�C�}�̏�����
//		IWDTT���g�p
//
void CTaskCtrl::InitializeWatchDogTimer()
{
//	WDT.WRITE.WINA = 0xa522;
}

////////////////////////////////////////
// �E�H�b�`�h�b�O�^�C�}�̃��t���b�V��
//
void CTaskCtrl::RefreshWatchDogTimer()
{
	// ���t���b�V��
	//
	IWDT.IWDTRR = 0;
	IWDT.IWDTRR = 0xff;
}

////////////////////////////////////////
// ��S�̑҂�
//		�����݋֎~�ɂ��āA�T�C�N�����������
//
//	����	us	�ʕb
//
#define	PLL_DIV			2
#define	PLL_MUL			8
#define	ICLK_DIV		2
#define	WU_FIXED_STATE	120		// 100 + 6(int) + 6(rte) + 5 + 1
#define	WU_LOOP_STATE	10		// 10
void CTaskCtrl::WaitUs(int us)
{
	// ���荞�ݒ��́A�G���[�Ƃ���
	//
	OUTSIDE_INTERRUPT_CHECK();

	//	�Œ�X�e�[�g	���O�v�Z�ƌďo�A�߂�X�e�[�g
	//	���[�v�X�e�[�g	
	//

	
	// �V�X�e���N���b�N
	//	�O���N���b�N��PLL�N���b�N��ICLK
	//
	int nIclk = m_nCpuClockFreq / PLL_DIV * PLL_MUL / ICLK_DIV;

	// �X�e�[�g��
	//	�Œ�X�e�[�g + ���[�v�X�e�[�g x n = �g�[�^���X�e�[�g
	//
	int nTotalState = us * 1000 / nIclk;

	// ���[�v��
	//
	int nLoopTimes = (nTotalState - WU_FIXED_STATE) / WU_LOOP_STATE;

	g_nWaitUsLoop = nLoopTimes;

	int_exception(VECT_WAIT_US);
}

////////////////////////////////////////
// ���߂�SwitchTask�����s���ꂽ�Ƃ��̕����֖߂�
//
void CTaskCtrl::RestorSystem()
{
	int_exception(VECT_RESTORE_SYSTEM);
}

////////////////////////////////////////
//
void CTaskCtrl::SwitchTask()
{
	int_exception(VECT_TASK_SWITCH);	// �^�X�N�ؑ�
}

void CTaskCtrl::ChangeSupervisor()
{
	int_exception(VECT_SUPERVISOR);		// �X�[�p�[�o�C�U�[�֐ؑ�
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
// �^�X�N���W�X�^��؂�ւ���
//		��ɂ���SwitchTask()����Ă΂��
//		�ؑւ����^�X�N�̃��W�X�^�z��A�h���X���擾����
//
void CTaskCtrl::SetupTaskParam()
{
	// ���荞�ݒ��łȂ���΁A�G���[�Ƃ���
	//
	INSIDE_INTERRUPT_CHECK();

	// �X�^�b�N�t���[���`�F�b�N
	CTask* pTask = g_TaskCtrl.GetCurrentTask();
	if (pTask)
		CHECK_STACK_SIGN(pTask->m_pStackFrame, g_TaskCtrl.m_nTaskID, ERROR_TASK_STACK_OVERFLOW);

	// ���[�U�[�^�X�N����̐ؑւȂ�A�V�X�e���^�X�N�֐؂�ւ���
	//	���킹�āA�֑ؑO�̃^�X�NID�̕ۑ�
	if (g_TaskCtrl.m_nTaskID != SYSTEM_TASK_ID)
		g_nNextTask = SYSTEM_TASK_ID;

	// �J�����g�^�X�N�ɐݒ�
	g_TaskCtrl.m_nTaskID = g_nNextTask;

	// ���W�X�^�G���A
	pTask = g_TaskCtrl.GetTask(g_nNextTask);
	g_pTaskReg = pTask->GetTaskReg();
}

////////////////////////////////////////
// �C���^�[�o��������(WDT)
//
void CTaskCtrl::IntervalIntProc()
{
	ENTER_INTERRUPT();

	// SLEEP�ȂǂŎg�p����ms�J�E���^�̌��Z
	//
	g_TaskCtrl.DecrementWaitMs();

	// �C���^�[�o���R�[���o�b�N�T�[�r�X
	//
	g_TaskCtrl.IntervalTimerScan();

	// �`�b�N�J�E���g�̃C���N�������g
	//
	g_TaskCtrl.m_dwTickCount++;

	LEAVE_INTERRUPT();
}
