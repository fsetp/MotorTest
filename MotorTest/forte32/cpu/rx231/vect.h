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
/************************************************************************
*
* Device     : RX/RX200/RX231
*
* File Name  : vect.h
*
* Abstract   : Definition of Vector.
*
* History    : 0.50  (2014-09-18)  [Hardware Manual Revision : 0.50]
*            : 1.00  (2015-05-18)  [Hardware Manual Revision : 1.00]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2015 (2014) Renesas Electronics Corporation.
*
************************************************************************/

// Exception(Supervisor Instruction)
#pragma interrupt (Excep_SuperVisorInst)
void Excep_SuperVisorInst(void);

// Exception(Access Instruction)
#pragma interrupt (Excep_AccessInst)
void Excep_AccessInst(void);

// Exception(Undefined Instruction)
#pragma interrupt (Excep_UndefinedInst)
void Excep_UndefinedInst(void);

// Exception(Floating Point)
#pragma interrupt (Excep_FloatingPoint)
void Excep_FloatingPoint(void);

// NMI
#pragma interrupt (NonMaskableInterrupt)
void NonMaskableInterrupt(void);

// Dummy
#pragma interrupt (Dummy)
void Dummy(void);

// BRK
#pragma interrupt (Excep_BRK(vect=0))
void Excep_BRK(void);

// vector  1 reserved
// vector  2 reserved
// vector  3 reserved
// vector  4 reserved
// vector  5 reserved
// vector  6 reserved
// vector  7 reserved
// vector  8 reserved
// vector  9 reserved
// vector 10 reserved
// vector 11 reserved
// vector 12 reserved
// vector 13 reserved
// vector 14 reserved
// vector 15 reserved

// BSC BUSERR
#pragma interrupt (Excep_BSC_BUSERR(vect=16))
void Excep_BSC_BUSERR(void);

// vector 17 reserved
// vector 18 reserved
// vector 19 reserved
// vector 20 reserved
// vector 21 reserved
// vector 22 reserved

// FCU FRDYI
#pragma interrupt (Excep_FCU_FRDYI(vect=23))
void Excep_FCU_FRDYI(void);

// vector 24 reserved
// vector 25 reserved
// vector 26 reserved

// ICU SWINT
#pragma interrupt (Excep_ICU_SWINT(vect=27))
void Excep_ICU_SWINT(void);

// CMT0 CMI0
//#pragma interrupt (Excep_CMT0_CMI0(vect=28))
//void Excep_CMT0_CMI0(void);

// CMT1 CMI1
#pragma interrupt (Excep_CMT1_CMI1(vect=29))
void Excep_CMT1_CMI1(void);

// CMT2 CMI2
#pragma interrupt (Excep_CMT2_CMI2(vect=30))
void Excep_CMT2_CMI2(void);

// CMT3 CMI3
#pragma interrupt (Excep_CMT3_CMI3(vect=31))
void Excep_CMT3_CMI3(void);

// CAC FERRF
#pragma interrupt (Excep_CAC_FERRF(vect=32))
void Excep_CAC_FERRF(void);

// CAC MENDF
#pragma interrupt (Excep_CAC_MENDF(vect=33))
void Excep_CAC_MENDF(void);

// CAC OVFF
#pragma interrupt (Excep_CAC_OVFF(vect=34))
void Excep_CAC_OVFF(void);

// vector 35 reserved

// USB0 D0FIFO0
#pragma interrupt (Excep_USB0_D0FIFO0(vect=36))
void Excep_USB0_D0FIFO0(void);

// USB0 D1FIFO0
#pragma interrupt (Excep_USB0_D1FIFO0(vect=37))
void Excep_USB0_D1FIFO0(void);

// USB0 USBI0
#pragma interrupt (Excep_USB0_USBI0(vect=38))
void Excep_USB0_USBI0(void);

// vector 39 reserved

// SDHI SBFAI
#pragma interrupt (Excep_SDHI_SBFAI(vect=40))
void Excep_SDHI_SBFAI(void);

// SDHI CDETI
#pragma interrupt (Excep_SDHI_CDETI(vect=41))
void Excep_SDHI_CDETI(void);

