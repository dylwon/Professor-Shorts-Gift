//***************************************************************************
//
// File Name : messages.h
// Title :
// Date : 4/19/2024
// Version : 1.0
// Target MCU : AVR128DB48
// Target Hardware : AVR128DB48
// Author : Dylan Wong
//
// This file stores the messages to be displayed on the DOG LCD
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


#ifndef NAMES_H_
#define NAMES_H_
#include <stdlib.h>

char *names[33] = {
	"Stanley Cokro",
	"Nisat Nosin",
	"Dylan Wong",
	"Luke Melfa",
	"Eric Yang",
	"Farhaan Khan",
	"Johnson Varghese",
	"Hillary Ng",
	"John Shin",
	"Ben Weng",
	"Savi Kessler",
	"Kenny Procacci",
	"Shaun Varghese",
	"Christina Wong",
	"Mahima Karanth",
	"Aritro Sarkar",
	"Kyle Han",
	"Spencer Wu",
	"Rachel Leong",
	"Natalie Sid",
	"Dilshoda Sayfillaeva",
	"Alexander Monov",
	"Pranay Srivastava",
	"Katherine Trusinski",
	"Eric Wu",
	"Devin Lee"
};

char message[] = { 
	"Thank you for teaching us, through good health and sickness, you've always been there and we appreciate you. We hope you get better soon"
};

char special_thanks[] = {
	"Special Thanks to Bryant Gonzaga for organizing this student project"
};

char idle_screen[] = {
	
};

#endif /* NAMES_H_ */