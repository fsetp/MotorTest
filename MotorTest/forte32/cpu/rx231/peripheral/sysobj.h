////////////////////////////////////////////////////////////////////////////////
// System Objects Control
//
// サポートクラス
//		CCpuObject			CPU特有情報管理オブジェクト
//		CModuleStopObject	モジュールストップコントローラ
//		CIcuObject			割込みコントローラ
//		CDioObject			I/Oポートコントローラ
//		CMpcObject			マルチファンクションピンコントローラ
//		CMtuObject			マルチファンクションタイマパルスユニット2コントローラ
//		CCmtObject			コンペアマッチタイマコントローラ
//		CIwdtObject			独立ウォッチドッグタイマコントローラ
//		CUsbObject			USB2.0ホスト／ファンクションモジュールコントローラ
//		CSciObject			シリアルコミュニケーションインタフェースコントローラ
//		CAdcS12Object		12ビットA/Dコンバータコントローラ
//

#ifndef _SYS_OBJ_H_
#define _SYS_OBJ_H_

#include <machine.h>
#include "iodefine.h"
//#define	NDEBUG				// この定義を行うと、アサーションブロックが削除される
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// CPU特有情報管理オブジェクト
//
class CCpuObject
{
public:
	CCpuObject()  {}
	~CCpuObject() {}

public:

	// CPUモードの取得
	__inline static unsigned short GetMode();

	// 内蔵ROMの設定と取得
	__inline static void EnableInternalRom(bool bEnable);
	__inline static bool IsEnableInternalRom();

	// 外部バスの設定と取得
	__inline static void EnableExtendBus(bool bEnable);
	__inline static bool IsEnableExtendBus();

	// 内蔵RAMの設定と取得
	__inline static void EnableInternalRam(bool bFlag);
	__inline static bool IsEnableInternalRam();

	// システムクロック分周比の設定と取得
	enum {
		ICLK_DIV1, ICLK_DIV2, ICLK_DIV4, ICLK_DIV8, ICLK_DIV16, ICLK_DIV32, ICLK_DIV64
	};
	__inline static void SelectSystemClock(unsigned char div);
	__inline static unsigned char GetSystemClock();

	// 周辺クロック分周比の設定と取得
	enum {
		PCLK_DIV1, PCLK_DIV2, PCLK_DIV4, PCLK_DIV8, PCLK_DIV16, PCLK_DIV32, PCLK_DIV64
	};
	__inline static void SelectPeripheralClockA(unsigned char div);
	__inline static unsigned char GetPeripheralClockA();
	__inline static void SelectPeripheralClockB(unsigned char div);
	__inline static unsigned char GetPeripheralClockB();
	__inline static void SelectPeripheralClockD(unsigned char div);
	__inline static unsigned char GetPeripheralClockD();

	// 外部バスクロック分周比の設定と取得
	enum {
		BCLK_DIV1, BCLK_DIV2, BCLK_DIV4, BCLK_DIV8, BCLK_DIV16, BCLK_DIV32, BCLK_DIV64
	};
	__inline static void SelectBusClock(unsigned char div);
	__inline static unsigned char GetBusClock();

	// FlashIFクロック分周比設定と取得
	enum {
		FCLK_DIV1, FCLK_DIV2, FCLK_DIV4, FCLK_DIV8, FCLK_DIV16, FCLK_DIV32, FCLK_DIV64
	};
	__inline static void SelectFlashIFClock(unsigned char div);
	__inline static unsigned char GetFlashIFClock();

	// 外部バスクロック出力停止の設定と取得
	__inline static void StopBusClock(bool bEnable);
	__inline static bool IsStopBusClock();

	// クロックソースの設定と取得
	enum {
		CLKSRC_LOCO, CLKSRC_HOCO, CLKSRC_MAINOSC, CLKSRC_SUBOSC, CLKSRC_PLL
	};
	__inline static void SelectClockSource(unsigned char src);
	__inline static unsigned char GetClockSource();

	// PLL入力分周比の設定と取得
	enum {
		PLL_DIV1, PLL_DIV2, PLL_DIV4
	};
	__inline static void SelectPllInputDivider(unsigned char div);
	__inline static unsigned char GetPllInputDivider();

