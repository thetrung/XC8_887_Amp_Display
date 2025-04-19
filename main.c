/*
 * 20190312.014
 * OLED 128x32
 *
 * File: main.c
 * Processor: PIC16F1825
 * Author: wizlab.it
 */

#include "main.h"
#define SSD1306_128_64
#include "ssd1306_unbuffered.h"




int AN0_value;
int AN0_prev;
void ADC_Init(void)
{    
    TRISA = 0xFF;	/* Set as input port */
    ADCON0 = 0x0E;	/* Ref vtg is VDD and Configure pin as analog pin */
    ADCON1 = 0x92;	/* Right Justified, 4Tad and Fosc/32. */
    ADRESH=0;		/* Flush ADC output Register */
    ADRESL=0;   
}
int ADC_Read(int channel)
{
    int digital;

    /* Channel 0 is selected i.e.(CHS3CHS2CHS1CHS0=0000) & ADC is disabled */
    ADCON0 =(ADCON0 & 0b11000011)|((channel<<2) & 0b00111100);  
    
    ADCON0 |= ((1<<ADON)|(1<<GO));	/*Enable ADC and start conversion*/

    /* Wait for End of conversion i.e. Go/done'=0 conversion completed */
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);	/*Combine 8-bit LSB and 2-bit MSB*/
    return(digital/10);
}
/*==============================================================================
 * Main routine
 *  - Initialize system
 *  - Loop forever
 *============================================================================*/
void main(void) {
//    TRISD = 0x00;
    
    init();
    delay(1000);
    
    blink();       // indicate finished init()
    delay(1000);
      
    init_OLED();
    delay(1000);
    
     OLED_Draw_H_Line(0, 127, 0);
     delay(10);
     OLED_Draw_H_Line(0, 127, 8);
     delay(10);
 //    OLED_Draw_H_Line(0, 127, 16);
     OLED_Draw_H_Line(0, 127, 32);
     delay(10);
 //    OLED_Draw_H_Line(0, 127, 47);
     OLED_Draw_H_Line(0, 127, 55);
     delay(10);
     OLED_Draw_H_Line(0, 127, 63);
     delay(10);
 //    OLED_Draw_H_Line(0, 127, 2);
 //    OLED_Draw_H_Line(0, 127, 7);
 //    OLED_Draw_H_Line(0, 127, 8);
     OLED_Draw_V_Line(0, 0, 63);
     delay(10);
     OLED_Draw_V_Line(63, 0, 63);
     delay(10);
     OLED_Draw_V_Line(127, 0, 63);
     delay(10);
    
    /**
     * TEXT RENDERING TEST :
     * - 8 LINES at cost of 9 Pixels each.
     * - HEIGHT [0..9]
     * - WIDTH 5 
     */
//    OLED_Printf(" Hello World...Super Long "  ,0, 0);
//    delay(1);
//    OLED_Printf(" 01234567890 ~"  ,0, 9);
//    delay(1);
//    OLED_Printf(" QWERTYUIOP[]\\",0, 18);
//    delay(1);
//    OLED_Printf(" ASDFGHJKL:;  " ,0, 27);
//    delay(1);
//    OLED_Printf(" ZXCVBNM,./  "  ,0, 36);
//    delay(1);
//    OLED_Printf(" +-*/=!@#$%^&"  ,0, 45);
//    delay(1);
//    OLED_Printf(" NEWLINE "      ,0, 54);
//    delay(1);
    OLED_Printfi("        R E A D Y         "  ,0, 63);
    blink();
    delay(100);
//    OLED_StartScrollRight(0x00, 0x0F);
//    delay(2000);
//    OLED_StopScroll();
//    delay(1000);
//    OLED_StartScrollLeft(0x00, 0x0F);
//    delay(2000);
//    OLED_StopScroll();
//    delay(1000);    
//    OLED_StartScrollDiagRight(0x00, 0x07);
//    delay(2000);
//    OLED_StartScrollDiagLeft(0x00, 0x07);
//    delay(2000);
//    OLED_StopScroll();

//    ADC_Init();
    delay(10); //ms
    //Loop
    while(1) loop();
}

void blink(void){
    // Set pin RA0 as OUTPUT 
    pinMode(RA1, OUTPUT);
   
    delay(100);
    digitalWrite(RA1, HIGH);
    delay(100);
    digitalWrite(RA1, LOW);
}

//char text_ADC[12];
//bool wait_first = true;
//bool is_cleared = false; // clear once :
/*==============================================================================
 * Loop routine
 *============================================================================*/
void loop(void) {
    
    blink();
    
    delay(1000);
//    
//    AN0_value = ADC_Read(0);
//    if(wait_first && AN0_value!=0) {
//        AN0_prev=AN0_value;
//        wait_first = false; // done.
//    }
//    if(AN0_value!=AN0_prev){
//        if(!is_cleared){
//            OLED_ClearDisplay();
//            is_cleared = true;
//        }
//        sprintf(text_ADC, "ADC = %d", AN0_value);
//        OLED_Printf(text_ADC, 0, 36);
//        OLED_Erase_H_Line(AN0_value, 102, 48);
//        delay(1);
//        OLED_Draw_H_Line(0, AN0_value, 48);
//        AN0_prev = AN0_value;
//        delay(16);
//    }

    //Test
//   const ui8_t image[] = {
//        
//        0b00001111,
//        0b00001001,
//        0b00001001,
//        0b00001001,
//        0b00001001,
//        0b00001001,
//        0b00001001,
//        0b00001001,
//        0b00001001,
//        0b00001001,
//        0b00001111,
//        
//   };
//   OLED_DrawBitmap(0, 0, 0, 127, image, sizeof(image));
   //Invert
//    OLED_InvertDisplay(invert);
//    invert++;
//    if(invert > 0x01) invert = 0x00;
}