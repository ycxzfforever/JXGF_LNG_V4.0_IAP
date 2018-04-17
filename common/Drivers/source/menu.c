/***************************************************************************
**�ı���menu.c
**˵�������ڲ˵���ʾ�����ں����϶࣬�Ҵ�ͬС�죬
                        ����ֻ��ÿ������������
**������ �����
**�������ڣ�2016-4-15 09:08:28

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
#include "kernel.h"

fun         DispUI;         //����ˢ�´�����
fun         DispNextUI;     //������Ϣ��ʾ�����һ������

//���ò���ʱ��ʾ numpoint:С�������λ��,0,2,4��Ч������5ʱΪѡ������
void DispSet(uint8_t numpoint,uint32_t data,char*buf)
{
    char buff[16];
    NumKeyHandler();   
    globalvar.pointnum=0;
    globalvar.beforedex=data;
    if(numpoint<5)//��С��������(С�������Ϊ0,2,4)
    {
        Fyd12864DispPrintf(1,1,buf);
        if(numpoint==0)
        {
			if(ScreenInput.array[ScreenInput.inputlen-1] == 0x2E)
			{
				ScreenInput.array[ScreenInput.inputlen-1]=0x20;
				ScreenInput.inputlen--;
			}
            globalvar.pointnum=numpoint;
            globalvar.afterdex=atol((char *)ScreenInput.array);
            Fyd12864DispPrintf(2,1,"��ֵ:%ld",data);  
        }            
        else if(numpoint==2)
        {
            globalvar.pointnum=numpoint;
            sprintf(buff,"%f",atof((char *)ScreenInput.array)*100);
            globalvar.afterdex=atol(buff);
            Fyd12864DispPrintf(2,1,"��ֵ:%d.%02d",data/100,data%100);
        }
        else if(numpoint==4)
        {
            globalvar.pointnum=numpoint;
            sprintf(buff,"%f",atof((char *)ScreenInput.array)*10000);
            globalvar.afterdex=atol(buff);
            Fyd12864DispPrintf(2,1,"��ֵ:%d.%04d",data/10000,data%10000);
        }
        Fyd12864DispPrintf(3,1,"��ֵ:%s",ScreenInput.array);                
        Fyd12864ClearLine(4); 
    }
    else  if(numpoint==5)   //ѡ������
    {
		if(ScreenInput.array[ScreenInput.inputlen-1] == 0x2E)
		{
			ScreenInput.array[ScreenInput.inputlen-1]=0x20;
			ScreenInput.inputlen--;
		}
        globalvar.afterdex=atol((char *)ScreenInput.array);
        Fyd12864DispPrintf(1,1,"%s %d",buf,data);
        Fyd12864DispPrintf(4,1,"��ֵ:%s",ScreenInput.array);
    } 
    else  if(numpoint==6)   //��������
    {
        globalvar.afterdex=atol((char *)ScreenInput.array);
        Fyd12864DispPrintf(1,1,"%s",buf);
        Fyd12864DispPrintf(2,1,"������:%s",ScreenInput.array);
        Fyd12864ClearLine(3); 
        Fyd12864ClearLine(4); 
    }
    else  if(numpoint==7)   //������ʾ
    {
        globalvar.afterdex=atol((char *)ScreenInput.array);
        Fyd12864DispPrintf(1,1,buf);
        Fyd12864DispPrintf(2,1,"��ֵ:%ld",data);    
        Fyd12864DispPrintf(3,1,"��ֵ:%s",ScreenInput.array);                
        Fyd12864ClearLine(4);
    }
}

//���óɹ���ʾ
void SetSuccess(void)
{
    Fyd12864DispPrintf(4,1,"���óɹ�");
}

//����ʧ����ʾ
void SetFault(void)
{
    Fyd12864DispPrintf(4,1,"����ʧ��");
}

//ȷ�ϴ���
void DispOK(void)
{      
    uint32_t   addr,id;
	if(ScreenInput.inputlen>0)
	{
		if(globalvar.displayonce==1)
		{
			if(globalvar.paraindex!=34)  tmp_sysparas.modnum++;
			UpLoad5BData.modifyid=tmp_sysparas.modnum;  //�޸���ˮ�š�
			UpLoad5BData.classnum=tmp_sysparas.shiftnum;//��š�
			memcpy(&UpLoad5BData.cardstaffnum[0],&m1card.cardnum[0],8);//���š�
			UpLoad5BData.paraindex=globalvar.paraindex;      //���������š�
			UpLoad5BData.dexnum=globalvar.pointnum;          //С��λ��
			UpLoad5BData.beforeparameter=globalvar.beforedex;//�޸�ǰ��ֵ��
			UpLoad5BData.afterparameter=globalvar.afterdex;  //�޸ĺ��ֵ��
			UpLoad5BData.modifytime[0]=0x20;
			memcpy(&UpLoad5BData.modifytime[1],&time.year,6);//�޸�ʱ�䡣  
			sysparas=tmp_sysparas;        
			if(SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas)))
			{
				id=sysparas.modnum%FlashModMaxID;
				if(id != 0)
				{
					addr=FlashModSADDR+FlashModLen*(id-1);
					SaveFlash(0,addr, (uint8_t *)&UpLoad5BData.modifyid, sizeof(Reply5B));
					SaveFlash(1,addr, (uint8_t *)&UpLoad5BData.modifyid, sizeof(Reply5B));
				}
				SetSuccess();   
			}
			else
			{
				SetFault();   
			}
			globalvar.displayonce=0;
		} 
		if(DispNextUI!=NULL)
		{
			TaskMessage(10, DispNextUI);
		}
	}
	else
	{
		SetFault();
		TaskMessage(10, DispNextUI);
	}
}


//��������ʵʱ���°�����Ľ��棬
//funname1:��������Ҫִ�еĺ���,funname2:������Ϣ��ʾ�����һ������
void TaskAdd(uint32_t keyvalue,fun funname1,fun funname2)
{    
    if(globalvar.KeyValue==keyvalue)
    {
		globalvar.KeyValue=0;
        DispUI=funname1;
        if(funname2!=NULL)  DispNextUI=funname2;        
        else    DispNextUI=NULL;
    }
  
}

//��Ϣ��ʾ��ms_100:Ϊ��ʾʱ�䣬100msΪ��λ��funname:ʱ�䵽�˹�����Ҫִ�еĺ���
void TaskMessage(uint32_t ms_100,fun funname)
{    
    if((globalvar.MsgTime==0)&&(globalvar.MsgFlag==0))//��һ�����msg����
    {
        globalvar.MsgFlag=1;
        globalvar.MsgTime=ms_100;
    }
    if((globalvar.MsgTime==0)&&(globalvar.MsgFlag==1))//ʱ�䵽�˹�����msg����
    {
        globalvar.MsgFlag=0;
        DispUI=funname;
    }
}

//��ʼ����������buf
void InputInit(void)
{
    memset(&ScreenInput,0,sizeof(Screeninput));
    globalvar.displayonce=1;
}

//������λ����
void DispSetUnit(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==1)||(tmp==0))
    {
        tmp_sysparas.unit=tmp;
        globalvar.paraindex=28;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
   
    DispSet(5,sysparas.unit,"������λ����");
    Fyd12864DispPrintf(2,1,"0.����");
    Fyd12864DispPrintf(3,1,"1.�귽");
    TaskAdd(KEY_RET, DispUnitQcReset,NULL);
    TaskAdd(KEY_OK, DispOK,DispUnitQcReset);
}


//Ĭ�ϵ�������
void DispSetDefPrice(void)
{   
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=5;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.price[0]=atol(buf);
    globalvar.paraindex=7;
	if(tmp_sysparas.price[0] > 10000)
	{
		Fyd12864DispPrintf(4,1,"���۳���");
		NumKeyHandler(); 
        return;
	} 
    DispSet(2,sysparas.price[0],"Ĭ�ϵ�������");   
    TaskAdd(KEY_RET, DispSetPrice,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetPrice);
}

//�Żݵ���1����
void DispSetPrice1(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=5;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.price[1]=atol(buf);
    globalvar.paraindex=8;
	if(tmp_sysparas.price[1] > 10000)
	{
		Fyd12864DispPrintf(4,1,"���۳���");
		NumKeyHandler(); 
        return;
	}
    DispSet(2,sysparas.price[1],"����1");   
    TaskAdd(KEY_RET, DispSetOthPrice1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetOthPrice1);
}

//�Żݵ���2����
void DispSetPrice2(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=5;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.price[2]=atol(buf);
    globalvar.paraindex=9;
	if(tmp_sysparas.price[2] > 10000)
	{
		Fyd12864DispPrintf(4,1,"���۳���");
		NumKeyHandler(); 
        return;
	}
    DispSet(2,sysparas.price[2],"����2");   
    TaskAdd(KEY_RET, DispSetOthPrice1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetOthPrice1);
}

//�Żݵ���3����
void DispSetPrice3(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=5;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.price[3]=atol(buf);
    globalvar.paraindex=10;
	if(tmp_sysparas.price[3] > 10000)
	{
		Fyd12864DispPrintf(4,1,"���۳���");
		NumKeyHandler(); 
        return;
	}
    DispSet(2,sysparas.price[3],"����3");   
    TaskAdd(KEY_RET, DispSetOthPrice1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetOthPrice1);
}

//�Żݵ���4����
void DispSetPrice4(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=5;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.price[4]=atol(buf);
	if(tmp_sysparas.price[4] > 10000)
	{
		Fyd12864DispPrintf(4,1,"���۳���");
		NumKeyHandler(); 
        return;
	}
    DispSet(2,sysparas.price[4],"����4"); 
    globalvar.paraindex=11;
    TaskAdd(KEY_RET, DispSetOthPrice1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetOthPrice1);
}

//�Żݵ�������1
void DispSetOthPrice1(void)
{   
    InputInit();
    Fyd12864DispPrintf(1,1,"1.����1 %d.%02d",sysparas.price[1]/100,sysparas.price[1]%100);
    Fyd12864DispPrintf(2,1,"2.����2 %d.%02d",sysparas.price[2]/100,sysparas.price[2]%100);
    Fyd12864DispPrintf(3,1,"3.����3 %d.%02d",sysparas.price[3]/100,sysparas.price[3]%100);
    Fyd12864DispPrintf(4,1,"4.����4 %d.%02d",sysparas.price[4]/100,sysparas.price[4]%100);

    TaskAdd(KEY_1, DispSetPrice1,NULL);
    TaskAdd(KEY_2, DispSetPrice2,NULL);
    TaskAdd(KEY_3, DispSetPrice3,NULL);
    TaskAdd(KEY_4, DispSetPrice4,NULL);  
    TaskAdd(KEY_RET, DispSetPrice,NULL);
}

//�ʼ�ģʽ����
void DispSetQCMode(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==0)||(tmp==1)||(tmp==2)||(tmp==3))
    {
        tmp_sysparas.qcmode=tmp;
        globalvar.paraindex=29;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.qcmode,"�ʼ�ģʽ����");
    Fyd12864DispPrintf(2,1,"0.����  1.�ֳֻ�");
    Fyd12864DispPrintf(3,1,"2.IC��  3.���ܰ�");
    TaskAdd(KEY_RET, DispUnitQcReset,NULL);
    TaskAdd(KEY_OK, DispOK,DispUnitQcReset);
}


//��������
void DispSetPrice(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.Ĭ�ϵ�������");
    Fyd12864DispPrintf(2,1,"2.�Żݵ�������");
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);    
    TaskAdd(KEY_1, DispSetDefPrice,NULL);
    TaskAdd(KEY_2, DispSetOthPrice1,NULL);
    TaskAdd(KEY_RET, DispMenu_1,NULL);
}

//ǹ������
void DispSetGunNum(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=2;
    tmp_sysparas.gunnum=atol((char *)ScreenInput.array);  
	if(tmp_sysparas.gunnum > 0)
	{
		TaskAdd(KEY_OK, DispOK,DispSysSet1);
	}
    DispSet(0,sysparas.gunnum,"ǹ������");
    globalvar.paraindex=2;
    TaskAdd(KEY_RET, DispSysSet1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSysSet1);
}

//����������
void DispSetTotvolume(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.totvolume=atol(buf);   
    globalvar.paraindex=5;
    DispSet(2,sysparas.totvolume,"����������");
    TaskAdd(KEY_RET, DispSysSet1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSysSet1);
}

//�ܽ������
void DispSetTotmoney(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.totmoney=atol(buf);  
    globalvar.paraindex=6;
    DispSet(2,sysparas.totmoney,"�ܽ������");
    TaskAdd(KEY_RET, DispSysSet1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSysSet1);
}

//��������������
void DispSetModbusType(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==1)||(tmp==2))
    {
        tmp_sysparas.modbustype=tmp;
        globalvar.paraindex=27;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.modbustype,"��������������");
    Fyd12864DispPrintf(2,1,"1.��˹����");
    Fyd12864DispPrintf(3,1,"2.E+H");
    TaskAdd(KEY_RET, DispSysSet1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSysSet1);
}

//ϵͳ��������1
void DispSysSet1(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.ǹ��");    
    Fyd12864DispPrintf(2,1,"2.������");
    Fyd12864DispPrintf(3,1,"3.�ܽ��");
    Fyd12864DispPrintf(4,1,"4.����������");
    TaskAdd(KEY_1,DispSetGunNum,NULL);
    TaskAdd(KEY_2,DispSetTotvolume,NULL);
    TaskAdd(KEY_3,DispSetTotmoney,NULL);    
    TaskAdd(KEY_4,DispSetModbusType,NULL); 
    //TaskAdd(KEY_UP, DispSysSet2,NULL);
    TaskAdd(KEY_DOWN, DispSysSet2,NULL);
    TaskAdd(KEY_RET, DispMenu_1,NULL);
}

//��ˮ������
void DispSetTransnum(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    ScreenInput.pointnum=0;
    tmp_sysparas.transnum=atol((char *)ScreenInput.array);   
    globalvar.paraindex=3;
    DispSet(0,sysparas.transnum,"��ˮ������");
    TaskAdd(KEY_RET, DispSysSet2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSysSet2);
}

//�Ѵ���ˮ������
void DispSetUpTransnum(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    ScreenInput.pointnum=0;
    tmp_sysparas.uptransnum=atol((char *)ScreenInput.array);   
    globalvar.paraindex=4;
    DispSet(0,sysparas.uptransnum,"�Ѵ���ˮ������");
    TaskAdd(KEY_RET, DispSysSet2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSysSet2);
}

//�޸ļ�¼��ˮ������
void DispSetModnum(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    ScreenInput.pointnum=0;
    tmp_sysparas.modnum=atol((char *)ScreenInput.array);   
    globalvar.paraindex=34;
    DispSet(0,sysparas.modnum,"�޸���ˮ������");
    TaskAdd(KEY_RET, DispSysSet2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSysSet2);
}

//�Ѵ������޸���ˮ������
void DispSetUpModnum(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    ScreenInput.pointnum=0;
    tmp_sysparas.upmodnum=atol((char *)ScreenInput.array);   
    globalvar.paraindex=35;
    DispSet(0,sysparas.upmodnum,"�Ѵ��޸ĺ�����");
    TaskAdd(KEY_RET, DispSysSet2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSysSet2);
}


//ϵͳ��������2
void DispSysSet2(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.��ˮ��");
    Fyd12864DispPrintf(2,1,"2.�Ѵ���ˮ��");
    Fyd12864DispPrintf(3,1,"3.�޸ļ�¼��ˮ��");
    Fyd12864DispPrintf(4,1,"4.�Ѵ��޸���ˮ��");

    TaskAdd(KEY_1,DispSetTransnum,NULL);
    TaskAdd(KEY_2,DispSetUpTransnum,NULL);
    TaskAdd(KEY_3,DispSetModnum,NULL);    
    TaskAdd(KEY_4,DispSetUpModnum,NULL);
    TaskAdd(KEY_UP, DispSysSet1,NULL);
    //TaskAdd(KEY_DOWN, DispSysSet1,NULL);
    TaskAdd(KEY_RET, DispMenu_1,NULL);
}

//���ð��
void DispSetShiftNum(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.shiftnum=atol((char *)ScreenInput.array); 
	if((tmp_sysparas.shiftnum>0)&&(tmp_sysparas.shiftnum<=tmp_sysparas.shiftmaxnum))
	{
		TaskAdd(KEY_OK, DispOK,DispSetShiftInfo1);;
	}
	else if(tmp_sysparas.shiftnum > tmp_sysparas.shiftmaxnum)
	{
		Fyd12864DispPrintf(4,1,"��ų���");
		NumKeyHandler(); 
		return;
	}
    globalvar.paraindex=36;
    DispSet(0,sysparas.shiftnum,"�������");
    TaskAdd(KEY_RET,DispSetShiftInfo1 ,NULL);
}

//���������
void DispSetShiftMaxNum(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=2;
    tmp_sysparas.shiftmaxnum=atol((char *)ScreenInput.array);  
	if(tmp_sysparas.shiftmaxnum>0)
	{
		TaskAdd(KEY_OK, DispOK,DispSetShiftInfo1);
	}
    globalvar.paraindex=37;
    DispSet(0,sysparas.shiftmaxnum,"���������");
    TaskAdd(KEY_RET,DispSetShiftInfo1 ,NULL);
}

//���ð���ˮ��
void DispSetShiftTransNum(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    tmp_sysparas.shiftransnum=atol((char *)ScreenInput.array); 
    globalvar.paraindex=38;
    DispSet(0,sysparas.shiftransnum,"����ˮ������");
    TaskAdd(KEY_RET,DispSetShiftInfo1 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetShiftInfo1);
}

//�����Ѵ�����ˮ��
void DispSetShiftUpTransNum(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    tmp_sysparas.shiftuptransnum=atol((char *)ScreenInput.array);  
    globalvar.paraindex=39;
    DispSet(0,sysparas.shiftuptransnum,"�Ѵ�����ˮ������");
    TaskAdd(KEY_RET,DispSetShiftInfo1 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetShiftInfo1);
}


//������Ϣ����1
void DispSetShiftInfo1(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.���");
    Fyd12864DispPrintf(2,1,"2.�����");
    Fyd12864DispPrintf(3,1,"3.����ˮ��");
    Fyd12864DispPrintf(4,1,"4.�Ѵ�����ˮ��");
    TaskAdd(KEY_1,DispSetShiftNum,NULL);
    TaskAdd(KEY_2,DispSetShiftMaxNum,NULL);
    TaskAdd(KEY_3,DispSetShiftTransNum,NULL);    
    TaskAdd(KEY_4,DispSetShiftUpTransNum,NULL); 
    //TaskAdd(KEY_UP, DispSetShiftInfo3,NULL);
    TaskAdd(KEY_DOWN, DispSetShiftInfo2,NULL);
    TaskAdd(KEY_RET, DispMenu_1,NULL);
}

//�࿪ʼǹ������
void DispSetShiftStartvol(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.shiftstartvol=atol(buf);  
    globalvar.paraindex=41;
    DispSet(2,sysparas.shiftstartvol,"�࿪ʼǹ������");
    TaskAdd(KEY_RET, DispSetShiftInfo2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetShiftInfo2);
}

//�࿪ʼ�������
void DispSetShiftStartmon(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.shiftstartmon=atol(buf);   
    globalvar.paraindex=42;
    DispSet(2,sysparas.shiftstartmon,"�࿪ʼ�������");
    TaskAdd(KEY_RET, DispSetShiftInfo2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetShiftInfo2);
}

//���ۼ���������
void DispSetShiftTotvol(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.shiftotvol=atol(buf);  
    globalvar.paraindex=43;
    DispSet(2,sysparas.shiftotvol,"���ۼ���������");
    TaskAdd(KEY_RET, DispSetShiftInfo2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetShiftInfo2);
}


//���ۼƽ������
void DispSetShiftTotmon(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.shiftotmon=atol(buf); 
    globalvar.paraindex=44;
    DispSet(2,sysparas.shiftotmon,"���ۼƽ������");
    TaskAdd(KEY_RET, DispSetShiftInfo2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetShiftInfo2);
}

//������Ϣ����2
void DispSetShiftInfo2(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.�࿪ʼǹ��");
    Fyd12864DispPrintf(2,1,"2.�࿪ʼ���");
    Fyd12864DispPrintf(3,1,"3.���ۼ�����");
    Fyd12864DispPrintf(4,1,"4.���ۼƽ��");
    TaskAdd(KEY_1,DispSetShiftStartvol,NULL);
    TaskAdd(KEY_2,DispSetShiftStartmon,NULL);
    TaskAdd(KEY_3,DispSetShiftTotvol,NULL);    
    TaskAdd(KEY_4,DispSetShiftTotmon,NULL); 
    //TaskAdd(KEY_UP, DispSetShiftInfo1,NULL);
    TaskAdd(KEY_DOWN, DispSetShiftInfo3,NULL);
    TaskAdd(KEY_RET, DispMenu_1,NULL);
}

//���ð��������
void DispSetShiftFuelTimes(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    tmp_sysparas.shiftfueltimes=atol((char *)ScreenInput.array);  
    globalvar.paraindex=40;
    DispSet(0,sysparas.shiftfueltimes,"�������������");
    TaskAdd(KEY_RET,DispSetShiftInfo3 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetShiftInfo3);
}

//���ð࿪ʼ����
void DispSetShiftStartDate(void)
{
    uint8_t *tmp,i,ret=TRUE;
	if(globalvar.KeyValue != KEY_PT)
	{
		tmp_sysparas=sysparas;
		NumKeyHandler();
		ScreenInput.inputmaxlen=6;
		tmp=(uint8_t*)&tmp_sysparas.shiftstarttime;
	  
		for(i=0;i<(ScreenInput.inputlen/2+1);i++)
		{
			*(tmp+i)=AscToBcd(&ScreenInput.array[2*i]);
		}
		
		Fyd12864DispPrintf(1,1,"�࿪ʼ��������");  
		Fyd12864DispPrintf(2,1,"��ֵ: %02X��%02X��%02X��",
								sysparas.shiftstarttime[0],
								sysparas.shiftstarttime[1],
								sysparas.shiftstarttime[2]);
								
		Fyd12864DispPrintf(3,1,"��ֵ: %02X��%02X��%02X��",
								tmp_sysparas.shiftstarttime[0],
								tmp_sysparas.shiftstarttime[1],
								tmp_sysparas.shiftstarttime[2]);   
		if(ScreenInput.inputlen>5) ret=CheckDate(tmp);
		if(ret==FALSE)
		{
			Fyd12864DispPrintf(4,1,"�����ʽ����");
			return;
		}
		else
		{
			Fyd12864ClearLine(4);               
		}
		TaskAdd(KEY_RET,DispSetShiftInfo3 ,NULL);
		if(ScreenInput.inputlen==6)//������Ϻ���ܰ�ȷ�ϼ�
		{
			globalvar.paraindex=45;
			TaskAdd(KEY_OK,DispOK,DispSetShiftInfo3);
		}
	}
}

//���ð࿪ʼʱ��
void DispSetShiftStartTime(void)
{
    uint8_t *tmp,i,ret=TRUE;
	if(globalvar.KeyValue != KEY_PT)
	{
		tmp_sysparas=sysparas;
		NumKeyHandler();
		ScreenInput.inputmaxlen=6;
		tmp=(uint8_t*)&tmp_sysparas.shiftstarttime[3];
	  
		for(i=0;i<(ScreenInput.inputlen/2+1);i++)
		{
			*(tmp+i)=AscToBcd(&ScreenInput.array[2*i]);
		}
		
		Fyd12864DispPrintf(1,1,"�࿪ʼʱ������");  
		Fyd12864DispPrintf(2,1,"��ֵ: %02X:%02X:%02X",
								sysparas.shiftstarttime[3],
								sysparas.shiftstarttime[4],
								sysparas.shiftstarttime[5]);
								
		Fyd12864DispPrintf(3,1,"��ֵ: %02X:%02X:%02X",
								tmp_sysparas.shiftstarttime[3],
								tmp_sysparas.shiftstarttime[4],
								tmp_sysparas.shiftstarttime[5]);   
		if(ScreenInput.inputlen>5) ret=CheckTime(tmp);
		if(ret==FALSE)
		{
			Fyd12864DispPrintf(4,1,"�����ʽ����");
			return;
		}
		else
		{
			Fyd12864ClearLine(4);               
		}
		TaskAdd(KEY_RET,DispSetShiftInfo3 ,NULL);
		if(ScreenInput.inputlen==6)//������Ϻ���ܰ�ȷ�ϼ�
		{
			globalvar.paraindex=45;
			TaskAdd(KEY_OK,DispOK,DispSetShiftInfo3);
		}
	}
}


//������Ϣ����3
void DispSetShiftInfo3(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.���������");
    Fyd12864DispPrintf(2,1,"2.�࿪ʼ����");
    Fyd12864DispPrintf(3,1,"3.�࿪ʼʱ��");
    Fyd12864ClearLine(4);
    TaskAdd(KEY_1,DispSetShiftFuelTimes,NULL);
    TaskAdd(KEY_2,DispSetShiftStartDate,NULL);
    TaskAdd(KEY_3,DispSetShiftStartTime,NULL); 
    TaskAdd(KEY_UP, DispSetShiftInfo2,NULL);
    //TaskAdd(KEY_DOWN, DispSetShiftInfo1,NULL);
    TaskAdd(KEY_RET, DispMenu_1,NULL);
}

//������������
void DispSetCardMaxYuE()
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.cardmaxyue=atol(buf);  
    if(tmp_sysparas.cardmaxyue<=100000000)
	{
		TaskAdd(KEY_OK, DispOK,DispSetICInfo1);
	}
	else
	{
		Fyd12864DispPrintf(4,1,"�����");
        NumKeyHandler(); 
        return;
	}
    globalvar.paraindex=51;
    DispSet(2,sysparas.cardmaxyue,"������������");
    TaskAdd(KEY_RET, DispSetICInfo1,NULL);
    
}

//������������
void DispSetCardMinYuE()
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=16;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.cardminyue=atol(buf); 
    globalvar.paraindex=52;
    DispSet(2,sysparas.cardminyue,"������������");
    TaskAdd(KEY_RET, DispSetICInfo1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetICInfo1);
}

//PSAM����ѡ��
void DispSetPsamNum(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==1)||(tmp==2)||(tmp==3)||(tmp==4))
    {
        tmp_sysparas.cardpsamnum=tmp;
        globalvar.paraindex=54;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(0,sysparas.cardpsamnum,"PSAM����ѡ�� 1-4");
    TaskAdd(KEY_RET, DispSetICInfo1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetICInfo1);
}

//����IC��δʹ�������ʱ��
void DispSetCardMaxDay(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.cardmaxday=atol((char *)ScreenInput.array); 
    globalvar.paraindex=55;
    DispSet(0,sysparas.cardmaxday,"���������");
    TaskAdd(KEY_RET,DispSetICInfo2 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetICInfo2);
}

//�Ƿ��ÿ�����
void DispSetUseCard()
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==0)||(tmp==1))
    {
        tmp_sysparas.usecard=tmp;
        globalvar.paraindex=57;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.usecard,"�Ƿ�ֻ���ÿ�");
    Fyd12864DispPrintf(2,1,"0.�ɲ�ʹ�ÿ�");
    Fyd12864DispPrintf(3,1,"1.ֻ��ʹ�ÿ�");
    TaskAdd(KEY_RET, DispSetICInfo1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetICInfo1);
}

//IC����Ϣ����1
void DispSetICInfo1(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.��������");
    Fyd12864DispPrintf(2,1,"2.��������");
    Fyd12864DispPrintf(3,1,"3.�Ƿ��ÿ�");
    Fyd12864DispPrintf(4,1,"4.PSAM����ѡ��");
    TaskAdd(KEY_1,DispSetCardMaxYuE,NULL);
    TaskAdd(KEY_2,DispSetCardMinYuE,NULL);
    TaskAdd(KEY_3,DispSetUseCard,NULL); //ycx20161202
    TaskAdd(KEY_4,DispSetPsamNum,NULL); 
    TaskAdd(KEY_DOWN, DispSetICInfo2,NULL);
    TaskAdd(KEY_RET, DispMenu_1,NULL);
}

//�Ƿ�����ƿ����
void DispSetCardCheck(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==0)||(tmp==1))
    {
        tmp_sysparas.cardcheck=tmp;
        globalvar.paraindex=56;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.cardcheck,"�Ƿ�����ƿ");
    Fyd12864DispPrintf(2,1,"0.������");
    Fyd12864DispPrintf(3,1,"1.����");
    TaskAdd(KEY_RET, DispSetICInfo2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetICInfo2);
}


//IC����Ϣ����2
void DispSetICInfo2(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.�Ƿ�����ƿ");
    Fyd12864DispPrintf(2,1,"2.���������");
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    TaskAdd(KEY_1,DispSetCardCheck,NULL);
    TaskAdd(KEY_2,DispSetCardMaxDay,NULL);
    TaskAdd(KEY_UP, DispSetICInfo1,NULL);
    TaskAdd(KEY_RET, DispMenu_1,NULL);
}


//�˵�����1��ʾ
void DispMenu_1(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.��������");
    Fyd12864DispPrintf(2,1,"2.ϵͳ��������");
    Fyd12864DispPrintf(3,1,"3.������Ϣ����");
    Fyd12864DispPrintf(4,1,"4.IC����������");
    TaskAdd(KEY_1, DispSetPrice,NULL);
    TaskAdd(KEY_2, DispSysSet1,NULL);
    TaskAdd(KEY_3, DispSetShiftInfo1,NULL);
    TaskAdd(KEY_4, DispSetICInfo1,NULL);
    //TaskAdd(KEY_UP, DispMenu_3,NULL);
    TaskAdd(KEY_DOWN, DispMenu_2,NULL);
    TaskAdd(KEY_RET, DispFirst,NULL);
}

//ʡ��������
void DispSetShengCode(void)
{
    uint32_t    tmp;
	if(globalvar.KeyValue != KEY_PT)
	{
		tmp_sysparas=sysparas;
		NumKeyHandler();
		ScreenInput.inputmaxlen=4;
		tmp=atol((char *)ScreenInput.array);   
        HexToBcdbuf(tmp, (char *)tmp_sysparas.shengcode, 2);
		globalvar.paraindex=47;
		Fyd12864DispPrintf(1,1,"ʡ��������");  
		Fyd12864DispPrintf(2,1,"��ֵ: %02X%02X",
								sysparas.shengcode[0],
								sysparas.shengcode[1]);
		globalvar.beforedex=BcdbufToHex(sysparas.shengcode, 2);						
		Fyd12864DispPrintf(3,1,"��ֵ: %02X%02X",
								tmp_sysparas.shengcode[0],
								tmp_sysparas.shengcode[1]);
	    globalvar.afterdex=BcdbufToHex(tmp_sysparas.shengcode, 2);
        globalvar.pointnum=0;
		Fyd12864ClearLine(4);               
		TaskAdd(KEY_RET,DispGeneralInfo ,NULL);
		TaskAdd(KEY_OK,DispOK,DispGeneralInfo);
	}
}


//վ��������
void DispSetStationCode(void)
{
    uint32_t    tmp;
	if(globalvar.KeyValue != KEY_PT)
	{
		tmp_sysparas=sysparas;
		NumKeyHandler();
		ScreenInput.inputmaxlen=8;
        tmp=atol((char *)ScreenInput.array);   
        HexToBcdbuf(tmp, (char *)tmp_sysparas.stationcode, 4);
		globalvar.paraindex=48;
		Fyd12864DispPrintf(1,1,"վ��������");  
		Fyd12864DispPrintf(2,1,"��ֵ: %02X%02X%02X%02X",
								sysparas.stationcode[0],
								sysparas.stationcode[1],
								sysparas.stationcode[2],
								sysparas.stationcode[3]);
        globalvar.beforedex=BcdbufToHex(sysparas.stationcode, 4);								
		Fyd12864DispPrintf(3,1,"��ֵ: %02X%02X%02X%02X",
								tmp_sysparas.stationcode[0],
								tmp_sysparas.stationcode[1],
								tmp_sysparas.stationcode[2],
								tmp_sysparas.stationcode[3]);
        globalvar.afterdex=BcdbufToHex(tmp_sysparas.stationcode, 4);
        globalvar.pointnum=0;
	  
		Fyd12864ClearLine(4);               
		TaskAdd(KEY_RET,DispGeneralInfo ,NULL);
		TaskAdd(KEY_OK,DispOK,DispGeneralInfo);
	}
}

//���б�������
void DispSetCityCode(void)
{
    uint32_t    tmp;
	if(globalvar.KeyValue != KEY_PT)
	{
		tmp_sysparas=sysparas;
		NumKeyHandler();
		ScreenInput.inputmaxlen=4;
		tmp=atol((char *)ScreenInput.array);   
        HexToBcdbuf(tmp, (char *)tmp_sysparas.citycode, 2);
		globalvar.paraindex=49;
		Fyd12864DispPrintf(1,1,"���б�������");  
		Fyd12864DispPrintf(2,1,"��ֵ: %02X%02X",
								sysparas.citycode[0],
								sysparas.citycode[1]);
        globalvar.beforedex=BcdbufToHex(sysparas.citycode, 2);								
		Fyd12864DispPrintf(3,1,"��ֵ: %02X%02X",
								tmp_sysparas.citycode[0],
								tmp_sysparas.citycode[1]);
        globalvar.afterdex=BcdbufToHex(tmp_sysparas.citycode, 2);
        globalvar.pointnum=0;	  
		Fyd12864ClearLine(4);               
		TaskAdd(KEY_RET,DispGeneralInfo ,NULL);
		TaskAdd(KEY_OK,DispOK,DispGeneralInfo);
	}
}

//�ϼ���λ��������
void DispSetParentCode(void)
{
    uint32_t    tmp;
	if(globalvar.KeyValue != KEY_PT)
	{
		tmp_sysparas=sysparas;
		NumKeyHandler();
		ScreenInput.inputmaxlen=8;
		tmp=atol((char *)ScreenInput.array);   
        HexToBcdbuf(tmp, (char *)tmp_sysparas.parentcode, 4);
		globalvar.paraindex=50;		
		Fyd12864DispPrintf(1,1,"�ϼ���λ��������");  
		Fyd12864DispPrintf(2,1,"��ֵ: %02X%02X%02X%02X",
								sysparas.parentcode[0],
								sysparas.parentcode[1],
								sysparas.parentcode[2],
								sysparas.parentcode[3]);
        globalvar.beforedex=BcdbufToHex(sysparas.parentcode, 4);									
		Fyd12864DispPrintf(3,1,"��ֵ: %02X%02X%02X%02X",
								tmp_sysparas.parentcode[0],
								tmp_sysparas.parentcode[1],
								tmp_sysparas.parentcode[2],
								tmp_sysparas.parentcode[3]);
        globalvar.afterdex=BcdbufToHex(tmp_sysparas.parentcode, 4);
        globalvar.pointnum=0;		  
		Fyd12864ClearLine(4);               
		TaskAdd(KEY_RET,DispGeneralInfo ,NULL);
		TaskAdd(KEY_OK,DispOK,DispGeneralInfo);
	}
}


//ͨ����Ϣ����
void DispGeneralInfo(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.ʡ����");
    Fyd12864DispPrintf(2,1,"2.վ����");
    Fyd12864DispPrintf(3,1,"3.���д���");
    Fyd12864DispPrintf(4,1,"4.�ϼ���λ����");
    TaskAdd(KEY_1,DispSetShengCode ,NULL);
    TaskAdd(KEY_2,DispSetStationCode ,NULL);
    TaskAdd(KEY_3,DispSetCityCode ,NULL);
    TaskAdd(KEY_4,DispSetParentCode ,NULL);
    TaskAdd(KEY_RET, DispMenu_2,NULL);
}

void SystemReStart(void)
{
	Fyd12864ClearLine(1);
	Fyd12864ClearLine(3);
	Fyd12864ClearLine(4);
	Fyd12864DispPrintf(2,1,"ϵͳ������");
	MsDelay(500);
//	Fyd12864DispPrintf(2,1,"ϵͳ������.");
//	MsDelay(500);
//	Fyd12864DispPrintf(2,1,"ϵͳ������..");
//	MsDelay(500);
//	Fyd12864DispPrintf(2,1,"ϵͳ������...");
//	MsDelay(500);

//	LPC_WDT->TC = 0xFF;
//	LPC_WDT->MOD |= 1;
//	LPC_WDT->MOD |= 2;
//	LPC_WDT->FEED = 0xAA;
//    LPC_WDT->FEED = 0x55;
	while(1);
}

//�����Ʋ���������
void DispSetModbusBaud(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==1)||(tmp==2)||(tmp==3)||(tmp==4))
    {
        switch(tmp)
        {
            case 1: tmp_sysparas.modbusbaud=9600;   break;
            case 2: tmp_sysparas.modbusbaud=19200;  break;
            case 3: tmp_sysparas.modbusbaud=38400;  break;
            case 4: tmp_sysparas.modbusbaud=115200; break;
        }
        globalvar.paraindex=58;        
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.modbusbaud,"�����Ʋ�����");
    Fyd12864DispPrintf(2,1,"1.9600  2.19200");
    Fyd12864DispPrintf(3,1,"3.38400 4.115200");
    TaskAdd(KEY_RET,DispCommPara ,NULL);
	if(tmp != 0)
	{
		TaskAdd(KEY_OK, DispOK,SystemReStart);
	}
}



//��̨����������
void DispSetPCBaud(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==1)||(tmp==2)||(tmp==3)||(tmp==4))
    {
        switch(tmp)
        {
            case 1: tmp_sysparas.pcbaud=9600;   break;
            case 2: tmp_sysparas.pcbaud=19200;  break;
            case 3: tmp_sysparas.pcbaud=38400;  break;
            case 4: tmp_sysparas.pcbaud=115200; break;
        }
        globalvar.paraindex=59;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.pcbaud,"��̨������");
    Fyd12864DispPrintf(2,1,"1.9600  2.19200");
    Fyd12864DispPrintf(3,1,"3.38400 4.115200");
    TaskAdd(KEY_RET,DispCommPara ,NULL);
	if(tmp != 0)
	{
		TaskAdd(KEY_OK, DispOK,SystemReStart);
	}
}


//ͨѶ��������
void DispCommPara(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.�����Ʋ�����");
    Fyd12864DispPrintf(2,1,"2.��̨������");
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    TaskAdd(KEY_1,DispSetModbusBaud ,NULL);
    TaskAdd(KEY_2,DispSetPCBaud ,NULL);
    TaskAdd(KEY_RET, DispMenu_2,NULL);
}


//ѹ����У
void SetPressureRatioEnter(void)
{
	uint32_t  pressurevalue,temp;
	char buf[10];
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	pressurevalue=atol(buf);
	temp=globalvar.U32Temp-tmp_sysparas.pzero;
	temp*=100;
	tmp_sysparas.pcoeff=(uint16_t)(temp/pressurevalue);
    //globalvar.paraindex=33;
	Fyd12864ClearLine(2);
	if(globalvar.displayonce==1)
	{
			globalvar.displayonce=0;
			sysparas=tmp_sysparas;        
			if(SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas)))
			{
					Fyd12864DispPrintf(3,1,"    ���óɹ�");   
			}
			else
			{
					Fyd12864DispPrintf(3,1,"    ����ʧ��");   
			}
	} 
	Fyd12864DispPrintf(4,1,"�����밴�����ء�");
	TaskAdd(KEY_RET,DispSetPressure,NULL);
}
void SetPressureRatio(void)
{
	uint32_t rd;
	tmp_sysparas=sysparas;
	Fyd12864DispPrintf(1,1,"  ����ѹ��ϵ��");
	rd=PlADCGetValue();
	globalvar.U32Temp=rd;
	Fyd12864DispPrintf(2,1,"��ǰADֵ:%ld",rd);
	Fyd12864DispPrintf(3,1,"������");
	ScreenInput.inputmaxlen=5;
	NumKeyHandler();
	Fyd12864DispPrintf(4,1,"��ǰѹ��:%s",ScreenInput.array);
	TaskAdd(KEY_RET,DispSetPressure,NULL);
	TaskAdd(KEY_OK,SetPressureRatioEnter,NULL);
}

void SetPressureZeroEnter(void)
{
	Fyd12864ClearLine(2);
	if(globalvar.displayonce==1)
	{
			globalvar.displayonce=0;
			sysparas=tmp_sysparas;        
			if(SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas)))
			{
					Fyd12864DispPrintf(3,1,"    ���óɹ�");   
			}
			else
			{
					Fyd12864DispPrintf(3,1,"    ����ʧ��");   
			}
	} 
	Fyd12864DispPrintf(4,1,"�����밴�����ء�");
	TaskAdd(KEY_RET,DispSetPressure,NULL);
}
void SetPressureZero(void)
{
	uint32_t rd;
	tmp_sysparas=sysparas;
	Fyd12864DispPrintf(1,1,"  ����ѹ�����");
	rd=PlADCGetValue();
	tmp_sysparas.pzero=rd;
	if(tmp_sysparas.pzero<100)
	{
		Fyd12864ClearLine(2);
		Fyd12864DispPrintf(3,1,"ѹ���쳣");
	}
	else
	{
		//globalvar.paraindex=32;
		Fyd12864DispPrintf(2,1,"��ǰADֵ:%ld",rd);
		Fyd12864DispPrintf(3,1,"ȷ���밴��ȷ�ϡ�");
		TaskAdd(KEY_OK,SetPressureZeroEnter,NULL);
	}
	Fyd12864DispPrintf(4,1,"�����밴�����ء�");
	TaskAdd(KEY_RET,DispSetPressure,NULL);
	
}

void DispSetPressure(void)
{
	Fyd12864DispPrintf(1,1,"  У��ѹ������");
	Fyd12864DispPrintf(2,1,"1.����ѹ�����");
	Fyd12864DispPrintf(3,1,"2.����ѹ��ϵ��");
	Fyd12864ClearLine(4);
	TaskAdd(KEY_RET, DispCommonOpt1,NULL);
	TaskAdd(KEY_1,SetPressureZero,NULL);
	TaskAdd(KEY_2,SetPressureRatio,NULL);
}


//����ģʽ����
void DispSetFuelMode(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==0)||(tmp==1)||(tmp==2))
    {
        tmp_sysparas.fuelmode=tmp;
        globalvar.paraindex=60;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.fuelmode,"����ģʽ����");
    Fyd12864DispPrintf(2,1,"0.�ֶ�  1.��̨");
    Fyd12864DispPrintf(3,1,"2.���ӱ�ǩ");
    TaskAdd(KEY_RET, DispCommonOpt1,NULL);
    TaskAdd(KEY_OK, DispOK,DispCommonOpt1);
}

//�������ں�ʱ����ʾȷ��
void DispSetDateTimeOK(void)
{
    uint8_t tmp[6],i,ret=FALSE;
    if(globalvar.displayonce==1)
    {
        globalvar.displayonce=0;
        for(i=0;i<6;i++)
        {
            tmp[i]=AscToBcd(&ScreenInput.array[2*i]);
        }
        ret=Ds3232SetTime(tmp);
        if(ret==1)  Fyd12864DispPrintf(4,1,"���óɹ�");
        else        Fyd12864DispPrintf(4,1,"����ʧ��");
    }
    TaskMessage(10, DispCommonOpt);
}

//����ʱ������
void DispSetDateTime(void)
{
    uint8_t *tmp,i,ret=TRUE;
	Time    newtime={0};
	if(globalvar.KeyPreValue!=KEY_PT)
	{
		Fyd12864DispPrintf(1,1,"����ʱ������");    
		NumKeyHandler();
		ScreenInput.inputmaxlen=12;
		tmp=(uint8_t*)&newtime;
		newtime=time;
		for(i=0;i<(ScreenInput.inputlen/2+1);i++)
		{
			*(tmp+i)=AscToBcd(&ScreenInput.array[2*i]);
		}
		
		Fyd12864DispPrintf(2,1,"%02X��%02X��%02X��",
								newtime.year,
								newtime.month,
								newtime.day);
								
		Fyd12864DispPrintf(3,1,"%02Xʱ%02X��%02X��",
								newtime.hour,
								newtime.minute,
								newtime.second);	
		if(ScreenInput.inputlen>5) ret=CheckDate(tmp);
		if(ScreenInput.inputlen>9) ret=CheckTime(tmp+3);
		if(ret==FALSE)
		{
			Fyd12864DispPrintf(4,1,"�����ʽ����");
			return;
		}
		else
		{
			Fyd12864ClearLine(4);               
		}
		TaskAdd(KEY_RET, DispCommonOpt1,NULL);
		if(ScreenInput.inputlen>9)//������Ϻ���ܰ�ȷ�ϼ�
		{
			TaskAdd(KEY_OK, DispSetDateTimeOK,NULL);
		}  
	}	
}

//СƱ��ӡ��ʽ����
void DispSetPrintMode(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==0)||(tmp==1))
    {
        tmp_sysparas.printmode=tmp;
        globalvar.paraindex=61;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.printmode,"��ӡ��ʽ����");
    Fyd12864DispPrintf(2,1,"0.�ֶ���ӡ");
    Fyd12864DispPrintf(3,1,"1.�Զ���ӡ");
    TaskAdd(KEY_RET, DispCommonOpt,NULL);
    TaskAdd(KEY_OK, DispOK,DispCommonOpt);
}

//�Ƿ�ȡ������
void DispSetIntNum()
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==0)||(tmp==1))
    {
        tmp_sysparas.intnumflag = tmp;
        globalvar.paraindex=85;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.intnumflag,"�Ƿ�ȡ������");
    Fyd12864DispPrintf(2,1,"0.����ȡ��");
    Fyd12864DispPrintf(3,1,"1.����ȡ��");
    TaskAdd(KEY_RET, DispCommonOpt,NULL);
    TaskAdd(KEY_OK, DispOK,DispCommonOpt);
}

//���ò�������
void DispCommonOpt(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.��Ļѡ��");
	Fyd12864DispPrintf(2,1,"2.����ģʽ");
    Fyd12864DispPrintf(3,1,"3.�Ƿ�ȡ��");
    Fyd12864DispPrintf(4,1,"4.��ӡ��ʽ");
	TaskAdd(KEY_1,DispSetPing, NULL);
	TaskAdd(KEY_2,DispSetOffline, NULL);//huanghao 20161025    
	TaskAdd(KEY_3,DispSetIntNum, NULL);//ycx20161202
	TaskAdd(KEY_4,DispSetPrintMode ,NULL);
    TaskAdd(KEY_DOWN, DispCommonOpt1,NULL);
    TaskAdd(KEY_RET, DispMenu_2,NULL);
}
//���ò�������1��
void DispCommonOpt1(void)
{
	Fyd12864ClearLine(4);
	Fyd12864DispPrintf(1,1,"1.ѹ����У");
    Fyd12864DispPrintf(2,1,"2.����ģʽ����");
    Fyd12864DispPrintf(3,1,"3.����ʱ������");
    TaskAdd(KEY_1,DispSetPressure ,NULL);
    TaskAdd(KEY_2,DispSetFuelMode ,NULL);
    TaskAdd(KEY_3,DispSetDateTime ,NULL);
	TaskAdd(KEY_RET, DispMenu_2, NULL);
	TaskAdd(KEY_UP, DispCommonOpt, NULL);
}
//�ѻ�ģʽѡ��
void DispSetOffline(void)//huanghao 20161025
{
	uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==0)||(tmp==1))
    {
        tmp_sysparas.offlinemode = tmp;
        globalvar.paraindex=80;
		if(tmp == 1)
        {
			tmp_sysparas.yesnolock=0; //����
			tmp_sysparas.usecard=0;   //���Բ�ʹ�ÿ�
        }
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.offlinemode,"����ģʽ����");
    Fyd12864DispPrintf(2,1,"0.����ģʽ");
    Fyd12864DispPrintf(3,1,"1.�ѻ�ģʽ");
    TaskAdd(KEY_RET, DispCommonOpt,NULL);
	if(tmp_sysparas.offlinemode && globalvar.KeyValue == KEY_OK)
		CloseUartRcvIRQ(3);
	else
		OpenUartRcvIRQ(3);
    TaskAdd(KEY_OK, DispOK,DispCommonOpt);
}
//��Ļѡ������
void DispSetPing(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==0)||(tmp==1))
    {
        tmp_sysparas.lcdtype=tmp;
        globalvar.paraindex=81;
        //������Ļʱ��ˢһ����ycx20170330
        if(tmp==0) OsplcShowB(fuelrecordinfo.volume,fuelrecordinfo.money,fuelrecordinfo.price,0,1);
        else OsplcShowN(fuelrecordinfo.volume,fuelrecordinfo.money,fuelrecordinfo.price);
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.lcdtype,"��Ļѡ������");
    Fyd12864DispPrintf(2,1,"0.����");
    Fyd12864DispPrintf(3,1,"1.����");
    TaskAdd(KEY_RET, DispCommonOpt,NULL);
    TaskAdd(KEY_OK, DispOK,DispCommonOpt);
}

//�˵�����2��ʾ
void DispMenu_2(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.ͨ����Ϣ����");
    Fyd12864DispPrintf(2,1,"2.���Ʋ�������");
    Fyd12864DispPrintf(3,1,"3.ͨѶ��������");
    Fyd12864DispPrintf(4,1,"4.���ò�������");
    TaskAdd(KEY_1,DispGeneralInfo ,NULL);
    TaskAdd(KEY_2,DispSetControl1,NULL);
    TaskAdd(KEY_3,DispCommPara ,NULL);
    TaskAdd(KEY_4,DispCommonOpt ,NULL);
    TaskAdd(KEY_UP, DispMenu_1,NULL);
    TaskAdd(KEY_DOWN, DispMenu_3,NULL);
    TaskAdd(KEY_RET, DispFirst,NULL);
}

//���۰汾������
void DispSetPriceVer(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.pricever=atol((char *)ScreenInput.array);   
    globalvar.paraindex=62;
    DispSet(0,sysparas.pricever,"���۰汾������");
    TaskAdd(KEY_RET,DispVerNum1 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispVerNum1);
}

//�������汾������
void DispSetBlackVer(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.blackver=atol((char *)ScreenInput.array);  
    globalvar.paraindex=63;
    DispSet(0,sysparas.blackver,"�������汾������");
    TaskAdd(KEY_RET,DispVerNum1 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispVerNum1);
}

//�������汾������
void DispSetWhiteVer(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.whitever=atol((char *)ScreenInput.array);   
    globalvar.paraindex=64;
    DispSet(0,sysparas.whitever,"�������汾������");
    TaskAdd(KEY_RET,DispVerNum1 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispVerNum1);
}

//�����������汾������
void DispSetAddBkVer(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.addbkver=atol((char *)ScreenInput.array);
    globalvar.paraindex=65;
    DispSet(0,sysparas.addbkver,"�����������汾��");
    TaskAdd(KEY_RET,DispVerNum1 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispVerNum1);
}

//��ɾ�������汾������
void DispSetDelBkVer(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.delbkver=atol((char *)ScreenInput.array);    
    globalvar.paraindex=66;
    DispSet(0,sysparas.delbkver,"��ɾ�������汾��");
    TaskAdd(KEY_RET,DispVerNum2 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispVerNum2);
}

//ͨ����Ϣ�汾������
void DispSetCommInfoVer(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.comminfover=atol((char *)ScreenInput.array);
    globalvar.paraindex=67;
    DispSet(0,sysparas.comminfover,"ͨ����Ϣ�汾��");
    TaskAdd(KEY_RET,DispVerNum2 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispVerNum2);
}

//��Կ��Ϣ�汾������
void DispSetLockVer(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.lockver=atol((char *)ScreenInput.array); 
    globalvar.paraindex=68;
    DispSet(0,sysparas.lockver,"��Կ��Ϣ�汾��");
    TaskAdd(KEY_RET,DispVerNum2 ,NULL);
    TaskAdd(KEY_OK, DispOK,DispVerNum2);
}

//�汾�������1
void DispVerNum1()
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.���۰汾��");
    Fyd12864DispPrintf(2,1,"2.�������汾��");
    Fyd12864DispPrintf(3,1,"3.�������汾��");
    Fyd12864DispPrintf(4,1,"4.�����ڰ汾��");
    TaskAdd(KEY_1, DispSetPriceVer,NULL);
    TaskAdd(KEY_2, DispSetBlackVer,NULL);
    TaskAdd(KEY_3, DispSetWhiteVer,NULL);
    TaskAdd(KEY_4, DispSetAddBkVer,NULL);
    //TaskAdd(KEY_UP,DispVerNum2 ,NULL);
    TaskAdd(KEY_DOWN, DispVerNum2,NULL);
    TaskAdd(KEY_RET, DispMenu_3,NULL);
}

//�汾�������2
void DispVerNum2()
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.��ɾ�ڰ汾��");
    Fyd12864DispPrintf(2,1,"2.ͨ����Ϣ�汾��");
    Fyd12864DispPrintf(3,1,"3.��Կ��Ϣ�汾��");

    Fyd12864ClearLine(4);
    TaskAdd(KEY_1,DispSetDelBkVer ,NULL);
    TaskAdd(KEY_2,DispSetCommInfoVer ,NULL);
    TaskAdd(KEY_3,DispSetLockVer ,NULL);
    TaskAdd(KEY_UP,DispVerNum1 ,NULL);
    //TaskAdd(KEY_DOWN,DispVerNum1 ,NULL);
    TaskAdd(KEY_RET, DispMenu_3,NULL);
}

//�ж������Ƿ���ȷ
void DispJudgePwd(void)
{
    InputInit();
    if(globalvar.U8Temp==1)
    {
        if(globalvar.U32Temp!=sysparas.menupwd)
        {
            Fyd12864DispPrintf(4,1,"�������");
            TaskAdd(KEY_CLR,DispInputOldPwd ,NULL);
            TaskAdd(KEY_RET,DispInputOldPwd ,NULL);
        }
        else
        {
            DispUI=DispSetMenuPwd;
        }
    }
    else if(globalvar.U8Temp==2)
    {
        if(globalvar.U32Temp!=sysparas.qcpwd)
        {
            Fyd12864DispPrintf(4,1,"�������");
            TaskAdd(KEY_CLR,DispInputOldPwd ,NULL);
            TaskAdd(KEY_RET,DispInputOldPwd ,NULL);
        }
        else
        {
            DispUI=DispSetQcPwd;
        }
    }
    else if(globalvar.U8Temp==3)
    {
        if(globalvar.U32Temp!=sysparas.testpwd)
        {
            Fyd12864DispPrintf(4,1,"�������");
            TaskAdd(KEY_CLR,DispInputOldPwd ,NULL);
            TaskAdd(KEY_RET,DispInputOldPwd ,NULL);
        }
        else
        {
            DispUI=DispSetTestPwd;
        }
    }
    else if(globalvar.U8Temp==4)
    {
        if(globalvar.U32Temp!=sysparas.resetpwd)
        {
            Fyd12864DispPrintf(4,1,"�������");
            TaskAdd(KEY_CLR,DispInputOldPwd ,NULL);
            TaskAdd(KEY_RET,DispInputOldPwd ,NULL);
        }
        else
        {
            DispUI=DispSetResetPwd;
        }
    }
	else if(globalvar.U8Temp==5)
	{
		if(globalvar.U32Temp!=sysparas.onoffworkpwd)
        {
            Fyd12864DispPrintf(4,1,"�������");
            TaskAdd(KEY_CLR,DispInputOldPwd ,NULL);
            TaskAdd(KEY_RET,DispInputOldPwd ,NULL);
        }
        else
        {
            DispUI=DispSetonoffworkpwd;
        }
	}
}

//����������
void DispInputOldPwd(void)
{
    char     buf[6]={0},i;    
    ScreenInput.inputmaxlen=6;
    NumKeyHandler(); 
    Fyd12864ClearLine(1);
    Fyd12864DispPrintf(2,1,"�����������");
    Fyd12864ClearLine(4);
    for(i=0;i<ScreenInput.inputlen;i++) buf[i]='*';
    Fyd12864DispPrintf(3,1,buf);
    globalvar.U32Temp=atol((char *)ScreenInput.array);
    TaskAdd(KEY_RET,DispOthMsg1 ,NULL);
    TaskAdd(KEY_OK, DispJudgePwd,NULL);
}

//�˵���������
void DispSetMenuPwd(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=6;
    tmp_sysparas.menupwd=atol((char *)ScreenInput.array);    
    globalvar.paraindex=73;
    DispSet(6,sysparas.menupwd,"�˵�����");
    TaskAdd(KEY_RET,DispOthMsg1 ,NULL);
    if(ScreenInput.inputlen>0)//��ֱֹ�Ӱ���ȷ�ϼ�
    {
        TaskAdd(KEY_OK, DispOK,DispOthMsg1);
    }
}

//�ʼ���������
void DispSetQcPwd(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=6;
    tmp_sysparas.qcpwd=atol((char *)ScreenInput.array);  
    globalvar.paraindex=74;
    DispSet(6,sysparas.qcpwd,"�ʼ�����");
    TaskAdd(KEY_RET,DispOthMsg1 ,NULL);
    if(ScreenInput.inputlen>0)//��ֱֹ�Ӱ���ȷ�ϼ�
    {
        TaskAdd(KEY_OK, DispOK,DispOthMsg1);
    }
}

//ά����������
void DispSetTestPwd(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=6;
    tmp_sysparas.testpwd=atol((char *)ScreenInput.array);  
    globalvar.paraindex=75;
    DispSet(6,sysparas.testpwd,"ά������");
    TaskAdd(KEY_RET,DispOthMsg1 ,NULL);
    if(ScreenInput.inputlen>0)//��ֱֹ�Ӱ���ȷ�ϼ�
    {
        TaskAdd(KEY_OK, DispOK,DispOthMsg1);
    }
}

//�ָ�������������
void DispSetResetPwd(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=6;
    tmp_sysparas.resetpwd=atol((char *)ScreenInput.array); 
    globalvar.paraindex=76;
    DispSet(6,sysparas.resetpwd,"�ָ�������������");
    TaskAdd(KEY_RET,DispOthMsg1 ,NULL);
    if(ScreenInput.inputlen>0)//��ֱֹ�Ӱ���ȷ�ϼ�
    {
        TaskAdd(KEY_OK, DispOK,DispOthMsg1);
    }
}

void DispSetonoffworkpwd(void)//huanghao 20161025
{
	tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=6;
    tmp_sysparas.onoffworkpwd=atol((char *)ScreenInput.array); 
    globalvar.paraindex=77;
    DispSet(6,sysparas.onoffworkpwd,"���°�����");
    TaskAdd(KEY_RET,DispOthMsg1 ,NULL);
    if(ScreenInput.inputlen>0)//��ֱֹ�Ӱ���ȷ�ϼ�
    {
        TaskAdd(KEY_OK, DispOK,DispOthMsg1);
    }
}
//������Ϣ����1
void DispOthMsg1(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.�˵���������");
    Fyd12864DispPrintf(2,1,"2.�ʼ���������");
    Fyd12864DispPrintf(3,1,"3.ά����������");
    Fyd12864DispPrintf(4,1,"4.�ָ���������");
    if(globalvar.KeyValue==KEY_1)
    {
        globalvar.U8Temp=1;
        TaskAdd(KEY_1, DispInputOldPwd,NULL);
    }
    if(globalvar.KeyValue==KEY_2)
    {
        globalvar.U8Temp=2;
        TaskAdd(KEY_2, DispInputOldPwd,NULL);
    }
    if(globalvar.KeyValue==KEY_3)
    {
        globalvar.U8Temp=3;
        TaskAdd(KEY_3, DispInputOldPwd,NULL);
    }
    if(globalvar.KeyValue==KEY_4)
    {
        globalvar.U8Temp=4;
        TaskAdd(KEY_4, DispInputOldPwd,NULL);
    }

    //TaskAdd(KEY_UP,DispOthMsg2 ,NULL);
    TaskAdd(KEY_DOWN,DispOthMsg2 ,NULL);
    TaskAdd(KEY_RET, DispMenu_3,NULL);
}

//���ü��ܰ�
void SetPwdBoard(void)
{
    if(globalvar.displayonce==1)
    {
        globalvar.displayonce=0;
        sysparas=tmp_sysparas; 
        if(tmp_sysparas.usepwdboard==0)
        {
           if(SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas)))
           {
               SetSuccess();
           }else
           {
               SetFault();
           }
        }else
        {
            LockDataStore();
        }
    } 
    if(DispNextUI!=NULL)
    {
        TaskMessage(10, DispNextUI);
    }      
}

//���ܰ�����
void DispSetPwdBoard(void)
{
	uint8_t tmp;
	if(sysparas.usepwdboard==1 && SI_INDATE!=0)			//ʹ�ü��ܰ嵫��������ģʽ
	{
		Fyd12864ClearLine(1);
		Fyd12864ClearLine(3);
		Fyd12864ClearLine(4);
        Fyd12864DispPrintf(2,1,"������Normalģʽ");
		TaskAdd(KEY_RET, DispOthMsg2,NULL);
	}
	else
	{
		tmp_sysparas=sysparas;
		ScreenInput.inputmaxlen=1;
		tmp=atol((char *)ScreenInput.array);   
		if((tmp==0)||(tmp==1))
		{
			tmp_sysparas.usepwdboard=tmp;
            globalvar.paraindex=79;
		}
		else if(ScreenInput.inputlen>0)
		{
			Fyd12864DispPrintf(4,1,"�������");
			NumKeyHandler(); 
			return;
		}
		DispSet(5,sysparas.usepwdboard,"���ܰ�����");
		Fyd12864DispPrintf(2,1,"0.��ʹ�ü��ܰ�");
		Fyd12864DispPrintf(3,1,"1.ʹ�ü��ܰ�");
		TaskAdd(KEY_RET, DispOthMsg2,NULL);
		TaskAdd(KEY_OK, SetPwdBoard,DispOthMsg2);
	}
}

//�����������
void DispInputPwd(char* pwdbuf,fun funname)
{
    char     buf[6]={0},i;    
    ScreenInput.inputmaxlen=6;
    NumKeyHandler(); 
    Fyd12864ClearLine(1);          //���һ�С�
    Fyd12864DispPrintf(2,1,pwdbuf);//��ʾ��������˵����롱��
    Fyd12864ClearLine(4);          //������С�
    for(i=0;i<ScreenInput.inputlen;i++) buf[i]='*';
    Fyd12864DispPrintf(3,1,buf);
    globalvar.U32Temp=atol((char *)ScreenInput.array);
    TaskAdd(KEY_RET,DispFirst,NULL);
    TaskAdd(KEY_OK, funname,NULL); 
}

//�˵�������֤
void DispJudgeMenuPwd(void)
{
    if(globalvar.U32Temp!=sysparas.menupwd)
    {
        Fyd12864DispPrintf(4,1,"�������");
        TaskAdd(KEY_CLR,DispInputMenuPwd ,NULL);
        TaskAdd(KEY_RET,DispInputMenuPwd ,NULL);
    }
    else
    {
        DispUI=DispMenu_1;
    }
}

//����˵�����
void DispInputMenuPwd(void)
{
    DispInputPwd("������˵�����",DispJudgeMenuPwd);    
}

//�ʼ�������֤
void DispJudgeQcPwd(void)
{
    if(globalvar.U32Temp!=sysparas.qcpwd)
    {
        Fyd12864DispPrintf(4,1,"�������");
        TaskAdd(KEY_CLR,DispInputQcPwd ,NULL);
        TaskAdd(KEY_RET,DispInputQcPwd ,NULL);
    }
    else
    {
        DispUI=DispQcPara;
    }
}

//�����ʼ�����
void DispInputQcPwd(void)
{
    DispInputPwd("�������ʼ�����",DispJudgeQcPwd);    
}


//ά��������֤
void DispJudgeTestPwd(void)
{
    if(globalvar.U32Temp!=sysparas.testpwd)
    {
        Fyd12864DispPrintf(4,1,"�������");
        TaskAdd(KEY_CLR,DispInputTestPwd ,NULL);
        TaskAdd(KEY_RET,DispInputTestPwd ,NULL);
    }
    else
    {
        DispUI=DeviceTestMenu;
    }
}

//����ά������
void DispInputTestPwd(void)
{
    DispInputPwd("������ά������",DispJudgeTestPwd);
}

//�ָ���������������֤
void DispJudgeResetPwd(void)
{
    if((globalvar.U32Temp!=sysparas.resetpwd)&&(globalvar.U32Temp!=760103))
    {
        Fyd12864DispPrintf(4,1,"�������");
        TaskAdd(KEY_CLR,DispInputResetPwd ,NULL);
        TaskAdd(KEY_RET,DispInputResetPwd ,NULL);
    }
    else
    {
        DispUI=DispUnitQcReset;
    }
}

//����ָ���������
void DispInputResetPwd(void)
{
    DispInputPwd("������ָ�����",DispJudgeResetPwd);
}

//�ָ���������
void DispFactoryReset(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"  �ָ���������");
    Fyd12864DispPrintf(2,1,"��ȷ�ϡ�  ����");
    Fyd12864DispPrintf(3,1,"�����ء�  ȡ��");
    Fyd12864ClearLine(4);
    FactoryReset();
    if(globalvar.KeyValue == KEY_OK)
		ScreenInput.inputlen = 1;
    TaskAdd(KEY_OK, DispOK,SystemReStart);
    TaskAdd(KEY_RET, DispUnitQcReset,NULL);
}


//������Ϣ����2
void DispOthMsg2(void)
{
	/*
    InputInit();
    Fyd12864DispPrintf(1,1,"1.���ܰ�����");
    Fyd12864ClearLine(2);
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    TaskAdd(KEY_1, DispSetPwdBoard,NULL);         
    TaskAdd(KEY_UP,DispOthMsg1 ,NULL);
    TaskAdd(KEY_RET, DispMenu_3,NULL);
	*/
	InputInit();
	Fyd12864DispPrintf(1,1,"1.���°���������");
    Fyd12864DispPrintf(2,1,"2.���ܰ�����");

    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
	if(globalvar.KeyValue==KEY_1)
    {
        globalvar.U8Temp=5;
        TaskAdd(KEY_1, DispInputOldPwd,NULL);
    }
    TaskAdd(KEY_2, DispSetPwdBoard,NULL);         
    TaskAdd(KEY_UP,DispOthMsg1 ,NULL);
    TaskAdd(KEY_RET, DispMenu_3,NULL);
}

