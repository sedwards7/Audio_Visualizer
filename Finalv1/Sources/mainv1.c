// Example 1a: Turn on every other segment on 7-seg display
#include <hidef.h>      /* common defines and macros */
#include <mc9s12dg256.h>     /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12dg256b"

#include "main_asm.h" /* interface to the assembly module */

unsigned int wait, counter, ad, countTimes, countTimes2, freq, adHighTimes, column;

unsigned int leds[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

int freqLed();
int columnHeight();

void interrupt 7 handler(){ 
  if (countTimes < 80){  //stops until main runs through
    
  ad = ad0conv(3); //read analog signal to ad
  ad = ad >> 1;
     
  if (ad >= 265 && wait == 0){   //wait until signal goes back low
    counter++;       //count when signal is over 265
    wait = 1;
  }  
    
  if(ad < 265 && wait == 1){   //when signal goes low clear wait flag
    wait = 0; 
  }
    
  countTimes++;
  countTimes2++;
  }
  clear_RTI_flag();
}

void main(void) {
  unsigned char col, row;
  char* string;
  char* hz;
  PLL_init();        // set system clock frequency to 24 MHz 
  ad0_enable();       //enable A/D converter
  SW_enable();
  PTP = 0x00;         // enable all 7-segment displays
  RTI_init();         //enable RTI
  lcd_init();         //enable the lcd
  SPI0_init();        //enable the spi to communicate with the led board
  wait = 0;           //wait is initially 0
  
  
  string = "Frequency:";
  hz = "hz";
          
  while(1){
  
    if ((PTH & 0x01) == 0){   //display frequency when switch 5 is pressed
     set_lcd_addr(0x00);
     type_lcd(string);
     set_lcd_addr(0x40);
     write_int_lcd(freq*100);
     set_lcd_addr(0x45);
     type_lcd(hz);
     
     countTimes2 = 0;
    }
    
    if (countTimes >= 80){
     freq = counter;        //the counter is the frequency for 1ms intervals
     
     row = columnHeight();  //set rows according to volume
     col = freqLed();       //set columns according to the frequency
     
     SS0_LO();               
     send_SPI0(~row);       //send columns out SPI
     send_SPI0(col);        //send row out SPI
     SS0_HI();              //pulse the clock to shift information to 595 shift registers
     
     counter = 0;
     countTimes = 0;
     
    }
  }
}
//returns LED# according to frequency range
int freqLed(){
  if(freq >=1){
    
  if(freq >= 1 && freq < 5){            //bewteen 100hz and 500hz
    return leds[0];
  } else if(freq >= 5 && freq < 7){     //bewteen 500hz and 700hz
    return leds[1];
  } else if(freq >= 7 && freq < 11){    //bewteen 700hz and 1100hz
    return leds[2];
  } else if(freq >= 11 && freq < 17){   //bewteen 1100hz and 1700hz
    return leds[3];
  } else if(freq >= 17 && freq < 21){   //bewteen 1700hz and 2100hz
    return leds[4];
  } else if(freq >= 21 && freq < 25){   //bewteen 2100hz and 2500hz
    return leds[5];
  } else if(freq >= 25 && freq < 30){   //bewteen 2500hz and 3000hz
    return leds[6];
  } else if(freq >= 30 && freq < 35){   //bewteen 3000hz and 3500hz
    return leds[7];
  }
  }
  else{
    return 0; //turn off if no frequency
  }
}

//returns the column height led value according to the volume of sound
int columnHeight(){
  int i;
  int frq = 265;
  for (i = 0; i < 8; i++){
    if (ad >= frq){       //if wave is above threshold
        column |= leds[i];     //columns get the LED value
       adHighTimes |= leds[i];
   }else{
     if(countTimes >= 80){    //check every 1ms
       if((adHighTimes & leds[i]) == 0){  //if mic has been silent for 1ms
         column &= ~leds[i];   //turn off led
       }
       adHighTimes = 0;
      }
   }
    frq += 15;
  }
  column &= 0xff;
  return column;
}
