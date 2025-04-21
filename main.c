/* NOTE on PIC16F887 ::
 * The conversion process from 45K50 encountered a lot of bummer :
 * 
 * - Different usage of "const" : 
 * Can no longer in arguments.
 * 
 * - Limited stack level : 
 * May lead to crash for no reason. Especially when 
 * I tried to call drawing function from another file.
 * 
 * - Configuration Bits:
 * Limited code-protection + debugging.
 *
 */
#include "main.h"
#include "adc.h"
#include "oled.h"

#define SSD1306_128_64
/*==============================================================================
 * Variables 
 *============================================================================*/
int AN0_prev;
int AN0_value;
bool wait_first = true;
bool is_cleared = false; // clear once :
/*==============================================================================
 * Main routine
 *  - Initialize system
 *  - Loop forever
 *============================================================================*/
void main(void) {
    
    init();
    delay(100);
    
    blink();       // indicate finished init()
      
    init_OLED();
    delay(100);
    
     OLED_Draw_H_Line(0, 127, 0);
     delay(10);
     OLED_Draw_H_Line(0, 127, 8);
     delay(10);
     OLED_Draw_H_Line(0, 127, 32);
     delay(10);
     OLED_Draw_H_Line(0, 127, 55);
     delay(10);
     OLED_Draw_H_Line(0, 127, 63);
     delay(10);
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
    OLED_Printf(" Hello World...Super Long "  ,0, 0);
    delay(10);
    OLED_Printf(" 01234567890 ~"  ,0, 9);
    delay(10);
    OLED_Printf(" QWERTYUIOP[]\\",0, 18);
    delay(10);
    OLED_Printf(" ASDFGHJKL:;  " ,0, 27);
    delay(10);
    OLED_Printf(" ZXCVBNM,./  "  ,0, 36);
    delay(10);
    OLED_Printf(" +-*/=!@#$%^&"  ,0, 45);
    delay(10);
    OLED_Printf(" NEWLINE "      ,0, 54);
    delay(10);
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
    ADC_Init();
    delay(10); //ms
   
    //Loop
    while(1) loop();
}
void blink(void){
    pinMode(RA1, OUTPUT);
    delay(10);
    digitalWrite(RA1, HIGH);
    delay(10);
    digitalWrite(RA1, LOW);
}
/*==============================================================================
 * Loop routine
 *============================================================================*/
void loop(void) {
    // blink = delay.
    blink();
    // update ADC:
    AN0_value = ADC_Read(0);
    delay(2);
    
    if(wait_first && AN0_value!=0) {
        AN0_prev=AN0_value;
        wait_first = false; // done.
    }
    if(AN0_value!=AN0_prev){
        if(!is_cleared){
            OLED_ClearDisplay();
            is_cleared = true;
            delay(10);
        }
        // Format Text :
        char text_ADC[10] = "ADC = XXX";
        sprintf(text_ADC, "ADC = %d", AN0_value);
        
        // Display Text Label :
        OLED_Printf(text_ADC, 0, 36);
        
        // & Progress Bar :
        draw_progressbar(0, 102, 48, (u8)AN0_value);
        
        // Update cached value for AN0:
        AN0_prev = AN0_value;
    }

// Test Bitmap drawing :
//   const u8 image[] = {
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
}