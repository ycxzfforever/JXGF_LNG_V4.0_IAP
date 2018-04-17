/***************************************************************************
**�ı���DispControl.c
**˵��������CNG����LNG�е�"���Ʋ���"�˵���ʾ
**������ �����
**�������ڣ�2016-4-21 17:17:20

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/

#include "kernel.h"
//���Ʋ�����ʾCNG��LNG��Ҫ�ֿ���д������

//Ԥ�䳬ʱʱ������
void DispSetPrecoolingTimeout(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=4;
    tmp_sysparas.precooltimeout=atol((char *)ScreenInput.array);    
    globalvar.paraindex=206;
    DispSet(0,sysparas.precooltimeout,"Ԥ�䳬ʱʱ��");
    TaskAdd(KEY_RET, DispSetControl1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl1);
}

//Ԥ���ٽ��¶�����
void DispSetPrecoolingTemp(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.precooltemp=atol((char *)ScreenInput.array);  
    globalvar.paraindex=203;
    DispSet(7,sysparas.precooltemp,"Ԥ���ٽ��¶�����");
    TaskAdd(KEY_RET, DispSetControl1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl1);
}

//Ԥ���ٽ���������
void DispSetPrecoolingGain(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.precoolgain=atol((char *)ScreenInput.array); 
    globalvar.paraindex=204;
    DispSet(0,sysparas.precoolgain,"Ԥ���ٽ���������");
    TaskAdd(KEY_RET, DispSetControl1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl1);
}

//Ԥ���ٽ��ܶ�����
void DispSetPrecoolingDensty(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=6;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*10000);
    tmp_sysparas.precooldensty=atol(buf);   
    globalvar.paraindex=205;
    DispSet(4,sysparas.precooldensty,"Ԥ���ٽ��ܶ�����");
    TaskAdd(KEY_RET, DispSetControl1,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl1);
}

//LNG���ÿ��Ʋ���1
void DispSetControl1(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.Ԥ�䳬ʱʱ��");
    Fyd12864DispPrintf(2,1,"2.Ԥ���ٽ��¶�");
    Fyd12864DispPrintf(3,1,"3.Ԥ���ٽ�����");
    Fyd12864DispPrintf(4,1,"4.Ԥ���ٽ��ܶ�");
    TaskAdd(KEY_1, DispSetPrecoolingTimeout,NULL);
    TaskAdd(KEY_2, DispSetPrecoolingTemp,NULL);
    TaskAdd(KEY_3, DispSetPrecoolingGain,NULL);
    TaskAdd(KEY_4, DispSetPrecoolingDensty,NULL);
    TaskAdd(KEY_DOWN, DispSetControl2,NULL);
    TaskAdd(KEY_RET, DispMenu_2,NULL);
}

//ͣ����ʱʱ������
void DispSetDelayStopTime(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.delaystoptime=atol((char *)ScreenInput.array);   
    globalvar.paraindex=207;
    DispSet(0,sysparas.delaystoptime,"ͣ����ʱʱ��");
    TaskAdd(KEY_RET, DispSetControl2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl2);
}

//��ѹͣ������
void DispSetPressureStop(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=4;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    tmp_sysparas.pressurestop=atol(buf);   
    globalvar.paraindex=212;
    DispSet(2,sysparas.pressurestop,"��ѹͣ������");
    TaskAdd(KEY_RET, DispSetControl2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl2);
}

//��ͣ����������
void DispSetLowRateStop(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.lowratestop=atol((char *)ScreenInput.array);  
    globalvar.paraindex=208;
    DispSet(0,sysparas.lowratestop,"��ͣ����������");
    TaskAdd(KEY_RET, DispSetControl2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl2);
}

//��ͣ����������
void DispSetHighRateStop(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=4;
    tmp_sysparas.highratestop=atol((char *)ScreenInput.array);   
    globalvar.paraindex=209;
    DispSet(0,sysparas.highratestop,"��ͣ����������");
    TaskAdd(KEY_RET, DispSetControl2,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl2);
}

//LNG���ÿ��Ʋ���2
void DispSetControl2(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.ͣ����ʱ");
    Fyd12864DispPrintf(2,1,"2.��ѹͣ��");
    Fyd12864DispPrintf(3,1,"3.������ͣ��");
    Fyd12864DispPrintf(4,1,"4.������ͣ��");
    TaskAdd(KEY_1, DispSetDelayStopTime,NULL);
    TaskAdd(KEY_2, DispSetPressureStop,NULL);
    TaskAdd(KEY_3, DispSetLowRateStop,NULL);
    TaskAdd(KEY_4, DispSetHighRateStop,NULL);
    TaskAdd(KEY_UP, DispSetControl1,NULL);
    TaskAdd(KEY_DOWN, DispSetControl3,NULL);
    TaskAdd(KEY_RET, DispMenu_2,NULL);
}

//�����ܶ�ͣ������
void DispSetBackDenstyStop(void)
{
    char buf[16]={0};
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=6;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*10000);
    tmp_sysparas.backdenstystop=atol(buf);   
    globalvar.paraindex=210;
    DispSet(4,sysparas.backdenstystop,"�����ܶ�ͣ��");
    TaskAdd(KEY_RET, DispSetControl3,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl3);
}

//��������ͣ������
void DispSetBackGainStop(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.backgainstop=atol((char *)ScreenInput.array);    
    globalvar.paraindex=211;
    DispSet(0,sysparas.backgainstop,"��������ͣ��");
    TaskAdd(KEY_RET, DispSetControl3,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl3);
}

//�Ƿ��������
void DispSetBackGasCalc(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==0)||(tmp==1))
    {
        tmp_sysparas.backgascalc=tmp;
        globalvar.paraindex=201;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.backgascalc,"�Ƿ��������");
    Fyd12864DispPrintf(2,1,"0.������");
    Fyd12864DispPrintf(3,1,"1.����");
    TaskAdd(KEY_RET, DispSetControl3,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl3);
}

//Ԥ�䷽ʽѡ��
void DispSetPrecoolMode(void)
{
    uint8_t tmp;
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=1;
    tmp=atol((char *)ScreenInput.array);   
    if((tmp==0)||(tmp==1))
    {
        tmp_sysparas.precoolmode=tmp;
        globalvar.paraindex=202;
    }
    else if(ScreenInput.inputlen>0)
    {
        Fyd12864DispPrintf(4,1,"�������");
        NumKeyHandler(); 
        return;
    }
    DispSet(5,sysparas.precoolmode,"�Ƿ��������");
    Fyd12864DispPrintf(2,1,"0.Сѭ��Ԥ��");
    Fyd12864DispPrintf(3,1,"1.��ѭ��Ԥ��");
    TaskAdd(KEY_RET, DispSetControl3,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl3);
}

//LNG���ÿ��Ʋ���3
void DispSetControl3(void)
{
    InputInit();
    Fyd12864DispPrintf(1,1,"1.�����ܶ�ͣ��");
    Fyd12864DispPrintf(2,1,"2.��������ͣ��");
    Fyd12864DispPrintf(3,1,"3.�Ƿ��������");
    Fyd12864DispPrintf(4,1,"4.Ԥ�䷽ʽѡ��");
    TaskAdd(KEY_1, DispSetBackDenstyStop,NULL);
    TaskAdd(KEY_2, DispSetBackGainStop,NULL);
    TaskAdd(KEY_3, DispSetBackGasCalc,NULL);
    TaskAdd(KEY_4, DispSetPrecoolMode,NULL);
    TaskAdd(KEY_UP, DispSetControl2,NULL);
    TaskAdd(KEY_DOWN, DispSetControl4,NULL);
    TaskAdd(KEY_RET, DispMenu_2,NULL);
}

//��Һ��ͣ����ʱʱ��
void DispSetStopPumpTime(void)
{
    tmp_sysparas=sysparas;
    ScreenInput.inputmaxlen=3;
    tmp_sysparas.stoppumptime=atol((char *)ScreenInput.array);    
    globalvar.paraindex=213;
    DispSet(0,sysparas.stoppumptime,"��Һ��ͣ����ʱ");
    TaskAdd(KEY_RET, DispSetControl4,NULL);
    TaskAdd(KEY_OK, DispOK,DispSetControl4);
}

//LNG���ÿ��Ʋ���4
void DispSetControl4(void)
{
    InputInit();    
    Fyd12864DispPrintf(1,1,"1.ͣ����ʱ%d",sysparas.stoppumptime);
    Fyd12864ClearLine(2);
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    TaskAdd(KEY_1, DispSetStopPumpTime,NULL);
    TaskAdd(KEY_UP, DispSetControl3,NULL);
    TaskAdd(KEY_RET, DispMenu_2,NULL);
}



//���Ʋ���1��ѯ
void DispCtrlParas1(void)
{
    Fyd12864DispPrintf(1,1,"Ԥ�䳬ʱʱ��%d",sysparas.precooltimeout);
    Fyd12864DispPrintf(2,1,"Ԥ���ٽ��¶�-%d",sysparas.precooltemp);
    Fyd12864DispPrintf(3,1,"Ԥ���ٽ�����%d",sysparas.precoolgain);
    Fyd12864DispPrintf(4,1,"Ԥ���ٽ��ܶ�%d.%04d",sysparas.precooldensty/10000,sysparas.precooldensty%10000);
    TaskAdd(KEY_DOWN, DispCtrlParas2,NULL);
    TaskAdd(KEY_RET, DispQuery1,NULL);
}

//���Ʋ���2��ѯ
void DispCtrlParas2(void)
{
    Fyd12864DispPrintf(1,1,"ͣ����ʱ%d",sysparas.delaystoptime);
    Fyd12864DispPrintf(2,1,"��ѹͣ��%d.%02d",sysparas.pressurestop/100,sysparas.pressurestop%100);
    Fyd12864DispPrintf(3,1,"������ͣ��%d",sysparas.lowratestop);
    Fyd12864DispPrintf(4,1,"������ͣ��%d",sysparas.highratestop);
    TaskAdd(KEY_UP, DispCtrlParas1,NULL);
    TaskAdd(KEY_DOWN, DispCtrlParas3,NULL);
    TaskAdd(KEY_RET, DispQuery1,NULL);
}

//���Ʋ���3��ѯ
void DispCtrlParas3(void)
{
    Fyd12864DispPrintf(1,1,"�����ܶ�ͣ��%d.%04d",sysparas.backdenstystop/10000,sysparas.backdenstystop%10000);
    Fyd12864DispPrintf(2,1,"��������ͣ��%d",sysparas.backgainstop);
    Fyd12864DispPrintf(3,1,"�Ƿ��������%d",sysparas.backgascalc);
    Fyd12864DispPrintf(4,1,"Ԥ�䷽ʽѡ��%d",sysparas.precoolmode);
    TaskAdd(KEY_UP, DispCtrlParas2,NULL);
    TaskAdd(KEY_DOWN, DispCtrlParas4,NULL);
    TaskAdd(KEY_RET, DispQuery1,NULL);
}

//���Ʋ���4��ѯ
void DispCtrlParas4(void)
{
    Fyd12864DispPrintf(1,1,"ͣ����ʱ%d",sysparas.stoppumptime);
    Fyd12864ClearLine(2);
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    TaskAdd(KEY_UP, DispCtrlParas3,NULL);
    TaskAdd(KEY_RET, DispQuery1,NULL);
}


//�ָ���������ʱ��������������
void FactoryReset(void)
{
    uint8_t i;
    tmp_sysparas.head=0xAA;            //ͷ
    tmp_sysparas.writeE2num=0;         //дeeprom����
    
    tmp_sysparas.gunnum=1;             //ǹ��
    tmp_sysparas.transnum=0;           //������ˮ��     
    tmp_sysparas.uptransnum=0;         //�Ѵ�������ˮ��     
    
    tmp_sysparas.totvolume=0;          //ǹ������    2λС��
    tmp_sysparas.totmoney=0;           //ǹ�ܽ��    2λС��

    for(i=0;i<20;i++) tmp_sysparas.price[i]=450;      //20�����ۣ�����0ΪĬ�ϵ��ۣ����ݵ��۵ȼ�ѡ����Ӧ����
    
    //������Ϣ
    tmp_sysparas.shiftnum=1;           //���
    tmp_sysparas.shiftmaxnum=3;        //�����
    tmp_sysparas.shiftransnum=0;       //����ˮ��     
    tmp_sysparas.shiftuptransnum=0;    //�Ѵ�����ˮ��     
    tmp_sysparas.shiftfueltimes=0;     //���������
    tmp_sysparas.shiftstartvol=0;      //�࿪ʼǹ��    2λС��
    tmp_sysparas.shiftstartmon=0;      //�࿪ʼ���    2λС��
    tmp_sysparas.shiftotvol=0;         //���ۼ�����    2λС��
    tmp_sysparas.shiftotmon=0;         //���ۼƽ��    2λС��

    tmp_sysparas.shiftstarttime[0]=time.year;    //�࿪ʼʱ��    BCD
    tmp_sysparas.shiftstarttime[1]=time.month;
    tmp_sysparas.shiftstarttime[2]=time.day;
    tmp_sysparas.shiftstarttime[3]=time.hour;    
    tmp_sysparas.shiftstarttime[4]=time.minute;
    tmp_sysparas.shiftstarttime[5]=time.second;

    for(i=0;i<6;i++) tmp_sysparas.shiftendtime[i]=0;    //�����ʱ��    BCD
    
    //ͨ����Ϣ
    for(i=0;i<2;i++) tmp_sysparas.shengcode[i]=0;       //ʡ����
    for(i=0;i<4;i++) tmp_sysparas.stationcode[i]=0;     //վ����
    for(i=0;i<2;i++) tmp_sysparas.citycode[i]=0;        //���д���
    for(i=0;i<4;i++) tmp_sysparas.parentcode[i]=0;      //�ϼ���λ����

    //����Ϣ
    tmp_sysparas.cardmaxyue=100000000;   //���������    2λС�� Ĭ��100��
    tmp_sysparas.cardminyue=100;         //���������    2λС�� 
    tmp_sysparas.cardpsamnum=1;          //psam������ѡ��
    tmp_sysparas.cardmaxday=90;          //��δʹ�����������
    tmp_sysparas.cardcheck=0;            //�Ƿ�����ƿ
    tmp_sysparas.usecard=0;              //Ĭ�ϲ��ÿ�

    //����������
    tmp_sysparas.modbusbaud=19200;       //�����Ʋ�����
    tmp_sysparas.pcbaud=38400;           //��̨������

    tmp_sysparas.fuelmode=0;             //����ģʽ
    tmp_sysparas.printmode=0;            //СƱ��ӡ��ʽ

    //�汾�������
    tmp_sysparas.pricever=0;             //���۰汾��
    tmp_sysparas.blackver=0;             //�������汾��
    tmp_sysparas.whitever=0;             //�������汾��
    tmp_sysparas.addbkver=0;             //�����������汾��
    tmp_sysparas.delbkver=0;             //��ɾ�������汾��
    tmp_sysparas.comminfover=0;          //ͨ����Ϣ�汾��

    //��������
    tmp_sysparas.menupwd=0;              //�˵���������
    tmp_sysparas.qcpwd=0;                //�ʼ�����
    tmp_sysparas.testpwd=0;              //�������룬�簴������ŷ��Ȳ���
    tmp_sysparas.resetpwd=0;             //�ָ���������
    for(i=0;i<6;i++) tmp_sysparas.pwdboard[i]=0;        //���ܰ�����
    tmp_sysparas.usepwdboard=0;          //0-��ʹ�ü��ܰ壬1-ʹ�ü��ܰ�
    
    
    tmp_sysparas.modbustype=1;           //����������
    tmp_sysparas.unit=0;                 //������λ
    tmp_sysparas.qcmode=0;               //�ʼ�ģʽ����

    tmp_sysparas.backgascalc=0;          //0-������������1-��������
    tmp_sysparas.precoolmode=0;          //Ԥ�䷽ʽ��0-Сѭ����1-��ѭ��
    tmp_sysparas.precooltemp=120;        //Ԥ���ٽ��¶�
    tmp_sysparas.precoolgain=100;        //Ԥ���ٽ�����
    tmp_sysparas.precooldensty=3000;     //Ԥ���ٽ��ܶȣ�4λС����0-1.0000
    tmp_sysparas.precooltimeout=900;     //Ԥ�䳬ʱʱ��
    tmp_sysparas.delaystoptime=30;       //�տ�ʼ��Һʱ,���ж�ͣ����������ʱʱ��
    tmp_sysparas.lowratestop=300;        //������ͣ��
    tmp_sysparas.highratestop=4000;      //������ͣ��
    tmp_sysparas.backdenstystop=4000;    //�����ܶ�ͣ����4λС����0-1.0000
    tmp_sysparas.backgainstop=30;        //��������ͣ��
    tmp_sysparas.pressurestop=180;       //ͣ��ѹ��,��ѹͣ������λС��
    tmp_sysparas.stoppumptime=10;        //��Һ����ʱͣ��

    tmp_sysparas.densty=6804;            //�ܶ�              4λС��
    tmp_sysparas.coeff=10000;            //����ϵ��  4λС��
    tmp_sysparas.pzero=820;              //ѹ�����  
    tmp_sysparas.pcoeff=105;             //ѹ��ϵ��  2λС��       
    
    tmp_sysparas.modnum=0;               //�޸ļ�¼��ˮ��     
    tmp_sysparas.upmodnum=0;             //�Ѵ��޸ļ�¼��ˮ��           
    tmp_sysparas.simulation=0;           //Ĭ������������
    tmp_sysparas.intnumflag=0;           //Ĭ�ϲ�ȡ��
    tmp_sysparas.devicetype=0;           //�豸����
    tmp_sysparas.lcdtype=1;              //LNGĬ�ϴ���

}

