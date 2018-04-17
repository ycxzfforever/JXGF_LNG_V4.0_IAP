/************��Ա���쿻ʹ�ӡ������*********/
#include "kernel.h"

uint8_t stopreasonbuf[][16]=
{
    "�ֶ�ͣ��",         // 0
    "�����������쳣",   // 1
    "Һ���������쳣",   // 2
    "˲ʱ���ݴ���",     // 3
    "ѹ���������쳣",   // 4
    "�����λͣ��",   // 5
    "������ͣ��",       // 6
    "�����ͣ��",       // 7
    "������ͣ��",       // 8
    "������ͣ��",       // 9
    "�����ܶ�ͣ��",     // 10
    "��������ͣ��",     // 11
    "��ѹͣ��",         // 12
    "�ӿ�ͣ��",         // 13
    "���˳���",         // 14
    "���γ������",   // 15    
};

void WHPrinterInit(void)
{
    SELECT_PRINT();
    UartSendByte(PrintPort,0x1B);
    UartSendByte(PrintPort,0x40);
    UartSendByte(PrintPort,0x0D);
    SELECT_CARD();
}

void WHPrinterNormalCmd(void)
{
    unsigned char cmd[14] = {0x1b, 0x40, 0x1c, 0x26, 0x1b, 0x31, 0x08,
                             0x1b, 0x70, 0x02, 0x1b, 0x63, 0x00, 0x0D
                            };
    //0x1c 26:���뺺�ִ�ӡ��ʽ
    //0x1b 56 02:�����߶�������ӡ
    //0x1b 63 00:�ַ���������ķ����ӡ
    UartSendStr(PrintPort,cmd,14);
}


void WHPrinterFreeRow(unsigned char rows)
//����
{
    while(rows--)
    {
        UartSendByte(PrintPort,0x0A);//����
        UartSendByte(PrintPort,0x0D);//�س���ȷ�ϴ�ӡ����(��֮ǰ��)����
    }
}
//////////////////////////////////////////////////////////////////////////////ͷ
void WHPrinterHead(void)
{
    unsigned char buff[8] = {0x1c, 0x26, 0x1b, 0x56, 0x02, 0x1b, 0x63, 0x00};
    //0x1c 26:���뺺�ִ�ӡ��ʽ
    //0x1b 56 02:�����߶�������ӡ
    //0x1b 63 00:�ַ���������ķ����ӡ
    UartSendStr(PrintPort,buff,8);
}

uint8_t WHPrinterCheck(void)
{
    uint8_t  val;
    UartSendByte(PrintPort,0x1c);
    UartSendByte(PrintPort,0x76);
    UartSendByte(PrintPort,0x0D);
    val=uart0GetByte();    
    if(val == 0xFF)    return(0xFF);
    if(val == 0x04)    return(0x04);
    return TRUE; 
}

uint8_t WHPrintReady(void)
{
    if(WHPrinterCheck()!=TRUE)    return FALSE;
    return TRUE;
}

void WHPrintf(char *fmt,...)
{
    va_list ap;
    char str[40];
    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    uart0PutString(str);
    va_end(ap);
}

