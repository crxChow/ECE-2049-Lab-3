#include <msp430.h>
#include "peripherals.h"

long unsigned int intervals;
struct time editTime;
void timerA2config(void)
{
    TA2CTL = TASSEL_1 + ID_0 + MC_1; // use ACLK, put in up mode, and use division by 1 = same
    TA2CCR0 = 32767; //max value timer counts up to that number equal to .005004 seconds
    TA2CCTL0 = CCIE;
    _enable_interrupt();
}
enum Editing{
    month,
    day,
    hour,
    minute,
    second
};
void configButtons(void){
    P2SEL &= ~(BIT1);
    P2DIR &= ~(BIT1);
    P2REN |= (BIT1);
    P2OUT |= (BIT1);

    P1SEL &= ~(BIT1);
    P1DIR &= ~(BIT1);
    P1REN |= (BIT1);
    P1OUT |= (BIT1);
}
//Returns 0 if no button is pressed and if S1 is pressed returns 1 else if S2 is pressed returns 2
int ReadButtons(){
    int ret_val = 0;
    if (!(P2IN & BIT1))
    {
        ret_val += 1;
    }
    else if(!(P1IN & BIT1)){
        ret_val += 2;
    }
    return ret_val;
}



enum State{
    Editing,
    Timing
};
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

 int calcTime(){

     int ret_val = 0;
     ret_val += editTime.seconds;
     ret_val += editTime.minute*60;
     ret_val += editTime.hour*3600;
     ret_val += editTime.day*86400;
     switch(editTime.month){
default:
    break;
     case February:
         ret_val += 2678400;
         break;
     case March:
         ret_val += 5097600;
         break;
     case April:
         ret_val += 7776000;
         break;
     case May:
         ret_val += 1036800;
         break;
     case June:
         ret_val += 13046400;
         break;
     case July:
         ret_val += 15638400;
         break;
     case August:
         ret_val += 18316800;
         break;
     case September:
         ret_val += 20995200;
         break;
     case October:
         ret_val += 2587200;
         break;
     case November:
         ret_val += 26265600;
         break;
     case December:
         ret_val += 28857600;
         break;
     }
     return ret_val;
 }

 void displayTimeStruct(struct time ToBeDisplayed){
     unsigned char Time[10];
          Time[0] = ' ';
          Time[9] = ' ';
          Time[3] = ':';
          Time[6] = ':';
          Time[1] = (ToBeDisplayed.hour / 10) + 0x30;
          Time[2] = (ToBeDisplayed.hour % 10) + 0x30;
          Time[4] = (ToBeDisplayed.minute / 10) + 0x30;
          Time[5] = (ToBeDisplayed.minute % 10) + 0x30;

          Time[7] = (ToBeDisplayed.seconds / 10) + 0x30;
          Time[8] = (ToBeDisplayed.seconds % 10) + 0x30;
          Graphics_drawStringCentered(&g_sContext, Time, 10, 30, 35,
                                           TRANSPARENT_TEXT);

          char disp[8];
               char dispSZ = 8;
               disp[0] = ' ';
               disp[7] = ' ';
               disp[4] = 32;
               switch(ToBeDisplayed.month){
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
               disp[5] = ToBeDisplayed.day/10+0x30;
               disp[6] = ToBeDisplayed.day%10+0x30;
               Graphics_drawStringCentered(&g_sContext, disp, dispSZ, 24, 20, OPAQUE_TEXT);
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
    Graphics_drawStringCentered(&g_sContext, TempF, 8, 24, 65,TRANSPARENT_TEXT);
}


#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)
#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)
//Variables
unsigned int in_temp;
unsigned int Scroll_Wheel;
volatile unsigned int bits30, bits85;

volatile float temperatureDegC = 0;
volatile float temperatureDegF = 0;
volatile float degC_per_bit;
float temp[36];
enum State state = Timing;
void main(void){
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer

    volatile float scroll;
    {
        volatile unsigned int i = 0;
        for(i =0; i<36;i++){
            temp[i] = 0;
        }
    }
    //Configure peripherals
    configDisplay();
    configKeypad();
    configButtons();
    initLeds();
    //Configure Timer
    timerA2config();


    bits30 = CALADC12_15V_30C;
    bits85 = CALADC12_15V_85C;
    degC_per_bit = ((float)(85.0 - 30.0))/((float)(bits85-bits30));

    //ADC setup
      REFCTL0 &= ~REFMSTR;    // Reset REFMSTR to hand over control of internal reference voltages to ADC12_A control registers

      ADC12CTL0 = ADC12SHT0_15 | ADC12REFON | ADC12ON;//Set Reference voltage on and sample and hold on select bit 15

      ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;// Enable sample timer and Set Sequential Channel Reading

      // Using ADC12MEM0 to store reading of Temp
      ADC12MCTL0 = ADC12SREF_1 | ADC12INCH_10;
      // Using ADC12MEM1 to store reading of Scroll Wheel
      ADC12MCTL1 = ADC12SREF_0 | ADC12INCH_0 |ADC12EOS;
      __delay_cycles(100);                    // delay to allow Ref to settle
      ADC12CTL0 |= ADC12ENC;              // Enable conversion

      while(1){
          if(state == Timing){
            if(ReadButtons() == 1){
                state=Editing;
                editTime = CounterToDate(intervals);
            }
          }
          if(state == Editing){
              if(ReadButtons() == 1){
                  state = Timing;
              }
          }
      }

}
enum Editing currEdit = month;
int LastButtonPressedStatus = 0;
#pragma vector = TIMER2_A0_VECTOR
interrupt void A2TimerINTR(void)
{
    if(state == Timing)
    {
        intervals+= 1;
    }
    ADC12CTL0 &= ~ADC12SC;  // clear the start bit
    ADC12CTL0 |= ADC12SC;       // Sampling and conversion start
    in_temp = ADC12MEM0;      // Read in results if conversion
    Scroll_Wheel = ADC12MEM1;
    temperatureDegC = (float)((long)in_temp - CALADC12_15V_30C) * degC_per_bit +30.0;
    temp[intervals%36] = temperatureDegC;
    Graphics_clearDisplay(&g_sContext);
    volatile float average=0;
    {
        volatile int k =0;
        if(intervals<36){
            average = temperatureDegC;
        }
        else{
            for(k = 0; k<36;k++){
                average += temp[k]/36;
            }
        }

    }
    displayTemp(average);
    if(state == Editing){
        int buttonStatus = ReadButtons();
        if(currEdit == month){
            editTime.month = (int)(Scroll_Wheel/345);
            if(buttonStatus == 2 & LastButtonPressedStatus != 2){
                currEdit++;
            }
        }
        else if(currEdit == day){
            if(editTime.month == September ||  editTime.month == April || editTime.month == June|| editTime.month == November){
                editTime.day = (int)(Scroll_Wheel/140)+1; //30 days
            }
            else if(editTime.month == February){
                editTime.day = (int)(Scroll_Wheel/146)+1; //28 days
            }
            else{
                editTime.day = (int)(Scroll_Wheel/135)+1; //31 days
            }
            if(buttonStatus == 2 &  LastButtonPressedStatus != 2){
                currEdit++;
            }
        }
        else if(currEdit == hour){
            editTime.hour = (int)(Scroll_Wheel/170);
            if(buttonStatus == 2 &  LastButtonPressedStatus != 2){
                currEdit++;
            }
        }
        else if(currEdit == minute){
            editTime.minute = (int)(Scroll_Wheel/69);
            if(buttonStatus == 2 &  LastButtonPressedStatus != 2){
                currEdit++;
            }
        }
        else if(currEdit == second){
            editTime.seconds = (int)(Scroll_Wheel/69);
            if(buttonStatus == 2 &  LastButtonPressedStatus != 2){
                currEdit = month;
            }
        }

        if(getKey() == '*'){
            intervals = calcTime();

            state = Timing;
        }
        LastButtonPressedStatus = buttonStatus;
        displayTimeStruct(editTime);
    }
    else{
        displayTime(intervals);
    }
    Graphics_flushBuffer(&g_sContext);
}



// Main
//void main(void)
//
//{
//    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
//    P1SEL = P1SEL & ~BIT0;          // Select P1.0 for digital IO
//    P1DIR |= BIT0;
//    P4SEL = P4SEL & ~BIT7;          // Select P4.7 for digital IO
//    P4DIR |= BIT7;
//    unsigned char currKey=0;
//    configDisplay();
//    configKeypad();
//    timerA2config();
//
//    while(1){
//        Graphics_clearDisplay(&g_sContext);
//        displayTime(intervals);
//        displayTemp(123.4);
//        Graphics_flushBuffer(&g_sContext);
//        swDelay(1);
//
//    }
//    /*unsigned char currKey=0, dispSz = 3;
//    unsigned char dispThree[3];
//
//    // Define some local variables
//    float a_flt = 190.68;
//    int  test = 0x0600, i=0;     // In C prefix 0x means the number that follows is in hex
//    long unsigned X= 123456;    // No prefix so number is assumed to be in decimal
//    unsigned char myGrade='A';
//    unsigned char initial='S';
//    //unsigned char your_name[14] = "Your Name Here";
//                                    // What happens when you change the array length?
//                                    // What should it be? Do you need null terminator /n ?
//
//
//    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
//                                 // You can then configure it properly, if desired
//
//    // Some utterly useless instructions -- Step through them
//    // What size does the Code Composer MSP430 Compiler use for the
//    // following variable types? A float, an int, a long integer and a char?
//    a_flt = a_flt*test;
//    X = test+X;
//    test = test-myGrade;    // A number minus a letter?? What's actually going on here?
//                            // What value stored in myGrade (i.e. what's the ASCII code for "A")?
//                            // Thus, what is the new value of test? Explain?
//
//    // Useful code starts here
//    initLeds();
//
//    configDisplay();
//    configKeypad();
//
//    // *** Intro Screen ***
//    Graphics_clearDisplay(&g_sContext); // Clear the display
//
//    // Write some text to the display
//    Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
//    Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
//    Graphics_drawStringCentered(&g_sContext, "ECE2049-C22!", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
//
//    // Draw a box around everything because it looks nice
//    Graphics_Rectangle box = {.xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
//    Graphics_drawRectangle(&g_sContext, &box);
//
//    // We are now done writing to the display.  However, if we stopped here, we would not
//    // see any changes on the actual LCD.  This is because we need to send our changes
//    // to the LCD, which then refreshes the display.
//    // Since this is a slow operation, it is best to refresh (or "flush") only after
//    // we are done drawing everything we need.
//    Graphics_flushBuffer(&g_sContext);
//
//    dispThree[0] = ' ';
//    dispThree[2] = ' ';
//
//    while (1)    // Forever loop
//    {
//        // Check if any keys have been pressed on the 3x4 keypad
//        currKey = getKey();
//        if (currKey == '*')
//            BuzzerOn();
//        if (currKey == '#')
//            BuzzerOff();
//        if ((currKey >= '0') && (currKey <= '9'))
//            setLeds(currKey - 0x30);
//
//        if (currKey)
//        {
//            dispThree[1] = currKey;
//            // Draw the new character to the display
//            Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 48, 55, OPAQUE_TEXT);
//
//            // Refresh the display so it shows the new data
//            Graphics_flushBuffer(&g_sContext);
//
//            // wait awhile before clearing LEDs
//            swDelay(1);
//            setLeds(0);
//        }
//
//    }  // end while (1)*/
//}



//
//int main(void)
//{
//    unsigned int in_value = 0;
//  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//  // Configure P8.0 as digital IO output and set it to 1
//  // This supplied 3.3 volts across scroll wheel potentiometer
//  // See schematic at end or MSP-EXP430F5529 board users guide
//  P6SEL &= ~BIT0;
//  P6DIR |= BIT0;
//  P6OUT |= BIT0;
//  REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control
//                                            // internal reference voltages to
//                   // ADC12_A control registers
//  ADC12CTL0 = ADC12SHT0_9 | ADC12ON;
//
//  ADC12CTL1 = ADC12SHP;                     // Enable sample timer
//
//  // Use ADC12MEM0 register for conversion results
//  ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_0;   // ADC12INCH0 = Scroll wheel = A0
//                   // ACD12SREF_0 = Vref+ = Vcc
//  __delay_cycles(100);                      // delay to allow Ref to settle
//  ADC12CTL0 |= ADC12ENC;     // Enable conversion
//  _enable_interrupt();
//  while(1)
//  {
//    ADC12CTL0 &= ~ADC12SC;  // clear the start bit
//    ADC12CTL0 |= ADC12SC;               // Sampling and conversion start
//     // Single conversion (single channel)
//    // Poll busy bit waiting for conversion to complete
//    while (ADC12CTL1 & ADC12BUSY)
//     __no_operation();
//    in_value = ADC12MEM0;               // Read results if conversion done
//    __no_operation();                       // SET BREAKPOINT HERE
//  }
//}







//
//#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)
//#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)
//int main(void)
//{
//    unsigned int in_temp;
//    unsigned int in_temp_test;
//  volatile float temperatureDegC;
//  volatile float temperatureDegF;
//  volatile float degC_per_bit;
//  volatile unsigned int bits30, bits85;
//  WDTCTL = WDTPW + WDTHOLD;      // Stop WDT
//
//  REFCTL0 &= ~REFMSTR;    // Reset REFMSTR to hand over control of
//                          // internal reference voltages to
//           // ADC12_A control registers
//
//  ADC12CTL0 = ADC12SHT0_15 | ADC12REFON | ADC12ON;     // Internal ref = 1.5V
//
//  ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;                     // Enable sample timer
//
//  // Using ADC12MEM0 to store reading
//  ADC12MCTL0 = ADC12SREF_1 | ADC12INCH_10 ;  // ADC i/p ch A10 = temp sense
//  ADC12MCTL1 = ADC12SREF_0 | ADC12INCH_0 |ADC12EOS ;  // ADC i/p ch A10 = temp sense
//                                       // ACD12SREF_1 = internal ref = 1.5v
//  __delay_cycles(100);                    // delay to allow Ref to settle
//  ADC12CTL0 |= ADC12ENC;              // Enable conversion
//  // Use calibration data stored in info memory
//  bits30 = CALADC12_15V_30C;
//  bits85 = CALADC12_15V_85C;
//  degC_per_bit = ((float)(85.0 - 30.0))/((float)(bits85-bits30));
//  while(1)
//  {
//    ADC12CTL0 &= ~ADC12SC;  // clear the start bit
//    ADC12CTL0 |= ADC12SC;       // Sampling and conversion start
//             // Single conversion (single channel)
//    // Poll busy bit waiting for conversion to complete
////    while (ADC12CTL1 & ADC12BUSY)
////     __no_operation();
//    in_temp = ADC12MEM0;      // Read in results if conversion
//    in_temp_test = ADC12MEM1;
//    // Temperature in Celsius. See the Device Descriptor Table section in the
//    // System Resets, Interrupts, and Operating Modes, System Control Module
//    // chapter in the device user's guide for background information on the
//    // used formula.
//    temperatureDegC = (float)((long)in_temp - CALADC12_15V_30C) * degC_per_bit
//+30.0;
//    // Temperature in Fahrenheit Tf = (9/5)*Tc + 32
//    temperatureDegF = temperatureDegC * 9.0/5.0 + 32.0;
//
//    __no_operation();                       // SET BREAKPOINT HERE
//  }
//}

