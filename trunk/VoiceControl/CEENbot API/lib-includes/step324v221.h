// Auth: Jose Santos
// Desc: Header file for the stepper module subsystem.  The stepper module
//       exposes functions and features for manipulating the stepper motors on
//       the CEENBoT.

#ifndef __STEP324V221_H__
#define __STEP324V221_H__

#include <avr/io.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utils324v221.h"
#include "sys324v221.h"

// ============================ defines ===================================== //
#define STEPS_PER_RVLTN     200     /* There are 200 steps/revolution */

#define STEPPER_REVS(r) ( (r) * (STEPS_PER_RVLTN))
// Desc: Use this helper macro to specify the 'step speed' in 'revolutions 
// per second' instead of 'steps per second'.  It can be used for any function 
// with a 'nStepsPerSec' argument.  'r' is given in revolutions per second 
// and it can contain a fractional decimal value as shown in the example
// below.
//
// EX:  STEPPER_set_speed( BOTH_STEPPERS, REVS_PER_SEC( 1.7 ) );
//
// EX:  STEPPER_run( LEFT_STEPPER, STEPPER_FWD, REVS_PER_SEC( 2.3 ) );
//
#define REVS_PER_SEC(r) \
    (( unsigned short int )( ( r ) * ( STEPS_PER_RVLTN ) ) )

// Desc: The following helper macro allows you to specify travel distance
//       when operating in STEP mode in units of 'feet'.
#define STEP_FEET(d) \
    (( unsigned short int )( ( d ) / ( 0.04328f ) )


// Desc: The following helper macros are 'slightly' simpler versions of the
//       'STEPPER_step()' function.  They both save an specifying two additional
//       arguments, being the 'wait mode' and the 'notifier flag' which are
//       set accordingly.
//
//       'stepwt' = 'step & wait'.
#define STEPPER_stepwt( w, d, steps, speed, brk_mode ) \
    STEPPER_step( w, d, steps, speed, brk_mode, STEPPER_WAIT, NULL )

//       'stepnb' = 'step no-block'
#define STEPPER_stepnb( w, d, steps, speed, brk_mode ) \
    STEPPER_step( w, d, steps, speed, brk_mode, STEPPER_NO_WAIT, &step_done )

// Desc: The following helper macros are 'slightly' simpler versions of
//       'STEPPER_move()' function.  They save the need to specify certain
//       arguments which are normally not needed.  For example, when 
//       'STEPPER_move()' is issued in FREERUNNING mode, the distance (steps),
//       brake mode and stepper events are 'don't care'.  We can supply these
//       values internally.
//
//       However, when operating in one of the step modes, the stepper events
//       are still supplied 'NULL'.  Therefore, the macro-functions are for
//       cases where the user doesn't use stepper events -- that way they don't
//       have to waste time always having to specify 'NULL' and instead, have
//       the macro-function supply this internally.
#define STEPPER_move_rn( which, dir_L, speed_L, accel_L,    \
                                dir_R, speed_R, accel_R   ) \
    STEPPER_move( STEPPER_FREERUNNING, which,               \
        dir_L, 0, speed_L, accel_L, STEPPER_BRK_OFF, NULL,  \
        dir_R, 0, speed_R, accel_R, STEPPER_BRK_OFF, NULL )


#define STEPPER_move_stnb( which, dir_L, steps_L, speed_L, accel_L, brkmode_L, \
                                  dir_R, steps_R, speed_R, accel_R, brkmode_R )\
    STEPPER_move( STEPPER_STEP_NO_BLOCK, which,                 \
        dir_L, steps_L, speed_L, accel_L, brkmode_L, NULL,      \
        dir_R, steps_R, speed_R, accel_R, brkmode_R, NULL )

#define STEPPER_move_stwt( which, dir_L, steps_L, speed_L, accel_L, brkmode_L, \
                                  dir_R, steps_R, speed_R, accel_R, brkmode_R )\
    STEPPER_move( STEPPER_STEP_BLOCK, which,                    \
        dir_L, steps_L, speed_L, accel_L, brkmode_L, NULL,      \
        dir_R, steps_R, speed_R, accel_R, brkmode_R, NULL )


// Desc: The following macro is used for defining 'stepper events'.
#define STEPPER_EVENT( event_name )   void event_name( void )

// Desc: Alternate translations.
#define LEFT_STEPPER    STEPPER_LEFT
#define RIGHT_STEPPER   STEPPER_RIGHT
#define BOTH_STEPPERS   STEPPER_BOTH

// ======================== type declarations =============================== //
// Type declaration for declaring function pointers for 'stepper events'.
typedef void ( *STEPPER_EVENT_PTR )( void );

