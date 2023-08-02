////////////////////////////////////////////////////////////////////////////////
// RX231
//

#ifndef	_CPU_DEF_H_
#define	_CPU_DEF_H_

////////////////////////////////////////
// CPU Unique Definition

#define	CPU_NAME	"RX231"
#define	REG_NUM		20				// R0Å`R15 + PC + PSW + ACCL + ACCH
#define	VECT_TASK_SWITCH		1	// TRAPA Vector
#define	VECT_SUPERVISOR			2	// TRAPA Vector
#define	VECT_ENTER_CRITICAL		3	// TRAPA Vector
#define	VECT_LEAVE_CRITICAL		4	// TRAPA Vector
#define	VECT_WAIT_US			5	// TRAPA Vector
#define	VECT_RESTORE_SYSTEM		6	// TRAPA Vector
#define	VECT_DISABLE_INTERRUPT	7	// TRAPA Vector
#define	VECT_ENABLE_INTERRUPT	8	// TRAPA Vector

////////////////////////////////////////////////////////////////////////////////
#endif	// _CPU_DEF_H_
