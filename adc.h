/* 
 * File:   adc.h
 * Author: thetrung
 *
 * Created on April 21, 2025, 1:27 PM
 */
#ifndef ADC_H
#define	ADC_H
#include "commons.h"
#include "oled.h"
/**===========================
 * CONFIG  
 *===========================*/
#ifndef ANALOG_AMOUNT
#define ANALOG_AMOUNT 14
#endif
#define ROUND_SCAN 5
const char* TEXT_SCANNING = "Scanning.. ( %d / %d )";
/**===========================
 * FUNCTION IMPLEMENTATION 
 *===========================*/
void ADC_Init(void)
{    
//    TRISA = 0xFF;	/* Set as input port */
    ADCON0 = 0xFD;	/* Ref vtg is VDD and Configure pin as analog pin */
    ADCON1 = 0xC0;	/* (max=1011) Right Justified, 4Tad and Fosc/32. */
    ADRESH=0;		/* Flush ADC output Register */
    ADRESL=0;   
}
int ADC_Read(u8 channel)
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
u8 ADC_Discovery(void){
    // Initialize : 
    u8 count_active = 0;
    u8 analog_value[ANALOG_AMOUNT];
    bool analog_active[ANALOG_AMOUNT]; // 1-bit array (!?)
    
    // Filling empty value :
    for(u8 i = 0; i < ANALOG_AMOUNT;i++) analog_value[i] = 99;
    
    /**
     * TODO:
     * We should update the discovery algorithm :
     * - Scan each pin for N-rounds
     * - If any pin show unstable value -> Discard instantly
     * - Record stable pin as 1-bit in total 14-bit value 
     * - Return that.
     */
    
    // Scan all Analogs to automatically determine using amount :
    for(u8 i = 0; i < ANALOG_AMOUNT;i++){
        
        count_active += 1; // Update discovered analog channel.
        
        for(u8 round = 0; round < ROUND_SCAN; round++){
            u8 current = ADC_Read(i)/3;
            delay(2);
            
            // Update fresh values :
            if(analog_value[i] == 99){
                analog_value[i] = current; // 1st value.
                analog_active[i] = false;  // disabled by default.
            } 
            // test if value is stable after 1st round :
            else if(analog_value[i]== current){
                // OK! Next Round !
            }
            else if(round > 0){
                count_active -= 1;
                analog_active[i] = false;
                // stop scanning from here.
                break;
            }
        }
        delay(10);
        char* text_report[35];
        sprintf(text_report, TEXT_SCANNING , i, ANALOG_AMOUNT);
        OLED_Erase_H_Line(30, 120, 36); 
        OLED_Printf(text_report, 0, 36);
        OLED_Draw_Progressbar(0, 102, 48, i * 9);
    }
    return count_active;
}

#endif	/* ADC_H */