//�ܶ�����
void DispSetDensty(void)
{   
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=6;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*10000);
    tmp_sysparas.densty=atol(buf);
    globalvar.paraindex=30;
	if(tmp_sysparas.densty > 0 && tmp_sysparas.densty < 10000)
	{
		TaskAdd(KEY_OK,DispOK,DispQcPara);
	}
	else if((ScreenInput.inputlen>5))
	{
		Fyd12864DispPrintf(4,1,"�ܶȳ���");
        NumKeyHandler(); 
		return;
	}
    DispSet(4,sysparas.densty,"�ܶ�����");   
    TaskAdd(KEY_RET, DispQcPara,NULL);
}

//����ϵ������
void DispSetCoeff(void)
{   
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=6;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*10000);
    tmp_sysparas.coeff=atol(buf);
	if(tmp_sysparas.coeff > 0 && tmp_sysparas.coeff <= 15000)
	{
		TaskAdd(KEY_OK, DispOK,DispQcPara);
	}
	else if((ScreenInput.inputlen>5))
	{
		
		Fyd12864DispPrintf(4,1,"����ϵ������");
        NumKeyHandler(); 
		return;
	}
    globalvar.paraindex=31;
    DispSet(4,sysparas.coeff,"����ϵ������");   
    TaskAdd(KEY_RET, DispQcPara,NULL);
    TaskAdd(KEY_OK, DispOK,DispQcPara);
}

