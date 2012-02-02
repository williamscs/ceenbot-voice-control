// Auth: Jose Santos
// Desc: Header file for the ADC subsystem module.

#ifndef __ADC324V221_H__
#define __ADC324V221_H__

#include <avr/io.h>

#include "utils324v221.h"
#include "sys324v221.h"

// =============================== defines ================================== //
// ============================ type declarations =========================== //
// Custom enumerated type for specifying the ADC channel to capture.
typedef enum ADC_CHAN_TYPE {

    ADC_CHAN0 = 0,
    ADC_CHAN1,
    ADC_CHAN2,
    ADC_CHAN3,
    ADC_CHAN4,
    ADC_CHAN5,
    ADC_CHAN6,
    ADC_CHAN7,
    ADC_CHAN_VBG = 30,   // Band-gap voltage channel (1.1V).
    ADC_CHAN_GND = 31    // GND reference channel (0V).

} ADC_CHAN;

// Custom enumerated type for specifying the reference voltage for the ADC.
typedef enum ADC_VREF_TYPE {

    ADC_VREF_AREF = 0,   // 'AREF', Internal VREF turned OFF.
    ADC_VREF_AVCC,       // 'AVCC' with external cap at AREF pin.
    ADC_VREF_1P1V,       // Internal 1.10V Voltage Reference.
    ADC_VREF_2P56V       // Internal 2.56V Voltage Reference.

} ADC_VREF;

// Custom type for storing a single 10-bit sample acquired via 'ADC_sample()'
// function.
typedef unsigned short int ADC_SAMPLE;

// =============================== prototypes =============================== //
// TODO: (finish)
// Desc: Function acquires and initializes resources for using the ADC subsystem
//       module.
extern SUBSYS_OPENSTAT ADC_open( void );
// -------------------------------------------------------------------------- //
// TODO: (finish)
// Desc: Function de-allocates and releases resources in use by the ADC subsys-
//       tem module.
extern void ADC_close( void );
// -------------------------------------------------------------------------- //
// TODO: (finish)
// Desc: Function allows the user to set the input channel for ADC conversion.
extern void ADC_set_channel( ADC_CHAN which );
// -------------------------------------------------------------------------- //
// TODO: (finish)
// Desc: Function allows the user to set the Voltage Reference for the ADC.
extern void ADC_set_VREF( ADC_VREF which );
// -------------------------------------------------------------------------- //
// TODO: (finish)
// Desc: Function allows you to 'sample' (convert) the currently selected
//       channel to its digital representation.
extern ADC_SAMPLE ADC_sample( void );
// ======================== external declarations =========================== //

#endif /* __ADC324V221_H__ */
