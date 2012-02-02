// Auth: Jose Santos
// Desc: Header file for the 'system' module.  This module provides resources
//       and access to system-wide variables.  Specifically, 'accessor 
//       functions' to system variables to other subsystem modules.

#ifndef __SYS324V221_H__
#define __SYS324V221_H__

// =========================== global declarations ========================== //
typedef enum SUBSYS_ST_TYPE 
{

    SUBSYS_CLOSED = 0,
    SUBSYS_OPEN,
    SUBSYS_IN_USE,
    SUBSYS_NOT_AVAILBLE,
    SUBSYS_ERROR,
    SUBSYS_INIT_FAILED

} SUBSYS_ST;

typedef enum SUBSYS_TYPE
{

    SUBSYS_CPU0 = 0,
    SUBSYS_CPU1,
    SUBSYS_SPI,
    SUBSYS_SPIFLASH,
    SUBSYS_PSXC,
    SUBSYS_LCD,
    SUBSYS_LED,
    SUBSYS_STEPPER,
    SUBSYS_TMRSRVC,
    SUBSYS_SPKR,
    SUBSYS_BEEP,
    SUBSYS_SWATCH,
    SUBSYS_USONIC,
    SUBSYS_UART0,
    SUBSYS_UART1,
    SUBSYS_ADC,
    SUBSYS_ISR,
    SUBSYS_TI,
    SUBSYS_I2C,
    SUBSYS_NA

} SUBSYS;

// Structure type declaration used to hold the result when an attempt to open
// a 'subsystem' fails.  A function may return a variable of this type 
// specifying what happed (subsystem state is 'closed', or 'in-use'), and what
// subsystem generated the error (the actual subsystem).
typedef struct SUBSYS_OPENSTAT_TYPE {

    SUBSYS       subsys;
    SUBSYS_ST    state;

} SUBSYS_OPENSTAT;

// Structure type declaration used to hold the state of various subsystems.
typedef struct CBOT_SYSTEM_TYPE {

    volatile SUBSYS_ST CPU0_subsys;     // ATmega324 state.
    volatile SUBSYS_ST CPU1_subsys;     // ATtiny48  state.
    volatile SUBSYS_ST SPI_subsys;      // SPI subsystem state.
    volatile SUBSYS_ST SPIFLASH_subsys; // SPIFLASH subsystem state.
    volatile SUBSYS_ST LCD_subsys;      // LCD subsystem state.
    volatile SUBSYS_ST PSXC_subsys;     // PSX controller subsys state.
    volatile SUBSYS_ST LED_subsys;      // LED subsystem state.
    volatile SUBSYS_ST STEP_subsys;     // Stepper Motor subsys state.
    volatile SUBSYS_ST TMRSRVC_subsys;  // Timer Service subsys state.
    volatile SUBSYS_ST SPKR_subsys;     // Speaker service subsys state.
    volatile SUBSYS_ST BEEP_subsys;     // Beeper service subsys state.
    volatile SUBSYS_ST SWATCH_subsys;   // Stop-watch service subsys state.
    volatile SUBSYS_ST USONIC_subsys;   // Ultrasonic service subsys state.
    volatile SUBSYS_ST UART0_subsys;    // UART0 subsystem state.
    volatile SUBSYS_ST UART1_subsys;    // UART1 subsystem state.
    volatile SUBSYS_ST ADC_subsys;      // ADC subsystem state.
    volatile SUBSYS_ST ISR_subsys;      // ISR subsystem state.
    volatile SUBSYS_ST TI_subsys;       // TI subsystem state.
    volatile SUBSYS_ST I2C_subsys;      // I2C subsystem state.

} CBOT_SYSTEM;


// =========================== prototypes =================================== //
// Input  Args: TODO
// Output Args:
// Globals  Read:
// Globals Write:
// Returns:
// Desc: Initializes the internal state of the system-wide state variables.
extern void SYS_init( void );
// -------------------------------------------------------------------------- //
// Input  Args: TODO
// Output Args:
// Globals  Read:
// Globals Write:
// Returns:
// Desc: Function used to get the subsystem state.
extern SUBSYS_ST SYS_get_state( SUBSYS which );
// -------------------------------------------------------------------------- //
// Input  Args: TODO
// Output Args:
// Globals  Read:
// Globals Write:
// Returns:
// Desc: Function used to set the subsystem state.
extern void SYS_set_state( SUBSYS which, SUBSYS_ST state );

// ======================== external globals ================================ //
extern struct CBOT_SYSTEM_TYPE CBOT_system;

#endif /* __SYS324V221_H__ */