//�ʼ��������
void DispQcPara(void)
{
	if(sysparas.usepwdboard==1 && SI_INDATE!=0)			//ʹ�ü��ܰ嵫��������ģʽ
	{
		Fyd12864ClearLine(1);
		Fyd12864ClearLine(3);
		Fyd12864ClearLine(4);
        Fyd12864DispPrintf(2,1,"������Normalģʽ");
		TaskAdd(KEY_RET, DispFirst,NULL);
	}
	else if(sysparas.usepwdboard==0 || (sysparas.usepwdboard==1 && LockDataCompare()))
	{													//��ʹ�ü��ܰ��ʹ�ü��ܰ���������ȷ
		InputInit();
		Fyd12864DispPrintf(1,1,"1.�ܶ�����");
		Fyd12864DispPrintf(2,1,"2.����ϵ������");
		Fyd12864ClearLine(3);
		Fyd12864ClearLine(4);
		TaskAdd(KEY_1, DispSetDensty,NULL);
		TaskAdd(KEY_2, DispSetCoeff,NULL);
		TaskAdd(KEY_RET, DispFirst,NULL);
	}
	else
	{
		Fyd12864ClearLine(1);
		Fyd12864ClearLine(3);
		Fyd12864ClearLine(4);
        Fyd12864DispPrintf(2,1,"���ܰ岻ƥ�䣡��");
		TaskAdd(KEY_RET, DispFirst,NULL);
	}
}

