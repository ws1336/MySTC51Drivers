#include "AD12C5A.h"

    
/*----------------------------
Get ADC result
----------------------------*/
u8 GetADCResult(u8 ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEED | ch | ADC_START;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //Return ADC result
}


/*----------------------------
Initial ADC sfr
----------------------------*/
void InitADC()
{
    P1ASF = ADC_IO;                 //Open 8 channels ADC function
    ADC_RES = 0;                    //Clear previous result
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    delay_ms(20);                   //ADC power-on and delay
}