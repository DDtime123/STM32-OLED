#include "bsp_i2c.h"
#include "delay.h"

uint8_t   ack_status=0;
uint8_t   readByte[6];
uint8_t   AHT20_status=0;

uint32_t  H1=0;  //æ¹¿åº¦
uint32_t  T1=0;  //æ¸©åº¦

uint8_t  AHT20_OutData[4];
uint8_t  AHT20sendOutData[10] = {0xFA, 0x06, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// åˆå§‹åŒ– I2Cçš„GPIO
void IIC_Init3(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //è®¾ç½®ä¸ºæŽ¨æŒ½è¾“å‡ºæ¨¡å¼
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	IIC_SCL3=1;
	IIC_SDA3=1;
 
}
//å¯åŠ¨I2Cé€šä¿¡
void IIC_Start3(void)
{
	SDA_OUT();     //sda??¨º?3?
	IIC_SDA3=1;	  	  
	IIC_SCL3=1;
	delay_us(4);
 	IIC_SDA3=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL3=0;//?¡¥¡Á?I2C¡Á¨¹??¡ê?¡Á?¡À?¡¤¡é?¨ª?¨°?¨®¨º?¨ºy?Y 
}	  
//åœæ­¢I2Cé€šä¿¡
void IIC_Stop3(void)
{
	SDA_OUT();//sda??¨º?3?
	IIC_SCL3=0;
	IIC_SDA3=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL3=1; 
	IIC_SDA3=1;//¡¤¡é?¨ªI2C¡Á¨¹???¨¢¨º?D?o?
	delay_us(4);							   	
}
//¦Ì¨¨¡äy¨®|¡äeD?o?¦Ì?¨¤¡ä
//¡¤¦Ì???¦Ì¡êo1¡ê??¨®¨º?¨®|¡äe¨º¡ì¡ã¨¹
//        0¡ê??¨®¨º?¨®|¡äe3¨¦1|
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA¨¦¨¨???a¨º?¨¨?  
	IIC_SDA3=1;delay_us(1);	   
	IIC_SCL3=1;delay_us(1);	 
	while(READ_SDA3)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop3();
			return 1;
		}
	}
	IIC_SCL3=0;//¨º¡À?¨®¨º?3?0 	   
	return 0;  
} 
//2¨²¨¦¨²ACK¨®|¡äe
void IIC_Ack(void)
{
	IIC_SCL3=0;
	SDA_OUT();
	IIC_SDA3=0;
	delay_us(2);
	IIC_SCL3=1;
	delay_us(2);
	IIC_SCL3=0;
}
//2?2¨²¨¦¨²ACK¨®|¡äe		    
void IIC_NAck(void)
{
	IIC_SCL3=0;
	SDA_OUT();
	IIC_SDA3=1;
	delay_us(2);
	IIC_SCL3=1;
	delay_us(2);
	IIC_SCL3=0;
}					 				     
//IIC¡¤¡é?¨ª¨°???¡Á??¨²
//¡¤¦Ì??¡ä¨®?¨²¨®D?T¨®|¡äe
//1¡ê?¨®D¨®|¡äe
//0¡ê??T¨®|¡äe			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL3=0;//¨¤-¦Ì¨ª¨º¡À?¨®?a¨º?¨ºy?Y¡ä?¨º?
    for(t=0;t<8;t++)
    {              
        IIC_SDA3=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //??TEA5767?a¨¨y???¨®¨º¡À??¨º?¡À?D?¦Ì?
		IIC_SCL3=1;
		delay_us(2); 
		IIC_SCL3=0;	
		delay_us(2);
    }	 
} 	    
//?¨¢1??¡Á??¨²¡ê?ack=1¨º¡À¡ê?¡¤¡é?¨ªACK¡ê?ack=0¡ê?¡¤¡é?¨ªnACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA¨¦¨¨???a¨º?¨¨?
  for(i=0;i<8;i++ )
	{
    IIC_SCL3=0; 
    delay_us(2);
		IIC_SCL3=1;
    receive<<=1;
    if(READ_SDA3)receive++;   
		delay_us(1); 
  }					 
	if (!ack)
			IIC_NAck();//¡¤¡é?¨ªnACK
	else
			IIC_Ack(); //¡¤¡é?¨ªACK   
	return receive;
}
 
