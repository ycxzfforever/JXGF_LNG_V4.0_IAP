#include "kernel.h"
/********************************************************
**��������:PlADCUinit
**��������:Һ��ѹ���ɼ���ʼ��
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-14
**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void PlADCUinit(void)
{
  LPC_SC->PCONP |= (0x1<<12);       /* ʹ��ADC���ʿ�����                     */
  LPC_IOCON->P0_23 &= ~0x9F;  
  LPC_IOCON->P0_23 |= 0x01;         /* ADC IN0  ѡ������ΪAD0ģʽ            */
	
  LPC_ADC->CR = (0x01<<0) |         /* ѡ��ADC[0]ΪADC����                   */
								(149<<8) |          /* CLKDIV = Fpclk / ADC_Clk - 1  60000000/400000-1=149 ����Ϊ400k HZת������*/ 
								( 0 << 16 ) |       /* BURST = 0, �������ת��               */
								( 1 << 21 ) |       /* PDN = 1,  ��������ģʽ                */
								( 0 << 24 ) |       /* START = 0  ������ADת��               */
								( 0 << 27 );        /* EDGE = 0  ����������                  */ 
  return;
}
/********************************************************
**��������:PgADCUinit
**��������:Һ��ѹ���ɼ���ʼ��
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-14
**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void PgADCUinit(void)
{
  LPC_SC->PCONP |= (0x1<<12);       /* ʹ��ADC���ʿ�����                     */
  LPC_IOCON->P0_24 &= ~0x9F;  
  LPC_IOCON->P0_24 |= 0x01;         /* ADC IN0  ѡ������ΪAD0ģʽ            */
	
  LPC_ADC->CR = (0x01<<1) |         /* ѡ��ADC[0]ΪADC����                   */
								(149<<8) |          /* CLKDIV = Fpclk / ADC_Clk - 1  60000000/400000-1=149 ����Ϊ400k HZת������*/ 
								( 0 << 16 ) |       /* BURST = 0, �������ת��               */
								( 1 << 21 ) |       /* PDN = 1,  ��������ģʽ                */
								( 0 << 24 ) |       /* START = 0  ������ADת��               */
								( 0 << 27 );        /* EDGE = 0  ����������                  */ 
  return;
}

/********************************************************
**��������:PlADCGetValueOneTime
**��������:Һ��ѹ���ɼ�һ��
**�������:��
**���ֵ  :Һ��ѹ��ADת�����

**������  :�ƺ���
**��������:2016-04-14
**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint32_t PlADCGetValueOneTime(void)
{
	uint32_t adcvalue;
	adcvalue=0;
	LPC_ADC->CR |=(0x01<<0)|(1UL<<24);      /* START = 1  ����ADת��*/
	while ((LPC_ADC->STAT&(1<<0))==0);//�ȴ�����
	adcvalue=LPC_ADC->DR[0]; /*��ȡͨ�����ݼĴ�����ֵ*/
	adcvalue=(adcvalue>>4)&0xfff;
	return(adcvalue);
}
/********************************************************
**��������:PgADCGetValueOneTime
**��������:����ѹ���ɼ�һ��
**�������:��
**���ֵ  :����ѹ��ADת�����

**������  :�ƺ���
**��������:2016-04-14
**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint32_t PgADCGetValueOneTime(void)
{
	uint32_t adcvalue;
	adcvalue=0;
	LPC_ADC->CR |=(0x01<<1)|(1UL<<24);      /* START = 1  ����ADת��*/
	while ((LPC_ADC->STAT&(1<<1))==0);//�ȴ�����
	adcvalue=LPC_ADC->DR[1]; /*��ȡͨ�����ݼĴ�����ֵ*/
	adcvalue=(adcvalue>>4)&0xfff;
	return(adcvalue);
}

/********************************************************
**��������:PlADCGetValue
**��������:Һ�࣬20��ADת����ֵ
**�������:��
**���ֵ  :Һ��20��ADת����ֵ

**������  :�ƺ���
**��������:2016-04-14

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint32_t PlADCGetValue(void)
{
	uint32_t adcvalue;
	uint8_t count;
	PlADCUinit();
	adcvalue=0;
	for(count=0;count<20;count++)
	{
		adcvalue+=PlADCGetValueOneTime();
	}
	adcvalue=(uint32_t)(adcvalue/20);
	LPC_ADC->CR &= 0xF8FFFFFF;         /* ֹͣת�� */
	return adcvalue;
}

/********************************************************
**��������:PgADCGetValue
**��������:���࣬20��ADת����ֵ
**�������:��
**���ֵ  :����20��ADת����ֵ

**������  :�ƺ���
**��������:2016-04-14

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint32_t PgADCGetValue(void)
{
	uint32_t adcvalue;
	uint8_t count;
	PgADCUinit();
	adcvalue=0;
	for(count=0;count<20;count++)
	{
		adcvalue+=PgADCGetValueOneTime();
	}
	adcvalue=(uint32_t)(adcvalue/20);
	LPC_ADC->CR &= 0xF8FFFFFF;         /* ֹͣת�� */
	return adcvalue;
}

/********************************************************
**��������:PressureGetValue
**��������:��ȡѹ��ֵ������ֵ��realtime.cur_pressure
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-12-12

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t PressureGetValue(void)	
{
	
	uint32_t curAD=0;
	uint32_t deltaAD=0;
	uint32_t curPressure;
	curAD=PlADCGetValue(); //��ȡ��ǰѹ��ADֵ

	if((curAD+80)<=sysparas.pzero) //ѹ�����
	{
		return false;
	}    
	else if(curAD<=sysparas.pzero)
	{
		deltaAD=0;
	}
	else
	{
		deltaAD=curAD-sysparas.pzero;  
	}
  
	curPressure = (100*deltaAD)/sysparas.pcoeff; //����ѹ��ֵ
	realtime.cur_pressure = curPressure;
	return true;  
}

