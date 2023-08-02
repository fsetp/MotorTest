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
//
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
	CBdc()
	{
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

	~CBdc()
	{
	}

	void Forward()
	{
		CDioObject::SetData(CDioObject::P_PE, 5, true);
		CDioObject::SetData(CDioObject::P_PE, 4, true);
	}
	
	void Reverse()
	{
		CDioObject::SetData(CDioObject::P_PE, 5, true);
		CDioObject::SetData(CDioObject::P_PE, 4, false);
	}

	void Brake()
	{
		CDioObject::SetData(CDioObject::P_PE, 5, false);
		CDioObject::SetData(CDioObject::P_PE, 4, false);
	}

	void Sleep(bool bSleep)
	{
		CDioObject::SetData(CDioObject::P_PA, 1, !bSleep);
	}

private:


protected:

};

#endif
