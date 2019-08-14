#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "timer.h"
#include "pwm.h"
#include "driver.h"
#include "lcd.h"
#include "stmflash.h"
#include  "24cxx.h"
#include "string.h"
#include "remote.h"
#include "stdio.h"
#include "stdlib.h"
#include "touch.h" 
void touchscreen();


extern float p,i,d;

extern long anglebuf;
extern unsigned int angle;
extern void TIM13_PWM_Init(u32 arr,u32 psc);
extern char uartbuf[];
extern float speed;
unsigned char setspeed = 50;
u8 TEXT_Buffer[4]={"0"};

unsigned long pwm;
char _pidbuf[6];
void AllInit(void);
void uart(void);
void Draw(void);
void pid1(void);
void anglecontrol(long angle);
void remote(void);
void touch(void);
u8 choose[10]="0";
//extern struct  strpid pid;
void KEY();


void Load_Drow_Dialog(void)
{
	/*LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  	POINT_COLOR=RED;//设置画笔蓝色 */
}


//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  

//电阻触摸屏测试函数
void rtp_test(void)
{  
	while(1)
	{
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Drow_Dialog();//清除
				//else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//画图	  			   
			}
		}else delay_ms(10);	//没有按键按下的时候 	    
		/*if(key==KEY0_PRES)	//KEY0按下,则执行校准程序
		{
			LCD_Clear(WHITE);	//清屏
		    TP_Adjust();  		//屏幕校准 
			TP_Save_Adjdata();	 
			Load_Drow_Dialog();
		}*/
	}
}










int main(void)
{ 
	
	AllInit();		//初始化
	

	while(1)
	{
		Draw();		//显示部分
		pid1();
		uart();		//驱动
		remote();
		touch();
		
		if(setspeed>115)
			setspeed=115;
		
		 KEY();
		
	}
}


