#include "kernel.h"

/********************************************************
**��������:At24c512Init
**��������:At24c512��ʼ��
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-14

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void At24c512Init(void)
{
	LPC_GPIO3->DIR |=1ul<<26;	               /*Ƭ����*/
	LPC_GPIO3->CLR |=1ul<<26;
	UsDelay(2);
}

/********************************************************
**��������:At24C512WriteNByte
**��������:��At24C512дnum������
**�������:addr---��ʼ��ַ
					 data---д������ָ��
           num----д�����ݸ���
**���ֵ  :����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��

**������  :�ƺ���
**��������:2016-04-14

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t At24C512WriteNByte(uint32_t addr, uint8_t *data, uint16_t num)
{
	uint8_t pageoffset,pagenum;
	uint32_t startaddr;
	uint16_t count;
	startaddr=addr;
	count=0;
	pageoffset=(uint8_t)(addr&0x007f);
	while(count<num)
	{
		if((startaddr&0x007f)==0)
		{
			if((num-count)>128)
			{
				pagenum=128;
			}
			else
			{
				pagenum=num-count;
			}
		}
		else
		{
			pagenum=128-pageoffset;
			if(num<=pagenum)
			{
				pagenum=num;
			}
		}
		I2C0WriteNByte(0xa0,2,startaddr,data+count,pagenum);
		MsDelay(50);
		count+=pagenum;
		startaddr+=pagenum;
	}
	return 0x01;
}

/********************************************************
**��������:At24C512ReadNByte
**��������:��At24C512��num������
**�������:addr---��ʼ��ַ
					 data---������ָ��
           num----�����ݸ���
**���ֵ  :����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��

**������  :�ƺ���
**��������:2016-04-14

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t At24C512ReadNByte(uint32_t addr, uint8_t *data, uint32_t num)
{
	return I2C0ReadNByte(0xa0,2,addr,data,num);
}

