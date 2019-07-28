#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "key.h"


//ALIENTEK Ì½Ë÷ÕßSTM32F407¿ª·¢°å ÊµÑé9
//PWMÊä³öÊµÑé-¿âº¯Êý°æ±¾
//¼¼ÊõÖ§³Ö£ºwww.openedv.com
//ÌÔ±¦µêÆÌ£ºhttp://eboard.taobao.com  
//¹ãÖÝÊÐÐÇÒíµç×Ó¿Æ¼¼ÓÐÏÞ¹«Ë¾  
//×÷Õß£ºÕýµãÔ­×Ó @ALIENTEK

int main(void)
{ 
	u16 led0pwmval=0;    
	u8 dir=1;
	KEY_Init();       //³õÊ¼»¯Óë°´¼üÁ¬½ÓµÄÓ²¼þ½Ó¿Ú
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ÉèÖÃÏµÍ³ÖÐ¶ÏÓÅÏÈ¼¶·Ö×é2
	delay_init(168);  //³õÊ¼»¯ÑÓÊ±º¯Êý
	uart_init(115200);//³õÊ¼»¯´®¿Ú²¨ÌØÂÊÎª115200
  TIM13_PWM_Init(500-1,84-1);	//84M/84=1MhzµÄ¼ÆÊýÆµÂÊ,ÖØ×°ÔØÖµ500£¬ËùÒÔPWMÆµÂÊÎª 1M/500=2Khz.     
   while(1) //ÊµÏÖ±È½ÏÖµ´Ó0-300µÝÔö£¬µ½300ºó´Ó300-0µÝ¼õ£¬Ñ­»·
	{
		
 		delay_ms(10);	
    		
		if(dir)led0pwmval++;//dir==1 led0pwmvalµÝÔö
		else led0pwmval--;	//dir==0 led0pwmvalµÝ¼õ 
 		if(led0pwmval>490)dir=0;//led0pwmvalµ½´ï300ºó£¬·½ÏòÎªµÝ¼õ
		if(led0pwmval==0)dir=1;	//led0pwmvalµÝ¼õµ½0ºó£¬·½Ïò¸ÄÎªµÝÔö
 
		TIM_SetCompare1(TIM12,led0pwmval);	//ÐÞ¸Ä±È½ÏÖµ£¬ÐÞ¸ÄÕ¼¿Õ±È
	}
}
