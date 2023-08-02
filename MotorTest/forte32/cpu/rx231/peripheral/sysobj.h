////////////////////////////////////////////////////////////////////////////////
// System Objects Control
//
// �T�|�[�g�N���X
//		CCpuObject			CPU���L���Ǘ��I�u�W�F�N�g
//		CModuleStopObject	���W���[���X�g�b�v�R���g���[��
//		CIcuObject			�����݃R���g���[��
//		CDioObject			I/O�|�[�g�R���g���[��
//		CMpcObject			�}���`�t�@���N�V�����s���R���g���[��
//		CMtuObject			�}���`�t�@���N�V�����^�C�}�p���X���j�b�g2�R���g���[��
//		CCmtObject			�R���y�A�}�b�`�^�C�}�R���g���[��
//		CIwdtObject			�Ɨ��E�H�b�`�h�b�O�^�C�}�R���g���[��
//		CUsbObject			USB2.0�z�X�g�^�t�@���N�V�������W���[���R���g���[��
//		CSciObject			�V���A���R�~���j�P�[�V�����C���^�t�F�[�X�R���g���[��
//		CAdcS12Object		12�r�b�gA/D�R���o�[�^�R���g���[��
//

#ifndef _SYS_OBJ_H_
#define _SYS_OBJ_H_

#include <machine.h>
#include "iodefine.h"
//#define	NDEBUG				// ���̒�`���s���ƁA�A�T�[�V�����u���b�N���폜�����
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// CPU���L���Ǘ��I�u�W�F�N�g
//
class CCpuObject
{
public:
	CCpuObject()  {}
	~CCpuObject() {}

public:

	// CPU���[�h�̎擾
	__inline static unsigned short GetMode();

	// ����ROM�̐ݒ�Ǝ擾
	__inline static void EnableInternalRom(bool bEnable);
	__inline static bool IsEnableInternalRom();

	// �O���o�X�̐ݒ�Ǝ擾
	__inline static void EnableExtendBus(bool bEnable);
	__inline static bool IsEnableExtendBus();

	// ����RAM�̐ݒ�Ǝ擾
	__inline static void EnableInternalRam(bool bFlag);
	__inline static bool IsEnableInternalRam();

	// �V�X�e���N���b�N������̐ݒ�Ǝ擾
	enum {
		ICLK_DIV1, ICLK_DIV2, ICLK_DIV4, ICLK_DIV8, ICLK_DIV16, ICLK_DIV32, ICLK_DIV64
	};
	__inline static void SelectSystemClock(unsigned char div);
	__inline static unsigned char GetSystemClock();

	// ���ӃN���b�N������̐ݒ�Ǝ擾
	enum {
		PCLK_DIV1, PCLK_DIV2, PCLK_DIV4, PCLK_DIV8, PCLK_DIV16, PCLK_DIV32, PCLK_DIV64
	};
	__inline static void SelectPeripheralClockA(unsigned char div);
	__inline static unsigned char GetPeripheralClockA();
	__inline static void SelectPeripheralClockB(unsigned char div);
	__inline static unsigned char GetPeripheralClockB();
	__inline static void SelectPeripheralClockD(unsigned char div);
	__inline static unsigned char GetPeripheralClockD();

	// �O���o�X�N���b�N������̐ݒ�Ǝ擾
	enum {
		BCLK_DIV1, BCLK_DIV2, BCLK_DIV4, BCLK_DIV8, BCLK_DIV16, BCLK_DIV32, BCLK_DIV64
	};
	__inline static void SelectBusClock(unsigned char div);
	__inline static unsigned char GetBusClock();

	// FlashIF�N���b�N������ݒ�Ǝ擾
	enum {
		FCLK_DIV1, FCLK_DIV2, FCLK_DIV4, FCLK_DIV8, FCLK_DIV16, FCLK_DIV32, FCLK_DIV64
	};
	__inline static void SelectFlashIFClock(unsigned char div);
	__inline static unsigned char GetFlashIFClock();

	// �O���o�X�N���b�N�o�͒�~�̐ݒ�Ǝ擾
	__inline static void StopBusClock(bool bEnable);
	__inline static bool IsStopBusClock();

	// �N���b�N�\�[�X�̐ݒ�Ǝ擾
	enum {
		CLKSRC_LOCO, CLKSRC_HOCO, CLKSRC_MAINOSC, CLKSRC_SUBOSC, CLKSRC_PLL
	};
	__inline static void SelectClockSource(unsigned char src);
	__inline static unsigned char GetClockSource();

	// PLL���͕�����̐ݒ�Ǝ擾
	enum {
		PLL_DIV1, PLL_DIV2, PLL_DIV4
	};
	__inline static void SelectPllInputDivider(unsigned char div);
	__inline static unsigned char GetPllInputDivider();

	// PLL���g�����{���̐ݒ�Ǝ擾
	enum {
		PLL_x4    = 7,  PLL_x4_5 = 8,  PLL_x5    = 9,  PLL_x5_5  = 10, PLL_x6    = 11,
		PLL_x6_5  = 12, PLL_x7   = 13, PLL_x7_5  = 14, PLL_x8    = 15, PLL_x8_5  = 16,
		PLL_x9    = 17, PLL_x9_5 = 18, PLL_x10   = 19, PLL_x10_5 = 20, PLL_x11   = 21,
		PLL_x11_5 = 22, PLL_x12  = 23, PLL_x12_5 = 24, PLL_x13   = 25, PLL_x13_5 = 26
	};
	__inline static void SelectPllOutputMultiplier(unsigned char multi);
	__inline static unsigned char GetPllOutputMultiplier();

	// PLL��~�̐ݒ�Ǝ擾
	__inline static void StopPll(bool bEnable);
	__inline static bool IsStopPll();

	// USB��pPLL���͕�����̐ݒ�Ǝ擾
	__inline static void SelectUsbPllInputDivider(unsigned char div);
	__inline static unsigned char GetUsbPllInputDivider();

	// USB��pPLL���g�����{���̐ݒ�Ǝ擾
	enum {
		USBPLL_x4 = 7, USBPLL_x6 = 11, USBPLL_x8 = 15, USBPLL_x12 = 23
	};
	__inline static void SelectUsbPllOutputMultiplier(unsigned char multi);
	__inline static unsigned char GetUsbPllOutputMultiplier();

	// UCLK�\�[�XUSB��pPLL�̐ݒ�Ǝ擾
	enum {
		UCLK_SYSCLK, UCLK_USBPLL
	};
	__inline static void SelectUCLKSource(unsigned char sel);
	__inline static unsigned char GetUCLKSource();

	// USB��pPLL��~�̐ݒ�Ǝ擾
	__inline static void StopUsbPll(bool bEnable);
	__inline static bool IsStopUsbPll();

	// BCLK�o�͂̐ݒ�Ǝ擾
	__inline static void SelectBCLKDivider(unsigned char div);
	__inline static unsigned char GetBCLKDivider();

	// ���C���N���b�N���U���~�̐ݒ�Ǝ擾
	__inline static void StopMainClockOscillator(bool bEnable);
	__inline static bool IsStopMainClockOscillator();

	// �T�u�N���b�N���U���~�̐ݒ�Ǝ擾
	__inline static void StopSubClockOscillator(bool bEnable);
	__inline static bool IsStopSubClockOscillator();

	// LOCO��~�̐ݒ�Ǝ擾
	__inline static void StopLOCO(bool bEnable);
	__inline static bool IsStopLOCO();

	// IWDT��p�I���`�b�v�I�V���[�^��~�̐ݒ�Ǝ擾
	__inline static void StopIWDTOscillator(bool bEnable);
	__inline static bool IsStopIWDTOscillator();

	// HOCO��~�̐ݒ�Ǝ擾
	__inline static void StopHOCO(bool bEnable);
	__inline static bool IsStopHOCO();

	// HOCO���g���̐ݒ�Ǝ擾
	enum {
		HOCO_32MHZ = 0, HOCO_54MHZ = 3
	};
	__inline static void SelectHOCOFrequency(unsigned char freq);
	__inline static unsigned char GetHOCOFrequency();

	// ���C���N���b�N���U����̎擾
	__inline static bool IsStabilizeMainClock();

