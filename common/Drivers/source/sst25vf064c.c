#include "kernel.h"

/***************************************************************************
**�������ƣ�FlashSpiRead
**�������ܣ�ͨ��Spi0��SST�������ݡ�
**��ڲ�������
**�� �� ֵ��dat--�������ݡ�
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t FlashSpiRead(void)
{
    uint8_t  dat = 0;
    WATCH_DOG();
    Fm25L04AndFlashSpiReceive(&dat,1 );
    return dat;  
}

/***************************************************************************
**�������ƣ�FlashSpiWrite
**�������ܣ�ͨ��Spi0��SSTд�����ݡ�
**��ڲ�����*buff--�����飻
           num--���ݸ�����
**�� �� ֵ��num
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint32_t FlashSpiWrite(uint8_t *buff, uint32_t num)
{   
    WATCH_DOG();
    Fm25L04AndFlashSpiSend(buff, num);
    return num;
}

/***************************************************************************
**�������ƣ�SpiSimpleCmd
**�������ܣ�д״̬�Ĵ�����
**��ڲ�����flashnum--��һ��FlashֵΪ0���ڶ���FlashֵΪ1��
            cmd--Ҫд������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void SpiSimpleCmd(uint8_t flashnum,uint8_t cmd)
{
    if(flashnum==0)
        SPI_FLASH_A_ENABLE();  //ʹ��Flash1.
    else
        SPI_FLASH_B_ENABLE();  //ʹ��Flash2.
    FlashSpiWrite(&cmd, 1);    //��Flashд��1�����
    if(flashnum==0)
        SPI_FLASH_A_DISABLE(); //����Flash1.
    else
        SPI_FLASH_B_DISABLE(); //����Flash2.
}  

/***************************************************************************
**�������ƣ�SSTReadID
**�������ܣ���ȡ������ID��������ID��
**��ڲ�����flashnum--��һ��FlashֵΪ0���ڶ���FlashֵΪ1��
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t SSTReadID(uint8_t flashnum)
{
    uint8_t  dataout[4], datain[4];
    uint8_t id, i;
    
    dataout[0] = SPI_FLASH_RDID;
    dataout[1] = 0x00;
    dataout[2] = 0x00;
    dataout[3] = 0x00;//0x00�������������ID--BF��Ȼ���������ID--4B��0x01��֮��
                      //0x00���������ID��BFֵ������4Bֵ����0x01�������ID�����4Bֵ������BFֵ����
    
    if(flashnum==0)
        SPI_FLASH_A_ENABLE();
    else
        SPI_FLASH_B_ENABLE();
    
    FlashSpiWrite(dataout, 4);
    
    for(i = 0; i < 4; i++) {
       datain[i] = FlashSpiRead();
    } 
    
    if(flashnum==0)
        SPI_FLASH_A_DISABLE();
    else
        SPI_FLASH_B_DISABLE();
    
    id=datain[0]; 
    return id;

}

/***************************************************************************
**�������ƣ�SSTReadFlashStatus
**�������ܣ���ȡFlash״̬�Ĵ�����״ֵ̬��
**��ڲ�����flashnum--��һ��FlashֵΪ0���ڶ���FlashֵΪ1��
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t SSTReadFlashStatus(uint8_t flashnum)
{
    uint8_t datain, dataout[1];
    
    dataout[0] = SPI_FLASH_RDSR;       //��ȡ״̬�Ĵ�����
    
    if(flashnum==0)
        SPI_FLASH_A_ENABLE();          //ʹ��Flash1Ƭѡ��
    else
        SPI_FLASH_B_ENABLE();          //ʹ��Flash2Ƭѡ��
         
    FlashSpiWrite(dataout, 1);         //���Ͷ�ȡ״̬�Ĵ���ָ�
 
    datain = FlashSpiRead();           //��ȡ״̬�Ĵ�����
  
    
    if(flashnum==0)
        SPI_FLASH_A_DISABLE();         //����Flash1Ƭѡ��
    else
        SPI_FLASH_B_DISABLE();         //����Flash2Ƭѡ��

    return datain;
}

/***************************************************************************
**�������ƣ�WriteSpiFlashStatus
**�������ܣ���״̬�Ĵ���д��ֵ��
**��ڲ�����flashnum--��һ��FlashֵΪ0���ڶ���FlashֵΪ1��
            status--Ҫд���״ֵ̬��
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void WriteSpiFlashStatus(uint8_t flashnum,uint8_t status)
{
    uint8_t dataout[2];
    
    SpiSimpleCmd(flashnum,SPI_FLASH_WREN);                                      /*  ����дʹ���ź�               */ 
    
    dataout[0] = SPI_FLASH_WRSR;
    dataout[1] = status;
    
    if(flashnum==0)
        SPI_FLASH_A_ENABLE();
    else
        SPI_FLASH_B_ENABLE();
    
    FlashSpiWrite(dataout, 2);    

    if(flashnum==0)
        SPI_FLASH_A_DISABLE();
    else
        SPI_FLASH_B_DISABLE();
}

