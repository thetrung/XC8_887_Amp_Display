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
#define SSD1306_128_64
#define ANALOG_AMOUNT 14 // Pins
/*==============================================================================
 * Variables 
 *============================================================================*/
u8 analog_current = 0;
u8 analog_cache[ANALOG_AMOUNT] = {};
u8 analog_value[ANALOG_AMOUNT] = {};
const char* analog_names[ANALOG_AMOUNT] = {
    " T R E B L E ", 
    " B A S S ",
    " B O O S T ", 
    " E C H O ", 
    " D E L A Y ", 
    " R E V R B ", 
    " T R E B L E ", 
    " T R E B L E ", 
    " T R E B L E ", 
    " T R E B L E ", 
    " T R E B L E ", 
    " T R E B L E ", 
    " T R E B L E ", 
    " T R E B L E "
}; // Pretending this was actual 14 Names.
const char* TEXT_NO_ANALOG = "No Analog. Restart Now.";
const char* TEXT_FOUND_CHANNEL = "Found %d channels.";
const char* TEXT_ADC_VALUE = "ADC ( %d ) = %d";
/*==============================================================================
 * FLAGS
 *==============================================================================*/  
bool wait_first = true;
bool is_cleared = false; // clear once :
/*==============================================================================
 * System initialization
 *============================================================================*/
void init(void) {
    /*
    * Init Oscillator: 16MHz
    *  - SPLLEN[7]: 0 (4x PLL enabled)
    *  - IRCF[6-4]: 111 (8MHz/32MHz)
    *  - SCS[1-0]: 00 (Clock by FOSC [FOSC = INTOSC])
    */
//    OSCCON = 0x00; // 0b0000 0000
//    OSCCONbits.IRCF = 0b111; // IRCF = 0b111 => 8 Mhz
//    OSCCONbits.SCS = 0b11; // using external high-speed crystal @ 16Mhz
    OSCCONbits.SCS = 0b00;  // Select OSC source = FOSC.
    /*
    * Initialize I2C bus
    */
    i2c_init(400000);

    /*
    * Interrupts
    * TMR0IE: 0 (Timer0 interrupt disabled)
    * SSP1 (I2C): 0 (SSP1 interrupt initially disabled)
    * PEIE: 1 (Peripheral interrupts disabled)
    * GIE: 1 (Global interrupts disabled)
    */
    TMR0IE = 0;
    SSPIE = 0;
    PEIE = 0;
    GIE = 0;
    
    PORTA = 0xFF; // enable all Port A I/O.
}
void demo_rendering(void){
    
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
    OLED_StartScrollRight(0x00, 0x0F);
    delay(2000);
    OLED_StopScroll();
    delay(1000);
    OLED_StartScrollLeft(0x00, 0x0F);
    delay(2000);
    OLED_StopScroll();
    delay(1000);    
    OLED_StartScrollDiagRight(0x00, 0x07);
    delay(2000);
    OLED_StartScrollDiagLeft(0x00, 0x07);
    delay(2000);
    OLED_StopScroll();
}
/*==============================================================================
 * Interrupt Service Routine
 *============================================================================*/
void __interrupt() isr(void) {
}
/*==============================================================================
 * Main routine
 *  - Initialize system
 *  - Loop forever
 *============================================================================*/
void main(void) {
    
    init();
    blink();       // = delay
      
    init_OLED();
    blink();       // = delay

//    demo_rendering();
//    blink();       // = delay
    
    OLED_Printfi("        R E A D Y         "  ,0, 36);
    blink();
    delay(1000);
    
    ADC_Init();
    blink();       // = delay
    
    OLED_ClearDisplay();
    
    // Scan ADC channels :
    u8 analog_discovered = ADC_Discovery();
    
    // Format Text :
    char text_report[20];
    
    // If found none channels :
    if(analog_discovered == 0){
        sprintf(text_report, TEXT_NO_ANALOG);
    } 
    else { 
        sprintf(text_report, TEXT_FOUND_CHANNEL, analog_discovered);
    }
    OLED_Erase_H_Line(35, 120, 36); // erase half-line 
    OLED_Printf(text_report, 0, 36);
    blink();
    if(analog_discovered > 0) delay(1000); else delay(5000);
   /*==============================================================================
    *  Loop routine
    *============================================================================*/
    if(analog_discovered > 0)
    while(1) {
      // blink = delay.
        blink();
        // update ADC:
        for(u8 i = 0; i < analog_discovered;i++){
            analog_value[i] = ADC_Read(i)/3;
            /** NOTE :
             * A simple division will help with
             * noisy value smoothening...
             */
            if(wait_first && analog_value[i]!=0) {
                analog_cache[i]=analog_value[i];
                analog_current = 99; // force update label 1st time.
                wait_first = false;  // done.
            }
            if(analog_value[i]!=analog_cache[i]){
                if(!is_cleared){
                    OLED_ClearDisplay();
                    is_cleared = true;
                    delay(10);
                }
                // Format Text : Compiler is stupid enough to not know array size 
                char text_ADC[15];
                sprintf(text_ADC, TEXT_ADC_VALUE, i, analog_value[i]);

                // Erase previous Analog name :
                if(analog_current!=i){
                    OLED_Erase_H_Line(0, 128/2, 36);     // Erase old Text Label
                    OLED_Printf(analog_names[i], 0, 36); // Display Text Label
                }
                OLED_Erase_H_Line(30, 75 , 17);    // Clear Index + Value
                OLED_Printf(text_ADC, 0, 17);      // Display current value

                // & Progress Bar :
                OLED_Draw_Progressbar(0, 102, 48, (u8)analog_value[i] * 3);

                // Cache value for Analog Input:
                analog_cache[i] = analog_value[i];
                analog_current = i;
            }
        }
        delay(1);
        
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
}
void blink(void){
    pinMode(RA4, OUTPUT);
    delay(10);
    digitalWrite(RA4, HIGH);
    delay(10);
    digitalWrite(RA4, LOW);
}