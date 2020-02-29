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
  ad = ad0conv(3); //read analog signal to ad
  ad = ad >> 1;  
  if (countTimes >= 80){  //reset counter after 1 millisecond
    freq = counter;
    counter = 0;
    countTimes = 0;
  } else {
     
    if (ad >= 265 && wait == 0){   //wait until signal goes back low
      counter++;       //count when signal is over 265
      wait = 1;
    }else{
      if(ad < 265 && wait == 1){   //when signal goes low clear wait flag
        wait = 0; 
      }
    }
  countTimes++;        //count times of 1ms rti
  }
 
  clear_RTI_flag();
}

void main(void) {
  char col, row;
  int i;
  PLL_init();        // set system clock frequency to 24 MHz 
  DDRB  = 0xff;       // Port B is output
  DDRJ  = 0xff;       // Port J is output
  DDRP  = 0xff;       // Port P is output
  PTJ = 0x00;         // enable LED
  ad0_enable();
  PTP = 0x00;         // enable all 7-segment displays
  RTI_init();         //enable RTI
  lcd_init();
  SPI0_init();
  wait = 0;
  
  for (i = 0; i < 8; i++){
    PORTB |= leds[i];
    ms_delay(100);
  }
          
  while(1){
  
     
  
     row = columnHeight();  //turn rows on according to volume
     col = freqLed();       //turn columns on according to the frequency
     
     set_lcd_addr(0x00);
     write_int_lcd(row);
           
     set_lcd_addr(0x40);
     write_int_lcd(col);
     
     set_lcd_addr(0x48);
     write_int_lcd(ad);
     
     set_lcd_addr(0x08);
     write_int_lcd(freq);
    
     
     
     SS0_LO();
     send_SPI0(~col);        //send columns out SPI
     send_SPI0(row);        //send row out SPI
     SS0_HI();
     
  }
}

int freqLed(){
  if(freq >=1){
    
  if(freq >= 1 && freq < 2){      //returns LED# according to frequency range
    return leds[0];
  }
  if(freq >= 2 && freq < 3){
    return leds[1];
  }
  if(freq >= 3 && freq < 4){
    return leds[2];
  }
  if(freq >= 4 && freq < 5){
    return leds[3];
  }
  if(freq >= 5 && freq < 10){
    return leds[4];
  }
  if(freq >= 10 && freq < 17){
    return leds[5];
  }
  if(freq >= 17 && freq < 25){
    return leds[6];
  }
  if(freq >= 25 && freq < 35){
    return leds[7];
  }
  }
  else{
    return 0; //turn off if no frequency
  }
}

//returns the column height led value according to the volume of sound
int columnHeight(){
  if (wait == 0){
    if (ad >= 265){          //if sound is louder than LED value
      column |= leds[0];     //column gets LED value
      adHighTimes++;
      countTimes2++;
    }else{
      if(countTimes2 >= 80){  //if 1ms has passed 
        if(adHighTimes = 0){  //if AD has not gone HIGH in 1 ms
          column &= ~leds[0];  //turn off LED in the column
        }
        countTimes2 = 0;
        adHighTimes = 0;
      }
    }
    if (ad >= 280){          //if sound is louder than LED value
      column |= leds[1];     //column gets LED value
      adHighTimes++;
      countTimes2++;
    }else{
      if(countTimes2 >= 80){  //if 1ms has passed 
        if(adHighTimes = 0){  //if AD has not gone HIGH in 1 ms
          column &= ~leds[1];  //turn off LED in the column
        }
        countTimes2 = 0;
        adHighTimes = 0;
      }
    }
    if (ad >= 295){          //if sound is louder than LED value
      column |= leds[2];     //column gets LED value
      adHighTimes++;
      countTimes2++;
    }else{
      if(countTimes2 >= 80){  //if 1ms has passed 
        if(adHighTimes = 0){  //if AD has not gone HIGH in 1 ms
          column &= ~leds[2];  //turn off LED in the column
        }
        countTimes2 = 0;
        adHighTimes = 0;
      }
    }
    if (ad >= 310){          //if sound is louder than LED value
      column |= leds[3];     //column gets LED value
      adHighTimes++;
      countTimes2++;
    }else{
      if(countTimes2 >= 80){  //if 1ms has passed 
        if(adHighTimes = 0){  //if AD has not gone HIGH in 1 ms
          column &= ~leds[3];  //turn off LED in the column
        }
        countTimes2 = 0;
        adHighTimes = 0;
      }
    }
    if (ad >= 325){          //if sound is louder than LED value
      column |= leds[4];     //column gets LED value
      adHighTimes++;
      countTimes2++;
    }else{
      if(countTimes2 >= 80){  //if 1ms has passed 
        if(adHighTimes = 0){  //if AD has not gone HIGH in 1 ms
          column &= ~leds[4];  //turn off LED in the column
        }
        countTimes2 = 0;
        adHighTimes = 0;
      }
    }
    if (ad >= 340){          //if sound is louder than LED value
      column |= leds[5];     //column gets LED value
      adHighTimes++;
      countTimes2++;
    }else{
      if(countTimes2 >= 80){  //if 1ms has passed 
        if(adHighTimes = 0){  //if AD has not gone HIGH in 1 ms
          column &= ~leds[5];  //turn off LED in the column
        }
        countTimes2 = 0;
        adHighTimes = 0;
      }
    }
    if (ad >= 355){          //if sound is louder than LED value
      column |= leds[6];     //column gets LED value
      adHighTimes++;
      countTimes2++;
    }else{
      if(countTimes2 >= 80){  //if 1ms has passed 
        if(adHighTimes = 0){  //if AD has not gone HIGH in 1 ms
          column &= ~leds[6];  //turn off LED in the column
        }
        countTimes2 = 0;
        adHighTimes = 0;
      }
    }
    if (ad >= 370){          //if sound is louder than LED value
      column |= leds[7];     //column gets LED value
      adHighTimes++;
      countTimes2++;
    }else{
      if(countTimes2 >= 80){  //if 1ms has passed 
        if(adHighTimes = 0){  //if AD has not gone HIGH in 1 ms
          column &= ~leds[7];  //turn off LED in the column
        }
        countTimes2 = 0;
        adHighTimes = 0;
      }
    }
    }
  return column;
}
