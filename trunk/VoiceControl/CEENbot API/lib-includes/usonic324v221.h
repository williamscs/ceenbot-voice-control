// Auth: Jose Santos
// Desc: Header file for using the 'Ultrasonic Module'.

#ifndef __USONIC324V221_H__
#define __USONIC324V221_H__

#include "sys324v221.h"
#include "utils324v221.h"
#include "swatch324v221.h"
#include "tmrsrvc324v221.h"
#include "isr324v221.h"
#include "tiny324v221.h"

// ========================== type declarations ============================= //
// ============================ prototypes ================================== //
// TODO
// Desc: Fuction initializes and acquires resources to use the onboard ultra-
//       sonic module (when attached).
extern SUBSYS_OPENSTAT USONIC_open( void );
// -------------------------------------------------------------------------- //
// TODO
// Desc: Function releases resources acquired for use by the Ultrasonic module.
extern void USONIC_close( void );
// -------------------------------------------------------------------------- //
// TODO
// Desc: Function triggers the attached ultrasonic sensor to emit a 'ping'.  The
//       function returns the number 'ticks' it took the 'ping' sound wave to
//       echo back after it is emitted by the ultrasonic sensor.  That is the
//       number of ticks measures the ROUND TRIP of this sound wave.
//       The value of each 'tick' depends on the granularity of the stopwatch 
//       service.  At the time this function is being written, each stopwatch
//       'tick' corresponds to 10us/tick.  So a value of, say 257 corresponds
//       a sound round-trip travel time of 257*10us = 2.57ms.  This value can
//       then be used to determine distance to object/feature.
//
//       The function will 'busy-waits' in a series of loops until the pulse
//       returns from the echo.  As a result, this function will _block_ until
//       the echo returns.  If no device is attached when a 'ping' is issued,
//       the program will freeze because it is waiting for an 'echo' that will
//       never return, because it never left.
extern SWTIME USONIC_ping( void );

// ========================= external declarations ========================== //

// Nothing yet.

#endif /* __USONIC324V221_H__ */
