#include "main.h"
void showMyName();
void showHello();
void showSong();
void showHanZi(int len,u8 * mchain);
void showEnglish(int len,u8 * mchain);
int i;
int offset;
int rowoffset;
int pageoffset;
int absoffset=0;
int main(void)
{
	char buf1[] = {"There is only"};
	char buf2[] = {"word."};
	char author[] = {"Qing"};
	
	
	Sys_Delay_Init();
	Oled_Init();
	
	
	/***************AHT20******************/
	/**
	delay_init();     //?��o��y3?�??�	  
	uart_init(115200);	 //'??�3?�??�?a115200
	Oled_Init();
	IIC_Init3();
	Oled_Init();
		while(1)
	{
		read_AHT20_once();
		delay_ms(1);
  }
	*/
	/***************AHT20******************/
//	Oled_Display_Char(0,0,'A');//��ʾ�����ַ�
	
	//There is no luck.There is only work.�����ǲ����ڵģ�Ŭ������Ӳ����
	/**
	Oled_Display_String(0,0,buf); //��ʾ�ַ���
	Oled_Display_String(2,0,buf1); //��ʾ�ַ���
	Oled_Display_String(4,0,buf2); //��ʾ�ַ���
	Oled_Display_String(6,80,author);
	*/
//	Oled_Display_Pic(128,64,0,0,pic);//��ʾͼƬ
	//Oled_Display_Pic(32,32,2,48,chain);//��ʾ����
	//Oled_Display_Pic(32,32,2,48,chain2);//��ʾ����
//	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	//showHello();
	
	while(1)
	{
		showSong();
		//OLED_Clear(0x00);
		/**
		showHello();
		delay_ms(1000);
		fast_clear();
		showMyName();
		delay_ms(1000);
		fast_clear();
		*/
		//showSong();
		//delay_ms(1000);
		//OLED_Clear(0x00); //����
		//Oled_Display_Pic(32,32,2,48,chain);//��ʾ����
		//Oled_Display_Pic(32,32,2,48,chain2);//��ʾ����
		//showMyName();
		
		//Oled_Display_Pic(16,16,3,16*2,chain3+32*2);//��ʾ����
	}
}

void showMyName(){
	for(i=0;i<3;i++){
		Oled_Display_Pic(16,16,0,16*i,chain3+32*i);//��ʾ����
	}
	/**
	for(i=0;i<12;i++){
		Oled_Display_Pic(8,16,3,8*i,chain3+16*(i+7));//��ʾ����
	}*/
}
void showSong(){
	offset=0;
	rowoffset=0;
	pageoffset=0;
	
	showHanZi(9,chain5_name);
	offset=0;
	showHanZi(3,chain5_author);
	offset=0;
	showHanZi(64,chain5_song);
}
void showHello(){
	offset=0;
	rowoffset=0;
	pageoffset=0;
	showEnglish(5,chain4);
	showHanZi(14,chain4);
	showEnglish(3,chain4);
	showHanZi(4,chain4);
	/**
	for(i=0;i<5;i++){
		//Oled_Display_Pic(8,16,0,8*i,chain4+offset);//��ʾ����
		//Oled_Display_Pic(8,16,0,rowoffset,chain4+offset);//��ʾ����
		if((rowoffset+8)>128){
			pageoffset+=2;
			rowoffset=0;
		}
		Oled_Display_Pic(8,16,pageoffset,rowoffset,chain4+offset);//��ʾ����
		offset+=16;
		rowoffset+=8;	
	}
	for(i=0;i<14;i++){//14
		//Oled_Display_Pic(16,16,2,16*i,chain4+offset);//��ʾ����
		if((rowoffset+16)>128){
			pageoffset+=2;
			rowoffset=0;
		}
		Oled_Display_Pic(16,16,pageoffset,rowoffset,chain4+offset);//��ʾ����
		offset+=32;
		rowoffset+=16;
	}
	for(i=0;i<3;i++){//205
		if((rowoffset+8)>128){
			pageoffset+=2;
			rowoffset=0;
		}
		Oled_Display_Pic(8,16,pageoffset,rowoffset,chain4+offset);//��ʾ����
		offset+=16;
		rowoffset+=8;	
	}
	for(i=0;i<4;i++){//4
		if((rowoffset+16)>128){
			pageoffset+=2;
			rowoffset=0;
		}
		Oled_Display_Pic(16,16,pageoffset,rowoffset,chain4+offset);//��ʾ����
		offset+=32;
		rowoffset+=16;
	}
	*/
}
void showEnglish(int len,u8 * mchain){
	for(i=0;i<len;i++){//len
		if((rowoffset+8)>128){
			pageoffset+=2;
			rowoffset=0;
		}
		Oled_Display_Pic(8,16,pageoffset,rowoffset,mchain+offset);//��ʾ����
		offset+=16;
		rowoffset+=8;	
	}
}
void showHanZi(int len,u8 * mchain){
	for(i=0;i<len;i++){//len
		if((rowoffset+16)>128){
			pageoffset+=2;
			rowoffset=0;
			//delay_ms(100);
			if(pageoffset>=8){
				absoffset+=256;
				pageoffset=0;
				OLED_Clear(0x00); //����
				//fast_clear();
				//return;
			}
		}
		if(absoffset>=(76*32)){
			absoffset=0;
			OLED_Clear(0x00);
		}
		Oled_Display_Pic(16,16,pageoffset,rowoffset,mchain+offset+absoffset);//��ʾ����
		//Oled_Display_Pic(16,16,pageoffset,rowoffset,mchain+offset);//Дʾںؖ
		//delay_ms(100);
		offset+=32;
		rowoffset+=16;
	}
}
/*******************************************************************************
	@ u8 width ��ͼƬ���ַ����   
	@ u8 high��ͼƬ���ַ��߶�
	@ u8 page  ��ҳλ��
	@ u8 col   ����λ��	
	@ u8 *pic  ��ͼƬ���������׵�ַ
********************************************************************************/
//this oled has 128cols*64rows Picels
//
void FlashingdisplayCNString(u8 *str){
		OLED_Clear(0x00); //����
		
}