	// PLL周波数逓倍率の設定と取得
	enum {
		PLL_x4    = 7,  PLL_x4_5 = 8,  PLL_x5    = 9,  PLL_x5_5  = 10, PLL_x6    = 11,
		PLL_x6_5  = 12, PLL_x7   = 13, PLL_x7_5  = 14, PLL_x8    = 15, PLL_x8_5  = 16,
		PLL_x9    = 17, PLL_x9_5 = 18, PLL_x10   = 19, PLL_x10_5 = 20, PLL_x11   = 21,
		PLL_x11_5 = 22, PLL_x12  = 23, PLL_x12_5 = 24, PLL_x13   = 25, PLL_x13_5 = 26
	};
	__inline static void SelectPllOutputMultiplier(unsigned char multi);
	__inline static unsigned char GetPllOutputMultiplier();

	// PLL停止の設定と取得
	__inline static void StopPll(bool bEnable);
	__inline static bool IsStopPll();

	// USB専用PLL入力分周比の設定と取得
	__inline static void SelectUsbPllInputDivider(unsigned char div);
	__inline static unsigned char GetUsbPllInputDivider();

	// USB専用PLL周波数逓倍率の設定と取得
	enum {
		USBPLL_x4 = 7, USBPLL_x6 = 11, USBPLL_x8 = 15, USBPLL_x12 = 23
	};
	__inline static void SelectUsbPllOutputMultiplier(unsigned char multi);
	__inline static unsigned char GetUsbPllOutputMultiplier();

	// UCLKソースUSB専用PLLの設定と取得
	enum {
		UCLK_SYSCLK, UCLK_USBPLL
	};
	__inline static void SelectUCLKSource(unsigned char sel);
	__inline static unsigned char GetUCLKSource();

	// USB専用PLL停止の設定と取得
	__inline static void StopUsbPll(bool bEnable);
	__inline static bool IsStopUsbPll();

	// BCLK出力の設定と取得
	__inline static void SelectBCLKDivider(unsigned char div);
	__inline static unsigned char GetBCLKDivider();

	// メインクロック発振器停止の設定と取得
	__inline static void StopMainClockOscillator(bool bEnable);
	__inline static bool IsStopMainClockOscillator();

	// サブクロック発振器停止の設定と取得
	__inline static void StopSubClockOscillator(bool bEnable);
	__inline static bool IsStopSubClockOscillator();

	// LOCO停止の設定と取得
	__inline static void StopLOCO(bool bEnable);
	__inline static bool IsStopLOCO();

	// IWDT専用オンチップオシレータ停止の設定と取得
	__inline static void StopIWDTOscillator(bool bEnable);
	__inline static bool IsStopIWDTOscillator();

	// HOCO停止の設定と取得
	__inline static void StopHOCO(bool bEnable);
	__inline static bool IsStopHOCO();

	// HOCO周波数の設定と取得
	enum {
		HOCO_32MHZ = 0, HOCO_54MHZ = 3
	};
	__inline static void SelectHOCOFrequency(unsigned char freq);
	__inline static unsigned char GetHOCOFrequency();

	// メインクロック発振安定の取得
	__inline static bool IsStabilizeMainClock();

	// PLLクロック発振安定の取得
	__inline static bool IsStabilizePll();

	// HOCOクロック発振安定の取得
	__inline static bool IsStabilizeHOCO();

	// USB専用PLLクロック発振安定の取得
	__inline static bool IsStabilizeUsbPll();

	// 発振停止検出割込み許可の設定と取得
	__inline static void EnableStopOscillationInterrupt(bool bEnable);
	__inline static bool IsEnableStopOscillationInterrupt();

	// 発振停止検出機能許可の設定と取得
	__inline static void EnableStopOscillationFunction(bool bEnable);
	__inline static bool IsEnableStopOscillationFunction();

	// メインクロック発振停止の取得
	__inline static bool IsDetectStopMainClock();

	// メインクロック発振器ウェイト時間の設定と取得
	enum {
		CYCLE_2,    CYCLE_1024,  CYCLE_2048,  CYCLE_4096,
		CYCLE_8192, CYCLE_16384, CYCLE_32768, CYCLE_65536
	};
	__inline static void SelectMainClockOscillatorWaitTime(unsigned char cycle);
	__inline static unsigned char GetMainClockOscillatorWaitTime();

	// CLKOUT出力ソースの設定と取得
	__inline static void SelectCLKOUTSource(unsigned char src);
	__inline static unsigned char GetCLKOUTSource();

	// CLKOUT出力分周比の設定と取得
	enum {
		CLKOUT_DIV1, CLKOUT_DIV2, CLKOUT_DIV4, CLKOUT_DIV8, CLKOUT_DIV16
	};
	__inline static void SelectCLKOUTDivider(unsigned char div);
	__inline static unsigned char GetCLKOUTDivider();