void KEY()
{
	u8 temp[6]="0VAPID";
	static u8 tempindex=0;
	u8 key=0;
	key=KEY_Scan(0);
	if(key)
	{						   
		switch(key)
		{				 
			case WKUP_PRES:{u8 _pidbuf[10]="0";
										switch(choose[0])
										{
											case 'V':setspeed++;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",setspeed);sprintf(TEXT_Buffer,"%d",setspeed);AT24CXX_Write(0,(u8*)TEXT_Buffer,4);break;
											//case 'A':setspeed++;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",setspeed);break;
											case 'P':p+=0.01;sprintf(_pidbuf,"%f",p);
															AT24CXX_Write(4,_pidbuf,5);
															LCD_Fill(20,48,64,60,WHITE);
															Printf(20,48,16,"%f",p);break;
											case 'I':i+=0.01;sprintf(_pidbuf,"%f",i);
															AT24CXX_Write(10,_pidbuf,5);
															LCD_Fill(80,48,124,60,WHITE);
															Printf(80,48,16,"%f",i);break;
											case 'D':d+=0.01;sprintf(_pidbuf,"%f",d);
															AT24CXX_Write(16,_pidbuf,5);
															LCD_Fill(140,48,180,60,WHITE);
															Printf(140,48,16,"%f",d);break;
										}}break;
			case KEY0_PRES:{choose[0] = '0';
											LCD_Fill(84,23,97,48,WHITE);}break;
			case KEY1_PRES:{
											u8 _pidbuf[10]="0";
											switch(choose[0])
											{
												case 'V':setspeed--;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",setspeed);sprintf(TEXT_Buffer,"%d",setspeed);AT24CXX_Write(0,(u8*)TEXT_Buffer,4);break;
												//case 'A':setspeed++;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",setspeed);break;
												case 'P':p-=0.01;sprintf(_pidbuf,"%f",p);
																AT24CXX_Write(4,_pidbuf,5);
																LCD_Fill(20,48,64,60,WHITE);
																Printf(20,48,16,"%f",p);break;
												case 'I':i-=0.01;sprintf(_pidbuf,"%f",i);
																AT24CXX_Write(10,_pidbuf,5);
																LCD_Fill(80,48,124,60,WHITE);
																Printf(80,48,16,"%f",i);break;
												case 'D':d-=0.01;sprintf(_pidbuf,"%f",d);
																AT24CXX_Write(16,_pidbuf,5);
																LCD_Fill(140,48,180,60,WHITE);
																Printf(140,48,16,"%f",d);break;
											}}break;
			case KEY2_PRES:tempindex++;choose[0]=temp[tempindex];switch(choose[0])		//下一个
											{
												case 'V':LCD_Fast_ShowChar(85,24,'V',24,0,RED);break;
												case 'P':LCD_Fast_ShowChar(85,24,'P',24,0,RED);break;
												case 'I':LCD_Fast_ShowChar(85,24,'I',24,0,RED);break;
												case 'D':LCD_Fast_ShowChar(85,24,'D',24,0,RED);break;
											}break;	
		}
	}
}
void touch()
{
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>190&&tp_dev.y[0]<70)
				{
					if(tp_dev.y[0]>0&&tp_dev.y[0]<26)//UP
					{
						u8 _pidbuf[10]="0";
						switch(choose[0])
						{
							case 'V':setspeed++;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",setspeed);sprintf(TEXT_Buffer,"%d",setspeed);AT24CXX_Write(0,(u8*)TEXT_Buffer,4);break;
							//case 'A':setspeed++;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",setspeed);break;
							case 'P':p+=0.01;sprintf(_pidbuf,"%f",p);
											AT24CXX_Write(4,_pidbuf,5);
											LCD_Fill(20,48,64,60,WHITE);
											Printf(20,48,16,"%f",p);break;
							case 'I':i+=0.01;sprintf(_pidbuf,"%f",i);
											AT24CXX_Write(10,_pidbuf,5);
											LCD_Fill(80,48,124,60,WHITE);
											Printf(80,48,16,"%f",i);break;
							case 'D':d+=0.01;sprintf(_pidbuf,"%f",d);
											AT24CXX_Write(16,_pidbuf,5);
											LCD_Fill(140,48,180,60,WHITE);
											Printf(140,48,16,"%f",d);break;
						}
						
					}
					else if(tp_dev.y[0]>26&&tp_dev.y[0]<48)//DOEN
					{
						u8 _pidbuf[10]="0";
						switch(choose[0])
						{
							case 'V':setspeed--;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",setspeed);sprintf(TEXT_Buffer,"%d",setspeed);AT24CXX_Write(0,(u8*)TEXT_Buffer,4);break;
							//case 'A':setspeed++;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",setspeed);break;
							case 'P':p-=0.01;sprintf(_pidbuf,"%f",p);
											AT24CXX_Write(4,_pidbuf,5);
											LCD_Fill(20,48,64,60,WHITE);
											Printf(20,48,16,"%f",p);break;
							case 'I':i-=0.01;sprintf(_pidbuf,"%f",i);
											AT24CXX_Write(10,_pidbuf,5);
											LCD_Fill(80,48,124,60,WHITE);
											Printf(80,48,16,"%f",i);break;
							case 'D':d-=0.01;sprintf(_pidbuf,"%f",d);
											AT24CXX_Write(16,_pidbuf,5);
											LCD_Fill(140,48,180,60,WHITE);
											Printf(140,48,16,"%f",d);break;
						}
					}
					else if(tp_dev.y[0]>48&&tp_dev.y[0]<70)//ENTER
					{
						choose[0] = '0';
						LCD_Fill(84,23,97,48,WHITE);
					}
					
				}
				else if(tp_dev.x[0]>130&&tp_dev.y[0]<48)
				{
					if(tp_dev.y[0]>0&&tp_dev.y[0]<26)
						{
							choose[0]='V';
							LCD_Fast_ShowChar(85,24,'V',24,0,RED);
						}
					else if(tp_dev.y[0]>26&&tp_dev.y[0]<48)
					{
						choose[0]='A';
						LCD_Fast_ShowChar(85,24,'A',24,0,RED);
					}
				}
				else if(tp_dev.y[0]>48&&tp_dev.y[0]<70)
				{
					if(tp_dev.x[0]>0&&tp_dev.x[0]<60)
						{
							choose[0]='P';
							LCD_Fast_ShowChar(85,24,'P',24,0,RED);
						}
					else if(tp_dev.x[0]>60&&tp_dev.x[0]<125)
					{
						choose[0]='I';
						LCD_Fast_ShowChar(85,24,'I',24,0,RED);
					}
					else if(tp_dev.x[0]>125&&tp_dev.x[0]<190)
					{
						choose[0]='D';
						LCD_Fast_ShowChar(85,24,'D',24,0,RED);
					}
				}
			}
		}
}

