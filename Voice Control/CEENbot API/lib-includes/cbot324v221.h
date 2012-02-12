// Auth: Jose Santos
// Desc: Header for the CEENBoT (TM) module.

#ifndef __CBOT324V221_H__
#define __CBOT324V221_H__

// ============================ includes ==================================== //
#include<avr/io.h>
#include<avr/interrupt.h>

#include "capi324v221.h"

// ============================= prototypes ================================= //
// TODO
// Desc: This function is the user's version of 'main()'.  This is where all
//       of the user's code will reside in, and thus it MUST be defined by the
//       user.  The standard 'main()' function is used internally by the API
//       to initialize necessary subsystems.  Note that although this PROTOTYPE
//       is designated as 'private', it's definition is made publicly.
extern void CBOT_main( void );
// -------------------------------------------------------------------------- //
// TODO
// Desc: Initializes the CEENBoT's API.
extern void CBOT_init( void );
// -------------------------------------------------------------------------- //
// Desc: This function is needed here to force the linker to bring in the 
//       'CBOT_isr' object module to force undefined references to interrupt
//       service routines to be resolved.

extern void CBOT_ISR_init( void );

#endif /* __CBOT324V221_H__ */
