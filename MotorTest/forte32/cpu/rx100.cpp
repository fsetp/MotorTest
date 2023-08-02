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
#if	SIMULATOR_USE == false

	SYSTEM.PRCR.WORD = 0xa503;		// PLLCR, PLLCR2 write enable

	// 12MHz / 2 x 8 = 48MHz
	//
	SYSTEM.PLLCR.BIT.PLIDIV = 1;	// division by 2
	SYSTEM.PLLCR.BIT.STC    = 15;	// multiply by 8

	// Clock applications
	//	ICLK	: 32MHz max
	//	PCLKB	: 32MHz max
	//	PCLKD	: 32MHz max
	//	FCLK	: 32MHz max
	//	UCLK	: 48MHz
	//	CACCLK	: same as inpuc clock
	//	RTCSCLK	: 32.768kHz
	//	IWDTCLK	: 15kHz
	SYSTEM.SCKCR.BIT.ICK  = 1;	// 24MHz
	SYSTEM.SCKCR.BIT.FCK  = 1;	// 24MHz
	SYSTEM.SCKCR.BIT.PCKB = 1;	// 24MHz
	SYSTEM.SCKCR.BIT.PCKD = 1;	// 24MHz

	// Mail clock wait
	//
	while (SYSTEM.OSCOVFSR.BIT.MOOVF == 1)
		;
	SYSTEM.MOSCWTCR.BIT.MSTS = 0;	//

	// Enabre and wait
	//
	SYSTEM.PLLCR2.BIT.PLLEN = 0;	// enable PLL
	while (SYSTEM.OSCOVFSR.BIT.PLOVF == 0)
		;

	// Start main clock
	//
	SYSTEM.MOSCCR.BIT.MOSTP = 0;	// start main clock
	while (SYSTEM.OSCOVFSR.BIT.MOOVF == 0)
		;

	// Select main clock
	//
	SYSTEM.SCKCR3.BIT.CKSEL = 4;	// select PLL

	SYSTEM.PRCR.WORD = 0xa500;		//

#endif
}

////////////////////////////////////////
// �C���^�[�o���^�C�}�̏�����
//		CMT���g�p
//
void CTaskCtrl::InitializeInterval()
{
	// Enable CMT0
	//
	SYSTEM.PRCR.WORD = 0xa502;		// MSTPCRA write enable
	MSTP_CMT0 = 0;
	SYSTEM.PRCR.WORD = 0xa500;		//

	//	main clock = osc / 2 x 8
	//	pclk = main clock / 2
	//	cmtclk = pclk / 8
	//	1000 * 1000 / (1000 / cmtclk)
	unsigned int nDefCnt = 1000 * m_nCpuClockFreq / 2 * 8 / 2 / 8;

	// enable interrupt
	//
	IR(CMT0, CMI0) = 0;
	IPR(CMT0, CMI0) = 1;
	IEN(CMT0, CMI0) = 1;

	// start timer
	//
	CMT.CMSTR0.BIT.STR0 = 0;		// ��~
	CMT0.CMCR.BIT.CKS   = 0;		// division by 8
	CMT0.CMCR.BIT.CMIE  = 1;		// enable interrupt
	CMT0.CMCNT = 0;					// clear counter
	CMT0.CMCOR = nDefCnt;
	CMT.CMSTR0.BIT.STR0 = 1;		// �J�n
}

////////////////////////////////////////
// �E�H�b�`�h�b�O�^�C�}�̏�����
//		IWDTT���g�p
//
void CTaskCtrl::InitializeWatchDogTimer()
{
	IWDT.IWDTCR.BIT.TOPS = 1;	// 512 cycle
	IWDT.IWDTCR.BIT.CKS  = 5;	// IWDTCLK/256
	IWDT.IWDTCR.BIT.RPES = 2;	// �I��25%
	IWDT.IWDTCR.BIT.RPSS = 3;	// �J�n100%

//	IWDT.IWDTSR.BIT.CNTVAL
//	IWDT.IWDTSR.BIT.UNDFF
//	IWDT.IWDTSR.BIT.REFEF

	IWDT.IWDTRCR.BIT.RSTIRQS = 0;	// �m���}�X�J�u�����荞�݋���
	IWDT.IWDTCSTPR.BIT.SLCSTP = 0;	// �J�E���g��~����
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

////////////////////////////////////////
// �^�X�N���W�X�^��؂�ւ���
//		��ɂ���SwitchTask()����Ă΂��
//		�֑ؑ׃^�X�N�̃��W�X�^�z��A�h���X���擾����
//
void CTaskCtrl::SetupTaskParam()
{
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

	//
	g_TaskCtrl.m_dwTickCount++;

	LEAVE_INTERRUPT();
}
