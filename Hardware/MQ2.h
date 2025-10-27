#ifndef __MQ2_H
#define __MQ2_H

#include "stm32f10x.h"                  // Device header
#include "adcx.h"
#include "delay.h"
#include "math.h"

#define MQ2		  GPIO_Pin_4
#define MQ2_PROT  GPIOA

#define CAL_PPM  10  // У׼������PPMֵ
#define RL	     10  // RL��ֵ
#define R0	     35  // R0��ֵ

#define SMOG_READ_TIMES	10	//��������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ

void MQ2_Init(void);
u16 MQ2_Average_Data(void);
float MQ2_Voltage_Data(void);
float Smog_GetPPM(void);

#endif
