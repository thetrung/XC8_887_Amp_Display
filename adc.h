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
#define ROUND_SCAN 10
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
    u8 round_result;
    u8 analog_determined;
    u8 analog_value[ANALOG_AMOUNT];
    
    // Filling empty value :
    for(u8 i = 0; i < ANALOG_AMOUNT;i++) analog_value[i] = 99;
    
    // Scan all Analogs to automatically determine using amount :
    for(u8 round = 0; round < ROUND_SCAN; round++){
        for(u8 i = 0; i < ANALOG_AMOUNT;i++){
            u8 current = ADC_Read(i)/3;
            delay(2);
            
            // Update discovered analog channel :
            analog_determined = i;

            // Update fresh values :
            if(analog_value[i] == 99){
                analog_value[i] = current;
            } 
            // test if value is stable after 1st round :
            else if(analog_value[i]== current){
                // OK !
            }
            else if(round > 0){
                // stop scanning from here.
                break;
            }
        }
        delay(100);
        char* text_report[35];
        sprintf(text_report, TEXT_SCANNING , analog_determined, ANALOG_AMOUNT);
        OLED_Erase_H_Line(30, 120, 36); 
        OLED_Printf(text_report, 0, 36);
        OLED_Draw_Progressbar(0, 102, 48, (round+1) * 10);
        // accumulate Round :
        if(round > 0)  // Discard 1st two rounds :
        round_result += analog_determined;
    }
    // Average result :
    round_result /= (ROUND_SCAN-1);
    return round_result;
}

#endif	/* ADC_H */

