/*
 * Voice_Control.c
 *
 * Created: 2/11/2012 6:38:56 PM
 *  Author: Dubs
 */ 

#include "capi324v221.h"

void CBOT_main( void )
{
	// Open and initialize the LCD-subsystem.
	LCD_open();
	// Clear the LCD.
	LCD_clear();
	// Print a message.
	LCD_printf( "Hello, Dolly!\n" );
	// Don't leave.
	while( 1 );
} // end CBOT_main()