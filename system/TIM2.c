#include "tim2.h"                  // Device header

u8 Window_Flag_lock = 0;
u8 Buzzer_Flag_lock = 0;

void Timer2_Init(u16 Prescaler, u16 Period)	//0-65535
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = Period;
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}

void SetGizwitsMode(void)
{
	static uint32_t time;
	if (gizwitsModeFlag)
	{
		time++;
		if (time == 600)
		{
			Buzzer_OFF();
		}
		else if (time >= 2000)
		{
			printf("����ϵͳģʽ�л�ָ��\n");	
			gizwitsSetMode(gizwitsModeFlag);
			printf("�л�ϵͳģʽ\n");
			gizwitsModeFlag = 0;
		}
	}
}

void TIM2_IRQHandler(void)	//2ms��ʱ�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ) 
	{
		Key_scan();	//����ɨ�躯��

		//�жϰ���1�Ƿ񱻴���
		if(KeyNum == 11 && Menu == 1)
		{
			KeyNum = 0;
			Menu = 2;
		}
		
		//ϵͳ�����Զ�ģʽʱ����
		if(Model_Flag)
		{
			//�жϵ�ǰ�¶Ⱥ������Ƿ������ֵ
			if((Temp_value < Temp) || (Smog_value < ppm))	
			{
				Fan_Flag = 1;
				Relay_ON(1);	//��������	
				
			}
			else
			{
				Fan_Flag = 0;
				Relay_OFF(1);	//�رշ���	
			}

			//�жϵ�ǰʪ���Ƿ������ֵ
			if(Humi_value > Humi)	
			{
				Humidifier_Flag = 1;
				Relay_ON(2);	//������ʪ��
			}
			else
			{
				Humidifier_Flag = 0;
				Relay_OFF(2);	//�رռ�ʪ��	
			}

			//�жϵ�ǰPM2.5�Ƿ������ֵ
			if((PM25_value < pm25) && (Window_Flag_lock == 0))	
			{
				Window_Flag = 1;
				Window_Flag_ON = 1;	//�򿪴���
				Window_Flag_lock = 1;
			}
			else if((PM25_value > pm25) && (Window_Flag_lock == 1))
			{
				Window_Flag = 0;
				Window_Flag_OFF = 1;	//�򿪴���
				Window_Flag_lock = 0;				
			}
			
			//�������ܿ���
			if((Temp_value < Temp) || (Humi_value > Humi) || (Smog_value < ppm) || (PM25_value < pm25))
			{
				Buzzer_ON();
			}
			else
			{
				Buzzer_OFF();	
			}
		}
		
		//�����������������������½���������	
		SetGizwitsMode();

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		
	}
}