// Enumerated type declaration for the stepper motor 'phase'.
typedef enum STEP_PHASE_TYPE {

    PHASE0 = 0,
    PHASE1,
    PHASE2,
    PHASE3

} STEP_PHASE;

// Enumerated type declaration to indicate the stepper motor direction.
typedef enum STEPPER_DIR_TYPE {

    STEPPER_FWD,    // Stepper moves forward.
    STEPPER_REV     // Stepper moves backward (in reverse).

} STEPPER_DIR;

// Structure declaration for holding the direction mode for each motor.
typedef struct STPPER_DIRMODE_TYPE {

    STEPPER_DIR left;   // Holds direction mode for left motor.
    STEPPER_DIR right;  // Holds direction mode for right motor.

} STEPPER_DIRMODE;

// Enumerated type declaration to indicate the current operating stepper mode.
typedef enum STEPPER_MODE_TYPE {

    STEPPER_NORMAL_MODE,
    STEPPER_STEP_MODE

} STEPPER_MODE;

// Structure type declaration for holding the operating mode for each motor.
typedef struct STEPPER_OPMODE_TYPE {

    STEPPER_MODE left;      // Operating mode for left motor.
    STEPPER_MODE right;     // Operating mode for right motor.

} STEPPER_OPMODE;

// Enumerated type declaration to indicate the current 'brake' mode.
typedef enum STEPPER_BRKMODE_TYPE {

    STEPPER_BRK_OFF,
    STEPPER_BRK_ON

} STEPPER_BRKMODE;

// Structure type declaration to hold the current 'brake mode' for each
// stepper motor.
typedef struct STEPPER_BRAKE_TYPE {

    STEPPER_BRKMODE left;   // Holds current brake mode for left motor.
    STEPPER_BRKMODE right;  // Holds current brake mode for right motor.

} STEPPER_BRAKE;

// Structure type declaraton for holding the stepper motor velocity (in 
// steps, or 'step speed') for both left and right motors collectively.
//
// FIXME:  Change type to 'unsigned short int'.  This also include the
//         corresponding DDS accumulators.
//
typedef struct STEPPER_SPEED_TYPE {

    signed short int left;        // Holds left stepper 'step speed'.
    signed short int right;       // Holds right stepper 'step speed'.

} STEPPER_SPEED;

// Structure type declaration for holding the stepper motor acceleration
// values (in steps) or 'step acceleration' for both, left and right motors.
typedef struct STEPPER_ACCEL_TYPE {

    unsigned short int left;
    unsigned short int right;

} STEPPER_ACCEL;

// Structure type declaration for holding the remaining number of steps
// each stepper motor still has to progress through when running in 'STEP'
// mode ONLY.
typedef struct STEPPER_STEPS_TYPE {

    unsigned short int left;  // Holds number of steps for left motor.
    unsigned short int right; // Holds number of steps for right motor.

} STEPPER_STEPS;

// Structure type declaration for holding the current motor phase for both
// motors.
typedef struct STEPPER_PHASE_TYPE {

    STEP_PHASE left;        // Holds the current phase for left motor.
    STEP_PHASE right;       // Holds the current phase for right motor.

} STEPPER_PHASE;

// Enumerated type declaration to indicate whether the motors are active.
typedef enum STEPPER_STATE_TYPE {

    STEPPER_STOPPED = 0,    // State to indicate 'stopped' motor (NOT running).
    STEPPER_RUNNING,        // State to indicate 'running' motor.
    STEPPER_BRAKING         // State to indicate 'braking' motor ( the brake
                            // is engaged ).

} STEPPER_STATE;

// Structure declaration to hold the 'active state' for each stepper motor.
typedef struct STEPPER_ACTSTATE_TYPE {

    STEPPER_STATE   left;   // Holds left motor active state.
    STEPPER_STATE   right;  // Holds right motor active state.

} STEPPER_ACTSTATE;

// Enumerated type declaration for stepper motor designation.
typedef enum STEPPER_ID_TYPE {

    STEPPER_LEFT = 0,           // Left motor.
    STEPPER_RIGHT,              // Right motor.
    STEPPER_BOTH                // Refers to both, left and right steppers.

} STEPPER_ID;

// Custom type declaration for flag notification when a stepper motion
// completes when in STEP mode.
typedef unsigned char STEPPER_FLAG;

// Structure declaration for holding notification flags for both the left
// and right stepper motors. When a stepper flag is set to '1' it indicates
// the respective motor has completed cycling through the step sequence when
// in STEP mode.
typedef volatile struct STEPPER_NOTIFY_TYPE {

    STEPPER_FLAG left;
    STEPPER_FLAG right;

} STEPPER_NOTIFY;

