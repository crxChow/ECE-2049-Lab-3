#include <msp430.h>
#include "peripherals.h"

long long int intervals;
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
enum Editing currEdit = month;
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
 struct time CounterToDate(long long int Time){
     struct time output = {.month = January, .day = 0, .hour = 0, .seconds = 0, .minute = 0};
     long long int days = Time/86400;
     long long int secondsRemainingInDay = Time%86400;
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
     long long int test = (secondsRemainingInDay/3600);
     output.hour = (char)test; //hours are based on how many seconds we have //left in the day
     long long int test1 = (secondsRemainingInDay%3600)/60;
     output.minute = (char)test1; //knowing how many hours find the //number of minutes into the hour
     long long int test2 = secondsRemainingInDay-output.hour*3600-output.minute*60;
     output.seconds = (char)test2;//knowing how //many minutes fine the number of seconds we are into the day
 return output;
 }

 long long int calcTime(struct time editTime){
     long long int ret_val = 0;
     ret_val += (long long int)(editTime.seconds);
     ret_val += (long long int)(editTime.minute*60);
     long long int hour = (long long int)editTime.hour;
     long long int secondsperhour = 3600;
     ret_val += hour*secondsperhour;
     ret_val += (long long int)((editTime.day-1)*86400);
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
 void displayTime(long int inTime){
     struct time newTime = CounterToDate(inTime);
     int seconds = newTime.seconds;
     int minutes = newTime.minute;
     int hours = newTime.hour;
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


     Graphics_drawStringCentered(&g_sContext, Time, 10, 30, 35,TRANSPARENT_TEXT);


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
                currEdit = month;
                editTime = CounterToDate(intervals);
            }
          }
      }

}
Graphics_Rectangle Month_box = {.xMin = 4, .xMax = 25, .yMin = 15, .yMax = 25 };
Graphics_Rectangle Day_box = {.xMin = 27, .xMax = 42, .yMin = 15, .yMax = 25 };

int LastButtonPressedStatus = 0;
#pragma vector = TIMER2_A0_VECTOR
interrupt void A2TimerINTR(void)
{
    if(state == Timing)
    {
        intervals += 1;
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
    if(average<10){
        displayTemp(temperatureDegC);
    }
    else{
        displayTemp(average);
    }



    if(state == Editing){
        int buttonStatus = ReadButtons();
        if(currEdit == month){
            editTime.month = (int)(Scroll_Wheel/345);

            Graphics_drawRectangle(&g_sContext, &Month_box);
            if(buttonStatus == 1 & LastButtonPressedStatus != 1){
                currEdit++;
            }
        }
        else if(currEdit == day){
            Graphics_drawRectangle(&g_sContext, &Day_box);
            if(editTime.month == September ||  editTime.month == April || editTime.month == June|| editTime.month == November){
                editTime.day = (int)(Scroll_Wheel/140)+1; //30 days
            }
            else if(editTime.month == February){
                editTime.day = (int)(Scroll_Wheel/146)+1; //28 days
            }
            else{
                editTime.day = (int)(Scroll_Wheel/135)+1; //31 days
            }
            if(buttonStatus == 1 &  LastButtonPressedStatus != 1){
                currEdit++;
            }
        }
        else if(currEdit == hour){
            Graphics_drawLineH(&g_sContext, 4,19, 40);
            editTime.hour = (int)(Scroll_Wheel/171);
            if(buttonStatus == 1 &  LastButtonPressedStatus != 1){
                currEdit++;
            }
        }
        else if(currEdit == minute){
            Graphics_drawLineH(&g_sContext, 22, 39, 40);
            editTime.minute = (int)(Scroll_Wheel/69);
            if(buttonStatus == 1 &  LastButtonPressedStatus != 1){
                currEdit++;
            }
        }
        else if(currEdit == second){
            Graphics_drawLineH(&g_sContext, 42, 57, 40);
            editTime.seconds = (int)(Scroll_Wheel/69);
            if(buttonStatus == 1 &  LastButtonPressedStatus != 1){
                currEdit = month;
            }
        }
        LastButtonPressedStatus = buttonStatus;
        displayTimeStruct(editTime);

        if(buttonStatus == 2){
            intervals = calcTime(editTime);
            state = Timing;
        }
    }
    else{
        editTime = CounterToDate(intervals);
        displayTimeStruct(editTime);
    }
    Graphics_flushBuffer(&g_sContext);
}