//��������оƬ
void EraseChipA(void)
{
    if(globalvar.displayonce==1)
    {
        globalvar.displayonce=0;
        SSTEraseChip(0);//�������Σ�ȷ����ȷ
        SSTEraseChip(0);
        SetSuccess();          
    }
    TaskMessage(10,DispEraseChip);  
}

//��������оƬ
void EraseChipB(void)
{
    if(globalvar.displayonce==1)
    {
        globalvar.displayonce=0;
        SSTEraseChip(1);//�������Σ�ȷ����ȷ
        SSTEraseChip(1);
        SetSuccess();         
    }
    TaskMessage(10,DispEraseChip);  
}

//��������оƬѯ��
void DispEraseChipA(void)
{
    Fyd12864DispPrintf(1,1,"  ȷ�ϲ�������?");
    Fyd12864DispPrintf(2,1,"��ȷ�ϡ�  ����");
    Fyd12864DispPrintf(3,1,"�����ء�  ȡ��");
    Fyd12864ClearLine(4);
    TaskAdd(KEY_OK,EraseChipA, NULL);
    TaskAdd(KEY_RET,DispEraseChip ,NULL);
}

//��������оƬѯ��
void DispEraseChipB(void)
{
    Fyd12864DispPrintf(1,1,"  ȷ�ϲ�������?");
    Fyd12864DispPrintf(2,1,"��ȷ�ϡ�  ����");
    Fyd12864DispPrintf(3,1,"�����ء�  ȡ��");
    Fyd12864ClearLine(4);
    TaskAdd(KEY_OK,EraseChipB, NULL);
    TaskAdd(KEY_RET,DispEraseChip ,NULL);
}