void remote()
{
	static u8 key = 0;           //保存键值
	if(key!=Remote_Scan())
		{
			switch(Remote_Scan())
			{
				static u8 setspeedtemp,cishu=0;
				
				case 162:cishu=~cishu;if(cishu){setspeedtemp=setspeed;setspeed=0;}if(setspeed==0&&cishu==0){setspeed=setspeedtemp;}key=162;break;//power
			  case 168:setspeed--;key=168;break;//down   
				case 98:setspeed++;break;	    //up
				default:key=1;break;
			/*	case 2:str="PLAY";break;		 
				case 226:str="ALIENTEK";break;		  
				case 194:str="RIGHT";break;	   
				case 34:str="LEFT";break;		  
				case 224:str="VOL-";break;		  
					case 0:str="ERROR";break;			   
						   
				case 144:str="VOL+";break;	*/	    
				case 104:setspeed=10;break;//1		  
				case 152:setspeed=20;break;	   //2
				case 176:setspeed=30;;break;	    //3
				case 48:setspeed=40;;break;		  //4  
				case 24:setspeed=50;break;		    //5
				case 122:setspeed=60;break;		  //6
				case 16:setspeed=70;break;			//7   					
				case 56:setspeed=80;;break;	 //8
				case 90:setspeed=90;break;//9
				case 66:setspeed=100;break;//0
				//case 82:str="DELETE";break;		 
			}
			LCD_Fill(150,8,174,24,WHITE);		
			Printf(150,8,16,"%d",setspeed);
		}
}

void anglecontrol(long angle)
{
	long grid;
	grid = angle*0.93;
	pwm = 30;
	TIM_SetCompare1(TIM12,pwm);	//修改比较值，修改占空比
	while(anglebuf <= grid-100)
	{
		Draw();
		pwm += PID_realize(setspeed);		//pid算法控制
		if(pwm>=100)
			pwm = 99;
		TIM_SetCompare1(TIM12,pwm);	//修改比较值，修改占空比
	}

	TIM_SetCompare1(TIM12,0);	//修改比较值，修改占空比
	//while(1);
	
}
void AllInit()
{
	u8 datatemp[4];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200); 	//串口初始化 
	LED_Init();				  //初始化LED端口 
	KEY_Init();       //初始化与按键连接的硬件接口	
	PID_init();				//PID参数初始化
	AT24CXX_Init();			//IIC初始化 
	BEEP_Init();        //初始化蜂鸣器端口
	EXTIX_Init();       //初始化外部中断输入
	Remote_Init();				//红外接收初始化		
	LCD_Init();           //初始化LCD FSMC接�
	tp_dev.init();				//触摸屏初始化�
	POINT_COLOR=BLACK;      //画笔颜色：黑色
	
	AT24CXX_Read(0,datatemp,4);  //初始化PID及速度目标值
	setspeed = atoi(datatemp);
	AT24CXX_Read(4,datatemp,6);
	p=(float)atof(datatemp);
	AT24CXX_Read(10,datatemp,5);
	i=(float)atof(datatemp);
	AT24CXX_Read(16,datatemp,6);
	d=(float)atof(datatemp);
	

	//定时器初始化
 	TIM3_Int_Init(200-1,8400-1);		//计时测速度//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数200次为20ms     
 	TIM4_Int_Init(20-1,84);		//计时测速度//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数200次为20ms     
	TIM13_PWM_Init(100-1,8400-1); 	//pwm//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.     
	
	//LCD显示初始化
	Printf(15,8,16,"V:");Printf(30,8,16,"%d",100);Printf(62,8,16,"r/s");Printf(110,8,16,"AimV:");Printf(150,8,16,"%d",setspeed);LCD_Fill(190,5,239,26,BLUE);Printf(200,8,16,"Up");	
	Printf(15,28,16,"A:");Printf(30,28,16,"%d",360);LCD_Draw_Circle(66,32,2);	Printf(110,28,16,"AimA:");Printf(200,28,16,"Down");
	Printf(5,48,16,"P:");Printf(20,48,16,"%f",p);Printf(65,48,16,"I:");Printf(80,48,16,"%f",i);Printf(125,48,16,"D:");Printf(140,48,16,"%f",d);LCD_Fill(190,46,239,66,BLUE);Printf(195,48,16,"Enter");
	DrawAxis('y',215,1,320,80,6,120,3,12,RED);//右边的速度轴
	DrawAxis('y',20,0,320,80,6,360,3,12,BLACK);//左边的角度轴
}