void IIC_WriteByte(uint16_t addr,uint8_t data,uint8_t device_addr)
{
	IIC_Start3();  
	
	if(device_addr==0xA0) //eeprom¦Ì??¡¤¡ä¨®¨®¨²1¡Á??¨²
		IIC_Send_Byte(0xA0 + ((addr/256)<<1));//¡¤¡é?¨ª??¦Ì??¡¤
	else
		IIC_Send_Byte(device_addr);	    //¡¤¡é?¡Â?t¦Ì??¡¤
	IIC_Wait_Ack(); 
	IIC_Send_Byte(addr&0xFF);   //¡¤¡é?¨ª¦Ì¨ª¦Ì??¡¤
	IIC_Wait_Ack(); 
	IIC_Send_Byte(data);     //¡¤¡é?¨ª¡Á??¨²							   
	IIC_Wait_Ack();  		    	   
  IIC_Stop3();//2¨²¨¦¨²¨°???¨ª¡ê?1¨¬??t 
	if(device_addr==0xA0) //
		delay_ms(1);
	else
		delay_us(2);
}
 
uint16_t IIC_ReadByte(uint16_t addr,uint8_t device_addr,uint8_t ByteNumToRead)  //?¨¢??¡ä??¡Â?¨°?¨¢¨ºy?Y
{	
		uint16_t data;
		IIC_Start3();  
		if(device_addr==0xA0)
			IIC_Send_Byte(0xA0 + ((addr/256)<<1));
		else
			IIC_Send_Byte(device_addr);	
		IIC_Wait_Ack();
		IIC_Send_Byte(addr&0xFF);   //¡¤¡é?¨ª¦Ì¨ª¦Ì??¡¤
		IIC_Wait_Ack(); 
 
		IIC_Start3();  	
		IIC_Send_Byte(device_addr+1);	    //¡¤¡é?¡Â?t¦Ì??¡¤
		IIC_Wait_Ack();
		if(ByteNumToRead == 1)//LM75???¨¨¨ºy?Y?a11bit
		{
			data=IIC_Read_Byte(0);
		}
		else
			{
				data=IIC_Read_Byte(1);
				data=(data<<8)+IIC_Read_Byte(0);
			}
		IIC_Stop3();//2¨²¨¦¨²¨°???¨ª¡ê?1¨¬??t	    
		return data;
}


/**********
*¨¦???2?¡¤??aIO?¨²?¡ê?¨¦I2C????
*
*¡ä¨®?a¨°????a¨º??aAHT20¦Ì?????I2C
*o¡¥¨ºy??¨®DIICo¨ªI2C¦Ì???¡Àe¡ê???¡Á¡é¨°a¡ê?¡ê?¡ê?¡ê?¡ê?
*
*2020/2/23¡Á?o¨®DT??¨¨??¨²
*
***********/
void  read_AHT20_once(void)
{
	
	
	delay_ms(1);

	reset_AHT20();
	//Oled_Display_String(2,0,buf1);
	//Oled_Display_String(4,0,buf2);
	
	delay_ms(1);

	init_AHT20();
	delay_ms(1);

	startMeasure_AHT20();
	delay_ms(1);

	read_AHT20();
	delay_ms(1);
}


void  reset_AHT20(void)
{

	I2C_Start();

	I2C_WriteByte(0x70);
	ack_status = Receive_ACK();
	if(ack_status) printf("1");
	else printf("1-n-");
	I2C_WriteByte(0xBA);
	ack_status = Receive_ACK();
		if(ack_status) printf("2");
	else printf("2-n-");
	I2C_Stop();

	/*
	AHT20_OutData[0] = 0;
	AHT20_OutData[1] = 0;
	AHT20_OutData[2] = 0;
	AHT20_OutData[3] = 0;
	*/
}