	// PLL�N���b�N���U����̎擾
	__inline static bool IsStabilizePll();

	// HOCO�N���b�N���U����̎擾
	__inline static bool IsStabilizeHOCO();

	// USB��pPLL�N���b�N���U����̎擾
	__inline static bool IsStabilizeUsbPll();

	// ���U��~���o�����݋��̐ݒ�Ǝ擾
	__inline static void EnableStopOscillationInterrupt(bool bEnable);
	__inline static bool IsEnableStopOscillationInterrupt();

	// ���U��~���o�@�\���̐ݒ�Ǝ擾
	__inline static void EnableStopOscillationFunction(bool bEnable);
	__inline static bool IsEnableStopOscillationFunction();

	// ���C���N���b�N���U��~�̎擾
	__inline static bool IsDetectStopMainClock();

	// ���C���N���b�N���U��E�F�C�g���Ԃ̐ݒ�Ǝ擾
	enum {
		CYCLE_2,    CYCLE_1024,  CYCLE_2048,  CYCLE_4096,
		CYCLE_8192, CYCLE_16384, CYCLE_32768, CYCLE_65536
	};
	__inline static void SelectMainClockOscillatorWaitTime(unsigned char cycle);
	__inline static unsigned char GetMainClockOscillatorWaitTime();

	// CLKOUT�o�̓\�[�X�̐ݒ�Ǝ擾
	__inline static void SelectCLKOUTSource(unsigned char src);
	__inline static unsigned char GetCLKOUTSource();

	// CLKOUT�o�͕�����̐ݒ�Ǝ擾
	enum {
		CLKOUT_DIV1, CLKOUT_DIV2, CLKOUT_DIV4, CLKOUT_DIV8, CLKOUT_DIV16
	};
	__inline static void SelectCLKOUTDivider(unsigned char div);
	__inline static unsigned char GetCLKOUTDivider();

	// CLKOUT�o�͒�~�̐ݒ�Ǝ擾
	__inline static void StopCLKOUT(bool bEnable);
	__inline static bool IsStopCLKOUT();

	// ���C���N���b�N���U��̐ݒ�Ǝ擾(�O�����U����)
	__inline static void EnableExternalOscillationInput(bool bEnable);
	__inline static bool IsEnableExternalOscillationInput();

	// �������E�F�C�g�T�C�N�����̐ݒ�Ǝ擾
	__inline static void EnableMemoryWait(bool bEnable);
	__inline static bool IsEnableMemoryWait();

	// �v���e�N�g���W�X�^�������݂̐ݒ�
	__inline static void EnableWriteProtectRegister0(bool bEnable);
	__inline static void EnableWriteProtectRegister1(bool bEnable);
	__inline static void EnableWriteProtectRegister2(bool bEnable);
	__inline static void EnableWriteProtectRegister3(bool bEnable);

};

////////////////////////////////////////////////////////////////////////////////
// ���W���[���X�g�b�v�R���g���[��
//
class CModuleStopObject
{
public:
	CModuleStopObject()  {}
	~CModuleStopObject() {}

public:

	enum {
		// MSTPCRA
		M_NC22 = 0x1F,  M_NC21 = 0x1E,  M_NC20  = 0x1D,  M_DMADT = 0x1C,  M_NC19 = 0x1B,  M_NC18 = 0x1A,  M_NC17  = 0x19,  M_NC16  = 0x18,
		M_NC15 = 0x17,  M_NC14 = 0x16,  M_NC13  = 0x15,  M_NC12  = 0x14,  M_DA   = 0x13,  M_NC11 = 0x12,  M_S12AD = 0x11,  M_NC10  = 0x10,
		M_CMT0 = 0x0F,  M_CMT1 = 0x0E,  M_TPU0  = 0x0D,  M_NC09  = 0x0C,  M_NC08 = 0x0B,  M_NC07 = 0x0A,  M_MTU   = 0x09,  M_NC06  = 0x08,
		M_NC05 = 0x07,  M_NC04 = 0x06,  M_TMR10 = 0x05,  M_TMR32 = 0x04,  M_NC03 = 0x03,  M_NC02 = 0x02,  M_NC01  = 0x01,  M_NC00  = 0x00,

		// MSTPCRB
		M_SCI0 = 0x3F,  M_SCI1 = 0x3E,  M_NC41  = 0x3D,  M_NC40  = 0x3C,  M_NC39 = 0x3B,  M_SCI5 = 0x3A,  M_SCI6  = 0x39,  M_NC38  = 0x38,
		M_CRC  = 0x37,  M_NC37 = 0x36,  M_RIIC0 = 0x35,  M_NC36  = 0x34,  M_USB0 = 0x33,  M_NC35 = 0x32,  M_RSPI0 = 0x31,  M_NC34  = 0x30,
		M_NC33 = 0x2F,  M_NC32 = 0x2E,  M_NC31  = 0x2D,  M_NC30  = 0x2C,  M_NC29 = 0x2B,  M_COMP = 0x2A,  M_ELC   = 0x29,  M_NC28  = 0x28,
		M_NC27 = 0x27,  M_DOC  = 0x26,  M_NC26  = 0x25,  M_SCI12 = 0x24,  M_NC25 = 0x23,  M_NC24 = 0x22,  M_NC23  = 0x21,  M_RCAN0 = 0x20,

		// MSTPCRC
		M_DSLP = 0x5F,  M_NC67 = 0x5E,  M_NC66  = 0x5D,  M_NC65  = 0x5C,  M_SCI8 = 0x5B,  M_SCI9 = 0x5A,  M_NC64  = 0x59,  M_NC63  = 0x58,
		M_NC62 = 0x57,  M_NC61 = 0x56,  M_NC60  = 0x55,  M_IRDA  = 0x54,  M_CAC  = 0x53,  M_NC59 = 0x52,  M_NC58  = 0x51,  M_NC57  = 0x50,
		M_NC56 = 0x4F,  M_NC55 = 0x4E,  M_NC54  = 0x4D,  M_NC53  = 0x4C,  M_NC52 = 0x4B,  M_NC51 = 0x4A,  M_NC50  = 0x49,  M_NC49  = 0x48,
		M_NC48 = 0x47,  M_NC47 = 0x46,  M_NC46  = 0x45,  M_NC45  = 0x44,  M_NC44 = 0x43,  M_NC43 = 0x42,  M_NC42  = 0x41,  M_RAM0  = 0x40,

		// MSTPCRD
		M_SEC  = 0x7F,  M_NC95 = 0x7E,  M_NC94  = 0x7D,  M_NC93  = 0x7C,  M_NC92 = 0x7B,  M_NC91 = 0x7A,  M_NC90  = 0x79,  M_NC89  = 0x78,
		M_NC88 = 0x77,  M_NC87 = 0x76,  M_NC86  = 0x75,  M_NC85  = 0x74,  M_SDHI = 0x73,  M_NC84 = 0x72,  M_NC83  = 0x71,  M_NC82  = 0x70,
		M_SSI  = 0x6F,  M_NC81 = 0x6E,  M_NC80  = 0x6D,  M_NC79  = 0x6C,  M_NC78 = 0x6B,  M_CTSU = 0x6A,  M_NC77  = 0x69,  M_NC76  = 0x68,
		M_NC75 = 0x67,  M_NC74 = 0x66,  M_NC73  = 0x65,  M_NC72  = 0x64,  M_NC71 = 0x63,  M_NC70 = 0x62,  M_NC69  = 0x61,  M_NC68  = 0x60,
		NUM    = 0x80,
	};

	// ���W���[���̊J�n
	__inline static void RunModule(int mod);

	// ���W���[���̒�~
	__inline static void StopModule(int mod);

};


////////////////////////////////////////////////////////////////////////////////
// �����݃R���g���[��
//
class CIcuObject
{
public:
	CIcuObject()  {}
	~CIcuObject() {}

public:

	//
	__inline static void CleatInterruptRequest(unsigned int vect);
	__inline static bool IsInterruptRequest(unsigned int vect);

	// �����ݗv�����̐ݒ�
	__inline static void EnableInterrupt(unsigned int vect, bool bEnable);

	// �����ݗv������Ԃ̎擾
	__inline static bool IsEnableInterrupt(unsigned int vect);

