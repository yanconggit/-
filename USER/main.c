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


unsigned char setspeed = 50;
u8 TEXT_Buffer[4]={"0"};
u8 choose[10]="0";
unsigned long pwm;
char _pidbuf[6];


int main(void)
{ 
	
	AllInit();		//��ʼ��
	while(1)
	{
		Draw();		//��ʾ����
		pid1();			//PID�ٶȿ���
		uart();		//��������
		remote(); //�������
		touch();	//���������
		KEY(); //�������
	}
}


void KEY(void) //���������� �ϼ� K0��ȷ�� K1����  K2����һ��
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
											case 'V':setspeed++;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",3,setspeed);sprintf(TEXT_Buffer,"%d",setspeed);AT24CXX_Write(0,(u8*)TEXT_Buffer,4);break;
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
												case 'V':setspeed--;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",3,setspeed);sprintf(TEXT_Buffer,"%d",setspeed);AT24CXX_Write(0,(u8*)TEXT_Buffer,4);break;
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
			case KEY2_PRES:tempindex++;choose[0]=temp[tempindex];switch(choose[0])		//��һ��
											{
												case 'V':LCD_Fast_ShowChar(85,24,'V',24,0,RED);break;
												case 'P':LCD_Fast_ShowChar(85,24,'P',24,0,RED);break;
												case 'I':LCD_Fast_ShowChar(85,24,'I',24,0,RED);break;
												case 'D':LCD_Fast_ShowChar(85,24,'D',24,0,RED);break;
											}break;	
		}
		if(setspeed>115)
			setspeed=115;
	}
}
void touch(void)//������������
{
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
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
						case 'V':setspeed++;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",3,setspeed);sprintf(TEXT_Buffer,"%d",setspeed);AT24CXX_Write(0,(u8*)TEXT_Buffer,4);break;
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
						case 'V':setspeed--;LCD_Fill(150,8,174,24,WHITE);	Printf(150,8,16,"%d",3,setspeed);sprintf(TEXT_Buffer,"%d",setspeed);AT24CXX_Write(0,(u8*)TEXT_Buffer,4);break;
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
		if(setspeed>115)
		setspeed=115;
	}
}

void remote(void)//Զ�̿��ƣ����⣩
{
	static u8 key = 0;           //�����ֵ
	if(key!=Remote_Scan())
		{
			switch(Remote_Scan())
			{
				static u8 setspeedtemp,cishu=0;
				
				case 162:cishu=~cishu;if(cishu){setspeedtemp=setspeed;setspeed=0;}if(setspeed==0&&cishu==0){setspeed=setspeedtemp;}key=162;break;//power��֧���ظ���ͣ������
			  case 168:setspeed--;key=168;break;//down   �� ��֧���ظ�
				case 98:setspeed++;break;	    //up ��
				default:key=1;break;
			/*	case 2:str="PLAY";break;		 
				case 226:str="ALIENTEK";break;		  
				case 194:str="RIGHT";break;	   
				case 34:str="LEFT";break;		  
				case 224:str="VOL-";break;		  
					case 0:str="ERROR";break;			   
						   
				case 144:str="VOL+";break;	*/	    
				case 104:setspeed=10;break;//1		 		 �ٶ��趨10
				case 152:setspeed=20;break;	   //2			�ٶ��趨20
				case 176:setspeed=30;;break;	    //3		�ٶ��趨30
				case 48:setspeed=40;;break;		  //4  		�ٶ��趨40
				case 24:setspeed=50;break;		    //5		�ٶ��趨50
				case 122:setspeed=60;break;		  //6			�ٶ��趨60
				case 16:setspeed=70;break;			//7   	�ٶ��趨70
				case 56:setspeed=80;;break;	 //8				�ٶ��趨80
				case 90:setspeed=90;break;//9						�ٶ��趨90
				case 66:setspeed=100;break;//0					�ٶ��趨100
				//case 82:str="DELETE";break;		 
			}
			sprintf(TEXT_Buffer,"%d",setspeed);
			AT24CXX_Write(0,(u8*)TEXT_Buffer,4);
			LCD_Fill(150,8,174,24,WHITE);		
			Printf(150,8,16,"%d",3,setspeed);  //�����ٶ���ʾ
		}
}

