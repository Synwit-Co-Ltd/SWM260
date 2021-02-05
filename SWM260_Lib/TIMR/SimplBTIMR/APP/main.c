#include "SWM260.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//�������LED
	
	TIMR_Init(BTIMR0, TIMR_MODE_TIMER, SystemCoreClock/4, 1);	//ÿ0.25���Ӵ���һ���ж�
	
	TIMR_Start(BTIMR0);
	
	while(1==1)
	{
	}
}

void BTIMR0_Handler(void)
{
	static uint32_t cnt = 0;
	
	TIMR_INTClr(BTIMR0);
	
	GPIO_InvBit(GPIOA, PIN5);	//��תLED����״̬
	
	if(++cnt == 4)	//ǰ3������ʱ��250ms�����ĸ�����ʱ��125ms
	{
		cnt = 0;
		
		BTIMR0->LOAD = (SystemCoreClock/8) | (1 << TIMR_LOAD_LOAD_Pos);
		BTIMR0->LOAD = (SystemCoreClock/4);
	}
}