	// �����ݗD�揇�ʂ̐ݒ�
	enum {
		PLV0	= 0x00,		// �D�惌�x��0(�Œ�)
		PLV1	= 0x01,		// �D�惌�x��1
		PLV2	= 0x02,		// �D�惌�x��2
		PLV3	= 0x03,		// �D�惌�x��3
		PLV4	= 0x04,		// �D�惌�x��4
		PLV5	= 0x05,		// �D�惌�x��5
		PLV6	= 0x06,		// �D�惌�x��6
		PLV7	= 0x07,		// �D�惌�x��7
		PLV8	= 0x08,		// �D�惌�x��8
		PLV9	= 0x09,		// �D�惌�x��9
		PLV10	= 0x0A,		// �D�惌�x��10
		PLV11	= 0x0B,		// �D�惌�x��11
		PLV12	= 0x0C,		// �D�惌�x��12
		PLV13	= 0x0D,		// �D�惌�x��13
		PLV14	= 0x0E,		// �D�惌�x��14
		PLV15	= 0x0F,		// �D�惌�x��15(�ō�)
	};
	__inline static void SetPriority(unsigned int vect, unsigned char nPriority);

	// IRQ�����݌��o���@�̐ݒ�
	enum {
		LOW_LEVEL, LOW_EDGE, HIGH_EDGE, BOTH_EDGE
	};
	__inline static void SetIrqSense(unsigned char irq, unsigned char nSense);

};


////////////////////////////////////////////////////////////////////////////////
// I/O�|�[�g�R���g���[��
//
class CDioObject
{
public:
	CDioObject()  {}
	~CDioObject() {}

public:

	enum {
		P_P0, P_P1, P_P2, P_P3, P_P4, P_P5, P_PA, P_PB, P_PC, P_PD, P_PE, P_PH, P_PJ, NUM
	};

	// �|�[�g�������W�X�^�̐ݒ�
	__inline static void SetDir(int port, int num, bool bOut);

	// �|�[�g�o�̓f�[�^���W�X�^�̐ݒ�
	__inline static void SetData(int port, int num, bool bOn);

	// �|�[�g���̓f�[�^���W�X�^�̎擾
	__inline static bool GetLineStatus(int port, int num);

	// �|�[�g���[�h���W�X�^�̐ݒ�
	__inline static void SetModeFunc(int port, int num, bool bMode);

	// �I�[�v���h���C�����䃌�W�X�^�̐ݒ�
	__inline static void SetOpenDrain(int port, int num, bool bOdr);

	// �v���A�b�v���䃌�W�X�^�̐ݒ�
	__inline static void SetInputPullup(int port, int num, bool bPullup);

	// �|�[�g�ؑփ��W�X�^A�̐ݒ�
	enum {
		PSRA_PB6 = 0x00, PSRA_PC0 = 0x40,
		PSRA_PB7 = 0x00, PSRA_PC1 = 0x80
	};
	__inline static void SetPSRA(unsigned char flag);

	// �|�[�g�ؑփ��W�X�^B�̐ݒ�
	enum {
		PSRB_PB0 = 0x00, PSRB_PC0 = 0x01,
		PSRB_PB1 = 0x00, PSRB_PC1 = 0x02,
		PSRB_PB3 = 0x00, PSRB_PC2 = 0x08,
		PSRB_PB5 = 0x00, PSRB_PC3 = 0x20
	};
	__inline static void SetPSRB(unsigned char flag);

};

////////////////////////////////////////////////////////////////////////////////
// �}���`�t�@���N�V�����s���R���g���[��
//
class CMpcObject
{
public:
	CMpcObject()  {}
	~CMpcObject() {}

public:
	enum PORTSEL {
		P_P0,	P_P1,	P_P2,	P_P3,	P_P4,	P_P5,	P_PA,	P_PB,	P_PC,	P_PD,	P_PE,	P_PH,	P_PJ,
								P_P03,			P_P05,			P_P07,
						P_P12,	P_P13,	P_P14,	P_P15,	P_P16,	P_P17,
		P_P20,	P_P21,	P_P22,	P_P23,	P_P24,	P_P25,	P_P26,	P_P27,
		P_P30,	P_P31,	P_P32,	P_P33,	P_P34,
		P_P40,	P_P41,	P_P42,	P_P43,	P_P44,	P_P45,	P_P46,	P_P47,
		P_P50,	P_P51,	P_P52,	P_P53,	P_P54,	P_P55,
		P_PA0,	P_PA1,	P_PA2,	P_PA3,	P_PA4,	P_PA5,	P_PA6,	P_PA7,
		P_PB0,	P_PB1,	P_PB2,	P_PB3,	P_PB4,	P_PB5,	P_PB6,	P_PB7,
		P_PC0,	P_PC1,	P_PC2,	P_PC3,	P_PC4,	P_PC5,	P_PC6,	P_PC7,
		P_PD0,	P_PD1,	P_PD2,	P_PD3,	P_PD4,	P_PD5,	P_PD6,	P_PD7,
		P_PE0,	P_PE1,	P_PE2,	P_PE3,	P_PE4,	P_PE5,	P_PE6,	P_PE7,
		P_PH0,	P_PH1,	P_PH2,	P_PH3,
								P_PJ3,
	};

	enum PORTFUNC {
		HI_Z,																																		// �n�C�C���s�[�_���X
		NO_IRQ,			IRQ0,			IRQ1,			IRQ2,			IRQ3,			IRQ4,			IRQ5,			IRQ6,			IRQ7,		// ������
		CLKOUT,																																		// �N���b�N������H
		MTIOC0A,		MTIOC0B,		MTIOC0C,		MTIOC0D,																					// �}���`�t�@���N�V�����^�C�}���j�b�g2
		MTIOC1A,		MTIOC1B,
		MTIOC2A,		MTIOC2B,
		MTIOC3A,		MTIOC3B,		MTIOC3C,		MTIOC3D,
		MTIOC4A,		MTIOC4B,		MTIOC4C,		MTIOC4D,
		MTIC5U,			MTIC5V,			MTIC5W,
		MTCLKA,			MTCLKB,			MTCLKC,			MTCLKD,
		POE0,			POE1,			POE2,			POE3,			POE8,																		// �|�[�g�A�E�g�v�b�g�C�l�[�u��2
		TIOCA0,			TIOCB0,			TIOCC0,			TIOCD0,																						// 16�r�b�g�^�C�}�p���X���j�b�g
		TIOCA1,			TIOCB1,
		TIOCA2,			TIOCB2,
		TIOCA3,			TIOCB3,			TIOCC3,			TIOCD3,
		TIOCA4,			TIOCB4,
		TIOCA5,			TIOCB5,
		TCLKA,			TCLKB,			TCLKC,			TCLKD,
		TMO0,			TMCI0,			TMRI0,																										// 8�r�b�g�^�C�}
		TMO1,			TMCI1,			TMRI1,
		TMO2,			TMCI2,			TMRI2,
		TMO3,			TMCI3,			TMRI3,
		RXD0,			RXD1,			RXD5,			RXD6,			RXD8,			RXD9,			RXD12,			RXDX12,						// �V���A���R�~���j�P�[�V�����C���^�t�F�[�X
		SMISO0,			SMISO1,			SMISO5,			SMISO6,			SMISO8,			SMISO9,			SMISO12,
		SSCL0,			SSCL1,			SSCL5,			SSCL6,			SSCL8,			SSCL9,			SSCL12,
		TXD0,			TXD1,			TXD5,			TXD6,			TXD8,			TXD9,			TXD12,			TXDX12,
		SMOSI0,			SMOSI1,			SMOSI5,			SMOSI6,			SMOSI8,			SMOSI9,			SMOSI12,		SIOX12,
		SSDA0,			SSDA1,			SSDA5,			SSDA6,			SSDA8,			SSDA9,			SSDA12,
		SCK0,			SCK1,			SCK5,			SCK6,			SCK8,			SCK9,			SCK12,
		CTS0,			CTS1,			CTS5,			CTS6,			CTS8,			CTS9,			CTS12,
		RTS0,			RTS1,			RTS5,			RTS6,			RTS8,			RTS9,			RTS12,
		SS0,			SS1,			SS5,			SS6,			SS8,			SS9,			SS12,
		SCL,			SDA,																														// I2C�o�X�C���^�t�F�[�X
		RSPCKA,			MOSIA,			MISOA,																										// �V���A���y���t�F�����C���^�t�F�[�X
		SSLA0,			SSLA1,			SSLA2,			SSLA3,
		RTCOUT,			RTCIC0,			RTCIC1,			RTCIC2,																						// ���A���^�C���N���b�N
		IRTXD5,			IRRXD5,																														// IrDA�C���^�t�F�[�X
		CRXD0,			CTXD0,																														// CAN
		SSISCK0,		SSIWS0,			SSITXD0,		SSIRXD0,		AUDIO_MCLK,																	// �V���A���T�E���h�C���^�t�F�[�X
		SDHI_CLK,		SDHI_CMD,		SDHI_CD,		SDHI_WP,																					// SD�z�X�g�C���^�t�F�[�X
		SDHI_D0,		SDHI_D1,		SDHI_D2,		SDHI_D3,
		USB0_VBUS,		USB0_EXICEN,	USB0_VBUSEN,	USB0_OVRCURA,	USB0_OVRCURB,	USB0_ID,													// USB2.0�z�X�g�^�t�@���N�V�������W���[��
		NO_AD,			AN000,			AN001,			AN002,			AN003,			AN004,			AN005,			AN006,			AN007,		// 12�r�b�gAD�R���o�[�^
		AN016,			AN017,			AN018,			AN019,			AN020,			AN021,			AN022,			AN023,			AN024,
		AN025,			AN026,			AN027,			AN028,			AN029,			AN030,			AN031,
		ADTRG0,
		NO_DA,			DA0,			DA1,																										// DA�R���o�[�^
		CACREF,																																		// �N���b�N���g�����x�����H
		CMPA2,																																		// LVD�d�����o����
		CMPB0,			CMPB1,			CMPB2,			CMPB3,																						// �R���p���[�^B
		CVREFB0,		CVREFB1,		CVREFB2,		CVREFB3,
		CMPOB0,			CMPOB1,			CMPOB2,			CMPOB3,
		TSCAP,																																		// �Ód�e�ʃ^�b�`�Z���T
		TS0,			TS1,			TS2,			TS3,			TS4,			TS5,			TS6,			TS7,
		TS8,			TS9,			TS12,			TS13,			TS15,			TS16,			TS17,			TS18,
		TS19,			TS20,			TS22,			TS23,			TS27,			TS30,			TS33,			TS35,
	};

protected:

