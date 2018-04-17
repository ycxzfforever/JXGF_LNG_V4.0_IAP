#include "kernel.h"
GlobalVar        globalvar;                                     //ȫ�ֱ���
ShiftRecordInfo  shiftrecordinfo;                               //���°��¼��Ϣ
ShiftRecordInfo  shiftrecordinfo_tmp;
FuelRecordInfo   fuelrecordinfo;                                //������ˮ��¼��Ϣ
FuelRecordInfo   fuelrecordinfo_tmp;
Sysparas         sysparas;                                      //ϵͳ����
Sysparas         tmp_sysparas;                                  //�û��޸�ϵͳ����ʱ����ʱ�洢��Ҫ�޸ĵĲ���
M1carddata       m1card;       		                            //���ṹ���ݡ�

uint32_t         syserr;                                        //ϵͳ�쳣
uint32_t         stopreason;                                    //ͣ��ԭ��

//�����ÿ�����ʱ��δ�忨��ɨ���˵��ӱ�ǩ
void DispInserCard(void)
{   
    Fyd12864ClearLine(1);
    if(sysparas.offlinemode==1)
    {
        Fyd12864DispPrintf(2,1,"�ѻ�ģʽ�����ÿ�"); 
        Fyd12864ClearLine(3);
    }
    else
    {        
        Fyd12864DispPrintf(2,1,"�����IC������");
        if((sysparas.fuelmode==2)&&(globalvar.startstate==0))
        {
            Fyd12864DispPrintf(3,1,"ɨ����ӱ�ǩ");
        }
        else
        {
            Fyd12864DispPrintf(3,1,"����������");
        }
    }
    Fyd12864ClearLine(4);
    TaskAdd(KEY_RET,DispFirst,NULL);    
}

void DispDateTime(void)
{

    if(globalvar.U32Temp<5)
    {
        globalvar.U32Temp++;
        Fyd12864DispPrintf(4,1,"20%02X-%02X-%02X %02X:%02X", 
            time.year,time.month,time.day, time.hour,time.minute);        
    }
    else if(globalvar.U32Temp<10)
    {
        globalvar.U32Temp++;
        Fyd12864DispPrintf(4,1,"20%02X-%02X-%02X %02X %02X", 
            time.year,time.month,time.day, time.hour,time.minute); 
    }
    else globalvar.U32Temp=0;
}

char ErrorInfo[][16]=
{
    "��FLASH ���ʧ��",
    "��FLASH ���ʧ��",
    "д�����",
    "�������",
    "д�����",// 4
    "�������",
    "д�ڲ�EEPROM����",
    "���ڲ�EEPROM����",
    "д�ⲿEEPROM����",//8
    "���ⲿEEPROM����",
    "����EE���ݲ�һ��",
    "������Դ���",
    "����������ͨѶ��",
    "Һ��������ͨѶ��",// 13
    "ѹ���������쳣",
};

void DispErrorInfo()
{   
    uint8_t row=0,num=0,i=4;
    uint32_t tmpsyserror=syserr;
    globalvar.workstate=StaFault; //���������ϡ�
    for(row=0;row<4;)
    {
        num=GetOnePosition(tmpsyserror);//���ص�λ�ñ�ʵ��λ�ô�1
        if((num==13)||(num==14)||(num==15)) ReadModbusAndPressure();
        if(num>0)
        {
            ClrBit(tmpsyserror, num-1);//�ҵ�1��λ�ú������
            row++;
            Fyd12864DispPrintf(row,1,ErrorInfo[num-1]);
        }
        else break;
    }
    for(i=4;i>row;i--)
    {
        Fyd12864ClearLine(i);
    }          
    if(sysparas.lcdtype==1) //��Ҫ���ڴ�����ʾ����
    {        
        OsplcShowN(fuelrecordinfo.volume,fuelrecordinfo.money,fuelrecordinfo.price);
    } 
}


