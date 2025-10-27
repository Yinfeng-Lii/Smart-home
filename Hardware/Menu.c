#include "menu.h"

u8 Humi,Temp;
u16 pm25,ppm;

void Menu1(void)
{

	//��ʾ���¶ȣ�  C��
	OLED_ShowChinese(1,1,0);
	OLED_ShowChinese(1,2,5);
	OLED_ShowChar(1,5,':');
	OLED_ShowChar(1,8,'C');
	
	//��ʾ��ʪ��:   %��
	OLED_ShowChinese(1,5,1);
	OLED_ShowChinese(1,6,5);
	OLED_ShowChar(1,13,':');	
	OLED_ShowChar(1,16,'%');
	
	//��ʾ��������
	OLED_ShowChinese(2,1,2);
	OLED_ShowChinese(2,2,3);
	OLED_ShowChar(2,5,':');
	OLED_ShowString(2,12,"ppm");

	//��ʾ��PM2.5����
	OLED_ShowString(3,1,"PM2.5:");
	OLED_ShowString(3,12,"ug/m3");
	
	//��ʾ��ģʽ����
	OLED_ShowChinese(4,1,6);
	OLED_ShowChinese(4,2,7);
	OLED_ShowChar(4,5,':');
	
	//��ʾ DHT11��ʪ������
	DHT11_Read_Data(&Humi,&Temp);
	OLED_ShowNum(1,6,Temp,2);
	OLED_ShowNum(1,14,Humi,2);
	
	//��ʾ ����PPM����
	ppm = Smog_GetPPM();
	OLED_ShowNum(2,8,ppm,3);
	
	//��ʾ PM2.5����
	pm25 = Get_PM25_Average_Data();
	OLED_ShowNum(3,8,pm25,3);
	
	if(Model_Flag == 0)
	{
		//��ʾ ���ֶ�ģʽ��
		OLED_ShowChinese(4,4,10);
	    OLED_ShowChinese(4,5,11);
		OLED_ShowChinese(4,6,6);
		OLED_ShowChinese(4,7,7);
	}
	else
	{
		//��ʾ ���Զ�ģʽ��
		OLED_ShowChinese(4,4,8);
	    OLED_ShowChinese(4,5,9);
		OLED_ShowChinese(4,6,6);
		OLED_ShowChinese(4,7,7);
	}
		
}



void Menu2(void)
{

	//�¶���ֵ
	OLED_ShowChinese(1,2,0);
	OLED_ShowChinese(1,3,5);
	OLED_ShowChinese(1,4,14);
	OLED_ShowChinese(1,5,15);	
	OLED_ShowChar(1,11,':');
	
	//ʪ����ֵ
	OLED_ShowChinese(2,2,1);
	OLED_ShowChinese(2,3,5);
	OLED_ShowChinese(2,4,14);
	OLED_ShowChinese(2,5,15);
	OLED_ShowChar(2,11,':');

	//������ֵ
	OLED_ShowChinese(3,2,2);
	OLED_ShowChinese(3,3,3);
	OLED_ShowChinese(3,4,14);
	OLED_ShowChinese(3,5,15);
	OLED_ShowChar(3,11,':');

	//PM2.5��ֵ
	OLED_ShowString(4,3,"PM25");
	OLED_ShowChinese(4,4,14);
	OLED_ShowChinese(4,5,15);
	OLED_ShowChar(4,11,':');
}

void OLED_Option(u8 num)
{
	switch(num)
	{
		case 1:	
			OLED_ShowChar(2,1,' ');
			OLED_ShowChar(3,1,' ');
			OLED_ShowChar(4,1,' ');
			OLED_ShowChar(1,1,'>');
			break;
		case 2:	
			OLED_ShowChar(1,1,' ');
			OLED_ShowChar(3,1,' ');
			OLED_ShowChar(4,1,' ');
			OLED_ShowChar(2,1,'>');
			break;
		case 3:	
			OLED_ShowChar(1,1,' ');
			OLED_ShowChar(2,1,' ');
			OLED_ShowChar(4,1,' ');
			OLED_ShowChar(3,1,'>');
			break;
		case 4:	
			OLED_ShowChar(1,1,' ');
			OLED_ShowChar(2,1,' ');
			OLED_ShowChar(3,1,' ');
			OLED_ShowChar(4,1,'>');
			break;
		default: break;
	}
}
