/************** ECE2049 DEMO CODE ******************/
/**************  13 March 2019   ******************/
/***************************************************/

#include <msp430.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"

long unsigned int intervals;

#pragma vector = TIMER2_A0_VECTOR
interrupt void A2TimerINTR(void)
{
    intervals++;
}

void timerA2config(void)
{
    TA2CTL = TASSEL_1 + ID_0 + MC_1; // use ACLK, put in up mode, and use division by 1 = same
    TA2CCR0 = 32767; //max value timer counts up to that number equal to .005004 seconds
    TA2CCTL0 = CCIE;
    _enable_interrupt();
}

enum Month{
 January,
 February,
 March,
 April,
 May,
 June,
 July,
 August,
 September,
 October,
 November,
 December
 };
 struct time{
 enum Month month;
 char day;
 char hour;
 char minute;
 char seconds;
 };
 struct time CounterToDate(long unsigned int Time){
     struct time output = {.month = January, .day = 0, .hour = 0, .seconds = 0, .minute = 0};
     long unsigned int CurrTime = Time; // grab a copy of the current seconds
     int days = CurrTime/86400;
     int secondsRemainingInDay = CurrTime%86400;
     bool flag = false;
     output.month = January;
     while(!flag){
         if(output.month == September ||  output.month == April || output.month == June||output.month == November){
             if(days - 30 < 0){ //are we at the right month?, then break while loop;
                 flag = true;
             }
             else{
                 output.month++; //if not increment output month to the next month
                 days -= 30;
             }

         }
         else if(output.month == February){
             if(days - 28 < 0){ //are we at the right month?, then break while loop;
                 flag = true;
             }
             else{
                 output.month++; //if not increment output month to the next month
                 days -= 28;
             }
         }
         else{
             if(days - 31 < 0){ //are we at the right month?, then break while loop;
                 flag = true;
             }
             else{
                 output.month++; //if not increment output month to the next month
                 days -= 31;
             }
         }
     }
     output.day = days + 1; //we decrement days based on month so the leftover is the day;
     output.hour = secondsRemainingInDay/3600; //hours are based on how many seconds we have //left in the day
     output.minute = (secondsRemainingInDay%3600)/60; //knowing how many hours find the //number of minutes into the hour
     output.seconds = secondsRemainingInDay-output.hour*3600-output.minute*60;//knowing how //many minutes fine the number of seconds we are into the day
 return output;
 }

 void displayTime(long unsigned int inTime){
     int seconds = (inTime % 60);
     int minutes = (inTime % 3600) / 60;
     int hours = (inTime % 86400) / 3600;
     unsigned char Time[10];
     Time[0] = ' ';
     Time[9] = ' ';
     Time[3] = ':';
     Time[6] = ':';
     Time[1] = (hours / 10) + 0x30;
     Time[2] = (hours % 10) + 0x30;

     Time[4] = (minutes / 10) + 0x30;
     Time[5] = (minutes % 10) + 0x30;

     Time[7] = (seconds / 10) + 0x30;
     Time[8] = (seconds % 10) + 0x30;


     Graphics_drawStringCentered(&g_sContext, Time, 10, 30, 35,
                                 TRANSPARENT_TEXT);

     struct time newTime = CounterToDate(inTime);
     char disp[8];
     char dispSZ = 8;
     disp[0] = ' ';
     disp[7] = ' ';
     disp[4] = 32;
     switch(newTime.month){
     case January:
         disp[1] = 'J';
         disp[2] = 'A';
         disp[3] = 'N';
         break;
     case February:
         disp[1] = 'F';
         disp[2] = 'E';
         disp[3] = 'B';
         break;
     case March:
         disp[1] = 'M';
         disp[2] = 'A';
         disp[3] = 'R';
         break;
     case April:
         disp[1] = 'A';
         disp[2] = 'P';
         disp[3] = 'R';
         break;
     case May:
         disp[1] = 'M';
         disp[2] = 'A';
         disp[3] = 'Y';
         break;
     case June:
         disp[1] = 'J';
         disp[2] = 'U';
         disp[3] = 'N';
         break;
     case July:
         disp[1] = 'J';
         disp[2] = 'U';
         disp[3] = 'L';
         break;
     case August:
         disp[1] = 'A';
         disp[2] = 'U';
         disp[3] = 'G';
         break;
     case September:
         disp[1] = 'S';
         disp[2] = 'E';
         disp[3] = 'P';
         break;
     case October:
         disp[1] = 'O';
         disp[2] = 'C';
         disp[3] = 'T';
         break;
     case November:
         disp[1] = 'N';
         disp[2] = 'O';
         disp[3] = 'V';
         break;
     case December:
         disp[1] = 'D';
         disp[2] = 'E';
         disp[3] = 'C';
         break;
     }
     disp[5] = newTime.day/10+0x30;
     disp[6] = newTime.day%10+0x30;
     Graphics_drawStringCentered(&g_sContext, disp, dispSZ, 24, 20, OPAQUE_TEXT);

 }