	// CLKOUT出力停止の設定と取得
	__inline static void StopCLKOUT(bool bEnable);
	__inline static bool IsStopCLKOUT();

	// メインクロック発振器の設定と取得(外部発振入力)
	__inline static void EnableExternalOscillationInput(bool bEnable);
	__inline static bool IsEnableExternalOscillationInput();

	// メモリウェイトサイクル許可の設定と取得
	__inline static void EnableMemoryWait(bool bEnable);
	__inline static bool IsEnableMemoryWait();

	// プロテクトレジスタ書き込みの設定
	__inline static void EnableWriteProtectRegister0(bool bEnable);
	__inline static void EnableWriteProtectRegister1(bool bEnable);
	__inline static void EnableWriteProtectRegister2(bool bEnable);
	__inline static void EnableWriteProtectRegister3(bool bEnable);

};

////////////////////////////////////////////////////////////////////////////////
// モジュールストップコントローラ
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

	// モジュールの開始
	__inline static void RunModule(int mod);

	// モジュールの停止
	__inline static void StopModule(int mod);

};


////////////////////////////////////////////////////////////////////////////////
// 割込みコントローラ
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

	// 割込み要求許可の設定
	__inline static void EnableInterrupt(unsigned int vect, bool bEnable);

	// 割込み要求許可状態の取得
	__inline static bool IsEnableInterrupt(unsigned int vect);

	// 割込み優先順位の設定
	enum {
		PLV0	= 0x00,		// 優先レベル0(最低)
		PLV1	= 0x01,		// 優先レベル1
		PLV2	= 0x02,		// 優先レベル2
		PLV3	= 0x03,		// 優先レベル3
		PLV4	= 0x04,		// 優先レベル4
		PLV5	= 0x05,		// 優先レベル5
		PLV6	= 0x06,		// 優先レベル6
		PLV7	= 0x07,		// 優先レベル7
		PLV8	= 0x08,		// 優先レベル8
		PLV9	= 0x09,		// 優先レベル9
		PLV10	= 0x0A,		// 優先レベル10
		PLV11	= 0x0B,		// 優先レベル11
		PLV12	= 0x0C,		// 優先レベル12
		PLV13	= 0x0D,		// 優先レベル13
		PLV14	= 0x0E,		// 優先レベル14
		PLV15	= 0x0F,		// 優先レベル15(最高)
	};
	__inline static void SetPriority(unsigned int vect, unsigned char nPriority);

	// IRQ割込み検出方法の設定
	enum {
		LOW_LEVEL, LOW_EDGE, HIGH_EDGE, BOTH_EDGE
	};
	__inline static void SetIrqSense(unsigned char irq, unsigned char nSense);

};


////////////////////////////////////////////////////////////////////////////////
// I/Oポートコントローラ
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

	// ポート方向レジスタの設定
	__inline static void SetDir(int port, int num, bool bOut);

	// ポート出力データレジスタの設定
	__inline static void SetData(int port, int num, bool bOn);

	// ポート入力データレジスタの取得
	__inline static bool GetLineStatus(int port, int num);

	// ポートモードレジスタの設定
	__inline static void SetModeFunc(int port, int num, bool bMode);

	// オープンドレイン制御レジスタの設定
	__inline static void SetOpenDrain(int port, int num, bool bOdr);

	// プルアップ制御レジスタの設定
	__inline static void SetInputPullup(int port, int num, bool bPullup);

	// ポート切替レジスタAの設定
	enum {
		PSRA_PB6 = 0x00, PSRA_PC0 = 0x40,
		PSRA_PB7 = 0x00, PSRA_PC1 = 0x80
	};
	__inline static void SetPSRA(unsigned char flag);

	// ポート切替レジスタBの設定
	enum {
		PSRB_PB0 = 0x00, PSRB_PC0 = 0x01,
		PSRB_PB1 = 0x00, PSRB_PC1 = 0x02,
		PSRB_PB3 = 0x00, PSRB_PC2 = 0x08,
		PSRB_PB5 = 0x00, PSRB_PC3 = 0x20
	};
	__inline static void SetPSRB(unsigned char flag);

};

