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
	delay_init();     //?óê±o¯êy3?ê??¯	  
	uart_init(115200);	 //'??ú3?ê??¯?a115200
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
//	Oled_Display_Char(0,0,'A');//ÏÔÊ¾µ¥¸ö×Ö·û
	
	//There is no luck.There is only work.ÐÒÔËÊÇ²»´æÔÚµÄ£¬Å¬Á¦²ÅÊÇÓ²µÀÀí¡£
	/**
	Oled_Display_String(0,0,buf); //ÏÔÊ¾×Ö·û´®
	Oled_Display_String(2,0,buf1); //ÏÔÊ¾×Ö·û´®
	Oled_Display_String(4,0,buf2); //ÏÔÊ¾×Ö·û´®
	Oled_Display_String(6,80,author);
	*/
//	Oled_Display_Pic(128,64,0,0,pic);//ÏÔÊ¾Í¼Æ¬
	//Oled_Display_Pic(32,32,2,48,chain);//ÏÔÊ¾ºº×Ö
	//Oled_Display_Pic(32,32,2,48,chain2);//ÏÔÊ¾ºº×Ö
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
		//OLED_Clear(0x00); //ÇåÆÁ
		//Oled_Display_Pic(32,32,2,48,chain);//ÏÔÊ¾ºº×Ö
		//Oled_Display_Pic(32,32,2,48,chain2);//ÏÔÊ¾ºº×Ö
		//showMyName();
		
		//Oled_Display_Pic(16,16,3,16*2,chain3+32*2);//ÏÔÊ¾ºº×Ö
	}
}

void showMyName(){
	for(i=0;i<3;i++){
		Oled_Display_Pic(16,16,0,16*i,chain3+32*i);//ÏÔÊ¾ºº×Ö
	}
	/**
	for(i=0;i<12;i++){
		Oled_Display_Pic(8,16,3,8*i,chain3+16*(i+7));//ÏÔÊ¾ºº×Ö
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
		//Oled_Display_Pic(8,16,0,8*i,chain4+offset);//ÏÔÊ¾ºº×Ö
		//Oled_Display_Pic(8,16,0,rowoffset,chain4+offset);//ÏÔÊ¾ºº×Ö
		if((rowoffset+8)>128){
			pageoffset+=2;
			rowoffset=0;
		}
		Oled_Display_Pic(8,16,pageoffset,rowoffset,chain4+offset);//ÏÔÊ¾ºº×Ö
		offset+=16;
		rowoffset+=8;	
	}
	for(i=0;i<14;i++){//14
		//Oled_Display_Pic(16,16,2,16*i,chain4+offset);//ÏÔÊ¾ºº×Ö
		if((rowoffset+16)>128){
			pageoffset+=2;
			rowoffset=0;
		}
		Oled_Display_Pic(16,16,pageoffset,rowoffset,chain4+offset);//ÏÔÊ¾ºº×Ö
		offset+=32;
		rowoffset+=16;
	}
	for(i=0;i<3;i++){//205
		if((rowoffset+8)>128){
			pageoffset+=2;
			rowoffset=0;
		}
		Oled_Display_Pic(8,16,pageoffset,rowoffset,chain4+offset);//ÏÔÊ¾ºº×Ö
		offset+=16;
		rowoffset+=8;	
	}
	for(i=0;i<4;i++){//4
		if((rowoffset+16)>128){
			pageoffset+=2;
			rowoffset=0;
		}
		Oled_Display_Pic(16,16,pageoffset,rowoffset,chain4+offset);//ÏÔÊ¾ºº×Ö
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
		Oled_Display_Pic(8,16,pageoffset,rowoffset,mchain+offset);//ÏÔÊ¾ºº×Ö
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
				OLED_Clear(0x00); //ÇåÆÁ
				//fast_clear();
				//return;
			}
		}
		if(absoffset>=(76*32)){
			absoffset=0;
			OLED_Clear(0x00);
		}
		Oled_Display_Pic(16,16,pageoffset,rowoffset,mchain+offset+absoffset);//ÏÔÊ¾ºº×Ö
		//Oled_Display_Pic(16,16,pageoffset,rowoffset,mchain+offset);//Ð”Ê¾ÚºØ–
		//delay_ms(100);
		offset+=32;
		rowoffset+=16;
	}
}
/*******************************************************************************
	@ u8 width £ºÍ¼Æ¬»ò×Ö·û¿í¶È   
	@ u8 high£ºÍ¼Æ¬»ò×Ö·û¸ß¶È
	@ u8 page  £ºÒ³Î»ÖÃ
	@ u8 col   £ºÁÐÎ»ÖÃ	
	@ u8 *pic  £ºÍ¼Æ¬»òºº×ÖÊý×éÊ×µØÖ·
********************************************************************************/
//this oled has 128cols*64rows Picels
//
void FlashingdisplayCNString(u8 *str){
		OLED_Clear(0x00); //ÇåÆÁ
		
}