// SDHI CACI
#pragma interrupt (Excep_SDHI_CACI(vect=42))
void Excep_SDHI_CACI(void);

// SDHI SDACI
#pragma interrupt (Excep_SDHI_SDACI(vect=43))
void Excep_SDHI_SDACI(void);

// RSPI0 SPEI0
#pragma interrupt (Excep_RSPI0_SPEI0(vect=44))
void Excep_RSPI0_SPEI0(void);

// RSPI0 SPRI0
#pragma interrupt (Excep_RSPI0_SPRI0(vect=45))
void Excep_RSPI0_SPRI0(void);

// RSPI0 SPTI0
#pragma interrupt (Excep_RSPI0_SPTI0(vect=46))
void Excep_RSPI0_SPTI0(void);

// RSPI0 SPII0
#pragma interrupt (Excep_RSPI0_SPII0(vect=47))
void Excep_RSPI0_SPII0(void);

// vector 48 reserved
// vector 49 reserved
// vector 50 reserved
// vector 51 reserved

// RSCAN COMFRXINT
#pragma interrupt (Excep_RSCAN_COMFRXINT(vect=52))
void Excep_RSCAN_COMFRXINT(void);

// RSCAN RXFINT
#pragma interrupt (Excep_RSCAN_RXFINT(vect=53))
void Excep_RSCAN_RXFINT(void);

// RSCAN TXINT
#pragma interrupt (Excep_RSCAN_TXINT(vect=54))
void Excep_RSCAN_TXINT(void);

// RSCAN CHERRINT
#pragma interrupt (Excep_RSCAN_CHERRINT(vect=55))
void Excep_RSCAN_CHERRINT(void);

// RSCAN GLERRINT
#pragma interrupt (Excep_RSCAN_GLERRINT(vect=56))
void Excep_RSCAN_GLERRINT(void);

// DOC DOPCF
#pragma interrupt (Excep_DOC_DOPCF(vect=57))
void Excep_DOC_DOPCF(void);

// CMPB CMPB0
#pragma interrupt (Excep_CMPB_CMPB0(vect=58))
void Excep_CMPB_CMPB0(void);

// CMPB CMPB1
#pragma interrupt (Excep_CMPB_CMPB1(vect=59))
void Excep_CMPB_CMPB1(void);

// CTSU CTSUWR
#pragma interrupt (Excep_CTSU_CTSUWR(vect=60))
void Excep_CTSU_CTSUWR(void);

// CTSU CTSURD
#pragma interrupt (Excep_CTSU_CTSURD(vect=61))
void Excep_CTSU_CTSURD(void);

// CTSU CTSUFN
#pragma interrupt (Excep_CTSU_CTSUFN(vect=62))
void Excep_CTSU_CTSUFN(void);

// RTC CUP
#pragma interrupt (Excep_RTC_CUP(vect=63))
void Excep_RTC_CUP(void);

// ICU IRQ0
#pragma interrupt (Excep_ICU_IRQ0(vect=64))
void Excep_ICU_IRQ0(void);

// ICU IRQ1
#pragma interrupt (Excep_ICU_IRQ1(vect=65))
void Excep_ICU_IRQ1(void);

// ICU IRQ2
#pragma interrupt (Excep_ICU_IRQ2(vect=66))
void Excep_ICU_IRQ2(void);

// ICU IRQ3
#pragma interrupt (Excep_ICU_IRQ3(vect=67))
void Excep_ICU_IRQ3(void);

// ICU IRQ4
//#pragma interrupt (Excep_ICU_IRQ4(vect=68))
void Excep_ICU_IRQ4(void);

// ICU IRQ5
#pragma interrupt (Excep_ICU_IRQ5(vect=69))
void Excep_ICU_IRQ5(void);

// ICU IRQ6
#pragma interrupt (Excep_ICU_IRQ6(vect=70))
void Excep_ICU_IRQ6(void);

// ICU IRQ7
#pragma interrupt (Excep_ICU_IRQ7(vect=71))
void Excep_ICU_IRQ7(void);

// vector 72 reserved
// vector 73 reserved
// vector 74 reserved
// vector 75 reserved
// vector 76 reserved
// vector 77 reserved
// vector 78 reserved
// vector 79 reserved