//Flash�洢оƬ����
void DispEraseChip(void)
{
    globalvar.displayonce=1;
    Fyd12864DispPrintf(1,1,"1.��������оƬ");
    Fyd12864DispPrintf(2,1,"2.��������оƬ");
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    TaskAdd(KEY_1,DispEraseChipA ,NULL);
    TaskAdd(KEY_2,DispEraseChipB ,NULL);
    TaskAdd(KEY_RET, DispUnitQcReset,NULL);
}

// ������λ���ʼ�ģʽ���ָ���������
void DispUnitQcReset(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.������λ����");
    Fyd12864DispPrintf(2,1,"2.�ʼ�ģʽ����");
    Fyd12864DispPrintf(3,1,"3.�洢оƬ����");
    Fyd12864DispPrintf(4,1,"0.�ָ���������");  
    TaskAdd(KEY_1, DispSetUnit,NULL);
    TaskAdd(KEY_2, DispSetQCMode,NULL);
    TaskAdd(KEY_3, DispEraseChip,NULL);
    TaskAdd(KEY_0, DispFactoryReset,NULL);
    TaskAdd(KEY_RET, DispFirst,NULL);
}

void CirculateFuelingSet(void)
{
	uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
	tmp=ScreenInput.array[0];   
    if((tmp=='0')||(tmp=='1'))
    {
		globalvar.circulatefue = atol((char *)ScreenInput.array);
		DispUI = DispFirst;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,globalvar.circulatefue,"ѭ����������");
    Fyd12864DispPrintf(2,1,"0.��������");
    Fyd12864DispPrintf(3,1,"1.ѭ������");
    TaskAdd(KEY_RET, DispMenu_3,NULL);
    TaskAdd(KEY_OK, DispOK,DispMenu_3);
}


