//***************************************************************************
//
// File Name : DOGM163WA driver.h
// Title :
// Date : 4/14/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong & Baron Mai
//
// This program configures SPI communication between the AVR128DB48 and the
// Display on Glass LCD screen (DOG LCD).
// The SPI pins for the LCD display are listed as follows:
// MOSI -> PA4
// MISO -> PA5
// SCK -> PA6
// /SS0 -> PB0
// /SS1 -> PB1
// RS0 -> PC0
// RS1 -> PC1
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

#ifndef DOGM163WA_DRIVER_H_
#define DOGM163WA_DRIVER_H_
#include "messages.h"
#define F_CPU 4000000LU
#define LINES 100
#define MAX_SIZE 17
#define SCROLLSPEED 1000
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>

extern char *names[33];

char lcd0_buff[LINES][MAX_SIZE]; 
char lcd1_buff[LINES][MAX_SIZE];

static int lcd0_row = 0, lcd1_row = 0;

//***************************************************************************
//
// Function Name : void lcd_spi_transmit_CMD (uint8_t LCD, unsigned char cmd)
// Date : 3/29/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong & Kenneth Short
// 
// This function transmits a command character to the specified DOG LCD. The steps are shown below:
// 1) Select the device by pulling the /SS0 or /SS1 line low
// 2) Pull the RS0 or RS1 line to a 0 for the DOG LCD to interpret the serial byte packet as a command
// 3) Transmit the serial byte by placing cmd into the SPI data register
// 4) Wait until the data transmission is complete by polling for the IF flag
// 5) De-select the device by pulling the /SS0 or /SS1 line high
//
// Warnings : none
// Restrictions : none
// Algorithms : none
// References : none
//
// Revision History : Initial version
//
//**************************************************************************
 
void lcd_spi_transmit_CMD (uint8_t LCD, unsigned char cmd) {
	if (!LCD) {
		VPORTB.OUT |= PIN1_bm; // /SS1 = 1 to de-select LCD1
		VPORTB.OUT &= ~PIN0_bm; // /SS0 = 0 to select LCD0
		VPORTC.OUT &= ~PIN0_bm; // RS0 = 0 for command
	}
	else {
		VPORTB.OUT |= PIN0_bm;  // /SS0 = 1 to de-select LCD0
		VPORTB.OUT &= ~PIN1_bm; // /SS1 = 0 to select LCD1
		VPORTC.OUT &= ~PIN1_bm; // RS1 = 0 for command
	}
	
	SPI0.DATA = cmd;		//send command
	while(!(SPI0.INTFLAGS & SPI_IF_bm)) {}    // Wait until IF flag is set
		
	VPORTB.OUT |= PIN0_bm | PIN1_bm; // /SS0 = 1 and /SS1 = 1 to de-select LCD0 and LCD1
}

//***************************************************************************
//
// Function Name : void lcd_spi_transmit_CMD (uint8_t LCD, unsigned char cmd)
// Date : 3/29/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong & Kenneth Short
//
// This function transmits a data byte to the specified DOG LCD. The steps are shown below:
// 1) Select the device by pulling the /SS0 or /SS1 line low
// 2) Pull the RS0 or RS1 line to a 0 for the DOG LCD to interpret the serial byte packet as a data
// 3) Transmit the serial byte by placing cmd into the SPI data register
// 4) Wait until the data transmission is complete by polling for the IF flag
// 5) De-select the device by pulling the /SS0 or /SS1 line high
//
// Warnings : none
// Restrictions : none
// Algorithms : none
// References : none
//
// Revision History : Initial version
//
//**************************************************************************

void lcd_spi_transmit_DATA (uint8_t LCD, unsigned char cmd) {
	if (!LCD) {
		VPORTB.OUT |= PIN1_bm; // /SS1 = 1 to de-select LCD1
		VPORTB.OUT &= ~PIN0_bm;	// /SS0 = 0 to select LCD0
		VPORTC.OUT |= PIN0_bm; // RS0 = 1 for data
	}
	else {
		VPORTB.OUT |= PIN0_bm; // /SS0 = 1 to de-select LCD0
		VPORTB.OUT &= ~PIN1_bm; // /SS1 = 0 to select LCD1
		VPORTC.OUT |= PIN1_bm; // RS1 = 1 for data
	}
	
	SPI0.DATA = cmd;		//send command
	while(!(SPI0.INTFLAGS & SPI_IF_bm)) {}    // Wait until IF flag is set
	
	VPORTB.OUT |= PIN0_bm | PIN1_bm; // /SS0 = 1 and /SS1 = 1 to de-select LCD0 and LCD1
}

