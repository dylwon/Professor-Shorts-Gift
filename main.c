//***************************************************************************
//
// File Name : main.c
// Title : Professor Short's Gift
// Date : 4/19/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong
//
// This program interfaces with two DOG LCD displays using the SPI protocol.
// Both of the LCD displays will be write-only slaves, and will have to be
// selected one at a time for the SPI protocol to work with multi-slave
// SPI communication. Both LCDs must be directly side by side, with LCD0 on 
// the left and LCD1 on the right. 
//
// The SPI pins for the LCD display are listed as follows:
// MOSI -> PA4 
// MISO -> PA5
// SCK -> PA6 
// /SS0 -> PB0
// /SS1 -> PB1
// RS0 -> PC0
// RS1 -> PC1
//
// This program writes both LCD displays in 3 different stages:
// 
// [First Stage]
// The first and second LCD displays work in conjunction to display
// the full thank you message with down scroll
//
// [Second Stage]
// The first display shows the first name right-justified while the second
// display shows the last name left-justified. These names should also scroll down.
//
// [Third Stage]
// Change the font of the display so that a big "THANK YOU!" displays on both displays
// with left scroll
//
// Warnings :
// Restrictions : The column size of the display buffers must not exceed 16 displayable characters
// Algorithms : none
// References :
//
// Revision History : Initial version
//
//
//**************************************************************************
#include <avr/interrupt.h>		

#include "messages.h"																																
#include "DOGM163WA.h"
#include "functions.h"

int main(void) {
	//_delay_ms(500);
	init_lcd_dog();							// Configures LCDs
	
	PORTB.DIRCLR |= PIN2_bm;				// Configures PB2 (On-board active low pushbutton) as an input
	PORTB.PIN2CTRL |= PIN0_bm | PIN1_bm;	// Enables Interrupt on falling edge 
	PORTB.INTFLAGS |= PIN2_bm;				// Clears the Interrupt flag on PB2
	
	insert_split_msg(message);
	insert_split_names(names);
	insert_split_msg(special_thanks);
	
	still_display();
	
	sei();									// Enables global interrupts
	
	while (1) {
		still_display();
		asm volatile ("nop");
	}
	
}

ISR (PORTB_PORT_vect) {
	cli();									// Disables global interrupts

	down_scroll_display();
		
	PORTB.INTFLAGS |= PIN2_bm;				// Clears the Interrupt flag
	sei();									// Re-enables global interrupts
}


