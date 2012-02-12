// Auth: Jose Santos
// Desc: Header file for the ATmega324 module (otherwise known also as 'CPU0').

#ifndef __MEGA324V221_H__
#define __MEGA324V221_H__

// ============================= includes =================================== //
#include<avr/io.h>
#include<avr/interrupt.h>

#include "utils324v221.h"
#include "sys324v221.h"

// =========================== defines ====================================== //
#define ATMEGA_NORMAL_STARTUP_WAIT  40
#define ATMEGA_SLEEP_STARTUP_WAIT   28000

// ============================ globals ===================================== //

// Custom enumeration type declaration for specifying the operating speed of
// the primary MCU.
typedef enum CLKMODE_TYPE {

    CLKMODE_SLEEP = 0,
    CLKMODE_20MHZ,
    CLKMODE_2MHZ,
    CLKMODE_1MHZ

} CLKMODE;

// ============================ prototypes ================================== //
// Desc: Function allocates and initializes resources for the primary CPU/MCU.
//       This function will clear the global interrupt flag to inhibit all 
//       interrupts during initialization.  It is up to the user to manually
//       call 'sei()' at the apporiate time to re-enable the interrupts.
extern SUBSYS_OPENSTAT ATMEGA_open( void );
// -------------------------------------------------------------------------- //
// Desc: Function deallocates and releases resources used by the primary 
//       CPU/MCU.
extern void ATMEGA_close( void );
// -------------------------------------------------------------------------- //
extern void ATMEGA_set_clk_mode( CLKMODE clk_mode );
// -------------------------------------------------------------------------- //

#endif /* __MEGA324V221_H__ */
