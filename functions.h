//***************************************************************************
//
// File Name : functions.h
// Title :
// Date : 5/9/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong & Baron Mai
//
// This header file declares all the higher level functions to display the proper
// messages on the LCD screens
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

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define F_CPU 4000000LU
#define LINES 100
#define MAX_SIZE 17
#define SCROLLSPEED 500

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>

char lcd0_buff[LINES][MAX_SIZE];
char lcd1_buff[LINES][MAX_SIZE];

int lcd0_row, lcd1_row;

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

int sizeof_array(char* array);

int sizeof_matrix(char** matrix);

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

void still_display(void);

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

void insert_split_msg(char* message);

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

void insert_split_names(char** names);

//***************************************************************************
//
// Function Name : center_justify(char** matrix0, char** matrix1)
// Date : 5/10/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong
//
// This function takes two matrices and centers the strings across each of the rows
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

void center_justify(char** matrix0, char** matrix1);

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

void down_scroll_display(void);


#endif /* FUNCTIONS_H_ */