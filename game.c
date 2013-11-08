/*
 * game.c
 *
 *  Created on: Nov 4, 2013
 *      Author: C15Trevor.Moore
 */
#include <msp430g2553.h>
#include "game.h"
#include "lcd.h"

unsigned char initPlayer()
{
        return 0x80;
}

void printPlayer(unsigned char player)
{
        writeCommandByte(player);
        writeDataByte('*');
}

void clearPlayer(unsigned char player)
{
        writeCommandByte(player);
        writeDataByte(' ');
}

unsigned char movePlayer(unsigned char player, unsigned char direction)
{			clearPlayer(player);
        switch (direction) {

        	case UP:
        		if (player>= 0xc0){		//if player is on line 2
        			player -=0x40;
        		}
        		break;

        	case DOWN:
        		if(player<=0x87){		//if player is on line 1
        			player+=0x40;
        		}
        		break;

        	case LEFT:
        		if((player & 0x0f)>0){		//checks if 1's place is zero
        			player--;
        		}
        		break;

        	case RIGHT:
        		if (player < 0x87 || player >= 0xC0){ //makes sure player doesn't go off screen
        			player++;
        		}
        		break;

        }

        printPlayer(player);
        return player;
}

char didPlayerWin(unsigned char player)
{
        return player == 0xC7;
}

