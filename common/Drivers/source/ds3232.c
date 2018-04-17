#include "kernel.h"

Time    time;

/********************************************************
**��������:Ds3232ReadTime
**��������:��DS3232����ʱ��
**�������:
**���ֵ  :���ʱ�䣨˳��Ϊ��������ʱ���룬��ķ�ΧΪ00~99����ʽΪBCD��
           ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
           
**������  :�ƺ���
**��������:2016-04-14

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t Ds3232ReadTime(void)
{
	uint8_t tmpbuffer[7];
	uint8_t result;
	result=I2C0ReadNByte(0xd0,1,0x00,tmpbuffer,7);
    if(result==TRUE)//ycx20161129����ȷ�Ÿ���ʱ��
    {
    	time.year   =tmpbuffer[6];
    	time.month  =tmpbuffer[5];
    	time.day    =tmpbuffer[4];
    	time.hour   =tmpbuffer[2];
    	time.minute =tmpbuffer[1];
    	time.second =tmpbuffer[0];
    }
	return result;
}

/********************************************************
**��������:Ds3232SetTime
**��������:DS3232����ʱ��
**�������:����ʱ�䣨˳��Ϊ��������ʱ���룬��ķ�ΧΪ00~99����ʽΪBCD��
**���ֵ  :����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��

**������  :�ƺ���
**��������:2016-04-14

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t Ds3232SetTime(uint8_t *buffer)
{
	uint8_t tmpbuffer[7];
	tmpbuffer[0]=buffer[5];
	tmpbuffer[1]=buffer[4];
	tmpbuffer[2]=buffer[3];
	tmpbuffer[3]=0;
	tmpbuffer[4]=buffer[2];
	tmpbuffer[5]=buffer[1];
	tmpbuffer[6]=buffer[0];
	return I2C0WriteNByte(0xd0,1,0x00,tmpbuffer,7);
}

