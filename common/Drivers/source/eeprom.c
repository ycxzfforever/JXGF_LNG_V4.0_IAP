#include "kernel.h"
uint32_t    SYSPARA_ADDR=0;

/********************************************************
**��������:EEPROMInit
**��������:�ڲ�EEPROM��ʼ��
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void EEPROMInit(void) 
{
  uint32_t regVal;

  LPC_EEPROM->PWRDWN = 0x0;
/* 
* ����ʱ�ӣ�EEPROM ��Ҫһ��375kHz��ʱ�ӡ����ʱ����ϵͳʱ�ӷ�Ƶ��á�
*/
  regVal =( SystemCoreClock / 375000 )-1;
  LPC_EEPROM->CLKDIV = regVal;
  regVal = LPC_EEPROM->CLKDIV;

  regVal  = ((((SystemCoreClock / 1000000) * 15) / 1000) + 1);
  regVal |= (((((SystemCoreClock / 1000000) * 55) / 1000) + 1) << 8);
  regVal |= (((((SystemCoreClock / 1000000) * 35) / 1000) + 1) << 16);
  LPC_EEPROM->WSTATE = regVal;
  return;
}

/********************************************************
**��������:EepromInterWriteNByte
**��������:���ڲ�EEPROM��addr��ʼ��дnum�����ݣ�����������data
**�������:addr---EEPROM��ʼд����ʼ��ַ
           data---д������ָ��
           num----д�����ݸ���
**���ֵ  :0---��ʾ��ʼ��ַ����д���������4032
           1----��������

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t EepromInterWriteNByte(uint16_t addr,uint8_t *data,uint16_t num)
{
	uint16_t i;
	uint8_t result=1;
	uint8_t page_offset,page_address;
	if((addr+num)>4032)//�ڲ�EEPROM��СΪ(4K-64)�ֽڡ�
	{
		return 0;
	}
	page_offset=(uint8_t)(addr&0x003f);//ҳƫ�ơ�
	page_address=(uint8_t)(addr>>6);   //ҳ��ַ��
	LPC_EEPROM->INT_CLR_STATUS=((1<<EEPROM_ENDOF_RW)|(1<<EEPROM_ENDOF_PROG));
	LPC_EEPROM->ADDR=EEPROM_PAGE_OFFSET(page_offset);
																							/* ����д���ݵ�ַ              */
	LPC_EEPROM->INT_CLR_STATUS=(1<<EEPROM_ENDOF_RW);
	for(i=0;i<num;i++)
	{
		LPC_EEPROM->CMD=EEPROM_CMD_8_BIT_WRITE;                           //8λд�롣
																			 /* ������д��ҳ�Ĵ���           */
		LPC_EEPROM->WDATA=*(data)++;
		page_offset+=1;
		if((page_offset>=EEPROM_PAGE_NUM)|(i==num-1)){
																				/* ������д��EEPROM��           */
			LPC_EEPROM->INT_CLR_STATUS=(0x1<< EEPROM_ENDOF_PROG);
			LPC_EEPROM->ADDR=EEPROM_PAGE_ADRESS(page_address);
			LPC_EEPROM->CMD=EEPROM_CMD_ERASE_PRG_PAGE;
			while(!((LPC_EEPROM->INT_STATUS>>28)&0x01));
																				/* �ȴ�д���       c            */
		}
		if(page_offset>=EEPROM_PAGE_NUM)
		{
			page_offset=0;
			page_address+=1;
			LPC_EEPROM->ADDR =0;
			if(page_address>EEPROM_PAGE_NUM-1) page_address=0;
		}
	}
	return result;
}

/********************************************************
**��������:EepromInterReadNByte
**��������:���ڲ�EEPROM��addr��ʼ����um�����ݣ�����data
**�������:addr---EEPROM��ʼ������ʼ��ַ
           data---�������ݴ��ָ��
           num----�������ݸ���
**���ֵ  :0---��ʾ��ʼ��ַ�Ӷ�����������4032
           1----��������

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t EepromInterReadNByte(uint16_t addr,uint8_t *data,uint16_t num)
{
	uint16_t i;
	uint8_t result=1;
	if((addr+num)>4032)
	{
		return 0;
	}
	LPC_EEPROM->INT_CLR_STATUS=((1<<EEPROM_ENDOF_RW)|(1<< EEPROM_ENDOF_PROG));
	LPC_EEPROM->ADDR=addr;
	LPC_EEPROM->CMD=EEPROM_CMD_8_BIT_READ|EEPROM_CMD_RDPREFETCH;
	for(i=0;i<num;i++){                  /* ��EEPROM�е�����                */
		LPC_EEPROM->INT_CLR_STATUS=(1<<EEPROM_ENDOF_RW);
		*data++=(uint8_t)(LPC_EEPROM->RDATA);			/* �ȴ������                     */
		while(!((LPC_EEPROM->INT_STATUS >> 26)&0x01));
	}
	return(result);
}