//***************************************************************************
//
// Function Name : void init_spi_lcd (void)
// Date : 3/29/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong & Kenneth Short
//
// This function properly sets up the registers in the SPI module to enable serial communication
// between the AVR128DB48 and the DOG LCD. The steps for configuration are listed below:
// 1) Set up pin directions for MOSI, MISO, SCK, /SS0 /SS1, and RS0 RS1 pins
// 2) Sets AVR128DB48 as master, and enables SPI protocol
// 3) Enables SPI mode 3 (CPOL = 1, CPHA = 1) and sets data order to send MSB first
// 4) Pulls the /SS line to high to de-select the other peripherals, and initialize RS0 and RS1 to 0 to send commands
//
// Warnings : Ensure there's proper configuration of registers
// Restrictions : none
// Algorithms : none
// References : none
//
// Revision History : Initial version
//
//**************************************************************************

void init_spi_lcd (void) {
	// Generic clock generator 0, enabled at reset @ 4MHz, is used for peripheral clock
	
	// Pin Direction Configurations & Initializations for both LCDs
	VPORTA.DIR |= PIN4_bm | PIN6_bm; // PA4 is output for MOSI, PA5 is input for MISO, PA6 is output for SCK
	VPORTB.DIR |= PIN0_bm | PIN1_bm; // PB0 is output for /SS0 (SS for LCD0), PB1 is output for /SS1 (SS for LCD1)
	VPORTB.OUT |= PIN0_bm | PIN1_bm; // Idles /SS0 and /SS1 as high to de-select LCDs
	VPORTC.DIR |= PIN0_bm | PIN1_bm; // PC0 is output for RS0 of LCD0, PC1 is output for RS1 of LCD1
	
	// SPI Configuration
	SPI0.CTRLA |= SPI_MASTER_bm | SPI_ENABLE_bm; // Sets AVR128DB48 as master, and enables SPI protocol
	SPI0.CTRLB |= SPI_SSD_bm | SPI_MODE_3_gc; // Enables SPI mode 3 (CPOL = 1, CPHA = 1) and Data order sends MSB first

	VPORTC.OUT &= ~(PIN0_bm | PIN1_bm);	// RS0 = 0 and RS1 = 0 for command sends
}

//***************************************************************************
//
// Function Name : void init_lcd_dog(void)
// Date : 3/29/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong & Kenneth Short
//
// This function sends serial bytes to both of the DOG LCD to configure its settings.
// Configuring the functionality of the display requires sending well timed
// serial packets in the correct order and with the right delays.
//
// Warnings : Ensure serial packets for initialization are sent in the right order, 
//			  and proper delays are used in between the sent packets.
// Restrictions : none
// Algorithms : lcd_spi_transmit_CMD
// References : none
//
// Revision History : Initial version
//
//**************************************************************************

void init_lcd_dog (void) {
	init_spi_lcd();		//Initialize MCU for SPI with both LCD displays
	
	for (uint8_t i = 0; i < 2; i++) {
		//start_dly_40ms:
		_delay_ms(40);	//40ms delay for command to be processed

		//func_set1:
		lcd_spi_transmit_CMD(i, 0x39);   // send function set #1
		_delay_us(30);	//26.3us delay for command to be processed


		//func_set2:
		lcd_spi_transmit_CMD(i, 0x39);	//send function set #2
		_delay_us(30);	//26.3us delay for command to be processed


		//bias_set:
		lcd_spi_transmit_CMD(i, 0x1E);	//set bias value.
		_delay_us(30);	//26.3us delay for command to be processed


		//power_ctrl:
		lcd_spi_transmit_CMD(i, 0x55);	//~ 0x50 nominal for 5V
		//~ 0x55 for 3.3V (delicate adjustment).
		_delay_us(30);	//26.3us delay for command to be processed


		//follower_ctrl:
		lcd_spi_transmit_CMD(i, 0x6C);	//follower mode on...
		_delay_ms(200);	//200ms delay for command to be processed


		//contrast_set:
		lcd_spi_transmit_CMD(i, 0x7F);	//~ 77 for 5V, ~ 7F for 3.3V
		_delay_us(30);	//26.3us delay for command to be processed


		//display_on:
		lcd_spi_transmit_CMD(i, 0x0C);	//display on, cursor off, blink off
		_delay_us(30);	//26.3us delay for command to be processed


		//clr_display:
		lcd_spi_transmit_CMD(i, 0x01);	//clear display, cursor home
		_delay_us(30);	//26.3us delay for command to be processed


		//entry_mode:
		lcd_spi_transmit_CMD(i, 0x06);	//clear display, cursor home & auto-increment
		_delay_us(30);	//26.3us delay for command to be processed
	
	}
	
}

