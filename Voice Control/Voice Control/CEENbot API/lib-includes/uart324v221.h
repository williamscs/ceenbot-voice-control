// Auth: Jose Santos
// Desc: Header file for the UART subsystem module.

#ifndef __UART324V221_H__
#define __UART324V221_H__

#include <stdio.h>
#include <stdarg.h>
#include <avr/io.h>
#include "sys324v221.h"
#include "utils324v221.h"
#include "tmrsrvc324v221.h"

// ============================== defines =================================== //
#define  UART_TIMEOUT       TIMER16

#define  UART_MAX_TIMEOUT   30  /* Specify maximum timeout in seconds. */

// Helper macros that target a specific UART device.
#ifndef __NO_UART_PRINTF

    #define UART0_printf( ... )     UART_printf( UART_UART0,   __VA_ARGS__  )
    #define UART0_transmit( ... )   UART_transmit( UART_UART0, __VA_ARGS__ )
    #define UART0_receive( ... )    UART_receive( UART_UART0,  __VA_ARGS__  )
    #define UART0_has_data()        UART_has_data( UART_UART0 )

    #define UART1_printf( ... )     UART_printf( UART_UART1,   __VA_ARGS__  )
    #define UART1_transmit( ... )   UART_transmit( UART_UART1, __VA_ARGS__ )
    #define UART1_receive( ... )    UART_receive( UART_UART1,  __VA_ARGS__  )
    #define UART1_has_data()        UART_has_data( UART_UART1 )

    #define UART0_printf_PGM( ... ) UART_printf_PGM( UART_UART0, __VA_ARGS__ )
    #define UART1_printf_PGM( ... ) UART_printf_PGM( UART_UART1, __VA_ARGS__ )

#else

    #define UART0_printf( ... )
    #define UART0_transmit( ... )
    #define UART0_receive( ... )
    #define UART0_has_data()

    #define UART1_printf( ... )
    #define UART1_transmit( ... )
    #define UART1_receive( ... )
    #define UART1_has_data()

    #define UART0_printf_PGM( ... )
    #define UART1_printf_PGM( ... )
    
#endif /* __NO_UART_PRINTF */

// ========================== type declarations ============================= //
// Enumerated type declaration to specify which MCU UART device to open up
// for use.
typedef enum UART_ID_TYPE {

    UART_UART0,
    UART_UART1

} UART_ID;

// Enumerated type declaration for specifying whether a UART is ENABLED or
// DISABLED.
typedef enum UART_STATE_TYPE {

    UART_DISABLE,
    UART_ENABLE

} UART_STATE;

// Type declaration for specifying the baud-rate.
typedef unsigned long int UART_BAUD;

// Enumerated type declaration for specifying the number of data-bits 
// (character size)
// for UART transmission/reception.
typedef enum UART_DBITS_TYPE {

    UART_5DBITS = 0,
    UART_6DBITS,
    UART_7DBITS,
    UART_8DBITS
    // UART_9BITS // NOTE: Presently this mode is NOT supported.

} UART_DBITS;

// Enumerated type declaration for specifying the number of stop bits
// to use for UART communications.
typedef enum UART_SBITS_TYPE {

    UART_1SBIT  = 0,
    UART_2SBITS

} UART_SBITS;

// Enumerated type declaration for specifying the parity mode for UART
// communication.
typedef enum UART_PARITY_TYPE {

    UART_NO_PARITY = 0,
    UART_EVEN_PARITY = 2,
    UART_ODD_PARITY  = 3

} UART_PARITY;

// Enumerated type declarationfor specifying the status of a 
// transmit or receive sequence.
typedef enum UART_COMM_RESULT_TYPE {

    UART_COMM_OK,       // Communication sequence OK.
    UART_COMM_ERROR,    // Unknown communication ERROR.
    UART_COMM_TX_FULL,  // Transmitter is not ready to accept new data.
    UART_COMM_TIMEOUT   // Communication timeout while either sending or
                        // receiving.

} UART_COMM_RESULT;

// Structure type declaration which holds internal state and other info
// for each of the UART devices.
typedef struct UART_PARAMS_TYPE {

    // Parameters applicable to UART0 only.
    struct {

        BOOL TX_enabled;
        BOOL RX_enabled;
        BOOL configured;
        
        UART_TIMEOUT timeout_sec;

    } uart0;

    // Parameters applicable to UART1 only.
    struct {

        BOOL TX_enabled;
        BOOL RX_enabled;
        BOOL configured;

        UART_TIMEOUT timeout_sec;

    } uart1;

} UART_PARAMS;

