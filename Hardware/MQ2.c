#include "mq2.h"

/**
  * @brief  MQ2��ʼ������
  * @param  ��
  * @retval ��
  */
void MQ2_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA��ʱ��
	
	//PA4 ��Ϊģ��ͨ���������� 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = MQ2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MQ2_PROT, &GPIO_InitStructure);
}

/**
  * @brief  MQ2 ADC���ݶ�ȡ
  * @param  ��
  * @retval ����ת�����MQ2ģ���ź���ֵ
  */
uint32_t MQ2_ADC_Read(void)
{
	//����ָ��ADC�Ĺ�����ͨ��������ʱ��
	return ADC_GetValue(ADC_Channel_4, ADC_SampleTime_55Cycles5);
}

/**
  * @brief  ��ȡMQ2ģ���ź�ƽ��ֵ
  * @param  ��
  * @retval ���������MQ2ģ���ź�ƽ��ֵ
  */
u16 MQ2_Average_Data(void)
{
	u16 temp_val=0;
	u8 t;
	for(t=0;t<SMOG_READ_TIMES;t++)	//#define SMOG_READ_TIMES	10	��������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ
 
	{
		temp_val+=MQ2_ADC_Read();	//��ȡADCֵ
		Delay_ms(5);
	}
	temp_val/=SMOG_READ_TIMES;//�õ�ƽ��ֵ
    return (u16)temp_val;//���������ADCƽ��ֵ
}

/**
  * @brief  ��ȡMQ2�ĵ�ѹ��ֵ
  * @param  ��
  * @retval ���������MQ2��ѹ��ֵ
  */
float MQ2_Voltage_Data(void)
{
	u16 adc_value = 0;//���Ǵ�MQ-2������ģ���ѹ�����ADCת���л�õ�ԭʼ����ֵ����ֵ�ķ�ΧΪ0��4095����ģ���ѹ��ʾΪ����ֵ
	float voltage = 0;//MQ-2������ģ��ĵ�ѹ���
	
	adc_value = MQ2_Average_Data();
	Delay_ms(5);
	
    voltage  = ((3.3f/4096.f)*(adc_value)) * 2;
	
	return voltage;
}

/*********************
// ������У׼���������ݵ�ǰ����PPMֵ���õ�RS��ѹֵ�����Ƴ�R0ֵ��
// �ڿ��������й�����R0Ϊ35
float MQ2_PPM_Calibration()
{
	float RS = 0;
	float R0 = 0;
	RS = (3.3f - Smog_Get_Vol()) / Smog_Get_Vol() * RL;//RL	10  // RL��ֵ
	R0 = RS / pow(CAL_PPM / 98.322, 1 / -1.458f);//CAL_PPM  10  // У׼������PPMֵ
	return R0;
}
**********************/

/**
  * @brief  ��ȡMQ2��PPM��ֵ
  * @param  ��
  * @retval ���������MQ2 PPM��ֵ
  */
float Smog_GetPPM(void)	//���㷨�ó���ppm��ֵ��ﵽ����������ʾ��������ݲ�ʹ�á�
{
//	float RS = (3.3f - MQ2_Voltage_Data()) / MQ2_Voltage_Data() * RL;
//	float ppm = 98.322f * pow(RS/R0, -1.458f);
	float ppm = MQ2_Voltage_Data() * 100;
	return  ppm;
}

