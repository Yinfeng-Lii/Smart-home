#include "gp2y1014au.h"

/**
  * @brief  GP2Y��ʼ������
  * @param  ��
  * @retval ��
  */
void GP2Y_Init(void)
{
	//����GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	//PA0 ��Ϊ�����������
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GP2Y_LED;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GP2Y_LED_PROT, &GPIO_InitStructure);
	GP2Y_LED_High;
	
	//PA1��Ϊģ��ͨ���������� 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_InitStructure.GPIO_Pin = GP2Y_VO;
	GPIO_Init(GP2Y_VO_PROT, &GPIO_InitStructure);
}

/**
  * @brief  GP2Y ADC���ݶ�ȡ
  * @param  ��
  * @retval ����ת�����GP2Yģ���ź���ֵ
  */
uint32_t GP2Y_ADC_Read(void)
{
	//����ָ��ADC�Ĺ�����ͨ��������ʱ��
	return ADC_GetValue(ADC_Channel_1, ADC_SampleTime_239Cycles5);
}

/**
  * @brief  GP2Y ug/m3���ݶ�ȡ
  * @param  ��
  * @retval ����ת�����ug/m3����
  */
u16 Get_GP2Y_PM25(void)
{
	uint16_t ADCVal;
	int dustVal = 0;
	float Voltage;

	GP2Y_LED_High;	//��1  �����ڲ�LED
	Delay_us(280); 	// ����LED���280us�ĵȴ�ʱ��
	ADCVal = GP2Y_ADC_Read();  //PA1 ��������ȡADֵ
	Delay_us(19);			  //��ʱ19us����Ϊ����AD����������Ϊ239.5������ADת��һ�����ʱ21us��19��21�ټ�280�պ���320us
	GP2Y_LED_Low;	//��0  �ر��ڲ�LED
	Delay_us(9680);			//��Ҫ�����0.32ms/10ms��PWM�ź������������е�LED
	
	Voltage = 3.3f * ADCVal / 4096.f * 2; //���AO����ڵĵ�ѹֵ
	
	dustVal = (0.17*Voltage-0.1)*1000;  //����1000��λ����ug/m3//
//	dustVal = 170*(Voltage/100)-0.1;

	if (dustVal < 0)
		dustVal = 0;            //��λ//

	if (dustVal>500)        
		dustVal=500;

	return dustVal;
}


/**
  * @brief  ƽ��ֵ�˲���
  * @param  ��
  * @retval �����˲��������
  */
u16 Get_PM25_Average_Data(void)
{
	u16 temp_val=0;
	u8 t;
	for(t=0;t<PM25_READ_TIMES;t++)	//#define PM25_READ_TIMES	20	�����ȡ����,����ô���,Ȼ��ȡƽ��ֵ
 
	{
		temp_val+=Get_GP2Y_PM25();	//��ȡADCֵ
		Delay_ms(5);
	}
	temp_val/=PM25_READ_TIMES;//�õ�ƽ��ֵ
    return (u16)temp_val;//���������ADCƽ��ֵ
}