	// �v���e�N�g���W�X�^�������݂̐ݒ�
	__inline static void _EnableWriteForPFS(bool bEnable);

public:

	// �|�[�g�t�@���N�V�����̐ݒ�
	__inline static void SetPortFunction(PORTSEL ps, PORTFUNC pf);

	// ���Ӌ@�\�̐ݒ�
	__inline static void UsePeripheralPort(PORTSEL ps, int num, bool bEnable);

};

////////////////////////////////////////////////////////////////////////////////
// �}���`�t�@���N�V�����^�C�}�p���X���j�b�g2�R���g���[��
//
class CMtuObject
{
public:
	CMtuObject()  {}
	~CMtuObject() {}

public:

	// MTU���W���[���̐ݒ�
	enum {
		MTU_U0, MTU_UNIT_NUM
	};
	__inline static void RunModule(unsigned int nUnit, bool bRun);

	// TCR�̐ݒ�
	enum {
		TCR_IN_1		= 0x0000,		// �����N���b�N�FPCLK/1�ŃJ�E���g
		TCR_IN_4		= 0x0001,		// �����N���b�N�FPCLK/4�ŃJ�E���g
		TCR_IN_16		= 0x0002,		// �����N���b�N�FPCLK/16�ŃJ�E���g
		TCR_IN_64		= 0x0003,		// �����N���b�N�FPCLK/64�ŃJ�E���g
		TCR_IN_256		= 0x0004,		// �����N���b�N�FPCLK/256�ŃJ�E���g
		TCR_IN_1024		= 0x0005,		// �����N���b�N�FPCLK/1024�ŃJ�E���g
		TCR_MTCLKA		= 0x0006,		// �O���N���b�N�FMTCLKA�[�q���͂ŃJ�E���g
		TCR_MTCLKB		= 0x0007,		// �O���N���b�N�FMTCLKB�[�q���͂ŃJ�E���g
		TCR_MTCLKC		= 0x0008,		// �O���N���b�N�FMTCLKC�[�q���͂ŃJ�E���g
		TCR_MTCLKD		= 0x0009,		// �O���N���b�N�FMTCLKD�[�q���͂ŃJ�E���g
		TCR_TCNT		= 0x000A		// TCNTn�̃I�[�o�t���[�^�A���_�t���[�ŃJ�E���g
	};
	enum {
		TCR_HIGH_EDGE	= 0x0000,		// �����オ��G�b�W�ŃJ�E���g
		TCR_LOW_EDGE	= 0x0010,		// ����������G�b�W�ŃJ�E���g
		TCR_BOTH_EDGE	= 0x0020		// ���G�b�W�ŃJ�E���g
	};
	enum {
		TCR_DISABLE		= 0x0000,		// TCNT�̃N���A�֎~
		TCR_TGRA		= 0x0100,		// TGRA�̃R���y�A�}�b�`�^�C���v�b�g�L���v�`����TCNT�N���A
		TCR_TGRB		= 0x0200,		// TGRB�̃R���y�A�}�b�`�^�C���v�b�g�L���v�`����TCNT�N���A
		TCR_TGRC		= 0x0300,		// TGRC�̃R���y�A�}�b�`�^�C���v�b�g�L���v�`����TCNT�N���A
		TCR_TGRD		= 0x0400,		// TGRD�̃R���y�A�}�b�`�^�C���v�b�g�L���v�`����TCNT�N���A
		TCR_OTHER		= 0x0500		// �����N���A�^������������Ă��鑼�̃`���l���̃J�E���^�N���A��TCNT���N���A
	};
	__inline static void SetControl(int ch, unsigned int flag, unsigned int flagv = 0, unsigned int flagw = 0);

	// TMDR�̐ݒ�
	enum {
		TMDR_NORMAL		= 0x00,			// �m�[�}�����[�h
		TMDR_PWM1		= 0x02,			// PWM���[�h1
		TMDR_PWM2		= 0x03,			// PWM���[�h2
		TMDR_PHASE1		= 0x04,			// �ʑ��v�����[�h1
		TMDR_PHASE2		= 0x05,			// �ʑ��v�����[�h2
		TMDR_PHASE3		= 0x06,			// �ʑ��v�����[�h3
		TMDR_PHASE4		= 0x07,			// �ʑ��v�����[�h4
		TMDR_RPWM1		= 0x08,			// ���Z�b�g����PWM���[�h
		TMDR_DPWM1		= 0x0D,			// ����PWM���[�h1(�R�œ]��)
		TMDR_DPWM2		= 0x0E,			// ����PWM���[�h2(�J�œ]��)
		TMDR_DPWM3		= 0x0F			// ����PWM���[�h3(�R�E�J�œ]��)
	};
	enum {
		TMDR_BFA		= 0x10,			// TGRA��TGRC�̓o�b�t�@����
		TMDR_BFB		= 0x20,			// TGRB��TGRD�̓o�b�t�@����
		TMDR_BFE		= 0x40			// TGRE��TGRF�̓o�b�t�@����
	};
	__inline static void SetMode(int ch, unsigned char mode);

	// TIOR�̐ݒ�
	enum {
		TIOR_TIOCA_DISABLE		= 0x00,	// �o�͋֎~
		TIOR_TIOCA_CMATCH_LOW	= 0x01,	// �R���y�A�}�b�`��Low�o��
		TIOR_TIOCA_CMATCH_HIGH	= 0x02,	// �R���y�A�}�b�`��High�o��
		TIOR_TIOCA_CMATCH_TGL	= 0x03,	// �R���y�A�}�b�`�Ńg�O���o��
		TIOR_TIOCA_DEFOUT_LOW	= 0x00,	// �����o��Low
		TIOR_TIOCA_DEFOUT_HIGH	= 0x04,	// �����o��High

