#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "timer.h"
#include "pwm.h"
#include "lcd.h"
#include "math.h"
#include "driver.h"

//PID**************************************************************
struct  strpid{
	float setspeed;
	float actualspeed;
	float err;			//ƫ��ֵ
	float errnext;  //��һ��
	float errlast;	//����ǰ
	//float p,i,d;
}pid;

float p,i,d;

void PID_init()
{
	pid.actualspeed = 0.0;
	pid.err = 0.0;
	pid.errlast = 0.0;
	pid.errnext = 0.0;
	pid.setspeed =	0.0;
	/*p = 0.2;			//
	i = 0.6;			//����  ������������
	d = 0.5;			//΢��  ��Ӧ�źű仯����   ����ƫ�ǰ����  ���ӵ����ٶ�*/
}
void Actualspeed(float actualspeed)
{
	pid.actualspeed = actualspeed;
}
float PID_realize(float speed)
	{
		pid.setspeed=speed; 
		pid.err=pid.setspeed-pid.actualspeed; 
		float incrementSpeed=p*(pid.err-pid.errnext)+i*pid.err+d*(pid.err-2*pid.errnext+pid.errlast);
		//pid.actualspeed+=incrementSpeed;
		pid.errlast=pid.errnext; pid.errnext=pid.err; 
		if(speed>=100)
		{
			pid.actualspeed =2*incrementSpeed;
		}
		else pid.actualspeed = incrementSpeed;
		return pid.actualspeed; 
	}

	
	
	
//��������
//mode   'x'/��X' xaxis   'y'/'Y'yaxis
//val    ��һ�������ֵ	��y��ʱΪx������
	//direction  ������������ķ���  0:�����  1���һ���	
//begin  end  ��ʼ/��������  ��������ͷ
//copies ����ּ��ݲ���ԭ��   ������ԭ���Ǽ�������
//num    �������ֵ
//len �������λ��
//size  ���ֵĳߴ�	
//ռ�ô�С  ��val-4,end)  (val+size*len,begin)	
void DrawAxis(u8 mode,u16 val,u8 direction,u16 begin,u16 end,u8 copies,u16 num,u8 len,u8 size,u16 color)
{	
	if(mode=='y'||mode=='Y')
	{
		int temp=0;
		u16 onecopie = 0,onenum = num/copies;
		int times = copies;
		LCD_Fast_DrawLine(val,begin,val,end,color);   //����ֱ��
		LCD_Fast_DrawLine(val-5,end+7,val,end,color);LCD_Fast_DrawLine(val+5,end+7,val,end,color); //����ͷ
		
		temp=begin - end;
		
		onecopie=temp/copies ;  //ÿ�����ز�
		if(direction)
		{
			for(;times>=0;times--)
			{
				LCD_Fast_DrawLine(val-4,begin-onecopie*times,val,begin-onecopie*times,color);
				LCD_Fast_ShowxNum(val+3,begin-onecopie*times-6,num-(6-times)*onenum,len,size,1,color);
			}
		}
		else
		{
			for(;times>=0;times--)
			{
				LCD_Fast_DrawLine(val+4,begin-onecopie*times,val,begin-onecopie*times,color);
				LCD_Fast_ShowxNum(val-20,begin-onecopie*times-6,num-(6-times)*onenum,len,size,1,color);
			}
		}
	}
	else if(mode=='x'||mode=='X')
	{
		int temp=0;
		u16 onecopie = 0,onenum = num/copies;
		int times = copies;
		LCD_Fast_DrawLine(begin,val,end,val,color);   //����ֱ��
		LCD_Fast_DrawLine(end+7,val-5,end,val,color);LCD_Fast_DrawLine(end+7,val+5,end,val,color); //����ͷ
		
		temp=begin - end;
		onecopie=temp/copies ;  //ÿ�����ز�
		if(direction)
		{
			for(;times>=0;times--)
			{
				LCD_Fast_DrawLine(begin-onecopie*times,val+4,begin-onecopie*times,val,color);
				LCD_Fast_ShowxNum(begin-onecopie*times-6,val-15,num-(6-times)*onenum,len,size,1,color);
			}
		}
		else 
		{
			for(;times>=0;times--)
			{
				LCD_Fast_DrawLine(begin-onecopie*times,val-4,begin-onecopie*times,val,color);
				LCD_Fast_ShowxNum(begin-onecopie*times-6,val+3,num-(6-times)*onenum,len,size,1,color);
			}
		}
	}
}