// ELC ELSR8I
#pragma interrupt (Excep_ELC_ELSR8I(vect=80))
void Excep_ELC_ELSR8I(void);

// vector 81 reserved
// vector 82 reserved
// vector 83 reserved
// vector 84 reserved
// vector 85 reserved
// vector 86 reserved
// vector 87 reserved

// LVD LVD1
#pragma interrupt (Excep_LVD_LVD1(vect=88))
void Excep_LVD_LVD1(void);

// LVD LVD2
#pragma interrupt (Excep_LVD_LVD2(vect=89))
void Excep_LVD_LVD2(void);

// CMPA CMPA1
//#pragma interrupt (Excep_CMPA_CMPA1(vect=88))
//void Excep_CMPA_CMPA1(void);

// CMPA CMPA2
//#pragma interrupt (Excep_CMPA_CMPA2(vect=89))
//void Excep_CMPA_CMPA2(void);

// USB0 USBR0
#pragma interrupt (Excep_USB0_USBR0(vect=90))
void Excep_USB0_USBR0(void);

// VBATT VBTLVDI
#pragma interrupt (Excep_VBATT_VBTLVDI(vect=91))
void Excep_VBATT_VBTLVDI(void);

// RTC ALM
#pragma interrupt (Excep_RTC_ALM(vect=92))
void Excep_RTC_ALM(void);

// RTC PRD
#pragma interrupt (Excep_RTC_PRD(vect=93))
void Excep_RTC_PRD(void);

// vector 94 reserved
// vector 95 reserved
// vector 96 reserved
// vector 97 reserved
// vector 98 reserved
// vector 99 reserved
// vector 100 reserved
// vector 101 reserved

// S12AD S12ADI0
#pragma interrupt (Excep_S12AD_S12ADI0(vect=102))
void Excep_S12AD_S12ADI0(void);

// S12AD GBADI
#pragma interrupt (Excep_S12AD_GBADI(vect=103))
void Excep_S12AD_GBADI(void);

// CMPB1 CMPB2
#pragma interrupt (Excep_CMPB1_CMPB2(vect=104))
void Excep_CMPB1_CMPB2(void);

// CMPB1 CMPB3
#pragma interrupt (Excep_CMPB1_CMPB3(vect=105))
void Excep_CMPB1_CMPB3(void);

// ELC ELSR18I
#pragma interrupt (Excep_ELC_ELSR18I(vect=106))
void Excep_ELC_ELSR18I(void);

// ELC ELSR19I
#pragma interrupt (Excep_ELC_ELSR19I(vect=107))
void Excep_ELC_ELSR19I(void);

// SSI0 SSIF0
#pragma interrupt (Excep_SSI0_SSIF0(vect=108))
void Excep_SSI0_SSIF0(void);

// SSI0 SSIRXI0
#pragma interrupt (Excep_SSI0_SSIRXI0(vect=109))
void Excep_SSI0_SSIRXI0(void);

// SSI0 SSITXI0
#pragma interrupt (Excep_SSI0_SSITXI0(vect=110))
void Excep_SSI0_SSITXI0(void);

// SECURITY RD
#pragma interrupt (Excep_SECURITY_RD(vect=111))
void Excep_SECURITY_RD(void);

// SECURITY WR
#pragma interrupt (Excep_SECURITY_WR(vect=112))
void Excep_SECURITY_WR(void);

// SECURITY ERR
#pragma interrupt (Excep_SECURITY_ERR(vect=113))
void Excep_SECURITY_ERR(void);

// MTU0 TGIA0
#pragma interrupt (Excep_MTU0_TGIA0(vect=114))
void Excep_MTU0_TGIA0(void);

// MTU0 TGIB0
#pragma interrupt (Excep_MTU0_TGIB0(vect=115))
void Excep_MTU0_TGIB0(void);

// MTU0 TGIC0
#pragma interrupt (Excep_MTU0_TGIC0(vect=116))
void Excep_MTU0_TGIC0(void);

// MTU0 TGID0
#pragma interrupt (Excep_MTU0_TGID0(vect=117))
void Excep_MTU0_TGID0(void);

