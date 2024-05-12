//***************************************************************************
//
// File Name : DOGM163WA.h
// Title :
// Date : 5/9/2024
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

#ifndef DOGM163WA_H_
#define DOGM163WA_H_

#define F_CPU 4000000LU

#include <avr/io.h>
#include <util/delay.h>

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
 
void lcd_spi_transmit_CMD (uint8_t LCD, unsigned char cmd);

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

void lcd_spi_transmit_DATA (uint8_t LCD, unsigned char cmd);

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

void init_spi_lcd (void);

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

void init_lcd_dog (void);

//***************************************************************************
//
// Function Name : void init_big_lcd_dog(void)
// Date : 3/29/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong & Kenneth Short
//
// This function sends serial bytes to both of the DOG LCD to configure its settings.
// Configuring the functionality of the display requires sending well timed
// serial packets in the correct order and with the right delays. This specific
// configuration makes the display one line with a large font. The buffers can only be
// 8 characters long (9 with null space).
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

void init_big_lcd_dog (void);


#endif /* DOGM163WA_H_ */