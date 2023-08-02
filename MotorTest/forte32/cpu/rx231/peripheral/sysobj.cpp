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
//	2023.06.18	CIcuObjectの関数が未使用にされていたのを復活
//				IRレジスタ制御関数を追加

#include "sysobj.h"

////////////////////////////////////////////////////////////////////////////////
// CPU特有情報管理オブジェクト
//

////////////////////////////////////////
// CPUモードの取得
//
unsigned short CCpuObject::GetMode()
{
	return SYSTEM.MDMONR.WORD;
}

////////////////////////////////////////
// 内蔵ROMの設定
//	bEnable	: true(許可) ／ false(禁止)
//
void CCpuObject::EnableInternalRom(bool bEnable)
{
	volatile unsigned short reg = SYSTEM.SYSCR0.WORD;
	reg |= 0x5A00 | (bEnable ? 0x0001 : 0x0000);
	SYSTEM.SYSCR0.WORD = reg;
}

////////////////////////////////////////
// 内蔵ROMの取得
//
bool CCpuObject::IsEnableInternalRom()
{
	return SYSTEM.SYSCR0.BIT.ROME;
}

////////////////////////////////////////
// 外部バスの設定
//	bEnable	: true(許可) ／ false(禁止)
//
void CCpuObject::EnableExtendBus(bool bEnable)
{
	volatile unsigned short reg = SYSTEM.SYSCR0.WORD;
	reg |= 0x5A00 | (bEnable ? 0x0002 : 0x0000);
	SYSTEM.SYSCR0.WORD = reg;
}

////////////////////////////////////////
// 外部バスの取得
//
bool CCpuObject::IsEnableExtendBus()
{
	return SYSTEM.SYSCR0.BIT.EXBE;
}

////////////////////////////////////////
// 内蔵RAMの設定
//	bEnable	: true(許可) ／ false(禁止)
//
void CCpuObject::EnableInternalRam(bool bEnable)
{
	SYSTEM.SYSCR1.BIT.RAME = bEnable ? 1 : 0;
}

////////////////////////////////////////
// 内蔵RAMの取得
//
bool CCpuObject::IsEnableInternalRam()
{
	return SYSTEM.SYSCR1.BIT.RAME;
}

////////////////////////////////////////
// システムクロック分周比の設定
//	div		: 分周比
//
void CCpuObject::SelectSystemClock(unsigned char div)
{
	switch (div) {
		case ICLK_DIV1:
		case ICLK_DIV2:
		case ICLK_DIV4:
		case ICLK_DIV8:
		case ICLK_DIV16:
		case ICLK_DIV32:
		case ICLK_DIV64:
		{
			SYSTEM.SCKCR.BIT.ICK = div;
			break;
		}
	}
}

////////////////////////////////////////
// システムクロック分周比の取得
//
unsigned char CCpuObject::GetSystemClock()
{
	return SYSTEM.SCKCR.BIT.ICK;
}

////////////////////////////////////////
// 周辺クロックA分周比の設定
//	div		: 分周比
//
void CCpuObject::SelectPeripheralClockA(unsigned char div)
{
	switch (div) {
		case PCLK_DIV1:
		case PCLK_DIV2:
		case PCLK_DIV4:
		case PCLK_DIV8:
		case PCLK_DIV16:
		case PCLK_DIV32:
		case PCLK_DIV64:
		{
			SYSTEM.SCKCR.BIT.PCKA = div;
			break;
		}
	}
}

////////////////////////////////////////
// 周辺クロックA分周比の取得
//
unsigned char CCpuObject::GetPeripheralClockA()
{
	return SYSTEM.SCKCR.BIT.PCKA;
}

////////////////////////////////////////
// 周辺クロックB分周比の設定
//	div		: 分周比
//
void CCpuObject::SelectPeripheralClockB(unsigned char div)
{
	switch (div) {
		case PCLK_DIV1:
		case PCLK_DIV2:
		case PCLK_DIV4:
		case PCLK_DIV8:
		case PCLK_DIV16:
		case PCLK_DIV32:
		case PCLK_DIV64:
		{
			SYSTEM.SCKCR.BIT.PCKB = div;
			break;
		}
	}
}

////////////////////////////////////////
// 周辺クロックB分周比の取得
//
unsigned char CCpuObject::GetPeripheralClockB()
{
	return SYSTEM.SCKCR.BIT.PCKB;
}

////////////////////////////////////////
// 周辺クロックD分周比の設定
//	div		: 分周比
//
void CCpuObject::SelectPeripheralClockD(unsigned char div)
{
	switch (div) {
		case PCLK_DIV1:
		case PCLK_DIV2:
		case PCLK_DIV4:
		case PCLK_DIV8:
		case PCLK_DIV16:
		case PCLK_DIV32:
		case PCLK_DIV64:
		{
			SYSTEM.SCKCR.BIT.PCKD = div;
			break;
		}
	}
}

////////////////////////////////////////
// 周辺クロックD分周比の取得
//
unsigned char CCpuObject::GetPeripheralClockD()
{
	return SYSTEM.SCKCR.BIT.PCKD;
}

////////////////////////////////////////
// 外部バスクロック分周比の設定
//	div		: 分周比
//
void CCpuObject::SelectBusClock(unsigned char div)
{
	switch (div) {
		case BCLK_DIV1:
		case BCLK_DIV2:
		case BCLK_DIV4:
		case BCLK_DIV8:
		case BCLK_DIV16:
		case BCLK_DIV32:
		case BCLK_DIV64:
		{
			SYSTEM.SCKCR.BIT.BCK = div;
			break;
		}
	}
}

////////////////////////////////////////
// 外部バスクロック分周比の取得
//
unsigned char CCpuObject::GetBusClock()
{
	return SYSTEM.SCKCR.BIT.BCK;
}

////////////////////////////////////////
// FlashIFクロック分周比の設定
//	div		: 分周比
//
void CCpuObject::SelectFlashIFClock(unsigned char div)
{
	switch (div) {
		case FCLK_DIV1:
		case FCLK_DIV2:
		case FCLK_DIV4:
		case FCLK_DIV8:
		case FCLK_DIV16:
		case FCLK_DIV32:
		case FCLK_DIV64:
		{
			SYSTEM.SCKCR.BIT.FCK = div;
			break;
		}
	}
}

////////////////////////////////////////
// FlashIFクロック分周比の取得
//
unsigned char CCpuObject::GetFlashIFClock()
{
	return SYSTEM.SCKCR.BIT.FCK;
}

////////////////////////////////////////
// 外部バスクロック出力停止の設定
//	bEnable	: true(停止) ／ false(動作)
//
void CCpuObject::StopBusClock(bool bEnable)
{
	SYSTEM.SCKCR.BIT.PSTOP1 = bEnable ? 1 : 0;
}

////////////////////////////////////////
// 外部バスクロック出力停止の取得
//
bool CCpuObject::IsStopBusClock()
{
	return SYSTEM.SCKCR.BIT.PSTOP1;
}

////////////////////////////////////////
// クロックソースの設定
//	src		: クロックソース
//
void CCpuObject::SelectClockSource(unsigned char src)
{
	switch (src) {
		case CLKSRC_LOCO:
		case CLKSRC_HOCO:
		case CLKSRC_MAINOSC:
		case CLKSRC_SUBOSC:
		case CLKSRC_PLL:
		{
			SYSTEM.SCKCR3.BIT.CKSEL = src;
			break;
		}
	}
}

////////////////////////////////////////
// クロックソースの取得
//
unsigned char CCpuObject::GetClockSource()
{
	return SYSTEM.SCKCR3.BIT.CKSEL;
}

////////////////////////////////////////
// PLL入力分周比の設定
//	div		: 分周比
//
void CCpuObject::SelectPllInputDivider(unsigned char div)
{
	switch (div) {
		case PLL_DIV1:
		case PLL_DIV2:
		case PLL_DIV4:
		{
			SYSTEM.PLLCR.BIT.PLIDIV = div;
			break;
		}
	}
}

////////////////////////////////////////
// PLL入力分周比の取得
//
unsigned char CCpuObject::GetPllInputDivider()
{
	return SYSTEM.PLLCR.BIT.PLIDIV;
}

////////////////////////////////////////
// PLL周波数逓倍率の設定
//	multi	: 逓倍比
//
void CCpuObject::SelectPllOutputMultiplier(unsigned char multi)
{
	switch (multi) {
		case PLL_x4:
		case PLL_x4_5:
		case PLL_x5:
		case PLL_x5_5:
		case PLL_x6:
		case PLL_x6_5:
		case PLL_x7:
		case PLL_x7_5:
		case PLL_x8:
		case PLL_x8_5:
		case PLL_x9:
		case PLL_x9_5:
		case PLL_x10:
		case PLL_x10_5:
		case PLL_x11:
		case PLL_x11_5:
		case PLL_x12:
		case PLL_x12_5:
		case PLL_x13:
		case PLL_x13_5:
		{
			SYSTEM.PLLCR.BIT.STC = multi;
			break;
		}
	}
}

////////////////////////////////////////
// PLL周波数逓倍率の取得
//
unsigned char CCpuObject::GetPllOutputMultiplier()
{
	return SYSTEM.PLLCR.BIT.STC;
}

////////////////////////////////////////
// PLL停止の設定
//	bEnable	: true(停止) ／ false(動作)
//
void CCpuObject::StopPll(bool bEnable)
{
	SYSTEM.PLLCR2.BIT.PLLEN = bEnable ? 1 : 0;
}

////////////////////////////////////////
// PLL停止の取得
//
bool CCpuObject::IsStopPll()
{
	return SYSTEM.PLLCR2.BIT.PLLEN;
}

////////////////////////////////////////
// USB専用PLL入力分周比の設定
//	div		: 分周比
//
void CCpuObject::SelectUsbPllInputDivider(unsigned char div)
{
	switch (div) {
		case PLL_DIV1:
		case PLL_DIV2:
		case PLL_DIV4:
		{
			SYSTEM.UPLLCR.BIT.UPLIDIV = div;
			break;
		}
	}
}

////////////////////////////////////////
// USB専用PLL入力分周比の取得
//
unsigned char CCpuObject::GetUsbPllInputDivider()
{
	return SYSTEM.UPLLCR.BIT.UPLIDIV;
}

////////////////////////////////////////
// USB専用PLL周波数逓倍率の設定
//	multi	: 逓倍比
//
void CCpuObject::SelectUsbPllOutputMultiplier(unsigned char multi)
{
	switch (multi) {
		case USBPLL_x4:
		case USBPLL_x6:
		case USBPLL_x8:
		case USBPLL_x12:
		{
			SYSTEM.UPLLCR.BIT.USTC = multi;
			break;
		}
	}
}

////////////////////////////////////////
// USB専用PLL周波数逓倍率の取得
//
unsigned char CCpuObject::GetUsbPllOutputMultiplier()
{
	return SYSTEM.UPLLCR.BIT.USTC;
}

////////////////////////////////////////
// UCLKソースUSB専用PLLの設定
//	sel		: UCLKソース
//
void CCpuObject::SelectUCLKSource(unsigned char sel)
{
	switch (sel) {
		case UCLK_SYSCLK:
		case UCLK_USBPLL:
		{
			SYSTEM.UPLLCR.BIT.UCKUPLLSEL = sel;
			break;
		}
	}
}

////////////////////////////////////////
// UCLKソースUSB専用PLLの取得
//
unsigned char CCpuObject::GetUCLKSource()
{
	return SYSTEM.UPLLCR.BIT.UCKUPLLSEL;
}

////////////////////////////////////////
// USB専用PLL停止の設定
//	bEnable	: true(停止) ／ false(動作)
//
void CCpuObject::StopUsbPll(bool bEnable)
{
	SYSTEM.UPLLCR2.BIT.UPLLEN = bEnable ? 1 : 0;
}

////////////////////////////////////////
// USB専用PLL停止の取得
//
bool CCpuObject::IsStopUsbPll()
{
	return SYSTEM.UPLLCR2.BIT.UPLLEN;
}

////////////////////////////////////////
// BCLK出力の設定
//	div		: 分周比
//
void CCpuObject::SelectBCLKDivider(unsigned char div)
{
	switch (div) {
		case BCLK_DIV1:
		case BCLK_DIV2:
		{
			SYSTEM.BCKCR.BIT.BCLKDIV = div;
			break;
		}
	}
}

////////////////////////////////////////
// BCLK出力の取得
//
unsigned char CCpuObject::GetBCLKDivider()
{
	return SYSTEM.BCKCR.BIT.BCLKDIV;
}

////////////////////////////////////////
// メインクロック発振器停止の設定
//	bEnable	: true(停止) ／ false(動作)
//
void CCpuObject::StopMainClockOscillator(bool bEnable)
{
	SYSTEM.MOSCCR.BIT.MOSTP = bEnable ? 1 : 0;
}

////////////////////////////////////////
// メインクロック発振器停止の取得
//
bool CCpuObject::IsStopMainClockOscillator()
{
	return SYSTEM.MOSCCR.BIT.MOSTP;
}

////////////////////////////////////////
// サブクロック発振器停止の設定
//	bEnable	: true(停止) ／ false(動作)
//
void CCpuObject::StopSubClockOscillator(bool bEnable)
{
	SYSTEM.SOSCCR.BIT.SOSTP = bEnable ? 1 : 0;
}

////////////////////////////////////////
// サブクロック発振器停止の取得
//
bool CCpuObject::IsStopSubClockOscillator()
{
	return SYSTEM.SOSCCR.BIT.SOSTP;
}

////////////////////////////////////////
// LOCO停止の設定
//	bEnable	: true(停止) ／ false(動作)
//
void CCpuObject::StopLOCO(bool bEnable)
{
	SYSTEM.LOCOCR.BIT.LCSTP = bEnable ? 1 : 0;
}

////////////////////////////////////////
// LOCO停止の取得
//
bool CCpuObject::IsStopLOCO()
{
	return SYSTEM.LOCOCR.BIT.LCSTP;
}

////////////////////////////////////////
// IWDT専用オンチップオシレータ停止の設定
//	bEnable	: true(停止) ／ false(動作)
//
void CCpuObject::StopIWDTOscillator(bool bEnable)
{
	SYSTEM.ILOCOCR.BIT.ILCSTP = bEnable ? 1 : 0;
}

////////////////////////////////////////
// IWDT専用オンチップオシレータ停止の取得
//
bool CCpuObject::IsStopIWDTOscillator()
{
	return SYSTEM.ILOCOCR.BIT.ILCSTP;
}

////////////////////////////////////////
// HOCO停止の設定
//	bEnable	: true(停止) ／ false(動作)
//
void CCpuObject::StopHOCO(bool bEnable)
{
	SYSTEM.HOCOCR.BIT.HCSTP = bEnable ? 1 : 0;
}

////////////////////////////////////////
// HOCO停止の取得
//
bool CCpuObject::IsStopHOCO()
{
	return SYSTEM.HOCOCR.BIT.HCSTP;
}

////////////////////////////////////////
// HOCO周波数の設定
//	freq	: 周波数
//
void CCpuObject::SelectHOCOFrequency(unsigned char freq)
{
	switch (freq) {
		case HOCO_32MHZ:
		case HOCO_54MHZ:
		{
			SYSTEM.HOCOCR2.BIT.HCFRQ = freq;
			break;
		}
	}
}

////////////////////////////////////////
// HOCO周波数の取得
//
unsigned char CCpuObject::GetHOCOFrequency()
{
	return SYSTEM.HOCOCR2.BIT.HCFRQ;
}

////////////////////////////////////////
// メインクロック発振安定の取得
//
bool CCpuObject::IsStabilizeMainClock()
{
	return SYSTEM.OSCOVFSR.BIT.MOOVF;
}

////////////////////////////////////////
// PLLクロック発振安定の取得
//
bool CCpuObject::IsStabilizePll()
{
	return SYSTEM.OSCOVFSR.BIT.PLOVF;
}

////////////////////////////////////////
// HOCOクロック発振安定の取得
//
bool CCpuObject::IsStabilizeHOCO()
{
	return SYSTEM.OSCOVFSR.BIT.HCOVF;
}

////////////////////////////////////////
// USB専用PLLクロック発振安定の取得
//
bool CCpuObject::IsStabilizeUsbPll()
{
	return SYSTEM.OSCOVFSR.BIT.UPLOVF;
}

////////////////////////////////////////
// 発振停止検出割込み許可の設定
//	bEnable	: true(許可) ／ false(禁止)
//
void CCpuObject::EnableStopOscillationInterrupt(bool bEnable)
{
	SYSTEM.OSTDCR.BIT.OSTDIE = bEnable ? 1 : 0;
}

////////////////////////////////////////
// 発振停止検出割込み許可の取得
//
bool CCpuObject::IsEnableStopOscillationInterrupt()
{
	return SYSTEM.OSTDCR.BIT.OSTDIE;
}

////////////////////////////////////////
// 発振停止検出機能許可の設定
//	bEnable	: true(許可) ／ false(禁止)
//
void CCpuObject::EnableStopOscillationFunction(bool bEnable)
{
	SYSTEM.OSTDCR.BIT.OSTDE = bEnable ? 1 : 0;
}

////////////////////////////////////////
// 発振停止検出機能許可の取得
//
bool CCpuObject::IsEnableStopOscillationFunction()
{
	return SYSTEM.OSTDCR.BIT.OSTDE;
}

////////////////////////////////////////
// メインクロック発振停止の取得
//
bool CCpuObject::IsDetectStopMainClock()
{
	return SYSTEM.OSTDSR.BIT.OSTDF;
}

////////////////////////////////////////
// メインクロック発振器ウェイト時間の設定
//	cycle	: サイクル数
//
void CCpuObject::SelectMainClockOscillatorWaitTime(unsigned char cycle)
{
	switch (cycle) {
		case CYCLE_2:
		case CYCLE_1024:
		case CYCLE_2048:
		case CYCLE_4096:
		case CYCLE_8192:
		case CYCLE_16384:
		case CYCLE_32768:
		case CYCLE_65536:
		{
			SYSTEM.MOSCWTCR.BIT.MSTS = cycle;
			break;
		}
	}
}

////////////////////////////////////////
// メインクロック発振器ウェイト時間の取得
//
unsigned char CCpuObject::GetMainClockOscillatorWaitTime()
{
	return SYSTEM.MOSCWTCR.BIT.MSTS;
}

////////////////////////////////////////
// CLKOUT出力ソースの設定
//	src		: CLKOUT出力ソース
//
void CCpuObject::SelectCLKOUTSource(unsigned char src)
{
	switch (src) {
		case CLKSRC_LOCO:
		case CLKSRC_HOCO:
		case CLKSRC_MAINOSC:
		case CLKSRC_SUBOSC:
		case CLKSRC_PLL:
		{
			SYSTEM.CKOCR.BIT.CKOSEL = src;
			break;
		}
	}
}

////////////////////////////////////////
// CLKOUT出力ソースの取得
//
unsigned char CCpuObject::GetCLKOUTSource()
{
	return SYSTEM.CKOCR.BIT.CKOSEL;
}

////////////////////////////////////////
// CLKOUT出力分周比の設定
//	div		: 分周比
//
void CCpuObject::SelectCLKOUTDivider(unsigned char div)
{
	switch (div) {
		case CLKOUT_DIV1:
		case CLKOUT_DIV2:
		case CLKOUT_DIV4:
		case CLKOUT_DIV8:
		case CLKOUT_DIV16:
		{
			SYSTEM.CKOCR.BIT.CKODIV = div;
			break;
		}
	}
}

////////////////////////////////////////
// CLKOUT出力分周比の取得
//
unsigned char CCpuObject::GetCLKOUTDivider()
{
	return SYSTEM.CKOCR.BIT.CKODIV;
}

////////////////////////////////////////
// CLKOUT出力停止の設定
//	bEnable	: true(停止) ／ false(動作)
//
void CCpuObject::StopCLKOUT(bool bEnable)
{
	SYSTEM.CKOCR.BIT.CKOSTP = bEnable ? 1 : 0;
}

////////////////////////////////////////
// CLKOUT出力停止の取得
//
bool CCpuObject::IsStopCLKOUT()
{
	return SYSTEM.CKOCR.BIT.CKOSTP;
}

////////////////////////////////////////
// メインクロック発振器の設定(外部発振入力)
//	bEnable	: true(許可) ／ false(禁止)
//
void CCpuObject::EnableExternalOscillationInput(bool bEnable)
{
	SYSTEM.MOFCR.BIT.MOSEL = bEnable ? 1 : 0;
}

////////////////////////////////////////
// メインクロック発振器の取得(外部発振入力)
//
bool CCpuObject::IsEnableExternalOscillationInput()
{
	return SYSTEM.MOFCR.BIT.MOSEL;
}

////////////////////////////////////////
// メモリウェイトサイクル許可の設定
//	bEnable	: true(許可) ／ false(禁止)
//
void CCpuObject::EnableMemoryWait(bool bEnable)
{
	SYSTEM.MEMWAIT.BIT.MEMWAIT = bEnable ? 1 : 0;
}

////////////////////////////////////////
// メモリウェイトサイクル許可の取得
//
bool CCpuObject::IsEnableMemoryWait()
{
	return SYSTEM.MEMWAIT.BIT.MEMWAIT;
}

////////////////////////////////////////
// プロテクトレジスタ書き込みの設定
//	bEnable	: true(許可) ／ false(禁止)
//
void CCpuObject::EnableWriteProtectRegister0(bool bEnable)
{
	volatile unsigned short reg = SYSTEM.PRCR.WORD;
	reg |= 0xA500 | (bEnable ? 0x0001 : 0x0000);
	SYSTEM.PRCR.WORD = reg;
}

void CCpuObject::EnableWriteProtectRegister1(bool bEnable)
{
	volatile unsigned short reg = SYSTEM.PRCR.WORD;
	reg |= 0xA500 | (bEnable ? 0x0002 : 0x0000);
	SYSTEM.PRCR.WORD = reg;
}

void CCpuObject::EnableWriteProtectRegister2(bool bEnable)
{
	volatile unsigned short reg = SYSTEM.PRCR.WORD;
	reg |= 0xA500 | (bEnable ? 0x0004 : 0x0000);
	SYSTEM.PRCR.WORD = reg;
}

void CCpuObject::EnableWriteProtectRegister3(bool bEnable)
{
	volatile unsigned short reg = SYSTEM.PRCR.WORD;
	reg |= 0xA500 | (bEnable ? 0x0008 : 0x0000);
	SYSTEM.PRCR.WORD = reg;
}

////////////////////////////////////////////////////////////////////////////////
// モジュールストップコントローラ
//

////////////////////////////////////////
// モジュールの開始
//	mod		: モジュール
//
void CModuleStopObject::RunModule(int mod)
{
	assert(0 <= mod && mod < CModuleStopObject::NUM);

	// MSTPCR*の該当ビットをクリアすることで、モジュールストップ解除となる
	//
	if (0x00 <= mod && mod < 0x20) {
		SYSTEM.MSTPCRA.LONG &= ~(0x00000001 << (mod & 0x1F));
	}
	else if (0x20 <= mod && mod < 0x40) {
		SYSTEM.MSTPCRB.LONG &= ~(0x00000001 << (mod & 0x1F));
	}
	else if (0x40 <= mod && mod < 0x60) {
		SYSTEM.MSTPCRC.LONG &= ~(0x00000001 << (mod & 0x1F));
	}
	else if (0x60 <= mod && mod < 0x80) {
		SYSTEM.MSTPCRD.LONG &= ~(0x00000001 << (mod & 0x1F));
	}
}

////////////////////////////////////////
// モジュールの停止
//	mod		: モジュール
//
void CModuleStopObject::StopModule(int mod)
{
	assert(0 <= mod && mod < CModuleStopObject::NUM);

	// MSTPCR*の該当ビットをセットすることで、モジュールストップ遷移となる
	if (0x00 <= mod && mod < 0x20) {
		SYSTEM.MSTPCRA.LONG |= 0x00000001 << (mod & 0x1F);
	}
	else if (0x20 <= mod && mod < 0x40) {
		SYSTEM.MSTPCRB.LONG |= 0x00000001 << (mod & 0x1F);
	}
	else if (0x40 <= mod && mod < 0x60) {
		SYSTEM.MSTPCRC.LONG |= 0x00000001 << (mod & 0x1F);
	}
	else if (0x60 <= mod && mod < 0x80) {
		SYSTEM.MSTPCRD.LONG |= 0x00000001 << (mod & 0x1F);
	}
}

////////////////////////////////////////////////////////////////////////////////
// 割込みコントローラ
//

