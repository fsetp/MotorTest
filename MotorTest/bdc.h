#ifndef	_BDC_H_
#define	_BDC_H_

#include "forte32\cpu\rx231\peripheral\sysobj.h"

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


class CBdc
{
public:
	CBdc();
	~CBdc();

	void EnableP24(bool bEnable);
	void EnableBDC(bool bEnable);
	void Forward();
	void Reverse();
	void Brake();
	void Sleep(bool bSleep);

private:

protected:

public:

#pragma	interrupt CBdc::KeyInt(vect = 68)
	static void KeyInt();
};

#endif