void  init_AHT20(void)
{
	
	I2C_Start();

	I2C_WriteByte(0x70);
	ack_status = Receive_ACK();
	if(ack_status) printf("3");
	else printf("3-n-");	
	I2C_WriteByte(0xE1);
	ack_status = Receive_ACK();
	if(ack_status) printf("4");
	else printf("4-n-");
	I2C_WriteByte(0x08);
	ack_status = Receive_ACK();
	if(ack_status) printf("5");
	else printf("5-n-");
	I2C_WriteByte(0x00);
	ack_status = Receive_ACK();
	if(ack_status) printf("6");
	else printf("6-n-");
	I2C_Stop();
}



void  startMeasure_AHT20(void)
{
	//------------
	I2C_Start();

	I2C_WriteByte(0x70);
	ack_status = Receive_ACK();
	if(ack_status) printf("7");
	else printf("7-n-");
	I2C_WriteByte(0xAC);
	ack_status = Receive_ACK();
	if(ack_status) printf("8");
	else printf("8-n-");
	I2C_WriteByte(0x33);
	ack_status = Receive_ACK();
	if(ack_status) printf("9");
	else printf("9-n-");
	I2C_WriteByte(0x00);
	ack_status = Receive_ACK();
	if(ack_status) printf("10");
	else printf("10-n-");
	I2C_Stop();
}



void read_AHT20(void)
{
	char buf1[20];
	char buf2[20];
	char buf[] = {"templeture:"};
	uint8_t   i;

	for(i=0; i<6; i++)
	{
		readByte[i]=0;
	}

	//-------------
	I2C_Start();

	I2C_WriteByte(0x71);
	ack_status = Receive_ACK();
	readByte[0]= I2C_ReadByte();
	Send_ACK();

	readByte[1]= I2C_ReadByte();
	Send_ACK();

	readByte[2]= I2C_ReadByte();
	Send_ACK();

	readByte[3]= I2C_ReadByte();
	Send_ACK();

	readByte[4]= I2C_ReadByte();
	Send_ACK();

	readByte[5]= I2C_ReadByte();
	SendNot_Ack();
	//Send_ACK();

	

	I2C_Stop();

	//--------------
	if( (readByte[0] & 0x68) == 0x08 )
	{
		H1 = readByte[1];
		H1 = (H1<<8) | readByte[2];
		H1 = (H1<<8) | readByte[3];
		H1 = H1>>4;

		H1 = (H1*1000)/1024/1024;

		T1 = readByte[3];
		T1 = T1 & 0x0000000F;
		T1 = (T1<<8) | readByte[4];
		T1 = (T1<<8) | readByte[5];

		T1 = (T1*2000)/1024/1024 - 500;

		AHT20_OutData[0] = (H1>>8) & 0x000000FF;
		AHT20_OutData[1] = H1 & 0x000000FF;

		AHT20_OutData[2] = (T1>>8) & 0x000000FF;
		AHT20_OutData[3] = T1 & 0x000000FF;
	}
	else
	{
		AHT20_OutData[0] = 0xFF;
		AHT20_OutData[1] = 0xFF;

		AHT20_OutData[2] = 0xFF;
		AHT20_OutData[3] = 0xFF;
		printf("lyy");

	}
	sprintf(buf1, "wendu:%d%d.%d",T1/100,(T1/10)%10,T1%10);
	sprintf(buf2, "shidu:%d%d.%d",H1/100,(H1/10)%10,H1%10);
	//OLED_Clear(0x00); 
	Oled_Display_String(0,0,buf);
	Oled_Display_String(2,0,buf1);
	Oled_Display_String(4,0,buf2);
	
	printf("\r\n");
	printf("wendu:%d%d.%d",T1/100,(T1/10)%10,T1%10);
	printf("shidu:%d%d.%d",H1/100,(H1/10)%10,H1%10);
	printf("\r\n");
	
	printf("\r\n");
	printf("wendu:%d%d.%d",T1/100,(T1/10)%10,T1%10);
	printf("shidu:%d%d.%d",H1/100,(H1/10)%10,H1%10);
	printf("\r\n");
}