//�˵�����3��ʾ
void DispMenu_3(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.�汾�������");
    Fyd12864DispPrintf(2,1,"2.������Ϣ����");
	Fyd12864DispPrintf(3,1,"3.ģ���������");
    Fyd12864DispPrintf(4,1,"4.ѭ����������");
 
    TaskAdd(KEY_1, DispVerNum1,NULL);
    TaskAdd(KEY_2, DispOthMsg1,NULL);
	TaskAdd(KEY_3, SimulatorSet, NULL);
    TaskAdd(KEY_4, CirculateFuelingSet,NULL);
    TaskAdd(KEY_UP, DispMenu_2,NULL);
    //TaskAdd(KEY_DOWN, DispMenu_1,NULL);
    TaskAdd(KEY_RET, DispFirst,NULL);
}

//�ϰࡣ
void OnWork(void)
{
    uint8_t *tmp;
    if(sysparas.offonwork==1)
    {
         Fyd12864ClearLine(1);
         Fyd12864ClearLine(3);
         Fyd12864ClearLine(4);
         Fyd12864DispPrintf(2,1,"�����ظ��ϰ�");
         TaskAdd(KEY_RET, DispFirst,NULL);
    }
    else
    {
		Fyd12864ClearLine(1);
		Fyd12864ClearLine(2);
		Fyd12864ClearLine(4);
    	
		sysparas.shiftransnum++;
		shiftrecordinfo.classid=sysparas.shiftransnum;          //���°��¼��ˮ�ţ�4�ֽڡ�
		shiftrecordinfo.transactiontype=0;                      //�������ͣ�1�ֽڡ�
		sysparas.shiftnum++;
		if(sysparas.shiftnum>sysparas.shiftmaxnum)              //�ж��Ƿ��������š�
		{
			sysparas.shiftnum=1;
		}
		shiftrecordinfo.classnum=sysparas.shiftnum;             //��ţ�1�ֽڡ�
		if(sysparas.offlinemode == 1)
		{
			shiftrecordinfo.cardstaffnum[0] = 0;
			shiftrecordinfo.cardstaffnum[1] = 0;
			shiftrecordinfo.cardstaffnum[2] = 0;
			shiftrecordinfo.cardstaffnum[3] = 0;
			shiftrecordinfo.cardstaffnum[4] = 0;
			shiftrecordinfo.cardstaffnum[5] = 0;
			shiftrecordinfo.cardstaffnum[6] = 0;
			shiftrecordinfo.cardstaffnum[7] = 0;
		}
		else
		{
			memcpy(&shiftrecordinfo.cardstaffnum[0],&m1card.cardnum[0],8);	//Ա�����ţ�BCD�롣
		}
				
		shiftrecordinfo.classstarttime[0]=0x20;                 //�࿪ʼʱ�䣬7�ֽڣ�BCD�롣
		memcpy(&shiftrecordinfo.classstarttime[1],&time.year,6);
		memcpy(&sysparas.shiftstarttime[0],&time.year,6);
		  
		memset(&shiftrecordinfo.classendtime[0],0,7);           //�����ʱ�䣬7�ֽڣ�BCD�롣
		memset(&sysparas.shiftendtime[0],0,6);                  
		
		shiftrecordinfo.allgunnum=sysparas.totvolume;           //ǹ�����ۼƣ�4�ֽڡ�
		shiftrecordinfo.allgunmoney=sysparas.totmoney;          //ǹ����ۼ�,4�ֽڡ�
		shiftrecordinfo.classgasnum=0;                          //��������4�ֽڡ�
		shiftrecordinfo.classgasmoney=0;                        //����,4�ֽڡ�
		shiftrecordinfo.classtimes=0;                           //�����������4�ֽڡ�
		sysparas.shiftfueltimes=0;
		shiftrecordinfo.devicetype=sysparas.devicetype;         //��Ʒ���룬1�ֽڣ�0-CNG��1-LNG��
		shiftrecordinfo.measuretype=sysparas.unit;              //������λ��1�ֽڣ�0-�귽��1-Kg��
		shiftrecordinfo.classiding=sysparas.shiftransnum;       //�ϰ��¼��ˮ�š�
	
		sysparas.shiftotvol=0;                                  //��������0��
		sysparas.shiftotmon=0;                                  //�������㡣
		sysparas.shiftstartvol=sysparas.totvolume;              //�࿪ʼǹ�ۡ�
		sysparas.shiftstartmon=sysparas.totmoney;               //�࿪ʼ��
		
		tmp=(uint8_t *)(&shiftrecordinfo.classid);
		shiftrecordinfo.crc=CRC16Calculate(tmp, sizeof(shiftrecordinfo)-2);
		if(SaveFuelRecord(1))          //�����ˮ��
		{
			sysparas.offonwork=1;       //�ϰ��־��
			globalvar.workstate=StaIdle;//�ϰ�ɹ�ʹ���Ϊ����״̬��
			if(SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas)))
			{
				Fyd12864ClearLine(1);
				Fyd12864DispPrintf(2,1,"�ϰ�ɹ���"); 
				Fyd12864ClearLine(3);
				Fyd12864ClearLine(4);
			}
			else
			{							//һ������������ᱨ��
//				sysparas.offonwork = 0;  //���ʧ�ܾ�ǿ����0���������ϰ࣬ 
				Fyd12864ClearLine(1);
				Fyd12864DispPrintf(2,1,"�ϰ�ʧ�ܣ�");
				Fyd12864ClearLine(3);
				Fyd12864DispPrintf(4,1,"�洢����ʧ�ܣ�");
			}
					
		}
		else
		{
			Fyd12864ClearLine(1);
			Fyd12864DispPrintf(2,1,"�ϰ�ʧ�ܣ�");
			Fyd12864ClearLine(3);
			Fyd12864DispPrintf(4,1,"�洢����ˮʧ�ܣ�"); 
		}	
    }
    DispUI=Diswork;  
}