		TIOR_TIOCB_DISABLE		= 0x00,	// �o�͋֎~
		TIOR_TIOCB_CMATCH_LOW	= 0x01,	// �R���y�A�}�b�`��Low�o��
		TIOR_TIOCB_CMATCH_HIGH	= 0x02,	// �R���y�A�}�b�`��High�o��
		TIOR_TIOCB_CMATCH_TGL	= 0x03,	// �R���y�A�}�b�`�Ńg�O���o��
		TIOR_TIOCB_DEFOUT_LOW	= 0x00,	// �����o��Low
		TIOR_TIOCB_DEFOUT_HIGH	= 0x04,	// �����o��High

		TIOR_TIOCC_DISABLE		= 0x00,	// �o�͋֎~
		TIOR_TIOCC_CMATCH_LOW	= 0x01,	// �R���y�A�}�b�`��Low�o��
		TIOR_TIOCC_CMATCH_HIGH	= 0x02,	// �R���y�A�}�b�`��High�o��
		TIOR_TIOCC_CMATCH_TGL	= 0x03,	// �R���y�A�}�b�`�Ńg�O���o��
		TIOR_TIOCC_DEFOUT_LOW	= 0x00,	// �����o��Low
		TIOR_TIOCC_DEFOUT_HIGH	= 0x04,	// �����o��High

		TIOR_TIOCD_DISABLE		= 0x00,	// �o�͋֎~
		TIOR_TIOCD_CMATCH_LOW	= 0x01,	// �R���y�A�}�b�`��Low�o��
		TIOR_TIOCD_CMATCH_HIGH	= 0x02,	// �R���y�A�}�b�`��High�o��
		TIOR_TIOCD_CMATCH_TGL	= 0x03,	// �R���y�A�}�b�`�Ńg�O���o��
		TIOR_TIOCD_DEFOUT_LOW	= 0x00,	// �����o��Low
		TIOR_TIOCD_DEFOUT_HIGH	= 0x04,	// �����o��High

		TIOR_TIOCA_CAPTURE_HIGH	= 0x08,	// �C���v�b�g�L���v�`��(�����オ��G�b�W)
		TIOR_TIOCA_CAPTURE_LOW	= 0x09,	// �C���v�b�g�L���v�`��(����������G�b�W)
		TIOR_TIOCA_CAPTURE_BOTH	= 0x0A,	// �C���v�b�g�L���v�`��(���G�b�W)
		TIOR_TIOCA_CAPTURE_ICLK	= 0x0B,	// �C���v�b�g�L���v�`��(1/�J�E���g�N���b�N)

		TIOR_TIOCB_CAPTURE_HIGH	= 0x08,	// �C���v�b�g�L���v�`��(�����オ��G�b�W)
		TIOR_TIOCB_CAPTURE_LOW	= 0x09,	// �C���v�b�g�L���v�`��(����������G�b�W)
		TIOR_TIOCB_CAPTURE_BOTH	= 0x0A,	// �C���v�b�g�L���v�`��(���G�b�W)
		TIOR_TIOCB_CAPTURE_ICLK	= 0x0B	// �C���v�b�g�L���v�`��(1/�J�E���g�N���b�N)
	};
	__inline static void SetIOControl(int ch, unsigned char tgra, unsigned char tgrb, unsigned char tgrc, unsigned char tgrd);

	// �^�C�}�����݋��̐ݒ�
	enum {
		TIER_TGIEA	= 0x01,				// ���荞�ݗv��(TGIA)�̋���
		TIER_TGIEB	= 0x02,				// ���荞�ݗv��(TGIB)�̋���
		TIER_TGIEC	= 0x04,				// ���荞�ݗv��(TGIC)�̋���
		TIER_TGIED	= 0x08,				// ���荞�ݗv��(TGID)�̋���
		TIER_TGIEV	= 0x10,				// ���荞�ݗv��(TCIV)�̋���
		TIER_TGIEU	= 0x20,				// ���荞�ݗv��(TCIU)�̋���
		TIER_TTGE2	= 0x40,				// TCNT�J�E���^�̃A���_�t���[(�J)�ɂ��A/D�ϊ��v���̋���
		TIER_TTGE	= 0x80				// A/D�ϊ��J�n�v���̔����̋���
	};
	enum {
		TIER_TGIE5W	= 0x01,				// TGI5W���荞�ݗv���̋���
		TIER_TGIE5V	= 0x02,				// TGI5V���荞�ݗv���̋���
		TIER_TGIE5U	= 0x04				// TGI5U���荞�ݗv���̋���
	};
	enum {
		TIER2_TGIEE	= 0x01,				// ���荞�ݗv��(TGIE)�̋���
		TIER2_TGIEF	= 0x02				// ���荞�ݗv��(TGIF)�̋���
	};
	__inline static void EnableInterrupt(int ch, unsigned char tier, unsigned char tier2 = 0);

	// �^�C�}�X�e�[�^�X�t���O�̐ݒ�Ǝ擾
	enum {
		TSR_TC_DN = 0x00,
		TSR_TC_UP = 0x80
	};
	__inline static void SetTSR(int ch, unsigned char flag);
	__inline static bool GetTSR(int ch, unsigned char flag);

	// �^�C�}�J�E���^�̐ݒ�Ǝ擾
	__inline static void SetTCNT(int ch, unsigned int tcnt, unsigned int tcntv = 0, unsigned int tcntw = 0);
	__inline static void GetTCNT(int ch, unsigned int &tcnt, unsigned int &tcntv, unsigned int &tcntw);

	// �^�C�}�W�F�l�������W�X�^�̐ݒ�Ǝ擾
	enum {
		TGR_TGRA,
		TGR_TGRB,
		TGR_TGRC,
		TGR_TGRD,
		TGR_TGRE,
		TGR_TGRF,
		TGR_TGRU,
		TGR_TGRV,
		TGR_TGRW
	};
	__inline static void SetTGR(int ch, int flag, unsigned int val);
	__inline static unsigned int GetTGR(int ch, int flag);

	// �^�C�}�X�^�[�g�ƃ^�C�}�X�g�b�v
	enum {
		TSTR_NONE	= 0x0000,
		TSTR_MTU0	= 0x0001,
		TSTR_MTU1	= 0x0002,
		TSTR_MTU2	= 0x0004,
		TSTR_MTU3	= 0x0040,
		TSTR_MTU4	= 0x0080,
		TSTR_MTU5W	= 0x0100,
		TSTR_MTU5V	= 0x0200,
		TSTR_MTU5U	= 0x0400
	};
	__inline static void Start(int flag);
	__inline static void Stop(int flag);

	// �^�C�}�V���N�����W�X�^�̐ݒ�
	__inline static void SyncEnable(int ch, bool bEnable);

};

////////////////////////////////////////////////////////////////////////////////
// �R���y�A�}�b�`�^�C�}�R���g���[��
//
class CCmtObject
{
public:
	CCmtObject()  {}
	~CCmtObject() {}

public:

	// CMT���W���[���̐ݒ�
	enum {
		CMT_U0, CMT_U1, CMT_UNIT_NUM
	};
	__inline static void RunModule(unsigned int nUnit, bool bRun);

	// �C���^�[�o���^�C�}�̐ݒ�
	__inline static bool SetIntervalTimer(int ch, int nMHz, int ms);

	// �J�E���g����̊J�n
	__inline static void StartCounter(int ch, bool bEnable);

	// �N���b�N�̐ݒ�
	enum {
		PCLK_1_8, PCLK_1_32, PCLK_1_128, PCLK_1_512
	};
	__inline static void SelectClock(int ch, int cks);

	// �R���y�A�}�b�`�����݋��̐ݒ�
	__inline static void EnableInterrupt(int ch, bool bEnable);

	// �J�E���^�̐ݒ�Ǝ擾
	__inline static void SetCounter(int ch, int cnt);
	__inline static int GetCounter(int ch);

	// �C���^�[�o���̐ݒ�Ǝ擾
	__inline static void SetInterval(int ch, int cnt);
	__inline static int GetInterval(int ch);

};

