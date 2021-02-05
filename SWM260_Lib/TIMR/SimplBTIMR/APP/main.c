#include "SWM260.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//输出，接LED
	
	TIMR_Init(BTIMR0, TIMR_MODE_TIMER, SystemCoreClock/4, 1);	//每0.25秒钟触发一次中断
	
	TIMR_Start(BTIMR0);
	
	while(1==1)
	{
	}
}

void BTIMR0_Handler(void)
{
	static uint32_t cnt = 0;
	
	TIMR_INTClr(BTIMR0);
	
	GPIO_InvBit(GPIOA, PIN5);	//反转LED亮灭状态
	
	if(++cnt == 4)	//前3个周期时长250ms，第四个周期时长125ms
	{
		cnt = 0;
		
		BTIMR0->LOAD = (SystemCoreClock/8) | (1 << TIMR_LOAD_LOAD_Pos);
		BTIMR0->LOAD = (SystemCoreClock/4);
	}
}
