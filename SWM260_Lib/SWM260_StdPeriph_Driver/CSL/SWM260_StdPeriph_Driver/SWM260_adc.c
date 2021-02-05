/****************************************************************************************************************************************** 
* �ļ�����:	SWM260_adc.c
* ����˵��:	SWM260��Ƭ����ADC��ģת��������������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����: V1.0.0		2016��1��30��
* ������¼: 
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology  
*******************************************************************************************************************************************/
#include "SWM260.h"
#include "SWM260_adc.h"


/****************************************************************************************************************************************** 
* ��������: ADC_Init()
* ����˵��:	ADCģ��ת������ʼ��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����Чֵ����ADC0
*			ADC_InitStructure * initStruct		����ADC����ض�ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_Init(ADC_TypeDef * ADCx, ADC_InitStructure * initStruct)
{		
	switch((uint32_t)ADCx)
	{
	case ((uint32_t)ADC0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_ADC0_Pos);
		break;
	}
	
	ADC_Close(ADCx);		//һЩ�ؼ��Ĵ���ֻ����ADC�ر�ʱ����
	
	SYS->CLKSEL &= ~SYS_CLKSEL_ADC_Msk;
	SYS->CLKSEL |= (initStruct->clk_src << SYS_CLKSEL_ADC_Pos);
	
	ADCx->CTRL3 &= ~(ADC_CTRL3_EXTREF_Msk | ADC_CTRL3_CLKDIV1_Msk | ADC_CTRL3_CLKDIV2_Msk);
	ADCx->CTRL3 |= (ADC_REFP_EXTERNAL   << ADC_CTRL3_EXTREF_Pos)  |
	               (1                   << ADC_CTRL3_CLKDIV1_Pos) |
				   (initStruct->clk_div << ADC_CTRL3_CLKDIV2_Pos);
	
	ADCx->CTRL2 &= ~(ADC_CTRL2_ADJH_Msk | ADC_CTRL2_ADJL_Msk);
	ADCx->CTRL2 |= (0 << ADC_CTRL2_ADJH_Pos) | (15 << ADC_CTRL2_ADJL_Pos);
	
	ADCx->CTRL &= ~(               0xFF << ADC_CTRL_CH0_Pos);
	ADCx->CTRL |= (initStruct->channels << ADC_CTRL_CH0_Pos);
	
	ADCx->CTRL &= ~(ADC_CTRL_AVG_Msk | ADC_CTRL_TRIG_Msk | ADC_CTRL_CONT_Msk);
	ADCx->CTRL |= (initStruct->samplAvg << ADC_CTRL_AVG_Pos)  |
				  (initStruct->trig_src << ADC_CTRL_TRIG_Pos) |
				  (initStruct->Continue << ADC_CTRL_CONT_Pos);
	
	ADCx->CALIBSET = *((volatile uint32_t *) 0x4000009C);
	ADCx->CALIBEN = (1 << ADC_CALIBEN_K_Pos) | (1 << ADC_CALIBEN_OFFSET_Pos);
	
	ADCx->IE = 0;
	ADCx->IF = 0x3FFFF;		//����жϱ�־
	
	ADCx->IE |= (((initStruct->EOC_IEn & ADC_CH0) ? 1 : 0) << ADC_IE_CH0EOC_Pos) |
				(((initStruct->EOC_IEn & ADC_CH1) ? 1 : 0) << ADC_IE_CH1EOC_Pos) |
				(((initStruct->EOC_IEn & ADC_CH2) ? 1 : 0) << ADC_IE_CH2EOC_Pos) |
				(((initStruct->EOC_IEn & ADC_CH3) ? 1 : 0) << ADC_IE_CH3EOC_Pos) |
				(((initStruct->EOC_IEn & ADC_CH4) ? 1 : 0) << ADC_IE_CH4EOC_Pos) |
				(((initStruct->EOC_IEn & ADC_CH5) ? 1 : 0) << ADC_IE_CH5EOC_Pos) |
				(((initStruct->EOC_IEn & ADC_CH6) ? 1 : 0) << ADC_IE_CH6EOC_Pos) |
				(((initStruct->EOC_IEn & ADC_CH7) ? 1 : 0) << ADC_IE_CH7EOC_Pos);
	ADCx->IE |= (((initStruct->OVF_IEn & ADC_CH0) ? 1 : 0) << ADC_IE_CH0OVF_Pos) |
				(((initStruct->OVF_IEn & ADC_CH1) ? 1 : 0) << ADC_IE_CH1OVF_Pos) |
				(((initStruct->OVF_IEn & ADC_CH2) ? 1 : 0) << ADC_IE_CH2OVF_Pos) |
				(((initStruct->OVF_IEn & ADC_CH3) ? 1 : 0) << ADC_IE_CH3OVF_Pos) |
				(((initStruct->OVF_IEn & ADC_CH4) ? 1 : 0) << ADC_IE_CH4OVF_Pos) |
				(((initStruct->OVF_IEn & ADC_CH5) ? 1 : 0) << ADC_IE_CH5OVF_Pos) |
				(((initStruct->OVF_IEn & ADC_CH6) ? 1 : 0) << ADC_IE_CH6OVF_Pos) |
				(((initStruct->OVF_IEn & ADC_CH7) ? 1 : 0) << ADC_IE_CH7OVF_Pos);
	
	switch((uint32_t)ADCx)
	{
	case ((uint32_t)ADC0):		
		if(initStruct->EOC_IEn | initStruct->OVF_IEn) NVIC_EnableIRQ(ADC0_IRQn);
		break;
	}
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Open()
* ����˵��:	ADC����������������������Ӳ������ADCת��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_Open(ADC_TypeDef * ADCx)
{
	ADCx->CTRL |= (0x01 << ADC_CTRL_EN_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Close()
* ����˵��:	ADC�رգ��޷�������������Ӳ������ADCת��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_Close(ADC_TypeDef * ADCx)
{
	ADCx->CTRL &= ~(0x01 << ADC_CTRL_EN_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Start()
* ����˵��:	��������ģʽ������ADCת��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_Start(ADC_TypeDef * ADCx)
{
	ADCx->START |= (0x01 << ADC_START_GO_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Stop()
* ����˵��:	��������ģʽ��ֹͣADCת��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_Stop(ADC_TypeDef * ADCx)
{									 
	ADCx->START &= ~(0x01 << ADC_START_GO_Pos);
}

static uint32_t chn2idx(uint32_t chn)
{
	uint32_t idx = 0;
	
	switch(chn)
	{
		case 0x01: idx = 0; break;
		case 0x02: idx = 1; break;
		case 0x04: idx = 2; break;
		case 0x08: idx = 3; break;
		case 0x10: idx = 4; break;
		case 0x20: idx = 5; break;
		case 0x40: idx = 6; break;
		case 0x80: idx = 7; break;
	}
	
	return idx;
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Read()
* ����˵��:	��ָ��ͨ����ȡת�����
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chn			Ҫ��ȡת�������ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7		
* ��    ��: uint32_t				��ȡ����ת�����
* ע������: ��
******************************************************************************************************************************************/
uint32_t ADC_Read(ADC_TypeDef * ADCx, uint32_t chn)
{
	uint32_t dat = 0;
	uint32_t idx = chn2idx(chn);
	
	dat = ADCx->CH[idx].DATA & ADC_DATA_VAL_Msk;
	
	ADCx->CH[idx].STAT = 0x01;		//���EOC��־
	
	return dat;
}