// MTU0 TCIV0
#pragma interrupt (Excep_MTU0_TCIV0(vect=118))
void Excep_MTU0_TCIV0(void);

// MTU0 TGIE0
#pragma interrupt (Excep_MTU0_TGIE0(vect=119))
void Excep_MTU0_TGIE0(void);

// MTU0 TGIF0
#pragma interrupt (Excep_MTU0_TGIF0(vect=120))
void Excep_MTU0_TGIF0(void);

// MTU1 TGIA1
#pragma interrupt (Excep_MTU1_TGIA1(vect=121))
void Excep_MTU1_TGIA1(void);

// MTU1 TGIB1
#pragma interrupt (Excep_MTU1_TGIB1(vect=122))
void Excep_MTU1_TGIB1(void);

// MTU1 TCIV1
#pragma interrupt (Excep_MTU1_TCIV1(vect=123))
void Excep_MTU1_TCIV1(void);

// MTU1 TCIU1
#pragma interrupt (Excep_MTU1_TCIU1(vect=124))
void Excep_MTU1_TCIU1(void);

// MTU2 TGIA2
#pragma interrupt (Excep_MTU2_TGIA2(vect=125))
void Excep_MTU2_TGIA2(void);

// MTU2 TGIB2
#pragma interrupt (Excep_MTU2_TGIB2(vect=126))
void Excep_MTU2_TGIB2(void);

// MTU2 TCIV2
#pragma interrupt (Excep_MTU2_TCIV2(vect=127))
void Excep_MTU2_TCIV2(void);

// MTU2 TCIU2
#pragma interrupt (Excep_MTU2_TCIU2(vect=128))
void Excep_MTU2_TCIU2(void);

// MTU3 TGIA3
#pragma interrupt (Excep_MTU3_TGIA3(vect=129))
void Excep_MTU3_TGIA3(void);

// MTU3 TGIB3
#pragma interrupt (Excep_MTU3_TGIB3(vect=130))
void Excep_MTU3_TGIB3(void);

// MTU3 TGIC3
#pragma interrupt (Excep_MTU3_TGIC3(vect=131))
void Excep_MTU3_TGIC3(void);

// MTU3 TGID3
#pragma interrupt (Excep_MTU3_TGID3(vect=132))
void Excep_MTU3_TGID3(void);

// MTU3 TCIV3
#pragma interrupt (Excep_MTU3_TCIV3(vect=133))
void Excep_MTU3_TCIV3(void);

// MTU4 TGIA4
#pragma interrupt (Excep_MTU4_TGIA4(vect=134))
void Excep_MTU4_TGIA4(void);

// MTU4 TGIB4
#pragma interrupt (Excep_MTU4_TGIB4(vect=135))
void Excep_MTU4_TGIB4(void);

// MTU4 TGIC4
#pragma interrupt (Excep_MTU4_TGIC4(vect=136))
void Excep_MTU4_TGIC4(void);

// MTU4 TGID4
#pragma interrupt (Excep_MTU4_TGID4(vect=137))
void Excep_MTU4_TGID4(void);

// MTU4 TCIV4
#pragma interrupt (Excep_MTU4_TCIV4(vect=138))
void Excep_MTU4_TCIV4(void);

// MTU5 TGIU5
#pragma interrupt (Excep_MTU5_TGIU5(vect=139))
void Excep_MTU5_TGIU5(void);

// MTU5 TGIV5
#pragma interrupt (Excep_MTU5_TGIV5(vect=140))
void Excep_MTU5_TGIV5(void);

// MTU5 TGIW5
#pragma interrupt (Excep_MTU5_TGIW5(vect=141))
void Excep_MTU5_TGIW5(void);

// TPU0 TGI0A
#pragma interrupt (Excep_TPU0_TGI0A(vect=142))
void Excep_TPU0_TGI0A(void);

// TPU0 TGI0B
#pragma interrupt (Excep_TPU0_TGI0B(vect=143))
void Excep_TPU0_TGI0B(void);

// TPU0 TGI0C
#pragma interrupt (Excep_TPU0_TGI0C(vect=144))
void Excep_TPU0_TGI0C(void);

// TPU0 TGI0D
#pragma interrupt (Excep_TPU0_TGI0D(vect=145))
void Excep_TPU0_TGI0D(void);

