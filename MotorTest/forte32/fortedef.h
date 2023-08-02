////////////////////////////////////////////////////////////////////////////////
// Forte32 Global Definitions
//
//

#ifndef	_FORTEDEF_H_
#define	_FORTEDEF_H_

#include "cpu/rx231/cpu_def.h"
#include "cpu/rx231/iodefine.h"

////////////////////////////////////////
// Forte32 Global Definitions
//
#define	MAX_TASKS				32		// Maximum Task
#define	MAX_SYNC_OBJECTS		32
#define	MAX_INTERVAL_CALLBACK	10
#define	MAX_WAIT_HANDLES		4
#define	SYS_STACK_SIZE			512		// System Call Stack Size
#define	SYS_HEAP_SIZE			4096	// System Heap Size
#define	USB_HEAP_SIZE			16384

#define	OSC_EXTERNAL			false
#define	EXTERNAL_INPUT_MHZ		12
#define	ICLK_MULTIPLY			8
#define	PCLK_MULTIPLY			4
#define	BCLK_MULTIPLY			4

#define	INTERNAL_OSC_MHZ		54
#define	ICLK_DIVISER			1
#define	PCLK_DIVISER			2
#define	BCLK_DIVISER			2


////////////////////////////////////////////////////////////////////////////////
#endif	// _FORTEDEF_H_
