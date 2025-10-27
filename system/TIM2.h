#ifndef __TIM2_H__
#define __TIM2_H__

#include "stm32f10x.h"                  // Device header
#include "menu.h"
#include "buzzer.h"
#include "key.h"
#include "relay.h"

extern u8 Menu;
extern u8 Model_Flag;
extern u8 Temp_value;
extern u8 Humi_value;
extern u16 Smog_value;
extern u16 PM25_value;

extern u8 Window_Flag_ON;	
extern u8 Window_Flag_OFF;	

extern uint8_t gizwitsModeFlag;			// 配网标志位
extern u8 Fan_Flag;			// 风扇状态标志位
extern u8 Humidifier_Flag;		// 加湿器状态标志位
extern u8 Window_Flag;			// 窗户状态标志位

void Timer2_Init(u16 Prescaler, u16 Period);

#endif
