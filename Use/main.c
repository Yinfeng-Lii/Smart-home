#include "stm32f10x.h"                  // Device header
#include "gizwits_product.h"
#include "oled.h"
#include "menu.h"
#include "dht11.h"
#include "motor.h"
#include "key.h"
#include "buzzer.h"
#include "flash.h"
#include "usart.h"
#include "sys.h"
#include "relay.h"
#include "tim3.h"
#include "tim2.h"
#include "iwdg.h"
#include "mq2.h"
#include "gp2y1014au.h"

#define FLASH_START_ADDR	0x0801f000	//д�����ʼ��ַ

u8 Menu = 1;	//�˵�����

u8 Model_Flag;	//ϵͳģʽ��־λ
u8 Window_Flag_ON = 0;	//������־λ
u8 Window_Flag_OFF = 0;	//�ش���־λ

u8 Fan_Flag;			// ����״̬��־λ
u8 Humidifier_Flag;		// ��ʪ��״̬��־λ
u8 Window_Flag;			// ����״̬��־λ

u8 Temp_value,Humi_value;	//�¶���ֵ��ʪ����ֵ
u16 Smog_value,PM25_value;	//������ֵ��PM2.5��ֵ
u8 old_Temp_value,old_Humi_value;	//�ɵ�������ֵ��PM2.5��ֵ
u16 old_Smog_value,old_PM25_value;	//�ɵ�������ֵ��PM2.5��ֵ

u8 value_Option_Num = 1;	//��ֵ���ڽ���λ��ѡ�����
uint8_t gizwitsModeFlag;	// ������־λ


