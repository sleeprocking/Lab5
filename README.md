Lab5
====

a simple game

This game makes use of the lcd.h and button.h libraries used in lab 4; the code is unchanged

The game uses a timer interrupt to determine if two seconds have passed with no action, thus ending the game
It also uses an ISR to handle the button presses, rather than polling the buttons for player movement

The game also features a replay(); subroutine that allows the game to be played again without needing to hard reset
