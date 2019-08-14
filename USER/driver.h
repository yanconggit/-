#ifndef __DRIVER 
#define __DRIVER
#include "sys.h" 

void Actualspeed(float actualspeed);
void PID_init(void);
float PID_realize(float speed);

void DrawAxis(u8 mode,u16 val,u8 direction,u16 begin,u16 end,u8 copies,u16 num,u8 len,u8 size,u16 color);


extern unsigned char choose[];
















#endif


