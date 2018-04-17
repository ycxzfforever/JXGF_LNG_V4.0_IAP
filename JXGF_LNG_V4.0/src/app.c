#include "kernel.h"

Reply51 UpLoad51Data;
Reply52 UpLoad52Data; 
Reply53 UpLoad53Data;
Reply54 UpLoad54Data;
Reply55 UpLoad55Data;
Reply56 UpLoad56Data; 
Reply57 UpLoad57Data;
Reply58 UpLoad58Data;
Reply59 UpLoad59Data; 
Reply5A UpLoad5AData; 
Reply5B UpLoad5BData;
Reply5C UpLoad5CData;
Reply5D UpLoad5DData;
Reply5E UpLoad5EData;
Reply5F UpLoad5FData;
Receive55data receive55data;
uint8_t SendBack485Buf[BUFSIZE];
uint8_t RecBack485Buf[BUFSIZE];

BWInfo    bwinfo;
uint8_t   LoadSuccess = 0;   //���������Ƿ���ɡ�
uint16_t  allpart=0;         //��̨�������������ܶ�����

/********************************************************
**�ṹ����:BufReverse1(uint8_t *buf1,uint8_t num,uint8_t *buf2)
**�ṹ����:����Ҫbuf1���ݳ�Ա�ߵ�����buf2�С�
            *buf1-����1��num-���鳤�ȣ�buf2-����2��

**�� �� ��:���
**��������:2016-06-3

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void BufReverse1(uint8_t *buf1,uint8_t num,uint8_t *buf2)
{
    if(num==2)
    {
        buf2[0]=buf1[1];
        buf2[1]=buf1[0];
    }
   
    else if(num==4)
    {
        buf2[0]=buf1[3];
        buf2[1]=buf1[2];
        buf2[2]=buf1[1];
        buf2[3]=buf1[0];
    }
}

/********************************************************
**�ṹ����:BufReverse2(uint8_t *buf, uint8_t num)
**�ṹ����:����Ҫbuf���ݳ�Ա�ߵ�����buf�С�
           num-���鳤�ȡ�

**�� �� ��:���
**��������:2016-06-3

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void BufReverse2(uint8_t *buf, uint8_t num)
{
	uint8_t  temp;
	if(num == 2)
	{
		temp   = buf[0];
		buf[0] = buf[1];
		buf[1] = temp;
	}
	if(num == 4)
	{
		temp   = buf[0];
		buf[0] = buf[3];
		buf[3] = temp;
		temp   = buf[1];
		buf[1] = buf[2];
		buf[2] = temp;
	}
}

/********************************************************
**�ṹ����:StrReverse(uint8_t *buf,uint8_t num,uint8_t n)
**�ṹ����:��һ�����ݵߵ�λ�á�
           buf-�����׵�ַ��num-Ҫת�������ݸ�����n-ת�����ݵ��ֽ�����

**�� �� ��:���
**��������:2016-06-3

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void StrReverse(uint8_t *buf,uint8_t num,uint8_t n)
{
    uint8_t i=0;
    if(n==2)
    {
        for(i=0;i<num;i++)
        {
            BufReverse2(buf,2);
            buf+=2;
        }
    }
    if(n==4)
    {
        for(i=0;i<num;i++)
        {
            BufReverse2(buf,4);
            buf+=4;
        }
    }
}
    
/********************************************************
**�ṹ����:ClearBuf(volatile uint8_t *buf,uint8_t num,uint16_t len)
**�ṹ����:�崮�ڽ������顣*buf-���飬num-�����鸳��ֵ��len-���ȡ�

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void ClearBuf(volatile uint8_t *buf,uint8_t num,uint16_t len)
{
    uint16_t i;
    for(i=0;i<len;i++)
    {
        buf[i]=num;
    }
}

/********************************************************
**�ṹ����:BackCommunicationHead(uint8_t cmd)
**�ṹ����:���̨ͨ�ţ�֡ͷ��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/  
void BackCommunicationHead(uint8_t cmd)
{
    SendBack485Buf[0] = 0xBB;    			//��ʼ��1��      
    SendBack485Buf[1] = 0xEE;    			//��ʼ��2��    
    SendBack485Buf[2] = 0x00;    			//Ŀ���豸��  
    SendBack485Buf[3] = sysparas.gunnum;    //Դ�豸?
    SendBack485Buf[4] = cmd;     			//���
}

/********************************************************
**�ṹ����:BackCommunicationEnd(uint16_t len)
**�ṹ����:���̨ͨ�ţ�֡β��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void BackCommunicationEnd(uint16_t len)
{
	uint16_t CRC;
    SendBack485Buf[5] = len/256;      //��Ϣ�����ݳ���H��	
    SendBack485Buf[6] = len%256;      //��Ϣ�����ݳ���L��
	CRC= CRC16Calculate(&SendBack485Buf[2], len+5);
    SendBack485Buf[len+7]=CRC/256;
    SendBack485Buf[len+8]=CRC%256;
	SendBack485Buf[len+9] = 0xDD;     //��ֹ1��
	SendBack485Buf[len+10] = 0xEE; 	  //��ֹ2��
}

/********************************************************
**�ṹ����:SendBackCommunicationData(uint8_t cmd,uint16_t len)
**�ṹ����:���̨��������cmd-���len-��Ϣ�峤�ȡ�

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void SendBackCommunicationData(uint8_t cmd,uint16_t len)
{
    BackCommunicationHead(cmd);
    BackCommunicationEnd(len);
    UartSendStr(PCPort,&SendBack485Buf[0], len+11);
}

void GetRep51Data(void)
{
    UpLoad51Data.LCNG=sysparas.devicetype;                                                     //CNG/LNG��ǡ�
    UpLoad51Data.status=globalvar.workstate;                                 //����������״̬��
    UpLoad51Data.price =realtime.price;                                      //���׼۸�
    
    UpLoad51Data.classnum=sysparas.shiftnum;                                 //��š�
    memcpy(&UpLoad51Data.employeenum[0],&shiftrecordinfo.cardstaffnum[0],8); //Ա���š�
    memcpy(&UpLoad51Data.cardnum[0],&m1card.cardnum[0],8);                   //�û����š�
    memcpy(&UpLoad51Data.carnum[0],&m1card.carnum[0],9);                     //���ƺš�
    if(globalvar.workstate==StaIdle)
    {
        UpLoad51Data.curpressure=0;           								 //��ǰѹ����
    
        UpLoad51Data.inflowrate=0;           								 //��Һ������
        UpLoad51Data.intemperature=0;        							     //��Һ�¶ȡ�
        UpLoad51Data.inamount=0;              								 //��Һ������
        UpLoad51Data.indensity=0;             								 //��Һ�ܶȡ�
        UpLoad51Data.ingain=0;                								 //��Һ���档
        
        UpLoad51Data.gasnum=0;                								 //��ǰ����������
        UpLoad51Data.gasmoney=0;              								 //��ǰ���׽�
        
        UpLoad51Data.returnflowrate=0;        								 //����������
        UpLoad51Data.returntemperature=0;     								 //�����¶ȡ�
        UpLoad51Data.returnamount=0;          								 //����������
        UpLoad51Data.returndensity=0;         								 //�����ܶȡ�
        UpLoad51Data.returngain=0;            								 //�������档

    }else
    {
        UpLoad51Data.curpressure=realtime.cur_pressure;                 	 //��ǰѹ����
        UpLoad51Data.inflowrate=realtime.cur_flowrate_l;                	 //��Һ������
        UpLoad51Data.intemperature=realtime.cur_temperature_l;          	 //��Һ�¶ȡ�
        UpLoad51Data.inamount=realtime.cur_totalize_l;                  	 //��Һ������
        UpLoad51Data.indensity=realtime.cur_density_l;                  	 //��Һ�ܶȡ�
        UpLoad51Data.ingain=realtime.cur_gain_l;                         	 //��Һ���档
        if(globalvar.workstate==StaFuel)
        {
            UpLoad51Data.gasnum=fuelrecordinfo.volume;                       //��ǰ����������
            UpLoad51Data.gasmoney=fuelrecordinfo.money;                      //��ǰ���׽�
        }
        else
        {
            UpLoad51Data.gasnum=0;                      	 
            UpLoad51Data.gasmoney=0;
        }
        
        UpLoad51Data.returnflowrate=realtime.cur_flowrate_g;            	 //����������
        UpLoad51Data.returntemperature=realtime.cur_temperature_g;      	 //�����¶ȡ�
        UpLoad51Data.returnamount=realtime.cur_totalize_g;              	 //����������
        UpLoad51Data.returndensity=realtime.cur_density_g;              	 //�����ܶȡ�
        UpLoad51Data.returngain=realtime.cur_gain_g;                    	 //�������档
    }
    UpLoad51Data.systemalarm=syserr;                                    	 //ϵͳ������
    
}

/********************************************************
**�ṹ����:Receive51H()
**�ṹ����:�ظ�51���

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 

void Receive51H(void)
{    
    memset(SendBack485Buf,0,sizeof(SendBack485Buf));                    //�巢�����顣
    if(globalvar.workstate==StaFuel)                                    //������״̬��������
    {
        UpLoad51H();
    }
    else if(globalvar.workstate==StaPay)                                //������״̬���ۿ
    {
        if((m1card.debittimes!=0)&&(m1card.existflag==1))            		//����ۿ10��δ�ɹ�����ֹͣ����ۿ
        {
            UpLoad55H(0);
        } 
    }
    else if((globalvar.workstate==StaIdle)||(globalvar.workstate==StaOffDuty))                           
    {                
        if(abs(((RecBack485Buf[12] >> 4) - (time.minute >> 4))* 10 + (RecBack485Buf[12] & 0x0F) - (time.minute & 0x0F)) > 2 	
            || memcmp(&RecBack485Buf[8],&time.year,4) !=0)	            //�����̨ʱ��ͼ�����ʱ�����2���ӣ���ô�Ժ�̨��ʱ��Ϊ׼��
        {
            Ds3232SetTime(&RecBack485Buf[8]);
        }
        else if((RecBack485Buf[14]!=sysparas.blackver)&&(RecBack485Buf[14]!=0))            //�����������汾�Ų��ȡ�0 ��ʾ�������������Ϸ���û��
        {
            if(LoadSuccess==0)                       					//��ʾ��������Ҫ���ء�                    
            {
                UpLoad53Data.loadtype=1;             					//��ʾ��Ҫ���ػ�����������
                UpLoad53H();                         					//���������ݰ汾���ж�,����������Ӧ���ݡ�
            }
            if(LoadSuccess==1)                       					//������ػ�û����ɣ������������ݡ�
            {
                UpLoad54H();
            }
            if(LoadSuccess==2)                       					//���������ɡ�
            {
                sysparas.blackver=RecBack485Buf[14];
                LoadSuccess=0;
																		//��������Ϣ����EE
                SaveEeprom(BLACK_INFO_ADDR,(uint8_t*)&bwinfo.version,sizeof(bwinfo)); 
                SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
            }
        }
        else if((RecBack485Buf[15]!=sysparas.addbkver)&&(RecBack485Buf[15]!=0)) //�����������汾�Ų��ȡ�0 ��ʾ�������������Ϸ���û��
        {
            if(LoadSuccess==0)                       					//��ʾ��������Ҫ���ء�                    
            {
                UpLoad53Data.loadtype=2;             					//��ʾ��Ҫ���ػ�����������
                UpLoad53H();                         					//���������ݰ汾���ж�,����������Ӧ���ݡ�
            }
            if(LoadSuccess==1)                       					//������ػ�û����ɣ������������ݡ�
            {
                UpLoad54H();
            }
            if(LoadSuccess==2)                       					//���������ɡ�
            {
                sysparas.addbkver=RecBack485Buf[15];
                LoadSuccess=0;
																		//��������Ϣ����EE
                SaveEeprom(BLACK_ADD_INFO_ADDR,(uint8_t*)&bwinfo.version,sizeof(bwinfo)); 
                SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
            }
        }
        else if((RecBack485Buf[16]!=sysparas.delbkver)&&(RecBack485Buf[16]!=0))                //�����������汾�Ų��ȡ�0 ��ʾ��ɾ���������Ϸ���û��
        {
            if(LoadSuccess==0)                       					//��ʾ��������Ҫ���ء�                    
            {
                UpLoad53Data.loadtype=3;             					//��ʾ��Ҫ���ػ�����������
                UpLoad53H();                         					//���������ݰ汾���ж�,����������Ӧ���ݡ�
            }
            if(LoadSuccess==1)                       					//������ػ�û����ɣ������������ݡ�
            {
                UpLoad54H();
            }
            if(LoadSuccess==2)                       					//���������ɡ�
            {
                sysparas.delbkver=RecBack485Buf[16];
                LoadSuccess=0;
																		//��������Ϣ����EE
                SaveEeprom(BLACK_DEL_INFO_ADDR,(uint8_t*)&bwinfo.version,sizeof(bwinfo)); 
                SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
            }
        }
        else if((RecBack485Buf[17]!=sysparas.whitever)&&(RecBack485Buf[17]!=0))                //�������汾�Ų��ȡ�0 ��ʾ���������Ϸ���û��
        {
            if(LoadSuccess==0)                       					//��ʾ��������Ҫ���ء�                    
            {
                UpLoad53Data.loadtype=4;             					//��ʾ��Ҫ���ػ�����������
                UpLoad53H();                         					//���������ݰ汾���ж�,����������Ӧ���ݡ�
            }
            if(LoadSuccess==1)                       					//������ػ�û����ɣ������������ݡ�
            {
                UpLoad54H();
            }
            if(LoadSuccess==2)                      					//���������ɡ�
            {
                sysparas.whitever=RecBack485Buf[17];
                LoadSuccess=0;
                SaveEeprom(WHITE_INFO_ADDR,(uint8_t*)&bwinfo.version,sizeof(bwinfo)); //��������Ϣ����EE
                SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
            }
        }
        else if((RecBack485Buf[18]!=sysparas.pricever)&&(RecBack485Buf[18]!=0))                //���۰汾�Ų��ȡ�0 ��ʾ���۰汾���Ϸ���û��.
        {
            UpLoad5EH();
        }
        else if((RecBack485Buf[19]!=sysparas.lockver)&&(RecBack485Buf[19]!=0)) //�ж���Կ�汾�š�
        {
            if(LoadSuccess==0)
            {
                UpLoad53Data.loadtype=6;
                UpLoad53H();                         					//���������ݰ汾���ж�,����������Ӧ���ݡ�
            }
            if(LoadSuccess==2)
            {
                sysparas.lockver=RecBack485Buf[19];
                LoadSuccess=0;
                SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));              
            } 
        }
        else if(sysparas.transnum>sysparas.uptransnum)//�ж��Ƿ���δ�ϴ���ˮ��
        {
           UpLoad52H();
        }
        else if(m1card.greycard==1)  //ycx20161201��ˮ�ϴ����˺�������ҡ�
        {
            UpLoad55H(1);
        }
        else if(m1card.checkflag==1)    //ycx20161201��ˮ�ϴ����˺�������鿨��
        {
            UpLoad5CH();
        }
        else if(sysparas.shiftransnum>sysparas.shiftuptransnum)//�ж��Ƿ���δ�ϴ�����ˮ��
        {
            UpLoad5AH();
        }
        else if(sysparas.modnum>sysparas.upmodnum)//�ж��Ƿ���δ�ϴ��޸���ˮ�š�
        {
            UpLoad5BH();
        }
        else                                                   			//51H����-���¿���ʱ�ϴ���51H���ݡ�
        {
            UpLoad51H(); 
        }
    }
    else                                                   				//51H����-���¿���ʱ�ϴ���51H���ݡ�
    {
        UpLoad51H();
    }
}
/********************************************************
**�ṹ����:UpLoad51H(void)
**�ṹ����:52H����,�������ϴ����׼�¼��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad51H(void)
{ 
    GetRep51Data();                            
    memcpy(&SendBack485Buf[7],&UpLoad51Data.LCNG,sizeof(Reply51));
    SendBackCommunicationData(0x51,sizeof(Reply51));
}

/********************************************************
**�ṹ����:UpLoad(void)
**�ṹ����:52H����,�������ϴ����׼�¼��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad52H(void)
{ 
    if(globalvar.uptransnum==sysparas.uptransnum)
    {
        globalvar.UploadIdCount++;
        if(globalvar.UploadIdCount>10)//�ϴ�10�β��ɹ��������˱���ˮ
        {
            globalvar.UploadIdCount=0;
            sysparas.uptransnum++;
            SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
            return;
        }        
    } 
    globalvar.uptransnum=sysparas.uptransnum;
    fuelrecordinfo_tmp=fuelrecordinfo;
    if(ReadFuelRecord(sysparas.uptransnum,0))
    {
       memcpy(&SendBack485Buf[7],&fuelrecordinfo.transnum,sizeof(fuelrecordinfo)-2);
       fuelrecordinfo=fuelrecordinfo_tmp;
       SendBackCommunicationData(0x52,sizeof(fuelrecordinfo)-2);
    } 
}

/********************************************************
**�ṹ����:Receive52H()
**�ṹ����:�ظ�52H����,��̨ϵͳ�ظ��������ϴ����׼�¼��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive52H(void)
{
    
    if(RecBack485Buf[7]==0)       //�ϴ�ʧ�ܡ�
    {
        globalvar.UploadIdCount++;          //��ʮ��֮���Ƿ����㣿������      
    }
    else if(RecBack485Buf[7]==1)  //�ϴ��ɹ���
    {
		globalvar.UploadIdCount=0;        
        if(sysparas.uptransnum<sysparas.transnum)
		{
			 sysparas.uptransnum++;
             SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
		}        
    }
}

/********************************************************
**�ṹ����:UpLoad53H()
**�ṹ����:53H������������ݰ汾���ж��Ƿ���Ҫ������Ӧ���ݡ�

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad53H(void)
{
    memset(SendBack485Buf,0,sizeof(SendBack485Buf));        //�����������㡣
    memcpy(&SendBack485Buf[7],&UpLoad53Data.loadtype,7);
    SendBackCommunicationData(0x53,7);
}

/********************************************************
**�ṹ����:Receive53H()
**�ṹ����:�ظ�53���

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive53H(void)
{
    uint32_t len=0;
    if(RecBack485Buf[7]==6)
    {
        if(SaveEeprom(LOCKVER_ADDR,&RecBack485Buf[12],16));//�洢��Կ��
        {
           LoadSuccess=2;
        } 
    }
    else
    {
        BufReverse2(&RecBack485Buf[8], 4);       			//��С�˵�����
        memcpy(&len,&RecBack485Buf[8],4);        			//�õ����ݳ��ȡ�
        allpart=len ; 					         			//�õ������������������ֽ�������
        
        UpLoad54Data.loadtype = RecBack485Buf[7];			//Ҫ���ص����ݡ�
        UpLoad54Data.partoffset=0;               			//�����ؿ��Ÿ�����  
       
        UpLoad54Data.partnum = 20;        		 			//�������ؿ��Ÿ�����
        memcpy(&SendBack485Buf[7],&UpLoad54Data.loadtype,4); 
        SendBackCommunicationData(0x54,4);
    }
    
}

/********************************************************
**�ṹ����:UpLoad54H()
**�ṹ����:54H����,���������������������ݡ�

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad54H(void)
{
    memset(SendBack485Buf,0,sizeof(SendBack485Buf));        //�����������㡣
    memcpy(&SendBack485Buf[7],&UpLoad54Data.loadtype,4); 
    SendBackCommunicationData(0x54,4);
}

/********************************************************
**�ṹ����:Receive54H()
**�ṹ����:�ظ�54���

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive54H(void)
{
	uint16_t loaded; 						   //�����ض�����
	uint16_t loading;						   //�������صĶ�����
	uint32_t addr;
	uint8_t  src[160];
    
    BufReverse2(&RecBack485Buf[8], 2);         //��С�˵�����
	memcpy(&loaded,&RecBack485Buf[8],2);
	loading=RecBack485Buf[10];
	memcpy(src,&RecBack485Buf[27],loading*8);
	CloseUartRcvIRQ(3);
	CloseUartRcvIRQ(0);
    switch(RecBack485Buf[7])
    {
		
        case 1:								   //�������������
			addr=FlashBlkSADDR+loaded*FlashBlkLen;
            SaveFlash(0,addr,src, loading*8);
			SaveFlash(1,addr,src, loading*8);
            break;
        case 2:								   //��������������
            addr=FlashAddBlkSADDR+loaded*FlashAddBlkLen;
            SaveFlash(0,addr,src, loading*8);
			SaveFlash(1,addr,src, loading*8);
            break;
        case 3:								   //����ɾ��������
            addr=FlashDelBlkSADDR+loaded*FlashDelBlkLen;
            SaveFlash(0,addr,src, loading*8);
			SaveFlash(1,addr,src, loading*8);
            break;
        case 4:								   //���������
            addr=FlashWhiteSADDR+loaded*FlashWhiteLen;
            SaveFlash(0,addr,src, loading*8);
			SaveFlash(1,addr,src, loading*8);
            break;
        default:
            break;
    }
	OpenUartRcvIRQ(PCPort);
	OpenUartRcvIRQ(CardPort);
	
    if((loaded+loading)<allpart)               //�ж������Ƿ���ϡ�
    {
		UpLoad54Data.loadtype=RecBack485Buf[7];//��Ҫ���ص����ݴ���
		UpLoad54Data.partoffset=loaded+loading;//�����ؿ��Ÿ�����
		UpLoad54Data.partnum=20;               //�������ؿ��Ÿ�����
        LoadSuccess=1;                     	   //����δ��ɡ�
    }
    else                                   	   //������ɡ�
    {										   //���سɹ���������Ϣ�������ṹ��
	    memcpy(&bwinfo.version, &RecBack485Buf[11], sizeof(bwinfo));  
        LoadSuccess=2;
        if(RecBack485Buf[7]==1)
        {
            sysparas.blacknum=allpart;
        }else if(RecBack485Buf[7]==2)
        {
            sysparas.blackaddnum=allpart;
        }else if(RecBack485Buf[7]==3)
        {
            sysparas.blackdelnum=allpart;
        }else if(RecBack485Buf[7]==4)
        {
            sysparas.whitenum=allpart;
        }
        SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
    }
}

/********************************************************
**�ṹ����:UpLoad55H()
**�ṹ����:�ϴ�55H��������������ҡ�0������ۿ�1��������

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad55H(uint8_t type)
{
	UpLoad55Data.type = type;
    UpLoad55Data.cardtype=m1card.cardtype;						   //�����͡�
	memcpy(UpLoad55Data.cardnum, m1card.cardnum, 8);
    if(type==0)													   //����ۿ
    {
        UpLoad55Data.gasnum=fuelrecordinfo.volume;        		   //��ǰ����������
        UpLoad55Data.money=fuelrecordinfo.money;    			   //��ǰ������
        memcpy(UpLoad55Data.stationnum, sysparas.stationcode, 4);  //վ���롣
        UpLoad55Data.gunnum = sysparas.gunnum;      			   //���һ�μ���ǹ�š�
        UpLoad55Data.gunnumber = sysparas.transnum; 			   //���һ�μ�����ˮ�š�
    }else if(type==1)											   //�����ҡ�
    {
        UpLoad55Data.gasnum=0;
        UpLoad55Data.money=0;
        memcpy(UpLoad55Data.stationnum, m1card.laststationcode, 4);
        UpLoad55Data.gunnum = m1card.gunnum;
        UpLoad55Data.gunnumber = m1card.transnum;
    }
	UpLoad55Data.cardover = m1card.cardbalance;					   //����
	
    memset(SendBack485Buf,0,sizeof(SendBack485Buf));        	   //�����������㡣
    memcpy(&SendBack485Buf[7],&UpLoad55Data.type,sizeof(Reply55)); //�����һ�ۿ����ݡ�
    SendBackCommunicationData(0x55,sizeof(Reply55));
}

/********************************************************
**�ṹ����:Receive55H()
**�ṹ����:����55H�����̨ϵͳ�·�����������ݡ�

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive55H(void)
{
    FuelRecordInfo fueltmp1,fueltmp2;
	Receive55data *ptr;
	ptr = (Receive55data *)(&RecBack485Buf[7]);
    m1card.debittimes=0;
    StrReverse((uint8_t *)&ptr->realmoney,4,4);
    StrReverse((uint8_t *)&ptr->price,1,2);
    StrReverse((uint8_t *)&ptr->volume,5,4);
    
	if(RecBack485Buf[8] == 1)										       //���
	{
		if(RecBack485Buf[7]==0)											   //�޴˿��Ҽ�¼
		{
			Fyd12864DispPrintf(3,1,"�޴˿��Ҽ�¼��");					   //δ�ҵ��Ҽ�¼��
		}
		else if(RecBack485Buf[7]==1)
		{
			if(memcmp(&RecBack485Buf[9],&m1card.cardnum,8) == 0)		   //�жϿ����Ƿ���ȷ
			{
                //��5
				m1card.lastoptmode = 3;                             	   //������ 
                m1card.trademoney=ptr->realtrademoney;              	   //���׶
                m1card.cardbalance=ptr->aftermoney;                 	   //��
                //��8
                m1card.sumtimes=ptr->totaltimes;                    	   //�ۼ����Ѵ�����
                m1card.summoney=ptr->totalmoney;                    	   //�ۼ����ѽ�
                m1card.sumvolme=ptr->totalvolume;                   	   //�ۼ�����������
                //��10
                m1card.cardstate = 1;	                            	   //���
                
                fueltmp1=fuelrecordinfo;
                sysparas.transnum++;
                fueltmp2.transnum=sysparas.transnum;                       //��ˮ�š�
                fueltmp2.classnum=sysparas.shiftnum;                       //��š�
                														   //Ա���š�
                memcpy(&fueltmp2.employeenum[0],&shiftrecordinfo.cardstaffnum[0],8);
                fueltmp2.transactiontype=2;                                //���۽��ס�
                memcpy(&fueltmp2.usernum[0],&m1card.cardnum[0],8);         //���š�
                fueltmp2.cardtype=m1card.cardtype;                     	   //�����͡�
                memcpy(&fueltmp2.carnum[0],&m1card.carnum[0],9);       	   //���ƺš�
                memcpy(&fueltmp2.stationnum[0],&sysparas.stationcode[0],4);//վ���롣
                memcpy(&fueltmp2.starttime[1], &time.year, 6);       //������ʼʱ�䡣
                memcpy(&fueltmp2.endtime[1], &time.year, 6);         //��������ʱ�䡣
                fueltmp2.price=ptr->price;                                 //���׼۸�
                fueltmp2.volume=ptr->volume;                               //����������
                fueltmp2.money=ptr->realmoney;                             //������
                fueltmp2.reallymoney=ptr->realtrademoney;                  //ʵ�ʿۿ��
                fueltmp2.beforebalance=ptr->beforemoney;                   //����ǰ��
                fueltmp2.afterbalance=ptr->aftermoney;                     //��������
                fueltmp2.allrechargetimes=m1card.sumchargetimes;           //�ۼƳ�ֵ������
                fueltmp2.allrechargemoney=m1card.sumchargemoney;           //�ۼƳ�ֵ��
                fueltmp2.allconsumptiontimes=ptr->totaltimes;              //�ۼ����Ѵ�����
                fueltmp2.allconsumptionnum=ptr->totalvolume;               //�ۼ�����������
                fueltmp2.allconsumptionmoney=ptr->totalmoney;              //�ۼ����ѽ�
                fueltmp2.allgunnum=sysparas.totvolume;                     //ǹ�����ۼơ�
                fueltmp2.allgunmoney=sysparas.totmoney;                    //ǹ����ۼơ�
                fueltmp2.beforepressure=0;                                 //����ǰѹ����
                fueltmp2.afterpressure=0;                                  //������ѹ����
                fueltmp2.density=sysparas.densty;                                        //�ܶȡ�
                fueltmp2.curnum=sysparas.coeff;                            //������
                fueltmp2.measuretype=sysparas.unit;                        //������λ��0-�귽��1-Kg��
                fueltmp2.devicetype=sysparas.devicetype;                                       //��Ʒ���롣
                fueltmp2.stopreason=0xFF;                                     //ͣ��ԭ��
                fueltmp2.jhid=ptr->removegrey;                             //��������ˮ���롣
                fueltmp2.classid=sysparas.shiftransnum;                    //����ˮ�š� 
                fueltmp2.fuelmode=sysparas.fuelmode;
                fueltmp2.crc=CRC16Calculate((uint8_t *)(&fueltmp2.transnum),sizeof(FuelRecordInfo)-2);
                
                StrReverse((uint8_t *)&m1card.trademoney,2,4);         	   //��5������
                StrReverse((uint8_t *)&m1card.sumtimes,3,4);               //��8������
                StrReverse((uint8_t *)&m1card.transnum,1,4);           	   //��10������
                 														   //����5���ݵ���6��
                Mt318WriteM1Card(KEYB,&m1card.keyB[0], 6, &m1card.balancecrc);   
				if(Mt318WriteM1Card(KEYB,&m1card.keyB[0], 5, (uint8_t *)&m1card.balancecrc))
				{	
                    if(Mt318WriteM1Card(KEYB,&m1card.keyB[0], 8,(uint8_t *)&m1card.sumtimes))
                    {
                        if(Mt318WriteM1Card(KEYB,&m1card.keyB[0],10,(uint8_t *)&m1card.cardstate))
                        {
                            m1card.greycard=0;
                            Fyd12864DispPrintf(2,1,"��ҳɹ���");
                            if(SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas)))
                            {
                                fuelrecordinfo=fueltmp2;
                                if(SaveFuelRecord(0))
                                {
                                    if(sysparas.printmode==1)  //�ж��Ƿ��Զ���ӡһ�Ρ�
                                    {
                                        Printid();
                                    }
                                    Fyd12864DispPrintf(4,1,"�洢��ˮ�ɹ�"); 
                                    DispUI=DispFirst;
                                }
                                else
                                {
                                    Fyd12864DispPrintf(4,1,"�洢��ˮʧ��");  
                                }
                                fuelrecordinfo=fueltmp1;
                            }
                        }
                        else
                        {
                            Fyd12864DispPrintf(2,1,"���ʧ�ܣ�");
                        }
                    }
					else
					{
						Fyd12864DispPrintf(2,1,"���ʧ�ܣ�");
					}
				}
				else
				{
					Fyd12864DispPrintf(2,1,"���ʧ�ܣ�");
				}					
			}
			else
			{
				Fyd12864DispPrintf(2,1,"�޴˿��ţ�");
			}
		}
	}
	else if (RecBack485Buf[8] == 0)									//�ۿ�
	{
        //��5
        m1card.lastoptmode = 1; 		                            //�ۿ�
        memcpy(m1card.lasttradetime, &fuelrecordinfo.endtime[1], 6);//��������ˮʱ�䡣
        m1card.trademoney = ptr->realtrademoney;                    //���׶
        
        if(m1card.cardtype==1)									    //�����
        {
            m1card.cardbalance -= ptr->realtrademoney;              //��
        }
        else if(m1card.cardtype==2)									//���˿���
        {
            m1card.cardbalance += ptr->realtrademoney;              //��
        }
        
        //��8
        m1card.sumtimes++;                                          //�����Ѵ�����
        m1card.summoney += ptr->realtrademoney;                     //�����ѽ�
        m1card.sumvolme += fuelrecordinfo.volume;                   //������������
        //��10
        m1card.cardstate = 1;						                //���
        m1card.gunnum = sysparas.gunnum;                            //�����ǹ���š�
        m1card.transnum = fuelrecordinfo.transnum;                  //�������ˮ�š�
        memcpy(m1card.laststationcode, sysparas.stationcode, 4);    //վ���롣

        StrReverse((uint8_t *)&m1card.trademoney,2,4);              //��5������
        StrReverse((uint8_t *)&m1card.sumtimes,3,4);                //��8������
        StrReverse((uint8_t *)&m1card.transnum,1,4);                //��10������
        														    //����5���ݵ���6��
        Mt318WriteM1Card(KEYB, &m1card.keyB[0], 6,&m1card.balancecrc);         
        if(Mt318WriteM1Card(KEYB, &m1card.keyB[0], 5, (uint8_t *)&m1card.balancecrc))         
        {
            if(Mt318WriteM1Card(KEYB, &m1card.keyB[0], 8, (uint8_t *)&m1card.sumtimes))  
            {
                if(Mt318WriteM1Card(KEYB,&m1card.keyB[0], 10,(uint8_t *)&m1card.cardstate))
                {
                    Fyd12864DispPrintf(2,1,"�ۿ�ɹ�!");
                    fuelrecordinfo.transactiontype=0;                   //�������͡�
                    fuelrecordinfo.reallymoney=ptr->realtrademoney;     //����ʵ�ʿۿ��
                    
                    StrReverse((uint8_t *)&m1card.summoney,1,4);  
                    fuelrecordinfo.allconsumptionmoney=m1card.summoney; //�ۼ����ѽ�
                    StrReverse((uint8_t *)&m1card.cardbalance,1,4);  
                    fuelrecordinfo.afterbalance=m1card.cardbalance;     //��������
                    fuelrecordinfo.crc=CRC16Calculate((uint8_t *)(&fuelrecordinfo.transnum),sizeof(FuelRecordInfo)-2);
                }
                else                                                    //д��10��
                {
                    Fyd12864DispPrintf(2,1,"��10�ۿ�ʧ��!");
                    fuelrecordinfo.transactiontype=0x1;                 //�������͡�
                    fuelrecordinfo.crc=CRC16Calculate((uint8_t *)(&fuelrecordinfo.transnum),sizeof(FuelRecordInfo)-2);
                }
            }
            else                                                        //д��8��                                                    
            {
                Fyd12864DispPrintf(2,1,"��8 �ۿ�ʧ��!");
                fuelrecordinfo.transactiontype=0x1;                     //�������͡�
                fuelrecordinfo.crc=CRC16Calculate((uint8_t *)(&fuelrecordinfo.transnum),sizeof(FuelRecordInfo)-2);
            }
        }
        else                                                            //д��5��
        {
            Fyd12864DispPrintf(2,1,"��5 �ۿ�ʧ��!");
            fuelrecordinfo.transactiontype=0x1;                         //�������͡�
            fuelrecordinfo.crc=CRC16Calculate((uint8_t *)(&fuelrecordinfo.transnum),sizeof(FuelRecordInfo)-2);
        }	
	}
}


/********************************************************
**�ṹ����:UpLoad56H()
**�ṹ����:�ϴ�56H��������������ѯ�ڰ�������

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad56H(uint8_t type)
{
    memset(SendBack485Buf,0,sizeof(SendBack485Buf));                     //�����������㡣
    memcpy(&UpLoad56Data.cardnum[0],&m1card.cardnum[0],8); 
    UpLoad56Data.seekname=type;
    memcpy(&SendBack485Buf[7],&UpLoad56Data.cardnum[0],sizeof(Reply56)); //�����ѯ�ڰ�������
    SendBackCommunicationData(0x56,sizeof(Reply56));
}

/********************************************************
**�ṹ����:Receive56H()
**�ṹ����:����56H�����̨�·��ڰײ�ѯ�����

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive56H(void)
{
    if(RecBack485Buf[15]==0)
    {
        m1card.blackorwhite=1;//���Ǻ�/�׿���
    }
    else if(RecBack485Buf[15]==1)
    {
        m1card.blackorwhite=2;//�Ǻ�/�׿���
    }    
}

/********************************************************
**�ṹ����:Receive57H()
**�ṹ����:����57H�����̨ϵͳ���������ۼ�����

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive57H(void)
{
    UpLoad57Data.transactionid = sysparas.transnum;            				//������ˮ�š�
	UpLoad57Data.allgasnum     = sysparas.totvolume;           				//�����������ۼơ�
    UpLoad57Data.allgasmoney   = sysparas.totmoney;            				//����������ۼơ�
    UpLoad57Data.allclassnum   = sysparas.shiftotvol;          				//���ۼ�������
    UpLoad57Data.allclassmoney = sysparas.shiftotmon;          			    //���ۼƽ�
    memset(SendBack485Buf,0,sizeof(SendBack485Buf));           				//�����������㡣
    memcpy(&SendBack485Buf[7], &UpLoad57Data.transactionid,sizeof(Reply57));//�õ��ۼ�����
   
    SendBackCommunicationData(0x57,sizeof(Reply57));
}

/********************************************************
**�ṹ����:Receive58H()
**�ṹ����:����58H�����̨ϵͳ��������ָ����ˮ��¼��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive58H(void)
{
    uint32_t id=0;
    memset(SendBack485Buf,0,sizeof(SendBack485Buf));//�����������㡣
    BufReverse2(&RecBack485Buf[7], 4);
    memcpy(&id,&RecBack485Buf[7],4);                //�õ�ָ����ˮId�š�
    if(id<=sysparas.transnum)                       //�ж��Ƿ��д�Id�š�
    {
        UpLoad58Data.transactionid=id;
        UpLoad58Data.ifid=1;                        //�иñ���ˮ�š�
        id=(id-1)%FlashRecMaxID;
        fuelrecordinfo_tmp=fuelrecordinfo;
        if(ReadFuelRecord(id,0))
        {
            memcpy(&SendBack485Buf[7],&UpLoad58Data.transactionid,sizeof(Reply58));
            memcpy(&SendBack485Buf[12],&fuelrecordinfo.transnum,sizeof(fuelrecordinfo)-2);
            fuelrecordinfo=fuelrecordinfo_tmp;
            SendBackCommunicationData(0x58,(sizeof(fuelrecordinfo)-2)+5);
        }
    }
    
    else                                            //û�д�Id�š�
    {
        UpLoad58Data.transactionid=id;
        UpLoad58Data.ifid=0;                        //û�иñ���ˮ�š�
        memcpy(&SendBack485Buf[7],&UpLoad58Data.transactionid,sizeof(Reply58));
        SendBackCommunicationData(0x58,sizeof(Reply58));
    }
}

/********************************************************
**�ṹ����:Receive59H()
**�ṹ����:����59H�����̨ϵͳ��ȡ������ͨ����Ϣ��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive59H(void)
{
    UpLoad59Data.gunnum            = sysparas.gunnum;                   //ǹ�š�
    UpLoad59Data.transactionid     = sysparas.transnum;                 //������ˮ�š�
    UpLoad59Data.baseblackversion  = sysparas.blackver;                 //�����������汾�š�
    UpLoad59Data.newaddblackversion= sysparas.addbkver;                 //�����������汾�š�
    UpLoad59Data.newdelblackversion= sysparas.delbkver;                 //��ɾ�������汾�š�
    UpLoad59Data.whitelistversion  = sysparas.whitever;                 //�������汾�š�
    UpLoad59Data.priceversion      = sysparas.pricever;                 //���۰汾�š�
    UpLoad59Data.baseinfoversion   = sysparas.comminfover;              //ͨ����Ϣ�汾�š�
    memcpy(&UpLoad59Data.stationnum[0]   ,&sysparas.stationcode[0],4);  //վ���룬BCD�롣
    memcpy(&UpLoad59Data.provincenum[0]  ,&sysparas.shengcode[0],4);    //ʡ���룬BCD�롣
    memcpy(&UpLoad59Data.citynum[0]      ,&sysparas.citycode[0],4);     //���д��룬BCD�롣
    memcpy(&UpLoad59Data.parentunitnum[0],&sysparas.parentcode[0],4);   //�ϼ���λ���룬BCD�롣
                                                                        //����վ���ƣ�ASCII�롣
                                                                        //Ƕ��ʽ����汾��,ASCII�롣
                                                                        //������������Ϣ��ASCII�롣
    memset(SendBack485Buf,0,sizeof(SendBack485Buf));                    //�����������㡣
    memcpy(&SendBack485Buf[7],&UpLoad59Data.gunnum,sizeof(Reply59));    //�õ�������ͨ����Ϣ��
    
    SendBackCommunicationData(0x59,sizeof(Reply59));
}

/********************************************************
**�ṹ����:Receive5AH()
**�ṹ����:����5AH���°������̨�����ϴ������

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive5AH(void)
{
    if(RecBack485Buf[7]==0)		 //�ϴ�ʧ�ܡ�
    {
        globalvar.UploadIdCount++;
    }
    else if(RecBack485Buf[7]==1) //�ϴ��ɹ���
    {
         globalvar.UploadIdCount=0;
         sysparas.shiftuptransnum++;
         SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));           
    }  
}
/********************************************************
**�ṹ����:UpLoad5AH()
**�ṹ����:�ϴ����°��¼

**�� �� ��:���
**��������:2016-12-1 16:56:03

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad5AH(void)
{
    if(globalvar.uptransnum==sysparas.shiftuptransnum)
    {
        globalvar.UploadIdCount++;
        if(globalvar.UploadIdCount>10)
        {
            globalvar.UploadIdCount=0;
            sysparas.shiftuptransnum++;
            SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
            return;
        }
    }    
    globalvar.uptransnum=sysparas.shiftuptransnum;
    shiftrecordinfo_tmp=shiftrecordinfo;
    if(ReadFuelRecord(sysparas.shiftuptransnum,1))
    {
       memcpy(&SendBack485Buf[7],&shiftrecordinfo.classid,sizeof(ShiftRecordInfo)-2);
       shiftrecordinfo=shiftrecordinfo_tmp;
       SendBackCommunicationData(0x5A,sizeof(ShiftRecordInfo)-2);
    }
}


/********************************************************
**�ṹ����:Receive5BH()
**�ṹ����:����5BH�����̨�����ϴ������

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive5BH(void)
{
    if(RecBack485Buf[7]==0)		//�ϴ�ʧ�ܡ�
    {
        globalvar.UploadIdCount++;
    }
    else if(RecBack485Buf[7]==1)//�ϴ��ɹ���
    {
        globalvar.UploadIdCount=0; 
        sysparas.upmodnum++;
        SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas)); 
    } 
}

/********************************************************
**�ṹ����:UpLoad5BH()
**�ṹ����:�ϴ������޸ļ�¼

**�� �� ��:���
**��������:2016-12-1 16:56:03

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad5BH(void)
{
    uint32_t addr,id;                                                   //������0x5B���
    uint8_t *pt;                                                        //������0x5B���
    Reply5B tmp;
    if(globalvar.uptransnum==sysparas.upmodnum)
    {
        globalvar.UploadIdCount++;
        if(globalvar.UploadIdCount>10)
        {
            globalvar.UploadIdCount=0;
            sysparas.upmodnum++;
            SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
            return;
        }
    }    
    globalvar.uptransnum=sysparas.upmodnum;
    id=sysparas.upmodnum%FlashModMaxID;
    addr=FlashModSADDR+id*FlashModLen;
    pt=(uint8_t *)(&tmp.modifyid);  
    if(ReadFlash(0,addr,pt,sizeof(Reply5B))==FALSE)
    {
        ReadFlash(1,addr,pt,sizeof(Reply5B));
    }
    memcpy(&SendBack485Buf[7],&tmp.modifyid,sizeof(Reply5B));
    SendBackCommunicationData(0x5B,sizeof(Reply5B));
}


/********************************************************
**�ṹ����:UpLoad5CH(void)
**�ṹ����:5CH�����������֤IC�������ԡ�

**�� �� ��:���
**��������:2016-06-8

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad5CH(void)
{
    memset(SendBack485Buf,0,sizeof(SendBack485Buf));    //�����������㡣
    memcpy(&SendBack485Buf[7],m1card.cardnum,8);		//��Ҫ��֤��IC�����š�
	memcpy(&SendBack485Buf[15],&m1card.cardbalance,4);	//���
	SendBack485Buf[19] = 0x20;							//����
	memcpy(&SendBack485Buf[20],m1card.lasttradetime,6);	//������ʱ����
    SendBackCommunicationData(0x5C,sizeof(Reply5C));						
}

/********************************************************
**�ṹ����:Receive5CH(void)
**�ṹ����:5CH�����̨�ָ�����������֤�����

**�� �� ��:���
**��������:2016-06-8

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive5CH(void)
{ 
	// 0-��������
    // 1-���󿨺Ų����ڡ� 
    // 2-�������š�  
    // 3-����쳣��
    // 4-�����ʱ���쳣��
    m1card.checkresult = RecBack485Buf[15];
    m1card.checkflag=0;
}

/********************************************************
**�ṹ����:UpLoad5DH(void)
**�ṹ����:5DH��������������°�����

**�� �� ��:���
**��������:2016-06-8

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad5DH(void)
{
    memset(SendBack485Buf,0,sizeof(SendBack485Buf));                  //�����������㡣
    memcpy(&SendBack485Buf[7],&UpLoad5DData.OffWork,sizeof(Reply5D)); //�õ��������ݡ�
    SendBackCommunicationData(0x5D,sizeof(Reply5D));
}

/********************************************************
**�ṹ����:Receive5DH()
**�ṹ����:����5DH�����̨�Լ������·��°����

**�� �� ��:���
**��������:2016-06-8

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive5DH(void)
{
    uint8_t *tmp;
    sysparas.shiftransnum++;
    shiftrecordinfo.classid=sysparas.shiftransnum;                                //���°��¼��ˮ�ţ�4�ֽڡ�
    shiftrecordinfo.transactiontype=1;                                            //�°ࡣ
    shiftrecordinfo.classnum=sysparas.shiftnum;                                   //��ţ�1�ֽڡ�
                                                                                  //Ա�����ź��ϰ�ʱһ�������ֲ��䣬BCD�롣
    shiftrecordinfo.classstarttime[0]=0x20;                                       //�࿪ʼʱ�䣬7�ֽڣ�BCD�롣
    memcpy(&shiftrecordinfo.classstarttime[1], &sysparas.shiftstarttime[0], 6);
    
    shiftrecordinfo.classendtime[0]=0x20;                                         //�����ʱ�䣬7�ֽڣ�BCD�롣
    memcpy(&shiftrecordinfo.classendtime[1],&time.year,6);
    memcpy(&sysparas.shiftendtime[1],&time.year,6);
    
    shiftrecordinfo.allgunnum=sysparas.totvolume;                                 //ǹ�����ۼƣ�4�ֽڡ�
    shiftrecordinfo.allgunmoney=sysparas.totmoney;                                //ǹ����ۼ�,4�ֽڡ�
    shiftrecordinfo.classgasnum=sysparas.shiftotvol;                              //��������4�ֽڡ�
    shiftrecordinfo.classgasmoney=sysparas.shiftotmon;                            //����,4�ֽڡ�
    shiftrecordinfo.classtimes=sysparas.shiftfueltimes;                           //�����������4�ֽڡ�
    shiftrecordinfo.devicetype=sysparas.devicetype;                                                    //��Ʒ���룬1�ֽڣ�0-CNG��1-LNG��
    shiftrecordinfo.measuretype=sysparas.unit;                                    //������λ��1�ֽڣ�0-�귽��1-Kg��
    shiftrecordinfo.classiding=sysparas.shiftransnum--;                           //�ϰ��¼��ˮ�š�
   
    tmp=(uint8_t *)(&shiftrecordinfo.classid);
    shiftrecordinfo.crc=CRC16Calculate(tmp, sizeof(shiftrecordinfo)-2);
    SaveFuelRecord(1);                                                            //�����ˮ��
    sysparas.offonwork = 0;                                                       //�°ࡣ
    globalvar.workstate=StaOffDuty;
    memcpy(&sysparas.shiftendtime[0], &time.year, 6);
    SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));         //��ϵͳ������
    
    UpLoad5DData.OffWork = 1;
   
    UpLoad5DH();																  //�ϴ��°�����
}

/********************************************************
**�ṹ����:Receive5EH()
**�ṹ����:����5EH�����̨�����·������������ݡ�

**�� �� ��:���
**��������:2016-06-8

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive5EH(void)
{
    StrReverse(&RecBack485Buf[17],RecBack485Buf[16],2);
    memcpy(&sysparas.price[0], &RecBack485Buf[17], RecBack485Buf[16] * 2);
    if(SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas)))
    {
        OsplcShow(0,0,sysparas.price[0],0,0);
        sysparas.pricever = RecBack485Buf[7]; 
    } 
}

/********************************************************
**�ṹ����:UpLoad5EH(void)
**�ṹ����:�����������·��������ݡ�

**�� �� ��:���
**��������:2016-07-15

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void UpLoad5EH(void)
{
     UpLoad5EData.lcng=1;
	 memset(SendBack485Buf,0,sizeof(SendBack485Buf));//�����������㡣
     SendBack485Buf[7]=UpLoad5EData.lcng;
	 SendBackCommunicationData(0x5E,sizeof(Reply5E));
}

/********************************************************
**�ṹ����:Receive5FH(void)
**�ṹ����:��̨��������������������

**�� �� ��:���
**��������:2016-07-15

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive5FH(void)
{
    if(RecBack485Buf[7]==0)     					 //������������
    {
        sysparas.yesnolock= 0;
    }
    else if(RecBack485Buf[7]==1)					 //����������
    {
        sysparas.yesnolock= 1;
    }
    SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
    
    UpLoad5FData.commdwork=1;
    memset(SendBack485Buf,0,sizeof(SendBack485Buf)); //�����������㡣
    SendBack485Buf[7] = UpLoad5FData.commdwork;
    SendBackCommunicationData(0x5F,sizeof(Reply5F));
    
}

/********************************************************
**�ṹ����:Receive60H()
**�ṹ����:���պ�̨���͵ĵ��ӱ�ǩ��������̨���ؼ�����״̬��

**�� �� ��:���
**��������:2016-011-22

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void Receive60H(void)
{	
	if((globalvar.workstate==StaOffDuty)||                       //�������°ࡣ
		(globalvar.workstate==StaIdle)||						 //���������С�
		(globalvar.workstate==StaFault))						 //���������ϡ�
	{
	    globalvar.startstate = RecBack485Buf[7];				 //���ӱ�ǩ������״̬��
		globalvar.startways  = RecBack485Buf[8];				 //���ӱ�ǩ��������ʽ��
		memcpy(&globalvar.startnum[0], &RecBack485Buf[9], 8);    //���ӱ�ǩ���������кš�
		globalvar.handnum    = RecBack485Buf[17];				 //���ӱ�ǩ���ֳֻ���š�
	
		memset(SendBack485Buf,0,sizeof(SendBack485Buf));         //�����������㡣
		SendBack485Buf[7]= globalvar.workstate;   
		SendBackCommunicationData(0x60,1);
	}	
}


#define IAP_START_ADDR						0x00000000UL
#define IAP_END_ADDR						0x00010000UL
__asm void ExceuteIAPlication(void)
{
		/* Load main stack pointer with application stack pointer initial value,
		   stored at first location of application area */
		ldr r0, =0x0000
		ldr r0, [r0]
		mov sp, r0

		/* Load program counter with application reset vector address, located at
		   second word of application area. */
		ldr r0, =0x0004
		ldr r0, [r0]
        BX  r0
}
/********************************************************
**�ṹ����:ReceiveAAH()
**�ṹ����:���պ�̨���͵ļ�����������������

**�� �� ��:���
**��������:2017-1-11 11:44:26

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void ReceiveAAH(void)
{	
	if((globalvar.workstate==StaOffDuty)||                       //�������°ࡣ
		(globalvar.workstate==StaIdle)||						 //���������С�
		(globalvar.workstate==StaFault))						 //���������ϡ�
	{
	    Fyd12864ClearLine(1);
	    Fyd12864DispPrintf(2, 1, "���������̼�����");
        Fyd12864DispPrintf(3, 1, "���Ժ�...");
        Fyd12864ClearLine(4);
	    SCB->VTOR  = IAP_START_ADDR;                             
        ExceuteIAPlication();
	}	
}

/********************************************************
**�ṹ����:ReceiveBBH()
**�ṹ����:��̨��ȡ����������汾��

**�� �� ��:���
**��������:2017-1-17 11:36:22

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
void ReceiveBBH(void)
{	
	if((globalvar.workstate==StaOffDuty)||                       //�������°ࡣ
		(globalvar.workstate==StaIdle)||						 //���������С�
		(globalvar.workstate==StaFault))						 //���������ϡ�
	{
	    char buf[16],i;
	    memset(SendBack485Buf,0,sizeof(SendBack485Buf));         //�����������㡣
        sprintf(buf,"%s",VERSION);
        for(i=0;i<16;i++)
        {
            SendBack485Buf[7+i]= buf[i];             
        }
		SendBackCommunicationData(0xBB,16);
	}	
}

#if 0
/********************************************************
**�ṹ����:RecBlckDataCheck(void)
**�ṹ����:���ڽ��պ�̨���ݼ�⣬����ֵ��Ч��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
bool RecBlckDataCheck(void)
{
    uint8_t i=0;
    uint16_t len=0;
    uint16_t CRC1=0x1122;
    uint16_t CRC2=0;
    if(UART3Buffer[0]!=0xAA)                     //���յ�һ���ַ����жϵ�һ���ֽ��Ƿ�Ϊ��ʼ��1��
    {
        UART3Count=0;
        ClearBuf(UART3Buffer,0,sizeof(UART3Buffer));
        return 0;
    }
    if((UART3Count==1)&&(UART3Buffer[1]!=0xFF))  //����֡ͷ��һ���ַ����жϵڶ����ֽ��Ƿ�Ϊ��ʼ��2��
    {
        return 0;
    }
    if((UART3Count==2)&&(UART3Buffer[1]!=0xFF))  //����֡ͷ��2���ַ����жϵڶ����ֽ��Ƿ�Ϊ��ʼ��2��
    {
        UART3Count=0;
        ClearBuf(UART3Buffer,0,sizeof(UART3Buffer));
        return 0;
    }
    if((UART3Buffer[UART3Count-2]==0xAA)&&(UART3Buffer[UART3Count-1]==0xFF)&&(UART3Count>2))//��֡ͷ���ж��Ƿ�����ʼ��1����ʼ��2��
    {
        UART3Count=2;
        ClearBuf(UART3Buffer,0,sizeof(UART3Buffer));
        UART3Buffer[0]=0xAA;
        UART3Buffer[1]=0xFF;
        return 0;
    }
    if((UART3Buffer[0]==0xAA)&&(UART3Buffer[1]==0xFF))//�ж�֡ͷ�Ƿ�Ϊ��ʼ����
    {
        if((UART3Buffer[UART3Count-2]==0xCC)&&(UART3Buffer[UART3Count-1]==0xFF))//�ж�֡β�Ƿ�Ϊ��ֹ����
        {
            CloseUartRcvIRQ(3);                                  //�ؽ����жϡ�
            memset(RecBack485Buf,0,sizeof(RecBack485Buf));
            for(i=0;i<UART3Count;i++)
            {
                RecBack485Buf[i]=UART3Buffer[i];                 //�����ջ�������ֵ�����顣
            }
            UART3Count=0;                                        //�����峤�����㡣
            ClearBuf(UART3Buffer,0,sizeof(UART3Buffer));
            
            len  = RecBack485Buf[5]*256+RecBack485Buf[6];        //��Ϣ�峤�ȡ� 
            CRC1 = CRC16Calculate(&RecBack485Buf[2], len+5);     //CRCУ�顣
            CRC2 = RecBack485Buf[len+7]*256+RecBack485Buf[len+8];//��̨�����Դ�CRC��
            if(CRC1 != CRC2)                                     //CRC�ȶԡ�
            {
                memset(RecBack485Buf,0,sizeof(RecBack485Buf));
                OpenUartRcvIRQ(PCPort);
                return 0;
            }
            
            MsDelay(1); 
            globalvar.comfailcount=0;                            //��̨������ͨ���Ƿ�������־
            globalvar.comreceiveflag=1;                             
            return 0;
        }
    }
    return 0;
}
#endif

/********************************************************
**�ṹ����:RecBlckDataCheck(void)
**�ṹ����:���ڽ��պ�̨���ݼ�⣬����ֵ��Ч��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :�ƺ���
**�޸�����:2016-11-3
**�޸�����:����д���жϺ���
********************************************************/ 
bool RecBlckDataCheck(void)
{
	uint16_t len=0;
    uint16_t CRC1=0x1122;
    uint16_t CRC2=0;
	len=UART3Buffer[5]*256+UART3Buffer[6]; 
	if(UART3Count>=(len+9))
	{
		CloseUartRcvIRQ(3);                                  		  //�ؽ����ж�
		CRC1 = CRC16Calculate((uint8_t *)&UART3Buffer[2], len+5);     //CRCУ�顣
		CRC2 = UART3Buffer[len+7]*256+UART3Buffer[len+8];			  //��̨�����Դ�CRC
		if(CRC1==CRC2)
		{
			memcpy(&RecBack485Buf[0],(uint8_t *)&UART3Buffer[0],len+9);
			if(RecBack485Buf[2]==sysparas.gunnum)
			{
				globalvar.comfailcount=0;                             //��̨������ͨ���Ƿ�������־
				globalvar.comreceiveflag=1;
			}
			else
			{
				UART3Count=0;     
				memset((uint8_t *)&UART3Buffer[0],0,sizeof(RecBack485Buf));	
				OpenUartRcvIRQ(3);
			}
		}
		else
		{
			UART3Count=0;     
			memset((uint8_t *)&UART3Buffer[0],0,sizeof(RecBack485Buf));	
			OpenUartRcvIRQ(3);
		}
	}
	if(UART3Count>=BUFSIZE)
	{
		UART3Count=0;     
		memset((uint8_t *)&UART3Buffer[0],0,sizeof(RecBack485Buf));	
		OpenUartRcvIRQ(3);
	}
	return 0;
}

