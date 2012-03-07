/*
 * Voice_Control.c
 *
 * Created: 2/11/2012 6:38:56 PM
 *  Author: Dubs
 */ 
#define F_CPU 20000000UL
#include "capi324v221.h"
#include<avr/interrupt.h>


/* UART calcs */
#define BAUD 9600UL
#define MYUBRR (F_CPU/(16*BAUD))-1 

/* Serial Commands */
#define BACKWARD	1
#define FORWARD		2
#define TURNRIGHT	3
#define TURNLEFT	4
#define TURNAROUND	5
#define STOP		0


/* FUNCTION PROTOTYPES */
void goForward();
void goBackward();
void turnRight();
void turnLeft();
void turnAround();
void resumePrev( uint8_t prev_state);
void stop();
void makeSandwich();
void USART_Init( unsigned int ubrr);

/* Global Variables */
volatile char cmd;
volatile uint8_t rxflag;

void CBOT_main( void )
{	
	/* Local Variable Declaration */
	uint8_t state = 0;
	uint8_t prev_state = 0;
	
	/* Setting Up */
	LCD_open();			// Open and initialize the LCD-subsystem.
	STEPPER_open();     // Open STEPPER module for use.
	USART_Init(MYUBRR);
	
	LCD_clear();		// Clear the LCD.
	LCD_printf( "Try saying:\n\"CEENbot Go\"" );// Print a message.
	
	while( 1 )
	{
		if (rxflag)
		{
			prev_state = state;
			state = cmd;
			rxflag = 0;
		}			
		switch (state) 
		{
			case FORWARD: /* Command to Go Forward */
				goForward();
				LCD_clear();
				printf("Forward");
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
				state = prev_state;
				break;
			case TURNLEFT:/* Command to turn left */
				turnLeft();
				LCD_clear();
				printf("Turn Left");
				resumePrev(prev_state);
				state = prev_state;
				break;
			case TURNAROUND: /* Command to do a U-turn */
				turnAround();
				LCD_clear();
				printf("Turn Around");
				resumePrev(prev_state);
				state = prev_state;
				break;
			case STOP:
				stop();
				LCD_clear();
				printf("Stop");
				break;
			default: /* execute default action */
				stop();
				LCD_clear();
				printf("DEFAULT STATE");
				break;
		}
		//STEPPER_wait_on( STEPPER_BOTH );
		//_delay_ms(5000);
		//cmd++;
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

void turnAround()
{
	//TURN RIGHT (~180-degrees)...
	STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_FWD, 300, 200, 400, STEPPER_BRK_OFF,   // Left
		STEPPER_REV, 300, 200, 400, STEPPER_BRK_OFF ); // Right
}

void resumePrev( uint8_t prev_state )
{
	if(prev_state == FORWARD)
		goForward();
	else if(prev_state == BACKWARD)
		goBackward();
	else if(prev_state == STOP )
		stop();
}

void stop()
{
	STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_OFF);
}

void makeSandwich()
{
	//It really doesn't make you a sandwich.
}

/* USART Functions */
void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;
    
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);      // Enable receiver and transmitter and interrupt receive
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);    // Set frame: 8data, 1 stp
}

ISR(USART0_RX_vect)
{
   cmd = UDR0; // Fetch the received byte value into the variable "ByteReceived"
   rxflag = 1;
}