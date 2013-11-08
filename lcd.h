//
// lcd.h
// Author: Todd Branchflower
// Date: 17 October 2013
// Description: Library to interact with the Geek Box LCD.
//

void initSPI();

void LCDinit();

void LCDclear();

void cursorToLineTwo();

void cursorToLineOne();

void writeChar(char asciiChar);

void writeString(char * string);

void scrollString(char * string1, char * string2);

void delayMicro();

void delayMilli();

void set_SS_lo();

void set_SS_hi();

void LCD_write_4(char Byte);

void SPI_send(char byteToSend);

void LCD_write_8(char byteToSend);


// Here are some helper functions from my LCD.c
// I don't expose these in my header, but may be helpful to you.

void writeDataByte(char dataByte);
void writeCommandNibble(char commandNibble);
void writeCommandByte(char commandByte);