// Enumerated type declaration for specifying the WAIT mode when operating in
// STEP mode.
typedef enum STEPPER_WAITMODE_TYPE {

    STEPPER_WAIT = 0, 
    STEPPER_NO_WAIT

} STEPPER_WAITMODE;

// Enumerated type declaration for specifying the 'run mode'.
typedef enum STEPPER_RUNMODE {

    STEPPER_STEP_BLOCK = 0,
    STEPPER_STEP_NO_BLOCK,
    STEPPER_FREERUNNING

} STEPPER_RUNMODE;    

// Enumerated type declaration for specifying the stepper 'POWER MODE'.
typedef enum STEPPER_PWRMODE_TYPE {

    STEPPER_PWR_LOW,
    STEPPER_PWR_HIGH

} STEPPER_PWRMODE;

// Structure declaration for DDS accumulators.
//
// FIXME: Change type to 'unsigned int'.  We look for overflow condition, and
//        sign really doesn't play a role -- I think it will be safe to use
//        non-negative type.
//
typedef struct STEPPER_DDSACCUM_TYPE {

    signed int left;    // Left  DDS accumulator.
    signed int right;   // Right DDS accumulator.

} STEPPER_DDSACCUM;

// Enumerated type declaration to hold the status of the stepper motor
// engine to indicate if it's busy 'stepping' or not.
typedef enum STEPPER_BUSY_STATUS_TYPE {

    STEPPER_NOT_BUSY = 0,
    STEPPER_BUSY

} STEPPER_BUSY_STATUS;

// Structure type declaration for storing internal parameters.
typedef struct STEPPER_PARAMS_TYPE {

   volatile STEPPER_OPMODE  op_mode;       // Holds 'operating mode' for 
                                            // both mottors.

    volatile STEPPER_SPEED step_speed;      // Holds currently set stepper 
                                            // 'step speeds' for both motors.

    volatile STEPPER_SPEED curr_speed;      // Holds the 'current' stepper
                                            // speed for both motors ( this is
                                            // the 'real-time' speed updated
                                            // on each stepper clock! ).

    volatile STEPPER_ACCEL step_accel;      // Holds stepper motors acceleration
                                            // constants.

    volatile STEPPER_ACCEL step_decel;      // Holds stepper motors deceleration
                                            // constants.

    volatile STEPPER_STEPS nSteps;          // Holds steps left (in 'STEP' mode)
                                            // for both motors.

    volatile STEPPER_STEPS decel_begin;     // Step value at which 'decel-
                                            // eration' should begin for both
                                            // both motors when in 'STEP' mode.

    volatile STEPPER_STEPS pwm_timeout;     // Holds time out counters for
                                            // implementing the stepper PWM
                                            // for low-power mode.

    volatile STEPPER_BRAKE brake;           // Holds current 'brake mode' for 
                                            // both motors.

    volatile STEPPER_BRAKE stop_mode;       // Holds the 'stop mode' for both 
                                            // motors when operating in STEP 
                                            // mode.

    volatile STEPPER_WAITMODE wait_mode;    // Holds the 'wait mode' for both
                                            // motors when operating in STEP
                                            // mode.

    volatile STEPPER_DIRMODE dir_mode;      // Holds the 'direction mode' for 
                                            // both motors.

    volatile STEPPER_PHASE phase;           // Holds *next* 'phase' when motors
                                            // become energized.

    volatile STEPPER_ACTSTATE astate;       // Holds the current 'state' of both
                                            // motors.

    volatile STEPPER_PWRMODE power_mode;    // Holds the power mode.

    volatile STEPPER_DDSACCUM dds_speed;    // DDS accumulators (private).
    volatile STEPPER_DDSACCUM dds_accel;    // DDS accumulators (private).

    volatile STEPPER_NOTIFY     pending;       // Pending notify flags for both
                                            // motors when in STEP mode.

    STEPPER_NOTIFY           *pNotify;      // Notify flags for completion 
                                            // when in STEP mode.
                                            //
                                            // NOTE: 'STEPPER_NOTIFY' is 
                                            // already inherently 'volatile'.

    volatile STEPPER_BUSY_STATUS busy_status; // Holds the 'busy' status.  
                                     // When active this variable prevents 
                                     // stepping to occur while variable 
                                     // parameters are being modified.

} STEPPER_PARAMS;

// =========================== prototypes =================================== //
// Input  Args:  None.
// Output Args:  None.
// Globals  Read: None.
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Returns a value of type 'SUBSYS_OPENSTAT'.  See 'sys324v---.h' for
//          details regarding this type.
//
// Desc: Initializes and acquires resources used by the Stepper subsystem 
//       module.  You MUST call this function before invoking any other funct-
//       ion services exposed by this module.
extern SUBSYS_OPENSTAT STEPPER_open( void );