void anglecontrol(long angle)//�Ƕȿ��ƺ����������б����ã�
{
	long grid;
	grid = angle*0.93;
	pwm = 30;
	TIM_SetCompare1(TIM12,pwm);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	while(anglebuf <= grid-100)
	{
		Draw();
		pwm += PID_realize(setspeed);		//pid�㷨����
		if(pwm>=100)
			pwm = 99;
		TIM_SetCompare1(TIM12,pwm);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	}

	TIM_SetCompare1(TIM12,0);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	//while(1);
	
}
void AllInit(void)//��ʼ������
{
	u8 datatemp[4];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200); 	//���ڳ�ʼ�� 
	LED_Init();				  //��ʼ��LED�˿� 
	KEY_Init();       //��ʼ���밴�����ӵ�Ӳ���ӿ�	
	PID_init();				//PID������ʼ��
	AT24CXX_Init();			//IIC��ʼ�� 
	BEEP_Init();        //��ʼ���������˿�
	EXTIX_Init();       //��ʼ���ⲿ�ж�����
	Remote_Init();				//������ճ�ʼ��	
	tp_dev.init();				//��������ʼ��
	
	AT24CXX_Read(0,datatemp,4);  //��ʼ��PID���ٶ�Ŀ��ֵ
	setspeed = atoi(datatemp);
	AT24CXX_Read(4,datatemp,6);
	p=(float)atof(datatemp);
	AT24CXX_Read(10,datatemp,5);
	i=(float)atof(datatemp);
	AT24CXX_Read(16,datatemp,6);
	d=(float)atof(datatemp);
	
	LCD_Init();           //��ʼ��LCD FSMC�ӿ�
	POINT_COLOR=BLACK;      //������ɫ����ɫ
	
	
	

	//��ʱ����ʼ��
 	TIM3_Int_Init(200-1,8400-1);		//��ʱ���ٶ�//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����200��Ϊ20ms     
 	TIM4_Int_Init(20-1,84);		//��ʱ���ٶ�//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����200��Ϊ20ms     
	TIM13_PWM_Init(100-1,8400-1); 	//pwm//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.     
	
	//LCD��ʾ��ʼ��
	Printf(15,8,16,"V:");Printf(30,8,16,"%D",3,101);Printf(62,8,16,"r/s");Printf(110,8,16,"AimV:");Printf(150,8,16,"%d",3,setspeed);LCD_Fill(190,5,239,26,BLUE);Printf(200,8,16,"Up");	
	Printf(15,28,16,"A:");Printf(30,28,16,"%d",3,360);LCD_Draw_Circle(66,32,2);	Printf(110,28,16,"AimA:");Printf(200,28,16,"Down");
	Printf(5,48,16,"P:");Printf(20,48,16,"%f",p);Printf(65,48,16,"I:");Printf(80,48,16,"%f",i);Printf(125,48,16,"D:");Printf(140,48,16,"%f",d);LCD_Fill(190,46,239,66,BLUE);Printf(195,48,16,"Enter");
	DrawAxis('y',215,1,320,80,6,120,3,12,RED);//�ұߵ��ٶ���
	DrawAxis('y',20,0,320,80,6,360,3,12,BLACK);//��ߵĽǶ���
}

void Draw(void)//������º���
{
	static unsigned int ab=23,bc=319,abc=23,bcd=319,y=319,y1=319;
	u8 times=0;
	ab++;  ab++; //ab++;    //ɨ�����
	bc=320 - angle*0.67;		//�Ƕ�
	y= 320-speed*2;						//�ٶ�
	if(ab>208)							//�����ұߺ󷵻�
		ab=23;
	else
	{			
		LCD_DrawLine(abc,bcd,ab,bc);			//���ǶȲ���
		LCD_Fast_DrawLine(abc,y1,ab,y,RED);			//�ٶȲ���  ��ɫ
		LCD_Fast_DrawLine(abc,y1+1,ab,y+1,RED);			//�ٶȲ��μӴ�
		LCD_Fast_DrawLine(abc,y1-1,ab,y-1,RED);			//�ٶȲ��μӴ�
	}
	abc=ab;bcd=bc;y1=y;				//��¼�ϴε�ֵ
	LCD_Fill(ab,80,ab+3,319,WHITE);			//������ǰ����
		
	
	LCD_Fill(30,8,62,27,WHITE);			//������ǰ����
	Printf(30,8,16,"%d",3,(int)speed);//˲ʱ�ٶ�
	LCD_Fill(30,28,62,47,WHITE);			//������ǰ����
	Printf(30,28,16,"%d",3,angle);//˲ʱ�ٶ�

	delay_ms(20);
	
}

void pid1(void)//pid����
{
	
	pwm += PID_realize(setspeed);		//pid�㷨����
	if(pwm>=100)
		pwm = 99;
	TIM_SetCompare1(TIM12,pwm);	//�޸ıȽ�ֵ���޸�ռ�ձ�
}




void uart()//����
{
	if(uartbuf[0]!='*'&&uartbuf[0]!='0')		
	{
		if(uartbuf[0] == 'C')//�޸Ĳ���
		{
			if(uartbuf[1] == 'V')//�޸��ٶ�Ŀ��ֵ
			{
				setspeed = atoi(uartbuf+2);
				sprintf(TEXT_Buffer,"%d",setspeed);//�����ٶ��趨ֵ
				AT24CXX_Write(0,(u8*)TEXT_Buffer,4);
				LCD_Fill(150,8,174,24,WHITE);	
				Printf(150,8,16,"%d",3,setspeed);	//��ʾ�ٶ��趨ֵ
				strcpy(uartbuf,"**********");
				strcpy(TEXT_Buffer,"00000000000\0");
				if(setspeed>115)
					setspeed=115;
			}
			else if(uartbuf[1] == 'P')//�޸�P
			{
				p=(float)atof(uartbuf+2);
				sprintf(_pidbuf,"%f",p);
				AT24CXX_Write(4,_pidbuf,5);
				LCD_Fill(20,48,64,60,WHITE);
				Printf(20,48,16,"%f",p);
				strcpy(uartbuf,"**********");
				strcpy(_pidbuf,"00000000000\0");
			}
			else if(uartbuf[1] == 'I')//�޸�I
			{
				i=(float)atof(uartbuf+2);
				sprintf(_pidbuf,"%f",i);
				AT24CXX_Write(10,_pidbuf,5);
				LCD_Fill(80,48,124,60,WHITE);
				Printf(80,48,16,"%f",i);
				strcpy(uartbuf,"**********");
				strcpy(_pidbuf,"00000000000\0");
			}
			else if(uartbuf[1] == 'D')//�޸�D
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
		else	if(uartbuf[0] == 'S')//�趨�Ƕ�
		{
			if(uartbuf[1] == 'A')
			{
				long angle1;
				angle1 = atol(uartbuf+2);
				TIM_SetCompare1(TIM12,0);	//�޸ıȽ�ֵ���޸�ռ�ձ�
				anglebuf = 0;
				LCD_Fill(150,28,170,44,WHITE);
				Printf(150,28,16,"%d",3,angle1);
				anglecontrol(angle1);
				setspeed=0;
				strcpy(uartbuf,"****************");
			}
		}
	}
}