////////////////////////////////////////
// IR
//
volatile __evenaccess unsigned char* const g_IrqIrReg[] = {
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//   0 -   7
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//   8 -  15
	&ICU.IR[0x02].BYTE,		NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IR[0x02].BYTE,		//  16 -  23
	NULL,					NULL,					NULL,					&ICU.IR[0x03].BYTE,		&ICU.IR[0x03].BYTE,		&ICU.IR[0x03].BYTE,		&ICU.IR[0x03].BYTE,		&ICU.IR[0x03].BYTE,		//  24 -  31

	&ICU.IR[0x04].BYTE,		&ICU.IR[0x04].BYTE,		&ICU.IR[0x04].BYTE,		NULL,					&ICU.IR[0x04].BYTE,		&ICU.IR[0x04].BYTE,		&ICU.IR[0x04].BYTE,		NULL,					//  32 -  39
	&ICU.IR[0x05].BYTE,		&ICU.IR[0x05].BYTE,		&ICU.IR[0x05].BYTE,		&ICU.IR[0x05].BYTE,		&ICU.IR[0x05].BYTE,		&ICU.IR[0x05].BYTE,		&ICU.IR[0x05].BYTE,		&ICU.IR[0x05].BYTE,		//  40 -  47
	NULL,					NULL,					NULL,					NULL,					&ICU.IR[0x06].BYTE,		&ICU.IR[0x06].BYTE,		&ICU.IR[0x06].BYTE,		&ICU.IR[0x06].BYTE,		//  48 -  55
	&ICU.IR[0x07].BYTE,		&ICU.IR[0x07].BYTE,		&ICU.IR[0x07].BYTE,		&ICU.IR[0x07].BYTE,		&ICU.IR[0x07].BYTE,		&ICU.IR[0x07].BYTE,		&ICU.IR[0x07].BYTE,		&ICU.IR[0x07].BYTE,		//  56 -  63

	&ICU.IR[0x08].BYTE,		&ICU.IR[0x08].BYTE,		&ICU.IR[0x08].BYTE,		&ICU.IR[0x08].BYTE,		&ICU.IR[0x08].BYTE,		&ICU.IR[0x08].BYTE,		&ICU.IR[0x08].BYTE,	&	ICU.IR[0x08].BYTE,		//  64 -  71
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//  72 -  79
	&ICU.IR[0x0A].BYTE,		NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//  80 -  87
	&ICU.IR[0x0B].BYTE,		&ICU.IR[0x0B].BYTE,		&ICU.IR[0x0B].BYTE,		&ICU.IR[0x0B].BYTE,		&ICU.IR[0x0B].BYTE,		&ICU.IR[0x0B].BYTE,		NULL,					NULL,					//  88 -  95

	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IR[0x0C].BYTE,		&ICU.IR[0x0C].BYTE,		//  96 - 103
	&ICU.IR[0x0D].BYTE,		&ICU.IR[0x0D].BYTE,		&ICU.IR[0x0D].BYTE,		&ICU.IR[0x0D].BYTE,		&ICU.IR[0x0D].BYTE,		&ICU.IR[0x0D].BYTE,		&ICU.IR[0x0D].BYTE,		&ICU.IR[0x0D].BYTE,		// 104 - 111
	&ICU.IR[0x0E].BYTE,		&ICU.IR[0x0E].BYTE,		&ICU.IR[0x0E].BYTE,		&ICU.IR[0x0E].BYTE,		&ICU.IR[0x0E].BYTE,		&ICU.IR[0x0E].BYTE,		&ICU.IR[0x0E].BYTE,		&ICU.IR[0x0E].BYTE,		// 112 - 119
	&ICU.IR[0x0F].BYTE,		&ICU.IR[0x0F].BYTE,		&ICU.IR[0x0F].BYTE,		&ICU.IR[0x0F].BYTE,		&ICU.IR[0x0F].BYTE,		&ICU.IR[0x0F].BYTE,		&ICU.IR[0x0F].BYTE,		&ICU.IR[0x0F].BYTE,		// 120 - 127

	&ICU.IR[0x10].BYTE,		&ICU.IR[0x10].BYTE,		&ICU.IR[0x10].BYTE,		&ICU.IR[0x10].BYTE,		&ICU.IR[0x10].BYTE,		&ICU.IR[0x10].BYTE,		&ICU.IR[0x10].BYTE,		&ICU.IR[0x10].BYTE,		// 128 - 135
	&ICU.IR[0x11].BYTE,		&ICU.IR[0x11].BYTE,		&ICU.IR[0x11].BYTE,		&ICU.IR[0x11].BYTE,		&ICU.IR[0x11].BYTE,		&ICU.IR[0x11].BYTE,		&ICU.IR[0x11].BYTE,		&ICU.IR[0x11].BYTE,		// 136 - 143
	&ICU.IR[0x12].BYTE,		&ICU.IR[0x12].BYTE,		&ICU.IR[0x12].BYTE,		&ICU.IR[0x12].BYTE,		&ICU.IR[0x12].BYTE,		&ICU.IR[0x12].BYTE,		&ICU.IR[0x12].BYTE,		&ICU.IR[0x12].BYTE,		// 144 - 151
	&ICU.IR[0x13].BYTE,		&ICU.IR[0x13].BYTE,		&ICU.IR[0x13].BYTE,		&ICU.IR[0x13].BYTE,		&ICU.IR[0x13].BYTE,		&ICU.IR[0x13].BYTE,		&ICU.IR[0x13].BYTE,		&ICU.IR[0x13].BYTE,		// 152 - 159

	&ICU.IR[0x14].BYTE,		&ICU.IR[0x14].BYTE,		&ICU.IR[0x14].BYTE,		&ICU.IR[0x14].BYTE,		&ICU.IR[0x14].BYTE,		&ICU.IR[0x14].BYTE,		&ICU.IR[0x14].BYTE,		&ICU.IR[0x14].BYTE,		// 160 - 167
	NULL,					NULL,					&ICU.IR[0x15].BYTE,		&ICU.IR[0x15].BYTE,		NULL,					NULL,					&ICU.IR[0x15].BYTE,		&ICU.IR[0x15].BYTE,		// 168 - 175
	&ICU.IR[0x16].BYTE,		&ICU.IR[0x16].BYTE,		&ICU.IR[0x16].BYTE,		&ICU.IR[0x16].BYTE,		&ICU.IR[0x16].BYTE,		&ICU.IR[0x16].BYTE,		&ICU.IR[0x16].BYTE,		&ICU.IR[0x16].BYTE,		// 176 - 183
	&ICU.IR[0x17].BYTE,		&ICU.IR[0x17].BYTE,		NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					// 184 - 191

	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IR[0x18].BYTE,		&ICU.IR[0x18].BYTE,		// 192 - 199
	&ICU.IR[0x19].BYTE,		&ICU.IR[0x19].BYTE,		NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					// 200 - 207
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IR[0x1A].BYTE,		&ICU.IR[0x1A].BYTE,		// 208 - 215
	&ICU.IR[0x1B].BYTE,		&ICU.IR[0x1B].BYTE,		&ICU.IR[0x1B].BYTE,		&ICU.IR[0x1B].BYTE,		&ICU.IR[0x1B].BYTE,		&ICU.IR[0x1B].BYTE,		&ICU.IR[0x1B].BYTE,		&ICU.IR[0x1B].BYTE,		// 216 - 223

	&ICU.IR[0x1C].BYTE,		&ICU.IR[0x1C].BYTE,		&ICU.IR[0x1C].BYTE,		&ICU.IR[0x1C].BYTE,		&ICU.IR[0x1C].BYTE,		&ICU.IR[0x1C].BYTE,		&ICU.IR[0x1C].BYTE,		&ICU.IR[0x1C].BYTE,		// 224 - 231
	&ICU.IR[0x1D].BYTE,		&ICU.IR[0x1D].BYTE,		&ICU.IR[0x1D].BYTE,		&ICU.IR[0x1D].BYTE,		&ICU.IR[0x1D].BYTE,		&ICU.IR[0x1D].BYTE,		&ICU.IR[0x1D].BYTE,		&ICU.IR[0x1D].BYTE,		// 232 - 239
	&ICU.IR[0x1E].BYTE,		&ICU.IR[0x1E].BYTE,		&ICU.IR[0x1E].BYTE,		&ICU.IR[0x1E].BYTE,		&ICU.IR[0x1E].BYTE,		&ICU.IR[0x1E].BYTE,		&ICU.IR[0x1E].BYTE,		&ICU.IR[0x1E].BYTE,		// 240 - 247
	&ICU.IR[0x1F].BYTE,		&ICU.IR[0x1F].BYTE,		NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					// 248 - 255
};

////////////////////////////////////////
// IER
//
volatile __evenaccess unsigned char* const g_IrqIerReg[] = {
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//   0 -   7
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//   8 -  15
	&ICU.IER[0x02].BYTE,	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IER[0x02].BYTE,	//  16 -  23
	NULL,					NULL,					NULL,					&ICU.IER[0x03].BYTE,	&ICU.IER[0x03].BYTE,	&ICU.IER[0x03].BYTE,	&ICU.IER[0x03].BYTE,	&ICU.IER[0x03].BYTE,	//  24 -  31

	&ICU.IER[0x04].BYTE,	&ICU.IER[0x04].BYTE,	&ICU.IER[0x04].BYTE,	NULL,					&ICU.IER[0x04].BYTE,	&ICU.IER[0x04].BYTE,	&ICU.IER[0x04].BYTE,	NULL,					//  32 -  39
	&ICU.IER[0x05].BYTE,	&ICU.IER[0x05].BYTE,	&ICU.IER[0x05].BYTE,	&ICU.IER[0x05].BYTE,	&ICU.IER[0x05].BYTE,	&ICU.IER[0x05].BYTE,	&ICU.IER[0x05].BYTE,	&ICU.IER[0x05].BYTE,	//  40 -  47
	NULL,					NULL,					NULL,					NULL,					&ICU.IER[0x06].BYTE,	&ICU.IER[0x06].BYTE,	&ICU.IER[0x06].BYTE,	&ICU.IER[0x06].BYTE,	//  48 -  55
	&ICU.IER[0x07].BYTE,	&ICU.IER[0x07].BYTE,	&ICU.IER[0x07].BYTE,	&ICU.IER[0x07].BYTE,	&ICU.IER[0x07].BYTE,	&ICU.IER[0x07].BYTE,	&ICU.IER[0x07].BYTE,	&ICU.IER[0x07].BYTE,	//  56 -  63

	&ICU.IER[0x08].BYTE,	&ICU.IER[0x08].BYTE,	&ICU.IER[0x08].BYTE,	&ICU.IER[0x08].BYTE,	&ICU.IER[0x08].BYTE,	&ICU.IER[0x08].BYTE,	&ICU.IER[0x08].BYTE,	&ICU.IER[0x08].BYTE,	//  64 -  71
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//  72 -  79
	&ICU.IER[0x0A].BYTE,	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//  80 -  87
	&ICU.IER[0x0B].BYTE,	&ICU.IER[0x0B].BYTE,	&ICU.IER[0x0B].BYTE,	&ICU.IER[0x0B].BYTE,	&ICU.IER[0x0B].BYTE,	&ICU.IER[0x0B].BYTE,	NULL,					NULL,					//  88 -  95

	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IER[0x0C].BYTE,	&ICU.IER[0x0C].BYTE,	//  96 - 103
	&ICU.IER[0x0D].BYTE,	&ICU.IER[0x0D].BYTE,	&ICU.IER[0x0D].BYTE,	&ICU.IER[0x0D].BYTE,	&ICU.IER[0x0D].BYTE,	&ICU.IER[0x0D].BYTE,	&ICU.IER[0x0D].BYTE,	&ICU.IER[0x0D].BYTE,	// 104 - 111
	&ICU.IER[0x0E].BYTE,	&ICU.IER[0x0E].BYTE,	&ICU.IER[0x0E].BYTE,	&ICU.IER[0x0E].BYTE,	&ICU.IER[0x0E].BYTE,	&ICU.IER[0x0E].BYTE,	&ICU.IER[0x0E].BYTE,	&ICU.IER[0x0E].BYTE,	// 112 - 119
	&ICU.IER[0x0F].BYTE,	&ICU.IER[0x0F].BYTE,	&ICU.IER[0x0F].BYTE,	&ICU.IER[0x0F].BYTE,	&ICU.IER[0x0F].BYTE,	&ICU.IER[0x0F].BYTE,	&ICU.IER[0x0F].BYTE,	&ICU.IER[0x0F].BYTE,	// 120 - 127

	&ICU.IER[0x10].BYTE,	&ICU.IER[0x10].BYTE,	&ICU.IER[0x10].BYTE,	&ICU.IER[0x10].BYTE,	&ICU.IER[0x10].BYTE,	&ICU.IER[0x10].BYTE,	&ICU.IER[0x10].BYTE,	&ICU.IER[0x10].BYTE,	// 128 - 135
	&ICU.IER[0x11].BYTE,	&ICU.IER[0x11].BYTE,	&ICU.IER[0x11].BYTE,	&ICU.IER[0x11].BYTE,	&ICU.IER[0x11].BYTE,	&ICU.IER[0x11].BYTE,	&ICU.IER[0x11].BYTE,	&ICU.IER[0x11].BYTE,	// 136 - 143
	&ICU.IER[0x12].BYTE,	&ICU.IER[0x12].BYTE,	&ICU.IER[0x12].BYTE,	&ICU.IER[0x12].BYTE,	&ICU.IER[0x12].BYTE,	&ICU.IER[0x12].BYTE,	&ICU.IER[0x12].BYTE,	&ICU.IER[0x12].BYTE,	// 144 - 151
	&ICU.IER[0x13].BYTE,	&ICU.IER[0x13].BYTE,	&ICU.IER[0x13].BYTE,	&ICU.IER[0x13].BYTE,	&ICU.IER[0x13].BYTE,	&ICU.IER[0x13].BYTE,	&ICU.IER[0x13].BYTE,	&ICU.IER[0x13].BYTE,	// 152 - 159

	&ICU.IER[0x14].BYTE,	&ICU.IER[0x14].BYTE,	&ICU.IER[0x14].BYTE,	&ICU.IER[0x14].BYTE,	&ICU.IER[0x14].BYTE,	&ICU.IER[0x14].BYTE,	&ICU.IER[0x14].BYTE,	&ICU.IER[0x14].BYTE,	// 160 - 167
	NULL,					NULL,					&ICU.IER[0x15].BYTE,	&ICU.IER[0x15].BYTE,	NULL,					NULL,					&ICU.IER[0x15].BYTE,	&ICU.IER[0x15].BYTE,	// 168 - 175
	&ICU.IER[0x16].BYTE,	&ICU.IER[0x16].BYTE,	&ICU.IER[0x16].BYTE,	&ICU.IER[0x16].BYTE,	&ICU.IER[0x16].BYTE,	&ICU.IER[0x16].BYTE,	&ICU.IER[0x16].BYTE,	&ICU.IER[0x16].BYTE,	// 176 - 183
	&ICU.IER[0x17].BYTE,	&ICU.IER[0x17].BYTE,	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					// 184 - 191

	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IER[0x18].BYTE,	&ICU.IER[0x18].BYTE,	// 192 - 199
	&ICU.IER[0x19].BYTE,	&ICU.IER[0x19].BYTE,	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					// 200 - 207
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IER[0x1A].BYTE,	&ICU.IER[0x1A].BYTE,	// 208 - 215
	&ICU.IER[0x1B].BYTE,	&ICU.IER[0x1B].BYTE,	&ICU.IER[0x1B].BYTE,	&ICU.IER[0x1B].BYTE,	&ICU.IER[0x1B].BYTE,	&ICU.IER[0x1B].BYTE,	&ICU.IER[0x1B].BYTE,	&ICU.IER[0x1B].BYTE,	// 216 - 223

	&ICU.IER[0x1C].BYTE,	&ICU.IER[0x1C].BYTE,	&ICU.IER[0x1C].BYTE,	&ICU.IER[0x1C].BYTE,	&ICU.IER[0x1C].BYTE,	&ICU.IER[0x1C].BYTE,	&ICU.IER[0x1C].BYTE,	&ICU.IER[0x1C].BYTE,	// 224 - 231
	&ICU.IER[0x1D].BYTE,	&ICU.IER[0x1D].BYTE,	&ICU.IER[0x1D].BYTE,	&ICU.IER[0x1D].BYTE,	&ICU.IER[0x1D].BYTE,	&ICU.IER[0x1D].BYTE,	&ICU.IER[0x1D].BYTE,	&ICU.IER[0x1D].BYTE,	// 232 - 239
	&ICU.IER[0x1E].BYTE,	&ICU.IER[0x1E].BYTE,	&ICU.IER[0x1E].BYTE,	&ICU.IER[0x1E].BYTE,	&ICU.IER[0x1E].BYTE,	&ICU.IER[0x1E].BYTE,	&ICU.IER[0x1E].BYTE,	&ICU.IER[0x1E].BYTE,	// 240 - 247
	&ICU.IER[0x1F].BYTE,	&ICU.IER[0x1F].BYTE,	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					// 248 - 255
};

////////////////////////////////////////
// IPR
//
volatile __evenaccess unsigned char* const g_IrqIprReg[] = {
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//   0 -   7
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//   8 -  15
	&ICU.IPR[0x00].BYTE,	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IPR[0x02].BYTE,	//  16 -  23
	NULL,					NULL,					NULL,					&ICU.IPR[0x03].BYTE,	&ICU.IPR[0x04].BYTE,	&ICU.IPR[0x05].BYTE,	&ICU.IPR[0x06].BYTE,	&ICU.IPR[0x07].BYTE,	//  24 -  31

	&ICU.IPR[0x20].BYTE,	&ICU.IPR[0x21].BYTE,	&ICU.IPR[0x22].BYTE,	NULL,					&ICU.IPR[0x24].BYTE,	&ICU.IPR[0x25].BYTE,	&ICU.IPR[0x26].BYTE,	NULL,					//  32 -  39
	&ICU.IPR[0x28].BYTE,	&ICU.IPR[0x29].BYTE,	&ICU.IPR[0x2A].BYTE,	&ICU.IPR[0x2B].BYTE,	&ICU.IPR[0x2C].BYTE,	&ICU.IPR[0x2C].BYTE,	&ICU.IPR[0x2C].BYTE,	&ICU.IPR[0x2C].BYTE,	//  40 -  47
	NULL,					NULL,					NULL,					NULL,					&ICU.IPR[0x34].BYTE,	&ICU.IPR[0x35].BYTE,	&ICU.IPR[0x36].BYTE,	&ICU.IPR[0x37].BYTE,	//  48 -  55
	&ICU.IPR[0x38].BYTE,	&ICU.IPR[0x39].BYTE,	&ICU.IPR[0x3A].BYTE,	&ICU.IPR[0x3B].BYTE,	&ICU.IPR[0x3C].BYTE,	&ICU.IPR[0x3C].BYTE,	&ICU.IPR[0x3C].BYTE,	&ICU.IPR[0x3F].BYTE,	//  56 -  63

	&ICU.IPR[0x40].BYTE,	&ICU.IPR[0x41].BYTE,	&ICU.IPR[0x42].BYTE,	&ICU.IPR[0x43].BYTE,	&ICU.IPR[0x44].BYTE,	&ICU.IPR[0x45].BYTE,	&ICU.IPR[0x46].BYTE,	&ICU.IPR[0x47].BYTE,	//  64 -  71
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//  72 -  79
	&ICU.IPR[0x50].BYTE,	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					//  80 -  87
	&ICU.IPR[0x58].BYTE,	&ICU.IPR[0x59].BYTE,	&ICU.IPR[0x5A].BYTE,	&ICU.IPR[0x5B].BYTE,	&ICU.IPR[0x5C].BYTE,	&ICU.IPR[0x5D].BYTE,	NULL,					NULL,					//  88 -  95

	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IPR[0x66].BYTE,	&ICU.IPR[0x67].BYTE,	//  96 - 103
	&ICU.IPR[0x68].BYTE,	&ICU.IPR[0x69].BYTE,	&ICU.IPR[0x6A].BYTE,	&ICU.IPR[0x6B].BYTE,	&ICU.IPR[0x6C].BYTE,	&ICU.IPR[0x6C].BYTE,	&ICU.IPR[0x6C].BYTE,	&ICU.IPR[0x6F].BYTE,	// 104 - 111
	&ICU.IPR[0x6F].BYTE,	&ICU.IPR[0x71].BYTE,	&ICU.IPR[0x72].BYTE,	&ICU.IPR[0x72].BYTE,	&ICU.IPR[0x72].BYTE,	&ICU.IPR[0x72].BYTE,	&ICU.IPR[0x76].BYTE,	&ICU.IPR[0x76].BYTE,	// 112 - 119
	&ICU.IPR[0x76].BYTE,	&ICU.IPR[0x79].BYTE,	&ICU.IPR[0x79].BYTE,	&ICU.IPR[0x7B].BYTE,	&ICU.IPR[0x7B].BYTE,	&ICU.IPR[0x7D].BYTE,	&ICU.IPR[0x7D].BYTE,	&ICU.IPR[0x7F].BYTE,	// 120 - 127

	&ICU.IPR[0x7F].BYTE,	&ICU.IPR[0x81].BYTE,	&ICU.IPR[0x81].BYTE,	&ICU.IPR[0x81].BYTE,	&ICU.IPR[0x81].BYTE,	&ICU.IPR[0x85].BYTE,	&ICU.IPR[0x86].BYTE,	&ICU.IPR[0x86].BYTE,	// 128 - 135
	&ICU.IPR[0x86].BYTE,	&ICU.IPR[0x86].BYTE,	&ICU.IPR[0x8A].BYTE,	&ICU.IPR[0x8B].BYTE,	&ICU.IPR[0x8B].BYTE,	&ICU.IPR[0x8B].BYTE,	&ICU.IPR[0x8E].BYTE,	&ICU.IPR[0x8E].BYTE,	// 136 - 143
	&ICU.IPR[0x8E].BYTE,	&ICU.IPR[0x8E].BYTE,	&ICU.IPR[0x92].BYTE,	&ICU.IPR[0x93].BYTE,	&ICU.IPR[0x93].BYTE,	&ICU.IPR[0x95].BYTE,	&ICU.IPR[0x95].BYTE,	&ICU.IPR[0x97].BYTE,	// 144 - 151
	&ICU.IPR[0x97].BYTE,	&ICU.IPR[0x99].BYTE,	&ICU.IPR[0x99].BYTE,	&ICU.IPR[0x9B].BYTE,	&ICU.IPR[0x9B].BYTE,	&ICU.IPR[0x9B].BYTE,	&ICU.IPR[0x9B].BYTE,	&ICU.IPR[0x9F].BYTE,	// 152 - 159

	&ICU.IPR[0xA0].BYTE,	&ICU.IPR[0xA0].BYTE,	&ICU.IPR[0xA2].BYTE,	&ICU.IPR[0xA2].BYTE,	&ICU.IPR[0xA4].BYTE,	&ICU.IPR[0xA4].BYTE,	&ICU.IPR[0xA6].BYTE,	&ICU.IPR[0xA6].BYTE,	// 160 - 167
	NULL,					NULL,					&ICU.IPR[0xAA].BYTE,	&ICU.IPR[0xAB].BYTE,	NULL,					NULL,					&ICU.IPR[0xAE].BYTE,	&ICU.IPR[0xAE].BYTE,	// 168 - 175
	&ICU.IPR[0xAE].BYTE,	&ICU.IPR[0xB1].BYTE,	&ICU.IPR[0xB1].BYTE,	&ICU.IPR[0xB1].BYTE,	&ICU.IPR[0xB4].BYTE,	&ICU.IPR[0xB4].BYTE,	&ICU.IPR[0xB4].BYTE,	&ICU.IPR[0xB7].BYTE,	// 176 - 183
	&ICU.IPR[0xB7].BYTE,	&ICU.IPR[0xB7].BYTE,	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					// 184 - 191

	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IPR[0xC6].BYTE,	&ICU.IPR[0xC7].BYTE,	// 192 - 199
	&ICU.IPR[0xC8].BYTE,	&ICU.IPR[0xC9].BYTE,	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					// 200 - 207
	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					&ICU.IPR[0xD6].BYTE,	&ICU.IPR[0xD6].BYTE,	// 208 - 215
	&ICU.IPR[0xD6].BYTE,	&ICU.IPR[0xD6].BYTE,	&ICU.IPR[0xDA].BYTE,	&ICU.IPR[0xDA].BYTE,	&ICU.IPR[0xDA].BYTE,	&ICU.IPR[0xDA].BYTE,	&ICU.IPR[0xDE].BYTE,	&ICU.IPR[0xDE].BYTE,	// 216 - 223

	&ICU.IPR[0xDE].BYTE,	&ICU.IPR[0xDE].BYTE,	&ICU.IPR[0xE2].BYTE,	&ICU.IPR[0xE2].BYTE,	&ICU.IPR[0xE2].BYTE,	&ICU.IPR[0xE2].BYTE,	&ICU.IPR[0xE6].BYTE,	&ICU.IPR[0xE6].BYTE,	// 224 - 231
	&ICU.IPR[0xE6].BYTE,	&ICU.IPR[0xE6].BYTE,	&ICU.IPR[0xEA].BYTE,	&ICU.IPR[0xEA].BYTE,	&ICU.IPR[0xEA].BYTE,	&ICU.IPR[0xEA].BYTE,	&ICU.IPR[0xEE].BYTE,	&ICU.IPR[0xEE].BYTE,	// 232 - 239
	&ICU.IPR[0xEE].BYTE,	&ICU.IPR[0xEE].BYTE,	&ICU.IPR[0xF2].BYTE,	&ICU.IPR[0xF3].BYTE,	&ICU.IPR[0xF4].BYTE,	&ICU.IPR[0xF5].BYTE,	&ICU.IPR[0xF6].BYTE,	&ICU.IPR[0xF7].BYTE,	// 240 - 247
	&ICU.IPR[0xF8].BYTE,	&ICU.IPR[0xF9].BYTE,	NULL,					NULL,					NULL,					NULL,					NULL,					NULL,					// 248 - 255
};

////////////////////////////////////////
// 割込み要求のクリア
//	vect	: ベクタテーブル番号
//
void CIcuObject::CleatInterruptRequest(unsigned int vect)
{
	assert(vect < 256);

	if (g_IrqIrReg[vect] != NULL)
		*g_IrqIrReg[vect] &= 0;
}

////////////////////////////////////////
// 割込み要求状態の取得
//	vect	: ベクタテーブル番号
//
bool CIcuObject::IsInterruptRequest(unsigned int vect)
{
	assert(vect < 256);

	if (g_IrqIrReg[vect] != NULL)
		return ((*g_IrqIrReg[vect] & 0x01) == 0x01);

	return false;
}

////////////////////////////////////////
// 割込み要求許可の設定
//	vect	: ベクタテーブル番号
//	bEnable	: true(許可) ／ false(禁止)
//
void CIcuObject::EnableInterrupt(unsigned int vect, bool bEnable)
{
	assert(vect < 256);

	if (g_IrqIerReg[vect] != NULL) {

		int bit_shift = vect % 8;

		if (bEnable) {
			*g_IrqIerReg[vect] |= 0x01 << bit_shift;
		}
		else {
			*g_IrqIerReg[vect] &= ~(0x01 << bit_shift);
		}
	}
}

////////////////////////////////////////
// 割込み要求許可状態の取得
//	vect	: ベクタテーブル番号
//
bool CIcuObject::IsEnableInterrupt(unsigned int vect)
{
	assert(vect < 256);

	if (g_IrqIerReg[vect] != NULL) {

		int bit_shift = vect % 8;

		return (*g_IrqIerReg[vect] & (0x01 << bit_shift));
	}

	return false;
}

////////////////////////////////////////
// 割込み優先順位の設定
//	vect		: ベクタテーブル番号
//	nPriority	: 優先順位 0 ～ 15
//
void CIcuObject::SetPriority(unsigned int vect, unsigned char nPriority)
{
	assert(nPriority <= PLV15);
	assert(vect < 256);

	if (g_IrqIprReg[vect] != NULL) {
		*g_IrqIprReg[vect] = nPriority;
	}
}

////////////////////////////////////////
// IRQ割込み検出方法の設定
//	irq		: IRQ(0 ～ 7)
//	nSense	: 検出方法
//
void CIcuObject::SetIrqSense(unsigned char irq, unsigned char nSense)
{
	switch (irq) {
		case 0:	ICU.IRQCR[0].BIT.IRQMD = nSense;	break;
		case 1:	ICU.IRQCR[1].BIT.IRQMD = nSense;	break;
		case 2:	ICU.IRQCR[2].BIT.IRQMD = nSense;	break;
		case 3:	ICU.IRQCR[3].BIT.IRQMD = nSense;	break;
		case 4:	ICU.IRQCR[4].BIT.IRQMD = nSense;	break;
		case 5:	ICU.IRQCR[5].BIT.IRQMD = nSense;	break;
		case 6:	ICU.IRQCR[6].BIT.IRQMD = nSense;	break;
		case 7:	ICU.IRQCR[7].BIT.IRQMD = nSense;	break;
	}
}


////////////////////////////////////////////////////////////////////////////////
// I/Oポートコントローラ
//

////////////////////////////////////////
// ポート方向レジスタの設定
//	port	: P_P0, P_P1, P_P2, P_P3, P_P4, P_P5, P_PA, P_PB, P_PC, P_PD, P_PE, P_PH, P_PJのいずれか
//	num		: ビット番号(0 ～ 7)
//	out		: true(出力) ／ false(入力)
//
void CDioObject::SetDir(int port, int num, bool bOut)
{
	if (port >= NUM || num >= 8)
		return;

	unsigned char nBitFlag = 0x01 << num;
	switch (port) {
		case P_P0:	if (bOut) PORT0.PDR.BYTE |= nBitFlag;	else PORT0.PDR.BYTE &= ~nBitFlag;	break;
		case P_P1:	if (bOut) PORT1.PDR.BYTE |= nBitFlag;	else PORT1.PDR.BYTE &= ~nBitFlag;	break;
		case P_P2:	if (bOut) PORT2.PDR.BYTE |= nBitFlag;	else PORT2.PDR.BYTE &= ~nBitFlag;	break;
		case P_P3:	if (bOut) PORT3.PDR.BYTE |= nBitFlag;	else PORT3.PDR.BYTE &= ~nBitFlag;	break;
		case P_P4:	if (bOut) PORT4.PDR.BYTE |= nBitFlag;	else PORT4.PDR.BYTE &= ~nBitFlag;	break;
		case P_P5:	if (bOut) PORT5.PDR.BYTE |= nBitFlag;	else PORT5.PDR.BYTE &= ~nBitFlag;	break;
		case P_PA:	if (bOut) PORTA.PDR.BYTE |= nBitFlag;	else PORTA.PDR.BYTE &= ~nBitFlag;	break;
		case P_PB:	if (bOut) PORTB.PDR.BYTE |= nBitFlag;	else PORTB.PDR.BYTE &= ~nBitFlag;	break;
		case P_PC:	if (bOut) PORTC.PDR.BYTE |= nBitFlag;	else PORTC.PDR.BYTE &= ~nBitFlag;	break;
		case P_PD:	if (bOut) PORTD.PDR.BYTE |= nBitFlag;	else PORTD.PDR.BYTE &= ~nBitFlag;	break;
		case P_PE:	if (bOut) PORTE.PDR.BYTE |= nBitFlag;	else PORTE.PDR.BYTE &= ~nBitFlag;	break;
		case P_PH:	if (bOut) PORTH.PDR.BYTE |= nBitFlag;	else PORTH.PDR.BYTE &= ~nBitFlag;	break;
		case P_PJ:	if (bOut) PORTJ.PDR.BYTE |= nBitFlag;	else PORTJ.PDR.BYTE &= ~nBitFlag;	break;
	}
}