// -------------------------------------------------------------------------- //
// Input  Args: None.
// Output Args: None.
// Globals  Read: None.
// Globals Write: None.
//
// Returns: Nothing.
//
// Desc: Function deallocates and releases used resources.
extern void STEPPER_close( void );
// -------------------------------------------------------------------------- //
// Input  Args: None.
// Output Args: None.
// Globals  Read: 'STEPPER_params' structure.
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.
//
// Desc: This function is the 'heart' of the stepper motor subsystem.  It MUST 
//       be placed inside of a timer interrupt as it drives the rest of the 
//       Stepper subsystem.
extern void STEPPER_clk( void );

// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//
//              'mode' - Must be one of the following enumerated constants:
//              
//                          'STEPPER_NORMAL_MODE'
//                          'STEPPER_STEP_MODE'
//
// Output Args: None.
// Globals  Read: None.
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.
//
// Desc: This function allows you to set the 'operating mode' of each stepper
//       motor, whether STEP mode or NORMAL (a.k.a., 'freerunning') mode.
extern void STEPPER_set_mode( STEPPER_ID which, STEPPER_MODE mode );
// -------------------------------------------------------------------------- //
// Input  Args: 'power_mode' - Must be one of the following enumerated 
//                             constants:
//
//                                  'STEPPER_PWR_LOW'
//                                  'STEPPER_PWR_HIGH'
//
// Output Args: None.
// Globals  Read: None.
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.
//
// Desc: This function allows the user to set the 'power mode' for both
//       steppers.  The default is 'LOW POWER' mode.  However, it is possible
//       to change to a 'HIGH POWER' mode if so desired.  'LOW POWER' uses 
//       pulse-width modulation to modulate the stepper signals resulting 
//       in significant power reduction resulting in increased battery life!  
//       The drawback is that stepper motors operating in LOW POWER mode 
//       exhibit much less 'torque' than the HIGH POWER mode.
//
//       Note the power mode is applied to BOTH steppers.  There is no way
//       to set the power mode independently for each stepper motor.
extern void STEPPER_set_pwr_mode( STEPPER_PWRMODE power_mode );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//
//              'nStepsPerSec' - The step velocity in steps per second.  The
//                               maximum value is 300.  A 0 value will stop
//                               the specified motor(s), while a non-zero value
//                               will make them 'go', provided the brake is not
//                               currently engaged.  Use the 'REVS_PER_SEC()' 
//                               macro to specify this parameter in revolutions
//                               per second.
//
// Output Args: None.
// Globals  Read: '__MAX_STEP_SPEED' (private).
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.
//
// Desc: This function allows you change the 'step speed' of each motor in 
//       either mode STEP or RUN modes while the motor is running.  A NON-zero
//       speed will immediately activate the stepper motors, so in essence, 
//       this function also serves as an 'ON/OFF' function for each stepper
//       motor.
extern
    void STEPPER_set_speed( STEPPER_ID which, unsigned short int nStepsPerSec );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//
//              'accel_rate' - The acceleration rate determines how the stepper
//                             speed ramps up to the specified speed.  This 
//                             value is in steps/sec^2.  This affects both
//                             acceleration AND deceleration!  The maximum
//                             rate is 1000 steps/sec^2.  If this value is '0',
//                             then the acceleration/deceleration feature for
//                             the specified stepper will be disabled.
//
// Outpug Args: None.
// Globals  Read: '__MAX_ACCEL_RATE' (private).
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.
//
// Desc:  This function sets the acceleration rate (which presently also
//        determine the deceleration rate) for each stepper motor in units
//        of steps/sec^2.  The maximum value for 'accel_rate' is 1000.  A 
//        non-zero value will activate acceleration processing for the 
//        corresponding stepper(s), while a value of zero will deactivate
//        acceleration processing -- meaning, the stepper motor will ramp
//        from 0 to the specified speed taking effect immediately.
extern
      void STEPPER_set_accel( STEPPER_ID which, unsigned short int accel_rate );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//              