/********************************************************
**��������:SaveEeprom
**��������:�����ݱ��浽�ڲ�EEPROM���ⲿEEPROM
**�������:addr---EEPROM��ʼд����ʼ��ַ
            data---д������ָ��
           num----д�����ݸ���
**���ֵ  :0---��ʾ��ʼ��ַ�Ӷ�����������4032
                            1----��������

**������  :���
**��������:2016-12-9 12:55:38

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t SaveEeprom(uint16_t addr,uint8_t *data,uint16_t num)
{
    uint8_t     *t1,*t2;
    uint16_t    i;
    Sysparas    tmp1,tmp2;
    t1=(uint8_t*)&tmp1;
    t2=(uint8_t*)&tmp2;
    if(EepromInterWriteNByte(addr,data,num))//д�ڲ�eeprom��ȷ
    {
        ClrBit(syserr,SYSERR_W_IN_EEPROM);
        if(At24C512WriteNByte(addr,data,num))//д�ⲿeeprom��ȷ
        {
            ClrBit(syserr,SYSERR_W_EX_EEPROM);
            if(EepromInterReadNByte(addr, t1, num))//���ڲ�eeprom��ȷ
            {
                if(At24C512ReadNByte(addr, t2, num))//���ⲿeeprom��ȷ
                {
                    for(i=0;i<num;i++)
                    {
                        if(t1[i]!=t2[i])    break;                            
                    }
                    if(i==num)
                    {
                        return 1;
                    }
                    else if(i<num)
                    {
                        return 0;
                    }
                }
                else
                {
                    SetBit(syserr,SYSERR_R_EX_EEPROM);
                    return 0;
                }
            }
            else
            {
                SetBit(syserr,SYSERR_R_IN_EEPROM);
                return 0;
            }
        }
        else
        {
            SetBit(syserr,SYSERR_W_EX_EEPROM);//д�ⲿeeprom����ϵͳ����0x40��
            return 0;
        }
    }
    else
    {
        SetBit(syserr,SYSERR_W_IN_EEPROM);
        return 0;
    }  
    return 0;
}

/********************************************************
**��������:SaveSysparas
**��������:���������浽�ڲ�EEPROM���ⲿEEPROM
**�������:addr---EEPROM��ʼд����ʼ��ַ
            data---д������ָ��
           num----д�����ݸ���
**���ֵ  :0---����ʧ��
                            1----����ɹ�
**������  :���
**��������:2016-4-19 09:10:38

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t SaveSysparas(uint16_t addr,uint8_t *data,uint16_t num)
{
    uint8_t     ret=0;
    uint16_t    crc;
    sysparas.writeE2num++;
    crc=CRC16Calculate(data, num-2);
    sysparas.crc=crc;
    if(sysparas.writeE2num<EEPROM_W_TIMES)
    {
        ret=SaveEeprom(addr, data, num);        
    }
    else//��eepromд�������ڵ������д����ʱ�����ڱ���ַ����һ�Σ�Ȼ���ַ����ƫ�ƣ�����ٱ���һ��
    {
        ret=SaveEeprom(addr, data, num);
        addr +=sizeof(Sysparas); 
        SYSPARA_ADDR=addr;
        sysparas.writeE2num=1;
        crc=CRC16Calculate(data, num-2);
        sysparas.crc=crc;
        ret=SaveEeprom(addr, data, num);
    }
    if(SYSPARA_ADDR > SYSPARA_MAX_ADDR) globalvar.eepromflag=1;
	return ret;
}

/********************************************************
**��������:ReadSysparas
**��������:���������ڲ�EEPROM���ⲿEEPROM����
**�������:addr---EEPROM��ʼ������ʼ��ַ
            data---��ȡ����ָ��
           num---��ȡ���ݸ���
**���ֵ  :0---��ʾ��ʼ��ַ�Ӷ�����������4032
                            1----��������

**������  :���
**��������:2016-4-19 09:10:38

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t ReadSysparas(uint16_t addr,uint8_t *data,uint16_t num)
{
    uint16_t    i,crc;
    Sysparas    tmp1,tmp2;
    uint8_t     *t1,*t2,ret=FALSE;
    t1=(uint8_t*)&tmp1;
    t2=(uint8_t*)&tmp2;
    if(EepromInterReadNByte(addr, t1, num))//���ڲ�eeprom��ȷ
    {
        crc=CRC16Calculate(t1, num-2);
        if(tmp1.crc==crc)
        {
            ClrBit(syserr,SYSERR_R_IN_EEPROM);
            ret=TRUE;//�ڲ�eeprom������ȷ
        }
        else
        {
            if(tmp1.writeE2num>100000) tmp1.writeE2num=0;//��ֹ�״ζ�һ���°��ӣ�����ȫΪ0xFF
            SetBit(syserr,SYSERR_R_IN_EEPROM);
            ret=FALSE;//�ڲ�eeprom���ݴ���
        }
        //��ʹCRC����Ҳ��ֵ
        for(i=0;i<num;i++)
        {
            data[i]=t1[i];            
        }        
    }
    if(ret==FALSE)//����ڲ�eeprom�����ٶ��ⲿeeprom
    {
        if(At24C512ReadNByte(addr, t2, num))//���ⲿeeprom��ȷ
        {
            crc=CRC16Calculate(t2, num-2);
            if(tmp2.crc==crc)
            {
                ClrBit(syserr,SYSERR_R_EX_EEPROM);
                ret=TRUE;//�ⲿeeprom������ȷ
            }
            else
            {
                if(tmp2.writeE2num>100000) tmp2.writeE2num=0;//��ֹ�״ζ�һ���°��ӣ�����ȫΪ0xFF
                SetBit(syserr,SYSERR_R_EX_EEPROM);
                ret=FALSE;//�ⲿeeprom���ݴ���
            }  
            //��ʹCRC����Ҳ��ֵ
            for(i=0;i<num;i++)
            {
                data[i]=t2[i];            
            }
        }
    }
    return ret;
}