// TPU0 TCI0V
#pragma interrupt (Excep_TPU0_TCI0V(vect=146))
void Excep_TPU0_TCI0V(void);

// TPU1 TGI1A
#pragma interrupt (Excep_TPU1_TGI1A(vect=147))
void Excep_TPU1_TGI1A(void);

// TPU1 TGI1B
#pragma interrupt (Excep_TPU1_TGI1B(vect=148))
void Excep_TPU1_TGI1B(void);

// TPU1 TCI1V
#pragma interrupt (Excep_TPU1_TCI1V(vect=149))
void Excep_TPU1_TCI1V(void);

// TPU1 TCI1U
#pragma interrupt (Excep_TPU1_TCI1U(vect=150))
void Excep_TPU1_TCI1U(void);

// TPU2 TGI2A
#pragma interrupt (Excep_TPU2_TGI2A(vect=151))
void Excep_TPU2_TGI2A(void);

// TPU2 TGI2B
#pragma interrupt (Excep_TPU2_TGI2B(vect=152))
void Excep_TPU2_TGI2B(void);

// TPU2 TCI2V
#pragma interrupt (Excep_TPU2_TCI2V(vect=153))
void Excep_TPU2_TCI2V(void);

// TPU2 TCI2U
#pragma interrupt (Excep_TPU2_TCI2U(vect=154))
void Excep_TPU2_TCI2U(void);

// TPU3 TGI3A
#pragma interrupt (Excep_TPU3_TGI3A(vect=155))
void Excep_TPU3_TGI3A(void);

// TPU3 TGI3B
#pragma interrupt (Excep_TPU3_TGI3B(vect=156))
void Excep_TPU3_TGI3B(void);

// TPU3 TGI3C
#pragma interrupt (Excep_TPU3_TGI3C(vect=157))
void Excep_TPU3_TGI3C(void);

// TPU3 TGI3D
#pragma interrupt (Excep_TPU3_TGI3D(vect=158))
void Excep_TPU3_TGI3D(void);

// TPU3 TCI3V
#pragma interrupt (Excep_TPU3_TCI3V(vect=159))
void Excep_TPU3_TCI3V(void);

// TPU4 TGI4A
#pragma interrupt (Excep_TPU4_TGI4A(vect=160))
void Excep_TPU4_TGI4A(void);

// TPU4 TGI4B
#pragma interrupt (Excep_TPU4_TGI4B(vect=161))
void Excep_TPU4_TGI4B(void);

// TPU4 TCI4V
#pragma interrupt (Excep_TPU4_TCI4V(vect=162))
void Excep_TPU4_TCI4V(void);

// TPU4 TCI4U
#pragma interrupt (Excep_TPU4_TCI4U(vect=163))
void Excep_TPU4_TCI4U(void);

// TPU5 TGI5A
#pragma interrupt (Excep_TPU5_TGI5A(vect=164))
void Excep_TPU5_TGI5A(void);

// TPU5 TGI5B
#pragma interrupt (Excep_TPU5_TGI5B(vect=165))
void Excep_TPU5_TGI5B(void);

// TPU5 TCI5V
#pragma interrupt (Excep_TPU5_TCI5V(vect=166))
void Excep_TPU5_TCI5V(void);

// TPU5 TCI5U
#pragma interrupt (Excep_TPU5_TCI5U(vect=167))
void Excep_TPU5_TCI5U(void);

// vector 168 reserved
// vector 169 reserved

// POE OEI1
#pragma interrupt (Excep_POE_OEI1(vect=170))
void Excep_POE_OEI1(void);

// POE OEI2
#pragma interrupt (Excep_POE_OEI2(vect=171))
void Excep_POE_OEI2(void);

// vector 172 reserved
// vector 173 reserved

// TMR0 CMIA0
#pragma interrupt (Excep_TMR0_CMIA0(vect=174))
void Excep_TMR0_CMIA0(void);

// TMR0 CMIB0
#pragma interrupt (Excep_TMR0_CMIB0(vect=175))
void Excep_TMR0_CMIB0(void);