//***************************************************************************
//
// Function Name : void still_display(void)
// Date : 3/29/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong
//
// This function updates the text shown on both the DOG LCDs. This function uses both of the
// SPI transmit functions written above to send the proper information. First, an address
// is sent to let the DOG LCD know which line it's writing in. Then, a for-loop is used to transmit
// all of the bytes in the display buffers. This step is repeated for all of the 3 lines and buffers.
//
// Warnings : Ensure that the 3 line buffers are populated with 17 total characters
// Restrictions : none
// Algorithms : init_spi_lcd, lcd_spi_transmit_CMD, lcd_spi_transmit_DATA
// References : none
//
// Revision History : Initial version
//
//**************************************************************************

void still_display(void) {
	
	for (uint8_t i = 0; i < 2; i++) {							// Loop to write left/right LCD display
		init_spi_lcd();
		lcd_spi_transmit_CMD(i, 0x80);							// init DDRAM address counter
		//lcd_spi_transmit_CMD(i, )
		for (uint8_t j = 0; j < 3; j++) {						// Loop to write rows
			_delay_us(30);
			for (uint8_t k = 0; k < 16; k++) {					// Loop to write each character in the rows
				if (!i)
					lcd_spi_transmit_DATA(i, lcd0_buff[j][k]);
				else
					lcd_spi_transmit_DATA(i, lcd1_buff[j][k]);
				_delay_us(30);
			}
		}
	}

	
}

//***************************************************************************
//
// Function Name : int sizeof_array(char* array) & int sizeof_matrix(char** matrix)
// Date : 4/20/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong
//
// These functions help calculate the sizes of each dimension in an array or matrix
//
//**************************************************************************

int sizeof_array(char* array) {
	int size = 0;
	while (array[size] != '\0') { size++; }
	return size;
}

int sizeof_matrix(char** matrix) {
	int size = 0;
	while (matrix[size] != NULL) { size++; }
	return size;
}

//***************************************************************************
//
// Function Name : void insert_split_msg(char* message)
// Date : 4/20/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong
//
// This function takes a string of any size and separates the message across two
// LCD buffers. The first (0th) buffer is used for the left LCD (LCD0), and the 
// second (1st) buffer is used for the right LCD (LCD1). Words that overflow
// the buffer on the right LCD display are completely moved to the next line of the
// left LCD for continuity. Spaces at the beginning of the first buffer row are removed
//
// Warnings : Make sure that the lcd0_buff and lcd1_buff have enough rows
//			  to support the length of the message string
// Restrictions : none
// Algorithms : none
// References : none
//
// Revision History : Initial version
//
//************************************************************************** 

void insert_split_msg(char* message) {
	/*
	if (lcd0_buff[lcd0_row] != NULL && lcd0_row != 0)
		lcd1_row = ++lcd0_row;
	if (lcd1_buff[lcd1_row] != NULL && lcd1_row != 0)
		lcd0_row = ++lcd1_row;
	*/
		
	uint8_t LCD_select = 0;
	int line_size = sizeof_array(message);
	for (int i = 0, col = 0; i < line_size; i++) {
		
		if (!LCD_select && !col && message[i] == ' ') // Skips any blank spaces at the beginning of the first LCD display
			continue;
		else if (!LCD_select) // Puts character into left LCD
			lcd0_buff[lcd0_row][col++] = message[i];
		else if (LCD_select && col != 0 && !(col % 15) && message[i] != ' ' && message[i] != '\0' && message[i + 1] != ' ' && message[i + 1] != '\0') { // Moves any word that would get cut off on the right LCD to the left LCD
			lcd1_buff[lcd1_row][16] = '\0';
			while (lcd1_buff[lcd1_row][col - 1] != ' ' && lcd1_buff[lcd1_row][col - 1] != '\0') { lcd1_buff[lcd1_row][--col] = ' '; i--; }
			i--;
			LCD_select = !LCD_select;
			col = 0;	
			lcd1_row++;
			continue;
		}
		else // Puts character into right LCD
			lcd1_buff[lcd1_row][col++] = message[i];
		
		if (col != 0 && !(col % 16)) { // Triggers every 16 iterations
			if (!LCD_select)
				lcd0_buff[lcd0_row++][col] = '\0';
			else
				lcd1_buff[lcd1_row++][col] = '\0';
			LCD_select = !LCD_select;
			col = 0;
		}
	}
	lcd0_row = ++lcd1_row;
}

//***************************************************************************
//
// Function Name : void insert_split_names(char** names)
// Date : 4/20/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong
//
// This function takes a matrix of stored names to display on both of the LCD
// displays. The first names are displayed on the left LCD right-justified. The
// last names are displayed on the right LCD left-justified. Both LCDs must be
// directly next to each other for the continuity of message.
//
// Warnings : Full names can only fill a maximum of 33 characters. The
//			  first and last name can fill a maximum of 16 characters each.
//			  Make sure that the lcd0_buff and lcd1_buff have enough rows
//			  to support the length of the message string
// Restrictions : none
// Algorithms : sizeof_array
// References : none
//
// Revision History : Initial version
//
//**************************************************************************

