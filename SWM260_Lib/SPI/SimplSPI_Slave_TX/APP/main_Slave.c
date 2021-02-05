#include "SWM260.h"

uint16_t *SPI1TXBuff = 0;
uint32_t SPI1TXCount = 0;
uint32_t SPI1TXIndex = 0;

void SerialInit(void);
void SPISlvInit(void);
void SPISlvSend(uint16_t buff[], uint32_t cnt);

int main(void)
{	
	uint16_t txbuff[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	
	SystemInit();
	
	SerialInit();	
	
	SPISlvInit();
	
	SPISlvSend(txbuff, 16);

	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);	// �жϺ�������ָʾ

	while(1==1)
	{
	}
}


void SPISlvInit(void)
{
	SPI_InitStructure SPI_initStruct;
	
	PORT_Init(PORTA, PIN10, PORTA_PIN10_SPI1_SSEL, 1);
	PORT_Init(PORTA, PIN11, PORTA_PIN11_SPI1_SCLK, 1);
	PORT_Init(PORTA, PIN12, PORTA_PIN12_SPI1_MOSI, 1);
	PORT_Init(PORTA, PIN13, PORTA_PIN13_SPI1_MISO, 0);
	
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_HIGH_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 0;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold = 4;		//����FIFO�����ݸ�����CTRL.TFTHRʱ�����ж�
	SPI_initStruct.TXThresholdIEn = 1;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI1, &SPI_initStruct);
	
	SPI_Open(SPI1);
	
	
	/* ��PA10�������������ڼ��SPI�ӻ���CS Assert�¼� */
	GPIO_Init(GPIOA, PIN9, 0, 1, 0, 0);			//���룬����ʹ��
	
	EXTI_Init(GPIOA, PIN9, EXTI_FALL_EDGE);		//�½��ش����ж�
	
	NVIC_EnableIRQ(GPIOA9_GPIOC6_IRQn);
		
	EXTI_Open(GPIOA, PIN9);
}

void SPISlvSend(uint16_t buff[], uint32_t cnt)
{
	SPI1TXBuff = buff;
	SPI1TXCount = cnt;
}

void SPI1_HALL_GPIOD_Handler(void)
{
	if(SPI1->IF & SPI_IF_TFTHR_Msk)
	{
		while(SPI1->STAT & SPI_STAT_TFNF_Msk)
		{
			if(SPI1TXIndex < SPI1TXCount)
			{
				SPI1->DATA = SPI1TXBuff[SPI1TXIndex++];
			}
			else
			{
				SPI1->DATA = 0xFF;	// û�����ˣ�����0xFF
			}
		}
		
		SPI1->IF = (1 << SPI_IF_TFTHR_Pos);
	}
}

void GPIOA9_GPIOC6_Handler(void)
{	
	EXTI_Clear(GPIOA, PIN9);
	
	SPI1TXIndex = 0;
	
	SPI1->CTRL |=  (1 << SPI_CTRL_TFCLR_Pos);	// ����FIFO���
	SPI1->CTRL &= ~(1 << SPI_CTRL_TFCLR_Pos);
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTC, PIN13, PORTC_PIN13_UART0_TX, 0);	//GPIOC.13����ΪUART0 TXD
	PORT_Init(PORTC, PIN14, PORTC_PIN14_UART0_RX, 1);	//GPIOC.14����ΪUART0 RXD
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}