void WHFuelRecord(void)
//��ӡ��ˮ��¼
{
    SELECT_PRINT();

    WHPrinterHead();
    WHPrintf("         LCNG����ƾ��\r\n");
    WHPrinterFreeRow(1);
    WHPrinterNormalCmd();
    WHPrintf("ǹ��:%d\r\n",sysparas.gunnum);
    WHPrintf("���:%d\r\n",fuelrecordinfo.classnum);
    WHPrintf("��ˮ��:%ld\r\n",fuelrecordinfo.transnum);
    WHPrintf("����:%ld.%02d Ԫ/kg\r\n",fuelrecordinfo.price/100,fuelrecordinfo.price%100);
    WHPrintf("����:%ld.%02d kg\r\n", fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
    WHPrintf("���:%ld.%02d Ԫ\r\n", fuelrecordinfo.money/100,fuelrecordinfo.money%100);
    WHPrintf("����:%02X%02X%02X%02X\r\n", fuelrecordinfo.usernum[4],fuelrecordinfo.usernum[5],
                                          fuelrecordinfo.usernum[6],fuelrecordinfo.usernum[7]);
    WHPrintf("ʵ�۽��:%ld.%02d Ԫ\r\n", fuelrecordinfo.reallymoney/100,fuelrecordinfo.reallymoney%100);
    WHPrintf("���:%ld.%02d Ԫ\r\n", fuelrecordinfo.afterbalance/100,fuelrecordinfo.afterbalance%100);
    if((sysparas.backgascalc==1)&&(fuelrecordinfo.stopreason!=0xFF))//������ݲ���ӡ��Һ���ͻ�����
    {
        WHPrintf("��Һ��:%ld.%02d kg\r\n", (fuelrecordinfo.volume+fuelrecordinfo.backgas)/100,(fuelrecordinfo.volume+fuelrecordinfo.backgas)%100);
        WHPrintf("������:%ld.%02d kg\r\n", fuelrecordinfo.backgas/100,fuelrecordinfo.backgas%100);
    }
    if(fuelrecordinfo.stopreason==0xFF)
        WHPrintf("��������:���\r\n");
    else
        WHPrintf("ͣ��ԭ��:%s\r\n", stopreasonbuf[fuelrecordinfo.stopreason]);
    WHPrintf("��ʼʱ��:20%02X-%02X-%02X %02X:%02X:%02X\r\n", fuelrecordinfo.starttime[1],fuelrecordinfo.starttime[2],fuelrecordinfo.starttime[3],
                                                          fuelrecordinfo.starttime[4],fuelrecordinfo.starttime[5],fuelrecordinfo.starttime[6]);
    WHPrintf("����ʱ��:20%02X-%02X-%02X %02X:%02X:%02X\r\n", fuelrecordinfo.endtime[1],fuelrecordinfo.endtime[2],fuelrecordinfo.endtime[3],
                                                          fuelrecordinfo.endtime[4],fuelrecordinfo.endtime[5],fuelrecordinfo.endtime[6]);
    
    WHPrinterFreeRow(1);
    WHPrintf("     ��ӭ����  ����ƽ��\r\n");//�·ָ���
    WHPrintf("     <�����Ʊ����ƾ��>\r\n");//��ע 
    WHPrinterFreeRow(6); 
    SELECT_CARD();
}

//��ӡ��ˮ��
void Printid(void)
{
    WHPrinterInit();//��ʼ����
    if(WHPrintReady())
    {
        WHFuelRecord();
    }
}

void WHFuelShifRecord(void)
//��ӡ����ˮ��¼
{
    SELECT_PRINT();

    WHPrinterHead();
    WHPrintf("         LCNG���ۼ�¼\r\n");
    WHPrinterFreeRow(1);
    WHPrinterNormalCmd();
    WHPrintf("ǹ��:%d\r\n",sysparas.gunnum);
    WHPrintf("���:%ld\r\n",shiftrecordinfo.classnum);
    WHPrintf("����ˮ��:%ld\r\n",shiftrecordinfo.classid);
    if(shiftrecordinfo.transactiontype==0)
        WHPrintf("��������:�ϰ�\r\n");
    else if(shiftrecordinfo.transactiontype==1)
        WHPrintf("��������:�°�\r\n");
    WHPrintf("Ա������:%02X%02X%02X%02X\r\n", shiftrecordinfo.cardstaffnum[4],shiftrecordinfo.cardstaffnum[5],
                                             shiftrecordinfo.cardstaffnum[6],shiftrecordinfo.cardstaffnum[7]);
    WHPrintf("��ʼʱ��:%02X-%02X-%02X %02X:%02X:%02X\r\n", shiftrecordinfo.classstarttime[1],shiftrecordinfo.classstarttime[2],shiftrecordinfo.classstarttime[3],
                                                           shiftrecordinfo.classstarttime[4],shiftrecordinfo.classstarttime[5],shiftrecordinfo.classstarttime[6]);
    WHPrintf("����ʱ��:%02X-%02X-%02X %02X:%02X:%02X\r\n", shiftrecordinfo.classendtime[1],shiftrecordinfo.classendtime[2],shiftrecordinfo.classendtime[3],
                                                           shiftrecordinfo.classendtime[4],shiftrecordinfo.classendtime[5],shiftrecordinfo.classendtime[6]);
    
    WHPrintf("ǹ���ۼ�:%ld.%02d\r\n",shiftrecordinfo.allgunnum/100,shiftrecordinfo.allgunnum%100);
    WHPrintf("ǹ���ۼ�:%ld.%02d\r\n",shiftrecordinfo.allgunmoney/100,shiftrecordinfo.allgunmoney%100);
    WHPrintf("�����ۼ�:%ld.%02d\r\n",shiftrecordinfo.classgasnum/100,shiftrecordinfo.classgasnum%100);
    WHPrintf("����ۼ�:%ld.%02d\r\n",shiftrecordinfo.classgasmoney/100,shiftrecordinfo.classgasmoney%100);
    
    WHPrintf("�������:%ld\r\n", shiftrecordinfo.classtimes);

    if(shiftrecordinfo.devicetype==0)
        WHPrintf("��Ʒ����:LNG\r\n");
    else if(shiftrecordinfo.devicetype==1)
        WHPrintf("��Ʒ����:CNG\r\n");    
    if(shiftrecordinfo.measuretype==0)
        WHPrintf("������λ:����\r\n");
    else if(shiftrecordinfo.measuretype==1)
        WHPrintf("������λ:�귽\r\n");
    
    WHPrintf("�ϰ���ˮ:%ld\r\n", shiftrecordinfo.classiding);

    WHPrinterFreeRow(1);
    WHPrintf("     ��ӭ����  ����ƽ��\r\n");//�·ָ���
    WHPrintf("     <�����Ʊ����ƾ��>\r\n");//��ע 
    WHPrinterFreeRow(6); 
    SELECT_CARD();
}

//��ӡ����
void PrintSum(void)
{
	WHPrinterInit();//��ʼ����
	if(WHPrintReady())
    {
        WHFuelSumRecord();
    }
}
void WHFuelSumRecord(void)
{
	SELECT_PRINT();

    WHPrinterHead();
    WHPrintf("         LCNG���ۼ�¼\r\n");
    WHPrinterFreeRow(1);
    WHPrinterNormalCmd();
    WHPrintf("ǹ��:%d\r\n",sysparas.gunnum);
    WHPrintf("������ˮ��:%ld\r\n",sysparas.transnum);
    WHPrintf("�����ۼ�:%ld.%02d\r\n",sysparas.totvolume/100,sysparas.totvolume%100);
    WHPrintf("�ܽ��ۼ�:%ld.%02d\r\n",sysparas.totmoney/100,sysparas.totmoney%100);   
    if(fuelrecordinfo.devicetype==0)
        WHPrintf("��Ʒ����:LNG\r\n");
    else if(fuelrecordinfo.devicetype==1)
        WHPrintf("��Ʒ����:CNG\r\n");    
    if(fuelrecordinfo.measuretype==0)
        WHPrintf("������λ:����\r\n");
    else if(fuelrecordinfo.measuretype==1)
        WHPrintf("������λ:�귽\r\n");   
    WHPrinterFreeRow(1);
    WHPrintf("     <�����Ʊ����ƾ��>\r\n");//��ע 
    WHPrinterFreeRow(6); 
    SELECT_CARD();
}
//��ӡϵͳ����
void PrintSysInfo(void)
{
	WHPrinterInit();//��ʼ����
    if(WHPrintReady())
    {
        WHFuelSysInfo();
    }
}
void WHFuelSysInfo(void)
{
	SELECT_PRINT();

    WHPrinterHead();
    WHPrintf("         LCNGϵͳ����\r\n");
    WHPrinterFreeRow(1);
    WHPrinterNormalCmd();
    WHPrintf("ǹ��:%d\r\n",sysparas.gunnum);
	WHPrintf("���:%d\r\n",sysparas.shiftnum);
	WHPrintf("�����:%d\r\n",sysparas.shiftmaxnum); 
    WHPrintf("����:%d.%02d\r\n",sysparas.price[0]/100,sysparas.price[0]%100);
	WHPrintf("������:%ld.%02d\r\n",sysparas.totvolume/100,sysparas.totvolume%100);
    WHPrintf("�ܽ��:%ld.%02d\r\n",sysparas.totmoney/100,sysparas.totmoney%100);
    WHPrintf("�޸���ˮ��:%ld\r\n",sysparas.modnum);
    WHPrintf("�Ѵ��޸ĺ�:%ld\r\n",sysparas.upmodnum);   

	WHPrintf("����������:%d\r\n",sysparas.modbustype);
    WHPrintf("������λ:%d\r\n",sysparas.unit);
    WHPrintf("�ʼ�ģʽ:%d\r\n",sysparas.qcmode);
    WHPrintf("EEд����:%ld\r\n",(SYSPARA_ADDR/sizeof(Sysparas))*EEPROM_W_TIMES+sysparas.writeE2num); 
	
	WHPrintf("�ܶ�:%d.%04d\r\n",sysparas.densty/10000,sysparas.densty%10000);
    WHPrintf("����ϵ��:%d.%04d\r\n",sysparas.coeff/10000,sysparas.coeff%10000);
    WHPrintf("ѹ�����:%d\r\n",sysparas.pzero);
    WHPrintf("ѹ��ϵ��:%d\r\n",sysparas.pcoeff); 
	
	WHPrintf("������:%ld.%02d\r\n",sysparas.cardmaxyue/100,sysparas.cardmaxyue%100);
    WHPrintf("������:%ld.%02d\r\n",sysparas.cardminyue/100,sysparas.cardminyue%100);
    WHPrintf("PSAM����:%d\r\n",sysparas.cardpsamnum);
    WHPrintf("�������:%d\r\n",sysparas.cardmaxday); 
		
	WHPrintf("�����Ʋ���:%ld\r\n",sysparas.modbusbaud);
    WHPrintf("��̨������:%ld\r\n",sysparas.pcbaud);
    WHPrintf("����ģʽ:%d\r\n",sysparas.fuelmode);
    WHPrintf("��ӡ��ʽ:%d\r\n",sysparas.printmode); 
	
	WHPrintf("���۰��:%d\r\n",sysparas.pricever);
    WHPrintf("���������:%d\r\n",sysparas.blackver);
    WHPrintf("���������:%d\r\n",sysparas.whitever);
    WHPrintf("�����ڰ��:%d\r\n",sysparas.addbkver); 
	
	WHPrintf("��ɾ�ڰ��:%d\r\n",sysparas.delbkver);
    WHPrintf("ͨ����Ϣ���:%d\r\n",sysparas.comminfover);
    WHPrintf("��Կ���:%d\r\n",sysparas.lockver);
    WHPrintf("�Ƿ�ʹ�ü��ܰ�:%d\r\n",sysparas.usepwdboard); 
	WHPrintf("ϵͳ�汾��:%s\r\n",VERSION);
 
    WHPrinterFreeRow(1);
    WHPrintf("     <�����Ʊ����ƾ��>\r\n");//��ע 
    WHPrinterFreeRow(6); 
    SELECT_CARD();
}

//��ӡ��ˮ��
void PrintShifid(void)
{
    WHPrinterInit();//��ʼ����
    if(WHPrintReady())
    {
        WHFuelShifRecord();
    }
}

void uart0PutString(char* string)
//�����ַ���    
{
    while(*string!='\0')
    {
        UartSendByte(PrintPort,*string);
        string++;
    }
}

 /***************************************************************************
    **�������ƣ� uart0GetByte
    **�������ܣ��Ӵ��ڽ���1�ֽ����ݣ�ʹ�ò�ѯ��ʽ����
    **��ڲ�������
    **����ֵ�� ���յ�������
    **�����ߣ����
    **�������ڣ�2016-3-29 08:59:46
    **
    **                            LPC_UART0->IER = 0;  // �ؽ����ж�
                                    LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS; //�������ж�
    **�޸��ߣ�
    **�޸����ڣ�
    **�޸����ݣ�
    ***************************************************************************/
uint8_t uart0GetByte (void)             //��ѯ��
{
    uint8_t     ucRcvData=0xFF;
    uint32_t    tickcount=0;
    while (((LPC_UART0->LSR & 0x01))&&(++tickcount<50000) == 0);  //UnLSR[0] ��λ�Ƿ���һ�������ܷ��FIFO�ж�ȡ����                                                           
    if(tickcount<50000) ucRcvData = LPC_UART0->RBR;                //UnRBRΪ���ջ���Ĵ���                            /* ��ȡ����                     */
    return (ucRcvData);   
}