void insert_split_names(char** names) {
	/*
	if (lcd0_buff[lcd0_row] != NULL && lcd0_row != 0)
		lcd1_row = ++lcd0_row;
	if (lcd1_buff[lcd1_row] != NULL && lcd1_row != 0)
		lcd0_row = ++lcd1_row;
	*/
		
	uint8_t line_size = 0;
	for (uint8_t i = 0, tmp_size = 0; i < LINES; i++) {
		if (names[i] == NULL) break;
		tmp_size = sizeof_array(names[i]);
		if (tmp_size > line_size) {
			line_size = tmp_size;
		}
	}
	
	uint8_t space;
	for (uint8_t i = 0; i < LINES; i++) {
		if (names[i] == NULL) break;
		for (space = 0; space < line_size; space++) // Grabs the index of where the space
			if (names[i][space] == ' ') 
				break;
		
		for (uint8_t j = 0, k = 0; j < MAX_SIZE; j++) {
			if (j >= MAX_SIZE - space - 1) {
				lcd0_buff[lcd0_row][j] = names[i][k++];
				continue;
			}
			lcd0_buff[lcd0_row][j] = ' ';
		}
		
		lcd0_buff[lcd0_row++][MAX_SIZE - 1] = '\0';
		
		for (uint8_t j = 0, brk = 1; j < MAX_SIZE; j++) {	
			if (names[i][space + j + 1] != NULL && brk)		
				lcd1_buff[lcd1_row][j] = names[i][space + j + 1];
			else {
				brk = 0;
				lcd1_buff[lcd1_row][j] = ' ';
			}
		}
	
		lcd1_buff[lcd1_row++][MAX_SIZE - 1] = '\0';
	}
}

void center_justify(char** matrix0, char** matrix1) {
	int count = 0;
	
	for (uint8_t i = 0; i < LINES; i++) {
		if (matrix0[lcd0_row][0] == ' ') // Skips if it's not a left-justified message
			continue;
			
		for (uint8_t j = MAX_SIZE - 2; j > 0; j--) { // Starts at index that can have last possible character and counts whitespaces/nulls
					
			if (matrix1[i][j] != ' ' && matrix1[i][j] != '\0')
				break;
			matrix1[i][j] = ' ';							// Replaces any other null characters with spaces
			count++;
		}
		
		for (uint8_t j = 0; j < count/2; j++) {
			for (uint8_t k = MAX_SIZE - 2; k > 1; k--) {	// Shifts all contents of matrix1 to the right by 1
				matrix1[i][k] = matrix1[i][k - 1];
				//memmove(&matrix1[i][j + 1], &matrix1[i][j], (MAX_SIZE - j - 1) * sizeof(uint8_t));
			}
			
			matrix1[i][0] = matrix0[i][MAX_SIZE - 2];				// First index of matrix1 gets the rolled over value of matrix0
			
			for (uint8_t k = MAX_SIZE - 2; k > 1; k--) {	// Shifts all contents of matrix0 to the right by 1
				matrix0[i][k] = matrix0[i][k - 1];
			}
		}
	}
	
	
}

//***************************************************************************
//
// Function Name : down_scroll_display(void)
// Date : 4/20/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong
//
// This function takes a matrix of stored names to display on both of the LCD
// displays. The first names are displayed on the left LCD right-justified. The
// last names are displayed on the right LCD left-justified. Both LCDs must be
// directly next to each other for the continuity of message.
//
// Warnings : Full names can only fill a maximum of 33 characters. The
//			  first and last name can fill a maximum of 16 characters each.
//			  Make sure that the lcd0_buff and lcd1_buff have enough rows
//			  to support the length of the message string
// Restrictions : none
// Algorithms : sizeof_array
// References : none
//
// Revision History : Initial version
//
//**************************************************************************

void down_scroll_display(void) {
	
	for (uint8_t i = 0; i < LINES; i++) {							// Loop for number of down scrolls
		if (lcd0_buff[i][0] == NULL || lcd1_buff[i][0] == NULL) break;
		for (uint8_t j = 0; j < 2; j++) {							// Loop to write left/right LCD display
			init_spi_lcd();
			lcd_spi_transmit_CMD(j, 0x80);							// init DDRAM address counter
			for (uint8_t k = 0; k < 3; k++) {						// Loop to write rows
				_delay_us(30);
				for (uint8_t l = 0; l < 16; l++) {					// Loop to write each character in the rows
					if (!j)
						lcd_spi_transmit_DATA(j, lcd0_buff[i + k][l]);
					else
						lcd_spi_transmit_DATA(j, lcd1_buff[i + k][l]);
					_delay_us(30);
				}
			}
		}
		_delay_ms(SCROLLSPEED);
	}
	_delay_ms(1000);
}

#endif