////////////////////////////////////////
// ポート出力データレジスタの設定
//	port	: P_P0, P_P1, P_P2, P_P3, P_P4, P_P5, P_PA, P_PB, P_PC, P_PD, P_PE, P_PH, P_PJのいずれか
//	num		: ビット番号(0 ～ 7)
//	bOn		: true(High) ／ false(Low)
//
void CDioObject::SetData(int port, int num, bool bOn)
{
	if (port >= NUM || num >= 8)
		return;

	unsigned char nBitFlag = 0x01 << num;
	switch (port) {
		case P_P0:	if (bOn) PORT0.PODR.BYTE |= nBitFlag;	else PORT0.PODR.BYTE &= ~nBitFlag;	break;
		case P_P1:	if (bOn) PORT1.PODR.BYTE |= nBitFlag;	else PORT1.PODR.BYTE &= ~nBitFlag;	break;
		case P_P2:	if (bOn) PORT2.PODR.BYTE |= nBitFlag;	else PORT2.PODR.BYTE &= ~nBitFlag;	break;
		case P_P3:	if (bOn) PORT3.PODR.BYTE |= nBitFlag;	else PORT3.PODR.BYTE &= ~nBitFlag;	break;
		case P_P4:	if (bOn) PORT4.PODR.BYTE |= nBitFlag;	else PORT4.PODR.BYTE &= ~nBitFlag;	break;
		case P_P5:	if (bOn) PORT5.PODR.BYTE |= nBitFlag;	else PORT5.PODR.BYTE &= ~nBitFlag;	break;
		case P_PA:	if (bOn) PORTA.PODR.BYTE |= nBitFlag;	else PORTA.PODR.BYTE &= ~nBitFlag;	break;
		case P_PB:	if (bOn) PORTB.PODR.BYTE |= nBitFlag;	else PORTB.PODR.BYTE &= ~nBitFlag;	break;
		case P_PC:	if (bOn) PORTC.PODR.BYTE |= nBitFlag;	else PORTC.PODR.BYTE &= ~nBitFlag;	break;
		case P_PD:	if (bOn) PORTD.PODR.BYTE |= nBitFlag;	else PORTD.PODR.BYTE &= ~nBitFlag;	break;
		case P_PE:	if (bOn) PORTE.PODR.BYTE |= nBitFlag;	else PORTE.PODR.BYTE &= ~nBitFlag;	break;
		case P_PH:	if (bOn) PORTH.PODR.BYTE |= nBitFlag;	else PORTH.PODR.BYTE &= ~nBitFlag;	break;
		case P_PJ:	if (bOn) PORTJ.PODR.BYTE |= nBitFlag;	else PORTJ.PODR.BYTE &= ~nBitFlag;	break;
	}
}

////////////////////////////////////////
// ポート入力データレジスタの取得
//	port	: P_P0, P_P1, P_P2, P_P3, P_P4, P_P5, P_PA, P_PB, P_PC, P_PD, P_PE, P_PH, P_PJのいずれか
//	num		: ビット番号(0 ～ 7)
//
bool CDioObject::GetLineStatus(int port, int num)
{
	if (port >= NUM || num >= 8)
		return false;

	unsigned char nBitFlag = 0x01 << num;
	switch (port) {
		case P_P0:	return (PORT0.PIDR.BYTE & nBitFlag);
		case P_P1:	return (PORT1.PIDR.BYTE & nBitFlag);
		case P_P2:	return (PORT2.PIDR.BYTE & nBitFlag);
		case P_P3:	return (PORT3.PIDR.BYTE & nBitFlag);
		case P_P4:	return (PORT4.PIDR.BYTE & nBitFlag);
		case P_P5:	return (PORT5.PIDR.BYTE & nBitFlag);
		case P_PA:	return (PORTA.PIDR.BYTE & nBitFlag);
		case P_PB:	return (PORTB.PIDR.BYTE & nBitFlag);
		case P_PC:	return (PORTC.PIDR.BYTE & nBitFlag);
		case P_PD:	return (PORTD.PIDR.BYTE & nBitFlag);
		case P_PE:	return (PORTE.PIDR.BYTE & nBitFlag);
		case P_PH:	return (PORTH.PIDR.BYTE & nBitFlag);
		case P_PJ:	return (PORTJ.PIDR.BYTE & nBitFlag);
	}
	return false;
}

////////////////////////////////////////
// ポートモードレジスタの設定
//	port	: P_P0, P_P1, P_P2, P_P3, P_P4, P_P5, P_PA, P_PB, P_PC, P_PD, P_PE, P_PH, P_PJのいずれか
//	num		: ビット番号(0 ～ 7)
//	mode	: true(周辺機能) ／ false(汎用入出力ポート)
//
void CDioObject::SetModeFunc(int port, int num, bool bMode)
{
	if (port >= NUM || num >= 8)
		return;

	unsigned char nBitFlag = 0x01 << num;
	switch (port) {
		case P_P0:	if (bMode) PORT0.PMR.BYTE |= nBitFlag;	else PORT0.PMR.BYTE &= ~nBitFlag;	break;
		case P_P1:	if (bMode) PORT1.PMR.BYTE |= nBitFlag;	else PORT1.PMR.BYTE &= ~nBitFlag;	break;
		case P_P2:	if (bMode) PORT2.PMR.BYTE |= nBitFlag;	else PORT2.PMR.BYTE &= ~nBitFlag;	break;
		case P_P3:	if (bMode) PORT3.PMR.BYTE |= nBitFlag;	else PORT3.PMR.BYTE &= ~nBitFlag;	break;
		case P_P4:	if (bMode) PORT4.PMR.BYTE |= nBitFlag;	else PORT4.PMR.BYTE &= ~nBitFlag;	break;
		case P_P5:	if (bMode) PORT5.PMR.BYTE |= nBitFlag;	else PORT5.PMR.BYTE &= ~nBitFlag;	break;
		case P_PA:	if (bMode) PORTA.PMR.BYTE |= nBitFlag;	else PORTA.PMR.BYTE &= ~nBitFlag;	break;
		case P_PB:	if (bMode) PORTB.PMR.BYTE |= nBitFlag;	else PORTB.PMR.BYTE &= ~nBitFlag;	break;
		case P_PC:	if (bMode) PORTC.PMR.BYTE |= nBitFlag;	else PORTC.PMR.BYTE &= ~nBitFlag;	break;
		case P_PD:	if (bMode) PORTD.PMR.BYTE |= nBitFlag;	else PORTD.PMR.BYTE &= ~nBitFlag;	break;
		case P_PE:	if (bMode) PORTE.PMR.BYTE |= nBitFlag;	else PORTE.PMR.BYTE &= ~nBitFlag;	break;
		case P_PH:	if (bMode) PORTH.PMR.BYTE |= nBitFlag;	else PORTH.PMR.BYTE &= ~nBitFlag;	break;
		case P_PJ:	if (bMode) PORTJ.PMR.BYTE |= nBitFlag;	else PORTJ.PMR.BYTE &= ~nBitFlag;	break;
	}
}

////////////////////////////////////////
// オープンドレイン制御レジスタの設定
//	port	: P_P1, P_P2, P_P3, P_P5, P_PA, P_PB, P_PC, P_PE, P_PJのいずれか
//	num		: ビット番号(0 ～ 7)
//	bOdr	: true(オープンドレイン) ／ false(CMOS)
//
void CDioObject::SetOpenDrain(int port, int num, bool bOdr)
{
	if (port >= NUM || num >= 8)
		return;

	if (0 <= num && num <= 3) {
		unsigned char nBitFlag = 0x01 << (2 * num);
		switch (port) {
			case P_P1:	if (bOdr) PORT1.ODR0.BYTE |= nBitFlag;	else PORT1.ODR0.BYTE &= ~nBitFlag;	break;
			case P_P2:	if (bOdr) PORT2.ODR0.BYTE |= nBitFlag;	else PORT2.ODR0.BYTE &= ~nBitFlag;	break;
			case P_P3:	if (bOdr) PORT3.ODR0.BYTE |= nBitFlag;	else PORT3.ODR0.BYTE &= ~nBitFlag;	break;
			case P_P5:	if (bOdr) PORT5.ODR0.BYTE |= nBitFlag;	else PORT5.ODR0.BYTE &= ~nBitFlag;	break;
			case P_PA:	if (bOdr) PORTA.ODR0.BYTE |= nBitFlag;	else PORTA.ODR0.BYTE &= ~nBitFlag;	break;
			case P_PB:	if (bOdr) PORTB.ODR0.BYTE |= nBitFlag;	else PORTB.ODR0.BYTE &= ~nBitFlag;	break;
			case P_PC:	if (bOdr) PORTC.ODR0.BYTE |= nBitFlag;	else PORTC.ODR0.BYTE &= ~nBitFlag;	break;
			case P_PE:	if (bOdr) PORTE.ODR0.BYTE |= nBitFlag;	else PORTE.ODR0.BYTE &= ~nBitFlag;	break;
			case P_PJ:	if (bOdr) PORTJ.ODR0.BYTE |= nBitFlag;	else PORTJ.ODR0.BYTE &= ~nBitFlag;	break;
		}
	}
	else if (4 <= num && num <= 7) {
		unsigned char nBitFlag = 0x01 << (2 * (num - 4));
		switch (port) {
			case P_P1:	if (bOdr) PORT1.ODR1.BYTE |= nBitFlag;	else PORT1.ODR1.BYTE &= ~nBitFlag;	break;
			case P_P2:	if (bOdr) PORT2.ODR1.BYTE |= nBitFlag;	else PORT2.ODR1.BYTE &= ~nBitFlag;	break;
			case P_P3:	if (bOdr) PORT3.ODR1.BYTE |= nBitFlag;	else PORT3.ODR1.BYTE &= ~nBitFlag;	break;
			case P_P5:	if (bOdr) PORT5.ODR1.BYTE |= nBitFlag;	else PORT5.ODR1.BYTE &= ~nBitFlag;	break;
			case P_PA:	if (bOdr) PORTA.ODR1.BYTE |= nBitFlag;	else PORTA.ODR1.BYTE &= ~nBitFlag;	break;
			case P_PB:	if (bOdr) PORTB.ODR1.BYTE |= nBitFlag;	else PORTB.ODR1.BYTE &= ~nBitFlag;	break;
			case P_PC:	if (bOdr) PORTC.ODR1.BYTE |= nBitFlag;	else PORTC.ODR1.BYTE &= ~nBitFlag;	break;
			case P_PE:	if (bOdr) PORTE.ODR1.BYTE |= nBitFlag;	else PORTE.ODR1.BYTE &= ~nBitFlag;	break;
		}
	}
}

////////////////////////////////////////
// プルアップ制御レジスタの設定
//	port	: P_P0, P_P1, P_P2, P_P3, P_P4, P_P5, P_PA, P_PB, P_PC, P_PD, P_PE, P_PH, P_PJのいずれか
//	num		: ビット番号(0 ～ 7)
//	bPullup	: true(有効) ／ false(無効)
//
void CDioObject::SetInputPullup(int port, int num, bool bPullup)
{
	if (port >= NUM || num >= 8)
		return;

	unsigned char nBitFlag = 0x01 << num;
	switch (port) {
		case P_P0:	if (bPullup) PORT0.PCR.BYTE |= nBitFlag;	else PORT0.PCR.BYTE &= ~nBitFlag;	break;
		case P_P1:	if (bPullup) PORT1.PCR.BYTE |= nBitFlag;	else PORT1.PCR.BYTE &= ~nBitFlag;	break;
		case P_P2:	if (bPullup) PORT2.PCR.BYTE |= nBitFlag;	else PORT2.PCR.BYTE &= ~nBitFlag;	break;
		case P_P3:	if (bPullup) PORT3.PCR.BYTE |= nBitFlag;	else PORT3.PCR.BYTE &= ~nBitFlag;	break;
		case P_P4:	if (bPullup) PORT4.PCR.BYTE |= nBitFlag;	else PORT4.PCR.BYTE &= ~nBitFlag;	break;
		case P_P5:	if (bPullup) PORT5.PCR.BYTE |= nBitFlag;	else PORT5.PCR.BYTE &= ~nBitFlag;	break;
		case P_PA:	if (bPullup) PORTA.PCR.BYTE |= nBitFlag;	else PORTA.PCR.BYTE &= ~nBitFlag;	break;
		case P_PB:	if (bPullup) PORTB.PCR.BYTE |= nBitFlag;	else PORTB.PCR.BYTE &= ~nBitFlag;	break;
		case P_PC:	if (bPullup) PORTC.PCR.BYTE |= nBitFlag;	else PORTC.PCR.BYTE &= ~nBitFlag;	break;
		case P_PD:	if (bPullup) PORTD.PCR.BYTE |= nBitFlag;	else PORTD.PCR.BYTE &= ~nBitFlag;	break;
		case P_PE:	if (bPullup) PORTE.PCR.BYTE |= nBitFlag;	else PORTE.PCR.BYTE &= ~nBitFlag;	break;
		case P_PH:	if (bPullup) PORTH.PCR.BYTE |= nBitFlag;	else PORTH.PCR.BYTE &= ~nBitFlag;	break;
		case P_PJ:	if (bPullup) PORTJ.PCR.BYTE |= nBitFlag;	else PORTJ.PCR.BYTE &= ~nBitFlag;	break;
	}
}

////////////////////////////////////////
// ポート切替レジスタAの設定
//	flag	: PSRA_*
//
void CDioObject::SetPSRA(unsigned char flag)
{
	PORT.PSRA.BYTE = flag;
}

////////////////////////////////////////
// ポート切替レジスタBの設定
//	flag	: PSRB_*
//
void CDioObject::SetPSRB(unsigned char flag)
{
	PORT.PSRB.BYTE = flag;
}

////////////////////////////////////////////////////////////////////////////////
// マルチファンクションピンコントローラ
//

////////////////////////////////////////
// プロテクトレジスタ書き込みの設定
//	bEnable	: true(許可) ／ false(禁止)
//
void CMpcObject::_EnableWriteForPFS(bool bEnable)
{
	MPC.PWPR.BIT.B0WI  = 0;
	MPC.PWPR.BIT.PFSWE = bEnable ? 1 : 0;
	MPC.PWPR.BIT.B0WI  = 1;
}

