#ifndef __DRIVER 
#define __DRIVER
#include "sys.h" 

void Actualspeed(float actualspeed);
void PID_init(void);
float PID_realize(float speed);

void DrawAxis(u8 mode,u16 val,u8 direction,u16 begin,u16 end,u8 copies,u16 num,u8 len,u8 size,u16 color);


extern unsigned char choose[];




extern float p,i,d;
extern float speed;
extern long anglebuf;
extern char uartbuf[];
extern unsigned int angle;
extern void TIM13_PWM_Init(u32 arr,u32 psc);


void KEY(void);
void uart(void);
void Draw(void);
void pid1(void);
void touch(void);
void remote(void);
void AllInit(void);
void touchscreen(void);
void anglecontrol(long angle);




#endif