//              'dir' - Must be one of the following enumerated constants:
//
//                          'STEPPER_FWD'
//                          'STEPPER_REV'
//
// Output Args: None.
// Globals  Read: None.
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.
//
// Desc: Use this function to set the direction of the specified stepper(s).
extern void STEPPER_set_dir( STEPPER_ID which, STEPPER_DIR dir );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//              
//              'brakeMode' - Must be one of the following enumerated constants:
//                  
//                              'STEPPER_BRK_OFF'
//                              'STEPPER_BRK_ON'
//
// Output Args: None.
// Globals  Read: None.
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.
// 
// Desc: Use this function to set the stop mode.  The stop mode refers to 
//       what should the specified stepper motor(s) do when they complete a
//       motion while in STEP mode ONLY.  If 'brakeMode' is 'STEPPER_BRK_ON',
//       then when the steps are completed, the motor will immediately follow
//       by locking the wheel (brake engaged).  Otherwise, if 'STEPPER_BRK_OFF'
//       is specified, then the motor will simply de-energize (be turned off)
//       when all the steps complete.
extern 
    void STEPPER_set_stop_mode( STEPPER_ID which, STEPPER_BRKMODE brakeMode );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//              
//              'nSteps - The number of steps the specified stepper motor is
//                        to move.  The maximum value is 32767 or ~163 revo-
//                        lutions.
//
// Output Args: None.
// Globals  Read: None.
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.
//
// Desc: This function is used to set the number of steps the specified motor(s)
//       should move while in STEP mode only.
extern void STEPPER_set_steps( STEPPER_ID which, unsigned short int nSteps );
// -------------------------------------------------------------------------- //
// Input  Args: None.
// Output Args: None.
// Globals  Read: 'STEPPER_params' structure.
// Globals Write: None.
// 
// Returns: A value of type 'STEPPER_SPEED'.  This type consists of a structure
//          containing fields 'left' and 'right' specifying the current speed
//          of each stepper.
//
// Desc: Function returns the current speed of the steppers (BOTH).
//
// Example: Suppose the motors are already moving.
//
//          // Declare storage for our speed data.
//
//          STEPPER_SPEED curr_speed;
//
//          // Wait until the current speed reaches zero for BOTH motors.
//
//          do {
//          
//              curr_speed = STEPPER_get_curr_speed();
//
//          } while( ( curr_speed.left != 0 ) || ( curr_speed.right != 0 ) );
//          
extern STEPPER_SPEED STEPPER_get_curr_speed( void );
// -------------------------------------------------------------------------- //
// Input  Args: None.
// Output Args: None.
// Globals  Read: 'STEPPER_params' structure.
// Globals Write: None.
//
// Returns: A value of type 'STEPPER_STEPS'.  This type consists of a structure
//          containing fields 'left' and 'right' specifying the number of steps
//          remaining in a motion issued in STEP mode.
//
// Desc: Function returns the number remaining steps for each steppers when 
//       operating in STEP mode.
//
// Example:  Suppose the left motor is already moving.
//
//          // Declare storage for our step data.
//          
//          STEPPER_STEPS steps_remaining;
//
//          // Wait until the left stepper is moving.
//          do {
//
//              steps_remaining = STEPPER_get_nSteps();
//
//          } while( steps_remaining.left != 0 );
//
extern STEPPER_STEPS STEPPER_get_nSteps( void );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//              
//              'brakeMode' - Must be one of the following enumerated constants:
//                  
//                              'STEPPER_BRK_OFF'
//                              'STEPPER_BRK_ON'
//
// Output Args: None.
// Globals  Read: None.
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.