////////////////////////////////////////////////////////////////////////////////
// マルチファンクションピンコントローラ
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
		HI_Z,																																		// ハイインピーダンス
		NO_IRQ,			IRQ0,			IRQ1,			IRQ2,			IRQ3,			IRQ4,			IRQ5,			IRQ6,			IRQ7,		// 割込み
		CLKOUT,																																		// クロック発生回路
		MTIOC0A,		MTIOC0B,		MTIOC0C,		MTIOC0D,																					// マルチファンクションタイマユニット2
		MTIOC1A,		MTIOC1B,
		MTIOC2A,		MTIOC2B,
		MTIOC3A,		MTIOC3B,		MTIOC3C,		MTIOC3D,
		MTIOC4A,		MTIOC4B,		MTIOC4C,		MTIOC4D,
		MTIC5U,			MTIC5V,			MTIC5W,
		MTCLKA,			MTCLKB,			MTCLKC,			MTCLKD,
		POE0,			POE1,			POE2,			POE3,			POE8,																		// ポートアウトプットイネーブル2
		TIOCA0,			TIOCB0,			TIOCC0,			TIOCD0,																						// 16ビットタイマパルスユニット
		TIOCA1,			TIOCB1,
		TIOCA2,			TIOCB2,
		TIOCA3,			TIOCB3,			TIOCC3,			TIOCD3,
		TIOCA4,			TIOCB4,
		TIOCA5,			TIOCB5,
		TCLKA,			TCLKB,			TCLKC,			TCLKD,
		TMO0,			TMCI0,			TMRI0,																										// 8ビットタイマ
		TMO1,			TMCI1,			TMRI1,
		TMO2,			TMCI2,			TMRI2,
		TMO3,			TMCI3,			TMRI3,
		RXD0,			RXD1,			RXD5,			RXD6,			RXD8,			RXD9,			RXD12,			RXDX12,						// シリアルコミュニケーションインタフェース
		SMISO0,			SMISO1,			SMISO5,			SMISO6,			SMISO8,			SMISO9,			SMISO12,
		SSCL0,			SSCL1,			SSCL5,			SSCL6,			SSCL8,			SSCL9,			SSCL12,
		TXD0,			TXD1,			TXD5,			TXD6,			TXD8,			TXD9,			TXD12,			TXDX12,
		SMOSI0,			SMOSI1,			SMOSI5,			SMOSI6,			SMOSI8,			SMOSI9,			SMOSI12,		SIOX12,
		SSDA0,			SSDA1,			SSDA5,			SSDA6,			SSDA8,			SSDA9,			SSDA12,
		SCK0,			SCK1,			SCK5,			SCK6,			SCK8,			SCK9,			SCK12,
		CTS0,			CTS1,			CTS5,			CTS6,			CTS8,			CTS9,			CTS12,
		RTS0,			RTS1,			RTS5,			RTS6,			RTS8,			RTS9,			RTS12,
		SS0,			SS1,			SS5,			SS6,			SS8,			SS9,			SS12,
		SCL,			SDA,																														// I2Cバスインタフェース
		RSPCKA,			MOSIA,			MISOA,																										// シリアルペリフェラルインタフェース
		SSLA0,			SSLA1,			SSLA2,			SSLA3,
		RTCOUT,			RTCIC0,			RTCIC1,			RTCIC2,																						// リアルタイムクロック
		IRTXD5,			IRRXD5,																														// IrDAインタフェース
		CRXD0,			CTXD0,																														// CAN
		SSISCK0,		SSIWS0,			SSITXD0,		SSIRXD0,		AUDIO_MCLK,																	// シリアルサウンドインタフェース
		SDHI_CLK,		SDHI_CMD,		SDHI_CD,		SDHI_WP,																					// SDホストインタフェース
		SDHI_D0,		SDHI_D1,		SDHI_D2,		SDHI_D3,
		USB0_VBUS,		USB0_EXICEN,	USB0_VBUSEN,	USB0_OVRCURA,	USB0_OVRCURB,	USB0_ID,													// USB2.0ホスト／ファンクションモジュール
		NO_AD,			AN000,			AN001,			AN002,			AN003,			AN004,			AN005,			AN006,			AN007,		// 12ビットADコンバータ
		AN016,			AN017,			AN018,			AN019,			AN020,			AN021,			AN022,			AN023,			AN024,
		AN025,			AN026,			AN027,			AN028,			AN029,			AN030,			AN031,
		ADTRG0,
		NO_DA,			DA0,			DA1,																										// DAコンバータ
		CACREF,																																		// クロック周波数精度測定回路
		CMPA2,																																		// LVD電圧検出入力
		CMPB0,			CMPB1,			CMPB2,			CMPB3,																						// コンパレータB
		CVREFB0,		CVREFB1,		CVREFB2,		CVREFB3,
		CMPOB0,			CMPOB1,			CMPOB2,			CMPOB3,
		TSCAP,																																		// 静電容量タッチセンサ
		TS0,			TS1,			TS2,			TS3,			TS4,			TS5,			TS6,			TS7,
		TS8,			TS9,			TS12,			TS13,			TS15,			TS16,			TS17,			TS18,
		TS19,			TS20,			TS22,			TS23,			TS27,			TS30,			TS33,			TS35,
	};

