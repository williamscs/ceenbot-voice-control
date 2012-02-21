// Auth: Jose Santos
// Desc: Header file for the power-management module.

#ifndef __PWRMGR324V221_H__
#define __PWRMGR324V221_H__

#include "sys324v221.h"
#include "utils324v221.h"
#include "adc324v221.h"
#include "tmrsrvc324v221.h"
#include "lcd324v221.h"

// Uncomment to enable.
// #define __DEBUG_PWRMGR_UART0_MSG
#define __DEBUG_PWRMGR_UART0_CHARGE_LOG
// #define __DEBUG_PWRMGR_UART0_MSG_SHORT

#if ( defined( __DEBUG_PWRMGR_UART0_MSG ) || \
      defined( __DEBUG_PWRMGR_UART0_CHARGE_LOG ) || \
      defined( __DEBUG_PWRMGR_UART0_MSG_SHORT ) )

    #include "uart324v221.h"

#endif

#include<avr/io.h>
#include<stdlib.h>
#include<avr/pgmspace.h>

// Alternate name for 'CLKDEV_I2C'
#define CLKDEV_TWI  CLKDEV_I2C

// ============================ type declarations =========================== //
// Desc: Enumerated type declaration for specifying a 'clock device' for
//       use with 'PWRMGR_stop_clock()' function.
typedef enum PWRMGR_CLKDEV_TYPE {

    CLKDEV_ADC = 0,
    CLKDEV_USART0,
    CLKDEV_SPI,
    CLKDEV_TIMER1,
    CLKDEV_USART1,
    CLKDEV_TIMER0,
    CLKDEV_TIMER2,
    CLKDEV_I2C

} PWRMGR_CLKDEV;

// Desc: Custom enumerated type to specify the operating 'power-mode'.
typedef enum PWRMGR_MODE_TYPE {

    PWRMGR_ON_BATTERY,      // Operating on BATTERY mode (default).
    PWRMGR_ON_AC,           // Operating on AC mode ( but NOT charging ).
    PWRMGR_CHARGING         // On AC-mode, CHARGING the battery.

} PWRMGR_MODE;

// Desc: Structure used to hold internal parameter data for the power 
//       manager subsystem module.
typedef struct PWRMGR_PARAMS_TYPE {

    // Holds individual ADC code samples for averaging purposes.
    //
    // NOTE: The storage size for the number of samples (currently '4'),
    //       should also be reflected in '__pwrmgr324v221.h' file as the
    //       '__MAX_SAMPLES' macro-constant.  Make sure this value is
    //       reflected there as well in the sources.
    //
    struct {

      volatile ADC_SAMPLE battery; // Holds ADC code for battery voltage level.
      volatile ADC_SAMPLE current; // Holds ADC code for 'current' consumption.
      volatile ADC_SAMPLE charger; // Holds ADC code for charger voltage level.

    } samples[ 4 ];

    // Holds the ADC code 'average' representations.
    struct {

        volatile ADC_SAMPLE battery; // Holds 'average' ADC code for Vbatt.
        volatile ADC_SAMPLE current; // Holds 'average' ADC code for Ibatt.
        volatile ADC_SAMPLE charger; // Holds 'average' ADC code for Vchrg.

    } averages;
    
    // Holds charger-specific parameters.
    struct {

        // Length of time the charging has been taking place.
        struct {

            volatile unsigned char seconds; // Holds seconds charging.
            volatile unsigned char minutes; // Holds minutes charging.
            volatile unsigned char hours;   // Holds hours charging.

        } time;

        volatile float charge_sum;          // Holds charge accumulated in 
                                            // 'mAH' since charging started
                                            // taking place.

        volatile BOOL stabilized;           // If 'TRUE', the voltages between
                                            // the battery and charger have
                                            // reached a certain 'equilibrium'.

        volatile BOOL check_charge;         // When true, a 'charge-check'
                                            // has to be made to determine
                                            // how the charging is going
                                            // since charging started.

        volatile unsigned char check_charge_interval; // The 'check_charge...
                                            // interval' is the time (in minu-
                                            // tes) that must elapse before
                                            // we do a battery voltage check
                                            // to see how charging is going
                                            // ever since it was started.

        volatile unsigned char stabilize_time;    // Holds number of sample
                                                  // 'ticks' that must elapse
                                                  // before we consider the 
                                                  // voltages between battery
                                                  // and charger stabilized.

        volatile unsigned short int pwm_control;  // Maintains the current PWM
                                                  // value during charging.

        volatile BOOL pwm_active;           // If TRUE, the PWM  is active.

        volatile BOOL gate_active;        // Indicates whether the charge gate
                                          // pins is 'manually' being toggled.
    } charger;

    unsigned char samples_collected;      // Holds the current sample number.

    volatile PWRMGR_MODE power_mode;      // Specifies the current power mode.

    volatile BOOL charger_attached;       // Indicates charger attchment
                                          // status.

    volatile BOOL battery_attached;       // Indicates battery attachment
                                          // status.

    volatile BOOL in_user_context;        // This is TRUE if execution has moved
                                          // from initialization to USER
                                          // program execution.


} PWRMGR_PARAMS;

