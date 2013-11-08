#include <msp430.h> 
#include"button.h"
#include"lcd.h"
#include"game.h"
/*
 * main.c
 */
    void init_timer();
    void init_buttons();
    void movePlayerInResponseToButtonPush(char buttonToTest);
    void replay();

    char player=0;
    char timerCount=0;

    void clearTimer(){
         timerCount = 0;
         TACTL |= TACLR;
    }

  int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    initSPI();
    LCDinit();
    LCDclear();
    init_timer();
    init_buttons();
    player = initPlayer();
    printPlayer(player);
    __enable_interrupt();

    while(1){
    	if (player ==0xc7){			//if player hits bottom left corner
    		TACTL &= ~TAIE;
    		LCDclear();
    		cursorToLineOne();
    		writeString("YOU");
    		cursorToLineTwo();
    		writeString("WON");
    		_delay_cycles(1000000);
    		replay();
    	}

    	if(timerCount >= 4){		//if time runs out
    		TACTL &= ~TAIE;
    		LCDclear();
    		cursorToLineOne();
    		writeString("GAME");
    		cursorToLineTwo();
    		writeString("OVER");
    		_delay_cycles(1000000);
    		replay();
    	}
    }
}
void init_timer()
{
	   TACTL &= ~(MC1|MC0);
       TACTL |= TACLR;
       TACTL |= TASSEL1;
       TACTL |= ID0|ID1;
       TACTL &= ~TAIFG;
       TACTL |= MC1;
       TACTL |= TAIE;
}

void init_buttons()
{
	configureP1PinAsButton(BIT1);
	configureP1PinAsButton(BIT2);
	configureP1PinAsButton(BIT3);
	configureP1PinAsButton(BIT4);
	P1IES |= BIT1|BIT2|BIT3|BIT4;
	P1IFG &=~BIT1|BIT2|BIT3|BIT4;
	P1IE |= BIT1|BIT2|BIT3|BIT4;
}
void testAndRespondToButtonPush(char buttonToTest)
{
    if (buttonToTest & P1IFG)
    {
        if (buttonToTest & P1IES)
        {
            movePlayerInResponseToButtonPush(buttonToTest);
            clearTimer();
        } else
        {
            debounce();
        }

        P1IES ^= buttonToTest;
        P1IFG &= ~buttonToTest;
    }
}
 void movePlayerInResponseToButtonPush(char buttonToTest){
	 	 switch(buttonToTest){
	 	 case BIT1:
	 		player =  movePlayer(player,UP);
	 		break;
	 	 case BIT2:
	 		player = movePlayer(player,DOWN);
	 		 break;
	 	 case BIT3:
	 		player = movePlayer(player,LEFT);
	 		 break;
	 	 case BIT4:
	 		player = movePlayer(player,RIGHT);
	 		 break;
	 	 }

}
 void replay(){								//restarts the game if any button is pushed
	 char *topLine =	"Play again?" ;
	 char *bottomLine = "Press any button";
	cursorToLineOne();
	writeString(topLine);
	cursorToLineTwo();
	writeString(bottomLine);

	char buttons[] = {BIT1, BIT2, BIT3, BIT4};
	char pressedButton = pollP1Buttons(buttons, 4);

	waitForP1ButtonRelease(pressedButton);
	LCDclear();
	timerCount = 0;
	player = initPlayer();
	printPlayer(player);
	init_timer();
 }
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(){
    TACTL &= ~TAIFG;
    timerCount++;
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void){
		testAndRespondToButtonPush(BIT1);
		testAndRespondToButtonPush(BIT2);
		testAndRespondToButtonPush(BIT3);
		testAndRespondToButtonPush(BIT4);
}