////////////////////////////////////////////////////////////////////////////////
// �Ɨ��E�H�b�`�h�b�O�^�C�}�R���g���[��
//
class CIwdtObject
{
public:
	CIwdtObject()  {}
	~CIwdtObject() {}

public:

	// �J�E���^�̃��t���b�V��
	__inline static void RefreshCounter();

	// �^�C���A�E�g���Ԃ̐ݒ�
	enum {
		CYCLE_128, CYCLE_512, CYCLE_1024, CYCLE_2048
	};
	__inline static void SetTimeoutPeriod(int cycle);

	// �N���b�N�̐ݒ�
	enum {
		IWDTCLK, IWDTCLK_1_16, IWDTCLK_1_32, IWDTCLK_1_64, IWDTCLK_1_128, IWDTCLK_1_256
	};
	__inline static void SelectClock(int cks);

	// �E�B���h�E�I���ʒu�̐ݒ�
	enum {
		END_POS_75, END_POS_50, END_POS_25, END_POS_0
	};
	__inline static void SetWindowEndPosition(int pos);

	// �E�B���h�E�J�n�ʒu�̐ݒ�
	enum {
		START_POS_25, START_POS_50, START_POS_75, START_POS_100
	};
	__inline static void SetWindowStartPosition(int pos);

	// �J�E���^�̎擾
	__inline static int GetCounter();

	// �A���_�[�t���[�t���O�̃N���A�Ǝ擾
	__inline static void ClearUnderflowFlag();
	__inline static bool GetUnderflowFlag();

	// ���t���b�V���G���[�t���O�̃N���A�Ǝ擾
	__inline static void ClearRefreshErrorFlag();
	__inline static bool GetRefreshErrorFlag();

	// �m���}�X�J�u�������݋��̐ݒ�
	__inline static void EnableNonMaskableInterrupt(bool bEnable);

	// �J�E���^��~�̐ݒ�(�����d�͏�Ԏ�)
	__inline static void StopSleepModeCount(bool bEnable);

};

////////////////////////////////////////////////////////////////////////////////
// USB2.0�z�X�g�^�t�@���N�V�������W���[���R���g���[��
//
class CUsbObject
{
public:
	CUsbObject()  {}
	~CUsbObject() {}

public:

	// USB���W���[���̐ݒ�
	__inline static void RunModule(int ch, bool bRun);

	// SYSCFG
	static void EnableModule(int ch, bool bEnable);
	static void EnablePullupMinus(int ch, bool bEnable);
	static void EnablePullupPlus(int ch, bool bEnable);
	static void EnablePulldown(int ch, bool bEnable);
	static void SelectHostController(int ch, bool bEnable);
	static void EnableSingleEndReceiver(int ch, bool bEnable);
	static void EnableSupplyClockToUsbModule(int ch, bool bEnable);

	// SYSSTS0
	enum LNST { SE0, J_STATE, K_STATE, SE1 };
	static LNST GetDataLineStatus(int ch);
	static bool IsUsbIdHigh(int ch);
	static bool IsStopHostSequencer(int ch);
	static int GetOverCurrentStatus(int ch);

	// DVSTCTR0
	enum RHST { UNSTABLE, BUS_RESET, LOW_SPEED, FULL_SPEED };
	static RHST GetBusResetStatus(int ch);
	static void EnableDownPort(int ch, bool bEnable);
	static void OutputResume(int ch, bool bEnable);
	static void OutputBusReset(int ch, bool bEnable);
	static void EnableDownportRemoteWakeup(int ch, bool bEnable);
	static void OutputRemoteWakeup(int ch, bool bEnable);
	static void SetVbusenHigh(int ch, bool bEnable);
	static void SetExicenHigh(int ch, bool bEnable);
	static void SetHostNegotiationProtocolHigh(int ch, bool bEnable);

	// CFIFO D0FIFO D1FIFO
	enum FIFO { CFIFO, D0FIFO, D1FIFO };
	static void SetFifoData(int ch, FIFO fifo, unsigned short data);
	static void SetFifoData(int ch, FIFO fifo, unsigned char data);
	static unsigned short GetFifoDataWord(int ch, FIFO fifo);
	static unsigned char GetFifoDataByte(int ch, FIFO fifo);

	// CFIFOSEL D0FIFOSEL D1FIFOSEL
	static void SelectFifoPipe(int ch, FIFO fifo, int nPipe);
	static int GetFifoPipe(int ch, FIFO fifo);
	static void SelectDcpFifoDirection(int ch, FIFO fifo, bool bWrite);			// Only DCP
	static bool IsDcpFifoDirectionWrite(int ch, FIFO fifo);						// Only DCP
	static void SelectFifoBigEndian(int ch, FIFO fifo, bool bEnable);
	static bool IsFifoBigEndian(int ch, FIFO fifo);
	static void SelectFifoWidth16(int ch, FIFO fifo, bool bEnable);
	static bool IsFifoWidth16(int ch, FIFO fifo);
	static void EnableRequestFifoDtc(int ch, FIFO fifo, bool bEnable);			// Only D0 and D1
	static void EnableFifoBufferAutoClear(int ch, FIFO fifo, bool bEnable);		// Only D0 and D1
	static void SelectFifoBufferPointerRewind(int ch, FIFO fifo, bool bEnable);
	static void EnableFifoReadDataCountDownMode(int ch, FIFO fifo, bool bEach);

	// CFIFOCTR D0FIFOCTR D1FIFOCTR
	static int GetFifoReceiveDataLength(int ch, FIFO fifo);
	static bool IsFifoPortReady(int ch, FIFO fifo);
	static void ClearFifoCpuBusBuffer(int ch, FIFO fifo);
	static void ValidateFifoBufferMemory(int ch, FIFO fifo, bool bEnable);

	// INTENB0
	static void EnableBufferReadyInterrupt(int ch, bool bEnable);
	static void EnableBufferNotReadyInterrupt(int ch, bool bEnable);
	static void EnableBufferEmptyInterrupt(int ch, bool bEnable);
	static void EnableControlTransferStageInterrupt(int ch, bool bEnable);
	static void EnableDeviceStateInterrupt(int ch, bool bEnable);
	static void EnableFrameNumberUpdateInterrupt(int ch, bool bEnable);
	static void EnableResumeInterrupt(int ch, bool bEnable);
	static void EnableVbusInterrupt(int ch, bool bEnable);

	// INTENB1
	static void EnableDetectPDDETINT0Interrupt(int ch, bool bEnable);
	static void EnableSetupTransactionNormalResponseInterrupt(int ch, bool bEnable);
	static void EnableSetupTransactionErrorInterrupt(int ch, bool bEnable);
	static void EnableEOFErrorInterrupt(int ch, bool bEnable);
	static void EnableDetectAttachInterrupt(int ch, bool bEnable);
	static void EnableDetectDetachInterrupt(int ch, bool bEnable);
	static void EnableChangeBusStateInterrupt(int ch, bool bEnable);
	static void EnableOverCurrentInterrupt(int ch, bool bEnable);

	// BRDYENB
	static void EnablePipeBufferReadyInterrupt(int ch, int nPipe, bool bEnable);

	// NRDYENB
	static void EnablePipeBufferNotReadyInterrupt(int ch, int nPipe, bool bEnable);

	// BEMPENB
	static void EnablePipeBufferEmptyInterrupt(int ch, int nPipe, bool bEnable);

	// SOFCFG
	static bool IsEdgeInterruptProcessing(int ch);
	static void SetClearBufferReadyInterruptStatusByModule(int ch, bool bEnable);
	static void SetTransactionEnabledTimeLowSpeed(int ch, bool bEnable);

	// INTSTS0
	enum CTSQ { IDLE_SETUP, READ_DATA, READ_STATUS, WRITE_DATA, WRITE_STATUS, WRITE_NO_DATA, SEQ_ERROR };
	static CTSQ GetControlTransferStage(int ch);
	static bool IsReceiveUsbSetupPacket(int ch);
	enum DVSQ { POWERED, DEFAULT, ADDRESS, CONFIGURED, SUSPENDED };
	static DVSQ GetDeviceState(int ch);
	static bool IsVbusStatusHigh(int ch);

