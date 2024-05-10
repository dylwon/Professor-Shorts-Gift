//***************************************************************************
//
// File Name : functions.c
// Title :
// Date : 5/9/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong & Baron Mai
//
// This function file defines all the higher level functions to display the proper
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

#include "functions.h"
#include "DOGM163WA.h"

extern int lcd0_row, lcd1_row = 0;

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
			lcd1_buff[lcd1_row][line_size - 1] = '\0';
			lcd1_row++;
			continue;
		}
		else // Puts character into right LCD
			lcd1_buff[lcd1_row][col++] = message[i];
		
		if (col != 0 && !(col % 16)) { // Triggers on 16th column index
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