/* 
 * File:   adc.h
 * Author: thetrung
 *
 * Created on April 21, 2025, 1:27 PM
 */

#ifndef ADC_H
#define	ADC_H
#include "commons.h"
/**===========================
 * FUNCTION DECLARATION 
 *===========================*/
void ADC_Init(void);
int ADC_Read(int channel);

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


#endif	/* ADC_H */