////////////////////////////////////////
// ポートファンクションの設定
//	ps		: ポート
//	pf		: 周辺機能
//
void CMpcObject::SetPortFunction(PORTSEL ps, PORTFUNC pf)
{
	_EnableWriteForPFS(true);

	// P03PFS
	if (ps == P_P03) {
		switch (pf) {
			case NO_DA:			MPC.P03PFS.BIT.ASEL = 0;	break;
			case DA0:			MPC.P03PFS.BIT.ASEL = 1;	break;
		}
	}

	// P05PFS
	if (ps == P_P05) {
		switch (pf) {
			case NO_DA:			MPC.P05PFS.BIT.ASEL = 0;	break;
			case DA1:			MPC.P05PFS.BIT.ASEL = 1;	break;
		}
	}

	// P14PFS
	if (ps == P_P14) {
		switch (pf) {
			case NO_IRQ:		MPC.P14PFS.BIT.ISEL = 0;	break;
			case IRQ4:			MPC.P14PFS.BIT.ISEL = 1;	break;
			case NO_AD:			MPC.P14PFS.BIT.ASEL = 0;	break;
			case CVREFB2:		MPC.P14PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.P14PFS.BIT.PSEL = 0x00;	break;
			case MTIOC3A:		MPC.P14PFS.BIT.PSEL = 0x01;	break;
			case MTCLKA:		MPC.P14PFS.BIT.PSEL = 0x02;	break;
			case TIOCB5:		MPC.P14PFS.BIT.PSEL = 0x03;	break;
			case TCLKA:			MPC.P14PFS.BIT.PSEL = 0x04;	break;
			case TMRI2:			MPC.P14PFS.BIT.PSEL = 0x05;	break;
			case CTS1:			MPC.P14PFS.BIT.PSEL = 0x0B;	break;
			case RTS1:			MPC.P14PFS.BIT.PSEL = 0x0B;	break;
			case SS1:			MPC.P14PFS.BIT.PSEL = 0x0B;	break;
			case CTXD0:			MPC.P14PFS.BIT.PSEL = 0x10;	break;
			case USB0_OVRCURA:	MPC.P14PFS.BIT.PSEL = 0x11;	break;
			case TS13:			MPC.P14PFS.BIT.PSEL = 0x19;	break;
		}
	}

	// P15PFS
	if (ps == P_P15) {
		switch (pf) {
			case NO_IRQ:		MPC.P15PFS.BIT.ISEL = 0;	break;
			case IRQ5:			MPC.P15PFS.BIT.ISEL = 1;	break;
			case NO_AD:			MPC.P15PFS.BIT.ASEL = 0;	break;
			case CMPB2:			MPC.P15PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.P15PFS.BIT.PSEL = 0x00;	break;
			case MTIOC0B:		MPC.P15PFS.BIT.PSEL = 0x01;	break;
			case MTCLKB:		MPC.P15PFS.BIT.PSEL = 0x02;	break;
			case TIOCB2:		MPC.P15PFS.BIT.PSEL = 0x03;	break;
			case TCLKB:			MPC.P15PFS.BIT.PSEL = 0x04;	break;
			case TMCI2:			MPC.P15PFS.BIT.PSEL = 0x05;	break;
			case RXD1:			MPC.P15PFS.BIT.PSEL = 0x0A;	break;
			case SMISO1:		MPC.P15PFS.BIT.PSEL = 0x0A;	break;
			case SSCL1:			MPC.P15PFS.BIT.PSEL = 0x0A;	break;
			case CRXD0:			MPC.P15PFS.BIT.PSEL = 0x10;	break;
			case TS12:			MPC.P15PFS.BIT.PSEL = 0x19;	break;
		}
	}

	// P16PFS
	if (ps == P_P16) {
		switch (pf) {
			case NO_IRQ:		MPC.P16PFS.BIT.ISEL = 0;	break;
			case IRQ6:			MPC.P16PFS.BIT.ISEL = 1;	break;
			case HI_Z:			MPC.P16PFS.BIT.PSEL = 0x00;	break;
			case MTIOC3C:		MPC.P16PFS.BIT.PSEL = 0x01;	break;
			case MTIOC3D:		MPC.P16PFS.BIT.PSEL = 0x02;	break;
			case TIOCB1:		MPC.P16PFS.BIT.PSEL = 0x03;	break;
			case TCLKC:			MPC.P16PFS.BIT.PSEL = 0x04;	break;
			case TMO2:			MPC.P16PFS.BIT.PSEL = 0x05;	break;
			case RTCOUT:		MPC.P16PFS.BIT.PSEL = 0x07;	break;
			case ADTRG0:		MPC.P16PFS.BIT.PSEL = 0x09;	break;
			case TXD1:			MPC.P16PFS.BIT.PSEL = 0x0A;	break;
			case SMOSI1:		MPC.P16PFS.BIT.PSEL = 0x0A;	break;
			case SSDA1:			MPC.P16PFS.BIT.PSEL = 0x0A;	break;
			case MOSIA:			MPC.P16PFS.BIT.PSEL = 0x0D;	break;
			case SCL:			MPC.P16PFS.BIT.PSEL = 0x0F;	break;
			case USB0_VBUS:		MPC.P16PFS.BIT.PSEL = 0x11;	break;
			case USB0_VBUSEN:	MPC.P16PFS.BIT.PSEL = 0x12;	break;
			case USB0_OVRCURB:	MPC.P16PFS.BIT.PSEL = 0x13;	break;
		}
	}

	// P17PFS
	if (ps == P_P17) {
		switch (pf) {
			case NO_IRQ:		MPC.P17PFS.BIT.ISEL = 0;	break;
			case IRQ7:			MPC.P17PFS.BIT.ISEL = 1;	break;
			case HI_Z:			MPC.P17PFS.BIT.PSEL = 0x00;	break;
			case MTIOC3A:		MPC.P17PFS.BIT.PSEL = 0x01;	break;
			case MTIOC3B:		MPC.P17PFS.BIT.PSEL = 0x02;	break;
			case TIOCB0:		MPC.P17PFS.BIT.PSEL = 0x03;	break;
			case TCLKD:			MPC.P17PFS.BIT.PSEL = 0x04;	break;
			case TMO1:			MPC.P17PFS.BIT.PSEL = 0x05;	break;
			case POE8:			MPC.P17PFS.BIT.PSEL = 0x07;	break;
			case SCK1:			MPC.P17PFS.BIT.PSEL = 0x0A;	break;
			case MISOA:			MPC.P17PFS.BIT.PSEL = 0x0D;	break;
			case SDA:			MPC.P17PFS.BIT.PSEL = 0x0F;	break;
			case CMPOB2:		MPC.P17PFS.BIT.PSEL = 0x10;	break;
			case SSITXD0:		MPC.P17PFS.BIT.PSEL = 0x17;	break;
		}
	}

	// P26PFS
	if (ps == P_P26) {
		switch (pf) {
			case NO_AD:			MPC.P26PFS.BIT.ASEL = 0;	break;
			case CMPB3:			MPC.P26PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.P26PFS.BIT.PSEL = 0x00;	break;
			case MTIOC2A:		MPC.P26PFS.BIT.PSEL = 0x01;	break;
			case TMO1:			MPC.P26PFS.BIT.PSEL = 0x05;	break;
			case TXD1:			MPC.P26PFS.BIT.PSEL = 0x0A;	break;
			case SMOSI1:		MPC.P26PFS.BIT.PSEL = 0x0A;	break;
			case SSDA1:			MPC.P26PFS.BIT.PSEL = 0x0A;	break;
			case USB0_VBUSEN:	MPC.P26PFS.BIT.PSEL = 0x11;	break;
			case SSIRXD0:		MPC.P26PFS.BIT.PSEL = 0x17;	break;
			case TS3:			MPC.P26PFS.BIT.PSEL = 0x19;	break;
		}
	}

	// P27PFS
	if (ps == P_P27) {
		switch (pf) {
			case NO_AD:			MPC.P27PFS.BIT.ASEL = 0;	break;
			case CVREFB3:		MPC.P27PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.P27PFS.BIT.PSEL = 0x00;	break;
			case MTIOC2B:		MPC.P27PFS.BIT.PSEL = 0x01;	break;
			case TMCI3:			MPC.P27PFS.BIT.PSEL = 0x05;	break;
			case SCK1:			MPC.P27PFS.BIT.PSEL = 0x0A;	break;
			case SSIWS0:		MPC.P27PFS.BIT.PSEL = 0x17;	break;
			case TS2:			MPC.P27PFS.BIT.PSEL = 0x19;	break;
		}
	}

	// P30PFS
	if (ps == P_P30) {
		switch (pf) {
			case NO_IRQ:		MPC.P30PFS.BIT.ISEL = 0;	break;
			case IRQ0:			MPC.P30PFS.BIT.ISEL = 1;	break;
			case HI_Z:			MPC.P30PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4B:		MPC.P30PFS.BIT.PSEL = 0x01;	break;
			case TMRI3:			MPC.P30PFS.BIT.PSEL = 0x05;	break;
			case POE8:			MPC.P30PFS.BIT.PSEL = 0x07;	break;
			case RXD1:			MPC.P30PFS.BIT.PSEL = 0x0A;	break;
			case SMISO1:		MPC.P30PFS.BIT.PSEL = 0x0A;	break;
			case SSCL1:			MPC.P30PFS.BIT.PSEL = 0x0A;	break;
			case CMPOB3:		MPC.P30PFS.BIT.PSEL = 0x10;	break;
			case AUDIO_MCLK:	MPC.P30PFS.BIT.PSEL = 0x17;	break;
		}
	}

	// P31PFS
	if (ps == P_P31) {
		switch (pf) {
			case NO_IRQ:		MPC.P31PFS.BIT.ISEL = 0;	break;
			case IRQ1:			MPC.P31PFS.BIT.ISEL = 1;	break;
			case HI_Z:			MPC.P31PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4D:		MPC.P31PFS.BIT.PSEL = 0x01;	break;
			case TMCI2:			MPC.P31PFS.BIT.PSEL = 0x05;	break;
			case CTS1:			MPC.P31PFS.BIT.PSEL = 0x0B;	break;
			case RTS1:			MPC.P31PFS.BIT.PSEL = 0x0B;	break;
			case SS1:			MPC.P31PFS.BIT.PSEL = 0x0B;	break;
			case SSISCK0:		MPC.P31PFS.BIT.PSEL = 0x17;	break;
		}
	}

	// P40PFS
	if (ps == P_P40) {
		switch (pf) {
			case NO_AD:			MPC.P40PFS.BIT.ASEL = 0;	break;
			case AN000:			MPC.P40PFS.BIT.ASEL = 1;	break;
		}
	}

	// P41PFS
	if (ps == P_P41) {
		switch (pf) {
			case NO_AD:			MPC.P41PFS.BIT.ASEL = 0;	break;
			case AN001:			MPC.P41PFS.BIT.ASEL = 1;	break;
		}
	}

	// P42PFS
	if (ps == P_P42) {
		switch (pf) {
			case NO_AD:			MPC.P42PFS.BIT.ASEL = 0;	break;
			case AN002:			MPC.P42PFS.BIT.ASEL = 1;	break;
		}
	}

	// P43PFS
	if (ps == P_P43) {
		switch (pf) {
			case NO_AD:			MPC.P43PFS.BIT.ASEL = 0;	break;
			case AN003:			MPC.P43PFS.BIT.ASEL = 1;	break;
		}
	}

	// P44PFS
	if (ps == P_P44) {
		switch (pf) {
			case NO_AD:			MPC.P44PFS.BIT.ASEL = 0;	break;
			case AN004:			MPC.P44PFS.BIT.ASEL = 1;	break;
		}
	}

	// P46PFS
	if (ps == P_P46) {
		switch (pf) {
			case NO_AD:			MPC.P46PFS.BIT.ASEL = 0;	break;
			case AN006:			MPC.P46PFS.BIT.ASEL = 1;	break;
		}
	}

	// P54PFS
	if (ps == P_P54) {
		switch (pf) {
			case HI_Z:			MPC.P54PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4B:		MPC.P54PFS.BIT.PSEL = 0x01;	break;
			case TMCI1:			MPC.P54PFS.BIT.PSEL = 0x05;	break;
			case CTXD0:			MPC.P54PFS.BIT.PSEL = 0x10;	break;
			case TS16:			MPC.P54PFS.BIT.PSEL = 0x19;	break;
		}
	}

	// P55PFS
	if (ps == P_P55) {
		switch (pf) {
			case HI_Z:			MPC.P55PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4D:		MPC.P55PFS.BIT.PSEL = 0x01;	break;
			case TMO3:			MPC.P55PFS.BIT.PSEL = 0x05;	break;
			case CRXD0:			MPC.P55PFS.BIT.PSEL = 0x10;	break;
			case TS15:			MPC.P55PFS.BIT.PSEL = 0x19;	break;
		}
	}

	// PA0PFS
	if (ps == P_PA0) {
		switch (pf) {
			case HI_Z:			MPC.PA0PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4A:		MPC.PA0PFS.BIT.PSEL = 0x01;	break;
			case TIOCA0:		MPC.PA0PFS.BIT.PSEL = 0x03;	break;
			case CACREF:		MPC.PA0PFS.BIT.PSEL = 0x07;	break;
			case SSLA1:			MPC.PA0PFS.BIT.PSEL = 0x0D;	break;
		}
	}

	// PA1PFS
	if (ps == P_PA1) {
		switch (pf) {
			case HI_Z:			MPC.PA1PFS.BIT.PSEL = 0x00;	break;
			case MTIOC0B:		MPC.PA1PFS.BIT.PSEL = 0x01;	break;
			case MTCLKC:		MPC.PA1PFS.BIT.PSEL = 0x02;	break;
			case TIOCB0:		MPC.PA1PFS.BIT.PSEL = 0x03;	break;
			case SCK5:			MPC.PA1PFS.BIT.PSEL = 0x0A;	break;
			case SSLA2:			MPC.PA1PFS.BIT.PSEL = 0x0D;	break;
			case SSISCK0:		MPC.PA1PFS.BIT.PSEL = 0x17;	break;
		}
	}

	// PA3PFS
	if (ps == P_PA3) {
		switch (pf) {
			case NO_IRQ:		MPC.PA3PFS.BIT.ISEL = 0;	break;
			case IRQ6:			MPC.PA3PFS.BIT.ISEL = 1;	break;
			case NO_AD:			MPC.PA3PFS.BIT.ASEL = 0;	break;
			case CMPB1:			MPC.PA3PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.PA3PFS.BIT.PSEL = 0x00;	break;
			case MTIOC0D:		MPC.PA3PFS.BIT.PSEL = 0x01;	break;
			case MTCLKD:		MPC.PA3PFS.BIT.PSEL = 0x02;	break;
			case TIOCD0:		MPC.PA3PFS.BIT.PSEL = 0x03;	break;
			case TCLKB:			MPC.PA3PFS.BIT.PSEL = 0x04;	break;
			case RXD5:			MPC.PA3PFS.BIT.PSEL = 0x0A;	break;
			case SMISO5:		MPC.PA3PFS.BIT.PSEL = 0x0A;	break;
			case SSCL5:			MPC.PA3PFS.BIT.PSEL = 0x0A;	break;
			case SSIRXD0:		MPC.PA3PFS.BIT.PSEL = 0x17;	break;
		}
	}

	// PA4PFS
	if (ps == P_PA4) {
		switch (pf) {
			case NO_IRQ:		MPC.PA4PFS.BIT.ISEL = 0;	break;
			case IRQ5:			MPC.PA4PFS.BIT.ISEL = 1;	break;
			case NO_AD:			MPC.PA4PFS.BIT.ASEL = 0;	break;
			case CVREFB1:		MPC.PA4PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.PA4PFS.BIT.PSEL = 0x00;	break;
			case MTIC5U:		MPC.PA4PFS.BIT.PSEL = 0x01;	break;
			case MTCLKA:		MPC.PA4PFS.BIT.PSEL = 0x02;	break;
			case TIOCA1:		MPC.PA4PFS.BIT.PSEL = 0x03;	break;
			case TMRI0:			MPC.PA4PFS.BIT.PSEL = 0x05;	break;
			case TXD5:			MPC.PA4PFS.BIT.PSEL = 0x0A;	break;
			case SMOSI5:		MPC.PA4PFS.BIT.PSEL = 0x0A;	break;
			case SSDA5:			MPC.PA4PFS.BIT.PSEL = 0x0A;	break;
			case SSLA0:			MPC.PA4PFS.BIT.PSEL = 0x0D;	break;
			case SSITXD0:		MPC.PA4PFS.BIT.PSEL = 0x17;	break;
		}
	}

	// PA6PFS
	if (ps == P_PA6) {
		switch (pf) {
			case HI_Z:			MPC.PA6PFS.BIT.PSEL = 0x00;	break;
			case MTIC5V:		MPC.PA6PFS.BIT.PSEL = 0x01;	break;
			case MTCLKB:		MPC.PA6PFS.BIT.PSEL = 0x02;	break;
			case TIOCA2:		MPC.PA6PFS.BIT.PSEL = 0x03;	break;
			case TMCI3:			MPC.PA6PFS.BIT.PSEL = 0x05;	break;
			case POE2:			MPC.PA6PFS.BIT.PSEL = 0x07;	break;
			case CTS5:			MPC.PA6PFS.BIT.PSEL = 0x0B;	break;
			case RTS5:			MPC.PA6PFS.BIT.PSEL = 0x0B;	break;
			case SS5:			MPC.PA6PFS.BIT.PSEL = 0x0B;	break;
			case MOSIA:			MPC.PA6PFS.BIT.PSEL = 0x0D;	break;
			case SSIWS0:		MPC.PA6PFS.BIT.PSEL = 0x17;	break;
		}
	}

	// PB0PFS
	if (ps == P_PB0) {
		switch (pf) {
			case HI_Z:			MPC.PB0PFS.BIT.PSEL = 0x00;	break;
			case MTIC5W:		MPC.PB0PFS.BIT.PSEL = 0x01;	break;
			case TIOCA3:		MPC.PB0PFS.BIT.PSEL = 0x03;	break;
			case RXD6:			MPC.PB0PFS.BIT.PSEL = 0x0B;	break;
			case SMISO6:		MPC.PB0PFS.BIT.PSEL = 0x0B;	break;
			case SSCL6:			MPC.PB0PFS.BIT.PSEL = 0x0B;	break;
			case RSPCKA:		MPC.PB0PFS.BIT.PSEL = 0x0D;	break;
			case SDHI_CMD:		MPC.PB0PFS.BIT.PSEL = 0x1A;	break;
		}
	}

	// PB1PFS
	if (ps == P_PB1) {
		switch (pf) {
			case NO_IRQ:		MPC.PB1PFS.BIT.ISEL = 0;	break;
			case IRQ4:			MPC.PB1PFS.BIT.ISEL = 1;	break;
			case HI_Z:			MPC.PB1PFS.BIT.PSEL = 0x00;	break;
			case MTIOC0C:		MPC.PB1PFS.BIT.PSEL = 0x01;	break;
			case MTIOC4C:		MPC.PB1PFS.BIT.PSEL = 0x02;	break;
			case TIOCB3:		MPC.PB1PFS.BIT.PSEL = 0x03;	break;
			case TMCI0:			MPC.PB1PFS.BIT.PSEL = 0x05;	break;
			case TXD6:			MPC.PB1PFS.BIT.PSEL = 0x0B;	break;
			case SMOSI6:		MPC.PB1PFS.BIT.PSEL = 0x0B;	break;
			case SSDA6:			MPC.PB1PFS.BIT.PSEL = 0x0B;	break;
			case CMPOB1:		MPC.PB1PFS.BIT.PSEL = 0x10;	break;
			case SDHI_CLK:		MPC.PB1PFS.BIT.PSEL = 0x1A;	break;
		}
	}

	// PB3PFS
	if (ps == P_PB3) {
		switch (pf) {
			case HI_Z:			MPC.PB3PFS.BIT.PSEL = 0x00;	break;
			case MTIOC0A:		MPC.PB3PFS.BIT.PSEL = 0x01;	break;
			case MTIOC4A:		MPC.PB3PFS.BIT.PSEL = 0x02;	break;
			case TIOCD3:		MPC.PB3PFS.BIT.PSEL = 0x03;	break;
			case TCLKD:			MPC.PB3PFS.BIT.PSEL = 0x04;	break;
			case TMO0:			MPC.PB3PFS.BIT.PSEL = 0x05;	break;
			case POE3:			MPC.PB3PFS.BIT.PSEL = 0x07;	break;
			case SCK6:			MPC.PB3PFS.BIT.PSEL = 0x0B;	break;
			case SDHI_WP:		MPC.PB3PFS.BIT.PSEL = 0x1A;	break;
		}
	}

	// PB5PFS
	if (ps == P_PB5) {
		switch (pf) {
			case HI_Z:			MPC.PB5PFS.BIT.PSEL = 0x00;	break;
			case MTIOC2A:		MPC.PB5PFS.BIT.PSEL = 0x01;	break;
			case MTIOC1B:		MPC.PB5PFS.BIT.PSEL = 0x02;	break;
			case TIOCB4:		MPC.PB5PFS.BIT.PSEL = 0x03;	break;
			case TMRI1:			MPC.PB5PFS.BIT.PSEL = 0x05;	break;
			case POE1:			MPC.PB5PFS.BIT.PSEL = 0x07;	break;
			case SCK9:			MPC.PB5PFS.BIT.PSEL = 0x0A;	break;
			case USB0_VBUS:		MPC.PB5PFS.BIT.PSEL = 0x11;	break;
			case SDHI_CD:		MPC.PB5PFS.BIT.PSEL = 0x1A;	break;
		}
	}

	// PB6PFS
	if (ps == P_PB6) {
		switch (pf) {
			case HI_Z:			MPC.PB6PFS.BIT.PSEL = 0x00;	break;
			case MTIOC3D:		MPC.PB6PFS.BIT.PSEL = 0x01;	break;
			case TIOCA5:		MPC.PB6PFS.BIT.PSEL = 0x03;	break;
			case RXD9:			MPC.PB6PFS.BIT.PSEL = 0x0A;	break;
			case SMISO9:		MPC.PB6PFS.BIT.PSEL = 0x0A;	break;
			case SSCL9:			MPC.PB6PFS.BIT.PSEL = 0x0A;	break;
			case SDHI_D1:		MPC.PB6PFS.BIT.PSEL = 0x1A;	break;
		}
	}

	// PB7PFS
	if (ps == P_PB7) {
		switch (pf) {
			case HI_Z:			MPC.PB7PFS.BIT.PSEL = 0x00;	break;
			case MTIOC3B:		MPC.PB7PFS.BIT.PSEL = 0x01;	break;
			case TIOCB5:		MPC.PB7PFS.BIT.PSEL = 0x03;	break;
			case TXD9:			MPC.PB7PFS.BIT.PSEL = 0x0A;	break;
			case SMOSI9:		MPC.PB7PFS.BIT.PSEL = 0x0A;	break;
			case SSDA9:			MPC.PB7PFS.BIT.PSEL = 0x0A;	break;
			case SDHI_D2:		MPC.PB7PFS.BIT.PSEL = 0x1A;	break;
		}
	}

	// PC2PFS
	if (ps == P_PC2) {
		switch (pf) {
			case HI_Z:			MPC.PC2PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4B:		MPC.PC2PFS.BIT.PSEL = 0x01;	break;
			case TCLKA:			MPC.PC2PFS.BIT.PSEL = 0x03;	break;
			case RXD5:			MPC.PC2PFS.BIT.PSEL = 0x0A;	break;
			case SMISO5:		MPC.PC2PFS.BIT.PSEL = 0x0A;	break;
			case SSCL5:			MPC.PC2PFS.BIT.PSEL = 0x0A;	break;
			case SSLA3:			MPC.PC2PFS.BIT.PSEL = 0x0D;	break;
			case TS30:			MPC.PC2PFS.BIT.PSEL = 0x19;	break;
			case SDHI_D3:		MPC.PC2PFS.BIT.PSEL = 0x1A;	break;
		}
	}

	// PC3PFS
	if (ps == P_PC3) {
		switch (pf) {
			case HI_Z:			MPC.PC3PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4D:		MPC.PC3PFS.BIT.PSEL = 0x01;	break;
			case TCLKB:			MPC.PC3PFS.BIT.PSEL = 0x03;	break;
			case TXD5:			MPC.PC3PFS.BIT.PSEL = 0x0A;	break;
			case SMOSI5:		MPC.PC3PFS.BIT.PSEL = 0x0A;	break;
			case SSDA5:			MPC.PC3PFS.BIT.PSEL = 0x0A;	break;
			case TS27:			MPC.PC3PFS.BIT.PSEL = 0x19;	break;
			case SDHI_D0:		MPC.PC3PFS.BIT.PSEL = 0x1A;	break;
		}
	}

	// PC4PFS
	if (ps == P_PC4) {
		switch (pf) {
			case HI_Z:			MPC.PC4PFS.BIT.PSEL = 0x00;	break;
			case MTIOC3D:		MPC.PC4PFS.BIT.PSEL = 0x01;	break;
			case MTCLKC:		MPC.PC4PFS.BIT.PSEL = 0x02;	break;
			case TMCI1:			MPC.PC4PFS.BIT.PSEL = 0x05;	break;
			case POE0:			MPC.PC4PFS.BIT.PSEL = 0x07;	break;
			case SCK5:			MPC.PC4PFS.BIT.PSEL = 0x0A;	break;
			case CTS8:			MPC.PC4PFS.BIT.PSEL = 0x0B;	break;
			case RTS8:			MPC.PC4PFS.BIT.PSEL = 0x0B;	break;
			case SS8:			MPC.PC4PFS.BIT.PSEL = 0x0B;	break;
			case SSLA0:			MPC.PC4PFS.BIT.PSEL = 0x0D;	break;
			case TSCAP:			MPC.PC4PFS.BIT.PSEL = 0x19;	break;
			case SDHI_D1:		MPC.PC4PFS.BIT.PSEL = 0x1A;	break;
		}
	}

	// PC5PFS
	if (ps == P_PC5) {
		switch (pf) {
			case HI_Z:			MPC.PC5PFS.BIT.PSEL = 0x00;	break;
			case MTIOC3B:		MPC.PC5PFS.BIT.PSEL = 0x01;	break;
			case MTCLKD:		MPC.PC5PFS.BIT.PSEL = 0x02;	break;
			case TMRI2:			MPC.PC5PFS.BIT.PSEL = 0x05;	break;
			case SCK8:			MPC.PC5PFS.BIT.PSEL = 0x0A;	break;
			case RSPCKA:		MPC.PC5PFS.BIT.PSEL = 0x0D;	break;
			case USB0_ID:		MPC.PC5PFS.BIT.PSEL = 0x11;	break;
			case TS23:			MPC.PC5PFS.BIT.PSEL = 0x19;	break;
		}
	}

	// PC6PFS
	if (ps == P_PC6) {
		switch (pf) {
			case HI_Z:			MPC.PC6PFS.BIT.PSEL = 0x00;	break;
			case MTIOC3C:		MPC.PC6PFS.BIT.PSEL = 0x01;	break;
			case MTCLKA:		MPC.PC6PFS.BIT.PSEL = 0x02;	break;
			case TMCI2:			MPC.PC6PFS.BIT.PSEL = 0x05;	break;
			case RXD8:			MPC.PC6PFS.BIT.PSEL = 0x0A;	break;
			case SMISO8:		MPC.PC6PFS.BIT.PSEL = 0x0A;	break;
			case SSCL8:			MPC.PC6PFS.BIT.PSEL = 0x0A;	break;
			case MOSIA:			MPC.PC6PFS.BIT.PSEL = 0x0D;	break;
			case USB0_EXICEN:	MPC.PC6PFS.BIT.PSEL = 0x11;	break;
			case TS22:			MPC.PC6PFS.BIT.PSEL = 0x19;	break;
		}
	}

	// PC7PFS
	if (ps == P_PC7) {
		switch (pf) {
			case HI_Z:			MPC.PC7PFS.BIT.PSEL = 0x00;	break;
			case MTIOC3A:		MPC.PC7PFS.BIT.PSEL = 0x01;	break;
			case MTCLKB:		MPC.PC7PFS.BIT.PSEL = 0x02;	break;
			case TMO2:			MPC.PC7PFS.BIT.PSEL = 0x05;	break;
			case CACREF:		MPC.PC7PFS.BIT.PSEL = 0x07;	break;
			case TXD8:			MPC.PC7PFS.BIT.PSEL = 0x0A;	break;
			case SMOSI8:		MPC.PC7PFS.BIT.PSEL = 0x0A;	break;
			case SSDA8:			MPC.PC7PFS.BIT.PSEL = 0x0A;	break;
			case MISOA:			MPC.PC7PFS.BIT.PSEL = 0x0D;	break;
		}
	}

	// PE0PFS
	if (ps == P_PE0) {
		switch (pf) {
			case NO_AD:			MPC.PE0PFS.BIT.ASEL = 0;	break;
			case AN016:			MPC.PE0PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.PE0PFS.BIT.PSEL = 0x00;	break;
			case SCK12:			MPC.PE0PFS.BIT.PSEL = 0x0C;	break;
		}
	}

	// PE1PFS
	if (ps == P_PE1) {
		switch (pf) {
			case NO_AD:			MPC.PE1PFS.BIT.ASEL = 0;	break;
			case AN017:			MPC.PE1PFS.BIT.ASEL = 1;	break;
			case CMPB0:			MPC.PE1PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.PE1PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4C:		MPC.PE1PFS.BIT.PSEL = 0x01;	break;
			case TXD12:			MPC.PE1PFS.BIT.PSEL = 0x0C;	break;
			case TXDX12:		MPC.PE1PFS.BIT.PSEL = 0x0C;	break;
			case SIOX12:		MPC.PE1PFS.BIT.PSEL = 0x0C;	break;
			case SMOSI12:		MPC.PE1PFS.BIT.PSEL = 0x0C;	break;
			case SSDA12:		MPC.PE1PFS.BIT.PSEL = 0x0C;	break;
		}
	}

	// PE2PFS
	if (ps == P_PE2) {
		switch (pf) {
			case NO_IRQ:		MPC.PE2PFS.BIT.ISEL = 0;	break;
			case IRQ7:			MPC.PE2PFS.BIT.ISEL = 1;	break;
			case NO_AD:			MPC.PE2PFS.BIT.ASEL = 0;	break;
			case AN018:			MPC.PE2PFS.BIT.ASEL = 1;	break;
			case CVREFB0:		MPC.PE2PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.PE2PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4A:		MPC.PE2PFS.BIT.PSEL = 0x01;	break;
			case RXD12:			MPC.PE2PFS.BIT.PSEL = 0x0C;	break;
			case RXDX12:		MPC.PE2PFS.BIT.PSEL = 0x0C;	break;
			case SMISO12:		MPC.PE2PFS.BIT.PSEL = 0x0C;	break;
			case SSCL12:		MPC.PE2PFS.BIT.PSEL = 0x0C;	break;
		}
	}

	// PE3PFS
	if (ps == P_PE3) {
		switch (pf) {
			case NO_AD:			MPC.PE3PFS.BIT.ASEL = 0;	break;
			case AN019:			MPC.PE3PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.PE3PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4B:		MPC.PE3PFS.BIT.PSEL = 0x01;	break;
			case POE8:			MPC.PE3PFS.BIT.PSEL = 0x07;	break;
			case CLKOUT:		MPC.PE3PFS.BIT.PSEL = 0x09;	break;
			case CTS12:			MPC.PE3PFS.BIT.PSEL = 0x0C;	break;
			case RTS12:			MPC.PE3PFS.BIT.PSEL = 0x0C;	break;
			case SS12:			MPC.PE3PFS.BIT.PSEL = 0x0C;	break;
			case AUDIO_MCLK:	MPC.PE3PFS.BIT.PSEL = 0x17;	break;
		}
	}

	// PE4PFS
	if (ps == P_PE4) {
		switch (pf) {
			case NO_AD:			MPC.PE4PFS.BIT.ASEL = 0;	break;
			case AN020:			MPC.PE4PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.PE4PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4D:		MPC.PE4PFS.BIT.PSEL = 0x01;	break;
			case MTIOC1A:		MPC.PE4PFS.BIT.PSEL = 0x02;	break;
			case CLKOUT:		MPC.PE4PFS.BIT.PSEL = 0x09;	break;
		}
	}

	// PE5PFS
	if (ps == P_PE5) {
		switch (pf) {
			case NO_IRQ:		MPC.PE5PFS.BIT.ISEL = 0;	break;
			case IRQ5:			MPC.PE5PFS.BIT.ISEL = 1;	break;
			case NO_AD:			MPC.PE5PFS.BIT.ASEL = 0;	break;
			case AN021:			MPC.PE5PFS.BIT.ASEL = 1;	break;
			case HI_Z:			MPC.PE5PFS.BIT.PSEL = 0x00;	break;
			case MTIOC4C:		MPC.PE5PFS.BIT.PSEL = 0x01;	break;
			case MTIOC2B:		MPC.PE5PFS.BIT.PSEL = 0x02;	break;
			case CMPOB0:		MPC.PE5PFS.BIT.PSEL = 0x10;	break;
		}
	}

	// PH0PFS
	if (ps == P_PH0) {
		switch (pf) {
			case HI_Z:			MPC.PH0PFS.BIT.PSEL = 0x00;	break;
			case CACREF:		MPC.PH0PFS.BIT.PSEL = 0x07;	break;
		}
	}

	// PH1PFS
	if (ps == P_PH1) {
		switch (pf) {
			case NO_IRQ:		MPC.PH1PFS.BIT.ISEL = 0;	break;
			case IRQ0:			MPC.PH1PFS.BIT.ISEL = 1;	break;
			case HI_Z:			MPC.PH1PFS.BIT.PSEL = 0x00;	break;
			case TMO0:			MPC.PH1PFS.BIT.PSEL = 0x05;	break;
		}
	}

	// PH2PFS
	if (ps == P_PH2) {
		switch (pf) {
			case NO_IRQ:		MPC.PH2PFS.BIT.ISEL = 0;	break;
			case IRQ1:			MPC.PH2PFS.BIT.ISEL = 1;	break;
			case HI_Z:			MPC.PH2PFS.BIT.PSEL = 0x00;	break;
			case TMRI0:			MPC.PH2PFS.BIT.PSEL = 0x05;	break;
		}
	}

	// PH3PFS
	if (ps == P_PH3) {
		switch (pf) {
			case HI_Z:			MPC.PH3PFS.BIT.PSEL = 0x00;	break;
			case TMCI0:			MPC.PH3PFS.BIT.PSEL = 0x05;	break;
		}
	}

	_EnableWriteForPFS(false);
}