// Desc: This function will stop the specified stepper(s) regardless of the
//       operating mode.  The 'brakeMode' determines if you want to stop AND
//       also engage the brake, or simply stop by turning the motors off.  If
//       the designated motor(s) ARE running, then two possible scenarios occur
//       when 'STEPPER_stop()' is invoked, depending on the value given for
//       'brakeMode'.  If RUNNING, and:
//
//          a) If brakeMode = STEPPER_BRK_OFF, the specified stepper(s) will 
//             will simply STOP.  When stopped this way, you can restart
//             the motors by simply issuing another (new) 'STEPPER_run()' or 
//             'STEPPER_step()' function.
//
//          b) If brakeMode = STEPPER_BRK_ON, the specified stepper(s) will
//             HOLD with the brake ON until ONE of the following happen -- 
//             either...
//
//                  1) You release the brake and continue at the current
//                     speed by issuing a 'STEPPER_go()' command on the 
//                     motor(s) currently braking, OR...
//
//                  2) You release the brake by issuing a 'STEPPER_stop()'
//                     on the braking motor(s) but this time with the brake
//                     mode set to 'STEPPER_BRK_OFF', at which point, the 
//                     brake will disengage and the motor will simply remain
//                     stationary.
//
//                  3) You re-issue a 'new' motion command via 'STEPPER_step()'
//                     function.  NOTE: Issuing a 'STEPPER_run()' will NOT
//                     disengage currently engaged brakes until 'STEPPER_go()'
//                     is issued to 'release the brakes'.
//
extern void STEPPER_stop( STEPPER_ID which, STEPPER_BRKMODE brakeMode );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//              
// Output Args: None.
// Globals  Read: None.
// Globals Write: 'STEPPER_params' structure.
// 
// Returns: Nothing.
//
// Desc:  This function is used to dis-engage the brake in the case where the
//        brake was engaged by 'STEPPER_stop()' while the motor(s) in question
//        were running.  This re-engages the motor(s) to run where they left
//        off.  This is ONLY applicable in RUN (also called
//        free-running mode).  STEP mode motions issued via 'STEPPER_step()'
//        automatically issue a 'STEPPER_go()' to disengage brakes on each
//        motion request.
extern void STEPPER_go( STEPPER_ID which );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//              
//              'dir' - Must be one of the following enumerated constants:
//
//                          'STEPPER_FWD'
//                          'STEPPER_REV'
//
//              'nStepsPerSec' - The step velocity in steps per second.  The
//                               maximum value is 300.  A 0 value will stop
//                               the specified motor(s), while a non-zero value
//                               will make them 'go', provided the brake is not
//                               currently engaged.  Use the 'REVS_PER_SEC()' 
//                               macro to specify this parameter in revolutions
//                               per second.
//
// Output Args:  None.
// Globals  Read: None.
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.
//
// Desc: This function is used when the operating stepper motor is in RUN mode
//       ONLY!  That is, when you specify 'STEPPER_NORMAL_MODE' with the
//       'STEPPER_set_mode()' function.
//
//       IF the 'step speed' is a non-zero value it will immediately
//       engage the specified stepper(s).  If the step speed is zero, this 
//       function can be used to 'prepare' the settings of the steppers and
//       then activated by issuing a 'STEPPER_set_speed()' function to engange
//       the motors using a non-zero step speed.
extern void STEPPER_run( STEPPER_ID which, STEPPER_DIR dir, 
                                            unsigned short int nStepsPerSec );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//              
//              'dir' - Must be one of the following enumerated constants:
//
//                          'STEPPER_FWD'
//                          'STEPPER_REV'
//
//              'nSteps - The number of steps the specified stepper motor is
//                        to move.  The maximum value is 32767 or ~163 revo-
//                        lutions.
//
//              'nStepsPerSec' - The step velocity in steps per second.  The
//                               maximum value is 300.  A 0 value will stop
//                               the specified motor(s), while a non-zero value
//                               will make them 'go', provided the brake is not
//                               currently engaged.  Use the 'REVS_PER_SEC()' 
//                               macro to specify this parameter in revolutions
//                               per second.
//
//              'brakeMode' - Must be one of the following enumerated constants:
//                  
//                              'STEPPER_BRK_OFF'
//                              'STEPPER_BRK_ON'
//
//              'wait_mode' - Must be one of the following enumerated constants:
//
//                              'STEPPER_WAIT'
//                              'STEPPER_NO_WAIT'
//
//              'pNotifyFlag' - The **ADDRESS** to a variable of type 
//                              'STEPPER_NOTIFY' must be supplied to this
//                              parameter.  The 'left' and 'right' fields will
//                              be set to '1' upon completion of a motor motion.
//                              If this feature is not desired, simply set to
//                              'NULL'.
// Output Args: None.
// Globals  Read: None.
// Globals Write: 'STEPPER_params' structure.
//
// Returns: Nothing.
//
// Desc: This function is used when the operating mode of the stepper is in 
//       STEP mode ONLY!  That is, when you specify 'STEPPER_STEP_MODE' with
//       the 'STEPPER_set_mode()' function.
//
//       It allows you to instruct the specified motor(s) to
//       move only a given number of 'steps' (as opposed to  when in 
//       'free running mode').  Parameters include the number of steps that
//       the specified motor(s) should move, the step speed at which they 
//       should move, the direction in which they should move and what braking 
//       mode will be issued upon completion of the motion -- that is, whether 
//       the stepper should engage the brake upon completion, or simply 
//       de-energize (turn off) the stepper.
//
//       The 'wait_mode' allows you specify whether 'STEPPER_step()' should
//       BLOCK and BUSY-WAIT until the stepping motion is complete, or 
//       NOT BLOCK and simply issue the motion command and move on to the next
//       function that follow if any.  Specific circumnstances determined by
//       the user determine whether one or the other is preferred -- that is,
//       whether 'STEPPER_step()' should wait to complete or not.
//
//       Users should always consider to use 'STEPPER_stepnb()' (NON-BLOCKING
//       version of 'STEPPER_step()') and 'STEPPER_stepwt()' (BLOCKING version
//       of 'STEPPER_step()') instead of calling 'STEPPER_step()' directly.
//       These are macros that implement 'STEPPER_step()' in a more simplified
//       manner by issuing 'wait_mode' and 'pNotifyFlag' internally hiding the
//       details from the user.
//
//       Finally, the user should also consider using 'STEPPER_move()' for 
//       a higher level of functionality.
extern
void STEPPER_step( STEPPER_ID which, STEPPER_DIR dir, 
                   unsigned short int nSteps, unsigned short int nStepsPerSec,
                   STEPPER_BRKMODE onStopDoWhat, STEPPER_WAITMODE wait_mode,
                   STEPPER_NOTIFY *pNotifyFlag );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//              
