/*
 * lcd.c
 *
 *  Created on: Oct 22, 2013
 *      Author: C15Trevor.Moore
 */
#include <msp430.h>
#include"lcd.h"
#define RS_MASK 0x40
char LCDSEND, LCDCON;
void LCDinit()
{
    writeCommandNibble(0x03);

    writeCommandNibble(0x03);

    writeCommandNibble(0x03);

    writeCommandNibble(0x02);

    writeCommandByte(0x28);

    writeCommandByte(0x0C);

    writeCommandByte(0x01);

    writeCommandByte(0x06);

    writeCommandByte(0x01);

    writeCommandByte(0x02);

    SPI_send(0);
    delayMicro();
}

void writeCommandNibble(char commandNibble)
{
    LCDCON &= ~RS_MASK;
    LCD_write_4(commandNibble);
    delayMilli();
}

void writeCommandByte(char commandByte)
{
    LCDCON &= ~RS_MASK;
    LCD_write_8(commandByte);
    delayMilli();
}

void writeDataByte(char dataByte)
{
    LCDCON |= RS_MASK;
    LCD_write_8(dataByte);
    delayMilli();
}

void LCD_write_8(char byteToSend)
{
    unsigned char sendByte = byteToSend;

    sendByte &= 0xF0;

    sendByte = sendByte >> 4;               // rotate to the right 4 times

    LCD_write_4(sendByte);

    sendByte = byteToSend;

    sendByte &= 0x0F;

    LCD_write_4(sendByte);
}

void SPI_send(char byteToSend)
{
     volatile char readByte;

    set_SS_lo();

    UCB0TXBUF = byteToSend;

    while(!(UCB0RXIFG & IFG2))
    {
        // wait until you've received a byte
    }

    readByte = UCB0RXBUF;

    set_SS_hi();
}
void initSPI(){
			UCB0CTL1|= UCSWRST;
			UCB0CTL0|= UCCKPH|UCMSB|UCMST|UCSYNC;
			UCB0CTL0&= ~UCCKPL;
			UCB0CTL1|= UCSSEL1;
			UCB0STAT|= UCLISTEN;
			P1SEL	|= BIT5|BIT6|BIT7;
			P1SEL2	|= BIT5|BIT6|BIT7;
			P1DIR	|= BIT0;
			UCB0CTL1&= ~UCSWRST;

}
void LCD_write_4(char Byte){
	                  char LCDDATA = Byte;
	                  LCDDATA &= 0x0f;
	                  LCDDATA|= LCDCON;                                            //; set LCD control nibble
	                  LCDDATA &= 0x7f;                                              //; set E low
	                  SPI_send(LCDDATA);
	                  delayMicro();
	                  LCDDATA |= 0x80;                                               //; set E high
	                  SPI_send(LCDDATA);
	                  delayMicro();
	                  LCDDATA &= 0x7f;                                               //; set E low
	                  SPI_send(LCDDATA);
	                  delayMicro();

}
void LCDclear(){
	writeCommandByte(1);
}
void cursorToLineTwo(){
	writeCommandByte(0xc0);
}
void cursorToLineOne(){
	writeCommandByte(0x01);
}
void writeChar(char asciiChar){
	writeDataByte(asciiChar);
}
void writeString(char * string){
	while(*string != 0){
		writeChar(*string);
		string++;
	}
}
void scrollString(char * string1, char * string2){
	char*string1Loop = string1;
	char*string2Loop = string2;
	while(1){
		cursorToLineOne();
		writeString(string1Loop);
		cursorToLineTwo();
		writeString(string2Loop);
		string1Loop++;
		if(*string1Loop == 0){
			string1Loop = string1;
		}
		string2Loop++;
		if(*string1Loop == 0){
					string1Loop = string1;
				}
		if(*string2Loop == 0){
					string2Loop = string2;
				}
		_delay_cycles(200000);
	}

}
void delayMicro(){
	_delay_cycles(45);
}
void delayMilli(){
	_delay_cycles(1817);
}
void set_SS_hi(){
 	  P1OUT|= BIT0;
}
void set_SS_lo(){
	  P1OUT&= ~BIT0;
}