	static bool IsDetectBufferReadyInterrupt(int ch);
	static bool IsDetectBufferNotReadyInterrupt(int ch);
	static bool IsDetectBufferEmptyInterrupt(int ch);
	static bool IsDetectControlTransferStageInterrupt(int ch);
	static bool IsDetectDeviceStateInterrupt(int ch);
	static bool IsDetectFrameNumberUpdateInterrupt(int ch);
	static bool IsDetectResumeInterrupt(int ch);
	static bool IsDetectVbusInterrupt(int ch);

	static void ClearValid(int ch);
	static void ClearControlTransferStageInterrupt(int ch);
	static void ClearDeviceStateInterrupt(int ch);
	static void ClearFrameNumberUpdateInterrupt(int ch);
	static void ClearResumeInterrupt(int ch);
	static void ClearVbusInterrupt(int ch);

	// INTSTS1
	static bool IsDetectPDDETINT0Interrupt(int ch);
	static bool IsDetectSetupTransactionNormalResponseInterrupt(int ch);
	static bool IsDetectSetupTransactionErrorInterrupt(int ch);
	static bool IsDetectEOFErrorInterrupt(int ch);
	static bool IsDetectAttachInterrupt(int ch);
	static bool IsDetectDetachInterrupt(int ch);
	static bool IsDetectChangeBusStateInterrupt(int ch);
	static bool IsDetectOverCurrentInterrupt(int ch);

	static void ClearPDDETINT0Interrupt(int ch);
	static void ClearSetupTransactionNormalResponseInterrupt(int ch);
	static void ClearSetupTransactionErrorInterrupt(int ch);
	static void ClearEOFErrorInterrupt(int ch);
	static void ClearDetectAttachInterrupt(int ch);
	static void ClearDetectDetachInterrupt(int ch);
	static void ClearChangeBusStateInterrupt(int ch);
	static void ClearOverCurrentInterrupt(int ch);

	// BRDYSTS
	static bool IsDetectPipeBufferReadyInterrupt(int ch, int nPipe);
	static void ClearPipeBufferReadyInterrupt(int ch, int nPipe);

	// NRDYSTS
	static bool IsDetectPipeBufferNotReadyInterrupt(int ch, int nPipe);
	static void ClearPipeBufferNotReadyInterrupt(int ch, int nPipe);

	// BEMPSTS
	static bool IsDetectPipeBufferEmptyInterrupt(int ch, int nPipe);
	static void ClearPipeBufferEmptyInterrupt(int ch, int nPipe);

	// FRMNUM
	static int GetFrameNumber(int ch);
	static bool IsDetectReceiveError(int ch);
	static bool IsDetectOverrunUnderrunError(int ch);
	static void ClearReceiveError(int ch);
	static void ClearOverrunUnderrunError(int ch);

	// USBREQ
	static int GetRequestType(int ch);
	static void SetRequestType(int ch, int type);
	static int GetRequestData(int ch);
	static void SetRequestData(int ch, int data);

	// USBVAL
	static int GetRequestValue(int ch);
	static void SetRequestValue(int ch, int value);

	// USBINDX
	static int GetRequestIndex(int ch);
	static void SetRequestIndex(int ch, int index);

	// USBLENG
	static int GetRequestLength(int ch);
	static void SetRequestLength(int ch, int length);

	// DCPCFG
	static void SetHostDirection(int ch, bool bTransfer);
	static void SetNakWhenTransferFinish(int ch, bool bNak);

	// DCPMAXP
	static void SetDcpMaxPacketSize(int ch, int size);
	static int GetDcpMaxPacketSize(int ch);
	static void SetDcpUsbAddress(int ch, int addr);

	// DCPCTR
	enum PID { NAK, BUF, STALL2, STALL3 };
	static void SetDcpResponsePid(int ch, PID pid);
	static PID GetDcpResponsePid(int ch);
	enum PID2 { PID_NAK, PID_BUF, PID_STALL };
	static void ChangeDcpResponsePid(int ch, PID2 pid);
	static void EnableDcpControlTransferEnd(int ch, bool bEnd);
	static bool IsDcpPipeBusy(int ch);
	enum SEQDATA { DATA0, DATA1 };
	static SEQDATA GetDcpSequenceToggleBit(int ch);
	static void SetDcpSequenceToggleBit(int ch, SEQDATA seqdata);
	static void ClearDcpSetupToken(int ch, bool bEnable);
	static void SetDcpSetupToken(int ch, bool bEnable);
	static bool IsEnableDcpBufferAccess(int ch);

	// PIPESEL
	static void _SelectPipe(int ch, int nPipe);
	static int _GetSelectedPipe(int ch);

	// PIPECFG
	static void SetPipeEndPointNumber(int ch, int num);
	static void SetPipeDirectionTransfer(int ch, bool bTransfer);
	static void EnablePipeWhenTransferEnd(int ch, bool bEnable);
	static void EnablePipeDoubleBuffer(int ch, bool bEnable);
	static bool IsPipeDoubleBuffer(int ch);
	enum BFRE { DATA_TRANSEIVE, READ_END };
	static void SetPipeBufferReadyInterruptAction(int ch, BFRE bfre);
	enum TYPE { NOT_USE, BULK, INTERRUPT, ISOCHRONOUS };
	static void SetPipeTransferType(int ch, TYPE type);

	// PIPEMAXP
	static void SetPipeMaxPacketSize(int ch, int size);
	static int GetPipeMaxPacketSize(int ch);
	static void SetPipeHostUsbAddress(int ch, int addr);

	// PIPEPERI
	static void SetPipeIntervalErrorDetectInterval(int ch, int value);
	static void FlushPipeIsochronousInBuffer(int ch, bool bEnable);

	// PIPEnCTR	n = 1 �` 9
	static void SetPipeResponsePid(int ch, int nPipe, PID pid);
	static PID GetPipeResponsePid(int ch, int nPipe);
	static void ChangePipeResponsePid(int ch, int nPipe, PID2 pid);
	static bool IsPipePipeBusy(int ch, int nPipe);
	static SEQDATA GetPipeSequenceToggleBit(int ch, int nPipe);
	static void SetPipeSequenceToggleBit(int ch, int nPipe, SEQDATA seqdata);
	static void SetPipeAutoClearMode(int ch, int nPipe, bool bEnable);
	static void EnablePipeAutoResponse(int ch, int nPipe, bool bEnable);
	static bool IsPipeBufferDataExist(int ch, int nPipe);
	static bool IsPipeEnableCpuBufferAccess(int ch, int nPipe);

	// PIPEnTRE	n = 1 �` 5
	static void ClearPipeTransactionCounter(int ch, int nPipe, bool bEnable);
	static void EnablePipeTransactionCounter(int ch, int nPipe, bool bEnable);

	// PIPEnTRN	n = 1 �` 5
	static int GetPipeTransactionCounter(int ch, int nPipe);
	static void SetPipeTransactionCounter(int ch, int nPipe, int cnt);

	// DEVADDn	n = 0 �` 5
	enum USBSPD { PIPE_NOT_USE, PIPE_LOW_SPEED, PIPE_FULL_SPEED };
	static void SetPipeUsbSpeed(int ch, int nPipe, USBSPD usbspd);

	// USBMC
	static void EnableUsbReferencePowerSupplyCircuit(int ch, bool bEnable);
	static bool IsEnableUsbReferencePowerSupplyCircuit(int ch);
	static void EnableUsbRegulator(int ch, bool bEnable);
	static bool IsEnableUsbRegulator(int ch);

	// USBBCCTRL0
	static void EnablePulldownMinus(int ch, bool bEnable);
	static void OutputIdpsrc(int ch, bool bEnable);
	static void EnableDetectInputDMinusPin(int ch, bool bEnable);
	static void OutputVdpsrc(int ch, bool bEnable);
	static void EnableDetectInputDPlusPin(int ch, bool bEnable);
	static void OutputVdmsrc(int ch, bool bEnable);
	static void EnableBatteryCharger(int ch, bool bEnable);
	static bool IsDetectInputDMinusPin(int ch);
	static bool IsDetectInputDPlusPin(int ch);

};

////////////////////////////////////////////////////////////////////////////////
// �V���A���R�~���j�P�[�V�����C���^�t�F�[�X�R���g���[��
//
class CSciObject
{
public:
	CSciObject()  {}
	~CSciObject() {}

public:

