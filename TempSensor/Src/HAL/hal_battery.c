#include "hal_battery.h"

#define   Num_of_Results   9

static u16 results[Num_of_Results] = {0};
static u32 Average = 0;


u16 Battery_Adc_Get()
{
#if 0           //采集N次，害怕出现while死循环
    unsigned int i;
    BATTERY_POWER_ON;
    P6SEL |= BIT0;                            // Enable A/D channel A5
    ADC12CTL0 = ADC12ON+SHT0_4+REFON+REF2_5V; // Turn on and set up ADC12
    ADC12CTL1 = SHP;                          // Use sampling timer
    ADC12MCTL0 = SREF_1;                      // Vr+=Vref+

    for(i = 0; i < 0x3600; i++){}             // Delay for reference start-up
    for(i = 0; i < 9; i++)
    {
      ADC12CTL0 |= ADC12SC + ENC;             // Enable conversions
      while((ADC12IFG & BIT0) == 0);
      results[i] = ADC12MEM0; 
      Average += results[i];
    }
    Average -= results[0];                    //除去第一组不准确的
    Average >>= 3;                            //求8次采集的平均值
    ADC12CTL0 &= ~(ENC + ADC12SC);
    // ADC12CTL0 &= ~(ENC);
    ADC12CTL0 &= ~(ADC12ON+REFON+REF2_5V);
    P6SEL &= ~BIT0;
    BATTERY_POWER_OFF;
    return Average;
#endif
    
#if 1                   //采集一次，避免while死循环
    u16 results = 0;
    u16 i = 0;
    
    BATTERY_POWER_ON;
    Delay_Ms(100);
    P6SEL |= BIT0;                            // Enable A/D channel A5
    ADC12CTL0 = ADC12ON+SHT0_4+REFON+REF2_5V; // Turn on and set up ADC12
    ADC12CTL1 = SHP;                          // Use sampling timer
    ADC12MCTL0 = SREF_1;                      // Vr+=Vref+

    for(i = 0; i < 0x3600; i++){}             // Delay for reference start-up

    ADC12CTL0 |= ADC12SC + ENC;             // Enable conversions
    Delay_Ms(100);
    results = ADC12MEM0; 
    ADC12CTL0 &= ~(ENC + ADC12SC);
    ADC12CTL0 &= ~(ADC12ON+REFON+REF2_5V);
    P6SEL &= ~BIT0;
    BATTERY_POWER_OFF;
    return results;
#endif    
}

    
float Battery_Analog_Value(u32 average)
{
    
    float Temp;

    Temp = (average * 2.5) / 4095;              //此处改成3是因为去掉了电源转换芯片，直接接的电池，否则改回2.5进行分压  
    Temp = Temp * 2;                       //乘以2测的电源模块电压(不是电池电压)
    return Temp;
}


/*
*     检测一次电池电量
*     返回值为电量的真实值
*/    
float Battery_Value_Get(void)
{
  u32    Temp;
  
  Temp = Battery_Adc_Get();                            //采集并转换电池电压值
  return Battery_Analog_Value(Temp);                   //进行真实值计算
}
    