// ============================= public prototypes ========================== //
// TODO: (finish).
// Desc: Function acquires and initializes needed resources for the Power
//       Manager subsystem module.
extern SUBSYS_OPENSTAT PWRMGR_open( void );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: Function deallocates and releases resources used by the Power Manag-
//       ement subsystem module.
extern void PWRMGR_close( void );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: Function triggers the 'cheap-O' PWM control if the PWM is enabled.
extern void PWRMGR_clk( void );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: Function can be used to stop the clock to a specific device.  Stopping
//       the clock to unused devices reduces the power consumption.  Note,
//       however that a device and it's registers are inaccessible while the
//       peripheral's clock is disabled.  In additions any I/O resources that
//       are used (if the peripheral was ENABLED when its clock was stopped), 
//       will remain inaccessible.  Therefore, a device should first be disa-
//       bled before its clock is stopped.
extern void PWRMGR_stop_clock( PWRMGR_CLKDEV  device );
// -------------------------------------------------------------------------- //
// Desc: Function can be used to stop ALL clocks for those devices whose
//       clocks can be stopped from a power-manager standpoint.  The function
//       behaves as if multiple back-to-back calls are made to 
//       'PWRMGR_stop_clock()' function.
extern void PWRMGR_stop_all_clocks( void );
// -------------------------------------------------------------------------- //
// TODO: (finish).
// Desc: Function can be used to start the clock of a specific device.  The 
//       device will continue operation in the state it was before its clock
//       was disabled (if the device was ENABLED).  Otherwise, nothing will 
//       happen.
extern void PWRMGR_start_clock( PWRMGR_CLKDEV device );
// -------------------------------------------------------------------------- //
// Desc: Function samples levels from three sources: battery, current, and
//       charger levels.  These values are stored in the parameters structure
//       as given.  The ADC subsystem must already be opened.
extern void PWRMGR_sample_levels( void );
// -------------------------------------------------------------------------- //
// Desc: Function represents the top-level function involved in monitoring
//       voltage levels and determining the presence or absence of power
//       sources.
extern void PWRMGR_process( void );
// -------------------------------------------------------------------------- //
// Desc: Function determines present active sources based on previous
//       measurements.
extern void PWRMGR_active_sources( void );
// -------------------------------------------------------------------------- //
// Desc: Function updates the 'charging time' when charging.
extern void PWRMGR_update_charge_time( void );
// -------------------------------------------------------------------------- //
// Desc: Function keeps track of the accumulated 'charge' while charging since
//       the 'charging' began.
extern void PWRMGR_update_charge( void );
// -------------------------------------------------------------------------- //
// Desc: Function performs charging tasks when actually 'charging' -- i.e.,
//       when the 'power mode' is actively CHARGING.
extern void PWRMGR_process_charge( void );
// -------------------------------------------------------------------------- //
// Desc: Function takes care of monitoring how the charging is going and
//       the whole battery charging process in general on each 'charge
//       check event'.
extern void PWRMGR_on_charge_check_event( void );

// ========================== external declarations ========================= //

extern PWRMGR_PARAMS PWRMGR_params;
extern TIMEROBJ      monitor_timer;

#endif /* __PWRMGR324V221_H__ */
