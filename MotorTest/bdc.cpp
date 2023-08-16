
#include "bdc.h"

////////////////////////////////////////
//
//	PA3		nFAULT		IN		Interrupt
//	PA1		nSLEEP		OUT
//	PA0		PHASE/PWM	OUT		PWM(1)
//	PE5		EN/IN1		OUT		ENABLE
//	PE4		PH/IN2		OUT		
//	PD7		LED1		OUT
//	PD6		LED2		OUT
//	P14		EN_P24V_S	OUT		
//	P22		EN_VM_BDC	OUT		
//
//	PB1		SW			IRQ4
//


//	nSLEEP	EN		PH		OUT1	OUT2
//	0		x		x		z		z		SLEEP
//	1		0		x		L		H		BRAKE
//	1		1		0		L		H		REVERS
//	1		1		1		H		L		FORWARD
//


CBdc::CBdc()
{
	// PB1	IRQ4
	//
	CDioObject::SetModeFunc(CDioObject::P_PB, 1, true);

	CIcuObject::CleatInterruptRequest(68);
	CIcuObject::SetPriority(68, CIcuObject::PLV1);
	CIcuObject::SetIrqSense(68, CIcuObject::LOW_EDGE);
	CMpcObject::SetPortFunction(CMpcObject::P_PB1, CMpcObject::IRQ4);

	CIcuObject::EnableInterrupt(68, true);

	//	P14		EN_P24V_S	OUT		
	CDioObject::SetData(CDioObject::P_P1, 4, false);
	CDioObject::SetModeFunc(CDioObject::P_P1, 4, false);
	CDioObject::SetOpenDrain(CDioObject::P_P1, 4, false);
	CDioObject::SetDir(CDioObject::P_P1, 4, true);

	//	P22		EN_VM_BDC	OUT		
	CDioObject::SetData(CDioObject::P_P2, 2, false);
	CDioObject::SetModeFunc(CDioObject::P_P2, 2, false);
	CDioObject::SetOpenDrain(CDioObject::P_P2, 2, false);
	CDioObject::SetDir(CDioObject::P_P2, 2, true);

	// nSLEEP
	//
	CDioObject::SetData(CDioObject::P_PA, 1, true);
	CDioObject::SetModeFunc(CDioObject::P_PA, 1, false);
	CDioObject::SetOpenDrain(CDioObject::P_PA, 1, false);
	CDioObject::SetDir(CDioObject::P_PA, 1, true);

	// PMODE
	//
	CDioObject::SetData(CDioObject::P_PA, 0, false);
	CDioObject::SetModeFunc(CDioObject::P_PA, 0, false);
	CDioObject::SetOpenDrain(CDioObject::P_PA, 0, false);
	CDioObject::SetDir(CDioObject::P_PA, 0, true);

	// EN
	//
	CDioObject::SetData(CDioObject::P_PE, 5, false);
	CDioObject::SetModeFunc(CDioObject::P_PE, 5, false);
	CDioObject::SetOpenDrain(CDioObject::P_PE, 5, false);
	CDioObject::SetDir(CDioObject::P_PE, 5, true);

	// PH
	//
	CDioObject::SetData(CDioObject::P_PE, 4, false);
	CDioObject::SetModeFunc(CDioObject::P_PE, 4, false);
	CDioObject::SetOpenDrain(CDioObject::P_PE, 4, false);
	CDioObject::SetDir(CDioObject::P_PE, 4, true);

	// Switch
	//
	CDioObject::SetData(CDioObject::P_PB, 1, false);
	CDioObject::SetModeFunc(CDioObject::P_PB, 1, false);
	CDioObject::SetOpenDrain(CDioObject::P_PB, 1, false);
	CDioObject::SetDir(CDioObject::P_PB, 1, false);
	CDioObject::SetInputPullup(CDioObject::P_PB, 1, false);
}

CBdc::~CBdc()
{
}

void CBdc::EnableP24(bool bEnable)
{
	CDioObject::SetData(CDioObject::P_P1, 4, bEnable);
}

void CBdc::EnableBDC(bool bEnable)
{
	CDioObject::SetData(CDioObject::P_P2, 2, bEnable);
}

void CBdc::Forward()
{
	CDioObject::SetData(CDioObject::P_PE, 5, true);
	CDioObject::SetData(CDioObject::P_PE, 4, true);
}

void CBdc::Reverse()
{
	CDioObject::SetData(CDioObject::P_PE, 5, true);
	CDioObject::SetData(CDioObject::P_PE, 4, false);
}

void CBdc::Brake()
{
	CDioObject::SetData(CDioObject::P_PE, 5, false);
	CDioObject::SetData(CDioObject::P_PE, 4, false);
}

void CBdc::Sleep(bool bSleep)
{
	CDioObject::SetData(CDioObject::P_PA, 1, !bSleep);
}

bool bLed = true;

void CBdc::KeyInt()
{
	CDioObject::SetData(CDioObject::P_PD, 6, bLed);	// low
	bLed = !bLed;
}