	// SCI���W���[���̐ݒ�
	__inline static void RunModule(int ch, bool bRun);

	// ��M�f�[�^�̎擾
	__inline static unsigned char GetReceiveData(int ch);
	__inline static unsigned char GetReceiveDataLow(int ch);
	__inline static unsigned char GetReceiveDataHigh(int ch);

	// ���M�f�[�^�̐ݒ�
	__inline static void WriteTransmitData(int ch, unsigned char data);
	__inline static void WriteTransmitDataLow(int ch, unsigned char data);
	__inline static void WriteTransmitDataHigh(int ch, unsigned char data);

	// �V���A�����[�h���W�X�^�̐ݒ�
	enum {
		SMR_CLKDIV0 = 0x00, SMR_CLKDIV4 = 0x01, SMR_CLKDIV16 = 0x02, SMR_CLKDIV64 = 0x03,
		SMR_MP      = 0x04,
		SMR_STOP1   = 0x00, SMR_STOP2   = 0x08,
		SMR_EVEN    = 0x00, SMR_ODD     = 0x10,
		SMR_PARITY  = 0x20,
		SMR_CHR     = 0x40,
		SMR_ASYNC   = 0x00, SMR_SYNC    = 0x80
	};
	__inline static void SetSMR(int ch, unsigned char flag);

	// �V���A���R���g���[�����W�X�^�̐ݒ�
	enum {
		SCR_SCKOUT = 0x01, SCR_SCKIN = 0x02,
		SCR_TEIE   = 0x04,
		SCR_MPIE   = 0x08,
		SCR_RE     = 0x10,
		SCR_TE     = 0x20,
		SCR_RIE    = 0x40,
		SCR_TIE    = 0x80
	};
	__inline static void SetSCR(int ch, unsigned char flag);

	// ��M�����ݗv�����̐ݒ�
	__inline static void EnableReceiveInterrupt(int ch, bool bEnable);

	// ��M�����ݗv�����̎擾
	__inline static bool IsEnableReceiveInterrupt(int ch);

	// ��M���싖�̐ݒ�
	__inline static void EnableRE(int ch, bool bEnable);

	// ���M�����ݗv�����̐ݒ�
	__inline static void EnableTransmitInterrupt(int ch, bool bEnable);

	// ���M�����ݗv�����̎擾
	__inline static bool IsEnableTransmitInterrupt(int ch);

	// ���M���싖�̐ݒ�
	__inline static void EnableTE(int ch, bool bEnable);

	// �V���A���X�e�[�^�X���W�X�^�̎擾
	__inline static unsigned char GetSSR(int ch);

	// �V���A���X�e�[�^�X���W�X�^TEND�̎擾
	__inline static bool GetTEND(int ch);

	// �V���A���X�e�[�^�XRDRF�̃N���A�Ǝ擾
	__inline static void ClearRDRF(int ch);
	__inline static bool GetRDRF(int ch);

	// �V���A���X�e�[�^�X���W�X�^TDRE�̃N���A�Ǝ擾
	__inline static void ClearTDRE(int ch);
	__inline static bool GetTDRE(int ch);

	// �G���[�t���O�̎擾
	__inline static unsigned char GetErrorFlag(int ch);

	// �X�}�[�g�J�[�h���[�h���W�X�^�̐ݒ�
	enum {
		SCMR_SMIF = 0x01,
		SCMR_SINV = 0x04,
		SCMR_SDIR = 0x08,
		SCMR_CHR1 = 0x10,
		SCMR_BCP2 = 0x80
	};
	__inline static void SetSCMR(int ch, unsigned char flag);

	// �r�b�g���[�g���W�X�^�̐ݒ�
	__inline static void SetBRR(int ch, unsigned char br);

};

////////////////////////////////////////////////////////////////////////////////
// 12�r�b�gA/D�R���o�[�^�R���g���[��
//
class CAdcS12Object
{
public:
	CAdcS12Object()  {}
	~CAdcS12Object() {}

public:

	// S12ADE���W���[���̐ݒ�
	__inline static void RunModule(bool bRun);

	// A/D�ϊ����ʂ̎擾
	__inline static unsigned short Get0()	{ return S12AD.ADDR0;  }
	__inline static unsigned short Get1()	{ return S12AD.ADDR1;  }
	__inline static unsigned short Get2()	{ return S12AD.ADDR2;  }
	__inline static unsigned short Get3()	{ return S12AD.ADDR3;  }
	__inline static unsigned short Get4()	{ return S12AD.ADDR4;  }
	__inline static unsigned short Get5()	{ return S12AD.ADDR5;  }
	__inline static unsigned short Get6()	{ return S12AD.ADDR6;  }
	__inline static unsigned short Get7()	{ return S12AD.ADDR7;  }
	__inline static unsigned short Get16()	{ return S12AD.ADDR16; }
	__inline static unsigned short Get17()	{ return S12AD.ADDR17; }
	__inline static unsigned short Get18()	{ return S12AD.ADDR18; }
	__inline static unsigned short Get19()	{ return S12AD.ADDR19; }
	__inline static unsigned short Get20()	{ return S12AD.ADDR20; }
	__inline static unsigned short Get21()	{ return S12AD.ADDR21; }
	__inline static unsigned short Get22()	{ return S12AD.ADDR22; }
	__inline static unsigned short Get23()	{ return S12AD.ADDR23; }
	__inline static unsigned short Get24()	{ return S12AD.ADDR24; }
	__inline static unsigned short Get25()	{ return S12AD.ADDR25; }
	__inline static unsigned short Get26()	{ return S12AD.ADDR26; }
	__inline static unsigned short Get27()	{ return S12AD.ADDR27; }
	__inline static unsigned short Get28()	{ return S12AD.ADDR28; }
	__inline static unsigned short Get29()	{ return S12AD.ADDR29; }
	__inline static unsigned short Get30()	{ return S12AD.ADDR30; }
	__inline static unsigned short Get31()	{ return S12AD.ADDR31; }

	// �ϊ��g���K�̐ݒ�
	enum {
		TRG_TMR		= 0,
		TRG_ADTRG	= 1
	};
	__inline static void SelectTrigger(unsigned char flag);

	// �X�L�����I�������݋��̐ݒ�
	__inline static void EnableInterrupt(bool bEnable);

	// �X�L�������[�h(�A���X�L�������[�h)�̐ݒ�
	enum {
		SCAN_SINGLE		= false,
		SCAN_CONTINUOUS	= true
	};
	__inline static void SetScanMode(bool bContinuous);

	// A/D�ϊ��̊J�n�ƒ�~
	__inline static void Start();
	__inline static void Stop();

	// A/D�ϊ����̎擾
	__inline static bool IsConverting();

	// A/D�ϊ��`�����l���̐ݒ�
	enum {
		NO_CH	= 0x00000000,
		SCAN_0	= 0x00000001,
		SCAN_1	= 0x00000002,
		SCAN_2	= 0x00000004,
		SCAN_3	= 0x00000008,
		SCAN_4	= 0x00000010,
		SCAN_5	= 0x00000020,
		SCAN_6	= 0x00000040,
		SCAN_7	= 0x00000080,
		SCAN_16	= 0x00000100,
		SCAN_17	= 0x00000200,
		SCAN_18	= 0x00000400,
		SCAN_19	= 0x00000800,
		SCAN_20	= 0x00001000,
		SCAN_21	= 0x00002000,
		SCAN_22	= 0x00004000,
		SCAN_23	= 0x00008000,
		SCAN_24	= 0x00010000,
		SCAN_25	= 0x00020000,
		SCAN_26	= 0x00040000,
		SCAN_27	= 0x00080000,
		SCAN_28	= 0x00100000,
		SCAN_29	= 0x00200000,
		SCAN_30	= 0x00400000,
		SCAN_31	= 0x00800000
	};
	__inline static void SelectChannel(int ch);

	// �T���v�����O���Ԃ̐ݒ�
	__inline static void SelectSamplingState(int ch, unsigned int nState);

};

////////////////////////////////////////////////////////////////////////////////
#endif	// _SYS_OBJ_H_