// ============================= prototypes ================================= //
// TODO: (finish).
// Desc: Function opens and initializes the specified UART subsystem module.
extern SUBSYS_OPENSTAT UART_open( UART_ID which );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// DESC: Function closes and releases the specified UART subsystem module.
extern void UART_close( UART_ID which );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: Function enables/disables the specified UART transmitter.  When 
//       the UART is ENABLED, the corresponding I/O communication pins are 
//       overriden and taken up by the UART device.
extern void UART_set_TX_state( UART_ID which, UART_STATE uart_state );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: Function enables/disables the specified UART receiver.  When 
//       the UART is ENABLED, the corresponding I/O communication pins are
//       overriden and taken up by the UART device.
extern void UART_set_RX_state( UART_ID which, UART_STATE uart_state );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Specifies which UART device.  Must be one of the 
//                        following enumerated constants:
//
//                          'UART_UART0' - UART0 Device.
//                          'UART_UART1' - UART1 Device.
//
//              'data_bits' - Specifies the 'character' size to be transmitted.
//                            It must be one of the following enumerated const-
//                            ants:
//
//                              'UART_5DBITS' - 5-Bits character size.
//                              'UART_6DBITS' - 6-Bits character size.
//                              'UART_7DBITS' - 7-Bits character size.
//                              'UART_8DBITS' - 8-Bits character size.
//
//              'stop_bits' - Specifies the number of 'stop bits' to use.  It
//                            must be one of the following enumerated constants:
//
//                              'UART_1SBIT'  - 1 Stop bit.
//                              'UART_2SBITS' - 2 Stop bits.
//
//              'parity' - Specifies the parity mode to use.  It must be one of
//                         the following enumerated constants:
//
//                              'UART_NO_PARITY'  - No Parity.
//                              'UART_EVEN_PARITY'- Use EVEN parity.
//                              'UART_ODD_PARITY' - Use ODD parity.
//
//              'baud_rate' - The baud rate in bits-per-second (bps).  The
//                            maximum baud rate is determined by the UART
//                            device.  The maximum theoretical speed is
//                            approximately 1.2Mbps.
//
// Output Args: None.
//
// Globals  Read: TODO: (finish).
// Globals Write: TODO: (finish).
//
// Returns: Nothing.
//
// Desc: Function *MUST* be called after 'UART_open()' to configure the
//       UART device with the appropriate settings.
extern void UART_configure( UART_ID     which,
                            UART_DBITS  data_bits,
                            UART_SBITS  stop_bits,
                            UART_PARITY parity,
                            UART_BAUD   baud_rate );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: This function sets the 'time-out' that functions that read data from
//       the UART will wait for data reception.  After the timeout time
//       has elapsed, waiting for incoming data will be canceled and the
//       user will be notified of a time-out error.  Note that the timeout
//       is specified in units of 'seconds'.  The maximum timeout is 30.
extern void UART_set_timeout( UART_ID which, UART_TIMEOUT timeout_sec );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: This function allows the user to transmit a single byte of data
//       via the specified UART device.
extern UART_COMM_RESULT UART_transmit( UART_ID which, unsigned char data );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: This function allows the user to receive a single byte of data
//       via the specified UART device.  The received data will be placed
//       in a destination variable whose **address of** is passed as an
//       argument to 'pDest'.
extern UART_COMM_RESULT UART_receive( UART_ID which, unsigned char *pDest );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: Use this function to determine if there is data to be read which has
//       been received.
extern BOOL UART_has_data( UART_ID which );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: The name says it all... 'printf()' facility via UART.  Useful when
//       working with a 'terminal' such as 'Br@y'.
extern void UART_printf( UART_ID which, const char *str_fmt, ... );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: This function works the same way as 'UART_printf()' except string
//       data is placed in FLASH instead of SRAM since strings eat up the
//       SRAM VERY QUICKLY... YIKES!  When invoking this function, or the
//       macro versions, you MUST use the 'PSTR()' macro for EVERY string
//       you intend to print via this function -- otherwise, you will not
//       reak in the benefit of storing the string data in program memory.
//
//       For example:
//
//              UART_printf_PGM( UART_UART0, PSTR( "Hello World!" ) );
//
//       Here the 'PSTR()' macro ensures the string gets placed in program mem-
//       mory and not SRAM.
//
//       Here's another example:
//
//              unsigned int val = 22;
//
//              UART_printf_PGM( UART_UART0, PSTR( "Value is = %d.\n" ), val );
//
//       Again, note usage of the 'PSTR()' macro -- it is REQUIRED!
extern void UART_printf_PGM( UART_ID which, const char *str_fmt, ... );
// ========================== external declarations ========================= //
extern UART_PARAMS UART_params;
extern TIMEROBJ    UART0_timeout_timer;
extern TIMEROBJ    UART1_timeout_timer;
extern FILE UART0_stdout;
extern FILE UART1_stdout;

#endif /* __UART324V221_H__ */
