//***************************************************************************
//
// File Name : DOGM163WA.c
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

#include "DOGM163WA.h"

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
