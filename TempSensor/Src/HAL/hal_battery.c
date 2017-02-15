#include "hal_battery.h"

#define   Num_of_Results   9

static u16 results[Num_of_Results] = {0};
static u32 Average = 0;


u16 Battery_Adc_Get()
{
#if 0           //�ɼ�N�Σ����³���while��ѭ��
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
    Average -= results[0];                    //��ȥ��һ�鲻׼ȷ��
    Average >>= 3;                            //��8�βɼ���ƽ��ֵ
    ADC12CTL0 &= ~(ENC + ADC12SC);
    // ADC12CTL0 &= ~(ENC);
    ADC12CTL0 &= ~(ADC12ON+REFON+REF2_5V);
    P6SEL &= ~BIT0;
    BATTERY_POWER_OFF;
    return Average;
#endif
    
#if 1                   //�ɼ�һ�Σ�����while��ѭ��
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

    Temp = (average * 2.5) / 4095;              //�˴��ĳ�3����Ϊȥ���˵�Դת��оƬ��ֱ�ӽӵĵ�أ�����Ļ�2.5���з�ѹ  
    Temp = Temp * 2;                       //����2��ĵ�Դģ���ѹ(���ǵ�ص�ѹ)
    return Temp;
}


/*
*     ���һ�ε�ص���
*     ����ֵΪ��������ʵֵ
*/    
float Battery_Value_Get(void)
{
  u32    Temp;
  
  Temp = Battery_Adc_Get();                            //�ɼ���ת����ص�ѹֵ
  return Battery_Analog_Value(Temp);                   //������ʵֵ����
}
    



