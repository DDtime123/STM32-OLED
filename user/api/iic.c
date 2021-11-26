#include "iic.h"

/*******************************************************************************
*������ԭ�ͣ�void IIC_Pin_Init(void)
*�����Ĺ��ܣ�IIC���ų�ʼ��
*�����Ĳ�����None
*��������ֵ��None
*������˵����
	SCL = PA6
	SDA = PA7
*������д�ߣ���
*������д���ڣ�2021/2/22
*�����İ汾�ţ�V1.0
********************************************************************************/
void IIC_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(IIC_GPIO_RCC,ENABLE);
	
	//SCL
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = IIC_SCL_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_GPIO,&GPIO_InitStruct);
	
	//SDA
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = IIC_SDA_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_GPIO,&GPIO_InitStruct);
}

/*******************************************************************************
*������ԭ�ͣ�void IIC_Start(void)
*�����Ĺ��ܣ�iic��ʼ����
*�����Ĳ�����None
*��������ֵ��None
*������˵����
*������д�ߣ���
*������д���ڣ�2021/2/22
*�����İ汾�ţ�V1.0
********************************************************************************/
void IIC_Start(void)
{
	IIC_SCL = 1;
	IIC_SDA = 1;
	delay_us(5);
	IIC_SDA = 0;
	delay_us(5);
	IIC_SCL = 0;
}

/*******************************************************************************
*������ԭ�ͣ�void IIC_Stop(void)
*�����Ĺ��ܣ�iicֹͣ����
*�����Ĳ�����None
*��������ֵ��None
*������˵����
*������д�ߣ���
*������д���ڣ�2021/2/22
*�����İ汾�ţ�V1.0
********************************************************************************/
void IIC_Stop(void)
{
	IIC_SCL = 0;
//	IIC_SDA = 0;
	
	IIC_SDA = 1;
	delay_us(5);
	IIC_SCL = 1;
	delay_us(5);
	IIC_SDA = 0;
}

/*******************************************************************************
*������ԭ�ͣ�void IIC_Send_Ack(u8 ack)
*�����Ĺ��ܣ�����Ӧ���ź�
*�����Ĳ�����
	@ u8 ack��Ӧ���ź� 0����Ӧ�� 1��Ӧ��
*��������ֵ��None
*������˵����
*������д�ߣ���
*������д���ڣ�2021/2/22
*�����İ汾�ţ�V1.0
********************************************************************************/  
void IIC_Send_Ack(u8 ack)
{
	IIC_SCL = 1;
	//ΪʲôӦ���ź����Ӧ���źſ�����һ�������У���
	//����ʱ���ź���ͬ�������źŵĳ�ʼ��ƽ��ͬ����  �ֿ�д
	
	delay_us(5);
	IIC_SCL = 0;
	delay_us(5);
	
	if(ack)
	{
		IIC_SDA = 1;
	}
	else
	{
		IIC_SDA = 0;
	}
	
	delay_us(5);
	//��������������
	IIC_SCL = 1;
	delay_us(5);
	IIC_SCL = 0;
}

/*******************************************************************************
*������ԭ�ͣ�u8 IIC_Reception_Ack(void)
*�����Ĺ��ܣ�����Ӧ���ź�
*�����Ĳ�����None
*��������ֵ��
	0����Ӧ��
	1��Ӧ��
*������˵�����ߵ�ƽ���ݲɼ� �͵�ƽ���ݷ���
*������д�ߣ���
*������д���ڣ�2021/2/22
*�����İ汾�ţ�V1.0
********************************************************************************/
u8 IIC_Reception_Ack(void)
{
	IIC_SDA = 1;
	delay_us(5);
	//----
	IIC_SCL = 0;
	delay_us(5);
	IIC_SCL = 1;
	
	if(IIC_SDA_IN)
	{
		return 1;
	}
	
	return 0;
}

//ÿһ���ֽں�Ҫ��һ����Ӧλ��ȷ�������Ƿ��ͳɹ���
//����һ�ֽ�    
/*******************************************************************************
*������ԭ�ͣ�u8 IIC_Send_Data(u8 data)
*�����Ĺ��ܣ���������
*�����Ĳ�����
	@ u8 data��һ�ֽ�����
*��������ֵ��Ӧ���ź�
*������˵����8λ���ݷ��� ÿ���ֽں��涼Ҫ��һ����Ӧ
*������д�ߣ���
*������д���ڣ�2021/2/22
*�����İ汾�ţ�V1.0
********************************************************************************/
u8 IIC_Send_Data(u8 data)
{
	u8 i = 0;
	
//	IIC_Start();
	
	for(i = 0; i < 8; i++)
	{
		//һλһλ�ķ�
//		IIC_SCL = 1;
//		delay_us(5);
		IIC_SCL = 0; 
		if(data & 0x80)
		{
			IIC_SDA = 1;
		}
		else
		{
			IIC_SDA = 0;
		}
		data <<= 1;
		delay_us(5);
	//��������������
		IIC_SCL = 1;
		delay_us(5);
//		IIC_SCL = 0;
	}
	
	IIC_SCL = 0;
	return IIC_Reception_Ack();
	//����Ӧ��
}


/*******************************************************************************
*������ԭ�ͣ�u8 IIC_Read_Data(void)
*�����Ĺ��ܣ���һ�ֽ�����
*�����Ĳ�����None
*��������ֵ��
	һ�ֽ�����
*������˵����
*������д�ߣ���
*������д���ڣ�2021/2/22
*�����İ汾�ţ�V1.0
********************************************************************************/
u8 IIC_Read_Data(void)
{
	u8 data = 0;
	u8 i = 0;
	
	IIC_SDA = 1;
	
	for(i = 0; i < 8; i++)
	{
		IIC_SCL = 0;
		delay_us(5);
		IIC_SCL = 1;
		
		if(IIC_SDA_IN)
		{
			data |= 1;
		}
		
		data <<= 1;
	}
	
	IIC_Send_Ack(1);
	
	return data;
}

/*********************************************************************/
void IIC_Start2(void)
{
	IIC_SCL = 1;
	IIC_SDA = 1;
	delay_us(2);
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 0;
}
void IIC_Stop2(void)
{
	IIC_SCL = 0;
//	IIC_SDA = 0;
	
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SDA = 0;
}
u8 IIC_Send_Data2(u8 data)
{
	u8 i = 0;
	
//	IIC_Start();
	
	for(i = 0; i < 8; i++)
	{
		//һλһλ�ķ�
//		IIC_SCL = 1;
//		delay_us(5);
		IIC_SCL = 0; 
		if(data & 0x80)
		{
			IIC_SDA = 1;
		}
		else
		{
			IIC_SDA = 0;
		}
		data <<= 1;
		delay_us(2);
	//��������������
		IIC_SCL = 1;
		delay_us(2);
//		IIC_SCL = 0;
	}
	
	IIC_SCL = 0;
	return IIC_Reception_Ack();
	//����Ӧ��
}