////////////////////////////////////////
// 周辺機能の設定
//	port	: ポート
//	num		: ビット番号(0 ～ 7)
//	bEnable	: true(周辺機能) ／ false(汎用入出力)
//
void CMpcObject::UsePeripheralPort(PORTSEL ps, int num, bool bEnable)
{
	unsigned char nBitFlag = 0x01 << num;

	switch (ps) {
		case P_P0:	if (bEnable) PORT0.PMR.BYTE |= nBitFlag;	else PORT0.PMR.BYTE &= ~nBitFlag;	break;
		case P_P1:	if (bEnable) PORT1.PMR.BYTE |= nBitFlag;	else PORT1.PMR.BYTE &= ~nBitFlag;	break;
		case P_P2:	if (bEnable) PORT2.PMR.BYTE |= nBitFlag;	else PORT2.PMR.BYTE &= ~nBitFlag;	break;
		case P_P3:	if (bEnable) PORT3.PMR.BYTE |= nBitFlag;	else PORT3.PMR.BYTE &= ~nBitFlag;	break;
		case P_P4:	if (bEnable) PORT4.PMR.BYTE |= nBitFlag;	else PORT4.PMR.BYTE &= ~nBitFlag;	break;
		case P_P5:	if (bEnable) PORT5.PMR.BYTE |= nBitFlag;	else PORT5.PMR.BYTE &= ~nBitFlag;	break;
		case P_PA:	if (bEnable) PORTA.PMR.BYTE |= nBitFlag;	else PORTA.PMR.BYTE &= ~nBitFlag;	break;
		case P_PB:	if (bEnable) PORTB.PMR.BYTE |= nBitFlag;	else PORTB.PMR.BYTE &= ~nBitFlag;	break;
		case P_PC:	if (bEnable) PORTC.PMR.BYTE |= nBitFlag;	else PORTC.PMR.BYTE &= ~nBitFlag;	break;
		case P_PD:	if (bEnable) PORTD.PMR.BYTE |= nBitFlag;	else PORTD.PMR.BYTE &= ~nBitFlag;	break;
		case P_PE:	if (bEnable) PORTE.PMR.BYTE |= nBitFlag;	else PORTE.PMR.BYTE &= ~nBitFlag;	break;
		case P_PH:	if (bEnable) PORTH.PMR.BYTE |= nBitFlag;	else PORTH.PMR.BYTE &= ~nBitFlag;	break;
		case P_PJ:	if (bEnable) PORTJ.PMR.BYTE |= nBitFlag;	else PORTJ.PMR.BYTE &= ~nBitFlag;	break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// マルチファンクションタイマパルスユニット2コントローラ
//

////////////////////////////////////////
// MTUモジュールの設定
//	nUnit	: ユニット
//	bRun	: true(開始) ／ false(停止)
//
void CMtuObject::RunModule(unsigned int nUnit, bool bRun)
{
	if (nUnit >= CMtuObject::MTU_UNIT_NUM)
		return;

	if (bRun) {
		CModuleStopObject::RunModule(CModuleStopObject::M_MTU);
	}
	else {
		CModuleStopObject::StopModule(CModuleStopObject::M_MTU);
	}
}

////////////////////////////////////////
// TCRの設定
//	ch		: チャンネル
//	flag	: TCR_*
//
void CMtuObject::SetControl(int ch, unsigned int flag, unsigned int flagv, unsigned int flagw)
{
	unsigned int nTimePrescaler = flag & 0x000F;
	unsigned int nClockEdge     = flag & 0x00F0;
	unsigned int nCounterClear  = flag & 0x0F00;

	switch (ch) {
		case 0:
		{
			switch (nTimePrescaler) {
				case TCR_IN_1:		MTU0.TCR.BIT.TPSC = 0x00;	break;
				case TCR_IN_4:		MTU0.TCR.BIT.TPSC = 0x01;	break;
				case TCR_IN_16:		MTU0.TCR.BIT.TPSC = 0x02;	break;
				case TCR_IN_64:		MTU0.TCR.BIT.TPSC = 0x03;	break;
				case TCR_MTCLKA:	MTU0.TCR.BIT.TPSC = 0x04;	break;
				case TCR_MTCLKB:	MTU0.TCR.BIT.TPSC = 0x05;	break;
				case TCR_MTCLKC:	MTU0.TCR.BIT.TPSC = 0x06;	break;
				case TCR_MTCLKD:	MTU0.TCR.BIT.TPSC = 0x07;	break;
			}
			switch (nClockEdge) {
				case TCR_HIGH_EDGE:	MTU0.TCR.BIT.CKEG = 0x00;	break;
				case TCR_LOW_EDGE:	MTU0.TCR.BIT.CKEG = 0x01;	break;
				case TCR_BOTH_EDGE:	MTU0.TCR.BIT.CKEG = 0x02;	break;
			}
			switch (nCounterClear) {
				case TCR_DISABLE:	MTU0.TCR.BIT.CCLR = 0x00;	break;
				case TCR_TGRA:		MTU0.TCR.BIT.CCLR = 0x01;	break;
				case TCR_TGRB:		MTU0.TCR.BIT.CCLR = 0x02;	break;
				case TCR_TGRC:		MTU0.TCR.BIT.CCLR = 0x05;	break;
				case TCR_TGRD:		MTU0.TCR.BIT.CCLR = 0x06;	break;
				case TCR_OTHER:		MTU0.TCR.BIT.CCLR = 0x07;	break;
			}
			break;
		}
		case 1:
		{
			switch (nTimePrescaler) {
				case TCR_IN_1:		MTU1.TCR.BIT.TPSC = 0x00;	break;
				case TCR_IN_4:		MTU1.TCR.BIT.TPSC = 0x01;	break;
				case TCR_IN_16:		MTU1.TCR.BIT.TPSC = 0x02;	break;
				case TCR_IN_64:		MTU1.TCR.BIT.TPSC = 0x03;	break;
				case TCR_MTCLKA:	MTU1.TCR.BIT.TPSC = 0x04;	break;
				case TCR_MTCLKB:	MTU1.TCR.BIT.TPSC = 0x05;	break;
				case TCR_IN_256:	MTU1.TCR.BIT.TPSC = 0x06;	break;
				case TCR_TCNT:		MTU1.TCR.BIT.TPSC = 0x07;	break;
			}
			switch (nClockEdge) {
				case TCR_HIGH_EDGE:	MTU1.TCR.BIT.CKEG = 0x00;	break;
				case TCR_LOW_EDGE:	MTU1.TCR.BIT.CKEG = 0x01;	break;
				case TCR_BOTH_EDGE:	MTU1.TCR.BIT.CKEG = 0x02;	break;
			}
			switch (nCounterClear) {
				case TCR_DISABLE:	MTU1.TCR.BIT.CCLR = 0x00;	break;
				case TCR_TGRA:		MTU1.TCR.BIT.CCLR = 0x01;	break;
				case TCR_TGRB:		MTU1.TCR.BIT.CCLR = 0x02;	break;
				case TCR_OTHER:		MTU1.TCR.BIT.CCLR = 0x03;	break;
			}
			break;
		}
		case 2:
		{
			switch (nTimePrescaler) {
				case TCR_IN_1:		MTU2.TCR.BIT.TPSC = 0x00;	break;
				case TCR_IN_4:		MTU2.TCR.BIT.TPSC = 0x01;	break;
				case TCR_IN_16:		MTU2.TCR.BIT.TPSC = 0x02;	break;
				case TCR_IN_64:		MTU2.TCR.BIT.TPSC = 0x03;	break;
				case TCR_MTCLKA:	MTU2.TCR.BIT.TPSC = 0x04;	break;
				case TCR_MTCLKB:	MTU2.TCR.BIT.TPSC = 0x05;	break;
				case TCR_MTCLKC:	MTU2.TCR.BIT.TPSC = 0x06;	break;
				case TCR_IN_1024:	MTU2.TCR.BIT.TPSC = 0x07;	break;
			}
			switch (nClockEdge) {
				case TCR_HIGH_EDGE:	MTU2.TCR.BIT.CKEG = 0x00;	break;
				case TCR_LOW_EDGE:	MTU2.TCR.BIT.CKEG = 0x01;	break;
				case TCR_BOTH_EDGE:	MTU2.TCR.BIT.CKEG = 0x02;	break;
			}
			switch (nCounterClear) {
				case TCR_DISABLE:	MTU2.TCR.BIT.CCLR = 0x00;	break;
				case TCR_TGRA:		MTU2.TCR.BIT.CCLR = 0x01;	break;
				case TCR_TGRB:		MTU2.TCR.BIT.CCLR = 0x02;	break;
				case TCR_OTHER:		MTU2.TCR.BIT.CCLR = 0x03;	break;
			}
			break;
		}
		case 3:
		{
			switch (nTimePrescaler) {
				case TCR_IN_1:		MTU3.TCR.BIT.TPSC = 0x00;	break;
				case TCR_IN_4:		MTU3.TCR.BIT.TPSC = 0x01;	break;
				case TCR_IN_16:		MTU3.TCR.BIT.TPSC = 0x02;	break;
				case TCR_IN_64:		MTU3.TCR.BIT.TPSC = 0x03;	break;
				case TCR_IN_256:	MTU3.TCR.BIT.TPSC = 0x04;	break;
				case TCR_IN_1024:	MTU3.TCR.BIT.TPSC = 0x05;	break;
				case TCR_MTCLKA:	MTU3.TCR.BIT.TPSC = 0x06;	break;
				case TCR_MTCLKB:	MTU3.TCR.BIT.TPSC = 0x07;	break;
			}
			switch (nClockEdge) {
				case TCR_HIGH_EDGE:	MTU3.TCR.BIT.CKEG = 0x00;	break;
				case TCR_LOW_EDGE:	MTU3.TCR.BIT.CKEG = 0x01;	break;
				case TCR_BOTH_EDGE:	MTU3.TCR.BIT.CKEG = 0x02;	break;
			}
			switch (nCounterClear) {
				case TCR_DISABLE:	MTU3.TCR.BIT.CCLR = 0x00;	break;
				case TCR_TGRA:		MTU3.TCR.BIT.CCLR = 0x01;	break;
				case TCR_TGRB:		MTU3.TCR.BIT.CCLR = 0x02;	break;
				case TCR_TGRC:		MTU3.TCR.BIT.CCLR = 0x05;	break;
				case TCR_TGRD:		MTU3.TCR.BIT.CCLR = 0x06;	break;
				case TCR_OTHER:		MTU3.TCR.BIT.CCLR = 0x07;	break;
			}
			break;
		}
		case 4:
		{
			switch (nTimePrescaler) {
				case TCR_IN_1:		MTU4.TCR.BIT.TPSC = 0x00;	break;
				case TCR_IN_4:		MTU4.TCR.BIT.TPSC = 0x01;	break;
				case TCR_IN_16:		MTU4.TCR.BIT.TPSC = 0x02;	break;
				case TCR_IN_64:		MTU4.TCR.BIT.TPSC = 0x03;	break;
				case TCR_IN_256:	MTU4.TCR.BIT.TPSC = 0x04;	break;
				case TCR_IN_1024:	MTU4.TCR.BIT.TPSC = 0x05;	break;
				case TCR_MTCLKA:	MTU4.TCR.BIT.TPSC = 0x06;	break;
				case TCR_MTCLKB:	MTU4.TCR.BIT.TPSC = 0x07;	break;
			}
			switch (nClockEdge) {
				case TCR_HIGH_EDGE:	MTU4.TCR.BIT.CKEG = 0x00;	break;
				case TCR_LOW_EDGE:	MTU4.TCR.BIT.CKEG = 0x01;	break;
				case TCR_BOTH_EDGE:	MTU4.TCR.BIT.CKEG = 0x02;	break;
			}
			switch (nCounterClear) {
				case TCR_DISABLE:	MTU4.TCR.BIT.CCLR = 0x00;	break;
				case TCR_TGRA:		MTU4.TCR.BIT.CCLR = 0x01;	break;
				case TCR_TGRB:		MTU4.TCR.BIT.CCLR = 0x02;	break;
				case TCR_TGRC:		MTU4.TCR.BIT.CCLR = 0x05;	break;
				case TCR_TGRD:		MTU4.TCR.BIT.CCLR = 0x06;	break;
				case TCR_OTHER:		MTU4.TCR.BIT.CCLR = 0x07;	break;
			}
			break;
		}
		case 5:
		{
			unsigned int nTimePrescalerV = flagv & 0x000F;
			unsigned int nTimePrescalerW = flagw & 0x000F;

			switch (nTimePrescaler) {
				case TCR_IN_1:		MTU5.TCRU.BIT.TPSC = 0x00;	break;
				case TCR_IN_4:		MTU5.TCRU.BIT.TPSC = 0x01;	break;
				case TCR_IN_16:		MTU5.TCRU.BIT.TPSC = 0x02;	break;
				case TCR_IN_64:		MTU5.TCRU.BIT.TPSC = 0x03;	break;
			}
			switch (nTimePrescalerV) {
				case TCR_IN_1:		MTU5.TCRV.BIT.TPSC = 0x00;	break;
				case TCR_IN_4:		MTU5.TCRV.BIT.TPSC = 0x01;	break;
				case TCR_IN_16:		MTU5.TCRV.BIT.TPSC = 0x02;	break;
				case TCR_IN_64:		MTU5.TCRV.BIT.TPSC = 0x03;	break;
			}
			switch (nTimePrescalerW) {
				case TCR_IN_1:		MTU5.TCRW.BIT.TPSC = 0x00;	break;
				case TCR_IN_4:		MTU5.TCRW.BIT.TPSC = 0x01;	break;
				case TCR_IN_16:		MTU5.TCRW.BIT.TPSC = 0x02;	break;
				case TCR_IN_64:		MTU5.TCRW.BIT.TPSC = 0x03;	break;
			}
			break;
		}
	}
}

////////////////////////////////////////
// TMDRの設定
//	ch		: チャンネル
//	mode	: TMDR_*
//
void CMtuObject::SetMode(int ch, unsigned char mode)
{
	switch (ch) {
		case 0:	MTU0.TMDR.BYTE = mode;	break;
		case 1:	MTU1.TMDR.BYTE = mode;	break;
		case 2:	MTU2.TMDR.BYTE = mode;	break;
		case 3:	MTU3.TMDR.BYTE = mode;	break;
		case 4:	MTU4.TMDR.BYTE = mode;	break;
	}
}

////////////////////////////////////////
// TIORの設定
//	ch		: チャンネル
//	tgra	: TIOR_*
//	tgrb	: TIOR_*
//	tgrc	: TIOR_*
//	tgrd	: TIOR_*
//
void CMtuObject::SetIOControl(int ch, unsigned char tgra, unsigned char tgrb, unsigned char tgrc, unsigned char tgrd)
{
	switch (ch) {
		case 0:
		{
			MTU0.TIORH.BIT.IOA = tgra;
			MTU0.TIORH.BIT.IOB = tgrb;
			MTU0.TIORL.BIT.IOC = tgrc;
			MTU0.TIORL.BIT.IOD = tgrd;
			break;
		}
		case 1:
		{
			MTU1.TIOR.BIT.IOA = tgra;
			MTU1.TIOR.BIT.IOB = tgrb;
			break;
		}
		case 2:
		{
			MTU2.TIOR.BIT.IOA = tgra;
			MTU2.TIOR.BIT.IOB = tgrb;
			break;
		}
		case 3:
		{
			MTU3.TIORH.BIT.IOA = tgra;
			MTU3.TIORH.BIT.IOB = tgrb;
			MTU3.TIORL.BIT.IOC = tgrc;
			MTU3.TIORL.BIT.IOD = tgrd;
			break;
		}
		case 4:
		{
			MTU4.TIORH.BIT.IOA = tgra;
			MTU4.TIORH.BIT.IOB = tgrb;
			MTU4.TIORL.BIT.IOC = tgrc;
			MTU4.TIORL.BIT.IOD = tgrd;
			break;
		}
		case 5:
		{
			MTU5.TIORU.BIT.IOC = tgra;
			MTU5.TIORV.BIT.IOC = tgrb;
			MTU5.TIORW.BIT.IOC = tgrc;
			break;
		}
	}
}

////////////////////////////////////////
// タイマ割込み許可の設定
//	ch		: チャンネル
//	tier	: TIER_*
//	tier2	: TIER2_*
//
void CMtuObject::EnableInterrupt(int ch, unsigned char tier, unsigned char tier2)
{
	switch (ch) {
		case 0:	MTU0.TIER.BYTE = tier;	MTU0.TIER2.BYTE = tier2;	break;
		case 1:	MTU1.TIER.BYTE = tier;								break;
		case 2:	MTU2.TIER.BYTE = tier;								break;
		case 3:	MTU3.TIER.BYTE = tier;								break;
		case 4:	MTU4.TIER.BYTE = tier;								break;
		case 5:	MTU5.TIER.BYTE = tier;								break;
	}
}

////////////////////////////////////////
// タイマステータスフラグの設定
//	ch		: チャンネル
//	flag	: TSR_*
//
void CMtuObject::SetTSR(int ch, unsigned char flag)
{
	switch (ch) {
		case 0:	MTU0.TSR.BYTE = flag | 0x7F;	break;
		case 1:	MTU1.TSR.BYTE = flag | 0x7F;	break;
		case 2:	MTU2.TSR.BYTE = flag | 0x7F;	break;
		case 3:	MTU3.TSR.BYTE = flag | 0x7F;	break;
		case 4:	MTU4.TSR.BYTE = flag | 0x7F;	break;
	}
}

////////////////////////////////////////
// タイマステータスフラグの取得
//	ch		: チャンネル
//	flag	: TSR_*
//
bool CMtuObject::GetTSR(int ch, unsigned char flag)
{
	switch (ch) {
		case 0:	return ((MTU0.TSR.BYTE & flag) == flag) ? true : false;
		case 1:	return ((MTU1.TSR.BYTE & flag) == flag) ? true : false;
		case 2:	return ((MTU2.TSR.BYTE & flag) == flag) ? true : false;
		case 3:	return ((MTU3.TSR.BYTE & flag) == flag) ? true : false;
		case 4:	return ((MTU4.TSR.BYTE & flag) == flag) ? true : false;
	}
	return false;
}

////////////////////////////////////////
// タイマカウンタの設定
//	ch		: チャンネル
//	tcnt	: TCNT_*
//	tcntv	: TCNT_*
//	tcntw	: TCNT_*
//
void CMtuObject::SetTCNT(int ch, unsigned int tcnt, unsigned int tcntv, unsigned int tcntw)
{
	switch (ch) {
		case 0:	MTU0.TCNT = tcnt;	break;
		case 1:	MTU1.TCNT = tcnt;	break;
		case 2:	MTU2.TCNT = tcnt;	break;
		case 3:	MTU3.TCNT = tcnt;	break;
		case 4:	MTU4.TCNT = tcnt;	break;
		case 5:
		{
			MTU5.TCNTU = tcnt;
			MTU5.TCNTV = tcntv;
			MTU5.TCNTW = tcntw;
			break;
		}
	}
}

////////////////////////////////////////
// タイマカウンタの取得
//	ch		: チャンネル
//	tcnt	: TCNT_*
//	tcntv	: TCNT_*
//	tcntw	: TCNT_*
//
void CMtuObject::GetTCNT(int ch, unsigned int &tcnt, unsigned int &tcntv, unsigned int &tcntw)
{
	switch (ch) {
		case 0:	tcnt = MTU0.TCNT;	tcntv = 0;			tcntw = 0;			break;
		case 1:	tcnt = MTU1.TCNT;	tcntv = 0;			tcntw = 0;			break;
		case 2:	tcnt = MTU2.TCNT;	tcntv = 0;			tcntw = 0;			break;
		case 3:	tcnt = MTU3.TCNT;	tcntv = 0;			tcntw = 0;			break;
		case 4:	tcnt = MTU4.TCNT;	tcntv = 0;			tcntw = 0;			break;
		case 5:	tcnt = MTU5.TCNTU;	tcntv = MTU5.TCNTV;	tcntw = MTU5.TCNTW;	break;
	}
}

////////////////////////////////////////
// タイマジェネラルレジスタの設定
//	ch		: チャンネル
//	flag	: TGR_*
//	val		: 値
//
void CMtuObject::SetTGR(int ch, int flag, unsigned int val)
{
	switch (ch) {
		case 0:
		{
			switch (flag) {
				case TGR_TGRA:	MTU0.TGRA = val;	break;
				case TGR_TGRB:	MTU0.TGRB = val;	break;
				case TGR_TGRC:	MTU0.TGRC = val;	break;
				case TGR_TGRD:	MTU0.TGRD = val;	break;
				case TGR_TGRE:	MTU0.TGRE = val;	break;
				case TGR_TGRF:	MTU0.TGRF = val;	break;
			}
			break;
		}
		case 1:
		{
			switch (flag) {
				case TGR_TGRA:	MTU1.TGRA = val;	break;
				case TGR_TGRB:	MTU1.TGRB = val;	break;
			}
			break;
		}
		case 2:
		{
			switch (flag) {
				case TGR_TGRA:	MTU2.TGRA = val;	break;
				case TGR_TGRB:	MTU2.TGRB = val;	break;
			}
			break;
		}
		case 3:
		{
			switch (flag) {
				case TGR_TGRA:	MTU3.TGRA = val;	break;
				case TGR_TGRB:	MTU3.TGRB = val;	break;
				case TGR_TGRC:	MTU3.TGRC = val;	break;
				case TGR_TGRD:	MTU3.TGRD = val;	break;
			}
			break;
		}
		case 4:
		{
			switch (flag) {
				case TGR_TGRA:	MTU4.TGRA = val;	break;
				case TGR_TGRB:	MTU4.TGRB = val;	break;
				case TGR_TGRC:	MTU4.TGRC = val;	break;
				case TGR_TGRD:	MTU4.TGRD = val;	break;
			}
			break;
		}
		case 5:
		{
			switch (flag) {
				case TGR_TGRU:	MTU5.TGRU = val;	break;
				case TGR_TGRV:	MTU5.TGRV = val;	break;
				case TGR_TGRW:	MTU5.TGRW = val;	break;
			}
			break;
		}
	}
}

////////////////////////////////////////
// タイマジェネラルレジスタの取得
//	ch		: チャンネル
//	flag	: TGR_*
//
unsigned int CMtuObject::GetTGR(int ch, int flag)
{
	switch (ch) {
		case 0:
		{
			switch (flag) {
				case TGR_TGRA:	return MTU0.TGRA;
				case TGR_TGRB:	return MTU0.TGRB;
				case TGR_TGRC:	return MTU0.TGRC;
				case TGR_TGRD:	return MTU0.TGRD;
				case TGR_TGRE:	return MTU0.TGRE;
				case TGR_TGRF:	return MTU0.TGRF;
			}
			break;
		}
		case 1:
		{
			switch (flag) {
				case TGR_TGRA:	return MTU1.TGRA;
				case TGR_TGRB:	return MTU1.TGRB;
			}
			break;
		}
		case 2:
		{
			switch (flag) {
				case TGR_TGRA:	return MTU2.TGRA;
				case TGR_TGRB:	return MTU2.TGRB;
			}
			break;
		}
		case 3:
		{
			switch (flag) {
				case TGR_TGRA:	return MTU3.TGRA;
				case TGR_TGRB:	return MTU3.TGRB;
				case TGR_TGRC:	return MTU3.TGRC;
				case TGR_TGRD:	return MTU3.TGRD;
			}
			break;
		}
		case 4:
		{
			switch (flag) {
				case TGR_TGRA:	return MTU4.TGRA;
				case TGR_TGRB:	return MTU4.TGRB;
				case TGR_TGRC:	return MTU4.TGRC;
				case TGR_TGRD:	return MTU4.TGRD;
			}
			break;
		}
		case 5:
		{
			switch (flag) {
				case TGR_TGRU:	return MTU5.TGRU;
				case TGR_TGRV:	return MTU5.TGRV;
				case TGR_TGRW:	return MTU5.TGRW;
			}
			break;
		}
	}
	return 0;
}

////////////////////////////////////////
// タイマスタート
//	ch		: チャンネル
//	flag	: TSTR_*
//
void CMtuObject::Start(int flag)
{
	MTU.TSTR.BYTE  |= (flag >> 0) & 0x00FF;
	MTU5.TSTR.BYTE |= (flag >> 8) & 0x00FF;
}

////////////////////////////////////////
// タイマストップ
//	ch		: チャンネル
//	flag	: TSTR_*
//
void CMtuObject::Stop(int flag)
{
	MTU.TSTR.BYTE  &= (~flag >> 0) & 0x00FF;
	MTU5.TSTR.BYTE &= (~flag >> 8) & 0x00FF;
}

////////////////////////////////////////
// タイマシンクロレジスタの設定
//	ch		: チャンネル
//	bEnable	: true(許可) ／ false(禁止)
//
void CMtuObject::SyncEnable(int ch, bool bEnable)
{
	switch (ch) {
		case 0:	MTU.TSYR.BIT.SYNC0 = bEnable ? 1 : 0;	break;
		case 1:	MTU.TSYR.BIT.SYNC1 = bEnable ? 1 : 0;	break;
		case 2:	MTU.TSYR.BIT.SYNC2 = bEnable ? 1 : 0;	break;
		case 3:	MTU.TSYR.BIT.SYNC3 = bEnable ? 1 : 0;	break;
		case 4:	MTU.TSYR.BIT.SYNC4 = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// コンペアマッチタイマコントローラ
//

////////////////////////////////////////
// CMTモジュールの設定
//	nUnit	: ユニット
//	bRun	: true(開始) ／ false(停止)
//
void CCmtObject::RunModule(unsigned int nUnit, bool bRun)
{
	if (nUnit >= CCmtObject::CMT_UNIT_NUM)
		return;

	if (bRun) {
		CModuleStopObject::RunModule(nUnit == CMT_U0 ? CModuleStopObject::M_CMT0 : CModuleStopObject::M_CMT1);
	}
	else {
		CModuleStopObject::StopModule(nUnit == CMT_U0 ? CModuleStopObject::M_CMT0 : CModuleStopObject::M_CMT1);
	}
}

////////////////////////////////////////
// インターバルタイマの設定
//	ch		: チャンネル
//	nMHz	: PCLK周波数
//	ms		: ミリ秒
//
bool CCmtObject::SetIntervalTimer(int ch, int nMHz, int ms)
{
	// cycle cnt = (ms * 1,000 * MHz) / div
	//
	for (int nDiv = 8; nDiv < 512; nDiv *= 4) {
		int nCnt = (ms * 1000 * nMHz) / nDiv;
		int nMod = (ms * 1000 * nMHz) % nDiv;

		// カウンタが65535以下で、余りが無い
		//
		if (nCnt < 65535 && nMod == 0) {

			switch (nDiv) {
				case 8:		SelectClock(ch, PCLK_1_8);		break;
				case 32:	SelectClock(ch, PCLK_1_32);		break;
				case 128:	SelectClock(ch, PCLK_1_128);	break;
				case 512:	SelectClock(ch, PCLK_1_512);	break;
			}

			SetCounter(ch, 0);
			SetInterval(ch, nCnt);
			return true;
		}
	}
	return false;
}

////////////////////////////////////////
// カウント動作の開始
//	ch		: チャンネル
//	bEnable	: true(開始) ／ false(停止)
//
void CCmtObject::StartCounter(int ch, bool bEnable)
{
	switch (ch) {
		case 0:	CMT.CMSTR0.BIT.STR0 = bEnable ? 1 : 0;	break;
		case 1:	CMT.CMSTR0.BIT.STR1 = bEnable ? 1 : 0;	break;
		case 2:	CMT.CMSTR1.BIT.STR2 = bEnable ? 1 : 0;	break;
		case 3:	CMT.CMSTR1.BIT.STR3 = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// クロックの設定
//	ch		: チャンネル
//	cks		: PCLK分周比
//
void CCmtObject::SelectClock(int ch, int cks)
{
	int nCks = 0;
	switch (cks) {
		case PCLK_1_8:		nCks = 0;	break;
		case PCLK_1_32:		nCks = 1;	break;
		case PCLK_1_128:	nCks = 2;	break;
		case PCLK_1_512:	nCks = 3;	break;
	}

	switch (ch) {
		case 0:	CMT0.CMCR.BIT.CKS = nCks;	break;
		case 1:	CMT1.CMCR.BIT.CKS = nCks;	break;
		case 2:	CMT2.CMCR.BIT.CKS = nCks;	break;
		case 3:	CMT3.CMCR.BIT.CKS = nCks;	break;
	}
}

////////////////////////////////////////
// コンペアマッチ割込み許可の設定
//	ch		: チャンネル
//	bEnable	: true(許可) ／ false(禁止)
//
void CCmtObject::EnableInterrupt(int ch, bool bEnable)
{
	switch (ch) {
		case 0:	CMT0.CMCR.BIT.CMIE = bEnable ? 1 : 0;	break;
		case 1:	CMT1.CMCR.BIT.CMIE = bEnable ? 1 : 0;	break;
		case 2:	CMT2.CMCR.BIT.CMIE = bEnable ? 1 : 0;	break;
		case 3:	CMT3.CMCR.BIT.CMIE = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// カウンタの設定
//	ch		: チャンネル
//	cnt		: カウンタ値
//
void CCmtObject::SetCounter(int ch, int cnt)
{
	switch (ch) {
		case 0:	CMT0.CMCNT = cnt;	break;
		case 1:	CMT1.CMCNT = cnt;	break;
		case 2:	CMT2.CMCNT = cnt;	break;
		case 3:	CMT3.CMCNT = cnt;	break;
	}
}

////////////////////////////////////////
// カウンタの取得
//	ch		: チャンネル
//
int CCmtObject::GetCounter(int ch)
{
	switch (ch) {
		case 0:	return CMT0.CMCNT;
		case 1:	return CMT1.CMCNT;
		case 2:	return CMT2.CMCNT;
		case 3:	return CMT3.CMCNT;
	}
	return 0;
}

////////////////////////////////////////
// インターバルの設定
//	ch		: チャンネル
//	cnt		: カウンタ値
//
void CCmtObject::SetInterval(int ch, int cnt)
{
	switch (ch) {
		case 0:	CMT0.CMCOR = cnt;	break;
		case 1:	CMT1.CMCOR = cnt;	break;
		case 2:	CMT2.CMCOR = cnt;	break;
		case 3:	CMT3.CMCOR = cnt;	break;
	}
}

////////////////////////////////////////
// インターバルの取得
//	ch		: チャンネル
//
int CCmtObject::GetInterval(int ch)
{
	switch (ch) {
		case 0:	return CMT0.CMCOR;
		case 1:	return CMT1.CMCOR;
		case 2:	return CMT2.CMCOR;
		case 3:	return CMT3.CMCOR;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// 独立ウォッチドッグタイマコントローラ
//

////////////////////////////////////////
// カウンタのリフレッシュ
//
void CIwdtObject::RefreshCounter()
{
	IWDT.IWDTRR = 0x00;
	IWDT.IWDTRR = 0xFF;
}

////////////////////////////////////////
// タイムアウト期間の設定
//	cycle	: サイクル数
//
void CIwdtObject::SetTimeoutPeriod(int cycle)
{
	switch (cycle) {
		case CYCLE_128:
		case CYCLE_512:
		case CYCLE_1024:
		case CYCLE_2048:
		{
			IWDT.IWDTCR.BIT.TOPS = cycle;
			break;
		}
	}
}

////////////////////////////////////////
// クロックの設定
//	cks		: IWDTCLK分周比
//
void CIwdtObject::SelectClock(int cks)
{
	int nCks = 0;
	switch (cks) {
		case IWDTCLK:		nCks = 0;	break;
		case IWDTCLK_1_16:	nCks = 2;	break;
		case IWDTCLK_1_32:	nCks = 3;	break;
		case IWDTCLK_1_64:	nCks = 4;	break;
		case IWDTCLK_1_128:	nCks = 15;	break;
		case IWDTCLK_1_256:	nCks = 5;	break;
	}

	IWDT.IWDTCR.BIT.CKS = nCks;
}

////////////////////////////////////////
// ウィンドウ終了位置の設定
//	pos		: ポジション
//
void CIwdtObject::SetWindowEndPosition(int pos)
{
	switch (pos) {
		case END_POS_75:
		case END_POS_50:
		case END_POS_25:
		case END_POS_0:
		{
			IWDT.IWDTCR.BIT.RPES = pos;
			break;
		}
	}
}

////////////////////////////////////////
// ウィンドウ開始位置の設定
//	pos		: ポジション
//
void CIwdtObject::SetWindowStartPosition(int pos)
{
	switch (pos) {
		case START_POS_25:
		case START_POS_50:
		case START_POS_75:
		case START_POS_100:
		{
			IWDT.IWDTCR.BIT.RPSS = pos;
			break;
		}
	}
}

////////////////////////////////////////
// カウンタの取得
//	ch		: チャンネル
//
int CIwdtObject::GetCounter()
{
	return IWDT.IWDTSR.BIT.CNTVAL;
}

////////////////////////////////////////
// アンダーフローフラグのクリア
//
void CIwdtObject::ClearUnderflowFlag()
{
	IWDT.IWDTSR.BIT.UNDFF = 0;
}

////////////////////////////////////////
// アンダーフローフラグの取得
//
bool CIwdtObject::GetUnderflowFlag()
{
	return IWDT.IWDTSR.BIT.UNDFF;
}

////////////////////////////////////////
// リフレッシュエラーフラグのクリア
//
void CIwdtObject::ClearRefreshErrorFlag()
{
	IWDT.IWDTSR.BIT.REFEF = 0;
}

////////////////////////////////////////
// リフレッシュエラーフラグの取得
//
bool CIwdtObject::GetRefreshErrorFlag()
{
	return IWDT.IWDTSR.BIT.REFEF;
}

////////////////////////////////////////
// ノンマスカブル割込み許可の設定
//	bEnable	: true(許可) ／ false(禁止)
//
void CIwdtObject::EnableNonMaskableInterrupt(bool bEnable)
{
	IWDT.IWDTRCR.BIT.RSTIRQS = bEnable ? 0 : 1;
}

////////////////////////////////////////
// カウンタ停止の設定(低消費電力状態時)
//	bEnable	: true(有効) ／ false(無効)
//
void CIwdtObject::StopSleepModeCount(bool bEnable)
{
	IWDT.IWDTCSTPR.BIT.SLCSTP = bEnable ? 1 : 0;
}

////////////////////////////////////////////////////////////////////////////////
// USB2.0ホスト／ファンクションモジュールコントローラ
//

////////////////////////////////////////
// USBモジュールの設定
//	ch		: チャンネル
//	bRun	: true(開始) ／ false(停止)
//
void CUsbObject::RunModule(int ch, bool bRun)
{
	if (bRun) {
		switch (ch) {
			case 0:	CModuleStopObject::RunModule(CModuleStopObject::M_USB0);	break;
		}
	}
	else {
		switch (ch) {
			case 0:	CModuleStopObject::StopModule(CModuleStopObject::M_USB0);	break;
		}
	}
}

////////////////////////////////////////
// SYSCFG
//
void CUsbObject::EnableModule(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.SYSCFG.BIT.USBE = bEnable ? 1 : 0;		break;
	}
}

void CUsbObject::EnablePullupMinus(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.SYSCFG.BIT.DMRPU = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnablePullupPlus(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.SYSCFG.BIT.DPRPU = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnablePulldown(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.SYSCFG.BIT.DRPD = bEnable ? 1 : 0;		break;
	}
}

void CUsbObject::SelectHostController(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.SYSCFG.BIT.DCFM = bEnable ? 1 : 0;		break;
	}
}

void CUsbObject::EnableSingleEndReceiver(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.SYSCFG.BIT.CNEN = bEnable ? 1 : 0;		break;
	}
}

void CUsbObject::EnableSupplyClockToUsbModule(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.SYSCFG.BIT.SCKE = bEnable ? 1 : 0;		break;
	}
}

////////////////////////////////////////
// SYSSTS0
//
CUsbObject::LNST CUsbObject::GetDataLineStatus(int ch)
{
	int nLnst = 0;
	switch (ch) {
//		case 0:	nLnst = USB0.SYSSTS0.BIT.LNST;	break;
	}
	switch (nLnst) {
		case 0:	return SE0;
		case 1:	return J_STATE;
		case 2:	return K_STATE;
		case 3:	return SE1;
	}
	return SE0;
}

bool CUsbObject::IsUsbIdHigh(int ch)
{
	switch (ch) {
//		case 0:	return USB0.SYSSTS0.BIT.IDMON;
	}
	return false;
}

bool CUsbObject::IsStopHostSequencer(int ch)
{
	switch (ch) {
//		case 0:	return !USB0.SYSSTS0.BIT.HTACT;
	}
	return false;
}

int CUsbObject::GetOverCurrentStatus(int ch)
{
	switch (ch) {
//		case 0:	return USB0.SYSSTS0.BIT.OVCMON;
	}
	return 0;
}

////////////////////////////////////////
// DVSTCTR0
//
CUsbObject::RHST CUsbObject::GetBusResetStatus(int ch)
{
	int nRhst = 0;
	switch (ch) {
//		case 0:	nRhst = USB0.DVSTCTR0.BIT.RHST;	break;
	}
	switch (nRhst) {
		case 0:	return UNSTABLE;
		case 1:	return BUS_RESET;
		case 2:	return LOW_SPEED;
		case 3:	return FULL_SPEED;
	}
	return UNSTABLE;
}

void CUsbObject::EnableDownPort(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DVSTCTR0.BIT.UACT = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::OutputResume(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DVSTCTR0.BIT.RESUME = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::OutputBusReset(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DVSTCTR0.BIT.USBRST = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableDownportRemoteWakeup(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DVSTCTR0.BIT.RWUPE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::OutputRemoteWakeup(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DVSTCTR0.BIT.WKUP = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::SetVbusenHigh(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DVSTCTR0.BIT.VBUSEN = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::SetExicenHigh(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DVSTCTR0.BIT.EXICEN = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::SetHostNegotiationProtocolHigh(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DVSTCTR0.BIT.HNPBTOA = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// CFIFO D0FIFO D1FIFO
//
void CUsbObject::SetFifoData(int ch, FIFO fifo, unsigned short data)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
				case CFIFO:	 	USB0.CFIFO.WORD  = data;	break;
				case D0FIFO:	USB0.D0FIFO.WORD = data;	break;
				case D1FIFO:	USB0.D1FIFO.WORD = data;	break;
			}
			break;
		}
	}
}

void CUsbObject::SetFifoData(int ch, FIFO fifo, unsigned char data)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
				case CFIFO:		USB0.CFIFO.BYTE.L  = data;	break;
				case D0FIFO:	USB0.D0FIFO.BYTE.L = data;	break;
				case D1FIFO:	USB0.D1FIFO.BYTE.L = data;	break;
			}
			break;
		}
	}
}

unsigned short CUsbObject::GetFifoDataWord(int ch, FIFO fifo)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
				case CFIFO:		return USB0.CFIFO.WORD;
				case D0FIFO:	return USB0.D0FIFO.WORD;
				case D1FIFO:	return USB0.D1FIFO.WORD;
			}
			break;
		}
	}
	return 0;
}

unsigned char CUsbObject::GetFifoDataByte(int ch, FIFO fifo)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
				case CFIFO:		return USB0.CFIFO.BYTE.L;
				case D0FIFO:	return USB0.D0FIFO.BYTE.L;
				case D1FIFO:	return USB0.D1FIFO.BYTE.L;
			}
			break;
		}
	}
	return 0;
}