/********************************************************
**�ṹ����:BGCommunication(uint8_t data)
**�ṹ����:�жϺ�̨�������ͣ�������ִ�в�ͬ���������

**�� �� ��:���
**��������:2016-05-17

**�޸���  :�ƺ���
**�޸�����:2016-11-3
**�޸�����:����д���жϺ���
********************************************************/ 
void BGCommunication(uint8_t data)
{
    switch(data)                             //�ж����
    {
        case 0x51:
                Receive51H();  
            break;
        case 0x52:
                Receive52H();
            break;
        case 0x53:
                Receive53H();
            break;
        case 0x54:
                Receive54H();
            break;
        case 0x55:
                Receive55H();
            break;
        case 0x56:
                Receive56H();
            break;
        case 0x57:
                Receive57H();
            break;
        case 0x58:
                Receive58H();
            break;
        case 0x59:                   
                Receive59H();
            break ;
        case 0x5A:
                Receive5AH();
            break;
        case 0x5B:                   
                Receive5BH();
            break ;
        case 0x5C:                   
                Receive5CH();
            break ;
        case 0x5D:                   
                Receive5DH();
            break ;
        case 0x5E:                   
                Receive5EH();
            break ;
        case 0x5F:
                Receive5FH();
            break ;
		case 0x60:
				Receive60H();
			break;
        case 0xAA:
				ReceiveAAH();
			break;
		case 0xBB:
				ReceiveBBH();
			break;
        default:
            break; 
    }
    UART3Count=0;                                        //�����峤�����㡣
    ClearBuf(UART3Buffer,0,sizeof(UART3Buffer));
    OpenUartRcvIRQ(PCPort);
}



    





