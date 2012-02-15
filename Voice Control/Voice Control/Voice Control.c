/*
 * Voice_Control.c
 *
 * Created: 2/11/2012 6:38:56 PM
 *  Author: Dubs
 */ 

#include "capi324v221.h"

/* FUNCTION PROTOTYPES */
void goForward();
void goBackward();
void turnRight();
void turnLeft();

void CBOT_main( void )
{	
	uint8_t cmd = 0;             // Store a single byte
	uint8_t state = 0;
	uint8_t prev_state = 0;
	
	/* Setting Up */
	// Open and initialize the LCD-subsystem.
	LCD_open();
	STEPPER_open();     // Open STEPPER module for use.
	UART_open(UART_UART0);
	UART_configure(UART_UART0, UART_8DBITS, UART_1SBIT, UART_NO_PARITY, 9600);
	
	// Clear the LCD.
	LCD_clear();
	// Print a message.
	LCD_printf( "Hello, Dolly!\n" );
	// Don't leave.
	
	while( 1 )
	{
		prev_state = state;
		state = cmd;
		switch (state) {
        case 5: /* execute unit of code 1 */
			
            break;
        case 10: /* Command to turn right */
		 
            break;
        default: /* execute default action */
            break;
    }
		
	}
} // end CBOT_main()

void goForward()
{
	STEPPER_run( STEPPER_BOTH, STEPPER_FWD, 150 );
}
void goBackward()
{
	STEPPER_run( STEPPER_BOTH, STEPPER_REV, 150 );
}
void turnLeft()
{
	//TURN LEFT (~90-degrees)...
	STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF,   // Left
		STEPPER_FWD, 150, 200, 400, STEPPER_BRK_OFF ); // Right
}
void turnRight()
{
	//TURN RIGHT (~90-degrees)...
	STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_FWD, 150, 200, 400, STEPPER_BRK_OFF,   // Left
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF ); // Right
}