protected:

	// プロテクトレジスタ書き込みの設定
	__inline static void _EnableWriteForPFS(bool bEnable);

public:

	// ポートファンクションの設定
	__inline static void SetPortFunction(PORTSEL ps, PORTFUNC pf);

	// 周辺機能の設定
	__inline static void UsePeripheralPort(PORTSEL ps, int num, bool bEnable);

};

////////////////////////////////////////////////////////////////////////////////
// マルチファンクションタイマパルスユニット2コントローラ
//
class CMtuObject
{
public:
	CMtuObject()  {}
	~CMtuObject() {}

public:

	// MTUモジュールの設定
	enum {
		MTU_U0, MTU_UNIT_NUM
	};
	__inline static void RunModule(unsigned int nUnit, bool bRun);

	// TCRの設定
	enum {
		TCR_IN_1		= 0x0000,		// 内部クロック：PCLK/1でカウント
		TCR_IN_4		= 0x0001,		// 内部クロック：PCLK/4でカウント
		TCR_IN_16		= 0x0002,		// 内部クロック：PCLK/16でカウント
		TCR_IN_64		= 0x0003,		// 内部クロック：PCLK/64でカウント
		TCR_IN_256		= 0x0004,		// 内部クロック：PCLK/256でカウント
		TCR_IN_1024		= 0x0005,		// 内部クロック：PCLK/1024でカウント
		TCR_MTCLKA		= 0x0006,		// 外部クロック：MTCLKA端子入力でカウント
		TCR_MTCLKB		= 0x0007,		// 外部クロック：MTCLKB端子入力でカウント
		TCR_MTCLKC		= 0x0008,		// 外部クロック：MTCLKC端子入力でカウント
		TCR_MTCLKD		= 0x0009,		// 外部クロック：MTCLKD端子入力でカウント
		TCR_TCNT		= 0x000A		// TCNTnのオーバフロー／アンダフローでカウント
	};
	enum {
		TCR_HIGH_EDGE	= 0x0000,		// 立ち上がりエッジでカウント
		TCR_LOW_EDGE	= 0x0010,		// 立ち下がりエッジでカウント
		TCR_BOTH_EDGE	= 0x0020		// 両エッジでカウント
	};
	enum {
		TCR_DISABLE		= 0x0000,		// TCNTのクリア禁止
		TCR_TGRA		= 0x0100,		// TGRAのコンペアマッチ／インプットキャプチャでTCNTクリア
		TCR_TGRB		= 0x0200,		// TGRBのコンペアマッチ／インプットキャプチャでTCNTクリア
		TCR_TGRC		= 0x0300,		// TGRCのコンペアマッチ／インプットキャプチャでTCNTクリア
		TCR_TGRD		= 0x0400,		// TGRDのコンペアマッチ／インプットキャプチャでTCNTクリア
		TCR_OTHER		= 0x0500		// 同期クリア／同期動作をしている他のチャネルのカウンタクリアでTCNTをクリア
	};
	__inline static void SetControl(int ch, unsigned int flag, unsigned int flagv = 0, unsigned int flagw = 0);

	// TMDRの設定
	enum {
		TMDR_NORMAL		= 0x00,			// ノーマルモード
		TMDR_PWM1		= 0x02,			// PWMモード1
		TMDR_PWM2		= 0x03,			// PWMモード2
		TMDR_PHASE1		= 0x04,			// 位相計数モード1
		TMDR_PHASE2		= 0x05,			// 位相計数モード2
		TMDR_PHASE3		= 0x06,			// 位相計数モード3
		TMDR_PHASE4		= 0x07,			// 位相計数モード4
		TMDR_RPWM1		= 0x08,			// リセット同期PWMモード
		TMDR_DPWM1		= 0x0D,			// 相補PWMモード1(山で転送)
		TMDR_DPWM2		= 0x0E,			// 相補PWMモード2(谷で転送)
		TMDR_DPWM3		= 0x0F			// 相補PWMモード3(山・谷で転送)
	};
	enum {
		TMDR_BFA		= 0x10,			// TGRAとTGRCはバッファ動作
		TMDR_BFB		= 0x20,			// TGRBとTGRDはバッファ動作
		TMDR_BFE		= 0x40			// TGREとTGRFはバッファ動作
	};
	__inline static void SetMode(int ch, unsigned char mode);

