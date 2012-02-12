// Auth: Jose Santos
// Desc: Header file for the 'led' services module.

#ifndef __LED324V221_H__
#define __LED324V221_H__

#define __LED324MODULE__

#include<avr/io.h>

#include "sys324v221.h"
#include "utils324v221.h"

// ============================== defines =================================== //
// Macro-constants used with 'LED_state()' and 'LED_toggle()' macros (below).
#define LED0         5
#define LED1         6
#define LED_Red      LED0
#define LED_Green    LED1

#define LED_ON      SET
#define LED_OFF     CLEAR

// Helper macros for single-bit LED handling.
#define LED_set( which )        LED_set_pattern( ( 1 << ( which ) ) )
#define LED_clr( which )        LED_clr_pattern( ( 1 << ( which ) ) )
#define LED_toggle( which )     LED_tog_pattern( ( 1 << ( which ) ) )
#define LED_state( which, state ) \
    ( state ? ( LED_set( ( which ) ) ) : ( LED_clr( ( which ) ) ) )


// ============================ prototypes ================================== //
// Input  Args: TODO
// Output Args:
// Globals  Read:
// Globals Write:
// Returns:
//
// Desc: Function initializes the hardware ports for manipulating the LEDs.
extern SUBSYS_OPENSTAT LED_open( void );
// -------------------------------------------------------------------------- //
// Input  Args: TODO
// Output Args:
// Globals  Read:
// Globals Write:
// Returns:
//
// Desc: Call this function if LED resources are no longer needed.  This opens
//       up port pins for other use.
extern void LED_close( void );
// -------------------------------------------------------------------------- //
// Input  Args: 'LED_pattern' - A '1' for a corresponding bit turns the LED
//                              on, while a '0' does nothing (i.e., it leaves
//                              the LED in the current state).  The LED
//                              assigned bit patterns are as follows:
//
//                                  bit5: Red   LED on PD5.
//                                  bit6: Green LED on PD6.
//
//                                  All others: Not-assigned (future expansion).
// Output Args: None.                                  
// Globals  Read: None.
// Globals Write: None.
// Returns: Nothing.
//
// Desc: User function to set LED pattern.
extern void LED_set_pattern( unsigned char LED_pattern );
// -------------------------------------------------------------------------- //
// Input  Args: 'LED_pattern' - A '1' for a corresponding bit turns the LED
//                              OFF, while a '0' does nothing (i.e., it leaves
//                              the LED in the current state).  The LED
//                              assigned bit patterns are as follows:
//
//                                  bit5: Red   LED on PD5.
//                                  bit6: Green LED on PD6.
//
//                                  All others: Not-assigned (future expansion).
// Output Args: None.                                  
// Globals  Read: None.
// Globals Write: None.
// Returns: Nothing.
//
// Desc: User function to clear the LED pattern.
extern void LED_clr_pattern( unsigned char LED_pattern );
// -------------------------------------------------------------------------- //
// Input  Args: 'LED_pattern' - A '1' for a corresponding bit will toggle the
//                              current state of the LED from ON to OFF, or
//                              from OFF to ON, etc., while a '0' does nothing 
//                              (i.e., it leaves the LED in the current state).
//                              The LED assigned bit patterns are as follows:
//
//                                  bit5: Red   LED on PD5.
//                                  bit6: Green LED on PD6.
//
//                                  All others: Not-assigned (future expansion).
// Output Args: None.                                  
// Globals  Read: None.
// Globals Write: None.
// Returns: Nothing.
//
// Desc: User function to clear the LED pattern.
extern void LED_tog_pattern( unsigned char LED_pattern );
// -------------------------------------------------------------------------- //

#endif /* __LED324V221_H__ */