displayTemp(float inAvgTempC){
    unsigned char TempC[8];
    TempC[0] = ' ';
    TempC[7] = ' ';
    TempC[4] = '.';
    TempC[1] = ((int)inAvgTempC / 100) + 0x30;
    TempC[2] = (((int)inAvgTempC % 100) / 10) + 0x30;
    TempC[3] = ((int)inAvgTempC % 10) + 0x30;
    TempC[5] = ((inAvgTempC - (int)inAvgTempC)*10) + 0x30;
    TempC[6] = 'C';


    Graphics_drawStringCentered(&g_sContext, TempC, 8, 24, 55,
                                TRANSPARENT_TEXT);
    float inAvgTempF = (inAvgTempC * (1.8f)) + 32;
    unsigned char TempF[8];
    TempF[0] = ' ';
    TempF[7] = ' ';
    TempF[4] = '.';
    TempF[1] = ((int)inAvgTempF / 100) + 0x30;
    TempF[2] = (((int)inAvgTempF % 100) / 10) + 0x30;
    TempF[3] = ((int)inAvgTempF % 10) + 0x30;
    TempF[5] = ((inAvgTempF - (int)inAvgTempF)*10) + 0x30;
    TempF[6] = 'F';

    Graphics_drawStringCentered(&g_sContext, TempF, 8, 24, 65,
                                        TRANSPARENT_TEXT);


}

// Function Prototypes
void swDelay(char numLoops);

// Declare globals here

// Main
void main(void)

{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
    P1SEL = P1SEL & ~BIT0;          // Select P1.0 for digital IO
    P1DIR |= BIT0;
    P4SEL = P4SEL & ~BIT7;          // Select P4.7 for digital IO
    P4DIR |= BIT7;
    unsigned char currKey=0;
    configDisplay();
    configKeypad();
    timerA2config();

    while(1){
        Graphics_clearDisplay(&g_sContext);
        displayTime(intervals);
        displayTemp(123.4);
        Graphics_flushBuffer(&g_sContext);
        swDelay(1);

    }
    /*unsigned char currKey=0, dispSz = 3;
    unsigned char dispThree[3];

    // Define some local variables
    float a_flt = 190.68;
    int  test = 0x0600, i=0;     // In C prefix 0x means the number that follows is in hex
    long unsigned X= 123456;    // No prefix so number is assumed to be in decimal
    unsigned char myGrade='A';
    unsigned char initial='S';
    //unsigned char your_name[14] = "Your Name Here";
                                    // What happens when you change the array length?
                                    // What should it be? Do you need null terminator /n ?


    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired

    // Some utterly useless instructions -- Step through them
    // What size does the Code Composer MSP430 Compiler use for the
    // following variable types? A float, an int, a long integer and a char?
    a_flt = a_flt*test;
    X = test+X;
    test = test-myGrade;    // A number minus a letter?? What's actually going on here?
                            // What value stored in myGrade (i.e. what's the ASCII code for "A")?
                            // Thus, what is the new value of test? Explain?

    // Useful code starts here
    initLeds();

    configDisplay();
    configKeypad();

    // *** Intro Screen ***
    Graphics_clearDisplay(&g_sContext); // Clear the display

    // Write some text to the display
    Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "ECE2049-C22!", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);

    // Draw a box around everything because it looks nice
    Graphics_Rectangle box = {.xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
    Graphics_drawRectangle(&g_sContext, &box);

    // We are now done writing to the display.  However, if we stopped here, we would not
    // see any changes on the actual LCD.  This is because we need to send our changes
    // to the LCD, which then refreshes the display.
    // Since this is a slow operation, it is best to refresh (or "flush") only after
    // we are done drawing everything we need.
    Graphics_flushBuffer(&g_sContext);

    dispThree[0] = ' ';
    dispThree[2] = ' ';

    while (1)    // Forever loop
    {
        // Check if any keys have been pressed on the 3x4 keypad
        currKey = getKey();
        if (currKey == '*')
            BuzzerOn();
        if (currKey == '#')
            BuzzerOff();
        if ((currKey >= '0') && (currKey <= '9'))
            setLeds(currKey - 0x30);

        if (currKey)
        {
            dispThree[1] = currKey;
            // Draw the new character to the display
            Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 48, 55, OPAQUE_TEXT);

            // Refresh the display so it shows the new data
            Graphics_flushBuffer(&g_sContext);

            // wait awhile before clearing LEDs
            swDelay(1);
            setLeds(0);
        }

    }  // end while (1)*/
}


void swDelay(char numLoops)
{
	// This function is a software delay. It performs
	// useless loops to waste a bit of time
	//
	// Input: numLoops = number of delay loops to execute
	// Output: none
	//
	// smj, ECE2049, 25 Aug 2013

	volatile unsigned int i,j;	// volatile to prevent removal in optimization
			                    // by compiler. Functionally this is useless code

	for (j=0; j<numLoops; j++)
    {
    	i = 50000 ;					// SW Delay
   	    while (i > 0)				// could also have used while (i)
	       i--;
    }
}
