2018/12/26
CSL\CMSIS\DeviceSupport\SWM260.h文件中 BTIRM0_IRQn 改正为 BTIMR0_IRQn

2018/12/28
CSL\SWM260_StdPeriph_Driver\SWM260_uart.c文件中 UART_IsTXBusy() 函数代码由
return (UARTx->CTRL & UART_CTRL_TXIDLE_Msk) ? 0 : 1;
改正为：
if(UARTx->FIFO & UART_FIFO_TXLVL_Msk)
{
	return 1;
}
else
{
	if(((UARTx->BAUD & UART_BAUD_TXD_Msk) == 0) || ((UARTx->CTRL & UART_CTRL_TXIDLE_Msk) == 0))
		return 1;
	else
		return 0;
}

2019/02/13
CSL\SWM260_StdPeriph_Driver\SWM260_exti.c 文件中 EXTI_RawState() 中，将
return (GPIOx->INTRAWSTAT >> 1) & 0x01;
修正为：
return (GPIOx->INTRAWSTAT >> n) & 0x01;

2019/02/13
CSL\SWM260_StdPeriph_Driver\SWM260_gpio.c 文件中，将
bits = 0xFFFFFF >> (24 - w);
修正为：
bits = 0xFFFF >> (16 - w);

2019/02/14
CSL\SWM260_StdPeriph_Driver\SWM260_uart.c文件中，UART_SetBaudrate()修正UART4波特率设置

2019/03/04
CSL\SWM260_StdPeriph_Driver\SWM260_i2c.c 文件中，I2C_Init()中将：
if(initStruct->MstIEn)  I2Cx->MSTCR &= ~(1 << I2C_MSTCR_IE_Pos);
else                    I2Cx->MSTCR |=  (1 << I2C_MSTCR_IE_Pos);
修正为：
if(initStruct->MstIEn)  I2Cx->MSTCR |=  (1 << I2C_MSTCR_IE_Pos);
else                    I2Cx->MSTCR &= ~(1 << I2C_MSTCR_IE_Pos);

CSL\SWM260_StdPeriph_Driver\SWM260_timr.c 文件中，TIMR_INTClr()中将：
TIMRx->IF |= (1 << TIMR_IF_TO_Pos);
修正为：
TIMRx->IF = (1 << TIMR_IF_TO_Pos);

CSL\SWM260_StdPeriph_Driver\SWM260_uart.c 文件中，UART_GetBaudrate()中将：
return (UARTx->BAUD & UART_BAUD_BAUD_Msk);
修正为：
if((UARTx == UART0) || (UARTx == UART1) || (UARTx == UART2) || (UARTx == UART3))
	return SystemCoreClock/16/(((UARTx->BAUD & UART_BAUD_BAUD_Msk) >> UART_BAUD_BAUD_Pos) + 1);
else
	return (SystemCoreClock/2)/16/(((UARTx->BAUD & UART_BAUD_BAUD_Msk) >> UART_BAUD_BAUD_Pos) + 1);

2019/03/08
CSL\CMSIS\DeviceSupport\SWM260.h 文件中，CLKSEL寄存器的bit5名称由LPR改为RTC

CSL\SWM260_StdPeriph_Driver\SWM260_rtc.c 文件中，RTC_Init()中添加时钟源选择

2019/05/15
CSL\SWM260_StdPeriph_Driver\SWM260_rtc.c 文件中，修正：Date取值1--31，Month取值1--12

2019/05/20
CSL\CMSIS\DeviceSupport\SWM260.h 文件中 PWMG->FORCEH 寄存器定义更正

2019/06/04
CSL\CMSIS\DeviceSupport\SWM260.h 文件中 SYS->BODCR.LVL 位拆分成 RSTLVL和INTLVL

2019/07/31
CSL\SWM260_StdPeriph_Driver\SWM260_uart.c 文件中 UART_LINIsDetected() 函数中，将：
return (UARTx->LINCR & UART_LINCR_BRKDETIE_Msk) ? 1 : 0;
修正为：
return (UARTx->LINCR & UART_LINCR_BRKDETIF_Msk) ? 1 : 0;

2019/08/21
CSL\SWM260_StdPeriph_Driver\SWM260_flash.c 文件中 FLASH_Write() 函数中，将：
if(addr/2048 != (addr+cnt*4)/2048) return FLASH_RES_ERR;	// 跨页
修正为：
if(addr/2048 != (addr+(cnt-1)*4)/2048) return FLASH_RES_ERR;	// 跨页
解决写入整页时误判跨页导致不执行写操作

ADC开启硬件校准

2019/10/18
CSL\CMSIS\DeviceSupport\SWM260.h 文件中，ADC_TypeDef 定义中将 TRIG 重命名为 TRGMSK

2019/11/25
CSL\CMSIS\DeviceSupport\SWM260.h 文件中，SYS_TypeDef 定义中添加 PRSTEN、PRSTR1、PRSTR2三个寄存器定义

2020/01/15
CSL\SWM260_StdPeriph_Driver\SWM260_adc.c 文件中 ADC_Init() 函数中，将：
ADCx->CTRL3 &= ~(ADC_CTRL3_EXTREF_Msk | ADC_CTRL3_CLKDIV_Msk);
ADCx->CTRL3 |= (ADC_REFP_EXTERNAL << ADC_CTRL3_EXTREF_Pos) | (2 << ADC_CTRL3_CLKDIV_Pos);
修正为：
ADCx->CTRL3 &= ~(ADC_CTRL3_EXTREF_Msk | ADC_CTRL3_CLKDIV1_Msk | ADC_CTRL3_CLKDIV2_Msk);
ADCx->CTRL3 |= (ADC_REFP_EXTERNAL   << ADC_CTRL3_EXTREF_Pos)  |
	           (1                   << ADC_CTRL3_CLKDIV1_Pos) |
			   (initStruct->clk_div << ADC_CTRL3_CLKDIV2_Pos);

2020/03/03
CSL\SWM260_StdPeriph_Driver\SWM260_i2c.c 文件中，添加 I2C_Start、I2C_Stop、I2C_Write、I2C_Read 函数定义

2020/05/12
CSL\SWM260_StdPeriph_Driver\SWM260_flash.c 文件中，FLASH_Erase、FLASH_Write 中执行操作前关闭全局中断