/****************************************************************************************************************************************** 
* ��������:	ADC_IsEOC()
* ����˵��:	ָ��ͨ���Ƿ�End Of Conversion
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chn			Ҫ��ѯ״̬��ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7		
* ��    ��: uint32_t				1 ��ͨ�������ת��    0 ��ͨ��δ���ת��
* ע������: ��
******************************************************************************************************************************************/
uint32_t ADC_IsEOC(ADC_TypeDef * ADCx, uint32_t chn)
{
	uint32_t idx = chn2idx(chn);
	
	return (ADCx->CH[idx].STAT & ADC_STAT_EOC_Msk) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* ��������:	ADC_ChnSelect()
* ����˵��:	ADCͨ��ѡͨ��ģ��ת������ѡͨ��ͨ�������β���ת��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chns			Ҫѡͨ��ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7������ϣ�������λ�����㣩
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_ChnSelect(ADC_TypeDef * ADCx, uint32_t chns)
{
	ADCx->CTRL &= ~(0xFF << ADC_CTRL_CH0_Pos);
	ADCx->CTRL |=  (chns << ADC_CTRL_CH0_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	ADC_IntEOCEn()
* ����˵��:	ת������ж�ʹ��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chn			Ҫ���õ�ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7		
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_IntEOCEn(ADC_TypeDef * ADCx, uint32_t chn)
{
	uint32_t idx = chn2idx(chn);
	
	ADCx->IE |= (0x01 << (idx*2));
}

/****************************************************************************************************************************************** 
* ��������:	ADC_IntEOCDis()
* ����˵��:	ת������жϽ�ֹ
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chn			Ҫ���õ�ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7		
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_IntEOCDis(ADC_TypeDef * ADCx, uint32_t chn)
{
	uint32_t idx = chn2idx(chn);
	
	ADCx->IE &= ~(0x01 << (idx*2));
}

/****************************************************************************************************************************************** 
* ��������:	ADC_IntEOCClr()
* ����˵��:	ת������жϱ�־���
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chn			Ҫ���õ�ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7		
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_IntEOCClr(ADC_TypeDef * ADCx, uint32_t chn)
{
	uint32_t idx = chn2idx(chn);
	
	ADCx->IF = (0x01 << (idx*2));
}

/****************************************************************************************************************************************** 
* ��������:	ADC_IntEOCStat()
* ����˵��:	ת������ж�״̬
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chn			Ҫ��ѯ��ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7		
* ��    ��: uint32_t				1 ��ͨ�������ת��    0 ��ͨ��δ���ת��
* ע������: ��
******************************************************************************************************************************************/
uint32_t ADC_IntEOCStat(ADC_TypeDef * ADCx, uint32_t chn)
{
	uint32_t idx = chn2idx(chn);
	
	return (ADCx->IF & (0x01 << (idx*2))) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* ��������:	ADC_IntOVFEn()
* ����˵��:	��������ж�ʹ��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chn			Ҫ���õ�ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7		
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_IntOVFEn(ADC_TypeDef * ADCx, uint32_t chn)
{
	uint32_t idx = chn2idx(chn);
	
	ADCx->IE |= (0x01 << (idx*2+1));
}

/****************************************************************************************************************************************** 
* ��������:	ADC_IntOVFDis()
* ����˵��:	��������жϽ�ֹ
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chn			Ҫ���õ�ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7		
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_IntOVFDis(ADC_TypeDef * ADCx, uint32_t chn)
{
	uint32_t idx = chn2idx(chn);
	
	ADCx->IE &= ~(0x01 << (idx*2+1));
}

/****************************************************************************************************************************************** 
* ��������:	ADC_IntOVFClr()
* ����˵��:	��������жϱ�־���
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chn			Ҫ���õ�ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7		
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_IntOVFClr(ADC_TypeDef * ADCx, uint32_t chn)
{
	uint32_t idx = chn2idx(chn);
	
	ADCx->IF = (0x01 << (idx*2+1));
}

/****************************************************************************************************************************************** 
* ��������:	ADC_IntOVFStat()
* ����˵��:	��������ж�״̬
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ�����õ�ADC����ȡֵ����ADC0
*			uint32_t chn			Ҫ��ѯ��ͨ������ЧֵADC_CH0��ADC_CH1��... ... ��ADC_CH7		
* ��    ��: uint32_t				1 ��ͨ�������ת��    0 ��ͨ��δ���ת��
* ע������: ��
******************************************************************************************************************************************/
uint32_t ADC_IntOVFStat(ADC_TypeDef * ADCx, uint32_t chn)
{
	uint32_t idx = chn2idx(chn);
	
	return (ADCx->IF & (0x01 << (idx*2+1))) ? 1 : 0;
}