	// TIORの設定
	enum {
		TIOR_TIOCA_DISABLE		= 0x00,	// 出力禁止
		TIOR_TIOCA_CMATCH_LOW	= 0x01,	// コンペアマッチでLow出力
		TIOR_TIOCA_CMATCH_HIGH	= 0x02,	// コンペアマッチでHigh出力
		TIOR_TIOCA_CMATCH_TGL	= 0x03,	// コンペアマッチでトグル出力
		TIOR_TIOCA_DEFOUT_LOW	= 0x00,	// 初期出力Low
		TIOR_TIOCA_DEFOUT_HIGH	= 0x04,	// 初期出力High

		TIOR_TIOCB_DISABLE		= 0x00,	// 出力禁止
		TIOR_TIOCB_CMATCH_LOW	= 0x01,	// コンペアマッチでLow出力
		TIOR_TIOCB_CMATCH_HIGH	= 0x02,	// コンペアマッチでHigh出力
		TIOR_TIOCB_CMATCH_TGL	= 0x03,	// コンペアマッチでトグル出力
		TIOR_TIOCB_DEFOUT_LOW	= 0x00,	// 初期出力Low
		TIOR_TIOCB_DEFOUT_HIGH	= 0x04,	// 初期出力High

		TIOR_TIOCC_DISABLE		= 0x00,	// 出力禁止
		TIOR_TIOCC_CMATCH_LOW	= 0x01,	// コンペアマッチでLow出力
		TIOR_TIOCC_CMATCH_HIGH	= 0x02,	// コンペアマッチでHigh出力
		TIOR_TIOCC_CMATCH_TGL	= 0x03,	// コンペアマッチでトグル出力
		TIOR_TIOCC_DEFOUT_LOW	= 0x00,	// 初期出力Low
		TIOR_TIOCC_DEFOUT_HIGH	= 0x04,	// 初期出力High

		TIOR_TIOCD_DISABLE		= 0x00,	// 出力禁止
		TIOR_TIOCD_CMATCH_LOW	= 0x01,	// コンペアマッチでLow出力
		TIOR_TIOCD_CMATCH_HIGH	= 0x02,	// コンペアマッチでHigh出力
		TIOR_TIOCD_CMATCH_TGL	= 0x03,	// コンペアマッチでトグル出力
		TIOR_TIOCD_DEFOUT_LOW	= 0x00,	// 初期出力Low
		TIOR_TIOCD_DEFOUT_HIGH	= 0x04,	// 初期出力High

		TIOR_TIOCA_CAPTURE_HIGH	= 0x08,	// インプットキャプチャ(立ち上がりエッジ)
		TIOR_TIOCA_CAPTURE_LOW	= 0x09,	// インプットキャプチャ(立ち下がりエッジ)
		TIOR_TIOCA_CAPTURE_BOTH	= 0x0A,	// インプットキャプチャ(両エッジ)
		TIOR_TIOCA_CAPTURE_ICLK	= 0x0B,	// インプットキャプチャ(1/カウントクロック)

		TIOR_TIOCB_CAPTURE_HIGH	= 0x08,	// インプットキャプチャ(立ち上がりエッジ)
		TIOR_TIOCB_CAPTURE_LOW	= 0x09,	// インプットキャプチャ(立ち下がりエッジ)
		TIOR_TIOCB_CAPTURE_BOTH	= 0x0A,	// インプットキャプチャ(両エッジ)
		TIOR_TIOCB_CAPTURE_ICLK	= 0x0B	// インプットキャプチャ(1/カウントクロック)
	};
	__inline static void SetIOControl(int ch, unsigned char tgra, unsigned char tgrb, unsigned char tgrc, unsigned char tgrd);