//�°ࡣ
void OffWork(void)
{
    uint8_t *tmp;
    if(sysparas.offonwork==0)
    {
         Fyd12864ClearLine(1);
         Fyd12864ClearLine(3);
         Fyd12864ClearLine(4);
         Fyd12864DispPrintf(2,1,"�����ظ��°�");
         TaskAdd(KEY_RET, DispFirst,NULL);
    }
    else
    {
		Fyd12864ClearLine(1);
		Fyd12864ClearLine(3);
		
		sysparas.shiftransnum++;
		shiftrecordinfo.classid=sysparas.shiftransnum;          //���°��¼��ˮ�ţ�4�ֽڡ�
		shiftrecordinfo.transactiontype=1;                      //�������ͣ�1�ֽڡ�
		shiftrecordinfo.classnum=sysparas.shiftnum;             //��ţ�1�ֽڡ�
			
		if(sysparas.offlinemode == 1)
		{
			shiftrecordinfo.cardstaffnum[0] = 0;
			shiftrecordinfo.cardstaffnum[1] = 0;
			shiftrecordinfo.cardstaffnum[2] = 0;
			shiftrecordinfo.cardstaffnum[3] = 0;
			shiftrecordinfo.cardstaffnum[4] = 0;
			shiftrecordinfo.cardstaffnum[5] = 0;
			shiftrecordinfo.cardstaffnum[6] = 0;
			shiftrecordinfo.cardstaffnum[7] = 0;
		}
		else
		{
			memcpy(&shiftrecordinfo.cardstaffnum[0],&m1card.cardnum[0],8);//Ա�����ţ�BCD�롣
		}
				  
		shiftrecordinfo.classstarttime[0]=0x20;                 //�࿪ʼʱ�䣬7�ֽڣ�BCD�롣
		memcpy(&shiftrecordinfo.classstarttime[1],&sysparas.shiftstarttime[0],6);
		
		shiftrecordinfo.classendtime[0]=0x20;                   //�����ʱ�䣬7�ֽڣ�BCD�롣
		memcpy(&shiftrecordinfo.classendtime[1],&time.year,6);
		memcpy(&sysparas.shiftendtime[1],&time.year,6);
		
		shiftrecordinfo.allgunnum=sysparas.totvolume;           //ǹ�����ۼƣ�4�ֽڡ�
		shiftrecordinfo.allgunmoney=sysparas.totmoney;          //ǹ����ۼ�,4�ֽڡ�
		shiftrecordinfo.classgasnum=sysparas.shiftotvol;        //��������4�ֽڡ�
		shiftrecordinfo.classgasmoney=sysparas.shiftotmon;      //����,4�ֽڡ�
		shiftrecordinfo.classtimes=sysparas.shiftfueltimes;     //�����������4�ֽڡ�
		shiftrecordinfo.devicetype=sysparas.devicetype;         //��Ʒ���룬1�ֽڣ�0-CNG��1-LNG��
		shiftrecordinfo.measuretype=sysparas.unit;              //������λ��1�ֽڣ�0-�귽��1-Kg��
		shiftrecordinfo.classiding=(sysparas.shiftransnum-1);   //�ϰ��¼��ˮ�š�
		
		tmp=(uint8_t *)(&shiftrecordinfo.classid);
		shiftrecordinfo.crc=CRC16Calculate(tmp, sizeof(shiftrecordinfo)-2);
		if(SaveFuelRecord(1))//�����ˮ��
		{
			globalvar.workstate=StaOffDuty;//������״̬���°ࡣ
			sysparas.offonwork=0;          //�°��־��
			if(SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas)))
			{
				Fyd12864ClearLine(1);
				Fyd12864DispPrintf(2,1,"�°�ɹ���");
				Fyd12864ClearLine(3);
				Fyd12864ClearLine(4);
				PrintShifid();
			}
			else
			{
				Fyd12864ClearLine(1);
				Fyd12864DispPrintf(2,1,"�°�ʧ�ܣ�");
				Fyd12864ClearLine(3);
				Fyd12864DispPrintf(4,1,"�洢����ʧ�ܣ�");
			}
		}
		else
		{
			Fyd12864ClearLine(1);
			Fyd12864DispPrintf(2,1,"�°�ʧ�ܣ�");
			Fyd12864ClearLine(3);
			Fyd12864DispPrintf(4,1,"�洢����ˮʧ�ܣ�");
		}	
	}         
	DispUI=Diswork;    
}

