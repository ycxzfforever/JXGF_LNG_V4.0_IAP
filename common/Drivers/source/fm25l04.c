#include "kernel.h"


uint16_t Fm25l04TestAdd = 0x100;
/***************************************************************************
**�������ƣ�FmSpiWriteByte
**�������ܣ�ͨ��spi0��FM25L04д��һ�����ݡ�
**��ڲ�����tem--Ҫд������ݡ�
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t FmSpiWriteByte(uint8_t tem)
{  
    Fm25L04AndFlashSpiSend(&tem, 1);
	return 0;
}

/***************************************************************************
**�������ƣ�FmSpiReadByte
**�������ܣ�ͨ��spi0��FM25L04����һ�����ݡ�
**��ڲ�������
**�� �� ֵ��tem--���������ݡ�
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t FmSpiReadByte(void)
{
	uint8_t tem = 0;
    Fm25L04AndFlashSpiReceive(&tem,1);
	return tem;
}

/***************************************************************************
**�������ƣ�Fm25l04Write
**�������ܣ���FM25L04���ض���ַд��һ�����ȵ����ݡ�
**��ڲ�����Addr--�洢�׵�ַ��
            *Data--�洢���ݣ�
            Length--���ݳ��ȡ�
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void Fm25l04Write(uint16_t Addr,uint8_t *Data,uint16_t Length)
{
	uint32_t i;
	uint16_t temp1 = 0xfff;
	uint16_t temp2 = 0xfff;

	SPI0_CS_ENABLE(); 
	FmSpiWriteByte(SPI_WREN);          //дʹ�ܡ�
	SPI0_CS_DISABLE();

	SPI0_CS_ENABLE();  

	temp1=(uint8_t)((Addr&0x100)>>5);
	temp2=SPI_WRITE|temp1;           //�õ�д�洢�Ĵ���������9λ��ַ��MSB��    
	FmSpiWriteByte(temp2);             //д�洢�Ĵ�����
	FmSpiWriteByte((uint8_t)(Addr));	 //д��ַ��

	for(i=0;i<Length;i++)
	{
		FmSpiWriteByte(*(Data+i));     //д�����ݡ�
	}
	SPI0_CS_DISABLE();

}

/***************************************************************************
**�������ƣ�Fm25l04Read
**�������ܣ���FM25L04���ض���ַ����һ�����ȵ����ݡ�
**��ڲ�����Addr--�洢�׵�ַ��
            *Data--�洢���ݣ�
            Length--���ݳ��ȡ�
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void Fm25l04Read(uint16_t Addr,uint8_t *Data,uint16_t Length)
{
	uint32_t i;
    uint16_t temp1 = 0xfff;
	uint16_t temp2 = 0xfff;
    
	SPI0_CS_ENABLE(); 
    temp1=(uint8_t)((Addr&0x100)>>5);
    temp2=SPI_READ|temp1;             //�õ����洢�Ĵ���������9λ��ַ��MSB��
	FmSpiWriteByte(temp2);             
	FmSpiWriteByte((uint8_t)(Addr));	  //д��ַ��

	for(i=0;i<Length;i++)
	{
		*(Data+i)=FmSpiReadByte();      //д���ݡ�
	}
	SPI0_CS_DISABLE();
}

/***************************************************************************
**�������ƣ�Fm25l04Test
**�������ܣ���FM25L04д��ָ�������ݣ�������������Ƚ϶������ݣ��ж�����
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ����
**�޸����ڣ�2016-12-8 10:47:46
**�޸����ݣ������������bufʹ��������
***************************************************************************/
uint8_t Fm25l04Test(void)
{
 	uint8_t DetData[10]={0x11,0x22,0x33,0x44,0x55,0xAA,0xBB,0xCC,0xDD,0xEE};
	uint8_t DetData_Temp[10]={0};
	uint8_t i,ErrorFlag=0;
    UnionU64    tmp;
    
	Fm25l04Read(Fm25l04TestAdd,&tmp.rdcv[0],8);//��������������ܴ���
	tmp.data++;
	Fm25l04Write(Fm25l04TestAdd,&tmp.rdcv[0],8); 
	
	memset(DetData_Temp,0,sizeof(DetData_Temp)); 
	Fm25l04Write(Fm25l04TestAdd+8,DetData,10);      //д��������   
	Fm25l04Read(Fm25l04TestAdd+8,DetData_Temp,10);	//����������  
	for(i=0;i<10;i++)
	{
		if(DetData_Temp[i]!=DetData[i])
		{
			ErrorFlag=1;
			Fm25l04Read(Fm25l04TestAdd+18,&tmp.rdcv[0],8);//����������������������ܴ���
			tmp.data++;
			Fm25l04Write(Fm25l04TestAdd+18,&tmp.rdcv[0],8);
            SetBit(syserr,SYSERR_T_FM25L04);
			break;
		}
		else
        {  
			ErrorFlag=0;            
        }
	}
    if((ErrorFlag==0)&&((GetBit(syserr,SYSERR_T_FM25L04))==1))
    {
        ClrBit(syserr,SYSERR_T_FM25L04);
    }
	return(ErrorFlag); 
}


