/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

#include "iodefine.h"
#include "forte32.h"
#include "sysobj.h"
#include "bdc.h"

class CThread0 : public CThread
{
public:
	CThread0()	{}
	~CThread0()	{}
protected:
	int ThreadFunc();
};

class CThread1 : public CThread
{
public:
	CThread1()	{}
	~CThread1()	{}
protected:
	int ThreadFunc();
};

class CThread2 : public CThread
{
public:
	CThread2()	{}
	~CThread2()	{}
protected:
	int ThreadFunc();
};

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

CBdc g_bdc;

void InitializeLed()
{
#if	0

//	PE5	// ena
//	PE4	// ph
	// PB1
	CDioObject::SetDir(CDioObject::P_PB, 1, false);
	CDioObject::SetInputPullup(CDioObject::P_PB, 1, false);
	CDioObject::SetModeFunc(CDioObject::P_PB, 1, false);

	// PE5	ena
	CDioObject::SetDir(CDioObject::P_PE, 5, true);
	CDioObject::SetData(CDioObject::P_PE, 5, false);		// disable
	CDioObject::SetModeFunc(CDioObject::P_PE, 5, false);
	CDioObject::SetOpenDrain(CDioObject::P_PE, 5, false);

	// PE4	phase
	CDioObject::SetDir(CDioObject::P_PE, 4, true);
	CDioObject::SetData(CDioObject::P_PE, 4, false);		// forward
	CDioObject::SetModeFunc(CDioObject::P_PE, 4, false);
	CDioObject::SetOpenDrain(CDioObject::P_PE, 4, false);

#else
	
	CDioObject::SetModeFunc(CDioObject::P_PD, 6, false);	// general purpose i/o
	CDioObject::SetInputPullup(CDioObject::P_PD, 6, false);	// no pullup
	CDioObject::SetData(CDioObject::P_PD, 6, false);		// high
	CDioObject::SetDir(CDioObject::P_PD, 6, true);			// output

	CDioObject::SetModeFunc(CDioObject::P_PD, 7, false);	// general purpose i/o
	CDioObject::SetInputPullup(CDioObject::P_PD, 7, false);	// no pullup
	CDioObject::SetData(CDioObject::P_PD, 7, false);		// high
	CDioObject::SetDir(CDioObject::P_PD, 7, true);			// output


#endif

}

void main(void)
{
	InitializeLed();

	GetTaskCtrl().Initialize(54);

#if 1
	CThread0 th0;
	th0.CreateThread(THREAD_PRIORITY_NORMAL, 256);
	th0.Run();
#else
	CThread1 th1;
	th1.CreateThread(THREAD_PRIORITY_NORMAL, 256);
	CThread2 th2;
	th2.CreateThread(THREAD_PRIORITY_NORMAL, 256);

	th1.Run();
	th2.Run();

#endif
	while (1)
		;

}

int CThread0::ThreadFunc()
{
	CThread1 th1;
	th1.CreateThread(THREAD_PRIORITY_NORMAL, 256);
	CThread2 th2;
	th2.CreateThread(THREAD_PRIORITY_NORMAL, 256);

	th1.Run();
	Sleep(100);
	th2.Run();


	Sleep(INFINITE);

	while (1)
		;

	return 0;
}

CEvent sweve(true);

int CThread1::ThreadFunc()
{

#if	0

	g_bdc.Sleep(false);
	Sleep(100);

	while (1) {
		g_bdc.Forward();
		Sleep(1000);
		
		g_bdc.Brake();
		Sleep(100);

		g_bdc.Reverse();
		Sleep(1000);

		g_bdc.Brake();
		Sleep(100);
	}	
	
#else

	while (1) {

		CDioObject::SetData(CDioObject::P_PD, 6, false);	// low
		Sleep(500);
		CDioObject::SetData(CDioObject::P_PD, 6, true);		// high
		Sleep(500);
	}

#endif

	return 0;
}

int CThread2::ThreadFunc()
{
	while (1) {
/*
		if (CDioObject::GetLineStatus(CDioObject::P_PB, 1) == false)
			sweve.SetEvent();

		Sleep(100);
*/

		CDioObject::SetData(CDioObject::P_PD, 7, false);	// low
		Sleep(100);
		CDioObject::SetData(CDioObject::P_PD, 7, true);		// high
		Sleep(100);
	}
	return 0;
}


#ifdef __cplusplus
void abort(void)
{

}
#endif
