#include "SWM260.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//GPIOA.5 => LED
	
	PORT->PORTA_PULLU |= (1 << PIN6);
	PORT_Init(PORTA, PIN6, PORTA_PIN6_TIMR2_IN, 1);
	
	TIMR_Init(TIMR2, TIMR_MODE_COUNTER, 3, 1);	//ÿ��3�����ؽ����ж�
	
	TIMR_Start(TIMR2);
	
	while(1==1)
	{
	}
}

void TIMR2_Handler(void)
{
	TIMR_Stop(TIMR2);			//�������������������жϣ�������������˳���ܵ�����������ν����жϴ�����
	TIMR_INTClr(TIMR2);
	TIMR_Start(TIMR2);
	
	GPIO_InvBit(GPIOA, PIN5);
}