void Diswork(void)
{
    TaskMessage(10, DispFirst);
}


//����ģ�������
void SimulatorSet(void)
{
	uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
	tmp=ScreenInput.array[0];   
    if((tmp=='0')||(tmp=='1'))
    {
		sysparas.simulation = atol((char *)ScreenInput.array);
        globalvar.paraindex=84;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.simulation,"ģ���������");
    Fyd12864DispPrintf(2,1,"0.��������");
    Fyd12864DispPrintf(3,1,"1.ģ�����");
    TaskAdd(KEY_RET, DispMenu_3,NULL);
    TaskAdd(KEY_OK, DispOK,DispMenu_3);
}
	
void WriteICcard(void)
{
	m1card.cardstate = 1;
	Mt318WriteM1Card(KEYB, m1card.keyB, 10, &m1card.cardstate);
}

void OnWorkWithPwd(void)
{
	DispInputPwd("�������ϰ�����",DispJudgeOnWorkPwd);
}
void DispJudgeOnWorkPwd(void)
{
    if(globalvar.U32Temp!=sysparas.onoffworkpwd)
    {
        Fyd12864DispPrintf(4,1,"�������");
        TaskAdd(KEY_CLR,OnWorkWithPwd, NULL);
        TaskAdd(KEY_RET,OnWorkWithPwd, NULL);
    }
    else
    {
        DispUI=OnWork;
    }
}

void OffWorkWithPwd(void)
{
	DispInputPwd("�������°�����",DispJudgeOffWorkPwd);
}
void DispJudgeOffWorkPwd(void)
{
    if(globalvar.U32Temp!=sysparas.onoffworkpwd)
    {
        Fyd12864DispPrintf(4,1,"�������");
        TaskAdd(KEY_CLR,OffWorkWithPwd, NULL);
        TaskAdd(KEY_RET,OffWorkWithPwd, NULL);
    }
    else
    {
        DispUI=OffWork;
    }
}
