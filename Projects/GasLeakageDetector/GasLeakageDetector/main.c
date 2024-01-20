//////////////////////////////////////////////////////////////////////////
//Gas Detector by Nubal Manhas
//
//Credits to Simon Walker for some of the libraries
//////////////////////////////////////////////////////////////////////////
#include <atmel_start.h>
#include <stdio.h>
#include "sci.h"
#include "timer.h"
#include "atd.h"
#include <util/delay.h>
#include <mq2.h>
#include "I2C.h"
#include <stdlib.h>
#include "utils/utils.h"
int main(void)
{
	// variable for managing the A/D update
	const unsigned int cuiAtoDEventCount = 500;
	unsigned int uiAtoDEventNext = cuiAtoDEventCount;
	// provided init by framework
	atmel_start_init();
	I2C_Init(16E6, I2CBus400);
	LCDInit();
	//unsigned char c[];
	// bring up A/D channel 0
	AtoD_Init(AtoD_Channel_1);
	unsigned char AD_low = ADCL; // must be read first
	unsigned char AD_high = ADCH;
	unsigned int AD_Raw = AD_low + AD_high * 256;
	unsigned int detected = 0;
	// the standard "don't leave me!" loop
	while (1)
	{
		
		
		AD_low = ADCL; // must be read first
		AD_high = ADCH;
		unsigned char buff [8] = {0};
		AD_Raw = AD_low + AD_high * 256;
		(void)sprintf(buff, "PPM:%X", AD_Raw);
		LCD_Write(buff);
		if(AD_Raw > 409){
			Buzzer_toggle_level(true);
			_delay_ms(200);
		}
		
		_delay_ms(2000);
		WriteInst(0x01);
		_delay_ms(200);
	}
	

}
//////////////////////////////////////////////////////////////////////////
//Initialize the LCD
//
//Page 18 of the datasheet:
//https://www.orientdisplay.com/wp-content/uploads/2019/10/AMC0802BR-B-Y6WFDY-I2C.pdf
//
//0111110 = address -> 0x3C
//Wait more than ms after vdd rises to 5V
//Start I2C using our address (0x3C)
//
//Instructions to send:
//				  8421
//Function Set -> 001DL NFXX -> 0010 1000 -> 0x38
//					   DL = 8bit/4bit display line (0)
//					   N = 2-line/1-line + display font (1)
//                     F = 5x11 dots/5x8 dots (0)
//                     X = don't care, just set it to 0 for now
//Wait > 100us -> 1ms delay
//Display on/off -> 0000 1DCB -> 0000 1111 -> 0x0F
//                       D = Set display on/off (1)
//                       C = Cursor on/off (1)
//						 B = Cursor Blinking on/off (1)
//Entry Mode Set -> 0000 01IDSH -> 0000 0110 -> 0x06
//						 ID = increment/decrement cursor (1)
//						 SH = shift (0)
//Wait 1ms
//Clear Display -> 0000 0001 -> 0x01
//Wait 10ms
//
//In the datasheet it says to clear the display before Entry Mode, but that
//doesn't seem to work for me?
//////////////////////////////////////////////////////////////////////////
int LCDInit(void){
	
	//wait for V rise
	_delay_ms(40);
	//Start our I2C routine, read = false
	I2C_Start(0x3C, false);
	_delay_ms(1);
	//Function Set
	I2C_Write8(0x38, false);
	_delay_ms(1);
	//Display On
	I2C_Write8(0x0F, false);
	_delay_ms(1);
	//Entry Mode Set
	I2C_Write8(0x06, false);
	_delay_ms(1);
	//Clear Display
	I2C_Write8(0x01, true);
	_delay_ms(10);
	
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//Write to the LCD
//
//Page 19 of the Datasheet:
//https://www.orientdisplay.com/wp-content/uploads/2019/10/AMC0802BR-B-Y6WFDY-I2C.pdf
//str = unsigned char array, contains the data to write
//
//iterate through str[]
//Start I2C using our address (0x3C)
//Set CGRAM(?) to 0x40
//write out the character at index i from str[] to the I2C
//
//Only the top part of the LCD works, won't need more than that for the
//purposes of this project
//////////////////////////////////////////////////////////////////////////
void LCD_Write(unsigned char str[]){
	for(int i = 0; i < strlen(str); ++i){
		I2C_Start(0x3C, false);
		_delay_ms(1);
		I2C_Write8(0x40, false);
		_delay_ms(1);
		
		I2C_Write8(str[i], true);
		_delay_ms(1);
	}
	
}

void WriteInst(unsigned char byInst)
{
	I2C_Start(0x3C, false);
	I2C_Write8(0x3C, false);
	I2C_Write8(0x00, false);
	I2C_Write8(byInst, true);

}

void InitRW1063(void)
{
	WriteInst (0x38); //DL=1: 8 bits; N=1: 2 line; F=0: 5 x 8dots
	WriteInst (0x0F); // D=1, display on; C=B=1; cursor on; blinking on;
	WriteInst (0x06); // I/D=1: Increment by 1; S=0: No shift
	
}




