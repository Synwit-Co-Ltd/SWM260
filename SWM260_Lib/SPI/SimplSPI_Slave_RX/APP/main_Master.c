#include "SWM260.h"

uint16_t SPI1RXBuffer[32] = {0};
uint32_t SPI1RXIndex = 0;

void SerialInit(void);
void SPIMstInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();	
	
	SPIMstInit();			// SPI0��SPI1��Ӧ��������
	
	while(1==1)
	{
		GPIO_ClrBit(GPIOA, PIN5);	// SPI_CS_Low()
		for(i = 0; i < 240; i++);	// CS���ͺ���Ҫ��ʱһ���ٷ���
		for(i = 0; i < 16; i++)
		{
			SPI0->DATA = i;
			while((SPI0->STAT & SPI_STAT_TFNF_Msk) == 0);
		}
		while(SPI_IsTXEmpty(SPI0) == 0);
		for(i = 0; i < 120; i++);	// ����FIFO���ѿգ������һ�����ݻ��ڷ�����λ�Ĵ������Ҫ��ʱ�ȴ������ͳ�ȥ
		GPIO_SetBit(GPIOA, PIN5);	// SPI_CS_High()
		
		for(i = 0; i < SystemCoreClock/10; i++);
	}
}


void SPIMstInit(void)
{
	SPI_InitStructure SPI_initStruct;
	
//	PORT_Init(PORTA, PIN5, PORTA_PIN5_SPI0_SSEL, 0);
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);					//�������Ƭѡ
#define SPI_CS_Low()	GPIO_ClrBit(GPIOA, PIN5)
#define SPI_CS_High()	GPIO_SetBit(GPIOA, PIN5)
	SPI_CS_High();
	
	PORT_Init(PORTA, PIN6, PORTA_PIN6_SPI0_MISO, 1);
	PORT_Init(PORTA, PIN7, PORTA_PIN7_SPI0_MOSI, 0);
	PORT_Init(PORTA, PIN8, PORTA_PIN8_SPI0_SCLK, 0);
	
	SPI_initStruct.clkDiv = SPI_CLKDIV_32;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_HIGH_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 1;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	SPI_Open(SPI0);
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