/***************************************************************************
**�������ƣ�SSTEraseSector
**�������ܣ�����ĳһ������ֵ��������ֵΪ0xFF��
**��ڲ�����flashnum--��һ��FlashֵΪ0���ڶ���FlashֵΪ1��
            addr--Ҫ�����ĵ�ַ��
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t SSTEraseSector(uint8_t flashnum,uint32_t addr)
{
    uint8_t  dataout[4];
    
    if(addr > MAX_ADDR) {
        return FALSE; 
    }
    
    SpiSimpleCmd(flashnum,SPI_FLASH_WREN);               //����дʹ���źš�   

    dataout[0] = SPI_FLASH_SE;                           //����4k�Ĵ洢�����С�
    dataout[1] = (uint8_t)((addr&0xFFFFFF) >> 16);       //��ַ1��
    dataout[2] = (uint8_t)((addr&0xFFFF) >>  8);         //��ַ2��
    dataout[3] = (uint8_t)(addr&0xFF);                   //��ַ3��

    if(flashnum==0)
        SPI_FLASH_A_ENABLE();
    else
        SPI_FLASH_B_ENABLE();
    
    FlashSpiWrite(dataout, 4);
    
    if(flashnum==0)
        SPI_FLASH_A_DISABLE();
    else
        SPI_FLASH_B_DISABLE();
    
    while ((SSTReadFlashStatus(flashnum) & 0x01) == 0x01);//�ж�æ��־��0��æ��1æ��
    
    return TRUE; 
}

/***************************************************************************
**�������ƣ�SSTEraseBlock
**�������ܣ�����ĳһ���ֵ��ÿһ��64K�ռ䡣
**��ڲ�����flashnum--��һ��FlashֵΪ0���ڶ���FlashֵΪ1��
            addr--Ҫ�����ĵ�ַ��
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t SSTEraseBlock(uint8_t flashnum,uint32_t addr)
{
    uint8_t  dataout[4];
    
    if(addr > MAX_ADDR) {
        return FALSE; 
    }
    
    SpiSimpleCmd(flashnum,SPI_FLASH_WREN);        //����дʹ���źš�              

    dataout[0] = SPI_FLASH_BE;                    //������ָ�
    dataout[1] = (uint8_t)((addr&0xFFFFFF) >> 16);//��ַ1��
    dataout[2] = (uint8_t)((addr&0xFFFF) >>  8);  //��ַ2��
    dataout[3] = (uint8_t)(addr&0xFF);            //��ַ3��

    if(flashnum==0)
        SPI_FLASH_A_ENABLE();
    else
        SPI_FLASH_B_ENABLE();
    
    FlashSpiWrite(dataout, 4);                    //���Ͳ���ָ�
    
    if(flashnum==0)
        SPI_FLASH_A_DISABLE();
    else
        SPI_FLASH_B_DISABLE();
    while ((SSTReadFlashStatus(flashnum) & 0x01) == 0x01);   //�ж�æ��־��0��æ��1æ��
       
    return TRUE;  
}

/***************************************************************************
**�������ƣ�SSTEraseBlock
**�������ܣ�����ĳһ���ֵ��ÿһ��64K�ռ䡣
**��ڲ�����flashnum--��һ��FlashֵΪ0���ڶ���FlashֵΪ1��
            sector--Ҫ�����Ŀ顣
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void SSTEraseChip(uint8_t flashnum)
{
    SpiSimpleCmd(flashnum,SPI_FLASH_WREN);                 //����дʹ���źš�
    SpiSimpleCmd(flashnum,SPI_FLASH_CE);                   //��Ƭ������
    while ((SSTReadFlashStatus(flashnum) & 0x01) == 0x01) ;//�ȴ�����������
}

/***************************************************************************
**�������ƣ�SSTReadNbyte
**�������ܣ���ȡָ����ַ�����ݡ�
**��ڲ�����flashnum--��һ��FlashֵΪ0���ڶ���FlashֵΪ1��
            addr--�洢���洢�׵�ַ��
            *src--Ҫ����������׵�ַ��
            num--�洢���ݸ�����
**�� �� ֵ��bool/1/0
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t SSTReadNbyte(uint8_t flashnum,
                   uint32_t addr, 
                   uint8_t *dst, 
                   uint32_t num, 
                   uint8_t readmode)
{
    uint8_t  dataout[5];

    if ((addr > MAX_ADDR) || ((readmode != SPI_FLASH_READ) &&
                              (readmode != SPI_FLASH_FAST_READ))) 
    {
        return FALSE;
    }

    dataout[1] = (uint8_t)((addr&0xFFFFFF) >> 16);//��ַ1��
    dataout[2] = (uint8_t)((addr&0xFFFF) >>  8);  //��ַ2��
    dataout[3] = (uint8_t)(addr&0xFF);            //��ַ3��
    dataout[4] = 0xff;
    if(flashnum==0)
        SPI_FLASH_A_ENABLE();
    else
        SPI_FLASH_B_ENABLE();
    
    if(readmode == SPI_FLASH_READ) 
    {  
        dataout[0] = SPI_FLASH_READ;     //��ͨ��ģʽ ��
        FlashSpiWrite(dataout, 4);
    } 
    else 
    {
       dataout[0] = SPI_FLASH_FAST_READ; //���ٶ�ģʽ ��
       FlashSpiWrite(dataout, 5);
    }

    while (num-- > 0) {
        *dst++= FlashSpiRead();
    }
    
    if(flashnum==0)
        SPI_FLASH_A_DISABLE();
    else
        SPI_FLASH_B_DISABLE();

    return TRUE;
}

/***************************************************************************
**�������ƣ�SSTWriteNbyte
**�������ܣ�ҳ��̣�����Ӧ��ַд�����ݡ�ÿҳ�����洢256�ֽ����ݣ����������
            ����Ŀ��ҳ����ʼ��ַ��ʼ��̡�������ѡҳ��̵�ַ���봦�ڲ���״̬����FF��
            ÿ��ҳ��̣������ҳ�׵�ַ���м��㡣
**��ڲ�����flashnum--��һ��FlashֵΪ0���ڶ���FlashֵΪ1��
            addr--�洢�׵�ַ��
            *src--Ҫ����������׵�ַ��
            num--�洢���ݸ�����
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t SSTWriteNbyte( uint8_t  flashnum,
                      uint32_t addr, 
                      uint8_t  *src, 
                      uint32_t num )
{
    uint8_t  dataout[4];
    
    if ((addr > MAX_ADDR) || (num > 256)) {
        return FALSE;
    }
    
    SpiSimpleCmd(flashnum,SPI_FLASH_WREN);   // ����дʹ���ź� ��            
    
    dataout[0] = SPI_FLASH_PP;               //ҳ������
    dataout[1] = (uint8_t)((addr&0xFFFFFF) >> 16);//��ַ1��
    dataout[2] = (uint8_t)((addr&0xFFFF) >>  8);  //��ַ2��
    dataout[3] = (uint8_t)(addr&0xFF);            //��ַ3��
  
    if(flashnum==0)
        SPI_FLASH_A_ENABLE();               //Flash1Ƭѡʹ�ܡ�
    else
        SPI_FLASH_B_ENABLE();               //Flash2Ƭѡʹ�ܡ�

    FlashSpiWrite(dataout, 4);              //��������ʹ洢��ַ��

    FlashSpiWrite(src, num);                //���ʹ洢���ݡ�
 
    if(flashnum==0)
        SPI_FLASH_A_DISABLE();              //Ƭѡ���ܡ�
    else
        SPI_FLASH_B_DISABLE();              //Ƭѡ���ܡ�   
    while ((SSTReadFlashStatus(flashnum) & 0x01) == 0x01);  //��æ���ȴ�����������
    
    return TRUE;
}

/***************************************************************************
**�������ƣ�SSTFlashTest
**�������ܣ���дFlash���ݣ���֤Flash����
**��ڲ�����flashnum: 0--Flash1��1--Flash2��
**�� �� ֵ��ErrorFlag=0�����󣻷�������
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t SSTFlashTest(uint8_t flashnum)
{
    uint8_t i = 0;
    uint8_t ErrorFlag = 0;                     //�����ǡ�
    //uint8_t flashnum=1;                        //Flashѡ��0--Flash1��1--Flash2��
	uint8_t Status;
    uint8_t WriteBuff[100],ReadBuff[100];

    for (i = 0; i < 100; i++) {                 //��ʼ�����ݻ�����  
        WriteBuff[i] = i;
        ReadBuff[i]  = 0;
    }                                              

    WriteSpiFlashStatus(flashnum,0x00);        //д״̬���ϵ�BP3��BP2��BP1��BP0����1�����п鱻���������ܶ�д��
    Status=SSTReadFlashStatus(flashnum);  	   //��״̬��
	if(Status!=0)
	{
		ErrorFlag=1;
		return ErrorFlag;
	}
	
    SSTEraseSector(flashnum,FlashTestSADDR);
    SSTWriteNbyte(flashnum,FlashTestSADDR, WriteBuff, 100);  //�洢���ݣ����Flash��
//    uart2SendStr(WriteBuff,20);
    MsDelay(5);
  
    SSTReadNbyte(flashnum,FlashTestSADDR, ReadBuff, 100, SPI_FLASH_FAST_READ); //��Flash����20�ֽ����ݡ�
//    uart2SendStr(ReadBuff,20);
    for(i=0;i<100;i++)
    {
        if(WriteBuff[i]!=ReadBuff[i])
        {
            ErrorFlag=1;
            break;
        }
        else  
        {
            ErrorFlag=0;
        }
    }
	return ErrorFlag;
}

/***************************************************************************
**�������ƣ�SaveFuelRecord
**�������ܣ����������ˮ��¼
**��ڲ�����0-����ˮ��1-�����ˮ��
**�� �� ֵ��TRUE:�ɹ���FALSE:ʧ��
**�� �� �ߣ����
**�������ڣ�2016-4-26 14:12:09

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t SaveFuelRecord(uint8_t select)
{
    uint8_t         ret=FALSE,*pt,*pt1,*pt2;
    uint32_t addr ,i,num,id;
    if(select==0)
    {
        FuelRecordInfo  tmp1,tmp2;
        num=sizeof(FuelRecordInfo);
        id = sysparas.transnum-1;  //��¼��ˮ�Ŵ�0��ʼ��
        pt=(uint8_t *)(&fuelrecordinfo.transnum);
        pt1=(uint8_t *)(&tmp1.transnum);
        pt2=(uint8_t *)(&tmp2.transnum);
        id = id%FlashRecMaxID;
        addr = FlashRecSADDR + id*FlashRecLen;
    }
    
    if(select==1)
    {
        ShiftRecordInfo tmp1,tmp2;
        num=sizeof(shiftrecordinfo);
        id = sysparas.shiftransnum-1;  //��¼�� ��ˮ�Ŵ�0��ʼ��
        pt=(uint8_t *)(&shiftrecordinfo.classid);
        pt1=(uint8_t *)(&tmp1.classid);
        pt2=(uint8_t *)(&tmp2.classid);
        id = id%FlashShiftMaxID;
        addr = FlashShiftSADDR + id*FlashShiftLen;
    }
    
    if(addr%SEC_SIZE==0)//����һ�������׵�ַ������������4k�ֽ�
	//if((addr&0x0fff)==0)
    {
        WriteSpiFlashStatus(0,0x00);
        SSTEraseSector(0,addr);
        MsDelay(5);
        WriteSpiFlashStatus(1,0x00);
        SSTEraseSector(1,addr);
        MsDelay(5);
    }
    if(SSTWriteNbyte(0,addr,pt,num))//д������
    {
        MsDelay(5);
        if(SSTReadNbyte(0, addr,pt1,num, SPI_FLASH_FAST_READ))//�������ж���
        {
            for(i=0; i<num; i++)
            {
                if(pt1[i]!=pt[i]) break; //����������д�����ݲ�һ��
            }
            if(i==num)
            {
                ret = TRUE;
            }
            else if(i<num)
            {
                ret = FALSE;
            }
        }
        else
        {
            ret=FALSE;
        }
    }
    else
    {
        SetBit(syserr,SYSERR_W_FLASH_A);//д�����ϵͳ����0x200
        ret=FALSE;
    }
    if(SSTWriteNbyte(1,addr,pt,num))//д�븱��
    {
        MsDelay(5);
        if(SSTReadNbyte(1, addr,pt2,num, SPI_FLASH_FAST_READ))//�Ӹ����ж���
        {
            for(i=0; i<num; i++)
            {
                if(pt2[i]!=pt[i]) break; //����������д�����ݲ�һ��
            }
            if(i==num)
            {
                if(ret!=TRUE) ret=FALSE;//����д��ʱ
            }
            else if(i<num)
            {
                ret = FALSE;
            }
        }
        else
        {
            ret=FALSE;
        }
    }
    else
    {
        SetBit(syserr,SYSERR_W_FLASH_B);//д�����ϵͳ����0x800��
        ret=FALSE;
    }
    return ret;
}

/***************************************************************************
**�������ƣ�ReadFuelRecord
**�������ܣ���������ˮ��¼
**��ڲ�����id:��ˮ��
**�� �� ֵ��TRUE:�ɹ���FALSE:ʧ��
**�� �� �ߣ����
**�������ڣ�2016-4-26 15:37:37

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t ReadFuelRecord(uint32_t id,uint8_t select)
{
    uint8_t         ret=FALSE,*pt1,*pt2;
    uint16_t        crc;
    uint32_t        addr,num;
    FuelRecordInfo  tmp1,tmp2;
    ShiftRecordInfo tmp3,tmp4;
    if(select==0)
    {
        num=sizeof(FuelRecordInfo);
        pt1=(uint8_t *)(&tmp1.transnum);
        pt2=(uint8_t *)(&tmp2.transnum);
        id = id%FlashRecMaxID;
        addr = FlashRecSADDR + id*FlashRecLen;
    }
    
    if(select==1)
    {
        num=sizeof(shiftrecordinfo);
        pt1=(uint8_t *)(&tmp3.classid);
        pt2=(uint8_t *)(&tmp4.classid);
        id = id%FlashShiftMaxID;
        addr = FlashShiftSADDR + id*FlashShiftLen;
    }
    
    if(SSTReadNbyte(0, addr,pt1,num, SPI_FLASH_FAST_READ))//�������ж���
    {
        MsDelay(10);
        crc=CRC16Calculate(&pt1[0], num-2);
        if((tmp1.crc==crc)||(tmp3.crc==crc))
        {
            ClrBit(syserr,SYSERR_R_FLASH_A);            
            ret=TRUE;
        }
        else
        {
            if((tmp1.transnum==0xFFFFFFFF)&&(select==0))//�����������flash
                memset(pt1,0,sizeof(FuelRecordInfo));
            else if((tmp3.classid==0xFFFFFFFF)&&(select==1))//�����������flash
                memset(pt1,0,sizeof(ShiftRecordInfo));   
            else
                SetBit(syserr,SYSERR_R_FLASH_A);//�������ϵͳ����0x400��
            ret=FALSE;
        }
        //��ʹCRC����Ҳ��ֵ
        if(select==0)
        {
            fuelrecordinfo=tmp1;
            
        }
        else if(select==1)
        {
            shiftrecordinfo=tmp3;            
        }
    }
    else
    {
        SetBit(syserr,SYSERR_R_FLASH_A);//�������ϵͳ����0x400��
        ret=FALSE;
    }
    if(ret==FALSE)//��������������ٶ�����
    {
        if(SSTReadNbyte(1, addr,pt2,num, SPI_FLASH_FAST_READ))//�Ӹ����ж���
        {
            MsDelay(10);
            crc=CRC16Calculate(pt2, num-2);
            if((tmp2.crc==crc)||(tmp4.crc==crc))
            {
                ClrBit(syserr,SYSERR_R_FLASH_B);
                ret=TRUE;
            }
            else
            {
                if((tmp2.transnum==0xFFFFFFFF)&&(select==0))//�����������flash
                    memset(pt2,0,sizeof(FuelRecordInfo));
                else if((tmp4.classid==0xFFFFFFFF)&&(select==1))//�����������flash
                    memset(pt2,0,sizeof(FuelRecordInfo)); 
                else
                    SetBit(syserr,SYSERR_R_FLASH_B);//�������
                ret=FALSE;
            }
            //��ʹCRC����Ҳ��ֵ
            if(select==0)
            {
                fuelrecordinfo=tmp2;
                
            }
            else if(select==1)
            {
                shiftrecordinfo=tmp4;              
            }
        }
        else
        {
            SetBit(syserr,SYSERR_R_FLASH_B);//�������ϵͳ����0x1000��
            ret=FALSE;
        }
    }
    return ret;
}
/***************************************************************************
**�������ƣ�SaveFlash
**�������ܣ��洢������������-��ɾ����������������
**��ڲ�����flashnum-flashѡ��addr-����Ҫ�洢�ĵ�ַ��*src-�����׵�ַ��num-���ݸ�����
**�� �� ֵ��0
**�� �� �ߣ����
**�������ڣ�2016-4-26 15:37:37

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t SaveFlash(uint8_t  flashnum,uint32_t addr, uint8_t  *src, uint32_t num )
{
    uint32_t addroffset, writenum;
	while(num > 0)                                             //
	{
		if(addr%SEC_SIZE==0)//����һ�������׵�ַ������������4k�ֽ�
		{
			if(flashnum == 0)
			{
				WriteSpiFlashStatus(0,0x00);
				SSTEraseSector(0,addr);
				MsDelay(5);
			}
			else
			{
				WriteSpiFlashStatus(1,0x00);
				SSTEraseSector(1,addr);
				MsDelay(5);
			}
		}
		if(addr > MAX_ADDR)                                    //�ж��Ƿ񳬹�����ַ��
		{
			return 0;
		}

		addroffset = addr % PAGE_SIZE;                         //��ҳƫ�ơ�
		if((num < PAGE_SIZE) && (addroffset + num < PAGE_SIZE))//���һҳ��д�����ݡ�
		{
			writenum = num;
		}
		else                                                   //���һҳ����д�����ݡ�
		{
			writenum = PAGE_SIZE - addroffset;
		}

        MsDelay(1);
	
        if(SSTWriteNbyte( flashnum,addr, src, writenum ) == TRUE)
		{   //��Flashд�����ݡ�
    		addr += writenum;
    		num  -= writenum;
            src  += writenum;
		}
		else
			return 0;
	}
    return 1;
}


uint8_t ReadFlash(uint8_t flashnum,uint32_t addr,uint8_t *pt,uint32_t num)
{
    uint8_t ret=FALSE;
    if(flashnum==0)
    {
        ret=SSTReadNbyte(0, addr,pt,num, SPI_FLASH_FAST_READ);
    }
    if(flashnum==1)
    {
        ret=SSTReadNbyte(1, addr,pt,num, SPI_FLASH_FAST_READ);
    }
    return ret;
}
/*********************************************************************************************************
  End File
*********************************************************************************************************/
