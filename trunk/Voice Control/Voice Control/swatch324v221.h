// Auth: Jose Santos
// Desc: Header file for 'stopwatch' services.  The stopwatch module provides
//       10us accurate timing services for measuring events to a time granu-
//       larity of 10us per tick.
//       As the name implies it is a start/stop watch.  This
//       module has been implemented primarily for determining pulse travel 
//       times of 'ping echoes' to measure distances using the Parallax Ping
//       module, but obviously it has other uses.  The module uses the 16-bit
//       timer (timer1 for the '324) while stop-watch services are in effect.
//       Timer 1 resources are relinquished upon completion of the stop-watch
//       task.

#ifndef __SWATCH324v221_H__
#define __SWATCH324v221_H__

// ================================ includes ================================ //
#include<avr/io.h>

#include "sys324v221.h"
#include "utils324v221.h"
#include "isr324v221.h"
#include "spkr324v221.h"

// ================================= defines ================================ //
// ============================== public declarations ======================= //
typedef unsigned short int SWTIME;  // Parameter for storing the 16-bit 
                                    // stopwatch value obtained from the 16-bit
                                    // timer.

// Structure type declaration for storing internal parameters for the 
// stopwatch service module.
typedef struct STOPWATCH_PARAMS_TYPE {

    volatile SWTIME sw_time;

} STOPWATCH_PARAMS;
// ================================ prototypes ============================== //
// TODO
// Desc: Function allocates and initializes resources needed by the stop-watch
//       service.
extern SUBSYS_OPENSTAT STOPWATCH_open( void );
// -------------------------------------------------------------------------- //
// TODO
// Desc: Function deallocates and releases resources used by the stop-watch
//       service.
extern void STOPWATCH_close( void );
// -------------------------------------------------------------------------- //
// TODO
// Desc: Stop watch clock function drives the 'ticking' of the stop-watch
//       subsystem.
extern void STOPWATCH_clk( void );
// -------------------------------------------------------------------------- //
// TODO
// Desc: Starts the micro-second stopwatch.  The current stop value is NOT
//       reset.  That is, if the stop-watch has already some non-zero value
//       (from a previous stop-watch event) it will simply continue counting
//       from this previous value.  use 'STOPWATCH_rest()' to reset the counter.
extern void STOPWATCH_start( void );
// -------------------------------------------------------------------------- //
// TODO
// Desc: Stops the micro-second stopwatch.  Function returns number of 'ticks'
//       occurred since the stopwatch was started.  Each tick is worth 10us.
//       Thus, since: 
//                      0 <= SWTIME <= 65535, 
//
//      accounts for a time interval of:
//
//                      0 <= SWTIME <= 655.35ms (655350us).
//
extern SWTIME STOPWATCH_stop( void );
// -------------------------------------------------------------------------- //
// TODO
// Desc:  Resets the stop-watch to zero.
extern SWTIME STOPWATCH_reset( void );
// -------------------------------------------------------------------------- //
// TODO
// Desc: Allows the user to set the initial count to a desired value.  The 
//       stop-watch (when started) will begin counting from this specified
//       value.
extern void STOPWATCH_set( SWTIME value );
// -------------------------------------------------------------------------- //
// TODO
// Desc: Allows the user to get the current value of the stop watch.  This is
//       only useful when the stop watch is NOT counting.
extern SWTIME STOPWATCH_get_ticks( void );

// ============================ external declarations ======================= //
extern STOPWATCH_PARAMS STOPWATCH_params;


#endif /* __SWATCH324v221_H__ */