////////////////////////////////////////
// CFIFOSEL D0FIFOSEL D1FIFOSEL
//
void CUsbObject::SelectFifoPipe(int ch, FIFO fifo, int nPipe)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		USB0.CFIFOSEL.BIT.CURPIPE  = nPipe;	break;
//				case D0FIFO:	USB0.D0FIFOSEL.BIT.CURPIPE = nPipe;	break;
//				case D1FIFO:	USB0.D1FIFOSEL.BIT.CURPIPE = nPipe;	break;
			}
			break;
		}
	}
}

int CUsbObject::GetFifoPipe(int ch, FIFO fifo)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		return USB0.CFIFOSEL.BIT.CURPIPE;
//				case D0FIFO:	return USB0.D0FIFOSEL.BIT.CURPIPE;
//				case D1FIFO:	return USB0.D1FIFOSEL.BIT.CURPIPE;
			}
			break;
		}
	}
	return -1;
}

void CUsbObject::SelectDcpFifoDirection(int ch, FIFO fifo, bool bWrite)
{
	// Only DCP
	if (fifo == CFIFO) {
		switch (ch) {
//			case 0:	USB0.CFIFOSEL.BIT.ISEL = bWrite ? 1 : 0;	break;
		}
	}
}

bool CUsbObject::IsDcpFifoDirectionWrite(int ch, FIFO fifo)
{
	// Only DCP
	if (fifo == CFIFO) {
		switch (ch) {
//			case 0:	return USB0.CFIFOSEL.BIT.ISEL;
		}
	}
	return false;
}

void CUsbObject::SelectFifoBigEndian(int ch, FIFO fifo, bool bEnable)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		USB0.CFIFOSEL.BIT.BIGEND  = bEnable ? 1 : 0;	break;
//				case D0FIFO:	USB0.D0FIFOSEL.BIT.BIGEND = bEnable ? 1 : 0;	break;
//				case D1FIFO:	USB0.D1FIFOSEL.BIT.BIGEND = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

bool CUsbObject::IsFifoBigEndian(int ch, FIFO fifo)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		return USB0.CFIFOSEL.BIT.BIGEND;
//				case D0FIFO:	return USB0.D0FIFOSEL.BIT.BIGEND;
//				case D1FIFO:	return USB0.D1FIFOSEL.BIT.BIGEND;
			}
			break;
		}
	}
	return false;
}

void CUsbObject::SelectFifoWidth16(int ch, FIFO fifo, bool bEnable)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		USB0.CFIFOSEL.BIT.MBW  = bEnable ? 1 : 0;	break;
//				case D0FIFO:	USB0.D0FIFOSEL.BIT.MBW = bEnable ? 1 : 0;	break;
//				case D1FIFO:	USB0.D1FIFOSEL.BIT.MBW = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

bool CUsbObject::IsFifoWidth16(int ch, FIFO fifo)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		return USB0.CFIFOSEL.BIT.MBW;
//				case D0FIFO:	return USB0.D0FIFOSEL.BIT.MBW;
//				case D1FIFO:	return USB0.D1FIFOSEL.BIT.MBW;
			}
			break;
		}
	}
	return false;
}

void CUsbObject::EnableRequestFifoDtc(int ch, FIFO fifo, bool bEnable)
{
	// Only D0 and D1
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case D0FIFO:	USB0.D0FIFOSEL.BIT.DREQE = bEnable ? 1 : 0;	break;
//				case D1FIFO:	USB0.D1FIFOSEL.BIT.DREQE = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

void CUsbObject::EnableFifoBufferAutoClear(int ch, FIFO fifo, bool bEnable)
{
	// Only D0 and D1
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case D0FIFO:	USB0.D0FIFOSEL.BIT.DCLRM = bEnable ? 1 : 0;	break;
//				case D1FIFO:	USB0.D1FIFOSEL.BIT.DCLRM = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

void CUsbObject::SelectFifoBufferPointerRewind(int ch, FIFO fifo, bool bEnable)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		USB0.CFIFOSEL.BIT.REW  = bEnable ? 1 : 0;	break;
//				case D0FIFO:	USB0.D0FIFOSEL.BIT.REW = bEnable ? 1 : 0;	break;
//				case D1FIFO:	USB0.D1FIFOSEL.BIT.REW = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

void CUsbObject::EnableFifoReadDataCountDownMode(int ch, FIFO fifo, bool bEnable)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		USB0.CFIFOSEL.BIT.RCNT  = bEnable ? 1 : 0;	break;
//				case D0FIFO:	USB0.D0FIFOSEL.BIT.RCNT = bEnable ? 1 : 0;	break;
//				case D1FIFO:	USB0.D1FIFOSEL.BIT.RCNT = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

////////////////////////////////////////
// CFIFOCTR D0FIFOCTR D1FIFOCTR
//
int CUsbObject::GetFifoReceiveDataLength(int ch, FIFO fifo)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		return USB0.CFIFOCTR.BIT.DTLN;
//				case D0FIFO:	return USB0.D0FIFOCTR.BIT.DTLN;
//				case D1FIFO:	return USB0.D1FIFOCTR.BIT.DTLN;
			}
			break;
		}
	}
	return 0;
}

bool CUsbObject::IsFifoPortReady(int ch, FIFO fifo)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		return USB0.CFIFOCTR.BIT.FRDY;
//				case D0FIFO:	return USB0.D0FIFOCTR.BIT.FRDY;
//				case D1FIFO:	return USB0.D1FIFOCTR.BIT.FRDY;
			}
			break;
		}
	}
	return false;
}

void CUsbObject::ClearFifoCpuBusBuffer(int ch, FIFO fifo)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		USB0.CFIFOCTR.BIT.BCLR  = 1;	break;
//				case D0FIFO:	USB0.D0FIFOCTR.BIT.BCLR = 1;	break;
//				case D1FIFO:	USB0.D1FIFOCTR.BIT.BCLR = 1;	break;
			}
			break;
		}
	}
}

void CUsbObject::ValidateFifoBufferMemory(int ch, FIFO fifo, bool bEnable)
{
	switch (ch) {
		case 0:
		{
			switch (fifo) {
//				case CFIFO:		USB0.CFIFOCTR.BIT.BVAL  = bEnable ? 1 : 0;	break;
//				case D0FIFO:	USB0.D0FIFOCTR.BIT.BVAL = bEnable ? 1 : 0;	break;
//				case D1FIFO:	USB0.D1FIFOCTR.BIT.BVAL = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

////////////////////////////////////////
// INTENB0
//
void CUsbObject::EnableBufferReadyInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB0.BIT.BRDYE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableBufferNotReadyInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB0.BIT.NRDYE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableBufferEmptyInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB0.BIT.BEMPE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableControlTransferStageInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB0.BIT.CTRE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableDeviceStateInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB0.BIT.DVSE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableFrameNumberUpdateInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB0.BIT.SOFE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableResumeInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB0.BIT.RSME = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableVbusInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB0.BIT.VBSE = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// INTENB1
//
void CUsbObject::EnableDetectPDDETINT0Interrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB1.BIT.PDDETINTE0 = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableSetupTransactionNormalResponseInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB1.BIT.SACKE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableSetupTransactionErrorInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB1.BIT.SIGNE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableEOFErrorInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB1.BIT.EOFERRE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableDetectAttachInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB1.BIT.ATTCHE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableDetectDetachInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB1.BIT.DTCHE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableChangeBusStateInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB1.BIT.BCHGE = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::EnableOverCurrentInterrupt(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.INTENB1.BIT.OVRCRE = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// BRDYENB
//
void CUsbObject::EnablePipeBufferReadyInterrupt(int ch, int nPipe, bool bEnable)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	USB0.BRDYENB.BIT.PIPE0BRDYE = bEnable ? 1 : 0;	break;
//				case 1:	USB0.BRDYENB.BIT.PIPE1BRDYE = bEnable ? 1 : 0;	break;
//				case 2:	USB0.BRDYENB.BIT.PIPE2BRDYE = bEnable ? 1 : 0;	break;
//				case 3:	USB0.BRDYENB.BIT.PIPE3BRDYE = bEnable ? 1 : 0;	break;
//				case 4:	USB0.BRDYENB.BIT.PIPE4BRDYE = bEnable ? 1 : 0;	break;
//				case 5:	USB0.BRDYENB.BIT.PIPE5BRDYE = bEnable ? 1 : 0;	break;
//				case 6:	USB0.BRDYENB.BIT.PIPE6BRDYE = bEnable ? 1 : 0;	break;
//				case 7:	USB0.BRDYENB.BIT.PIPE7BRDYE = bEnable ? 1 : 0;	break;
//				case 8:	USB0.BRDYENB.BIT.PIPE8BRDYE = bEnable ? 1 : 0;	break;
//				case 9:	USB0.BRDYENB.BIT.PIPE9BRDYE = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

////////////////////////////////////////
// NRDYENB
//
void CUsbObject::EnablePipeBufferNotReadyInterrupt(int ch, int nPipe, bool bEnable)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	USB0.NRDYENB.BIT.PIPE0NRDYE = bEnable ? 1 : 0;	break;
//				case 1:	USB0.NRDYENB.BIT.PIPE1NRDYE = bEnable ? 1 : 0;	break;
//				case 2:	USB0.NRDYENB.BIT.PIPE2NRDYE = bEnable ? 1 : 0;	break;
//				case 3:	USB0.NRDYENB.BIT.PIPE3NRDYE = bEnable ? 1 : 0;	break;
//				case 4:	USB0.NRDYENB.BIT.PIPE4NRDYE = bEnable ? 1 : 0;	break;
//				case 5:	USB0.NRDYENB.BIT.PIPE5NRDYE = bEnable ? 1 : 0;	break;
//				case 6:	USB0.NRDYENB.BIT.PIPE6NRDYE = bEnable ? 1 : 0;	break;
//				case 7:	USB0.NRDYENB.BIT.PIPE7NRDYE = bEnable ? 1 : 0;	break;
//				case 8:	USB0.NRDYENB.BIT.PIPE8NRDYE = bEnable ? 1 : 0;	break;
//				case 9:	USB0.NRDYENB.BIT.PIPE9NRDYE = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

////////////////////////////////////////
// BEMPENB
//
void CUsbObject::EnablePipeBufferEmptyInterrupt(int ch, int nPipe, bool bEnable)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	USB0.BEMPENB.BIT.PIPE0BEMPE = bEnable ? 1 : 0;	break;
//				case 1:	USB0.BEMPENB.BIT.PIPE1BEMPE = bEnable ? 1 : 0;	break;
//				case 2:	USB0.BEMPENB.BIT.PIPE2BEMPE = bEnable ? 1 : 0;	break;
//				case 3:	USB0.BEMPENB.BIT.PIPE3BEMPE = bEnable ? 1 : 0;	break;
//				case 4:	USB0.BEMPENB.BIT.PIPE4BEMPE = bEnable ? 1 : 0;	break;
//				case 5:	USB0.BEMPENB.BIT.PIPE5BEMPE = bEnable ? 1 : 0;	break;
//				case 6:	USB0.BEMPENB.BIT.PIPE6BEMPE = bEnable ? 1 : 0;	break;
//				case 7:	USB0.BEMPENB.BIT.PIPE7BEMPE = bEnable ? 1 : 0;	break;
//				case 8:	USB0.BEMPENB.BIT.PIPE8BEMPE = bEnable ? 1 : 0;	break;
//				case 9:	USB0.BEMPENB.BIT.PIPE9BEMPE = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

////////////////////////////////////////
// SOFCFG
//
bool CUsbObject::IsEdgeInterruptProcessing(int ch)
{
	switch (ch) {
//		case 0:	return USB0.SOFCFG.BIT.EDGESTS;
	}
	return false;
}

void CUsbObject::SetClearBufferReadyInterruptStatusByModule(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.SOFCFG.BIT.BRDYM = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::SetTransactionEnabledTimeLowSpeed(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.SOFCFG.BIT.TRNENSEL = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// INTSTS0
//
CUsbObject::CTSQ CUsbObject::GetControlTransferStage(int ch)
{
	int nCtsq = 0;
	switch (ch) {
//		case 0: nCtsq = USB0.INTSTS0.BIT.CTSQ;	break;
	}
	switch (nCtsq) {
		case 0:	return IDLE_SETUP;
		case 1:	return READ_DATA;
		case 2:	return READ_STATUS;
		case 3:	return WRITE_DATA;
		case 4:	return WRITE_STATUS;
		case 5:	return WRITE_NO_DATA;
		case 6:	return SEQ_ERROR;
	}
	return IDLE_SETUP;
}

bool CUsbObject::IsReceiveUsbSetupPacket(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS0.BIT.VALID;
	}
	return false;
}

CUsbObject::DVSQ CUsbObject::GetDeviceState(int ch)
{
	int nDvsq = 0;
	switch (ch) {
//		case 0:	nDvsq = USB0.INTSTS0.BIT.DVSQ;	break;
	}
	switch (nDvsq) {
		case 0:		return POWERED;
		case 1:		return DEFAULT;
		case 2:		return ADDRESS;
		case 3:		return CONFIGURED;
		default:	return SUSPENDED;
	}
	return POWERED;
}

bool CUsbObject::IsVbusStatusHigh(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS0.BIT.VBSTS;
	}
	return false;
}

bool CUsbObject::IsDetectBufferReadyInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS0.BIT.BRDY;
	}
	return false;
}

bool CUsbObject::IsDetectBufferNotReadyInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS0.BIT.NRDY;
	}
	return false;
}

bool CUsbObject::IsDetectBufferEmptyInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS0.BIT.BEMP;
	}
	return false;
}

bool CUsbObject::IsDetectControlTransferStageInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS0.BIT.CTRT;
	}
	return false;
}

bool CUsbObject::IsDetectDeviceStateInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS0.BIT.DVST;
	}
	return false;
}

bool CUsbObject::IsDetectFrameNumberUpdateInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS0.BIT.SOFR;
	}
	return false;
}

bool CUsbObject::IsDetectResumeInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS0.BIT.RESM;
	}
	return false;
}

bool CUsbObject::IsDetectVbusInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS0.BIT.VBINT;
	}
	return false;
}

void CUsbObject::ClearValid(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
		case 0:	USB0.INTSTS0.BIT.VALID = 0;	break;
	}
#else
	switch (ch) {
		case 0:	USB0.INTSTS0.WORD = 0xFFF7;	break;
	}
#endif
}

void CUsbObject::ClearControlTransferStageInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS0.BIT.CTRT = 0;	break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS0.WORD = 0xF7FF;	break;
	}
#endif
}

void CUsbObject::ClearDeviceStateInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS0.BIT.DVST = 0;	break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS0.WORD = 0xEFFF;	break;
	}
#endif
}

void CUsbObject::ClearFrameNumberUpdateInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS0.BIT.SOFR = 0;	break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS0.WORD = 0xDFFF;	break;
	}
#endif
}

void CUsbObject::ClearResumeInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS0.BIT.RESM = 0;	break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS0.WORD = 0xBFFF;	break;
	}
#endif
}

void CUsbObject::ClearVbusInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS0.BIT.VBINT = 0;	break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS0.WORD = 0x7FFF;	break;
	}
#endif
}

////////////////////////////////////////
// INTSTS1
//
bool CUsbObject::IsDetectPDDETINT0Interrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS1.BIT.PDDETINT0;
	}
	return false;
}

bool CUsbObject::IsDetectSetupTransactionNormalResponseInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS1.BIT.SACK;
	}
	return false;
}

bool CUsbObject::IsDetectSetupTransactionErrorInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS1.BIT.SIGN;
	}
	return false;
}

bool CUsbObject::IsDetectEOFErrorInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS1.BIT.EOFERR;
	}
	return false;
}

bool CUsbObject::IsDetectAttachInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS1.BIT.ATTCH;
	}
	return false;
}

bool CUsbObject::IsDetectDetachInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS1.BIT.DTCH;
	}
	return false;
}

bool CUsbObject::IsDetectChangeBusStateInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS1.BIT.BCHG;
	}
	return false;
}

bool CUsbObject::IsDetectOverCurrentInterrupt(int ch)
{
	switch (ch) {
//		case 0:	return USB0.INTSTS1.BIT.OVRCR;
	}
	return false;
}

void CUsbObject::ClearPDDETINT0Interrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS1.BIT.PDDETINT0 = 0;	break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS1.WORD = 0xFFFE;		break;
	}
#endif
}

void CUsbObject::ClearSetupTransactionNormalResponseInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS1.BIT.SACK = 0;		break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS1.WORD = 0xFFEF;		break;
	}
#endif
}

void CUsbObject::ClearSetupTransactionErrorInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS1.BIT.SIGN = 0;		break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS1.WORD = 0xFFDF;		break;
	}
#endif
}

void CUsbObject::ClearEOFErrorInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS1.BIT.EOFERR = 0;	break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS1.WORD = 0xFFBF;		break;
	}
#endif
}

void CUsbObject::ClearDetectAttachInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS1.BIT.ATTCH = 0;		break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS1.WORD = 0xF7FF;		break;
	}
#endif
}

void CUsbObject::ClearDetectDetachInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS1.BIT.DTCH = 0;		break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS1.WORD = 0xEFFF;		break;
	}
#endif
}

void CUsbObject::ClearChangeBusStateInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS1.BIT.BCHG = 0;		break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS1.WORD = 0xBFFF;		break;
	}
#endif
}

void CUsbObject::ClearOverCurrentInterrupt(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.INTSTS1.BIT.OVRCR = 0;		break;
	}
#else
	switch (ch) {
//		case 0:	USB0.INTSTS1.WORD= 0x7FFF;		break;
	}
#endif
}

////////////////////////////////////////
// BRDYSTS
//
bool CUsbObject::IsDetectPipeBufferReadyInterrupt(int ch, int nPipe)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	return USB0.BRDYSTS.BIT.PIPE0BRDY;
//				case 1:	return USB0.BRDYSTS.BIT.PIPE1BRDY;
//				case 2:	return USB0.BRDYSTS.BIT.PIPE2BRDY;
//				case 3:	return USB0.BRDYSTS.BIT.PIPE3BRDY;
//				case 4:	return USB0.BRDYSTS.BIT.PIPE4BRDY;
//				case 5:	return USB0.BRDYSTS.BIT.PIPE5BRDY;
//				case 6:	return USB0.BRDYSTS.BIT.PIPE6BRDY;
//				case 7:	return USB0.BRDYSTS.BIT.PIPE7BRDY;
//				case 8:	return USB0.BRDYSTS.BIT.PIPE8BRDY;
//				case 9:	return USB0.BRDYSTS.BIT.PIPE9BRDY;
			}
			break;
		}
	}
	return false;
}

void CUsbObject::ClearPipeBufferReadyInterrupt(int ch, int nPipe)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

#ifdef BIT_EXCLUSIVE
	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	USB0.BRDYSTS.BIT.PIPE0BRDY = 0;	break;
//				case 1:	USB0.BRDYSTS.BIT.PIPE1BRDY = 0;	break;
//				case 2:	USB0.BRDYSTS.BIT.PIPE2BRDY = 0;	break;
//				case 3:	USB0.BRDYSTS.BIT.PIPE3BRDY = 0;	break;
//				case 4:	USB0.BRDYSTS.BIT.PIPE4BRDY = 0;	break;
//				case 5:	USB0.BRDYSTS.BIT.PIPE5BRDY = 0;	break;
//				case 6:	USB0.BRDYSTS.BIT.PIPE6BRDY = 0;	break;
//				case 7:	USB0.BRDYSTS.BIT.PIPE7BRDY = 0;	break;
//				case 8:	USB0.BRDYSTS.BIT.PIPE8BRDY = 0;	break;
//				case 9:	USB0.BRDYSTS.BIT.PIPE9BRDY = 0;	break;
			}
			break;
		}
	}
#else
	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	USB0.BRDYSTS.WORD = 0xFFFE;		break;
//				case 1:	USB0.BRDYSTS.WORD = 0xFFFD;		break;
//				case 2:	USB0.BRDYSTS.WORD = 0xFFFB;		break;
//				case 3:	USB0.BRDYSTS.WORD = 0xFFF7;		break;
//				case 4:	USB0.BRDYSTS.WORD = 0xFFEF;		break;
//				case 5:	USB0.BRDYSTS.WORD = 0xFFDF;		break;
//				case 6:	USB0.BRDYSTS.WORD = 0xFFBF;		break;
//				case 7:	USB0.BRDYSTS.WORD = 0xFF7F;		break;
//				case 8:	USB0.BRDYSTS.WORD = 0xFEFF;		break;
//				case 9:	USB0.BRDYSTS.WORD = 0xFDFF;		break;
			}
			break;
		}
	}
#endif
}

////////////////////////////////////////
// NRDYSTS
//
bool CUsbObject::IsDetectPipeBufferNotReadyInterrupt(int ch, int nPipe)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	return USB0.NRDYSTS.BIT.PIPE0NRDY;
//				case 1:	return USB0.NRDYSTS.BIT.PIPE1NRDY;
//				case 2:	return USB0.NRDYSTS.BIT.PIPE2NRDY;
//				case 3:	return USB0.NRDYSTS.BIT.PIPE3NRDY;
//				case 4:	return USB0.NRDYSTS.BIT.PIPE4NRDY;
//				case 5:	return USB0.NRDYSTS.BIT.PIPE5NRDY;
//				case 6:	return USB0.NRDYSTS.BIT.PIPE6NRDY;
//				case 7:	return USB0.NRDYSTS.BIT.PIPE7NRDY;
//				case 8:	return USB0.NRDYSTS.BIT.PIPE8NRDY;
//				case 9:	return USB0.NRDYSTS.BIT.PIPE9NRDY;
			}
			break;
		}
	}
	return false;
}

void CUsbObject::ClearPipeBufferNotReadyInterrupt(int ch, int nPipe)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

#ifdef BIT_EXCLUSIVE
	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	USB0.NRDYSTS.BIT.PIPE0NRDY = 0;	break;
//				case 1:	USB0.NRDYSTS.BIT.PIPE1NRDY = 0;	break;
//				case 2:	USB0.NRDYSTS.BIT.PIPE2NRDY = 0;	break;
//				case 3:	USB0.NRDYSTS.BIT.PIPE3NRDY = 0;	break;
//				case 4:	USB0.NRDYSTS.BIT.PIPE4NRDY = 0;	break;
//				case 5:	USB0.NRDYSTS.BIT.PIPE5NRDY = 0;	break;
//				case 6:	USB0.NRDYSTS.BIT.PIPE6NRDY = 0;	break;
//				case 7:	USB0.NRDYSTS.BIT.PIPE7NRDY = 0;	break;
//				case 8:	USB0.NRDYSTS.BIT.PIPE8NRDY = 0;	break;
//				case 9:	USB0.NRDYSTS.BIT.PIPE9NRDY = 0;	break;
			}
			break;
		}
	}
#else
	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	USB0.NRDYSTS.WORD = 0xFFFE;		break;