	// タイマ割込み許可の設定
	enum {
		TIER_TGIEA	= 0x01,				// 割り込み要求(TGIA)の許可
		TIER_TGIEB	= 0x02,				// 割り込み要求(TGIB)の許可
		TIER_TGIEC	= 0x04,				// 割り込み要求(TGIC)の許可
		TIER_TGIED	= 0x08,				// 割り込み要求(TGID)の許可
		TIER_TGIEV	= 0x10,				// 割り込み要求(TCIV)の許可
		TIER_TGIEU	= 0x20,				// 割り込み要求(TCIU)の許可
		TIER_TTGE2	= 0x40,				// TCNTカウンタのアンダフロー(谷)によるA/D変換要求の許可
		TIER_TTGE	= 0x80				// A/D変換開始要求の発生の許可
	};
	enum {
		TIER_TGIE5W	= 0x01,				// TGI5W割り込み要求の許可
		TIER_TGIE5V	= 0x02,				// TGI5V割り込み要求の許可
		TIER_TGIE5U	= 0x04				// TGI5U割り込み要求の許可
	};
	enum {
		TIER2_TGIEE	= 0x01,				// 割り込み要求(TGIE)の許可
		TIER2_TGIEF	= 0x02				// 割り込み要求(TGIF)の許可
	};
	__inline static void EnableInterrupt(int ch, unsigned char tier, unsigned char tier2 = 0);

	// タイマステータスフラグの設定と取得
	enum {
		TSR_TC_DN = 0x00,
		TSR_TC_UP = 0x80
	};
	__inline static void SetTSR(int ch, unsigned char flag);
	__inline static bool GetTSR(int ch, unsigned char flag);

	// タイマカウンタの設定と取得
	__inline static void SetTCNT(int ch, unsigned int tcnt, unsigned int tcntv = 0, unsigned int tcntw = 0);
	__inline static void GetTCNT(int ch, unsigned int &tcnt, unsigned int &tcntv, unsigned int &tcntw);

	// タイマジェネラルレジスタの設定と取得
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

	// タイマスタートとタイマストップ
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

	// タイマシンクロレジスタの設定
	__inline static void SyncEnable(int ch, bool bEnable);

};

////////////////////////////////////////////////////////////////////////////////
// コンペアマッチタイマコントローラ
//
class CCmtObject
{
public:
	CCmtObject()  {}
	~CCmtObject() {}

public:

	// CMTモジュールの設定
	enum {
		CMT_U0, CMT_U1, CMT_UNIT_NUM
	};
	__inline static void RunModule(unsigned int nUnit, bool bRun);

	// インターバルタイマの設定
	__inline static bool SetIntervalTimer(int ch, int nMHz, int ms);

	// カウント動作の開始
	__inline static void StartCounter(int ch, bool bEnable);

	// クロックの設定
	enum {
		PCLK_1_8, PCLK_1_32, PCLK_1_128, PCLK_1_512
	};
	__inline static void SelectClock(int ch, int cks);

	// コンペアマッチ割込み許可の設定
	__inline static void EnableInterrupt(int ch, bool bEnable);

	// カウンタの設定と取得
	__inline static void SetCounter(int ch, int cnt);
	__inline static int GetCounter(int ch);

	// インターバルの設定と取得
	__inline static void SetInterval(int ch, int cnt);
	__inline static int GetInterval(int ch);

};

////////////////////////////////////////////////////////////////////////////////
// 独立ウォッチドッグタイマコントローラ
//
class CIwdtObject
{
public:
	CIwdtObject()  {}
	~CIwdtObject() {}

public:

	// カウンタのリフレッシュ
	__inline static void RefreshCounter();

	// タイムアウト期間の設定
	enum {
		CYCLE_128, CYCLE_512, CYCLE_1024, CYCLE_2048
	};
	__inline static void SetTimeoutPeriod(int cycle);

	// クロックの設定
	enum {
		IWDTCLK, IWDTCLK_1_16, IWDTCLK_1_32, IWDTCLK_1_64, IWDTCLK_1_128, IWDTCLK_1_256
	};
	__inline static void SelectClock(int cks);

	// ウィンドウ終了位置の設定
	enum {
		END_POS_75, END_POS_50, END_POS_25, END_POS_0
	};
	__inline static void SetWindowEndPosition(int pos);

	// ウィンドウ開始位置の設定
	enum {
		START_POS_25, START_POS_50, START_POS_75, START_POS_100
	};
	__inline static void SetWindowStartPosition(int pos);

	// カウンタの取得
	__inline static int GetCounter();

	// アンダーフローフラグのクリアと取得
	__inline static void ClearUnderflowFlag();
	__inline static bool GetUnderflowFlag();

	// リフレッシュエラーフラグのクリアと取得
	__inline static void ClearRefreshErrorFlag();
	__inline static bool GetRefreshErrorFlag();

	// ノンマスカブル割込み許可の設定
	__inline static void EnableNonMaskableInterrupt(bool bEnable);

	// カウンタ停止の設定(低消費電力状態時)
	__inline static void StopSleepModeCount(bool bEnable);

};