// TMR0 OVI0
#pragma interrupt (Excep_TMR0_OVI0(vect=176))
void Excep_TMR0_OVI0(void);

// TMR1 CMIA1
#pragma interrupt (Excep_TMR1_CMIA1(vect=177))
void Excep_TMR1_CMIA1(void);

// TMR1 CMIB1
#pragma interrupt (Excep_TMR1_CMIB1(vect=178))
void Excep_TMR1_CMIB1(void);

// TMR1 OVI1
#pragma interrupt (Excep_TMR1_OVI1(vect=179))
void Excep_TMR1_OVI1(void);

// TMR2 CMIA2
#pragma interrupt (Excep_TMR2_CMIA2(vect=180))
void Excep_TMR2_CMIA2(void);

// TMR2 CMIB2
#pragma interrupt (Excep_TMR2_CMIB2(vect=181))
void Excep_TMR2_CMIB2(void);

// TMR2 OVI2
#pragma interrupt (Excep_TMR2_OVI2(vect=182))
void Excep_TMR2_OVI2(void);

// TMR3 CMIA3
#pragma interrupt (Excep_TMR3_CMIA3(vect=183))
void Excep_TMR3_CMIA3(void);

// TMR3 CMIB3
#pragma interrupt (Excep_TMR3_CMIB3(vect=184))
void Excep_TMR3_CMIB3(void);

// TMR3 OVI3
#pragma interrupt (Excep_TMR3_OVI3(vect=185))
void Excep_TMR3_OVI3(void);

// vector 186 reserved
// vector 187 reserved
// vector 188 reserved
// vector 189 reserved
// vector 190 reserved
// vector 191 reserved
// vector 192 reserved
// vector 193 reserved
// vector 194 reserved
// vector 195 reserved
// vector 196 reserved
// vector 197 reserved

// DMAC DMAC0I
#pragma interrupt (Excep_DMAC_DMAC0I(vect=198))
void Excep_DMAC_DMAC0I(void);

// DMAC DMAC1I
#pragma interrupt (Excep_DMAC_DMAC1I(vect=199))
void Excep_DMAC_DMAC1I(void);

// DMAC DMAC2I
#pragma interrupt (Excep_DMAC_DMAC2I(vect=200))
void Excep_DMAC_DMAC2I(void);

// DMAC DMAC3I
#pragma interrupt (Excep_DMAC_DMAC3I(vect=201))
void Excep_DMAC_DMAC3I(void);

// vector 202 reserved
// vector 203 reserved
// vector 204 reserved
// vector 205 reserved
// vector 206 reserved
// vector 207 reserved
// vector 208 reserved
// vector 209 reserved
// vector 210 reserved
// vector 211 reserved
// vector 212 reserved
// vector 213 reserved

// SCI0 ERI0
#pragma interrupt (Excep_SCI0_ERI0(vect=214))
void Excep_SCI0_ERI0(void);

// SCI0 RXI0
#pragma interrupt (Excep_SCI0_RXI0(vect=215))
void Excep_SCI0_RXI0(void);

// SCI0 TXI0
#pragma interrupt (Excep_SCI0_TXI0(vect=216))
void Excep_SCI0_TXI0(void);

// SCI0 TEI0
#pragma interrupt (Excep_SCI0_TEI0(vect=217))
void Excep_SCI0_TEI0(void);

// SCI1 ERI1
#pragma interrupt (Excep_SCI1_ERI1(vect=218))
void Excep_SCI1_ERI1(void);

// SCI1 RXI1
#pragma interrupt (Excep_SCI1_RXI1(vect=219))
void Excep_SCI1_RXI1(void);

// SCI1 TXI1
#pragma interrupt (Excep_SCI1_TXI1(vect=220))
void Excep_SCI1_TXI1(void);

// SCI1 TEI1
#pragma interrupt (Excep_SCI1_TEI1(vect=221))
void Excep_SCI1_TEI1(void);

// SCI5 ERI5
#pragma interrupt (Excep_SCI5_ERI5(vect=222))
void Excep_SCI5_ERI5(void);

// SCI5 RXI5
#pragma interrupt (Excep_SCI5_RXI5(vect=223))
void Excep_SCI5_RXI5(void);

