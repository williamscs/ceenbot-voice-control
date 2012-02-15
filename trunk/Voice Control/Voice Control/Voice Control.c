/*
 * Voice_Control.c
 *
 * Created: 2/11/2012 6:38:56 PM
 *  Author: Dubs
 */ 
#define F_CPU 20000000UL
#include "capi324v221.h"

/* FUNCTION PROTOTYPES */
void goForward();
void goBackward();
void turnRight();
void turnLeft();
void turnAround();
void resumePrev( uint8_t prev_state);
void stop();
void makeSandwich();

void CBOT_main( void )
{	
	/* Local Variable Declaration */
	uint8_t cmd = 0;
	uint8_t state = 0;
	uint8_t prev_state = 0;
	
	/* Setting Up */
	LCD_open();			// Open and initialize the LCD-subsystem.
	STEPPER_open();     // Open STEPPER module for use.
	UART_open(UART_UART0);
	UART_configure(UART_UART0, UART_8DBITS, UART_1SBIT, UART_NO_PARITY, 9600);
	
	// Clear the LCD.
	LCD_clear();
	// Print a message.
	LCD_printf( "Try saying:\n\"CEENbot Go\"" );
	// Don't leave.
	
	while( 1 )
	{
		prev_state = state;
		state = cmd;
		switch (state) 
		{
			case 5: /* Command to Go Forward */
				goForward();
				break;
			case 6: /* Command to Reverse */
				goBackward();
				break;
			case 10: /* Command to turn right */
				turnRight();
				resumePrev(prev_state);
				break;
			case 11:/* Command to turn left */
				turnLeft();
				resumePrev(prev_state);
				break;
			case 15: /* Command to do a U-turn */
				turnAround();
				resumePrev(prev_state);
				break;
			case 99:
				stop();
				break;
			default: /* execute default action */
				stop();
				printf("DEFAULT STATE");
				break;
		}
		STEPPER_wait_on( STEPPER_BOTH );
	}
} // end CBOT_main()

/* Directional code
 * I made this for the sole reason of wanting to type less */
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
	STEPPER_move_stnb( STEPPER_BOTH,
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF,   // Left
		STEPPER_FWD, 150, 200, 400, STEPPER_BRK_OFF ); // Right
}

void turnRight()
{
	//TURN RIGHT (~90-degrees)...
	STEPPER_move_stnb( STEPPER_BOTH,
		STEPPER_FWD, 150, 200, 400, STEPPER_BRK_OFF,   // Left
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF ); // Right
}

void turnAround()
{
	//Just turn right twice
	turnRight();
	turnRight();
}

void resumePrev( uint8_t prev_state )
{
	if(prev_state == 5)
		goForward();
	else if(prev_state == 6)
		goBackward();
}

void stop()
{
	STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_OFF);
}

void makeSandwich()
{
	//It really doesn't make you a sandwich.
}