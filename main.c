#include <msp430.h> 


volatile unsigned char eigths = 0;
volatile unsigned char seconds = 0;
volatile unsigned char minutes_ones = 5;
volatile unsigned char minutes_tens = 3;
volatile unsigned char hours = 11;
char toPrint[4];
char playState = 0;
char printingNumber = 0;
char doubleDelay = 0;


void config_clk(){
    TA0CTL = TASSEL_1 | ID_3 | MC_1; //Use ACLK (hopefully 32768 Hz), divider 8
    //TA0CCR0 = 61439; //Count 15 seconds if divider is 8
    //TA0CCR0 = 4095; //Count 1 second if divider is 8
    TA0CCR0 = 511; // 1/8 second
    TA0CCTL0 = CCIE;
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR(void) {
    eigths++;
    if(playState){
        if(toPrint[printingNumber] != 0){
            if(!(P1OUT & BIT0)){ //Output is off
                P1OUT |= BIT0; //Turn output on
            }
            else{
                P1OUT &= ~BIT0;
                toPrint[printingNumber] --; //Decrement number being printed
            }
            doubleDelay = 1;
        }
        else if(doubleDelay){
            doubleDelay = 0;
        }
        else if(printingNumber <= 2){
            printingNumber ++;
        }
        else{//We've finished printing the last number
            playState = 0;
            printingNumber = 0;
            doubleDelay = 1;
            //P1OUT &= ~BIT0;
        }
    }
    if(eigths > 7){
        eigths = 0;
        seconds++;
        if(seconds > 59){
            seconds = 0;
            minutes_ones++;
            if(minutes_ones > 9){
                minutes_ones = 0;
                minutes_tens ++;
                if(minutes_tens > 5){
                    hours++;
                    minutes_tens = 0;
                    if(hours > 12){
                        hours = 1;
                    }
                }
            }
        }
    }
    //P1OUT ^= BIT0;
}


#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){
    P2IFG &= ~BIT3;           // P2.3 IFG cleared
    if(playState == 0){
        playState = 1;
    }
    else{
        return; //This stops the time from playing twice if you hit the button while it's playing
    }
    if(hours >= 10) {
        toPrint[0] = 1;
        toPrint[1] = hours - 10;
    }
    else{
        toPrint[0] = 0;
        toPrint[1] = hours;
    }
    //} else {
//        toPrint[0] = 0;
//        toPrint[1] = hours;
//    }
    //Sorry future code readers! I'm doing it this way because I don't want to include math.h.
    //I don't have enough room for heavy libraries here. I'm working with about 200 bytes.

    toPrint[2] = minutes_tens;
    toPrint[3] = minutes_ones;
}

void initLEDs(){
    P1SEL &= ~(BIT0 | BIT6);
    P1DIR |= BIT0 | BIT6;
    //P1OUT &= ~(BIT0|BIT6);


}

void initButton(){

    P2DIR &= ~BIT3;
    P2IE |= BIT3;
    P2IES |= BIT3;
    P2REN |= BIT3;
    P2IFG &= ~BIT3;
    P2OUT |= BIT3;
/*
    From when I was using the onboard button (1.3):

    P1DIR &= ~BIT3;           // Set P1.3 to input direction
    P1IE |=  BIT3;            // P1.3 interrupt enabled
    P1IES |= BIT3;            // P1.3 Hi/lo edge. Assuming this is when button pushed.
    P1REN |= BIT3;            // Enable Pull Up on SW2 (P1.3)
    P1IFG &= ~BIT3;           // P1.3 IFG cleared
    P1OUT |= BIT3;           // Pull resistor is pull up
*/
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    _BIS_SR(GIE);
    config_clk();
    initLEDs();
    initButton();
    P1OUT &= ~BIT6; //Turn off that other LED

    playState = 0;
    _BIS_SR(LPM3_bits|GIE);

    /*
    while(1){
	    _no_operation();
	}
	*/
}