////////////////////////////////////////////////////////////////////////////////
// USB2.0ホスト／ファンクションモジュールコントローラ
//
class CUsbObject
{
public:
	CUsbObject()  {}
	~CUsbObject() {}

public:

	// USBモジュールの設定
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

	// PIPEnCTR	n = 1 ～ 9
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

	// PIPEnTRE	n = 1 ～ 5
	static void ClearPipeTransactionCounter(int ch, int nPipe, bool bEnable);
	static void EnablePipeTransactionCounter(int ch, int nPipe, bool bEnable);

	// PIPEnTRN	n = 1 ～ 5
	static int GetPipeTransactionCounter(int ch, int nPipe);
	static void SetPipeTransactionCounter(int ch, int nPipe, int cnt);

	// DEVADDn	n = 0 ～ 5
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
// シリアルコミュニケーションインタフェースコントローラ
//
class CSciObject
{
public:
	CSciObject()  {}
	~CSciObject() {}

public:

	// SCIモジュールの設定
	__inline static void RunModule(int ch, bool bRun);

	// 受信データの取得
	__inline static unsigned char GetReceiveData(int ch);
	__inline static unsigned char GetReceiveDataLow(int ch);
	__inline static unsigned char GetReceiveDataHigh(int ch);

	// 送信データの設定
	__inline static void WriteTransmitData(int ch, unsigned char data);
	__inline static void WriteTransmitDataLow(int ch, unsigned char data);
	__inline static void WriteTransmitDataHigh(int ch, unsigned char data);

	// シリアルモードレジスタの設定
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

	// シリアルコントロールレジスタの設定
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

	// 受信割込み要求許可の設定
	__inline static void EnableReceiveInterrupt(int ch, bool bEnable);

	// 受信割込み要求許可の取得
	__inline static bool IsEnableReceiveInterrupt(int ch);

	// 受信動作許可の設定
	__inline static void EnableRE(int ch, bool bEnable);

	// 送信割込み要求許可の設定
	__inline static void EnableTransmitInterrupt(int ch, bool bEnable);

	// 送信割込み要求許可の取得
	__inline static bool IsEnableTransmitInterrupt(int ch);

	// 送信動作許可の設定
	__inline static void EnableTE(int ch, bool bEnable);

	// シリアルステータスレジスタの取得
	__inline static unsigned char GetSSR(int ch);

	// シリアルステータスレジスタTENDの取得
	__inline static bool GetTEND(int ch);

	// シリアルステータスRDRFのクリアと取得
	__inline static void ClearRDRF(int ch);
	__inline static bool GetRDRF(int ch);

	// シリアルステータスレジスタTDREのクリアと取得
	__inline static void ClearTDRE(int ch);
	__inline static bool GetTDRE(int ch);

	// エラーフラグの取得
	__inline static unsigned char GetErrorFlag(int ch);

	// スマートカードモードレジスタの設定
	enum {
		SCMR_SMIF = 0x01,
		SCMR_SINV = 0x04,
		SCMR_SDIR = 0x08,
		SCMR_CHR1 = 0x10,
		SCMR_BCP2 = 0x80
	};
	__inline static void SetSCMR(int ch, unsigned char flag);

	// ビットレートレジスタの設定
	__inline static void SetBRR(int ch, unsigned char br);

};

////////////////////////////////////////////////////////////////////////////////
// 12ビットA/Dコンバータコントローラ
//
class CAdcS12Object
{
public:
	CAdcS12Object()  {}
	~CAdcS12Object() {}

public:

	// S12ADEモジュールの設定
	__inline static void RunModule(bool bRun);

	// A/D変換結果の取得
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

	// 変換トリガの設定
	enum {
		TRG_TMR		= 0,
		TRG_ADTRG	= 1
	};
	__inline static void SelectTrigger(unsigned char flag);

	// スキャン終了割込み許可の設定
	__inline static void EnableInterrupt(bool bEnable);

	// スキャンモード(連続スキャンモード)の設定
	enum {
		SCAN_SINGLE		= false,
		SCAN_CONTINUOUS	= true
	};
	__inline static void SetScanMode(bool bContinuous);

	// A/D変換の開始と停止
	__inline static void Start();
	__inline static void Stop();

	// A/D変換中の取得
	__inline static bool IsConverting();

	// A/D変換チャンネルの設定
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

	// サンプリング時間の設定
	__inline static void SelectSamplingState(int ch, unsigned int nState);

};

////////////////////////////////////////////////////////////////////////////////
#endif	// _SYS_OBJ_H_