//				case 1:	USB0.NRDYSTS.WORD = 0xFFFD;		break;
//				case 2:	USB0.NRDYSTS.WORD = 0xFFFB;		break;
//				case 3:	USB0.NRDYSTS.WORD = 0xFFF7;		break;
//				case 4:	USB0.NRDYSTS.WORD = 0xFFEF;		break;
//				case 5:	USB0.NRDYSTS.WORD = 0xFFDF;		break;
//				case 6:	USB0.NRDYSTS.WORD = 0xFFBF;		break;
//				case 7:	USB0.NRDYSTS.WORD = 0xFF7F;		break;
//				case 8:	USB0.NRDYSTS.WORD = 0xFEFF;		break;
//				case 9:	USB0.NRDYSTS.WORD = 0xFDFF;		break;
			}
			break;
		}
	}
#endif
}

////////////////////////////////////////
// BEMPSTS
//
bool CUsbObject::IsDetectPipeBufferEmptyInterrupt(int ch, int nPipe)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	return USB0.BEMPSTS.BIT.PIPE0BEMP;
//				case 1:	return USB0.BEMPSTS.BIT.PIPE1BEMP;
//				case 2:	return USB0.BEMPSTS.BIT.PIPE2BEMP;
//				case 3:	return USB0.BEMPSTS.BIT.PIPE3BEMP;
//				case 4:	return USB0.BEMPSTS.BIT.PIPE4BEMP;
//				case 5:	return USB0.BEMPSTS.BIT.PIPE5BEMP;
//				case 6:	return USB0.BEMPSTS.BIT.PIPE6BEMP;
//				case 7:	return USB0.BEMPSTS.BIT.PIPE7BEMP;
//				case 8:	return USB0.BEMPSTS.BIT.PIPE8BEMP;
//				case 9:	return USB0.BEMPSTS.BIT.PIPE9BEMP;
			}
			break;
		}
	}
	return false;
}

void CUsbObject::ClearPipeBufferEmptyInterrupt(int ch, int nPipe)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

#ifdef BIT_EXCLUSIVE
	switch (ch) {
		case 0:
		{
			switch (nPipe) {
				case 0:	USB0.BEMPSTS.BIT.PIPE0BEMP = 0;	break;
//				case 1:	USB0.BEMPSTS.BIT.PIPE1BEMP = 0;	break;
//				case 2:	USB0.BEMPSTS.BIT.PIPE2BEMP = 0;	break;
//				case 3:	USB0.BEMPSTS.BIT.PIPE3BEMP = 0;	break;
//				case 4:	USB0.BEMPSTS.BIT.PIPE4BEMP = 0;	break;
//				case 5:	USB0.BEMPSTS.BIT.PIPE5BEMP = 0;	break;
//				case 6:	USB0.BEMPSTS.BIT.PIPE6BEMP = 0;	break;
//				case 7:	USB0.BEMPSTS.BIT.PIPE7BEMP = 0;	break;
//				case 8:	USB0.BEMPSTS.BIT.PIPE8BEMP = 0;	break;
//				case 9:	USB0.BEMPSTS.BIT.PIPE9BEMP = 0;	break;
			}
			break;
		}
	}
#else
	switch (ch) {
		case 0:
		{
			switch (nPipe) {
				case 0:	USB0.BEMPSTS.WORD = 0xFFFE;		break;
//				case 1:	USB0.BEMPSTS.WORD = 0xFFFD;		break;
//				case 2:	USB0.BEMPSTS.WORD = 0xFFFB;		break;
//				case 3:	USB0.BEMPSTS.WORD = 0xFFF7;		break;
//				case 4:	USB0.BEMPSTS.WORD = 0xFFEF;		break;
//				case 5:	USB0.BEMPSTS.WORD = 0xFFDF;		break;
//				case 6:	USB0.BEMPSTS.WORD = 0xFFBF;		break;
//				case 7:	USB0.BEMPSTS.WORD = 0xFF7F;		break;
//				case 8:	USB0.BEMPSTS.WORD = 0xFEFF;		break;
//				case 9:	USB0.BEMPSTS.WORD = 0xFDFF;		break;
			}
			break;
		}
	}
#endif
}

////////////////////////////////////////
// FRMNUM
//
int CUsbObject::GetFrameNumber(int ch)
{
	switch (ch) {
//		case 0:	return USB0.FRMNUM.BIT.FRNM;
	}
	return 0;
}

bool CUsbObject::IsDetectReceiveError(int ch)
{
	switch (ch) {
//		case 0:	return USB0.FRMNUM.BIT.CRCE;
	}
	return false;
}

bool CUsbObject::IsDetectOverrunUnderrunError(int ch)
{
	switch (ch) {
//		case 0:	return USB0.FRMNUM.BIT.OVRN;
	}
	return false;
}

void CUsbObject::ClearReceiveError(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.FRMNUM.BIT.CRCE = 0;	break;
	}
#else
	switch (ch) {
//		case 0:	USB0.FRMNUM.WORD = 0xBFFF;	break;
	}
#endif
}

void CUsbObject::ClearOverrunUnderrunError(int ch)
{
#ifdef BIT_EXCLUSIVE
	switch (ch) {
//		case 0:	USB0.FRMNUM.BIT.OVRN = 0;	break;
	}
#else
	switch (ch) {
//		case 0:	USB0.FRMNUM.WORD = 0x7FFF;	break;
	}
#endif
}

////////////////////////////////////////
// USBREQ
//
int CUsbObject::GetRequestType(int ch)
{
	switch (ch) {
//		case 0:	return USB0.USBREQ.BIT.BMREQUESTTYPE;
	}
	return 0;
}

void CUsbObject::SetRequestType(int ch, int type)
{
	switch (ch) {
//		case 0:	USB0.USBREQ.BIT.BMREQUESTTYPE = type;	break;
	}
}

int CUsbObject::GetRequestData(int ch)
{
	switch (ch) {
//		case 0:	return USB0.USBREQ.BIT.BREQUEST;
	}
	return 0;
}

void CUsbObject::SetRequestData(int ch, int data)
{
	switch (ch) {
//		case 0:	USB0.USBREQ.BIT.BREQUEST = data;	break;
	}
}

////////////////////////////////////////
// USBVAL
//
int CUsbObject::GetRequestValue(int ch)
{
	switch (ch) {
		case 0:	return USB0.USBVAL;
	}
	return 0;
}

void CUsbObject::SetRequestValue(int ch, int value)
{
	switch (ch) {
		case 0:	USB0.USBVAL = value;	break;
	}
}

////////////////////////////////////////
// USBINDX
//
int CUsbObject::GetRequestIndex(int ch)
{
	switch (ch) {
		case 0:	return USB0.USBINDX;
	}
	return 0;
}

void CUsbObject::SetRequestIndex(int ch, int index)
{
	switch (ch) {
		case 0:	USB0.USBINDX = index;	break;
	}
}

////////////////////////////////////////
// USBLENG
//
int CUsbObject::GetRequestLength(int ch)
{
	switch (ch) {
		case 0:	return USB0.USBLENG;
	}
	return 0;
}

void CUsbObject::SetRequestLength(int ch, int length)
{
	switch (ch) {
		case 0:	USB0.USBLENG = length;	break;
	}
}

////////////////////////////////////////
// DCPCFG
//
void CUsbObject::SetHostDirection(int ch, bool bTransfer)
{
	switch (ch) {
//		case 0:	USB0.DCPCFG.BIT.DIR = bTransfer ? 1 : 0;	break;
	}
}

void CUsbObject::SetNakWhenTransferFinish(int ch, bool bNak)
{
	switch (ch) {
//		case 0:	USB0.DCPCFG.BIT.SHTNAK = bNak ? 1 : 0;		break;
	}
}

////////////////////////////////////////
// DCPMAXP
//
void CUsbObject::SetDcpMaxPacketSize(int ch, int size)
{
	switch (ch) {
//		case 0:	USB0.DCPMAXP.BIT.MXPS = size;	break;
	}
}

int CUsbObject::GetDcpMaxPacketSize(int ch)
{
	switch (ch) {
//		case 0:	return USB0.DCPMAXP.BIT.MXPS;
	}
	return 0;
}

void CUsbObject::SetDcpUsbAddress(int ch, int addr)
{
	if (addr < 0)
		addr = 0;
	if (addr > 5)
		addr = 5;

	switch (ch) {
//		case 0:	USB0.DCPMAXP.BIT.DEVSEL = addr;	break;
	}
}

////////////////////////////////////////
// DCPCTR
//
void CUsbObject::SetDcpResponsePid(int ch, PID pid)
{
	int nPid = 0;
	switch (pid) {
		case NAK:		nPid = 0;	break;
		case BUF:		nPid = 1;	break;
		case STALL2:	nPid = 2;	break;
		case STALL3:	nPid = 3;	break;
	}
	switch (ch) {
//		case 0: USB0.DCPCTR.BIT.PID = nPid;	break;
	}
}

CUsbObject::PID CUsbObject::GetDcpResponsePid(int ch)
{
	int nPid = 0;
	switch (ch) {
//		case 0: nPid = USB0.DCPCTR.BIT.PID;	break;
	}
	switch (nPid) {
		case 0:	return NAK;
		case 1:	return BUF;
		case 2:	return STALL2;
		case 3:	return STALL3;
	}
	return NAK;
}

void CUsbObject::ChangeDcpResponsePid(int ch, PID2 pid)
{
	//	NAK(0)   -> BUF(1)		NAK(0)   -> BUF(1)
	//	NAK(0)   -> STALL		NAK(0)   -> STALL(2)
	//	BUF(1)   -> NAK(0)		BUF(1)   -> NAK(0)
	//	BUF(1)   -> STALL		BUF(1)   -> STALL(3)
	//	STALL(2) -> NAK(0)		STALL(2) -> NAK(0)
	//	STALL(2) -> BUF(1)		STALL(2) -> BUF(1)
	//	STALL(3) -> NAK(0)		STALL(3) -> STALL(2) -> NAK(0)
	//	STALL(3) -> BUF(1)		STALL(3) -> STALL(2) -> NAK(0) -> BUF(1)

	PID nCurrentPid = GetDcpResponsePid(ch);

	switch (nCurrentPid) {
		case NAK:
		{
			switch (pid) {
				case PID_BUF:
					SetDcpResponsePid(ch, BUF);
					break;

				case PID_STALL:
					SetDcpResponsePid(ch, STALL2);
					break;
			}
			break;
		}
		case BUF:
		{
			switch (pid) {
				case PID_NAK:
					SetDcpResponsePid(ch, NAK);
					while (!IsDcpPipeBusy(ch))
						;
					break;

				case PID_STALL:
					SetDcpResponsePid(ch, STALL3);
					break;
			}
			break;
		}
		case STALL2:
		{
			switch (pid) {
				case PID_NAK:
					SetDcpResponsePid(ch, NAK);
					while (!IsDcpPipeBusy(ch))
						;
					break;

				case PID_BUF:
					SetDcpResponsePid(ch, BUF);
					break;
			}
			break;
		}
		case STALL3:
		{
			switch (pid) {
				case PID_NAK:
					SetDcpResponsePid(ch, STALL2);
					SetDcpResponsePid(ch, NAK);
					break;

				case PID_BUF:
					SetDcpResponsePid(ch, STALL2);
					SetDcpResponsePid(ch, NAK);
					SetDcpResponsePid(ch, BUF);
					break;
			}
			break;
		}
	}
}

void CUsbObject::EnableDcpControlTransferEnd(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DCPCTR.BIT.CCPL = bEnable ? 1 : 0;	break;
	}
}

bool CUsbObject::IsDcpPipeBusy(int ch)
{
	switch (ch) {
//		case 0:	return USB0.DCPCTR.BIT.PBUSY;
	}
	return false;
}

CUsbObject::SEQDATA CUsbObject::GetDcpSequenceToggleBit(int ch)
{
	switch (ch) {
//		case 0:	return (USB0.DCPCTR.BIT.SQMON == 0) ? DATA0 : DATA1;
	}
	return DATA0;
}

void CUsbObject::SetDcpSequenceToggleBit(int ch, SEQDATA seqdata)
{
	switch (ch) {
		case 0:
		{
			if (seqdata == DATA0) {
//				USB0.DCPCTR.BIT.SQSET = 0;	// DATA1
//				USB0.DCPCTR.BIT.SQCLR = 1;	// DATA0
			}
			else {
//				USB0.DCPCTR.BIT.SQCLR = 0;	// DATA0
//				USB0.DCPCTR.BIT.SQSET = 1;	// DATA1
			}
			break;
		}
	}
}

void CUsbObject::ClearDcpSetupToken(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DCPCTR.BIT.SUREQCLR = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::SetDcpSetupToken(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.DCPCTR.BIT.SUREQ = bEnable ? 1 : 0;	break;
	}
}

bool CUsbObject::IsEnableDcpBufferAccess(int ch)
{
	switch (ch) {
//		case 0:	return USB0.DCPCTR.BIT.BSTS;
	}
	return false;
}

////////////////////////////////////////
// PIPESEL
//
void CUsbObject::_SelectPipe(int ch, int nPipe)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
//		case 0:	USB0.PIPESEL.BIT.PIPESEL = nPipe;	break;
	}
}

int CUsbObject::_GetSelectedPipe(int ch)
{
	switch (ch) {
//		case 0:	return USB0.PIPESEL.BIT.PIPESEL;
	}
	return 0;
}

////////////////////////////////////////
// PIPECFG
//
void CUsbObject::SetPipeEndPointNumber(int ch, int num)
{
	switch (ch) {
//		case 0:	USB0.PIPECFG.BIT.EPNUM = num;	break;
	}
}

void CUsbObject::SetPipeDirectionTransfer(int ch, bool bTransfer)
{
	switch (ch) {
//		case 0:	USB0.PIPECFG.BIT.DIR = bTransfer ? 1 : 0;	break;
	}
}

void CUsbObject::EnablePipeWhenTransferEnd(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.PIPECFG.BIT.SHTNAK = bEnable ? 0 : 1;	break;
	}
}

void CUsbObject::EnablePipeDoubleBuffer(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.PIPECFG.BIT.DBLB = bEnable ? 1 : 0;	break;
	}
}

bool CUsbObject::IsPipeDoubleBuffer(int ch)
{
	switch (ch) {
//		case 0:	return USB0.PIPECFG.BIT.DBLB;
	}
	return false;
}

void CUsbObject::SetPipeBufferReadyInterruptAction(int ch, BFRE bfre)
{
	switch (ch) {
//		case 0:	USB0.PIPECFG.BIT.BFRE = (bfre == READ_END) ? 1 : 0;	break;
	}
}

void CUsbObject::SetPipeTransferType(int ch, TYPE type)
{
	int nType = 0;
	switch (type) {
		case NOT_USE:		nType = 0;	break;
		case BULK:			nType = 1;	break;
		case INTERRUPT:		nType = 2;	break;
		case ISOCHRONOUS:	nType = 3;	break;
	}

	switch (ch) {
//		case 0:	USB0.PIPECFG.BIT.TYPE = nType;	break;
	}
}

////////////////////////////////////////
// PIPEMAXP
//
void CUsbObject::SetPipeMaxPacketSize(int ch, int size)
{
	switch (ch) {
//		case 0:	USB0.PIPEMAXP.BIT.MXPS = size;	break;
	}
}

int CUsbObject::GetPipeMaxPacketSize(int ch)
{
	switch (ch) {
//		case 0:	return USB0.PIPEMAXP.BIT.MXPS;
	}
	return 0;
}

void CUsbObject::SetPipeHostUsbAddress(int ch, int addr)
{
	if (addr < 0)
		addr = 0;
	if (addr > 5)
		addr = 5;

	switch (ch) {
//		case 0:	USB0.PIPEMAXP.BIT.DEVSEL = addr;	break;
	}
}

////////////////////////////////////////
// PIPEPERI
//
void CUsbObject::SetPipeIntervalErrorDetectInterval(int ch, int value)
{
	switch (ch) {
//		case 0:	USB0.PIPEPERI.BIT.IITV = value;	break;
	}
}

void CUsbObject::FlushPipeIsochronousInBuffer(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.PIPEPERI.BIT.IFIS = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// PIPEnCTR	n = 1 ～ 9
//
void CUsbObject::SetPipeResponsePid(int ch, int nPipe, PID pid)
{
	int nPid = 0;
	switch (pid) {
		case NAK:		nPid = 0;	break;
		case BUF:		nPid = 1;	break;
		case STALL2:	nPid = 2;	break;
		case STALL3:	nPid = 3;	break;
	}

	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 1:	USB0.PIPE1CTR.BIT.PID = nPid;	break;
//				case 2:	USB0.PIPE2CTR.BIT.PID = nPid;	break;
//				case 3:	USB0.PIPE3CTR.BIT.PID = nPid;	break;
//				case 4:	USB0.PIPE4CTR.BIT.PID = nPid;	break;
//				case 5:	USB0.PIPE5CTR.BIT.PID = nPid;	break;
//				case 6:	USB0.PIPE6CTR.BIT.PID = nPid;	break;
//				case 7:	USB0.PIPE7CTR.BIT.PID = nPid;	break;
//				case 8:	USB0.PIPE8CTR.BIT.PID = nPid;	break;
//				case 9:	USB0.PIPE9CTR.BIT.PID = nPid;	break;
			}
			break;
		}
	}
}

CUsbObject::PID CUsbObject::GetPipeResponsePid(int ch, int nPipe)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 9)
		nPipe = 9;

	int nPid = 0;
	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 1:	nPid = USB0.PIPE1CTR.BIT.PID;	break;
//				case 2:	nPid = USB0.PIPE2CTR.BIT.PID;	break;
//				case 3:	nPid = USB0.PIPE3CTR.BIT.PID;	break;
//				case 4:	nPid = USB0.PIPE4CTR.BIT.PID;	break;
//				case 5:	nPid = USB0.PIPE5CTR.BIT.PID;	break;
//				case 6:	nPid = USB0.PIPE6CTR.BIT.PID;	break;
//				case 7:	nPid = USB0.PIPE7CTR.BIT.PID;	break;
//				case 8:	nPid = USB0.PIPE8CTR.BIT.PID;	break;
//				case 9:	nPid = USB0.PIPE9CTR.BIT.PID;	break;
			}
			break;
		}
	}

	PID pid = NAK;
	switch (nPid) {
		case 0:	pid = NAK;		break;
		case 1:	pid = BUF;		break;
		case 2:	pid = STALL2;	break;
		case 3:	pid = STALL3;	break;
	}
	return pid;
}

void CUsbObject::ChangePipeResponsePid(int ch, int nPipe, PID2 pid)
{
	//	NAK(0)   -> BUF(1)		NAK(0)   -> BUF(1)
	//	NAK(0)   -> STALL		NAK(0)   -> STALL(2)
	//	BUF(1)   -> NAK(0)		BUF(1)   -> NAK(0)
	//	BUF(1)   -> STALL		BUF(1)   -> STALL(3)
	//	STALL(2) -> NAK(0)		STALL(2) -> NAK(0)
	//	STALL(2) -> BUF(1)		STALL(2) -> BUF(1)
	//	STALL(3) -> NAK(0)		STALL(3) -> STALL(2) -> NAK(0)
	//	STALL(3) -> BUF(1)		STALL(3) -> STALL(2) -> NAK(0) -> BUF(1)

	PID nCurrentPid = GetPipeResponsePid(ch, nPipe);

	switch (nCurrentPid) {
		case NAK:
		{
			switch (pid) {
				case PID_BUF:
					SetPipeResponsePid(ch, nPipe, BUF);
					break;

				case PID_STALL:
					SetPipeResponsePid(ch, nPipe, STALL2);
					break;
			}
			break;
		}
		case BUF:
		{
			switch (pid) {
				case PID_NAK:
					SetPipeResponsePid(ch, nPipe, NAK);
					break;

				case PID_STALL:
					SetPipeResponsePid(ch, nPipe, STALL3);
					break;
			}
			break;
		}
		case STALL2:
		{
			switch (pid) {
				case PID_NAK:
					SetPipeResponsePid(ch, nPipe, NAK);
					break;

				case PID_BUF:
					SetPipeResponsePid(ch, nPipe, BUF);
					break;
			}
			break;
		}
		case STALL3:
		{
			switch (pid) {
				case PID_NAK:
					SetPipeResponsePid(ch, nPipe, STALL2);
					SetPipeResponsePid(ch, nPipe, NAK);
					break;

				case PID_BUF:
					SetPipeResponsePid(ch, nPipe, STALL2);
					SetPipeResponsePid(ch, nPipe, NAK);
					SetPipeResponsePid(ch, nPipe, BUF);
					break;
			}
			break;
		}
	}
}

bool CUsbObject::IsPipePipeBusy(int ch, int nPipe)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 1:	return USB0.PIPE1CTR.BIT.PBUSY;
//				case 2:	return USB0.PIPE2CTR.BIT.PBUSY;
//				case 3:	return USB0.PIPE3CTR.BIT.PBUSY;
//				case 4:	return USB0.PIPE4CTR.BIT.PBUSY;
//				case 5:	return USB0.PIPE5CTR.BIT.PBUSY;
//				case 6:	return USB0.PIPE6CTR.BIT.PBUSY;
//				case 7:	return USB0.PIPE7CTR.BIT.PBUSY;
//				case 8:	return USB0.PIPE8CTR.BIT.PBUSY;
//				case 9:	return USB0.PIPE9CTR.BIT.PBUSY;
			}
			break;
		}
	}
	return false;
}

CUsbObject::SEQDATA CUsbObject::GetPipeSequenceToggleBit(int ch, int nPipe)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 1:	return (USB0.PIPE1CTR.BIT.SQMON == 0) ? DATA0 : DATA1;
//				case 2:	return (USB0.PIPE2CTR.BIT.SQMON == 0) ? DATA0 : DATA1;
//				case 3:	return (USB0.PIPE3CTR.BIT.SQMON == 0) ? DATA0 : DATA1;
//				case 4:	return (USB0.PIPE4CTR.BIT.SQMON == 0) ? DATA0 : DATA1;
//				case 5:	return (USB0.PIPE5CTR.BIT.SQMON == 0) ? DATA0 : DATA1;
//				case 6:	return (USB0.PIPE6CTR.BIT.SQMON == 0) ? DATA0 : DATA1;
//				case 7:	return (USB0.PIPE7CTR.BIT.SQMON == 0) ? DATA0 : DATA1;
//				case 8:	return (USB0.PIPE8CTR.BIT.SQMON == 0) ? DATA0 : DATA1;
//				case 9:	return (USB0.PIPE9CTR.BIT.SQMON == 0) ? DATA0 : DATA1;
			}
			break;
		}
	}
	return DATA0;
}

void CUsbObject::SetPipeSequenceToggleBit(int ch, int nPipe, SEQDATA seqdata)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
				case 1:
					if (seqdata == DATA0) {
//						USB0.PIPE1CTR.BIT.SQSET = 0;
//						USB0.PIPE1CTR.BIT.SQCLR = 1;
					}
					else {
//						USB0.PIPE1CTR.BIT.SQCLR = 0;
//						USB0.PIPE1CTR.BIT.SQSET = 1;
					}
					break;
				case 2:
					if (seqdata == DATA0) {
//						USB0.PIPE2CTR.BIT.SQSET = 0;
//						USB0.PIPE2CTR.BIT.SQCLR = 1;
					}
					else {
//						USB0.PIPE2CTR.BIT.SQCLR = 0;
//						USB0.PIPE2CTR.BIT.SQSET = 1;
					}
					break;
				case 3:
					if (seqdata == DATA0) {
//						USB0.PIPE3CTR.BIT.SQSET = 0;
//						USB0.PIPE3CTR.BIT.SQCLR = 1;
					}
					else {
//						USB0.PIPE3CTR.BIT.SQCLR = 0;
//						USB0.PIPE3CTR.BIT.SQSET = 1;
					}
					break;
				case 4:
					if (seqdata == DATA0) {
//						USB0.PIPE4CTR.BIT.SQSET = 0;
//						USB0.PIPE4CTR.BIT.SQCLR = 1;
					}
					else {
//						USB0.PIPE4CTR.BIT.SQCLR = 0;
//						USB0.PIPE4CTR.BIT.SQSET = 1;
					}
					break;
				case 5:
					if (seqdata == DATA0) {
//						USB0.PIPE5CTR.BIT.SQSET = 0;
//						USB0.PIPE5CTR.BIT.SQCLR = 1;
					}
					else {
//						USB0.PIPE5CTR.BIT.SQCLR = 0;
//						USB0.PIPE5CTR.BIT.SQSET = 1;
					}
					break;
				case 6:
					if (seqdata == DATA0) {
//						USB0.PIPE6CTR.BIT.SQSET = 0;
//						USB0.PIPE6CTR.BIT.SQCLR = 1;
					}
					else {
//						USB0.PIPE6CTR.BIT.SQCLR = 0;
//						USB0.PIPE6CTR.BIT.SQSET = 1;
					}
					break;
				case 7:
					if (seqdata == DATA0) {
//						USB0.PIPE7CTR.BIT.SQSET = 0;
//						USB0.PIPE7CTR.BIT.SQCLR = 1;
					}
					else {
//						USB0.PIPE7CTR.BIT.SQCLR = 0;
//						USB0.PIPE7CTR.BIT.SQSET = 1;
					}
					break;
				case 8:
					if (seqdata == DATA0) {
//						USB0.PIPE8CTR.BIT.SQSET = 0;
//						USB0.PIPE8CTR.BIT.SQCLR = 1;
					}
					else {
//						USB0.PIPE8CTR.BIT.SQCLR = 0;
//						USB0.PIPE8CTR.BIT.SQSET = 1;
					}
					break;
				case 9:
					if (seqdata == DATA0) {
//						USB0.PIPE9CTR.BIT.SQSET = 0;
//						USB0.PIPE9CTR.BIT.SQCLR = 1;
					}
					else {
//						USB0.PIPE9CTR.BIT.SQCLR = 0;
//						USB0.PIPE9CTR.BIT.SQSET = 1;
					}
					break;
			}
			break;
		}
	}
}

void CUsbObject::SetPipeAutoClearMode(int ch, int nPipe, bool bEnable)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 1:	USB0.PIPE1CTR.BIT.ACLRM = bEnable ? 1 : 0;	break;
//				case 2:	USB0.PIPE2CTR.BIT.ACLRM = bEnable ? 1 : 0;	break;
//				case 3:	USB0.PIPE3CTR.BIT.ACLRM = bEnable ? 1 : 0;	break;
//				case 4:	USB0.PIPE4CTR.BIT.ACLRM = bEnable ? 1 : 0;	break;
//				case 5:	USB0.PIPE5CTR.BIT.ACLRM = bEnable ? 1 : 0;	break;
//				case 6:	USB0.PIPE6CTR.BIT.ACLRM = bEnable ? 1 : 0;	break;
//				case 7:	USB0.PIPE7CTR.BIT.ACLRM = bEnable ? 1 : 0;	break;
//				case 8:	USB0.PIPE8CTR.BIT.ACLRM = bEnable ? 1 : 0;	break;
//				case 9:	USB0.PIPE9CTR.BIT.ACLRM = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

void CUsbObject::EnablePipeAutoResponse(int ch, int nPipe, bool bEnable)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 1:	USB0.PIPE1CTR.BIT.ATREPM = bEnable ? 1 : 0;	break;
//				case 2:	USB0.PIPE2CTR.BIT.ATREPM = bEnable ? 1 : 0;	break;
//				case 3:	USB0.PIPE3CTR.BIT.ATREPM = bEnable ? 1 : 0;	break;
//				case 4:	USB0.PIPE4CTR.BIT.ATREPM = bEnable ? 1 : 0;	break;
//				case 5:	USB0.PIPE5CTR.BIT.ATREPM = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