// Output Args: None.
// Globals  Read: 'step_done' (private).
// Globals Write: None.
//
// Returns: Nothing.
//
// Desc: This function is used AFTER a stepper motion has been issued in 
//       STEP mode via 'STEPPER_stepnb()' macro-function.  It can be used after
//       motor commands have been issued using the non-blocking version macro,
//       to wait for the stepping to complete.  The following example on usage
//       illustrates the idea:
//
// Example:  Suppose I want to set the parameters for the left and right motor
//           independently.  But I want to wait immediately after these commands
//           for motion to complete BEFORE anything else happens.  Here's one
//           way to achieve that:
//
//              // Set left and right motor parameters.
//              STEPPER_stepnb( LEFT_STEPPER, STEPPER_FWD, 200, 300, 
//                                                          STEPPER_BRK_OFF );
//              STEPPER_stepnb( RIGHT_STEPPER, STEPPER_REV, 100, 100, 
//                                                          STEPPER_BRK_OFF );
//
//              // The two commands above (being NON-BLOCKING versions), take
//              // effect immedately so they work as if the two had been issued
//              // simultaneously.  So now we just WAIT on them as follows:
//              STEPPER_wait_on( BOTH_STEPPERS );
//
//              // The program will hold on the instruction above UNTIL BOTH
//              // steppers complete their run.
extern void STEPPER_wait_on( STEPPER_ID which );
// -------------------------------------------------------------------------- //
// Input  Args: 'which' - Must be one of the following enumerated constants:
//
//                          'STEPPER_LEFT'  (or 'LEFT_STEPPER')
//                          'STEPPER_RIGHT' (or 'RIGHT_STEPPER')
//                          'STEPPER_BOTH'  (or 'BOTH_STEPPERS')
//              
//              ' ... ' - Variable arguments.  Must pass either one or two
//                        arguments of type 'STEPPER_EVENT_PTR'.  These are
//                        functions declared with the macro 'STEPPER_EVENT()'
//                        If 'which = LEFT_STEPPER', or 'which = RIGHT_STEPPER',
//                        then only ONE  parameter should be passed. The event
//                        given will be performed for the specified stepper.
//                        However if 'which = BOTH_STEPPERS', then TWO param-
//                        eters MUST be passed!  The 'event' corresponding to
//                        the left stepper, and the event corresponding to the
//                        right stepper.  Your 'stepper events' should be 
//                        short and sweet.
//                        
// Output Args: None.
// Globals  Read: 'step_done' (private).
// Globals Write: None.
//
// Returns: Nothing.
//
// Desc: This function is used AFTER a stepper motion has been issued in
//       STEP mode via 'STEPPER_stepnb()' macro-function, or via 'STEPPER_step()
//       using 'STEPPER_NO_WAIT' for the 'wait_mode'.  It can be used after
//       motor commands have been issued to hold program execution from moving
//       further until motor motion completes.  In addition, if 'stepper events'
//       are supplied (not 'NULL'), those stepper events will be triggered upon
//       completion.
//
// Example:  Here's an example:
//
//           Supose at the top of your source code you declare and define the
//           following stepper events:
//          
//              STEPPER_EVENT( on_left_complete ) {
//
//                  LCD_printf( "Left motor stopped!\n" );
//
//              }
//
//              STEPPER_EVENT( on_right_complete ) {
//
//                  LCD_printf( "Right motor stopped!\n" );
//
//              }
//
//          Then perhaps in 'main()' you would do the following:
//
//              // Start steppers but set parameters independently -- this 
//              // requires the first 'STEPPER_stepnb()' to not block the
//              // one the follows -- so we do the following:
//
//              STEPPER_stepnb( LEFT_STEPPER, STEPPER_FWD, 200, 200, 
//                                                          STEPPER_BRK_OFF );
//              STEPPER_stepnb( RIGHT_STEPPER, STEPPER_REV, 100, 100,
//                                                          STEPPER_BRK_OFF );
//
//              // Wait UNTIL motions complete and invoke the appropriate
//              // stepper events upon completion of each.
//
//              STEPPER_wait_and_then( BOTH_STEPPERS, on_left_complete,
//                                                    on_right_complete );
//
//
//          After having issued the 'STEPPER_stepnb()' instructions.  They will
//          occur nearly simultaneously.  Then the program will 'busy wait' 
//          at 'STEPPER_wait_and_then()' and when each stepper motor completes
//          the motion, the appropriate stepper event will be called and 
//          invoked.
//
extern void STEPPER_wait_and_then( STEPPER_ID which, ... );
// -------------------------------------------------------------------------- //
// Input  Args: 'run_mode' - Must be one of the following enumerated constants:
// 
//                              'STEPPER_FREERUNNING'
//                              'STEPPER_STEP_BLOCK'
//                              'STEPPER_STEP_NO_BLOCK'
//
//              'which' - Must be one of the following enumerated constants:
//
//                              'STEPPER_LEFT'   (or 'LEFT_STEPPER')
//                              'STEPPER_RIGHT'  (or 'RIGHT_STEPPER')
//                              'STEPPER_BOTH'   (or 'BOTH_STEPPERS')
//
//              'dir_L/dir_R' - Must be one of the following enumerated const-
//                              ants:
//
//                              'STEPPER_FWD'
//                              'STEPPER_REV'
//              
//              'steps_L/steps_R' - Distance that each 'wheel' should travel
//                                  in units of 'steps'.  The value of this
//                                  argument is DONT-CARE when 'run_mode' is
//                                  'STEPPER_FREERUNNING'.  You can specify '0'
//                                  in that case.
//
//              'speed_L/speed_R' - Speed each stepper is to move in steps/sec.
//
//              'accel_L/accel_R' - Acceleration in steps/sec^2 for each step-
//                                  er.  A value of '0' disables acceleration.
//
//              'brkmode_L/brkmode_R ' - Applicable ONLY when 'run_mode' is 
//                                       either 'STEPPER_STEP_BLOCK' or 
//                                       'STEPPER_STEP_NO_BLOCK'.  In these 
//                                       modes, this argument can be one of the
//                                       following enumerated constants:
//
//                                          'STEPPER_BRK_OFF'
//                                          'STEPPER_BRK_ON'
//
//                                       When 'run_mode' is 
//                                       'STEPPER_FREERUNNING', this parameter
//                                       has no effect.  However, you MUST still
//                                       provide one of these constants.
//
//              'step_event_L/step_event_R' - Basically provide the name of a
//                                            'stepper event' function declared
//                                            and defined using the 
//                                            'STEPPER_EVENT()' macro.  The 
//                                            stepper event function will be in-
//                                            voked when the motion completes.
//                                            Otherwise this parameter must be
//                                            'NULL'.  The value of this para-
//                                            meter is only meaningful if the
//                                            'run_mode' is set to
//                                            'STEPPER_STEP_BLOCK'.  Otherwise,
//                                            it is a DONT-CARE and it should
//                                            be set to 'NULL'.
// Output Args: None.
// Globals  Read: None.
// Globals Write: None.
//
// Returns: Nothing.
//
// Desc: The 'STEPPER_move()' function is a high-level function that allows you
//       to issue motion instructions to each of the stepper motors using
//       independent parameters.  This function allows you to issue motion
//       commmands in both 'free-running' mode and 'step' modes without having
//       to explicitly switch the modes via 'STEPPER_set_mode()' function, so
//       it should be the first choice when it comes to issuing motion commands
//       to the stepper motors.  In addition, you can provide the function the
//       name(s) of 'stepper events' functions that can be invoked when the
//       motion completes when the 'run_mode' is set to 'STEPPER_STEP_BLOCK'.
//       Some arguments/parameters are applicable in some 'run_modes' while
//       others are not -- but even if certain arguments are not used, they are
//       all REQUIRED even if the value is 'bogus'.
extern void STEPPER_move( STEPPER_RUNMODE      run_mode,
                          STEPPER_ID           which,

                          STEPPER_DIR          dir_L,
                          unsigned short int   steps_L,
                          unsigned short int   speed_L,
                          unsigned short int   accel_L,
                          STEPPER_BRKMODE      brkmode_L,
                          STEPPER_EVENT_PTR    step_event_L,

                          STEPPER_DIR          dir_R,
                          unsigned short int   steps_R,
                          unsigned short int   speed_R,
                          unsigned short int   accel_R,
                          STEPPER_BRKMODE      brkmode_R,
                          STEPPER_EVENT_PTR    step_event_R );

// ======================= external declarations ============================ //
extern STEPPER_PARAMS STEPPER_params;
extern STEPPER_NOTIFY step_done;
extern const char Motor_L_LUT[];
extern const char Motor_R_LUT[];

#endif /* __STEP324V221_H__ */