uint8_t  Receive_ACK(void)
{
	uint8_t result=0;
	uint8_t cnt=0;

	IIC_SCL3 = 0;
	SDA_IN(); 
	delay_us(1);

	IIC_SCL3 = 1;
	delay_us(1);

	while(READ_SDA3 && (cnt<100))
	{
		cnt++;
	}

	IIC_SCL3 = 0;
	delay_us(1);

	if(cnt<100)
	{
		result=1;
	}
	return result;
}



void  Send_ACK(void)
{
	SDA_OUT();
	IIC_SCL3 = 0;
	delay_us(4);

	IIC_SDA3 = 0;
	delay_us(4);

	IIC_SCL3 = 1;
	delay_us(4);
	IIC_SCL3 = 0;
	delay_us(4);

	SDA_IN();
}



void  SendNot_Ack(void)
{
	SDA_OUT();
	IIC_SCL3 = 0;
	delay_us(4);

	IIC_SDA3 = 1;
	delay_us(4);

	IIC_SCL3 = 1;
	delay_us(4);

	IIC_SCL3 = 0;
	delay_us(4);

	IIC_SDA3 = 0;
	delay_us(4);
}


void I2C_WriteByte(uint8_t  input)
{
	uint8_t  i;
	SDA_OUT();
	for(i=0; i<8; i++)
	{
		IIC_SCL3 = 0;
		delay_ms(5);

		if(input & 0x80)
		{
			IIC_SDA3 = 1;
			//delaymm(10);
		}
		else
		{
			IIC_SDA3 = 0;
			//delaymm(10);
		}

		IIC_SCL3 = 1;
		delay_ms(5);

		input = (input<<1);
	}

	IIC_SCL3 = 0;
	delay_us(4);

	SDA_IN();
	delay_us(4);
}	


uint8_t I2C_ReadByte(void)
{
	uint8_t  resultByte=0;
	uint8_t  i=0, a=0;

	IIC_SCL3 = 0;
	SDA_IN();
	delay_ms(4);

	for(i=0; i<8; i++)
	{
		IIC_SCL3 = 1;
		delay_ms(3);

		a=0;
		if(READ_SDA3)
		{
			a=1;
		}
		else
		{
			a=0;
		}

		//resultByte = resultByte | a;
		resultByte = (resultByte << 1) | a;

		IIC_SCL3 = 0;
		delay_ms(3);
	}

	SDA_IN();
	delay_ms(10);

	return   resultByte;
}


void  set_AHT20sendOutData(void)
{
	/* --------------------------
	 * 0xFA 0x06 0x0A temperature(2 Bytes) humility(2Bytes) short Address(2 Bytes)
	 * And Check (1 byte)
	 * -------------------------*/
	AHT20sendOutData[3] = AHT20_OutData[0];
	AHT20sendOutData[4] = AHT20_OutData[1];
	AHT20sendOutData[5] = AHT20_OutData[2];
	AHT20sendOutData[6] = AHT20_OutData[3];

//	AHT20sendOutData[7] = (drf1609.shortAddress >> 8) & 0x00FF;
//	AHT20sendOutData[8] = drf1609.shortAddress  & 0x00FF;

//	AHT20sendOutData[9] = getXY(AHT20sendOutData,10);
}


void  I2C_Start(void)
{
	SDA_OUT();
	IIC_SCL3 = 1;
	delay_ms(4);

	IIC_SDA3 = 1;
	delay_ms(4);
	IIC_SDA3 = 0;
	delay_ms(4);

	IIC_SCL3 = 0;
	delay_ms(4);
}



void  I2C_Stop(void)
{
	SDA_OUT();
	IIC_SDA3 = 0;
	delay_ms(4);

	IIC_SCL3 = 1;
	delay_ms(4);

	IIC_SDA3 = 1;
	delay_ms(4);
}

