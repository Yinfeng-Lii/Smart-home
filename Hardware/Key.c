#include "key.h"

u8 key1_long_flag,key2_long_flag,key3_long_flag,key4_long_flag;
u8 key1_lock_flag,key2_lock_flag,key3_lock_flag,key4_lock_flag;
u16 key1_cnt,key2_cnt,key3_cnt,key4_cnt;
u16 key1_cnt2,key2_cnt2,key3_cnt2,key4_cnt2;
u8 key1_lock_flag,key2_lock_flag,key3_lock_flag,key4_lock_flag;

u8 key1_short_flag,key2_short_flag,key3_short_flag,key4_short_flag;


u8 KeyNum = 0;
/**
  * @brief  Key��ʼ��
  * @param  ��
  * @retval ��
  */
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN | KEY2_GPIO_PIN | KEY3_GPIO_PIN | KEY4_GPIO_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY_PROT, &GPIO_InitStructure);
}

/**
  * @brief  Key��ֵ��ȡ
  * @param  ��
  * @retval ���ش��������ļ�ֵ
  */
void Key_scan(void)
{
	if(KEY1) //���û�а�������
	{
		key1_lock_flag = 0; //����������־
		key1_cnt = 0; //���������־
		if(key1_long_flag)
		{
			key1_long_flag = 0;	//���㳤����־λ
			KeyNum = 11; //����ֵ���� ������ֵ����
		}
		if(key1_short_flag)
		{
			key1_short_flag = 0;	//���㳤����־λ
			KeyNum = 1; //����ֵ���� ������ֵ����
		}		
	}
	else if(!key1_lock_flag)
	{
		key1_cnt++; //�ۼư���������ʱ����
		if(key1_cnt > KEY_DELAY_TIME)
		{
			key1_cnt = 0; //���������־
			key1_short_flag = 1; //����ֵ���� �̰���ֵ����
			key1_lock_flag = 1; //������־��1 ��ֹ��δ���
		}
	}
	else
	{
		key1_cnt++;	//�ۼư�������������ʱʱ��

		if(key1_cnt > KEY1_LONG_TIME)
		{
			key1_short_flag = 0;
			key1_long_flag = 1;	//��������־λ
		}
	}
	
	if(KEY2) //���û�а�������
	{
		key2_lock_flag = 0; //����������־
		key2_cnt = 0; //���������־
		if(key2_long_flag)
		{
			key2_long_flag = 0;	//���㳤����־λ
			KeyNum = 22; //����ֵ���� ������ֵ����
		}
		if(key2_short_flag)
		{
			key2_short_flag = 0;	//���㳤����־λ
			KeyNum = 2; //����ֵ���� ������ֵ����
		}		
	}
	else if(!key2_lock_flag)
	{
		key2_cnt++; //�ۼư���������ʱ����
		if(key2_cnt > KEY_DELAY_TIME)
		{
			key2_cnt = 0; //���������־
			key2_short_flag = 2; //����ֵ���� �̰���ֵ����
			key2_lock_flag = 1; //������־��1 ��ֹ��δ���
		}
	}
	else
	{
		key2_cnt++;	//�ۼư�������������ʱʱ��

		if(key2_cnt > KEY_LONG_TIME)
		{
			key2_short_flag = 0;
			key2_long_flag = 2;	//��������־λ
		}
	}
	
	if(KEY3) //���û�а�������
	{
		key3_lock_flag = 0; //����������־
		key3_cnt = 0; //���������־
		if(key3_long_flag)
		{
			key3_long_flag = 0;	//���㳤����־λ
			KeyNum = 33; //����ֵ���� ������ֵ����
		}
		if(key3_short_flag)
		{
			key3_short_flag = 0;	//���㳤����־λ
			KeyNum = 3; //����ֵ���� ������ֵ����
		}
		KeyHerdware();		
	}
	else if(!key3_lock_flag)
	{
		key3_cnt++; //�ۼư���������ʱ����
		if(key3_cnt > KEY_DELAY_TIME)
		{
			key3_cnt = 0; //���������־
			key3_short_flag = 3; //����ֵ���� �̰���ֵ����
			key3_lock_flag = 1; //������־��1 ��ֹ��δ���
		}
	}
	else
	{
		key3_cnt++;	//�ۼư�������������ʱʱ��

		if(key3_cnt > KEY1_LONG_TIME)
		{
			key3_short_flag = 0;
			key3_long_flag = 3;	//��������־λ
		}
	}
	
	if(KEY4) //���û�а�������
	{
		key4_lock_flag = 0; //����������־
		key4_cnt = 0; //���������־
		if(key4_long_flag)
		{
			key4_long_flag = 0;	//���㳤����־λ
			KeyNum = 44; //����ֵ���� ������ֵ����
		}
		if(key4_short_flag)
		{
			key4_short_flag = 0;	//���㳤����־λ
			KeyNum = 4; //����ֵ���� ������ֵ����
		}		
	}
	else if(!key4_lock_flag)
	{
		key4_cnt++; //�ۼư���������ʱ����
		if(key4_cnt > KEY_DELAY_TIME)
		{
			key4_cnt = 0; //���������־
			key4_short_flag = 4; //����ֵ���� �̰���ֵ����
			key4_lock_flag = 1; //������־��1 ��ֹ��δ���
		}
	}
	else
	{
		key4_cnt++;	//�ۼư�������������ʱʱ��

		if(key4_cnt > KEY1_LONG_TIME)
		{
			key4_short_flag = 0;
			key4_long_flag = 4;	//��������־λ
		}
	}
}