void scanGizwitsModeKey(void)
{
	if (GPIO_ReadInputDataBit(KEY_PROT, KEY3_GPIO_PIN) == 0)
	{
		Delay_ms(20);
		if (GPIO_ReadInputDataBit(KEY_PROT, KEY3_GPIO_PIN) == 0)
		{	
			Menu = 4;
			gizwitsModeFlag = 1;
			Buzzer_ON();
		}
	}
	else if(GPIO_ReadInputDataBit(KEY_PROT, KEY4_GPIO_PIN) == 0)
	{
		Delay_ms(20);
		if (GPIO_ReadInputDataBit(KEY_PROT, KEY4_GPIO_PIN) == 0)
		{	
			Menu = 4;
			gizwitsModeFlag = 2;
			Buzzer_ON();
		}			
	}
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	ADCX_Init();
	GP2Y_Init();
	MQ2_Init();
	DHT11_UserConfig();
	MOTOR_UserConfig();
	Key_Init();
	Relay_Init();
	Buzzer_Init();
	
	Temp_value = FLASH_R(FLASH_START_ADDR);	//��ָ��ҳ�ĵ�ַ��FLASH
	Humi_value = FLASH_R(FLASH_START_ADDR+2);	//��ָ��ҳ�ĵ�ַ��FLASH
	Smog_value = FLASH_R(FLASH_START_ADDR+4);	//��ָ��ҳ�ĵ�ַ��FLASH
	PM25_value = FLASH_R(FLASH_START_ADDR+6);	//��ָ��ҳ�ĵ�ַ��FLASH
	
	Timer2_Init(9,14398);
	uart2_init(9600);
	uart1_init(115200);
	
	GENERAL_TIM_Init();
	userInit();		//��ɻ����Ƴ�ʼ��ֵ
	gizwitsInit();	//����һ�����λ�����
	
	IWDG_Init();	//��ʼ�����Ź�
	Delay_ms(1000);
	scanGizwitsModeKey();
	while (1)
	{
		do
		{
			currentDataPoint.valueTemp_value = Temp_value;	//��ָ��ҳ�ĵ�ַ��FLASH
			currentDataPoint.valueHumi_value = Humi_value;	//��ָ��ҳ�ĵ�ַ��FLASH
			currentDataPoint.valueSmog_value = Smog_value;	//��ָ��ҳ�ĵ�ַ��FLASH
			currentDataPoint.valuePM25_value = PM25_value;	//��ָ��ҳ�ĵ�ַ��FLASH		
		}while(0);		
		
		IWDG_ReloadCounter(); //���¼��ؼ���ֵ ι��

		switch(Menu)
		{
			case 1:	
				Menu1();
			
				/* ��������ϵͳģʽ */
				if (KeyNum == 1)
				{
					KeyNum = 0;
					if (!Model_Flag)
					{
						Model_Flag = 1;
					}
					else
					{
						Model_Flag = 0;							
					}
					Buzzer_OFF();
					Relay_OFF(1);
					Relay_OFF(2);
					Fan_Flag = 0;
					Humidifier_Flag = 0;
				}
				
			
				/* �ֶ�ģʽ�°������� */
				if (!Model_Flag)
				{
					/* ���Ʒ��ȵ����� */
					if (KeyNum == 2 && !Fan_Flag)
					{
						KeyNum = 0;
						Fan_Flag = 1;
						Relay_ON(1);
						
					}
					else if (KeyNum == 2 && Fan_Flag)
					{
						KeyNum = 0;
						Fan_Flag = 0;
						Relay_OFF(1);
						
					}
					
					/* ���Ƽ�ʪ�������� */
					if (KeyNum == 3 && !Humidifier_Flag)
					{
						KeyNum = 0;
						Humidifier_Flag = 1;
						Relay_ON(2);
						
					}
					else if (KeyNum == 3 && Humidifier_Flag)
					{
						KeyNum = 0;
						Humidifier_Flag = 0;
						Relay_OFF(2);
						
					}

					/* ���ƴ��������� */
					if (KeyNum == 4 && !Window_Flag)
					{
						KeyNum = 0;
						Window_Flag = 1;
						Window_Flag_ON = 1;
						
					}
					else if (KeyNum == 4 && Window_Flag)
					{
						KeyNum = 0;
						Window_Flag = 0;
						Window_Flag_OFF = 1;
						
					}					
				}
			
				//�жϲ�������Ƿ���Ҫ������ת
				if(Window_Flag_ON == 1)
				{
					MOTOR_Direction_Angle(1,0,90,2);	//˳ʱ����ת90��
					MOTOR_STOP();
					Window_Flag_ON = 0;
				}
				if(Window_Flag_OFF == 1)
				{
					MOTOR_Direction_Angle(0,0,90,2);	//��ʱ����ת90��
					MOTOR_STOP();
					Window_Flag_OFF = 0;
				}
				break;
			case 2: 
				OLED_Clear();
				Menu2();
				Menu = 3;
		
				//�����ж���ֵ������û�б�����
				old_Temp_value = Temp_value;
				old_Humi_value = Humi_value;
				old_Smog_value = Smog_value;
				old_PM25_value = PM25_value;
				break;
			case 3: 
				//��ʾ��������ֵ
				OLED_ShowNum(1,13,Temp_value,2);
				OLED_ShowNum(2,13,Humi_value,2);
				OLED_ShowNum(3,13,Smog_value,4);
				OLED_ShowNum(4,13,PM25_value,4);

				//������ֵѡ��
				if(KeyNum == 1)
				{
					KeyNum = 0;
					value_Option_Num++;
					if(value_Option_Num > 4)
					{
						value_Option_Num = 1;
					}
				}
				//��ʾѡ���ʶ����>��
				OLED_Option(value_Option_Num);
				
				//�����ֵ��ֵ�ĵ���
				switch(value_Option_Num)
				{
					case 1: 
						if(KeyNum == 3)
						{
							KeyNum = 0;
							Temp_value++;
							if(Temp_value > 99)
							{
								Temp_value = 0;
							}
						}
						if(KeyNum == 4)
						{
							KeyNum = 0;
							Temp_value--;
							if(Temp_value > 99)
							{
								Temp_value = 99;
							}
						}
						break;
					case 2: 
						if(KeyNum == 3)
						{
							KeyNum = 0;
							Humi_value++;
							if(Humi_value > 99)
							{
								Humi_value = 0;
							}
						}
						if(KeyNum == 4)
						{
							KeyNum = 0;
							Humi_value--;
							if(Humi_value > 99)
							{
								Humi_value = 99;
							}
						}
						break;
					case 3: 
						if(KeyNum == 3)
						{
							KeyNum = 0;
							Smog_value++;
							if(Smog_value > 500)
							{
								Smog_value = 0;
							}
						}
						if(KeyNum == 4)
						{
							KeyNum = 0;
							Smog_value--;
							if(Smog_value > 500)
							{
								Smog_value = 500;
							}
						}
						break;
					case 4: 
						if(KeyNum == 3)
						{
							KeyNum = 0;
							PM25_value++;
							if(PM25_value > 500)
							{
								PM25_value = 0;
							}
						}
						if(KeyNum == 4)
						{
							KeyNum = 0;
							PM25_value--;
							if(PM25_value > 500)
							{
								PM25_value = 500;
							}									
						}
						break;
				}
				
				//�˳���ֵ���ý���
				if(KeyNum == 2)
				{
					KeyNum = 0;
					
					//����ֵ���ݱ����ĺ󣬲Ż�д��FLASH��
					if((old_Temp_value != Temp_value) || (old_Humi_value != Humi_value) || (old_Smog_value != Smog_value) || (old_PM25_value != PM25_value))
					{
						//д�봫������ֵ����
						FLASH_W(FLASH_START_ADDR,Temp_value,Humi_value,Smog_value,PM25_value);				
					}
					
					Menu = 1;	//�ص����˵�
					OLED_Clear();	//����
				}
				break;

			case 4:
				if (gizwitsModeFlag == 1)
				{
					//��ʾ���ȵ�������
					OLED_ShowChinese(2,3,22);
					OLED_ShowChinese(2,4,23);
					OLED_ShowChinese(2,5,24);
					OLED_ShowChinese(2,6,25);
				}
				else if (gizwitsModeFlag == 2)
				{
					//��ʾ��һ��������
					OLED_ShowChinese(2,3,18);
					OLED_ShowChinese(2,4,19);
					OLED_ShowChinese(2,5,20);
					OLED_ShowChinese(2,6,21);
				}
				if (!gizwitsModeFlag)
				{
					KeyNum = 0;
					OLED_Clear();
					Menu = 1;
				}				
				break;				

			default: break;				
		}
		
		if (ValueUpdateFlag)
		{
			ValueUpdateFlag = 0;
			//�洢�޸ĵĴ�������ֵ��flash��
			FLASH_W(FLASH_START_ADDR,Temp_value,Humi_value,Smog_value,PM25_value);						
		}	
		
		userHandle();	//���»��������ݵ�����洢��ֵ
		gizwitsHandle((dataPoint_t *)&currentDataPoint);	//�����ϴ���������					
	}


}

 