bool CUsbObject::IsPipeBufferDataExist(int ch, int nPipe)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 1:	return USB0.PIPE1CTR.BIT.INBUFM;
//				case 2:	return USB0.PIPE2CTR.BIT.INBUFM;
//				case 3:	return USB0.PIPE3CTR.BIT.INBUFM;
//				case 4:	return USB0.PIPE4CTR.BIT.INBUFM;
//				case 5:	return USB0.PIPE5CTR.BIT.INBUFM;
			}
			break;
		}
	}
	return false;
}

bool CUsbObject::IsPipeEnableCpuBufferAccess(int ch, int nPipe)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 9)
		nPipe = 9;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 1:	return USB0.PIPE1CTR.BIT.BSTS;
//				case 2:	return USB0.PIPE2CTR.BIT.BSTS;
//				case 3:	return USB0.PIPE3CTR.BIT.BSTS;
//				case 4:	return USB0.PIPE4CTR.BIT.BSTS;
//				case 5:	return USB0.PIPE5CTR.BIT.BSTS;
//				case 6:	return USB0.PIPE6CTR.BIT.BSTS;
//				case 7:	return USB0.PIPE7CTR.BIT.BSTS;
//				case 8:	return USB0.PIPE8CTR.BIT.BSTS;
//				case 9:	return USB0.PIPE9CTR.BIT.BSTS;
			}
			break;
		}
	}
	return false;
}

////////////////////////////////////////
// PIPEnTRE	n = 1 ～ 5
//
void CUsbObject::ClearPipeTransactionCounter(int ch, int nPipe, bool bEnable)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 5)
		nPipe = 5;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 1:	USB0.PIPE1TRE.BIT.TRCLR = bEnable ? 1 : 0;	break;
//				case 2:	USB0.PIPE2TRE.BIT.TRCLR = bEnable ? 1 : 0;	break;
//				case 3:	USB0.PIPE3TRE.BIT.TRCLR = bEnable ? 1 : 0;	break;
//				case 4:	USB0.PIPE4TRE.BIT.TRCLR = bEnable ? 1 : 0;	break;
//				case 5:	USB0.PIPE5TRE.BIT.TRCLR = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

void CUsbObject::EnablePipeTransactionCounter(int ch, int nPipe, bool bEnable)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 5)
		nPipe = 5;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 1:	USB0.PIPE1TRE.BIT.TRENB = bEnable ? 1 : 0;	break;
//				case 2:	USB0.PIPE2TRE.BIT.TRENB = bEnable ? 1 : 0;	break;
//				case 3:	USB0.PIPE3TRE.BIT.TRENB = bEnable ? 1 : 0;	break;
//				case 4:	USB0.PIPE4TRE.BIT.TRENB = bEnable ? 1 : 0;	break;
//				case 5:	USB0.PIPE5TRE.BIT.TRENB = bEnable ? 1 : 0;	break;
			}
			break;
		}
	}
}

////////////////////////////////////////
// PIPEnTRN	n = 1 ～ 5
//
int CUsbObject::GetPipeTransactionCounter(int ch, int nPipe)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 5)
		nPipe = 5;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
				case 1:	return USB0.PIPE1TRN;
				case 2:	return USB0.PIPE2TRN;
				case 3:	return USB0.PIPE3TRN;
				case 4:	return USB0.PIPE4TRN;
				case 5:	return USB0.PIPE5TRN;
			}
			break;
		}
	}
	return 0;
}

void CUsbObject::SetPipeTransactionCounter(int ch, int nPipe, int cnt)
{
	if (nPipe < 1)
		nPipe = 1;
	if (nPipe > 5)
		nPipe = 5;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
				case 1:	USB0.PIPE1TRN = cnt;	break;
				case 2:	USB0.PIPE2TRN = cnt;	break;
				case 3:	USB0.PIPE3TRN = cnt;	break;
				case 4:	USB0.PIPE4TRN = cnt;	break;
				case 5:	USB0.PIPE5TRN = cnt;	break;
			}
			break;
		}
	}
}

////////////////////////////////////////
// DEVADDn	n = 0 ～ 5
//
void CUsbObject::SetPipeUsbSpeed(int ch, int nPipe, USBSPD usbspd)
{
	if (nPipe < 0)
		nPipe = 0;
	if (nPipe > 5)
		nPipe = 5;

	int nUsbSpd = 0;
	if (usbspd == PIPE_LOW_SPEED)
		nUsbSpd = 1;
	else if (usbspd == PIPE_FULL_SPEED)
		nUsbSpd = 2;

	switch (ch) {
		case 0:
		{
			switch (nPipe) {
//				case 0:	USB0.DEVADD0.BIT.USBSPD = nUsbSpd;	break;
//				case 1:	USB0.DEVADD1.BIT.USBSPD = nUsbSpd;	break;
//				case 2:	USB0.DEVADD2.BIT.USBSPD = nUsbSpd;	break;
//				case 3:	USB0.DEVADD3.BIT.USBSPD = nUsbSpd;	break;
//				case 4:	USB0.DEVADD4.BIT.USBSPD = nUsbSpd;	break;
//				case 5:	USB0.DEVADD5.BIT.USBSPD = nUsbSpd;	break;
			}
			break;
		}
	}
}

////////////////////////////////////////
// USBMC
//
void CUsbObject::EnableUsbReferencePowerSupplyCircuit(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.USBMC.BIT.VDDUSBE = bEnable ? 1 : 0;	break;
	}
}

bool CUsbObject::IsEnableUsbReferencePowerSupplyCircuit(int ch)
{
	switch (ch) {
//		case 0:	return USB0.USBMC.BIT.VDDUSBE;
	}
	return false;
}

void CUsbObject::EnableUsbRegulator(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.USBMC.BIT.VDCEN = bEnable ? 1 : 0;		break;
	}
}

bool CUsbObject::IsEnableUsbRegulator(int ch)
{
	switch (ch) {
//		case 0:	return USB0.USBMC.BIT.VDCEN;
	}
	return false;
}

////////////////////////////////////////
// USBBCCTRL0
//
void CUsbObject::EnablePulldownMinus(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.USBBCCTRL0.BIT.RPDME0 = bEnable ? 1 : 0;		break;
	}
}

void CUsbObject::OutputIdpsrc(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.USBBCCTRL0.BIT.IDPSRCE0 = bEnable ? 1 : 0;		break;
	}
}

void CUsbObject::EnableDetectInputDMinusPin(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.USBBCCTRL0.BIT.IDMSINKE0 = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::OutputVdpsrc(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.USBBCCTRL0.BIT.VDPSRCE0 = bEnable ? 1 : 0;		break;
	}
}

void CUsbObject::EnableDetectInputDPlusPin(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.USBBCCTRL0.BIT.IDPSINKE0 = bEnable ? 1 : 0;	break;
	}
}

void CUsbObject::OutputVdmsrc(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.USBBCCTRL0.BIT.VDMSRCE0 = bEnable ? 1 : 0;		break;
	}
}

void CUsbObject::EnableBatteryCharger(int ch, bool bEnable)
{
	switch (ch) {
//		case 0:	USB0.USBBCCTRL0.BIT.BATCHGE0 = bEnable ? 1 : 0;		break;
	}
}

bool CUsbObject::IsDetectInputDMinusPin(int ch)
{
	switch (ch) {
//		case 0:	return USB0.USBBCCTRL0.BIT.CHGDETSTS0;
	}
	return false;
}

bool CUsbObject::IsDetectInputDPlusPin(int ch)
{
	switch (ch) {
//		case 0:	return USB0.USBBCCTRL0.BIT.PDDETSTS0;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// シリアルコミュニケーションインタフェースコントローラ
//

////////////////////////////////////////
// SCIモジュールの設定
//	ch		: チャンネル
//	bRun	: true(開始) ／ false(停止)
//
void CSciObject::RunModule(int ch, bool bRun)
{
	if (bRun) {
		switch (ch) {
			case 0:		CModuleStopObject::RunModule(CModuleStopObject::M_SCI0);	break;
			case 1:		CModuleStopObject::RunModule(CModuleStopObject::M_SCI1);	break;
			case 5:		CModuleStopObject::RunModule(CModuleStopObject::M_SCI5);	break;
			case 6:		CModuleStopObject::RunModule(CModuleStopObject::M_SCI6);	break;
			case 8:		CModuleStopObject::RunModule(CModuleStopObject::M_SCI8);	break;
			case 9:		CModuleStopObject::RunModule(CModuleStopObject::M_SCI9);	break;
			case 12:	CModuleStopObject::RunModule(CModuleStopObject::M_SCI12);	break;
		}
	}
	else {
		switch (ch) {
			case 0:		CModuleStopObject::StopModule(CModuleStopObject::M_SCI0);	break;
			case 1:		CModuleStopObject::StopModule(CModuleStopObject::M_SCI1);	break;
			case 5:		CModuleStopObject::StopModule(CModuleStopObject::M_SCI5);	break;
			case 6:		CModuleStopObject::StopModule(CModuleStopObject::M_SCI6);	break;
			case 8:		CModuleStopObject::StopModule(CModuleStopObject::M_SCI8);	break;
			case 9:		CModuleStopObject::StopModule(CModuleStopObject::M_SCI9);	break;
			case 12:	CModuleStopObject::StopModule(CModuleStopObject::M_SCI12);	break;
		}
	}
}

////////////////////////////////////////
// 受信データの取得
//	ch		: チャンネル
//
unsigned char CSciObject::GetReceiveData(int ch)
{
	switch (ch) {
		case 0:		return SCI0.RDR;
		case 1:		return SCI1.RDR;
		case 5:		return SCI5.RDR;
		case 6:		return SCI6.RDR;
		case 8:		return SCI8.RDR;
		case 9:		return SCI9.RDR;
		case 12:	return SCI12.RDR;
	}
	return 0;
}

unsigned char CSciObject::GetReceiveDataLow(int ch)
{
	switch (ch) {
		case 0:		return SCI0.RDRHL.BYTE.RDRL;
		case 1:		return SCI1.RDRHL.BYTE.RDRL;
		case 5:		return SCI5.RDRHL.BYTE.RDRL;
		case 6:		return SCI6.RDRHL.BYTE.RDRL;
		case 8:		return SCI8.RDRHL.BYTE.RDRL;
		case 9:		return SCI9.RDRHL.BYTE.RDRL;
		case 12:	return SCI12.RDRHL.BYTE.RDRL;
	}
	return 0;
}

unsigned char CSciObject::GetReceiveDataHigh(int ch)
{
	switch (ch) {
		case 0:		return SCI0.RDRHL.BYTE.RDRH;
		case 1:		return SCI1.RDRHL.BYTE.RDRH;
		case 5:		return SCI5.RDRHL.BYTE.RDRH;
		case 6:		return SCI6.RDRHL.BYTE.RDRH;
		case 8:		return SCI8.RDRHL.BYTE.RDRH;
		case 9:		return SCI9.RDRHL.BYTE.RDRH;
		case 12:	return SCI12.RDRHL.BYTE.RDRH;
	}
	return 0;
}

////////////////////////////////////////
// 送信データの設定
//	ch		: チャンネル
//	data	: 転送データ
//
void CSciObject::WriteTransmitData(int ch, unsigned char data)
{
	switch (ch) {
		case 0:		SCI0.TDR  = data;	break;
		case 1:		SCI1.TDR  = data;	break;
		case 5:		SCI5.TDR  = data;	break;
		case 6:		SCI6.TDR  = data;	break;
		case 8:		SCI8.TDR  = data;	break;
		case 9:		SCI9.TDR  = data;	break;
		case 12:	SCI12.TDR = data;	break;
	}
}

void CSciObject::WriteTransmitDataLow(int ch, unsigned char data)
{
	switch (ch) {
		case 0:		SCI0.TDRHL.BYTE.TDRL  = data;	break;
		case 1:		SCI1.TDRHL.BYTE.TDRL  = data;	break;
		case 5:		SCI5.TDRHL.BYTE.TDRL  = data;	break;
		case 6:		SCI6.TDRHL.BYTE.TDRL  = data;	break;
		case 8:		SCI8.TDRHL.BYTE.TDRL  = data;	break;
		case 9:		SCI9.TDRHL.BYTE.TDRL  = data;	break;
		case 12:	SCI12.TDRHL.BYTE.TDRL = data;	break;
	}
}

void CSciObject::WriteTransmitDataHigh(int ch, unsigned char data)
{
	switch (ch) {
		case 0:		SCI0.TDRHL.BYTE.TDRH  = data;	break;
		case 1:		SCI1.TDRHL.BYTE.TDRH  = data;	break;
		case 5:		SCI5.TDRHL.BYTE.TDRH  = data;	break;
		case 6:		SCI6.TDRHL.BYTE.TDRH  = data;	break;
		case 8:		SCI8.TDRHL.BYTE.TDRH  = data;	break;
		case 9:		SCI9.TDRHL.BYTE.TDRH  = data;	break;
		case 12:	SCI12.TDRHL.BYTE.TDRH = data;	break;
	}
}

////////////////////////////////////////
// シリアルモードレジスタの設定
//	ch		: チャンネル
//	flag	: SMR_*
//
void CSciObject::SetSMR(int ch, unsigned char flag)
{
	switch (ch) {
		case 0:		SCI0.SMR.BYTE  = flag;		break;
		case 1:		SCI1.SMR.BYTE  = flag;		break;
		case 5:		SCI5.SMR.BYTE  = flag;		break;
		case 6:		SCI6.SMR.BYTE  = flag;		break;
		case 8:		SCI8.SMR.BYTE  = flag;		break;
		case 9:		SCI9.SMR.BYTE  = flag;		break;
		case 12:	SCI12.SMR.BYTE = flag;		break;
	}
}

////////////////////////////////////////
// シリアルコントロールレジスタの設定
//	ch		: チャンネル
//	flag	: SCR_*
//
void CSciObject::SetSCR(int ch, unsigned char flag)
{
	switch (ch) {
		case 0:		SCI0.SCR.BYTE  = flag;		break;
		case 1:		SCI1.SCR.BYTE  = flag;		break;
		case 5:		SCI5.SCR.BYTE  = flag;		break;
		case 6:		SCI6.SCR.BYTE  = flag;		break;
		case 8:		SCI8.SCR.BYTE  = flag;		break;
		case 9:		SCI9.SCR.BYTE  = flag;		break;
		case 12:	SCI12.SCR.BYTE = flag;		break;
	}
}

////////////////////////////////////////
// 受信割込み要求許可の設定
//	ch		: チャンネル
//	bEnable	: true(許可) ／ false(禁止)
//
void CSciObject::EnableReceiveInterrupt(int ch, bool bEnable)
{
	switch (ch) {
		case 0:		SCI0.SCR.BIT.RIE  = bEnable ? 1 : 0;	break;
		case 1:		SCI1.SCR.BIT.RIE  = bEnable ? 1 : 0;	break;
		case 5:		SCI5.SCR.BIT.RIE  = bEnable ? 1 : 0;	break;
		case 6:		SCI6.SCR.BIT.RIE  = bEnable ? 1 : 0;	break;
		case 8:		SCI8.SCR.BIT.RIE  = bEnable ? 1 : 0;	break;
		case 9:		SCI9.SCR.BIT.RIE  = bEnable ? 1 : 0;	break;
		case 12:	SCI12.SCR.BIT.RIE = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// 受信割込み要求許可の取得
//	ch		: チャンネル
//
bool CSciObject::IsEnableReceiveInterrupt(int ch)
{
	switch (ch) {
		case 0:		return SCI0.SCR.BIT.RIE;
		case 1:		return SCI1.SCR.BIT.RIE;
		case 5:		return SCI5.SCR.BIT.RIE;
		case 6:		return SCI6.SCR.BIT.RIE;
		case 8:		return SCI8.SCR.BIT.RIE;
		case 9:		return SCI9.SCR.BIT.RIE;
		case 12:	return SCI12.SCR.BIT.RIE;
	}
	return false;
}

////////////////////////////////////////
// 受信動作許可の設定
//	ch		: チャンネル
//	bEnable	: true(許可) ／ false(禁止)
//
void CSciObject::EnableRE(int ch, bool bEnable)
{
	switch (ch) {
		case 0:		SCI0.SCR.BIT.RE  = bEnable ? 1 : 0;	break;
		case 1:		SCI1.SCR.BIT.RE  = bEnable ? 1 : 0;	break;
		case 5:		SCI5.SCR.BIT.RE  = bEnable ? 1 : 0;	break;
		case 6:		SCI6.SCR.BIT.RE  = bEnable ? 1 : 0;	break;
		case 8:		SCI8.SCR.BIT.RE  = bEnable ? 1 : 0;	break;
		case 9:		SCI9.SCR.BIT.RE  = bEnable ? 1 : 0;	break;
		case 12:	SCI12.SCR.BIT.RE = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// 送信割込み要求許可の設定
//	ch		: チャンネル
//	bEnable	: true(許可) ／ false(禁止)
//
void CSciObject::EnableTransmitInterrupt(int ch, bool bEnable)
{
	switch (ch) {
		case 0:		SCI0.SCR.BIT.TIE  = bEnable ? 1 : 0;	break;
		case 1:		SCI1.SCR.BIT.TIE  = bEnable ? 1 : 0;	break;
		case 5:		SCI5.SCR.BIT.TIE  = bEnable ? 1 : 0;	break;
		case 6:		SCI6.SCR.BIT.TIE  = bEnable ? 1 : 0;	break;
		case 8:		SCI8.SCR.BIT.TIE  = bEnable ? 1 : 0;	break;
		case 9:		SCI9.SCR.BIT.TIE  = bEnable ? 1 : 0;	break;
		case 12:	SCI12.SCR.BIT.TIE = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// 送信割込み要求許可の取得
//	ch		: チャンネル
//
bool CSciObject::IsEnableTransmitInterrupt(int ch)
{
	switch (ch) {
		case 0:		return SCI0.SCR.BIT.TIE;
		case 1:		return SCI1.SCR.BIT.TIE;
		case 5:		return SCI5.SCR.BIT.TIE;
		case 6:		return SCI6.SCR.BIT.TIE;
		case 8:		return SCI8.SCR.BIT.TIE;
		case 9:		return SCI9.SCR.BIT.TIE;
		case 12:	return SCI12.SCR.BIT.TIE;
	}
	return false;
}

////////////////////////////////////////
// 送信動作許可の設定
//	ch		: チャンネル
//	bEnable	: true(許可) ／ false(禁止)
//
void CSciObject::EnableTE(int ch, bool bEnable)
{
	switch (ch) {
		case 0:		SCI0.SCR.BIT.TE  = bEnable ? 1 : 0;	break;
		case 1:		SCI1.SCR.BIT.TE  = bEnable ? 1 : 0;	break;
		case 5:		SCI5.SCR.BIT.TE  = bEnable ? 1 : 0;	break;
		case 6:		SCI6.SCR.BIT.TE  = bEnable ? 1 : 0;	break;
		case 8:		SCI8.SCR.BIT.TE  = bEnable ? 1 : 0;	break;
		case 9:		SCI9.SCR.BIT.TE  = bEnable ? 1 : 0;	break;
		case 12:	SCI12.SCR.BIT.TE = bEnable ? 1 : 0;	break;
	}
}

////////////////////////////////////////
// シリアルステータスレジスタの取得
//	ch		: チャンネル
//
unsigned char CSciObject::GetSSR(int ch)
{
	switch (ch) {
		case 0:		return SCI0.SSR.BYTE;
		case 1:		return SCI1.SSR.BYTE;
		case 5:		return SCI5.SSR.BYTE;
		case 6:		return SCI6.SSR.BYTE;
		case 8:		return SCI8.SSR.BYTE;
		case 9:		return SCI9.SSR.BYTE;
		case 12:	return SCI12.SSR.BYTE;
	}
	return 0;
}

////////////////////////////////////////
// シリアルステータスレジスタTENDの取得
//	ch		: チャンネル
//
bool CSciObject::GetTEND(int ch)
{
	switch (ch) {
		case 0:		return SCI0.SSR.BIT.TEND;
		case 1:		return SCI1.SSR.BIT.TEND;
		case 5:		return SCI5.SSR.BIT.TEND;
		case 6:		return SCI6.SSR.BIT.TEND;
		case 8:		return SCI8.SSR.BIT.TEND;
		case 9:		return SCI9.SSR.BIT.TEND;
		case 12:	return SCI12.SSR.BIT.TEND;
	}
	return false;
}

////////////////////////////////////////
// シリアルステータスRDRFのクリア
//	ch		: チャンネル
//
void CSciObject::ClearRDRF(int ch)
{
	switch (ch) {
		case 0:		SCI0.SSR.BIT.RDRF  = 0;	break;
		case 1:		SCI1.SSR.BIT.RDRF  = 0;	break;
		case 5:		SCI5.SSR.BIT.RDRF  = 0;	break;
		case 6:		SCI6.SSR.BIT.RDRF  = 0;	break;
		case 8:		SCI8.SSR.BIT.RDRF  = 0;	break;
		case 9:		SCI9.SSR.BIT.RDRF  = 0;	break;
		case 12:	SCI12.SSR.BIT.RDRF = 0;	break;
	}
}

////////////////////////////////////////
// シリアルステータスレジスタRDRFの取得
//	ch		: チャンネル
//
bool CSciObject::GetRDRF(int ch)
{
	switch (ch) {
		case 0:		return SCI0.SSR.BIT.RDRF;
		case 1:		return SCI1.SSR.BIT.RDRF;
		case 5:		return SCI5.SSR.BIT.RDRF;
		case 6:		return SCI6.SSR.BIT.RDRF;
		case 8:		return SCI8.SSR.BIT.RDRF;
		case 9:		return SCI9.SSR.BIT.RDRF;
		case 12:	return SCI12.SSR.BIT.RDRF;
	}
	return false;
}

////////////////////////////////////////
// シリアルステータスレジスタTDREのクリア
//	ch		: チャンネル
//
void CSciObject::ClearTDRE(int ch)
{
	switch (ch) {
		case 0:		SCI0.SSR.BIT.TDRE  = 0;	break;
		case 1:		SCI1.SSR.BIT.TDRE  = 0;	break;
		case 5:		SCI5.SSR.BIT.TDRE  = 0;	break;
		case 6:		SCI6.SSR.BIT.TDRE  = 0;	break;
		case 8:		SCI8.SSR.BIT.TDRE  = 0;	break;
		case 9:		SCI9.SSR.BIT.TDRE  = 0;	break;
		case 12:	SCI12.SSR.BIT.TDRE = 0;	break;
	}
}

////////////////////////////////////////
// シリアルステータスレジスタTDREの取得
//	ch		: チャンネル
//
bool CSciObject::GetTDRE(int ch)
{
	switch (ch) {
		case 0:		return SCI0.SSR.BIT.TDRE;
		case 1:		return SCI1.SSR.BIT.TDRE;
		case 5:		return SCI5.SSR.BIT.TDRE;
		case 6:		return SCI6.SSR.BIT.TDRE;
		case 8:		return SCI8.SSR.BIT.TDRE;
		case 9:		return SCI9.SSR.BIT.TDRE;
		case 12:	return SCI12.SSR.BIT.TDRE;
	}
	return false;
}

////////////////////////////////////////
// エラーフラグの取得
//	ch		: チャンネル
//
unsigned char CSciObject::GetErrorFlag(int ch)
{
	switch (ch) {
		case 0:		return SCI0.SSR.BYTE  & 0x38;
		case 1:		return SCI1.SSR.BYTE  & 0x38;
		case 5:		return SCI5.SSR.BYTE  & 0x38;
		case 6:		return SCI6.SSR.BYTE  & 0x38;
		case 8:		return SCI8.SSR.BYTE  & 0x38;
		case 9:		return SCI9.SSR.BYTE  & 0x38;
		case 12:	return SCI12.SSR.BYTE & 0x38;
	}
	return 0xFF;
}

////////////////////////////////////////
// スマートカードモードレジスタの設定
//	ch		: チャンネル
//	flag	: SCMR_*
//
void CSciObject::SetSCMR(int ch, unsigned char flag)
{
	switch (ch) {
		case 0:		SCI0.SCMR.BYTE  = flag;	break;
		case 1:		SCI1.SCMR.BYTE  = flag;	break;
		case 5:		SCI5.SCMR.BYTE  = flag;	break;
		case 6:		SCI6.SCMR.BYTE  = flag;	break;
		case 8:		SCI8.SCMR.BYTE  = flag;	break;
		case 9:		SCI9.SCMR.BYTE  = flag;	break;
		case 12:	SCI12.SCMR.BYTE = flag;	break;
	}
}

////////////////////////////////////////
// ビットレートレジスタの設定
//	ch		: チャンネル
//	br		: ビットレート
//
void CSciObject::SetBRR(int ch, unsigned char br)
{
	switch (ch) {
		case 0:		SCI0.BRR  = br;	break;
		case 1:		SCI1.BRR  = br;	break;
		case 5:		SCI5.BRR  = br;	break;
		case 6:		SCI6.BRR  = br;	break;
		case 8:		SCI8.BRR  = br;	break;
		case 9:		SCI9.BRR  = br;	break;
		case 12:	SCI12.BRR = br;	break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// 12ビットA/Dコンバータコントローラ
//

////////////////////////////////////////
// S12ADEモジュールの設定
//	bRun	: true(開始) ／ false(停止)
//
void CAdcS12Object::RunModule(bool bRun)
{
	if (bRun) {
		CModuleStopObject::RunModule(CModuleStopObject::M_S12AD);
	}
	else {
		CModuleStopObject::StopModule(CModuleStopObject::M_S12AD);
	}
}

////////////////////////////////////////
// 変換トリガの設定
//	flag	: トリガ
//
void CAdcS12Object::SelectTrigger(unsigned char flag)
{
	S12AD.ADCSR.BIT.EXTRG = flag;
}

////////////////////////////////////////
// スキャン終了割込み許可の設定
//	bEnable	: true(許可) ／ false(禁止)
//
void CAdcS12Object::EnableInterrupt(bool bEnable)
{
	S12AD.ADCSR.BIT.ADIE = bEnable ? 1 : 0;
}

////////////////////////////////////////
// スキャンモード(連続スキャンモード)の設定
//	bContinuous	: true(許可) ／ false(禁止)
//
void CAdcS12Object::SetScanMode(bool bContinuous)
{
	S12AD.ADCSR.BIT.ADCS = bContinuous ? 0x02 : 0x00;
}

////////////////////////////////////////
// A/D変換の開始
//
void CAdcS12Object::Start()
{
	S12AD.ADCSR.BIT.ADST = 1;
}

////////////////////////////////////////
// A/D変換の停止
//
void CAdcS12Object::Stop()
{
	S12AD.ADCSR.BIT.ADST = 0;
}

////////////////////////////////////////
// A/D変換中の取得
//
bool CAdcS12Object::IsConverting()
{
	return S12AD.ADCSR.BIT.ADST;
}

////////////////////////////////////////
// A/D変換チャンネルの設定
//	ch		: チャンネル
//
void CAdcS12Object::SelectChannel(int ch)
{
	S12AD.ADANSA0.WORD = (ch >> 0) & 0x000000FF;
	S12AD.ADANSA1.WORD = (ch >> 8) & 0x0000FFFF;
}

////////////////////////////////////////
// サンプリング時間の設定
//	ch		: チャンネル
//	nState	: ステート数
//
void CAdcS12Object::SelectSamplingState(int ch, unsigned int nState)
{
	if (nState < 6)		// 6ステート以上を設定する
		return;

	switch (ch) {
		case SCAN_0:	S12AD.ADSSTR0 = nState;	break;
		case SCAN_1:	S12AD.ADSSTR1 = nState;	break;
		case SCAN_2:	S12AD.ADSSTR2 = nState;	break;
		case SCAN_3:	S12AD.ADSSTR3 = nState;	break;
		case SCAN_4:	S12AD.ADSSTR4 = nState;	break;
		case SCAN_5:	S12AD.ADSSTR5 = nState;	break;
		case SCAN_6:	S12AD.ADSSTR6 = nState;	break;
		case SCAN_7:	S12AD.ADSSTR7 = nState;	break;
	}

	if (SCAN_16 <= ch && ch <= SCAN_31) {
		S12AD.ADSSTRL = nState;
	}
}
