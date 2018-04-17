#include "kernel.h"


MassFlowModbus  modbus;
RealTimeData    realtime;

uint16_t _MODBUS_G_AUTO_RM_BUF[]= {0/*����*/,2/*����*/,1/*�¶�*/,3/*�ܶ�*/,9/*ѹ��*/};
uint16_t _MODBUS_L_AUTO_RM_BUF[]= {0/*����*/,2/*����*/,1/*�¶�*/,3/*�ܶ�*/,47/*����*/};

//9503Ϊ����Ĵ�����ֱ�Ӷ��ķ�ΧΪ0-100��ͨ��ɨ��Ĵ�����ȡ��ΧΪ:0-0.1
uint16_t _MODBUS_G_AUTO_EH_BUF[]= {2007/*����*/,2610/*����*/,2017/*�¶�*/,2013/*�ܶ�*/,2089/*ѹ��*/};
uint16_t _MODBUS_L_AUTO_EH_BUF[]= {2007/*����*/,2610/*����*/,2017/*�¶�*/,2013/*�ܶ�*/,9503/*����*/};


/***************************************************************************
**�������ƣ� ModbusCrc16
**�������ܣ�������CRC16����
**��ڲ�����buf:��Ҫ����CRC�����ݣ�len:��Ҫ��������ݳ���
**����ֵ�������CRC
**�����ߣ����
**�������ڣ�2016-3-30 15:45:03
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint16_t ModbusCrc16(uint8_t *buf, uint16_t Len)
{
    uint16_t IX, IY, CRC;
    CRC = 0xFFFF;
    if(Len <= 0)    return 0;
    for(IX = 0; IX < Len; IX++)
    {
        CRC = CRC ^ (uint16_t)(buf[IX]);
        for(IY = 0; IY < 8; IY++)
        {
            if((CRC &1) != 0)
                CRC = (CRC >> 1) ^ 0xA001;
            else
                CRC = CRC >> 1;
        }
    }
    return CRC;
}

/***************************************************************************
**�������ƣ� ModbusReadScanReg
**�������ܣ���ȡɨ��Ĵ�����ֵ
**��ڲ�����modbus_addr:�������豸��ַ��1-����2-Һ��
                                    RegAddr:�Ĵ�����ַ
                                    regnum:�Ĵ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-3-31 15:47:27
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void ModbusReadScanReg(uint8_t modbus_addr,uint16_t RegAddr, uint8_t regnum)
{
	uint16_t  crc;
    modbus.sendbuf[0] = modbus_addr;
    modbus.sendbuf[1] = 0x03;             //���Ĵ����Ĺ��ܴ���
    modbus.sendbuf[2] = RegAddr >> 8;     //�Ĵ�����λ
    modbus.sendbuf[3] = RegAddr & 0xFF;   //�Ĵ�����λ
    modbus.sendbuf[4] = 0;                //number of registers
    modbus.sendbuf[5] = regnum;
    crc=ModbusCrc16(&modbus.sendbuf[0],6);//FLOW_CRC16//����CRCУ��
    modbus.sendbuf[6] = (uint8_t)(crc &0x00FF);//У��ֵ��λ
    modbus.sendbuf[7] = crc >>8;

    OpenUartRcvIRQ(ModbusPort);
    UartSendStr(ModbusPort,&modbus.sendbuf[0],8);
}

/***************************************************************************
**�������ƣ� ModbusReadReg
**�������ܣ���ȡָ���Ĵ�����ֵ
**��ڲ�����modbus_addr:�������豸��ַ��1-����2-Һ��
                                    RegAddr:�Ĵ�����ַ
                                    regnum:�Ĵ�������
**����ֵ��TRUE:�ɹ���FALSE:ʧ��
**�����ߣ����
**�������ڣ�2016-3-31 15:47:27
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t ModbusReadReg(uint8_t modbus_addr,uint16_t RegAddr, uint8_t regnum)
{
    uint16_t  crc;
    uint32_t  timecount=0;

    modbus.sendbuf[0] = modbus_addr;
    modbus.sendbuf[1] = 0x03;             //���Ĵ����Ĺ��ܴ���
    modbus.sendbuf[2] = RegAddr >> 8;     //�Ĵ�����λ
    modbus.sendbuf[3] = RegAddr & 0xFF;   //�Ĵ�����λ
    modbus.sendbuf[4] = 0;                //number of registers
    modbus.sendbuf[5] = regnum;
    crc=ModbusCrc16(&modbus.sendbuf[0],6);//FLOW_CRC16//����CRCУ��
    modbus.sendbuf[6] = (uint8_t)(crc &0x00FF);//У��ֵ��λ
    modbus.sendbuf[7] = crc >>8;

    OpenUartRcvIRQ(ModbusPort);
    UartSendStr(ModbusPort,&modbus.sendbuf[0],8);
    while(timecount<MODBUS_TIMEOUT)
    {
        timecount++;
        if(modbus.flag==1)
        {
            modbus.flag=0;
			globalvar.modbus_g_error=0;
            return TRUE;
        }
    }
    CloseUartRcvIRQ(ModbusPort);
    return FALSE;
}


/***************************************************************************
**�������ƣ� ModbusWriteReg
**�������ܣ���ȡָ���Ĵ�����ֵ
**��ڲ�����modbus_addr:�������豸��ַ��1-����2-Һ��
                                    modbus_coil:д�Ĵ���������
                                    RegAddr:�Ĵ�����ַ
                                    v_value:��Ҫд��Ĵ�����ֵ
**����ֵ��TRUE:�ɹ���FALSE:ʧ��
**�����ߣ����
**�������ڣ�2016-3-31 15:50:39
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t ModbusWriteReg(uint8_t modbus_addr, uint8_t modbus_coil, uint16_t RegAddr, uint16_t v_value)
{
    uint32_t    timecount=0;
    uint16_t    crc = 0;

    modbus.sendbuf[0] = modbus_addr;
    modbus.sendbuf[1] = modbus_coil;
    modbus.sendbuf[2] = RegAddr >> 8;       //�Ĵ�����λ
    modbus.sendbuf[3] = RegAddr & 0xFF;     //�Ĵ�����λ
    modbus.sendbuf[4] = v_value >> 8;       //ֵ��λ
    modbus.sendbuf[5] = v_value &0x00FF;    //ֵ��λ

    crc = ModbusCrc16(&modbus.sendbuf[0], 6);   //FLOW_CRC16У��

    modbus.sendbuf[6] = (uint8_t)(crc &0x00FF); //У��ֵ��λ
    modbus.sendbuf[7] = crc >>8;

    OpenUartRcvIRQ(ModbusPort);
    UartSendStr(ModbusPort,&modbus.sendbuf[0], 8);
    while(timecount<MODBUS_TIMEOUT)
    {
        timecount++;
        if(modbus.flag==1)
        {
            modbus.flag=0;
            return TRUE;
        }
    }
    CloseUartRcvIRQ(ModbusPort);
    return FALSE;
}


/***************************************************************************
**�������ƣ� ModbusReadDeviceAddress
**�������ܣ���ȡ�豸��ַ������MODBUS����
**��ڲ�����modbus_addr:�������豸��ַ��1-����2-Һ��
                                    rdaddress:�����Ʒ��ص��豸��ַ
**����ֵ��TRUE:�ɹ���FALSE:ʧ��
**�����ߣ����
**�������ڣ�2016-3-31 15:52:10
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t ModbusReadDeviceAddress(uint8_t modbus_addr,uint16_t *rdaddress)
{
    uint8_t readresult;
    UnionU16 rdInt16reg;
    rdInt16reg.data = 0;

    if(sysparas.modbustype == MODBUS_TYPE_RM)
    {
        readresult=ModbusReadReg(modbus_addr,312,1);
    }
    else if(sysparas.modbustype == MODBUS_TYPE_EH)
    {
        readresult=ModbusReadReg(modbus_addr,4909,1);
    }


    if(readresult!= MODBUS_OK)
    {
        return 0;
    }

    rdInt16reg.rdcv[0] =modbus.recbuf[4];
    rdInt16reg.rdcv[1] =modbus.recbuf[3];

    *rdaddress=rdInt16reg.data;

    return MODBUS_OK ;
}

/***************************************************************************
**�������ƣ� ModbusWriteDeviceAddress
**�������ܣ�д�������豸��ַ
**��ڲ�����modbus_addr:�������豸��ַ��1-����2-Һ��
                                    rdaddress:�����Ƶ����豸��ַ
**����ֵ��TRUE:�ɹ���FALSE:ʧ��
**�����ߣ����
**�������ڣ�2016-3-31 16:27:23
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t ModbusWriteDeviceAddress(uint8_t modbus_addr,uint16_t rdaddress)
{
    uint8_t readresult;

    if(sysparas.modbustype == MODBUS_TYPE_RM)
    {
        readresult=ModbusWriteReg(modbus_addr,6,312,rdaddress);
    }
    else if(sysparas.modbustype == MODBUS_TYPE_EH)
    {
        readresult=ModbusWriteReg(modbus_addr,6,4909,rdaddress);
    }

    if(readresult!= MODBUS_OK)
    {
        return FALSE;
    }

    return MODBUS_OK ;
}


/***************************************************************************
**�������ƣ� ModbusReadTotal
**�������ܣ���ȡ��������
**��ڲ�����modbus_addr:�������豸��ַ��1-����2-Һ��
                                    total:�����Ʒ��ص�����ֵ
**����ֵ��TRUE:�ɹ���FALSE:ʧ��
**�����ߣ����
**�������ڣ�2016-3-31 15:52:10
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t ModbusReadTotal(uint8_t modbus_addr,float *total)
{
    uint8_t readresult;
    UnionFloat rdFreg;
    rdFreg.data = 0;

    if(sysparas.modbustype == MODBUS_TYPE_RM)
    {
        readresult=ModbusReadReg(modbus_addr,258,2);
    }
    else if(sysparas.modbustype == MODBUS_TYPE_EH)
    {
        readresult=ModbusReadReg(modbus_addr,2609,2);
    }

    if(readresult!= MODBUS_OK)
    {
        return FALSE;
    }

    rdFreg.rdcv[0] =  modbus.recbuf[4];
    rdFreg.rdcv[1] =  modbus.recbuf[3];
    rdFreg.rdcv[2] =  modbus.recbuf[6];
    rdFreg.rdcv[3] =  modbus.recbuf[5];

    rdFreg.data=rdFreg.data+0.005;
    *total=rdFreg.data;
    return MODBUS_OK ;
}

/***************************************************************************
**�������ƣ� ModbusReadFlow
**�������ܣ�������
**��ڲ�����modbus_addr:�������豸��ַ��1-����2-Һ��
                                    flow:�����Ʒ��ص�����ֵ
**����ֵ��TRUE:�ɹ���FALSE:ʧ��
**�����ߣ����
**�������ڣ�2016-3-31 15:53:16
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t ModbusReadFlow(uint8_t modbus_addr,float *flow)
{
    uint8_t readresult;
    UnionFloat rdFreg;
    rdFreg.data = 0;

    if(sysparas.modbustype == MODBUS_TYPE_RM)
    {
        readresult=ModbusReadReg(modbus_addr,246,2);
    }
    else if(sysparas.modbustype == MODBUS_TYPE_EH)
    {
        readresult=ModbusReadReg(modbus_addr,2006,2);
    }
    if(readresult!= MODBUS_OK)
    {
        return FALSE;
    }
    rdFreg.rdcv[0] =  modbus.recbuf[4];
    rdFreg.rdcv[1] =  modbus.recbuf[3];
    rdFreg.rdcv[2] =  modbus.recbuf[6];
    rdFreg.rdcv[3] =  modbus.recbuf[5];

    rdFreg.data=rdFreg.data+0.005;
    *flow=rdFreg.data;
    return MODBUS_OK ;
}


/***************************************************************************
**�������ƣ� ModbusClearTotal
**�������ܣ���λ�����Ĵ���
**��ڲ�����modbus_addr:�������豸��ַ��1-����2-Һ��
**����ֵ��TRUE:�ɹ���FALSE:ʧ��
**�����ߣ����
**�������ڣ�2016-3-31 15:54:31
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t ModbusClearTotal(uint8_t modbus_addr)
{
    uint8_t rdresult=FALSE;
    float   cngsum;

    if(sysparas.modbustype == MODBUS_TYPE_RM)
    {
        rdresult=ModbusWriteReg(modbus_addr,5,55,0xFF00);
    }
    else if(sysparas.modbustype == MODBUS_TYPE_EH)
    {
        rdresult=ModbusWriteReg(modbus_addr,6,2607,1);
    }
    if(rdresult)
    {
        rdresult=FALSE;
        rdresult=ModbusReadTotal(modbus_addr,&cngsum);
        if(cngsum<0.01)
        {
            return TRUE;
        }
    }
    return FALSE;
}


/***************************************************************************
**�������ƣ� ModbusInitReg
**�������ܣ�������scan buffer�Ĵ�����ʼ��
**��ڲ�������
**����ֵ��TRUE:�ɹ���FALSE:ʧ��
**�����ߣ����
**�������ڣ�2016-3-31 15:55:42
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t ModbusInitReg(void)
{
    uint8_t n = 0;    
    uint16_t AUTOSCAN_RM_BUF[]= {246/*����*/,258/*����*/,250/*�¶�*/,248/*�ܶ�*/,290/*����*/}; //��˹����ɨ��Ĵ���
    uint16_t AUTOSCAN_EH_BUF[]= {2007/*����*/,2610/*����*/,2017/*�¶�*/,2013/*�ܶ�*/,9503/*����*/};//E+Hɨ��Ĵ���

    if(sysparas.modbustype == MODBUS_TYPE_RM)
    {
        for(n=0; n<_MODBUS_G_AUTO_BUF_LEN; n++)
        {
            WATCH_DOG();
            if(!ModbusWriteReg(_MODBUS_G_,6, 654+n, AUTOSCAN_RM_BUF[n]))
            {
                if(sysparas.simulation==0)
                {
                    syserr |= 1<<SYSERR_MODBUS_G_AUTO_ERR;//����������ͨѶ����ϵͳ����0x04��               
                }                  
                return FALSE;
            }
            MsDelay(200);
        }

        for(n=0; n<_MODBUS_L_AUTO_BUF_LEN; n++)
        {
            WATCH_DOG();
            if(!ModbusWriteReg(_MODBUS_L_,6, 654+n, AUTOSCAN_RM_BUF[n]))
            {
                if(sysparas.simulation==0)
                {
                    syserr |= 1<<SYSERR_MODBUS_L_AUTO_ERR;//Һ��������ͨѶ����ϵͳ����0x08��
                }
                return FALSE;
            }
            MsDelay(200);
        }
    }
    else if(sysparas.modbustype == MODBUS_TYPE_EH)
    {
        for(n=0; n<_MODBUS_G_AUTO_BUF_LEN; n++)
        {
            WATCH_DOG();
            if(!ModbusWriteReg(_MODBUS_G_,6, 5000+n, AUTOSCAN_EH_BUF[n]))
            {
                if(sysparas.simulation==0)
                {
                    syserr |= 1<<SYSERR_MODBUS_G_AUTO_ERR;//����������ͨѶ����ϵͳ����0x04��
                }
                return FALSE;
            }
            MsDelay(200);
        }

        for(n=0; n<_MODBUS_L_AUTO_BUF_LEN; n++)
        {
            WATCH_DOG();
            if(!ModbusWriteReg(_MODBUS_L_,6, 5000+n, AUTOSCAN_EH_BUF[n]))
            {
                if(sysparas.simulation==0)
                {
                    syserr |= 1<<SYSERR_MODBUS_L_AUTO_ERR;//Һ��������ͨѶ����ϵͳ����0x08��
                }
                    return FALSE;
            }
            MsDelay(200);
        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

/***************************************************************************
**�������ƣ� ModbusDataDisposs
**�������ܣ�������scan buffer���ݴ���
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016��9��8��09:20:21
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void ModbusDataDisposs()
{
    UnionFloat rdFreg;
    rdFreg.data = 0;
    /*��������*/
    rdFreg.rdcv[0] =  modbus.recbuf[4];
    rdFreg.rdcv[1] =  modbus.recbuf[3];
    rdFreg.rdcv[2] =  modbus.recbuf[6];
    rdFreg.rdcv[3] =  modbus.recbuf[5];
    if(modbus.recbuf[0]==_MODBUS_G_)
    {
        realtime.cur_flowrate_g=rdFreg.data;
    }
    else if(modbus.recbuf[0]==_MODBUS_L_)
    {
        realtime.cur_flowrate_l=rdFreg.data;
    }

    /*��������*/
    rdFreg.rdcv[0] =  modbus.recbuf[8];
    rdFreg.rdcv[1] =  modbus.recbuf[7];
    rdFreg.rdcv[2] =  modbus.recbuf[10];
    rdFreg.rdcv[3] =  modbus.recbuf[9];
    if(modbus.recbuf[0]==_MODBUS_G_)
    {
        realtime.cur_totalize_g=rdFreg.data;
    }
    else if(modbus.recbuf[0]==_MODBUS_L_)
    {
        realtime.cur_totalize_l=rdFreg.data;
    }

    /*�����¶�*/
    rdFreg.rdcv[0] =  modbus.recbuf[12];
    rdFreg.rdcv[1] =  modbus.recbuf[11];
    rdFreg.rdcv[2] =  modbus.recbuf[14];
    rdFreg.rdcv[3] =  modbus.recbuf[13];
    if(modbus.recbuf[0]==_MODBUS_G_)
    {
        realtime.cur_temperature_g=rdFreg.data;
    }
    else if(modbus.recbuf[0]==_MODBUS_L_)
    {
        realtime.cur_temperature_l=rdFreg.data;
    }

    /*�����ܶ�*/
    rdFreg.rdcv[0] =  modbus.recbuf[16];
    rdFreg.rdcv[1] =  modbus.recbuf[15];
    rdFreg.rdcv[2] =  modbus.recbuf[18];
    rdFreg.rdcv[3] =  modbus.recbuf[17];
    if(modbus.recbuf[0]==_MODBUS_G_)
    {
        realtime.cur_density_g=rdFreg.data;
    }
    else if(modbus.recbuf[0]==_MODBUS_L_)
    {
        realtime.cur_density_l=rdFreg.data;
    }

    /*��������*/
    rdFreg.rdcv[0] =  modbus.recbuf[20];
    rdFreg.rdcv[1] =  modbus.recbuf[19];
    rdFreg.rdcv[2] =  modbus.recbuf[22];
    rdFreg.rdcv[3] =  modbus.recbuf[21];
    if(modbus.recbuf[0]==_MODBUS_G_)
    {
        if(rdFreg.data<=0.1)
        {
            rdFreg.data=rdFreg.data*1000;
        }
        else if(rdFreg.data>100)//����������Ϊ100.45��������ʾ����ʾ�ĸ�������
        {
            rdFreg.data=99.9;
        }
        realtime.cur_gain_g=rdFreg.data;
        globalvar.readLorG=0;//��������ʱ����һ�ζ�Һ��������
        globalvar.modbus_g_error=0;
        ClrBit(syserr, SYSERR_MODBUS_G_AUTO_ERR);//�������������ͨѶ����
    }
    else if(modbus.recbuf[0]==_MODBUS_L_)
    {
        if(rdFreg.data<=0.1)
        {
            rdFreg.data=rdFreg.data*1000;
        }
        else if(rdFreg.data>100)//����������Ϊ100.45��������ʾ����ʾ�ĸ�������
        {
            rdFreg.data=99.9;
        }
        realtime.cur_gain_l=rdFreg.data;
        globalvar.readLorG=1;//��������ʱ����һ�ζ�����������
        globalvar.modbus_l_error=0;
        ClrBit(syserr, SYSERR_MODBUS_L_AUTO_ERR);//���Һ��������ͨѶ����
    }
}


/***************************************************************************
**�������ƣ� ModbusReadScanBuffer
**�������ܣ���ȡ������ɨ��Ĵ����е�ֵ
**��ڲ�����modbus_addr:�������豸��ַ��1-����2-Һ��
**����ֵ����
**�����ߣ����
**�������ڣ�2016-3-31 15:56:30
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void ModbusReadScanBuffer(uint8_t modbus_addr)
{
    if(sysparas.modbustype == MODBUS_TYPE_RM)
    {
        ModbusReadScanReg(modbus_addr,686,10);
    }
    else if(sysparas.modbustype == MODBUS_TYPE_EH)
    {
        ModbusReadScanReg(modbus_addr,5050,10);
    }
}

/***************************************************************************
**�������ƣ� ModbusComTest
**�������ܣ���ȡ�����Ƶ��豸��ַ������������ͨѶ����
**��ڲ�����modbus_addr:�������豸��ַ��1-����2-Һ��
**����ֵ��TRUE:�ɹ���FALSE:ʧ��
**�����ߣ����
**�������ڣ�2016-3-31 16:07:31
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t ModbusComTest(uint8_t modbus_addr)
{
    uint16_t deviceaddr;
    if(ModbusReadDeviceAddress(modbus_addr,&deviceaddr))
    {
        if(deviceaddr==modbus_addr)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/***************************************************************************
   **�������ƣ� ModbusRecDataCheck
   **�������ܣ������ƽ������ݴ���
   **��ڲ�������
   **����ֵ��    ��
   **�����ߣ�    ���
   **�������ڣ�2016-3-31 09:57:21

   **�޸��ߣ�
   **�޸����ڣ�
   **�޸����ݣ�
   ***************************************************************************/
void ModbusRecDataCheck(void)
{
    uint32_t    len,i;
    uint16_t    CRC;
    //���ݴ���
    if(((UART1Buffer[0]==0x01)||(UART1Buffer[0]==0x02))&&
       ((UART1Buffer[1]==0x03)||(UART1Buffer[1]==0x06)||(UART1Buffer[1]==0x05)))   //������Ч���ж�
    {
        if((UART1Buffer[1]==0x06)||((UART1Buffer[1]==0x05)))//д�Ĵ���
        {
            len=8;
        }
        else
        {
            len=UART1Buffer[2]+5;   //���ݳ���
        }
        if(len<40)
        {
            if(UART1Count>=len)
            {
                CloseUartRcvIRQ(ModbusPort);
                CRC=ModbusCrc16((uint8_t *)UART1Buffer,len-2);
                if((UART1Buffer[len-2]==((uint8_t)(CRC &0x00FF)))&&
                   (UART1Buffer[len-1]==((uint8_t)(CRC>>8))))
                {                    
                    for(i=0; i<len; i++)
                    {
                        modbus.recbuf[i]=UART1Buffer[i] ;
                    }
                    modbus.flag = 1;
                }
                UART1Count=0;
                ClearBuf(UART1Buffer,0,40);
            }
        }
        else
        {
            OpenUartRcvIRQ(ModbusPort);
            UART1Count=0;
            ClearBuf(UART1Buffer,0,40);
        }
    }
    else
    {
        OpenUartRcvIRQ(ModbusPort);
        UART1Count=0;
        ClearBuf(UART1Buffer,0,40);
    }
}




/*********************************************************************************************************
** �������ƣ�UART1_IRQHandler
** ����������UART1�жϴ�����
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void UART1_IRQHandler(void)
{
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;

    uiIIRValue = LPC_UART1->IIR;

    uiIIRValue >>= 1;                     /* �жϱ�־�Ĵ���ֵ����1λ*/
    uiIIRValue &= 0x07;

    if(uiIIRValue == IIR_RLS)             /* ������״̬�ж� */
    {
        uiLSRValue = LPC_UART1->LSR;
        /*
                         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
                         */
        if(uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI))
        {
            UART1Status = uiLSRValue;
            uiDummy = LPC_UART1->RBR;
            return;
        }
        if(uiLSRValue & LSR_RDR)          /* ����FIFO��Ϊ���ж�*/
        {
            UART1Buffer[UART1Count] = LPC_UART1->RBR;
            UART1Count++;
            if(UART1Count == BUFSIZE)
            {
                UART1Count = 0;
            }
            if(UART1Count>=7)
            {
                ModbusRecDataCheck();
            }
			if((UART1Buffer[0] != 1) &&(UART1Buffer[0] != 2))
			{
				UART1Count = 0;	
			}				
        }
    }
    else if(uiIIRValue == IIR_RDA)       /* �������ݿ����ж�*/
    {
        UART1Buffer[UART1Count] = LPC_UART1->RBR;
        UART1Count++;
        if(UART1Count == BUFSIZE)
        {
            UART1Count = 0;
        }
        if(UART1Count>=7)
        {
            ModbusRecDataCheck();
        }
		if((UART1Buffer[0] != 1) &&(UART1Buffer[0] != 2))
		{
			UART1Count = 0;	
		}
    }
    else if(uiIIRValue == IIR_CTI)       /* �ַ���ʱ�ж�*/
    {
        UART1Status |= 0x100;
    }
    else if(uiIIRValue == IIR_THRE)      /* ���ͱ��ּĴ������ж�*/
    {
        uiLSRValue = LPC_UART1->LSR;
        if(uiLSRValue & LSR_THRE)
        {
            UART1TxEmpty = 1;
        }
        else
        {
            UART1TxEmpty = 0;
        }
    }
}


