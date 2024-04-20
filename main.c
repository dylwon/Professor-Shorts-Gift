//***************************************************************************
//
// File Name : Tmain.c
// Title : Thank You Professor Short!
// Date : 4/19/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong
//
// This program interfaces with two DOG LCD displays using the SPI protocol.
// Both of the LCD displays will be write-only slaves, and will have to be
// selected one at a time for the SPI protocol to work with multi-slave
// SPI communication.
//
// The SPI pins for the LCD display are listed as follows:
// MOSI -> PA4 
// SCK -> PA6 
// /SS -> PA7
// MISO -> PA5
// RS -> PC0
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
// Restrictions : none
// Algorithms : none
// References :
//
// Revision History : Initial version
//
//
//**************************************************************************
#include <avr/io.h>
#include "DOGM163WA_driver.h"

//extern char names[][33];
//extern char message[];
//extern char lcd0_buff[10][17];
//extern char lcd1_buff[10][17];

int main(void) {
	insert_split_names(names);
}


