/***************************************************************************
**�ı���query.c
**˵�������ڲ�ѯ��ʾ�����ں����϶࣬�Ҵ�ͬС�죬
                        ����ֻ��ÿ������������
**������ �����
**�������ڣ�2016-4-25 10:46:36

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/

#include "kernel.h"
Reply5B  tmp;//�����޸ļ�¼��ѯ����ʹ�á�
//���۲�ѯ��ʾ
void PrinterSum(void)
{
    PrintSum();
    DispUI=DispSum;
}

void DispSum(void)
{
    Fyd12864DispPrintf(1,1,"��ˮ��%ld",sysparas.transnum);
    Fyd12864DispPrintf(2,1,"�Ѵ���ˮ��%ld",sysparas.uptransnum);
    Fyd12864DispPrintf(3,1,"������%ld.%02d",sysparas.totvolume/100,sysparas.totvolume%100);
    Fyd12864DispPrintf(4,1,"�ܽ��%ld.%02d",sysparas.totmoney/100,sysparas.totmoney%100);
    TaskAdd(KEY_RET, DispQuery1,NULL); 
	TaskAdd(KEY_OK, PrinterSum,NULL); 
}

//����������ʾ������ݱ���¼���ݶ�ȡ��ѯ
void DispShiftSum(void)
{
    Fyd12864DispPrintf(1,1,"���%ld",sysparas.shiftnum);
    Fyd12864DispPrintf(2,1,"���������%ld",sysparas.shiftfueltimes);
    Fyd12864DispPrintf(3,1,"������%ld.%02d",sysparas.shiftotvol/100,sysparas.shiftotvol%100);
    Fyd12864DispPrintf(4,1,"����%ld.%02d",sysparas.shiftotmon/100,sysparas.shiftotmon%100);    
    TaskAdd(KEY_RET, DispQuery1,NULL); 
}

//��������ˮ��
void DispInputTransNum(void)
{
	if(sysparas.transnum > 0)
	{
		//Fyd12864ClearLine(1);
		Fyd12864DispPrintf(1,1,"          %05ld",sysparas.transnum);
		Fyd12864DispPrintf(2,1,"��������ˮ��");
		Fyd12864DispPrintf(3,1,"%s",ScreenInput.array);    
		ScreenInput.inputmaxlen=16;
		ScreenInput.pointnum=0;
		fuelrecordinfo.transnum=atol((char *)ScreenInput.array);
		NumKeyHandler(); 
		TaskAdd(KEY_RET, DispQuery1,NULL);
		if(fuelrecordinfo.transnum<=sysparas.transnum)
		{
			Fyd12864ClearLine(4);
			if(fuelrecordinfo.transnum==0)
			{
				fuelrecordinfo.transnum=sysparas.transnum;
			}
			TaskAdd(KEY_OK, DispRecord,NULL);
		}
		else
		{
			Fyd12864DispPrintf(4,1,"��ˮ�ų���");
		}
	}
	else
	{
		Fyd12864ClearLine(1);
		Fyd12864DispPrintf(2,1,"  ����ˮ��¼");
		Fyd12864ClearLine(3);
		Fyd12864ClearLine(4);
		TaskMessage(20, DispQuery1);
	}
}

//���������ˮ��
void DispInputShifTransNum(void)
{
    //Fyd12864ClearLine(1);
	Fyd12864DispPrintf(1,1,"          %05ld",sysparas.shiftransnum);
    Fyd12864DispPrintf(2,1,"���������ˮ��");
    Fyd12864DispPrintf(3,1,"%s",ScreenInput.array);    
    ScreenInput.inputmaxlen=16;
    ScreenInput.pointnum=0;
    shiftrecordinfo.classid=atol((char *)ScreenInput.array);
    NumKeyHandler(); 
    TaskAdd(KEY_RET, DispQuery2,NULL);
    if(shiftrecordinfo.classid<=sysparas.shiftransnum)
    {
        Fyd12864ClearLine(4);
        if(shiftrecordinfo.classid==0)
        {
            shiftrecordinfo.classid=sysparas.shiftransnum;
        }
        TaskAdd(KEY_OK, DispshifRecord,NULL);
    }
    else
    {
        Fyd12864DispPrintf(4,1,"��ˮ�ų���");
    }
}

//����ˮ��¼��ѯ��ʾ
void DispshifRecord(void)
{
    if(globalvar.displayonce==1)
    {
        globalvar.displayonce=0;
        ReadFuelRecord(shiftrecordinfo.classid-1,1);
    }
    Fyd12864DispPrintf(1,1,"����ˮ��: %ld",  shiftrecordinfo.classid);
    if(shiftrecordinfo.transactiontype==0)
        Fyd12864DispPrintf(2,1,"�ϰ࿨��%02x%02x%02x%02x",shiftrecordinfo.cardstaffnum[4],shiftrecordinfo.cardstaffnum[5],
                                            shiftrecordinfo.cardstaffnum[6],shiftrecordinfo.cardstaffnum[7]);
    else if(shiftrecordinfo.transactiontype==1)
        Fyd12864DispPrintf(2,1,"�°࿨��%02x%02x%02x%02x",shiftrecordinfo.cardstaffnum[4],shiftrecordinfo.cardstaffnum[5],
                                            shiftrecordinfo.cardstaffnum[6],shiftrecordinfo.cardstaffnum[7]);
    Fyd12864DispPrintf(3,1,"������:%ld.%02d",shiftrecordinfo.classgasnum/100,shiftrecordinfo.classgasnum%100);
    Fyd12864DispPrintf(4,1,"����:%ld.%02d",shiftrecordinfo.classgasmoney/100,shiftrecordinfo.classgasmoney%100);  
    TaskAdd(KEY_UP,RecordShifUp,NULL);
    TaskAdd(KEY_DOWN,RecordShifDown,NULL);
    TaskAdd(KEY_RET, DispQuery2,NULL); 
    TaskAdd(KEY_OK, Printershifid,NULL); 
}
void Printershifid(void)
{
    PrintShifid();
    DispUI=DispshifRecord;
}
//����ˮ���¼���ѯ
void RecordShifUp(void)
{
     if(shiftrecordinfo.classid>1)
    {
        shiftrecordinfo.classid--;
        globalvar.displayonce=1;
    }
    DispUI=DispshifRecord;
}

//����ˮ���ϼ���ѯ
void RecordShifDown(void)
{
    if(shiftrecordinfo.classid<sysparas.shiftransnum)
    {
        shiftrecordinfo.classid++;
        globalvar.displayonce=1;
    }
    DispUI=DispshifRecord;
}

//��ˮ���ϼ���ѯ
void RecordUp(void)
{
    if(fuelrecordinfo.transnum>1)
    {
        fuelrecordinfo.transnum--;
        globalvar.displayonce=1;
    }
    DispUI=DispRecord;
}

//��ˮ���¼���ѯ
void RecordDown(void)
{
    if(fuelrecordinfo.transnum<sysparas.transnum)
    {
        fuelrecordinfo.transnum++;
        globalvar.displayonce=1;
    }
    DispUI=DispRecord;
}

//��ˮ��¼��ѯ��ʾ
void DispRecord(void)
{
    if(globalvar.displayonce==1)
    {
        globalvar.displayonce=0;
        
        ReadFuelRecord(fuelrecordinfo.transnum-1,0);
    }
    Fyd12864DispPrintf(1,1,"��ˮ��:%ld",fuelrecordinfo.transnum);
    Fyd12864DispPrintf(2,1,"����:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
    Fyd12864DispPrintf(3,1,"����:%d.%02d",fuelrecordinfo.price/100,fuelrecordinfo.price%100);    
    Fyd12864DispPrintf(4,1,"���:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
    TaskAdd(KEY_UP,RecordUp,NULL);
    TaskAdd(KEY_DOWN,RecordDown,NULL);
    TaskAdd(KEY_RET, DispQuery1,NULL); 
    TaskAdd(KEY_OK, Printerid,NULL);
}

void Printerid(void)
{
    Printid();
    DispUI=DispRecord;
}

void PrinterSysInfo(void)
{
    PrintSysInfo();
    DispUI=DispSysParas1;
}

//ϵͳ��Ϣ��ʾ1
void DispSysParas1(void)
{
    Fyd12864DispPrintf(1,1,"ǹ��%02d",sysparas.gunnum);
    Fyd12864DispPrintf(2,1,"����%d.%02d",sysparas.price[0]/100,sysparas.price[0]%100);
    Fyd12864DispPrintf(3,1,"�޸���ˮ��%ld",sysparas.modnum);
    Fyd12864DispPrintf(4,1,"�Ѵ��޸ĺ�%ld",sysparas.upmodnum);
    TaskAdd(KEY_UP,DispSysParas9 ,NULL); 
    TaskAdd(KEY_DOWN,DispSysParas2 ,NULL); 
    TaskAdd(KEY_RET, DispQuery2,NULL); 
	TaskAdd(KEY_OK, PrinterSysInfo,NULL);
}

//ϵͳ��Ϣ��ʾ2
void DispSysParas2(void)
{
    Fyd12864DispPrintf(1,1,"������%ld.%02d",sysparas.totvolume/100,sysparas.totvolume%100);
    Fyd12864DispPrintf(2,1,"�ܽ��%ld.%02d",sysparas.totmoney/100,sysparas.totmoney%100);
    Fyd12864DispPrintf(3,1,"���%d",sysparas.shiftnum);
    Fyd12864DispPrintf(4,1,"�����%d",sysparas.shiftmaxnum);

    TaskAdd(KEY_UP,DispSysParas1 ,NULL);
    TaskAdd(KEY_DOWN,DispSysParas3 ,NULL); 
    TaskAdd(KEY_RET, DispQuery2,NULL); 
	TaskAdd(KEY_OK, PrinterSysInfo,NULL);
}

//ϵͳ��Ϣ��ʾ3
void DispSysParas3(void)
{
    Fyd12864DispPrintf(1,1,"����������%d",sysparas.modbustype);
    Fyd12864DispPrintf(2,1,"������λ%d",sysparas.unit);
    Fyd12864DispPrintf(3,1,"�ʼ�ģʽ%d",sysparas.qcmode);
    Fyd12864DispPrintf(4,1,"EEд����%ld",(SYSPARA_ADDR/sizeof(Sysparas))*EEPROM_W_TIMES+sysparas.writeE2num);

    TaskAdd(KEY_UP,DispSysParas2 ,NULL);
    TaskAdd(KEY_DOWN,DispSysParas4 ,NULL); 
    TaskAdd(KEY_RET, DispQuery2,NULL); 
	TaskAdd(KEY_OK, PrinterSysInfo,NULL);
}

//ϵͳ��Ϣ��ʾ4
void DispSysParas4(void)
{
    Fyd12864DispPrintf(1,1,"�ܶ�%d.%04d",sysparas.densty/10000,sysparas.densty%10000);
    Fyd12864DispPrintf(2,1,"����ϵ��%d.%04d",sysparas.coeff/10000,sysparas.coeff%10000);
    Fyd12864DispPrintf(3,1,"ѹ�����%d",sysparas.pzero);
    Fyd12864DispPrintf(4,1,"ѹ��ϵ��%d",sysparas.pcoeff);

    TaskAdd(KEY_UP,DispSysParas3 ,NULL);
    TaskAdd(KEY_DOWN,DispSysParas5 ,NULL); 
    TaskAdd(KEY_RET, DispQuery2,NULL); 
	TaskAdd(KEY_OK, PrinterSysInfo,NULL);
}


//ϵͳ��Ϣ��ʾ5
void DispSysParas5(void)
{
    Fyd12864DispPrintf(1,1,"������%ld.%02d",sysparas.cardmaxyue/100,sysparas.cardmaxyue%100);
    Fyd12864DispPrintf(2,1,"������%ld.%02d",sysparas.cardminyue/100,sysparas.cardminyue%100);
    Fyd12864DispPrintf(3,1,"PSAM����%d",sysparas.cardpsamnum);
    Fyd12864DispPrintf(4,1,"�������%d",sysparas.cardmaxday);

    TaskAdd(KEY_UP,DispSysParas4 ,NULL);
    TaskAdd(KEY_DOWN,DispSysParas6 ,NULL); 
    TaskAdd(KEY_RET, DispQuery2,NULL); 
	TaskAdd(KEY_OK, PrinterSysInfo,NULL);
}

//ϵͳ��Ϣ��ʾ6
void DispSysParas6(void)
{
    Fyd12864DispPrintf(1,1,"�Ƿ�����ƿ%d",sysparas.cardcheck);
    Fyd12864DispPrintf(2,1,"�����Ʋ���%ld",sysparas.modbusbaud);
    Fyd12864DispPrintf(3,1,"��̨������%ld",sysparas.pcbaud);
    Fyd12864DispPrintf(4,1,"����ģʽ%d",sysparas.fuelmode);

    TaskAdd(KEY_UP,DispSysParas5 ,NULL);
    TaskAdd(KEY_DOWN,DispSysParas7 ,NULL); 
    TaskAdd(KEY_RET, DispQuery2,NULL); 
	TaskAdd(KEY_OK, PrinterSysInfo,NULL);
}

//ϵͳ��Ϣ��ʾ7
void DispSysParas7(void)
{
    Fyd12864DispPrintf(1,1,"��ӡ��ʽ%d",sysparas.printmode);
    Fyd12864DispPrintf(2,1,"���۰��%d",sysparas.pricever);
    Fyd12864DispPrintf(3,1,"���������%d",sysparas.blackver);
    Fyd12864DispPrintf(4,1,"���������%d",sysparas.whitever);

    TaskAdd(KEY_UP,DispSysParas6 ,NULL);
    TaskAdd(KEY_DOWN,DispSysParas8 ,NULL); 
    TaskAdd(KEY_RET, DispQuery2,NULL); 
	TaskAdd(KEY_OK, PrinterSysInfo,NULL);
}

//ϵͳ��Ϣ��ʾ8
void DispSysParas8(void)
{
    Fyd12864DispPrintf(1,1,"�����ڰ��%d",sysparas.addbkver);
    Fyd12864DispPrintf(2,1,"��ɾ�ڰ��%d",sysparas.delbkver);
    Fyd12864DispPrintf(3,1,"ͨ����Ϣ���%d",sysparas.comminfover);
    Fyd12864DispPrintf(4,1,"��Կ���%d",sysparas.lockver);
   

    TaskAdd(KEY_UP,DispSysParas7 ,NULL);
    TaskAdd(KEY_DOWN,DispSysParas9 ,NULL); 
    TaskAdd(KEY_RET, DispQuery2,NULL);
	TaskAdd(KEY_OK, PrinterSysInfo,NULL);	
}

//ϵͳ��Ϣ��ʾ9
void DispSysParas9(void)
{
    Fyd12864ClearLine(2);
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    Fyd12864DispPrintf(1,1,"�Ƿ�ʹ�ü��ܰ�%d",sysparas.usepwdboard);
    TaskAdd(KEY_UP,DispSysParas8 ,NULL);
    TaskAdd(KEY_DOWN,DispSysParas1 ,NULL); 
    TaskAdd(KEY_RET, DispQuery2,NULL);
	TaskAdd(KEY_OK, PrinterSysInfo,NULL);
}

//��ѯϵͳ�汾��
void DispSystemVer()
{
    Fyd12864ClearLine(1);
    Fyd12864DispPrintf(2,1,"ϵͳ�汾��");
    Fyd12864DispPrintf(3,1,"%s",VERSION);
    Fyd12864ClearLine(4);
    TaskAdd(KEY_RET, DispQuery1,NULL);
}

//��ѯ�˵���ʾ1
void DispQuery1(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.�������ݲ�ѯ");
    Fyd12864DispPrintf(2,1,"2.�������ݲ�ѯ");
    Fyd12864DispPrintf(3,1,"3.��ˮ��¼��ѯ");
    Fyd12864DispPrintf(4,1,"4.���Ʋ�����ѯ");
    TaskAdd(KEY_1,DispSum ,NULL);
    TaskAdd(KEY_2,DispShiftSum ,NULL);
    TaskAdd(KEY_3,DispInputTransNum ,NULL);
    TaskAdd(KEY_4,DispCtrlParas1,NULL);
    TaskAdd(KEY_5,DispSystemVer,NULL);
    TaskAdd(KEY_DOWN,DispQuery2, NULL);
    TaskAdd(KEY_RET, DispFirst,NULL);
}

//��ѯ�˵���ʾ2
void DispQuery2(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.ϵͳ��Ϣ��ѯ");
    Fyd12864DispPrintf(2,1,"2.����ˮ�Ų�ѯ");
    Fyd12864DispPrintf(3,1,"3.�޸���ˮ��ѯ");
	Fyd12864DispPrintf(4,1,"4.�ڰ�������ѯ");
    
    TaskAdd(KEY_1,DispSysParas1 ,NULL);
    TaskAdd(KEY_2,DispInputShifTransNum ,NULL);
    TaskAdd(KEY_3,DispInputXGNum ,NULL);
	TaskAdd(KEY_4,DispHeiBai ,NULL);
    TaskAdd(KEY_UP,DispQuery1, NULL);
    TaskAdd(KEY_RET, DispFirst,NULL);
}

void DispHeiBai(void)
{
	Fyd12864DispPrintf(1,1,"1.������������ѯ");
    Fyd12864DispPrintf(2,1,"2.������������ѯ");
	Fyd12864DispPrintf(3,1,"3.��ɾ��������ѯ");
	Fyd12864DispPrintf(4,1,"4.������������ѯ");
	TaskAdd(KEY_1,DispHei ,NULL);
    TaskAdd(KEY_2,DispHeiAdd ,NULL);
	TaskAdd(KEY_3,DispHeiDel ,NULL);
	TaskAdd(KEY_4,DispBai ,NULL);
	TaskAdd(KEY_RET, DispQuery2,NULL);
}

void DispHei(void)
{
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
	Fyd12864DispPrintf(1,1,"������������");
	Fyd12864DispPrintf(2,1,"%ld",sysparas.blacknum);
    DebugHeiBai(FlashBlkSADDR, sysparas.blacknum,"������");
    TaskAdd(KEY_RET, DispHeiBai,NULL);
}
void DispHeiAdd(void)
{
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
	Fyd12864DispPrintf(1,1,"������������");
	Fyd12864DispPrintf(2,1,"%ld",sysparas.blackaddnum);
    DebugHeiBai(FlashAddBlkSADDR, sysparas.blackaddnum,"����������");
    TaskAdd(KEY_RET, DispHeiBai,NULL);
}
void DispHeiDel(void)
{
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
	Fyd12864DispPrintf(1,1,"��ɾ��������");
	Fyd12864DispPrintf(2,1,"%ld",sysparas.blackdelnum);
    DebugHeiBai(FlashDelBlkSADDR, sysparas.blackdelnum,"��ɾ������");
    TaskAdd(KEY_RET, DispHeiBai,NULL);
}
void DispBai(void)
{
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
	Fyd12864DispPrintf(1,1,"������������");
	Fyd12864DispPrintf(2,1,"%ld",sysparas.whitenum);
    DebugHeiBai(FlashWhiteSADDR, sysparas.whitenum,"������");
    TaskAdd(KEY_RET, DispHeiBai,NULL);
}
//�������޸���ˮ��
void DispInputXGNum(void)
{
	if(sysparas.modnum > 0)
	{
		//Fyd12864ClearLine(1);
		Fyd12864DispPrintf(1,1,"          %05ld",sysparas.modnum);
		Fyd12864DispPrintf(2,1,"�������޸���ˮ��");
		Fyd12864DispPrintf(3,1,"%s",ScreenInput.array);    
		ScreenInput.inputmaxlen=16;
		ScreenInput.pointnum=0;
		globalvar.xgtransnum=atol((char *)ScreenInput.array);
		NumKeyHandler(); 
		TaskAdd(KEY_RET, DispQuery2,NULL);
		if(globalvar.xgtransnum<=sysparas.modnum)
		{
			Fyd12864ClearLine(4);
			if(globalvar.xgtransnum==0)
			{
				globalvar.xgtransnum=1;
			}
			TaskAdd(KEY_OK, DispXGRecord,NULL);
		}
		else
		{
			Fyd12864DispPrintf(4,1,"��ˮ�ų���");
		}
	}
	else
	{
		Fyd12864ClearLine(1);
		Fyd12864DispPrintf(2,1,"  ���޸ļ�¼");
		Fyd12864ClearLine(3);
		Fyd12864ClearLine(4);
		TaskMessage(20, DispQuery2);
	}
}
//�޸���ˮ��¼��ѯ��ʾ
void DispXGRecord(void)
{
	uint32_t addr,id;
    uint8_t *pt;
    if(globalvar.displayonce==1)
    {
        globalvar.displayonce=0;
        id=globalvar.xgtransnum%FlashModMaxID;
        addr=FlashModSADDR+(id-1)*FlashModLen;
        pt=(uint8_t *)(&tmp.modifyid);
        ReadFlash(1,addr,pt,sizeof(Reply5B));
    }
	if((tmp.paraindex >=73) && (tmp.paraindex <= 77))
	{
		Fyd12864DispPrintf(1,1,"�޸���ˮ��%ld",  tmp.modifyid);
        Fyd12864DispPrintf(2,1,"����������%d",  tmp.paraindex);
        Fyd12864DispPrintf(3,1,"��ֵ��******");
        Fyd12864DispPrintf(4,1,"��ֵ��******"); 
	}
	else if(tmp.paraindex == 30 || tmp.paraindex == 31)
	{
		if(tmp.paraindex==30)
		{
			Fyd12864DispPrintf(1,1,"�ܶ�  ��ˮ��%ld",  tmp.modifyid);
		}
		else if(tmp.paraindex==31)
		{
			Fyd12864DispPrintf(1,1,"����  ��ˮ��%ld",  tmp.modifyid);
		}
		Fyd12864DispPrintf(2,1,"��ֵ��%ld.%02d",  tmp.beforeparameter/10000,tmp.beforeparameter%10000);
		Fyd12864DispPrintf(3,1,"��ֵ��%ld.%02d",  tmp.afterparameter/10000,tmp.afterparameter%10000); 
		Fyd12864DispPrintf(4,1,"%02x-%02x-%02x%02x:%02x:%02x", tmp.modifytime[1],tmp.modifytime[2],tmp.modifytime[3],
															tmp.modifytime[4],tmp.modifytime[5],tmp.modifytime[6]);
	}
	else
	{
		if(tmp.dexnum==2)
		{
			Fyd12864DispPrintf(1,1,"�޸���ˮ��%ld",  tmp.modifyid);		//tmp��һ��Reply5B�ṹ��
			Fyd12864DispPrintf(2,1,"����������%d",  tmp.paraindex);
			Fyd12864DispPrintf(3,1,"��ֵ��%ld.%02d",  tmp.beforeparameter/100,tmp.beforeparameter%100);
			Fyd12864DispPrintf(4,1,"��ֵ��%ld.%02d",  tmp.afterparameter/100,tmp.afterparameter%100);                  
		}else if(tmp.dexnum==4)
		{
			Fyd12864DispPrintf(1,1,"�޸���ˮ��%ld",  tmp.modifyid);
			Fyd12864DispPrintf(2,1,"����������%d",  tmp.paraindex);
			Fyd12864DispPrintf(3,1,"��ֵ��%ld.%02d",  tmp.beforeparameter/10000,tmp.beforeparameter%10000);
			Fyd12864DispPrintf(4,1,"��ֵ��%ld.%02d",  tmp.afterparameter/10000,tmp.afterparameter%10000); 
		}else if(tmp.dexnum==7)
		{
			Fyd12864DispPrintf(1,1,"�޸���ˮ��%ld",  tmp.modifyid);
			Fyd12864DispPrintf(2,1,"����������%d",  tmp.paraindex);
			Fyd12864DispPrintf(3,1,"��ֵ��-%ld",  tmp.beforeparameter,tmp.beforeparameter);
			Fyd12864DispPrintf(4,1,"��ֵ��-%ld",  tmp.afterparameter,tmp.afterparameter); 
		}else
		{
			Fyd12864DispPrintf(1,1,"�޸���ˮ��%ld",  tmp.modifyid);
			Fyd12864DispPrintf(2,1,"����������%d",  tmp.paraindex);
			Fyd12864DispPrintf(3,1,"��ֵ��%ld",  tmp.beforeparameter,tmp.beforeparameter);
			Fyd12864DispPrintf(4,1,"��ֵ��%ld",  tmp.afterparameter,tmp.afterparameter); 
		}
	}
	TaskAdd(KEY_RET, DispQuery2,NULL); 
	TaskAdd(KEY_UP,RecordXGUp,NULL);
    TaskAdd(KEY_DOWN,RecordXGDown,NULL);    TaskAdd(KEY_RET, DispQuery2,NULL); 
}
//�޸���ˮ���¼���ѯ
void RecordXGUp(void)
{
     if(globalvar.xgtransnum>1)
    {
        globalvar.xgtransnum--;
        globalvar.displayonce=1;
    }
    DispUI=DispXGRecord;
}

//�޸���ˮ���ϼ���ѯ
void RecordXGDown(void)
{
    if(globalvar.xgtransnum<sysparas.modnum)
    {
        globalvar.xgtransnum++;
        globalvar.displayonce=1;
    }
    DispUI=DispXGRecord;
}

void DebugHeiBai(uint32_t addr, uint32_t num,uint8_t *ptr)
{
	uint8_t pt[200], i;

#if 0
	uint8_t	blacktest[24]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};		
	SaveFlash(0 , FlashBlkSADDR, blacktest, 24 );
	SaveFlash(1 , FlashBlkSADDR, blacktest, 24 );
			
#endif
#if 1 
	if(globalvar.displayonce==1)
	{
		ReadFlash(1,addr, pt, num*8);//sysparas.blacknum *8);
		SELECT_PRINT();
		WHPrinterHead();
		WHPrintf("        %s\r\n",ptr);
		WHPrinterNormalCmd();
		for(i=0;i < num;i++)//sysparas.blacknum;i++)
		{
			WHPrintf("%d:%02x%02x%02x%02x%02x%02x%02x%02x\r\n", i, pt[i*8],pt[i*8+1],pt[i*8+2],pt[i*8+3],pt[i*8+4],pt[i*8+5],pt[i*8+6],pt[i*8+7]);
		}
		globalvar.displayonce=0;
		SELECT_CARD();
	}
#endif
}