void Draw()
{
	static unsigned int ab=23,bc=319,abc=23,bcd=319,y=319,y1=319;
	u8 times=0;
	ab++;  ab++; //ab++;    //扫描参数
	bc=320 - angle*0.67;		//角度
	y= 320-speed*2;						//速度
	if(ab>208)							//到最右边后返回
		ab=23;
	else
	{			
		LCD_DrawLine(abc,bcd,ab,bc);			//画角度波形
		LCD_Fast_DrawLine(abc,y1,ab,y,RED);			//速度波形  红色
		LCD_Fast_DrawLine(abc,y1+1,ab,y+1,RED);			//速度波形加粗
		LCD_Fast_DrawLine(abc,y1-1,ab,y-1,RED);			//速度波形加粗
	}
	abc=ab;bcd=bc;y1=y;				//记录上次的值
	LCD_Fill(ab,80,ab+3,319,WHITE);			//画波形前擦除
		
	
	LCD_Fill(30,8,62,27,WHITE);			//画波形前擦除
	Printf(30,8,16,"%d",(int)speed);//瞬时速度
	LCD_Fill(30,28,62,47,WHITE);			//画波形前擦除
	Printf(30,28,16,"%d",angle);//瞬时速度

		delay_ms(20);
	
}

void pid1()
{
	
	pwm += PID_realize(setspeed);		//pid算法控制
	if(pwm>=100)
		pwm = 99;
	TIM_SetCompare1(TIM12,pwm);	//修改比较值，修改占空比
}




void uart()//串口
{
	if(uartbuf[0]!='*'&&uartbuf[0]!='0')		
	{
		if(uartbuf[0] == 'C')//修改参数
		{
			if(uartbuf[1] == 'V')//修改速度目标值
			{
				setspeed = atoi(uartbuf+2);
				sprintf(TEXT_Buffer,"%d",setspeed);//保存速度设定值
				AT24CXX_Write(0,(u8*)TEXT_Buffer,4);
				LCD_Fill(150,8,174,24,WHITE);	
				Printf(150,8,16,"%d",setspeed);	//显示速度设定值
				strcpy(uartbuf,"**********");
				strcpy(TEXT_Buffer,"00000000000\0");
				
			}
			else if(uartbuf[1] == 'P')//修改P
			{
				p=(float)atof(uartbuf+2);
				sprintf(_pidbuf,"%f",p);
				AT24CXX_Write(4,_pidbuf,5);
				LCD_Fill(20,48,64,60,WHITE);
				Printf(20,48,16,"%f",p);
				strcpy(uartbuf,"**********");
				strcpy(_pidbuf,"00000000000\0");
			}
			else if(uartbuf[1] == 'I')//修改I
			{
				i=(float)atof(uartbuf+2);
				sprintf(_pidbuf,"%f",i);
				AT24CXX_Write(10,_pidbuf,5);
				LCD_Fill(80,48,124,60,WHITE);
				Printf(80,48,16,"%f",i);
				strcpy(uartbuf,"**********");
				strcpy(_pidbuf,"00000000000\0");
			}
			else if(uartbuf[1] == 'D')//修改D
			{
				d=(float)atof(uartbuf+2);
				sprintf(_pidbuf,"%f",d);
				AT24CXX_Write(16,_pidbuf,5);
				LCD_Fill(140,48,180,60,WHITE);
				Printf(140,48,16,"%f",d);
				strcpy(uartbuf,"*********");
				strcpy(_pidbuf,"00000000000\0");
			}
		}
		else	if(uartbuf[0] == 'S')//设定角度
		{
			if(uartbuf[1] == 'A')
			{
				long angle1;
				angle1 = atol(uartbuf+2);
				TIM_SetCompare1(TIM12,0);	//修改比较值，修改占空比
				anglebuf = 0;
				LCD_Fill(150,28,170,44,WHITE);
				Printf(150,28,16,"%d",angle1);
				anglecontrol(angle1);
				setspeed=0;
				strcpy(uartbuf,"****************");
			}
		}
	}
}