//�׽�����ʾ
void DispFirst(void)
{
    timer1off();
    InputInit();
    globalvar.timeout=0;           								//��ʱʱ�����㡣    
    globalvar.modbus_g_error=0;    								//�����������ƴ�������㡣
    globalvar.modbus_l_error=0;    								//��Һ�������ƴ�������㡣    
    globalvar.fuelstep=0;         								//�������衣
    globalvar.fix=0;               								//������־��
    globalvar.fixnum=0;
    globalvar.fixmoney=0;
    globalvar.U8Temp=1;
    globalvar.displayonce=1;
    globalvar.devicetest=0;
    globalvar.stoptimes=0;
    memset(&m1card,0,sizeof(M1carddata));   					//�����ݽṹ�����㡣    
    if(syserr)
    {
        DispErrorInfo();
    }
    else
    {
        if((sysparas.offonwork==1)&&(sysparas.yesnolock==0))    //�жϼ������Ƿ������������°�״̬��
        {
			globalvar.workstate=StaIdle;                        //���������С�
            if(sysparas.usepwdboard==1)                         //���ʹ�ü��ܰ塣
            {
                if(SI_INDATE==0)                                //������ܰ���SET״̬��
                {
                    Fyd12864DispPrintf(1,1,"������������״̬");
                }
                else
                {
                    Fyd12864DispPrintf(1,1,"ǹ��:%02d ���:%02d",sysparas.gunnum,sysparas.shiftnum);
                }
            }
            else
            {
                Fyd12864DispPrintf(1,1,"ǹ��:%02d ���:%02d",sysparas.gunnum,sysparas.shiftnum);
            }
            if(globalvar.eepromflag==1)
            {
                Fyd12864DispPrintf(2,1,"EEPROM��������");
                Fyd12864DispPrintf(3,1,"�뼰ʱ��������");
            }
            else
            {
                Fyd12864DispPrintf(2,1,"����:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
                Fyd12864DispPrintf(3,1,"���:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);            
            }
            if((globalvar.comfailcount>10)&&(sysparas.offlinemode==0)) //�ж��������̨�Ƿ�Ͽ����ӡ�          
            {
                Fyd12864DispPrintf(4,1,"�������ѵ���");
            }
            else
            {            
                DispDateTime();
            }
            TaskAdd(KEY_BACK, StartPrecooling,DispFirst);       //ֻ��Ԥ��
			if((sysparas.fuelmode==0)&&(sysparas.usecard==0))   //�ֶ�������
			{
				TaskAdd(KEY_START, StartPrecooling,StartFueling);//Ԥ����Һ��
			}
            else if(sysparas.fuelmode==2)                       //��ǩ������
			{
			    if(sysparas.usecard==0)
    			{
    			    TaskAdd(KEY_START, DispScanRFID,NULL);
    				if(globalvar.startstate!=0xff)//���ӱ�ǩ��������û�б�ǩ���
    				{											
    					DispUI=BQresults;
    				}
                }
                else if(globalvar.startstate==0)  //δ�忨�յ���ǩ��Ϣ
                {
                    globalvar.startstate=0xFF;
                    DispUI=DispInserCard;
                }
			}
            if(sysparas.usecard==0)
    		{
                TaskAdd(KEY_FIX, FixGas,NULL);                  //����������
            }
            else
            {
                TaskAdd(KEY_START, DispInserCard,NULL);
                TaskAdd(KEY_FIX, DispInserCard,NULL);
            }
        }
        else
        {
            Fyd12864ClearLine(1);             
            if(sysparas.yesnolock==1)
            {
                Fyd12864DispPrintf(2,1,"����������");
                Fyd12864ClearLine(3);
            }
            else if(sysparas.offonwork==0)
            {
				globalvar.workstate=StaOffDuty;                 //�������°ࡣ
                Fyd12864DispPrintf(2,1,"    �°���..");
                if(sysparas.offlinemode==1)//�ѻ�
                    Fyd12864DispPrintf(3,1,"����ֹͣ+7���ϰ�");
                else
                    Fyd12864ClearLine(3);
            }
            Fyd12864ClearLine(4);
        }       
    }
    if(sysparas.offlinemode==1)									//�ѻ�ģʽ
	{
	    if(sysparas.offonwork==0)
		    TaskAdd(KEY_COMB_7KEY,OnWorkWithPwd,NULL);          //ֹͣ+7�ϰ�
        else if(sysparas.offonwork==1)
            TaskAdd(KEY_COMB_8KEY,OffWorkWithPwd, NULL);        //ֹͣ+8�°�
	}       
    TaskAdd(KEY_QRY, DispQuery1,NULL);                          //��ѯ��
    TaskAdd(KEY_SET, DispInputMenuPwd,NULL);                    //���á�
    TaskAdd(KEY_COMB_SBACK,DispInputQcPwd,NULL);                //ֹͣ+���� �޸��ʼ����    (��������ʼ�ģʽ�޸�)   
    TaskAdd(KEY_COMB_SQRY,DispInputTestPwd,NULL);               //ֹͣ+��ѯ Ӳ�����ԡ�   
    TaskAdd(KEY_COMB_SSET,DispInputResetPwd, NULL);             //ֹͣ+�˵� ������λ���ʼ�ģʽ���ָ��������á�   
    
}

//ѭ����������ʱ������������
void CirculateFueling(void)
{
	globalvar.fixnum=100+time.day+time.hour+time.minute+time.second;
	globalvar.fix=1;										//����
	DispUI=StartPrecooling;
    if(sysparas.offlinemode==1)//�ѻ�
        DispNextUI=StartFueling;
    else
        DispNextUI=GreyLock;
    globalvar.displayonce=1; 
}


/*************************************************************************
** �������ƣ�Task100msProcess
** �������ܣ�100ms������
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-12-7 13:21:02

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void Task100msProcess(void)
{
    timer0.flag_100ms=0;
    if((globalvar.BeepCount--)==0){    BEEP_OFF();}
    if(globalvar.MsgTime>0)  globalvar.MsgTime--;   
    RecBlckDataCheck();
    if(globalvar.comreceiveflag==1)          				 //������������ٱ�־��
    {
        BGCommunication(RecBack485Buf[4]);  				 //��̨���������
        globalvar.comreceiveflag=0;
    }
    DispUI();
    globalvar.KeyValue=0; //����ˢ�º��轫��ֵ����
}

/*************************************************************************
** �������ƣ�Task500msProcess
** �������ܣ�500ms������
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-12-7 13:21:02

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void Task500msProcess(void)
{
    timer0.flag_500ms=0;  
    Ds3232ReadTime();                 //����ʱ�䡣
    RunLed1();                        //led1 500ms��˸һ��			    
    if((SI1STATUS!=0)&&(SI2STATUS!=0))//һ��˫����Һʱ���ж�״̬
    {
        globalvar.otherstate=1;
    }
    else
    {
        globalvar.otherstate=0;
    }    
}

/*************************************************************************
** �������ƣ�Task1sProcess
** �������ܣ�1s������
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-12-7 13:21:02

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void Task1sProcess(void)
{
    timer0.flag_1s=0;
    FEEDDOG();                      //ι����
    if((globalvar.devicetest==0)&&((globalvar.workstate==StaIdle)||
                                    (globalvar.workstate==StaOffDuty)||
                                    (globalvar.workstate==StaFault))) //�°࣬���У�������ʵʱ�жϣ���ŷ�����ʱ�����ж�
    {
        if(globalvar.otherstate==0)//�Է���Һ
        {
            VCTRL_OPENKEY();
            VCTRL_MID_CLOSE();      //�й�
            VCTRL_HIGH_CLOSE();     //�߹�
        }
        else if(globalvar.otherstate==1)//�Է�����
        {
            VCTRL_OPENKEY();
            VCTRL_MID_OPEN();      //�п�
            VCTRL_HIGH_CLOSE();    //�߹�
        }
    }
    if(globalvar.stoppumptime>0) 
    {
        globalvar.stoppumptime--;//��ʱͣ�õ���ʱ
        if((globalvar.stoppumptime==0)&&(globalvar.workstate!=StaFuel))//��Һ����ʱͣ��
        {
            if(globalvar.otherstate==1) //�Է�δ��Һ
            {
                PumpSwitch(StaIdle);
            }
            else                        //�Է���Һ
            {
                PumpSwitch(StaOtherFuel);
            }
        }	
    }
    if((sysparas.offlinemode==0)&&(m1card.existflag==0)&&
        ((globalvar.workstate==StaIdle)||(globalvar.workstate==StaOffDuty)))			 //�޿�״̬���ڿ����°�ʱѰ����
    {
        if(Mt318GetCardSerialNum())  						 //Ѱ����
        {                    
            DispUI=GetCardData;      						 //������
        }
    }
    if(m1card.existflag==1)									 //ycx20161125���п���ʵʱ��⿨״̬
    {
        if(Mt318GetCardStatus()==FALSE)
        {
            if(Mt318GetCardStatus()==FALSE)					 //�����ж϶�û�п���֤�������γ�
            {
                m1card.existflag=0;
                m1card.checkresult=0xFF;
            }
        }
    }
    if((globalvar.circulatefue == 1)&&(globalvar.workstate==StaIdle)
        &&(((sysparas.offlinemode==1)&&(globalvar.displayonce==1))||((m1card.existflag==1)&&(m1card.checkresult==0)))) //�Ƿ�ѭ��������
	{
	    globalvar.displayonce=0;
    	TaskMessage(20, CirculateFueling);
	} 
}



/*********************************************************************************************************
** �������ƣ�main
** ����������������,LPC1788ģ��
** �����������
** ����ֵ  ��ϵͳ����
*********************************************************************************************************/
int main(void)
{
    uint8_t count=0;
    MsDelay(800);                                                    //��ֹӲ����ѹ��û׼���ã����³�ʼ�����ɹ�
	SystemInit();                   
    KeyPinInit();                                                    //�������ʼ��
	GPIOInit();                                                      //GPIO��ʼ��
	WATCH_DOG();
	Fyd12864Init();                                                  //��������ʼ��   
    I2C0Init(400000);                                                //i2c0��ʼ�� ���400K
    I2C1Init(400000);                                                //i2c1��ʼ�� ���400K
    EEPROMInit();                                                    //�ڲ�eeprom��ʼ��
    At24c512Init();                                                  //�ⲿeeprom��ʼ��
    Fm25L04AndFlashSpiInit();	                                     //��ʼ��spi0    	        	
    timer0Init();                                                    //��ʱ��0��ʼ��
    timer1Init();                                                    //��ʱ��1��ʼ��
    Fm25l04Test();     
    LockPortInit();                                                  //���ܰ�set�������á�
    PowerCheck();                                                    //���緽������Ϊ���롣 
    DispUI=DispFirst;
    globalvar.comreceiveflag=0;                                      //��̨���������ٱ�־��
    globalvar.startstate=0xff;                                       //����ӱ�ǩ���
    if(SSTFlashTest(0)) SetBit(syserr, SYSERR_FLASH_A_CHK);
    if(SSTFlashTest(1)) SetBit(syserr, SYSERR_FLASH_B_CHK); 
    MsDelay(100);
    WATCH_DOG();
    ReadSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
    for(count=0;count<4;count++)
    {
        if(sysparas.writeE2num>=EEPROM_W_TIMES)
        {
            SYSPARA_ADDR +=sizeof(Sysparas);
            ReadSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
        }
        else break;
    }
    if(count==4) globalvar.eepromflag=1;
    else    globalvar.eepromflag=0;    
    MsDelay(100);
    UARTInit(0, 9600);      	                                     //����������ӡ��
	UARTInit(1, sysparas.modbusbaud);     	                         //������
	UARTInit(2, 9600);                                               //��������DB9              
	UARTInit(3, sysparas.pcbaud);                                    //��̨
	UARTInit(4, 9600);      	                                     //���ӱ�ǩ	
    CloseUartRcvIRQ(3);                                              //�غ�̨�жϡ�        
    Fm25l04Read(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData)); //�������ϴε��籣�����ݡ�
    MsDelay(100);
    WATCH_DOG();
    if((realtime.workstatus==StaFuel)||(realtime.workstatus==StaPay))//����ϴμ��������ڼ������߿۷ѹ����е��硣
    {
        PowerDownRealDate();
        MsDelay(1000);
    }
    else
    {
        if(sysparas.transnum>0) ReadFuelRecord(sysparas.transnum-1,0);
    }
    memset(&realtime.lcng,0,sizeof(RealTimeData));                   //����ʵʱ���ݡ�
    Fm25l04Write(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData));//�����硣   
    OsplcInit();                                                     //��������ʼ��                                                                                       
    if(sysparas.shiftransnum>0) ReadFuelRecord(sysparas.shiftransnum-1,1);
    OpenUartRcvIRQ(PCPort);                                          //����̨�����жϡ�        
    WATCH_DOG();
    MsDelay(800);
    if(sysparas.simulation==0)   ModbusInitReg();         
    PumpSwitch(StaIdle);        									 //�ϵ�󽫼���������Ϊ����״̬��
    
    while(1)
    {           
        realtime.workstatus=globalvar.workstate;
        KeyGetValue();  
        if(modbus.flag==1)
        {
            modbus.flag=0;
            ModbusDataDisposs();
        }        
        if(timer0.flag_100ms==1)
        {
            Task100msProcess();
        }
        if(timer0.flag_500ms==1)
        {	            
            Task500msProcess();
		}          
        if(timer0.flag_1s==1)
        {
            Task1sProcess();
        }               
    }
}


/*********************************************************************************************************
**                            End Of File
*********************************************************************************************************/
