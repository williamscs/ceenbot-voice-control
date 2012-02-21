/*
 * Voice_Control.c
 *
 * Created: 2/11/2012 6:38:56 PM
 *  Author: Dubs
 */ 
#define F_CPU 20000000UL
#include "capi324v221.h"

/* Serial Commands */
#define FORWARD		0
#define BACKWARD	1
#define TURNRIGHT	2
#define TURNLEFT	3
#define TURNAROUND	4
#define STOP		5


/* FUNCTION PROTOTYPES */
void goForward();
void goBackward();
void turnRight();
void turnLeft();
void turnAround();
void resumePrev( uint8_t prev_state);
void stopBot();
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
	
	
	LCD_clear();		// Clear the LCD.
	LCD_printf( "Try saying:\n\"CEENbot Go\"" );// Print a message.
	
	while( 1 )
	{
		prev_state = state;
		state = cmd;
		switch (state) 
		{
			case FORWARD: /* Command to Go Forward */
				goForward();
				LCD_clear();
				printf("Forward %d", cmd);
				break;
			case BACKWARD: /* Command to Reverse */
				goBackward();
				LCD_clear();
				printf("Backward");
				break;
			case TURNRIGHT: /* Command to turn right */
				turnRight();
				LCD_clear();
				printf("Turn Right");
				resumePrev(prev_state);
				break;
			case TURNLEFT:/* Command to turn left */
				turnLeft();
				LCD_clear();
				printf("Turn Left");
				resumePrev(prev_state);
				break;
			case TURNAROUND: /* Command to do a U-turn */
				turnAround();
				LCD_clear();
				printf("Turn Around");
				resumePrev(prev_state);
				break;
			case STOP:
				stopBot();
				break;
			default: /* execute default action */
				stopBot();
				LCD_clear();
				printf("DEFAULT STATE");
				break;
		}
		if( ATTINY_get_SW_state(ATTINY_SW3))
		{
			cmd = TURNLEFT;
			LCD_clear();
			printf( "TURN LEFT");
		}
		else if( ATTINY_get_SW_state(ATTINY_SW4))
		{
			cmd = FORWARD;
			LCD_clear();
			printf( "FORWARD" );
		}
		else if( ATTINY_get_SW_state(ATTINY_SW5))
		{
			cmd = TURNRIGHT;
			LCD_clear();
			printf( "TURN RIGHT" );
		}
		++cmd;
		if(cmd > STOP)
			cmd = FORWARD;
		_delay_ms(5000);
		stopBot();
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
	if(prev_state == FORWARD)
		goForward();
	else if(prev_state == BACKWARD)
		goBackward();
	else if(prev_state == STOP )
		stopBot();
}

void stopBot()
{
	STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_OFF);
	LCD_clear();
	printf( "Should Stop" );
}

void makeSandwich()
{
	//It really doesn't make you a sandwich.
}