// SCI5 TXI5
#pragma interrupt (Excep_SCI5_TXI5(vect=224))
void Excep_SCI5_TXI5(void);

// SCI5 TEI5
#pragma interrupt (Excep_SCI5_TEI5(vect=225))
void Excep_SCI5_TEI5(void);

// SCI6 ERI6
#pragma interrupt (Excep_SCI6_ERI6(vect=226))
void Excep_SCI6_ERI6(void);

// SCI6 RXI6
#pragma interrupt (Excep_SCI6_RXI6(vect=227))
void Excep_SCI6_RXI6(void);

// SCI6 TXI6
#pragma interrupt (Excep_SCI6_TXI6(vect=228))
void Excep_SCI6_TXI6(void);

// SCI6 TEI6
#pragma interrupt (Excep_SCI6_TEI6(vect=229))
void Excep_SCI6_TEI6(void);

// SCI8 ERI8
#pragma interrupt (Excep_SCI8_ERI8(vect=230))
void Excep_SCI8_ERI8(void);

// SCI8 RXI8
#pragma interrupt (Excep_SCI8_RXI8(vect=231))
void Excep_SCI8_RXI8(void);

// SCI8 TXI8
#pragma interrupt (Excep_SCI8_TXI8(vect=232))
void Excep_SCI8_TXI8(void);

// SCI8 TEI8
#pragma interrupt (Excep_SCI8_TEI8(vect=233))
void Excep_SCI8_TEI8(void);

// SCI9 ERI9
#pragma interrupt (Excep_SCI9_ERI9(vect=234))
void Excep_SCI9_ERI9(void);

// SCI9 RXI9
#pragma interrupt (Excep_SCI9_RXI9(vect=235))
void Excep_SCI9_RXI9(void);

// SCI9 TXI9
#pragma interrupt (Excep_SCI9_TXI9(vect=236))
void Excep_SCI9_TXI9(void);

// SCI9 TEI9
#pragma interrupt (Excep_SCI9_TEI9(vect=237))
void Excep_SCI9_TEI9(void);

// SCI12 ERI12
#pragma interrupt (Excep_SCI12_ERI12(vect=238))
void Excep_SCI12_ERI12(void);

// SCI12 RXI12
#pragma interrupt (Excep_SCI12_RXI12(vect=239))
void Excep_SCI12_RXI12(void);

// SCI12 TXI12
#pragma interrupt (Excep_SCI12_TXI12(vect=240))
void Excep_SCI12_TXI12(void);

// SCI12 TEI12
#pragma interrupt (Excep_SCI12_TEI12(vect=241))
void Excep_SCI12_TEI12(void);

// SCI12 SCIX0
#pragma interrupt (Excep_SCI12_SCIX0(vect=242))
void Excep_SCI12_SCIX0(void);

// SCI12 SCIX1
#pragma interrupt (Excep_SCI12_SCIX1(vect=243))
void Excep_SCI12_SCIX1(void);

// SCI12 SCIX2
#pragma interrupt (Excep_SCI12_SCIX2(vect=244))
void Excep_SCI12_SCIX2(void);

// SCI12 SCIX3
#pragma interrupt (Excep_SCI12_SCIX3(vect=245))
void Excep_SCI12_SCIX3(void);

// RIIC0 EEI0
#pragma interrupt (Excep_RIIC0_EEI0(vect=246))
void Excep_RIIC0_EEI0(void);

// RIIC0 RXI0
#pragma interrupt (Excep_RIIC0_RXI0(vect=247))
void Excep_RIIC0_RXI0(void);

// RIIC0 TXI0
#pragma interrupt (Excep_RIIC0_TXI0(vect=248))
void Excep_RIIC0_TXI0(void);

// RIIC0 TEI0
#pragma interrupt (Excep_RIIC0_TEI0(vect=249))
void Excep_RIIC0_TEI0(void);

// vector 250 reserved
// vector 251 reserved
// vector 252 reserved
// vector 253 reserved
// vector 254 reserved
// vector 255 reserved

//;<<VECTOR DATA START (POWER ON RESET)>>
//;Power On Reset PC
extern void PowerON_Reset_PC(void);
//;<<VECTOR DATA END (POWER ON RESET)>>
