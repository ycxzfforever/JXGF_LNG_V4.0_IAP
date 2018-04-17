#include "kernel.h"

/***************************************************************************
**�������ƣ� PumpSwitch
**�������ܣ����ݼ�������״̬�򿪺͹ر���Ӧ�ķ��ź�PLC�����ź�
**��ڲ�����state:��������״̬
**����ֵ����
**�����ߣ����
**�������ڣ�2016-4-1 12:35:28
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void PumpSwitch(unsigned char state)
{
    switch(state)
    {
        case StaClose://ͣ��
            PUMPCTRL_HIGH_CLOSE();  //�ø߹�
            PUMPCTRL_LOW_CLOSE();   //�õ͹�
            VCTRL_CLOSEKEY();       //������
            VCTRL_MID_CLOSE();      //�й�
            VCTRL_HIGH_CLOSE();     //�߹�
            break;
        case StaOtherFuel://�Է���Һ
            PUMPCTRL_LOW_OPEN();    //�õͿ�
            PUMPCTRL_HIGH_CLOSE();  //�ø߹�
            VCTRL_OPENKEY();        //������
            VCTRL_MID_CLOSE();      //�й�
            VCTRL_HIGH_CLOSE();     //�߹�
            break;
        case StaSPrecool://Сѭ��Ԥ��
            PUMPCTRL_HIGH_OPEN();
            PUMPCTRL_LOW_CLOSE();
            VCTRL_OPENKEY();
            VCTRL_MID_OPEN();
            VCTRL_HIGH_CLOSE();
            break;
        case StaBPrecool://��ѭ��Ԥ��
            PUMPCTRL_HIGH_OPEN();
            PUMPCTRL_LOW_CLOSE();
            VCTRL_OPENKEY();
            VCTRL_MID_CLOSE();
            VCTRL_HIGH_OPEN();
            break;
        case StaFuel://��Һ
            PUMPCTRL_LOW_OPEN();    //PLC
            PUMPCTRL_HIGH_OPEN();   //PLC
            VCTRL_OPENKEY();
            VCTRL_MID_CLOSE();
            VCTRL_HIGH_OPEN();
            break;
        case StaIdle://����
        default:
            PUMPCTRL_LOW_OPEN();
            PUMPCTRL_HIGH_CLOSE();
            VCTRL_OPENKEY();
            VCTRL_MID_OPEN();
            VCTRL_HIGH_CLOSE();
            break;
    }
}

/***************************************************************************
**�������ƣ� GetIntNum
**�������ܣ�����������ȡ��
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-12-2 11:10:10
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void GetIntNum()
{
    if(globalvar.KeyValue==KEY_PT)//��С����ȡ����������
    {
        globalvar.fix=2;          //����
        globalvar.fixmoney=((fuelrecordinfo.money+100)/100)*100;        
    }
}


/***************************************************************************
**�������ƣ� JudgeStopConditions
**�������ܣ��ж�ͣ������
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-5-10 13:06:28
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void JudgeStopConditions(void)
{
    double money_tmp;
    double price_tmp;
    double volume_tmp;
    if(globalvar.timeout > (uint32_t)(sysparas.delaystoptime))//ͣ����ʱ������ж�ͣ������
    {
        if(realtime.cur_flowrate_l<(float)sysparas.lowratestop) //������ͣ��
        {
            globalvar.stoptimes++;
            stopreason=8;
        }
        if(realtime.cur_flowrate_l>(float)sysparas.highratestop) //������ͣ��
        {
            globalvar.stoptimes++;
            stopreason=9;
        }
        if(realtime.cur_pressure>((float)sysparas.pressurestop)) //��ѹͣ��??
        {
            globalvar.stoptimes++;
            stopreason=12;
        }
        if(realtime.cur_density_g>((float)sysparas.backdenstystop/10000.0)) //�����ܶ�ͣ��
        {
            globalvar.stoptimes++;
            stopreason=10;
        }
        if(realtime.cur_gain_g>((float)sysparas.backgainstop)) //��������ͣ��
        {
            globalvar.stoptimes++;
            stopreason=11;
        }        
    }
    
    if(globalvar.stoptimes>100)
    {
        globalvar.KeyValue=KEY_STOP;
    }
    else
    {
        stopreason=0;
    }
    
    if(m1card.cardtype==2)//���˿���
    {
        if((fuelrecordinfo.money+m1card.cardbalance)>=sysparas.cardmaxyue)//������ޡ�
        {
            money_tmp=(double)(fuelrecordinfo.money);
            price_tmp=(double)(realtime.price);
            fuelrecordinfo.volume=(uint32_t)(money_tmp/price_tmp*100+0.5);
            stopreason=14;
            globalvar.KeyValue=KEY_STOP;
        }
    }
    if(sysparas.intnumflag==1)//�ж��Ƿ���Ҫȡ��
    {
        GetIntNum();        
    }    
    if(globalvar.fix==1)//��������
    {
        if(fuelrecordinfo.volume+10>=(globalvar.fixnum))//��ǰ0.1kg�ط�
        {
            fuelrecordinfo.volume=globalvar.fixnum;
            volume_tmp = (double)fuelrecordinfo.volume/100.0;
            fuelrecordinfo.money=(uint32_t)(volume_tmp*realtime.price+0.5);
            stopreason=6;
            globalvar.KeyValue=KEY_STOP;
        }
    }
    if(globalvar.fix==2)//����
    {
        if(fuelrecordinfo.money+5>=(globalvar.fixmoney))//��ǰ0.5Ԫ�ط�
        {
            fuelrecordinfo.money=(globalvar.fixmoney);
            money_tmp=(double)(globalvar.fixmoney);
            price_tmp=(double)(realtime.price);
            fuelrecordinfo.volume=(uint32_t)(money_tmp*100/price_tmp+0.5);
            stopreason=7;
            globalvar.KeyValue=KEY_STOP;
        }
    }

    if((m1card.existflag==0)&&(m1card.cardtype!=0))//�п�����ʱ����;�ο�
    {
        globalvar.KeyValue=KEY_STOP;
        stopreason=13;
    }    
}

/***************************************************************************
**�������ƣ� StartPrecooling
**�������ܣ�����ǰ��Ԥ��
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-5-10 10:08:51
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void StartPrecooling(void)
{
    globalvar.startstate=0xff;            //����ӱ�ǩ���    
    if(globalvar.displayonce==1)
    {
        timer1on();
        MsDelay(300);                    //��ֹ��һ�μ�����˴��ټ���������δ�ı�ͻ�ֱ������Ԥ��
        globalvar.displayonce=0;
    }
    if(sysparas.simulation)
    {
        realtime.cur_flowrate_l+=10;      //����
        realtime.cur_temperature_l-=10;   //Ԥ���¶�
        realtime.cur_gain_l-=10;          //Ԥ������
        realtime.cur_density_l+=10;       //Ԥ���ܶ�
    }
    
    if((DispNextUI==DispFirst)&&(sysparas.precoolmode==1))//ֻ�е���Ԥ�����ʱ��Ż���ڴ�ѭ������������¾�����Сѭ�� 
    {
        PumpSwitch(StaBPrecool);
        globalvar.workstate = StaBPrecool;
    }
    else 
    {
        PumpSwitch(StaSPrecool);
        globalvar.workstate = StaSPrecool;
    }

    if(realtime.cur_totalize_l > 20000)
    {
        timer1off();
        MsDelay(300);
        WATCH_DOG();
        ModbusClearTotal(_MODBUS_L_);
        timer1on();
        memset(&realtime.lcng,0,sizeof(RealTimeData));//�����ۺ�������¶�ȡ������
    }
    if(realtime.cur_totalize_g > 20000)
    {
        timer1off();
        MsDelay(300);
        WATCH_DOG();
        ModbusClearTotal(_MODBUS_G_);
        timer1on();
        memset(&realtime.lcng,0,sizeof(RealTimeData));//�����ۺ�������¶�ȡ������
    }

    Fyd12864DispPrintf(1,1,"����Ԥ��%d",globalvar.timeout);
    Fyd12864DispPrintf(2,1,"����:%.02f",realtime.cur_flowrate_l);
    Fyd12864DispPrintf(3,1,"�¶�:%.02f",realtime.cur_temperature_l);
    Fyd12864DispPrintf(4,1,"����:%.02f",realtime.cur_gain_l);
    OsplcShow(0,0,0,0,0);
    if(globalvar.timeout>sysparas.precooltimeout)//Ԥ�䳬ʱ
    {
        timer1off();
        if(globalvar.otherstate==1)//�Է�δ��Һ
        {
            PumpSwitch(StaIdle);
        }
        else
        {
            PumpSwitch(StaOtherFuel);
        } 
        DispUI=DispFirst;
    }
    else//�ﵽԤ��Ҫ��
    {
        if((realtime.cur_temperature_l<((float)(0-sysparas.precooltemp)))//Ԥ���¶�
           &&(realtime.cur_density_l>((float)(sysparas.precooldensty)/10000.0))//Ԥ���ܶ�
           &&(realtime.cur_gain_l<((float)(sysparas.precoolgain))))//Ԥ������
        {
            if((m1card.existflag==1)&&(m1card.checkresult==0)&&(DispNextUI!=DispFirst))
            {
                DispUI=GreyLock;
            }
            else if(DispNextUI!=NULL)
            {
                DispUI=DispNextUI;
                if(globalvar.otherstate==1)//�Է�δ��Һ
                {
                    PumpSwitch(StaSPrecool);
                }
                else
                {
                    PumpSwitch(StaOtherFuel);
                }           
            }
            else
            {
                DispUI=DispFirst;
            }
        }
    }
    globalvar.stoppumptime=sysparas.stoppumptime+2;//Ԥ���Ҳͣ����ʱ��
    if(globalvar.stoptimes>100)//�������쳣��ѹ���������쳣ͣ��
    {
        globalvar.KeyValue=KEY_STOP;        
    }
    TaskAdd(KEY_STOP,DispFirst, NULL);
}

/***************************************************************************
**�������ƣ� StartFueling
**�������ܣ���ʼ����,Ԥ����ɺ�ʼ��Һǰ
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-4-26 16:17:52
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void StartFueling(void)
{
    uint32_t  availmoney;     //IC�����ü������ 
    memset(&fuelrecordinfo,0,sizeof(FuelRecordInfo));                         //����ˮ�ṹ�塣
    PumpSwitch(StaFuel);
    globalvar.fuelstep=1;                                                    //��������1��
    globalvar.workstate=StaFuel;                                              //����״̬��
    globalvar.timeout=0;
    globalvar.stoptimes=0;
    stopreason=0;
    globalvar.displayonce=1;    
    fuelrecordinfo.classnum=sysparas.shiftnum;                                //��š�
    memcpy(&fuelrecordinfo.employeenum[0],&shiftrecordinfo.cardstaffnum[0],8);//Ա���š�
    memcpy(&fuelrecordinfo.usernum[0],&m1card.cardnum[0],8);              //�û����š�
    memcpy(&fuelrecordinfo.carnum[0],&m1card.carnum[0],9);                //���ƺš�
    fuelrecordinfo.cardtype=m1card.cardtype;                              //�����͡�

    memcpy(&fuelrecordinfo.stationnum[0],&sysparas.stationcode[0],4);         //վ���롣
    fuelrecordinfo.starttime[0]=0x20;
    memcpy(&fuelrecordinfo.starttime[1], &time.year, 6);                      //������ʼʱ�䡣
    if(m1card.existflag==1)
        fuelrecordinfo.price=sysparas.price[m1card.pricelevel];
    else
        fuelrecordinfo.price=sysparas.price[0];                               //���׼۸�
    realtime.price=fuelrecordinfo.price;
    fuelrecordinfo.beforepressure=realtime.cur_pressure;                      //����ǰѹ����
    fuelrecordinfo.beforebalance=m1card.cardbalance;                      //����ǰ��
    fuelrecordinfo.afterbalance=m1card.cardbalance;                       //��������
    fuelrecordinfo.allrechargemoney=m1card.sumchargemoney;                //�ۼƳ�ֵ���
    fuelrecordinfo.allrechargetimes=m1card.sumchargetimes;                //�ۼƳ�ֵ������
    fuelrecordinfo.allconsumptiontimes=m1card.sumtimes+1;                 //�ۼ����Ѵ�����

    globalvar.starttotalizeL=realtime.cur_totalize_l;    
    globalvar.pretotalL=realtime.cur_totalize_l;
    if(sysparas.backgascalc==1)                                               //��������
    {
        globalvar.starttotalizeG=realtime.cur_totalize_g;
        globalvar.pretotalG=realtime.cur_totalize_g;
    }

    fuelrecordinfo.devicetype=sysparas.devicetype;                                                 //��Ʒ���룬1�ֽڡ�
    if(m1card.cardtype==1)//���������� ycx20161201
    {
        availmoney=m1card.cardbalance-sysparas.cardminyue; 
        if(globalvar.fix==1)//֮ǰ����������
        {
            if(((globalvar.fixnum*fuelrecordinfo.price)/100)>availmoney)
            {
                globalvar.fix=2;
                globalvar.fixmoney=availmoney;
            }
        }
        else if(globalvar.fix==2)
        {
            if(globalvar.fixmoney>availmoney)
            {
                globalvar.fixmoney=availmoney; 
            }
        } 
        else
        {
            globalvar.fix=2;
            globalvar.fixmoney=availmoney; 
        }
    }
    DispUI=DispStartFueling;
    realtime.powerdown=0;                                                     //δ�����־��
}

//��Һ��ʾ
double	 lngvolume=0;//��Һ��
double   cngvolume=0;//������
void DispStartFueling(void)
{
    globalvar.workstate = StaFuel;
    globalvar.fuelstep=2;//��������2��    
    if(sysparas.simulation)
    {
        realtime.cur_totalize_l+=0.11;
        realtime.cur_flowrate_l++;
        realtime.cur_pressure+=1;
        if(realtime.cur_pressure>150)
        {
            realtime.cur_pressure=150;
        }
        if(realtime.cur_flowrate_l>400)
        {
            realtime.cur_flowrate_l=400;
        }
    }
    if(fabs(realtime.cur_totalize_l-globalvar.pretotalL)>2)//Һ�������쳣
    {
        globalvar.KeyValue=KEY_STOP;
        stopreason=3;
    }
    else
    {
        lngvolume=realtime.cur_totalize_l-globalvar.starttotalizeL;
        globalvar.pretotalL=realtime.cur_totalize_l;//���˴�ʵʱ���۸�ֵ�������´��ж�˲ʱ����
    }
    if(sysparas.backgascalc==1)//��������
    {
        if(globalvar.starttotalizeG<0.1)//��ֹ��һ��û�ж������������Ƶ���ʼ��
        {
            globalvar.starttotalizeG=realtime.cur_totalize_g;
        }
        else
        {
            if(fabs(realtime.cur_totalize_g-globalvar.pretotalG)>1)//���������쳣
            {
                globalvar.KeyValue=KEY_STOP;
                stopreason=3;
            }
            else
            {
                cngvolume=realtime.cur_totalize_g-globalvar.starttotalizeG;
                globalvar.pretotalG=realtime.cur_totalize_g;//���˴�ʵʱ���۸�ֵ�������´��ж�˲ʱ����
            }
        }
    }
    else
    {
        cngvolume=0;
    }        
    if(sysparas.unit==1)//���귽����
    {
        lngvolume = lngvolume/((double)sysparas.densty/10000.0);
        cngvolume = cngvolume/((double)sysparas.densty/10000.0);
    }
    lngvolume = (lngvolume-cngvolume)*((double)sysparas.coeff/10000.0);//��ǰ����������
    fuelrecordinfo.backgas=(uint32_t)(cngvolume*100.0);//��ǰ������
    fuelrecordinfo.volume=(uint32_t)(lngvolume*100.0); //��ǰ����������

    lngvolume = (double)fuelrecordinfo.volume/100.0;
    fuelrecordinfo.money=(uint32_t)(lngvolume*realtime.price+0.5);//��ǰ���׽�

    JudgeStopConditions();//�ж�ͣ��������
    realtime.volume=fuelrecordinfo.volume;
    realtime.money=fuelrecordinfo.money;
    Fyd12864DispPrintf(1,1,"״̬: ���ڼ�Һ");
    Fyd12864DispPrintf(2,1,"����:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
    Fyd12864DispPrintf(3,1,"���:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
    Fyd12864DispPrintf(4,1,"V:%.02f P:%ld.%02d",realtime.cur_flowrate_l,realtime.cur_pressure/100,realtime.cur_pressure%100);
    TaskAdd(KEY_STOP,StopFueling, NULL);
    realtime.powerdown=0;          //δ�����־��
    SaveFm25l04();                 //д���硣
    OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,0);
    if(PowerCheckIn!=0)            //�ж��Ƿ���硣
    {
        MsDelay(20);
        if(PowerCheckIn!=0)
        {
            realtime.powerdown=1;      //�ѵ��硣
            SaveFm25l04();             //д���硣
            PowerDown();
        }
    }
}

/***************************************************************************
**�������ƣ� StopFueling
**�������ܣ�ֹͣ����
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-4-26 16:17:52
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void StopFueling()
{
    timer1off();    
    globalvar.fuelstep=3;//��������3��
    if(globalvar.displayonce==1)
    {
        if(globalvar.otherstate==1)//�Է�δ��Һ
        {
            PumpSwitch(StaSPrecool);
        }
        else
        {
            PumpSwitch(StaOtherFuel);
        }
        globalvar.displayonce=0;
        lngvolume=0;                                    //��Һ�����������������㡣
        globalvar.stoppumptime=sysparas.stoppumptime+2; //��Һ��ͣ����ʱ��                
        sysparas.transnum++;
        sysparas.totvolume += fuelrecordinfo.volume;//ǹ��������
        sysparas.totmoney += fuelrecordinfo.money;  //ǹ�ܽ�
        sysparas.shiftotvol+= fuelrecordinfo.volume;//���ۼ�������
        sysparas.shiftotmon+= fuelrecordinfo.money; //���ۼƽ�
        sysparas.shiftfueltimes++;                  //�����������
        fuelrecordinfo.transnum=sysparas.transnum;  //������ˮ�š�
        fuelrecordinfo.allconsumptionnum=m1card.sumvolme+fuelrecordinfo.volume;//�ۼ�����������
        if(m1card.cardtype!=0)
        {
            fuelrecordinfo.transactiontype=0xFF;                        //�������͡�
            fuelrecordinfo.reallymoney=0;                               //����ʵ�ʿۿ��
            fuelrecordinfo.allconsumptionmoney=m1card.summoney;         //�ۼ����ѽ�
        }
        else
        {
            fuelrecordinfo.transactiontype=0;                           //�������͡�
            fuelrecordinfo.reallymoney=fuelrecordinfo.money;            //����ʵ�ʿۿ��
            fuelrecordinfo.allconsumptionmoney=fuelrecordinfo.money;    //�ۼ����ѽ�
        }
        fuelrecordinfo.endtime[0]=0x20;
        memcpy(&fuelrecordinfo.endtime[1], &time.year, 6);              //��������ʱ�䡣

        fuelrecordinfo.allgunnum=sysparas.totvolume;                    //ǹ�����ۼơ�
        fuelrecordinfo.allgunmoney=sysparas.totmoney;                   //ǹ����ۼơ�
        fuelrecordinfo.afterpressure=realtime.cur_pressure;             //������ѹ����
        fuelrecordinfo.curnum=sysparas.coeff;                           //������
        fuelrecordinfo.measuretype=sysparas.unit;                       //������λ��0-�귽��1-Kg��
        fuelrecordinfo.density=realtime.cur_density_l;                  //�ܶȡ�
        fuelrecordinfo.classid=sysparas.shiftransnum;                   //����ˮ�š�
        realtime.stopreason=stopreason;
        fuelrecordinfo.stopreason=realtime.stopreason;                  //ͣ��ԭ��
        fuelrecordinfo.fuelmode=sysparas.fuelmode;
        fuelrecordinfo.crc=CRC16Calculate((uint8_t *)(&fuelrecordinfo.transnum),sizeof(FuelRecordInfo)-2);
        OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,1);
        SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
        m1card.debittimes=10;                                           //�������10�οۿ
    }

    if((m1card.existflag==1)&&(PowerCheckIn==0))  //�ж��Ƿ��ÿ�������δ���硣
    {
        globalvar.workstate=StaPay;
        Fyd12864DispPrintf(1,1,"���ڵȴ��ۿ�..");
        if(m1card.debittimes==0)
        {
            globalvar.displayonce=1;
            DispUI=FuelEndDisplay;
        }
    }
    else if(PowerCheckIn==0)//δ���硣
    {
        globalvar.displayonce=1;
        DispUI=FuelEndDisplay;
    }

    realtime.powerdown=0;          //δ�����־��
    SaveFm25l04();                 //д���硣
    if(PowerCheckIn!=0)            //�ж��Ƿ���硣
    {
        MsDelay(20);
        if(PowerCheckIn!=0)
        {
            realtime.powerdown=1;  //�ѵ��硣
            SaveFm25l04();         //д���硣
            PowerDown();
        }
    }
}


/***************************************************************************
**�������ƣ� FuelEndDisplay
**�������ܣ���Һ��Ϻ�洢��ˮ�ʹ�ӡ
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-4-26 16:17:52
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void FuelEndDisplay(void)
{
    globalvar.workstate=StaIdle;   
    Fyd12864DispPrintf(1,1,"��Һ����!");
    if(globalvar.displayonce==1)
    {
        globalvar.displayonce=0;
        if(SaveFuelRecord(0))
        {
            Fyd12864DispPrintf(4,1,"�洢��ˮ�ɹ�");
        }
        else
        {
            Fyd12864DispPrintf(4,1,"�洢��ˮʧ��");
        }
        if(sysparas.printmode==1)  //�ж��Ƿ��Զ���ӡһ�Ρ�
        {
            Printid();
        }
        memset(&realtime.lcng,0,sizeof(RealTimeData));  //ycx����������ʵʱ���ݡ�
        Fm25l04Write(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData));
        globalvar.MsgFlag=0;
        globalvar.MsgTime=0;//��ֹ��ε��ú���TaskMessage����������
    }    
    Fyd12864DispPrintf(3,1,"%s",stopreasonbuf[stopreason]);
    TaskAdd(KEY_RET, DispFirst, NULL);
    TaskMessage(30, DispFirst);// 3s�󷵻�������
}

void DispCardError(void)
{
    Fyd12864DispPrintf(2,1,"��������");
    if(mt318reader.recbuf[6] == 0x33)
    {
        if(mt318reader.recbuf[4]==KEYA)
            Fyd12864DispPrintf(3,1,"A ����֤ʧ��");
        else if(mt318reader.recbuf[4]==KEYB)
            Fyd12864DispPrintf(3,1,"B ����֤ʧ��");
    }
    if(m1card.existflag==0)//�ο�ֱ�ӷ���������
        DispUI=DispFirst;
    else
        TaskMessage(50, DispFirst); // 5s�󷵻�������
}

/***************************************************************************
**�������ƣ�RemoveStringChar
**�������ܣ�ɾ���ַ���ǰ��������ַ�
**��ڲ�����
                                buf:��Ҫɾ�������ַ����ַ�������
                                ch:��Ҫɾ���������ַ�
                                len:�ַ�������ĳ���
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-11-30
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void RemoveStringChar(uint8_t *buf,uint8_t ch,uint16_t len)
{
    uint8_t *t1,*t2;
    uint16_t i=0,j=0;
    if((len==0)||(buf==NULL)) return;
    if(buf[0]!=ch)  return; //��һ���Ͳ����㣬ֱ�ӷ���
    t1=buf;                 //���ڱ仯����buf
    t2=buf;                 //������ӽ�����
    while(*buf==ch)         //�ҳ�����Ҫɾ�����ַ���һ���ĵ�һ���ַ�
    {
        buf++;
        i++;
    }
    len -= i;               //����buf�����������ݳ���
    j=len;                  //����˳��ȣ���������ַ���������
    while(len--)
    {
        *t1++ = *buf++;
    }
    t2[j]='\0';
}

/***************************************************************************
**�������ƣ�GetCardData
**�������ܣ������Կ�ȶԳɹ����ͻ�õ����ڲ���Ϣ��
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void GetCardData(void)
{
    uint8_t tmp=0;                              //������������
    uint8_t keystr[16];
    uint8_t keykk[8];                           //ThreeDES���ܺ����ݡ�
    uint8_t icnum[8];                           //���ο����кš�
    Fyd12864ClearLine(1);
    Fyd12864ClearLine(4);
    Fyd12864DispPrintf(2,1,"    ������");
    Fyd12864DispPrintf(3,1,"    ���Ժ�...");
    m1card.existflag=1;
    m1card.checkresult=0xFF;                     //����֤�����־��
    globalvar.KeyValue=0;                        //���κΰ���������Ч��
    BufReverse2(&mt318reader.recbuf[6],4);       //��ȡ��Կ��
    memcpy(&icnum[0], &mt318reader.recbuf[6],4);
    memcpy(&icnum[4], &mt318reader.recbuf[6],4); //�õ��������кš�

    At24C512ReadNByte(LOCKVER_ADDR, keystr, 16);
    ThreeDES(&keystr[0],&icnum[0],&keykk[0],1);  // 3DES��������õ�A�ܺ�B�ܡ�
    memcpy(&m1card.keyA[0], &keykk[0],6);                            //A�ܡ�
    memcpy(&m1card.keyB[0], &keykk[2],6);                            //B�ܡ�
    memcpy(&m1card.serialnumber[0], &mt318reader.recbuf[6],4);

    if(!Mt318ReadM1Card(KEYB,&m1card.keyB[0],1, &m1card.cardnum[0]))    //����1��
    {
        tmp++;
    }

    if(tmp==0)
    {
        if(!Mt318ReadM1Card(KEYB,&m1card.keyB[0],2, &m1card.stationcode[0]))    //����2��
        {
            tmp++;
        }
    }

    if(tmp==0)
    {
        if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],4, &m1card.password[0]))   //����4��
        {
            RemoveStringChar(&m1card.customername[0],0,13);
        }
        else tmp++;
    }

    if(tmp==0)
    {
        if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],5,(uint8_t *)&m1card.balancecrc))   //����5��
        {
            BufReverse2((uint8_t *)&m1card.balancecrc+8,4);
            BufReverse2((uint8_t *)&m1card.balancecrc+12,4);
        }
        else tmp++;
    }

    if(tmp==0)
    {
        if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],8,(uint8_t *)&m1card.sumtimes))    //����8��
        {
            StrReverse((uint8_t *)&m1card.sumtimes,3,4);
        }
        else tmp++;
    }

    if(tmp==0)
    {
        if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],9,(uint8_t *)&m1card.sumchargetimes))    //����9��
        {
            StrReverse((uint8_t *)&m1card.sumchargetimes,2,4);
        }
        else tmp++;
    }
    if(tmp==0)
    {
        if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],10,&m1card.cardstate))  //����10��
        {
            StrReverse(&m1card.cardstate+2,1,4);
        }
        else tmp++;
    }

    if(tmp==0)
    {
        if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],12,&m1card.cylindersdate[0]))  //����12��
        {
            RemoveStringChar(&m1card.carnum[0],0,9);
        }
        else tmp++;
    }

    if(tmp==0)                                                 //�����ɹ���
    {
        DispUI=IcCardPasswd;
    }
    else                                                       //����ʧ�ܡ�
    {
        DispUI=DispCardError;
    }
}

/***************************************************************************
**�������ƣ�IcCardPasswd
**�������ܣ������ɹ����жϿ������Ƿ�Ϊ0����Ϊ0����Ҫ�������롣
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void IcCardPasswd(void)
{
    uint8_t str[7];
    uint32_t stationcode1,stationcode2;
    memset(str,0,7);
    str[0]=((m1card.password[0]>>4)+0x30);   //��ѹ��BCD��ת��Ϊ�ַ���
    str[1]=((m1card.password[0]&0x0f)+0x30);
    str[2]=((m1card.password[1]>>4)+0x30);
    str[3]=((m1card.password[1]&0x0f)+0x30);
    str[4]=((m1card.password[2]>>4)+0x30);
    str[5]=((m1card.password[2]&0x0f)+0x30);
    m1card.key=atol((char *)str);               //���ַ�ת��Ϊ���֡�
    stationcode1=BcdbufToHex(m1card.stationcode, 4);
    stationcode2=BcdbufToHex(sysparas.stationcode, 4);
    if((stationcode2==0)||(stationcode1==stationcode2))//վ��ŵ���0ʱ���ж�
    {
       if(sysparas.cardcheck==0)                    //����Ҫ�����ƿ���ڡ�
        {
            if(m1card.key==0)                       //������Ϊ0��ֱ��������
            {
                DispUI=ApplyVerif;
            }
            else                                     //�����벻Ϊ0�����������롣
            {
                DispUI=InputIccardPasswd;
            }
        }
        else if(sysparas.cardcheck==1)               //��Ҫ�����ƿ���ڡ�
        {
            //�жϸ�ƿ�������ڡ�
            if((m1card.cylindersdate[1]>time.year)
               ||((m1card.cylindersdate[1]==time.year)&&(m1card.cylindersdate[2]>time.month))
               ||((m1card.cylindersdate[1]==time.year)&&(m1card.cylindersdate[2]==time.month)&&(m1card.cylindersdate[3]>time.day)))
            {
                if(m1card.key==0)                   //������Ϊ0��ֱ��������
                {
                    DispUI=ApplyVerif;
                }
                else                                 //�����벻Ϊ0�����������롣
                {
                    DispUI=InputIccardPasswd;
                }
            }
            else
            {
                DispUI=DisplayGP;
            }
        } 
    }
    else
    {
        Fyd12864DispPrintf(2,1,"�Ǳ�վ��");
        Fyd12864DispPrintf(3,1,"���˿�");
        if(m1card.existflag==0)
        {
            DispUI=DispFirst;
        }
    }
    
}

/***************************************************************************
**�������ƣ�DisplayGP
**�������ܣ���ʾ��ƿ�Ѿ����ڡ�
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void DisplayGP(void)
{
    Fyd12864ClearLine(4);
    Fyd12864DispPrintf(2,1,"��ƿ�Ѿ�����");
    Fyd12864DispPrintf(3,1,"%02X%02X-%02X-%02X",m1card.cylindersdate[0],m1card.cylindersdate[1],
                       m1card.cylindersdate[2],m1card.cylindersdate[3]);
    TaskAdd(KEY_RET,DispFirst, NULL);
    if(Mt318GetCardSerialNum()!=true)
    {
        DispUI=DispFirst;
    }
}

/***************************************************************************
**�������ƣ�InputIccardPasswd
**�������ܣ����뿨���롣
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void InputIccardPasswd(void)
{
    DispInputPwd("�����뿨����",ComparePasswd);
    if(m1card.existflag==0)
    {
        DispUI=DispFirst;
    }
}

/***************************************************************************
**�������ƣ�ComparePasswd
**�������ܣ��жϿ���������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void ComparePasswd(void)
{
    if(globalvar.U32Temp!=m1card.key)
    {
        Fyd12864DispPrintf(4,1,"�������");
        TaskAdd(KEY_CLR,InputIccardPasswd ,NULL);
        TaskAdd(KEY_RET,InputIccardPasswd ,NULL);
    }
    else
    {
        DispUI=ApplyVerif;
    }
}

//���ӱ�ǩ����ʱ��δ�յ���ǩ��Ϣ������������
void DispScanRFID(void)
{
    Fyd12864ClearLine(1);
    Fyd12864DispPrintf(2,1,"��ɨ����ӱ�ǩ");
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    if(globalvar.startstate!=0xff)//���ӱ�ǩ��������û�б�ǩ���
	{							
		DispUI=BQresults;
	}
    TaskAdd(KEY_RET,DispFirst,NULL);    
}


/***************************************************************************
**�������ƣ�DispCardInfo
**�������ܣ������ɹ�����ʾ����Ϣ���жϿ���;��
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void DispCardInfo(void)
{
#if 0
    WriteICcard();
#endif
    globalvar.timeout=0;
    if((sysparas.offonwork==1)&&(sysparas.yesnolock==0))  //�жϼ������Ƿ������������°�״̬��
    {
        Fyd12864DispPrintf(1,1,"����: %02X%02X%02X%02X",m1card.cardnum[4],m1card.cardnum[5],m1card.cardnum[6],m1card.cardnum[7]);
        Fyd12864DispPrintf(2,1,"���: %ld.%02d",m1card.cardbalance/100,m1card.cardbalance%100);
        Fyd12864DispPrintf(3,1,"����: %s",m1card.customername);
        Fyd12864DispPrintf(4,1,"����: %s",m1card.carnum);
        TaskAdd(KEY_FIX, FixGas,NULL);    //����������
        if(sysparas.fuelmode==0)          //�ֶ�������
        {
            TaskAdd(KEY_START, StartPrecooling, NULL);
            TaskAdd(KEY_BACK,StartPrecooling,DispFirst);
        }
        else if(sysparas.fuelmode==2)     //��ǩ������
        {
            TaskAdd(KEY_START, DispScanRFID,NULL);
            TaskAdd(KEY_BACK,StartPrecooling,DispFirst);
            if(globalvar.startstate!=0xff)//���ӱ�ǩ��������û�б�ǩ���
            {                
                DispUI=BQresults;
            }
        }
    }
    else
    {
        if(sysparas.yesnolock==1)
        {
            Fyd12864DispPrintf(2,1,"����������"); 
            Fyd12864ClearLine(3);
        }
        else if(sysparas.offonwork==0)
        {
            Fyd12864DispPrintf(2,1,"    �°���...");            
            if(m1card.shiftflag==1)//�ǰ������Ҿ��л��๦��
                Fyd12864DispPrintf(3,1,"����ֹͣ+7���ϰ�"); 
            else
                Fyd12864ClearLine(3);
        }
    }
    if((m1card.Shiftfunction == 1)&&(m1card.shiftflag==0))//Ա������
    {
        if(SearchBlackList(4,m1card.cardnum)==1)          //�ж��Ƿ�Ϊ����������
        {   
            m1card.shiftflag=1;            
        }
        else
        {
            Fyd12864DispPrintf(4,1,"�ǰ�������");
        }
    }
    if(m1card.shiftflag==1)
    {
        TaskAdd(KEY_SET, DispInputMenuPwd,NULL);      //���á�
        TaskAdd(KEY_COMB_7KEY,OnWork,NULL);           //ֹͣ+7�ϰ�
        TaskAdd(KEY_COMB_8KEY,OffWork, NULL);         //ֹͣ+8�°�
    }
    if(m1card.existflag==0)//�����γ�
    {
        DispUI=DispFirst;
    }
}


/***************************************************************************
**�������ƣ�ApplyVerif
**�������ܣ����̨��ѯ�˿��Ƿ���á�
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void ApplyVerif(void)
{
    globalvar.startstate=0xff;
    if(m1card.cardstate == 2)
    {
        m1card.greycard=1;
        Fyd12864DispPrintf(2,1,"��������...");
    }
    else
    {
        if(m1card.cardtype==2)//���˿���
        {
            if(m1card.cardbalance<sysparas.cardmaxyue)//�ж�����Ƿ�������ޡ�
            {
                m1card.checkflag=1;
                DispUI=VerificationCard;
                globalvar.displayonce=1;
            }
            else
            {
                Fyd12864ClearLine(1);
                Fyd12864DispPrintf(2,1,"���: %ld.%02d",m1card.cardbalance/100,m1card.cardbalance%100);
                Fyd12864DispPrintf(3,1,"���ޣ��븶��");
                Fyd12864ClearLine(4);
            }
        }
        else if(m1card.cardtype==1)//�����
        {
            if(m1card.cardbalance>sysparas.cardminyue)//�ж�����Ƿ����������ޡ�
            {
                m1card.checkflag=1;
                DispUI=VerificationCard;
                globalvar.displayonce=1;
            }
            else
            {
                Fyd12864ClearLine(1);      
                Fyd12864DispPrintf(2,1,"���: %ld.%02d",m1card.cardbalance/100,m1card.cardbalance%100);
                Fyd12864DispPrintf(3,1,"���㣬���ֵ");
                Fyd12864ClearLine(4);
            }
        }
    }
    if(m1card.existflag==0)//�����γ�
    {
        DispUI=DispFirst;
    }
}


/***************************************************************************
**�������ƣ�VerificationCard
**�������ܣ��ȴ���̨���ز�ѯ�˿��Ƿ���õĽ����
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void VerificationCard(void)
{
    if(globalvar.displayonce==1)
    {
        Fyd12864ClearLine(1);
        Fyd12864ClearLine(4);
        globalvar.displayonce=0;
    }
    if(m1card.checkresult==0)  //��������
    {
        DispUI=DispCardInfo;
        globalvar.displayonce=1;
    }
    else if(m1card.checkresult==0xFF)
    {

        Fyd12864DispPrintf(2,1,"���ڵȴ���֤");
        Fyd12864DispPrintf(3,1,"���Ժ�...");
    }
    else
    {
        Fyd12864DispPrintf(2,1,"��֤ʧ��");
        if(m1card.checkresult==1)
        {
            Fyd12864DispPrintf(3,1,"���󿨺Ų�����");
        }
        else if(m1card.checkresult==2)
        {
            Fyd12864DispPrintf(3,1,"��������");
        }
        else if(m1card.checkresult==3)
        {
            Fyd12864DispPrintf(3,1,"����쳣");
        }
        else if(m1card.checkresult==4)
        {
            Fyd12864DispPrintf(3,1,"�����ʱ���쳣");
        }
    }
    if(m1card.existflag==0) DispUI=DispFirst; //�����γ�
}

/***************************************************************************
**�������ƣ�GreyLock
**�������ܣ�����˿����ã��򽫿��ûҡ�
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void GreyLock(void)
{
    uint8_t tmpbuffer[16];
    m1card.cardstate=2;//�ûҡ�
    m1card.gunnum=sysparas.gunnum;
    m1card.transnum=sysparas.transnum+1;
    memcpy(&m1card.laststationcode[0],&sysparas.stationcode[0],4);
    memcpy(&tmpbuffer[0],&m1card.cardstate,16);
    StrReverse(&tmpbuffer[2],1,4);

    if(Mt318WriteM1Card(KEYB,&m1card.keyB[0],10,&tmpbuffer[0]))//д��
    {
        DispUI=StartFueling;
    }
    else
    {
        Fyd12864DispPrintf(2,1,"�û�ʧ�ܣ�");
        TaskMessage(10, DispFirst);// 1s�󷵻�������
    }
}

/***************************************************************************
**�������ƣ�CheckBlackorWhite
**�������ܣ���ѯ�˿��Ƿ�Ϊ�ڰ׿���
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void CheckBlackorWhite(void)
{
    Fyd12864ClearLine(1);
    Fyd12864ClearLine(4);
    Fyd12864DispPrintf(2,1,"2.��ѯ�Ƿ�ڿ�");
    Fyd12864DispPrintf(3,1,"3.��ѯ�Ƿ�׿�");
    TaskAdd(KEY_2, GetBlack, NULL);
    TaskAdd(KEY_3, GetWhite, NULL);
    globalvar.displayonce=1;
}

/***************************************************************************
**�������ƣ�GetBlack
**�������ܣ���ѯ�˿��Ƿ�Ϊ�ڿ���
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void GetBlack(void)
{
    if(globalvar.displayonce==1)
    {
        Fyd12864ClearLine(2);
        Fyd12864ClearLine(3);
        Fyd12864DispPrintf(2,1,"���ڵȴ���ѯ..");
        globalvar.displayonce=0;
        UpLoad56H(0);
    }

    if(m1card.blackorwhite==1)
    {
        m1card.blackorwhite=0;
        Fyd12864DispPrintf(2,1,"�˿����Ǻڿ�");
    }
    else if(m1card.blackorwhite==2)
    {
        m1card.blackorwhite=0;
        Fyd12864DispPrintf(2,1,"�˿����Ǻڿ�");
    }
    TaskAdd(KEY_RET, DispFirst, NULL);
}

/***************************************************************************
**�������ƣ�GetWhite
**�������ܣ���ѯ�˿��Ƿ�Ϊ�׿���
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void GetWhite(void)
{
    if(globalvar.displayonce==1)
    {
        Fyd12864ClearLine(2);
        Fyd12864ClearLine(3);
        globalvar.displayonce=0;
        Fyd12864DispPrintf(2,1,"���ڵȴ���ѯ..");
        UpLoad56H(1);
    }
    if(m1card.blackorwhite==1)
    {
        m1card.blackorwhite=0;
        Fyd12864DispPrintf(2,1,"�˿����ǰ׿�");
    }
    else if(m1card.blackorwhite==2)
    {
        m1card.blackorwhite=0;
        Fyd12864DispPrintf(2,1,"�˿����ǰ׿�");
    }
    TaskAdd(KEY_RET, DispFirst, NULL);
}

/***************************************************************************
**�������ƣ�PowerDown
**�������ܣ�������ʾ������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void PowerDown(void)
{
    PumpSwitch(StaClose);                                               //ͣ����
    Fyd12864DispPrintf(1,1,"ϵͳ�Ѿ�����...");
    Fyd12864DispPrintf(4,1,"�����ѱ���");
    DispUI=PowerDown;
    TaskAdd(KEY_RET, SoftReset,NULL);
}

/***************************************************************************
**�������ƣ�SaveFm25l04
**�������ܣ����籣�����ݺ�����
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void SaveFm25l04(void)
{
    realtime.lcng = sysparas.devicetype;                                                  //��Ʒ���롣
    if(globalvar.fuelstep==3)
    {
        realtime.transnum=sysparas.transnum;                            //��ǰ��ˮ�š�
    }
    else
    {
        realtime.transnum=sysparas.transnum+1;                          //��ǰ��ˮ�š�
    }
    realtime.price=fuelrecordinfo.price;                                //���ۡ�
    realtime.cardtype=m1card.cardtype;                                  //�����͡�
    realtime.shiftnum=sysparas.shiftnum;                                //��š�
    memcpy(&realtime.employeenum[0],&shiftrecordinfo.cardstaffnum[0],8);//Ա���š�
    memcpy(&realtime.cardnum[0],&m1card.cardnum[0],8);                  //�û����š�
    memcpy(&realtime.carnum[0],&m1card.carnum[0],9);                    //���ƺš�
    memcpy(&realtime.starttime[0], &fuelrecordinfo.starttime[0], 7);    //������ʼʱ�䡣
    realtime.endtime[0]=0x20;
    memcpy(&realtime.endtime[1], &time.year, 6);                        //��������ʱ�䡣
    realtime.beforepressure=fuelrecordinfo.beforepressure;              //����ǰѹ����
    realtime.afterpressure=realtime.cur_pressure;                       //������ѹ����
    realtime.beforebalance=fuelrecordinfo.beforebalance;                //����ǰ��
    realtime.afterbalance=fuelrecordinfo.afterbalance;                  //��������
    realtime.allrechargetimes=fuelrecordinfo.allrechargetimes;          //�ۼƳ�ֵ������
    realtime.allrechargemoney=fuelrecordinfo.allrechargemoney;          //�ۼƳ�ֵ��
    realtime.allconsumptiontimes=fuelrecordinfo.allconsumptiontimes;    //�ۼ����Ѵ�����
    realtime.backgas=fuelrecordinfo.backgas;                            //������
    realtime.volume=fuelrecordinfo.volume;                              //������
    realtime.money=fuelrecordinfo.money;                                //��
    realtime.allconsumptionnum=fuelrecordinfo.volume+m1card.sumvolme;   //�ۼ�����������
    realtime.allconsumptionmoney=m1card.summoney;  //�ۼ����ѽ����ں�̨��Ҽ���ʱ�Ѿ�����һ�Σ����Լ��������治��Ҫ����

    if(m1card.existflag==1)
    {
        if((globalvar.fuelstep==1)||(globalvar.fuelstep==2))         //����ڼ�������1����2��
        {
            realtime.reallymoney=0;                                  //ʵ�۽�
            realtime.transactiontype=0xFF;                           //�������͡�
        }
        else if(globalvar.fuelstep==3)                               //����ڼ�������3.
        {
            realtime.reallymoney=fuelrecordinfo.reallymoney;           //ʵ�۽�
            realtime.transactiontype=fuelrecordinfo.transactiontype;   //�������͡�
        }
    }
    else
    {
        realtime.reallymoney=fuelrecordinfo.money;                      //ʵ�۽�
        realtime.transactiontype=0x0;                                   //�������͡�
    }
    realtime.curnum=sysparas.coeff;                                     //������
    realtime.measuretype=sysparas.unit;                                 //������λ��
    realtime.stopreason=5;                                              //ͣ��ԭ��
    realtime.jhid=0;                                                    //��������ˮ�š�
    realtime.classid=sysparas.shiftransnum;                             //����ˮ�š�
    realtime.fuelmode=sysparas.fuelmode;
    Fm25l04Write(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData));   //�洢���硣

}

/***************************************************************************
**�������ƣ�PowerDownRealDate
**�������ܣ��������ݻָ�������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void PowerDownRealDate(void)
{
    if(realtime.workstatus==StaFuel)
    {
        sysparas.transnum=realtime.transnum;                            //��ˮ�š�
        sysparas.totvolume+=realtime.volume;                            //ǹ��������
        sysparas.totmoney+=realtime.money;                              //ǹ�۽�
        sysparas.shiftotvol+=realtime.volume;                           //����������
        sysparas.shiftotmon+=realtime.money;                            //����������
        sysparas.shiftfueltimes++;                                      //�����������
        SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas));
    }

    fuelrecordinfo.transnum=realtime.transnum;                          //��ˮ�š�
    fuelrecordinfo.classnum=sysparas.shiftnum;                          //��š�;
    memcpy(&fuelrecordinfo.employeenum[0],&realtime.employeenum[0],8);  //Ա���š�
    fuelrecordinfo.transactiontype=realtime.transactiontype;            //�ӿ����ס�
    memcpy(&fuelrecordinfo.usernum[0],&realtime.cardnum[0],8);          //�û����š�
    fuelrecordinfo.cardtype=realtime.cardtype;                          //�����͡�
    memcpy(&fuelrecordinfo.carnum[0],&realtime.carnum[0],9);            //���ƺš�
    memcpy(&fuelrecordinfo.stationnum[0],&sysparas.stationcode[0],4);   //վ���롣
    memcpy(&fuelrecordinfo.starttime[0],&realtime.starttime[0],7);      //������ʼʱ�䡣
    memcpy(&fuelrecordinfo.endtime[0],&realtime.endtime[0],7);          //��������ʱ�䡣

    fuelrecordinfo.price=realtime.price;                                //���ۡ�
    fuelrecordinfo.volume=realtime.volume;                              //������
    fuelrecordinfo.money=realtime.money;                                //��
    fuelrecordinfo.backgas=realtime.backgas;                            //������
    fuelrecordinfo.reallymoney=realtime.reallymoney;                    //ʵ�ʿۿ��
    fuelrecordinfo.beforebalance=realtime.beforebalance;                //����ǰ��
    fuelrecordinfo.afterbalance=realtime.afterbalance;                  //��������
    fuelrecordinfo.allrechargetimes=realtime.allrechargetimes;          //�ۼƳ�ֵ������


    fuelrecordinfo.allrechargemoney=realtime.allrechargemoney;          //�ۼƳ�ֵ��

    fuelrecordinfo.allconsumptiontimes=realtime.allconsumptiontimes;    //�ۼ����Ѵ�����
    fuelrecordinfo.allconsumptionnum=realtime.allconsumptionnum;        //�ۼ�����������
    fuelrecordinfo.allconsumptionmoney=realtime.allconsumptionmoney;    //�ۼ����ѽ�
    fuelrecordinfo.allgunnum=sysparas.totvolume;                        //ǹ��������
    fuelrecordinfo.allgunmoney=sysparas.totmoney;                       //ǹ�۽�
    fuelrecordinfo.beforepressure=realtime.beforepressure;              //����ǰѹ����
    fuelrecordinfo.afterpressure=realtime.afterpressure;                //������ѹ����
    fuelrecordinfo.density=(uint32_t)(realtime.cur_density_g*10000+0.5);//�ܶȡ�
    fuelrecordinfo.curnum=realtime.curnum;                              //������
    fuelrecordinfo.devicetype=realtime.lcng;                               //��Ʒ���롣
    fuelrecordinfo.measuretype=realtime.measuretype;                    //������λ��
    fuelrecordinfo.stopreason=realtime.stopreason;                      //ͣ��ԭ��
    fuelrecordinfo.jhid=realtime.jhid;                                  //��������ˮ�š�
    fuelrecordinfo.classid=realtime.classid;                            //����ˮ�š�
    fuelrecordinfo.fuelmode=realtime.fuelmode;
    fuelrecordinfo.crc=CRC16Calculate((uint8_t *)(&fuelrecordinfo.transnum),sizeof(FuelRecordInfo)-2);
   
    MsDelay(100);
    if(SaveFuelRecord(0))
    {
        Fyd12864DispPrintf(4,1,"�洢��ˮ�ɹ�");
    }
    else
    {
        Fyd12864DispPrintf(4,1,"�洢��ˮʧ��");
    }
}

/***************************************************************************
**�������ƣ�FixGas
**�������ܣ�����ѡ������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void FixGas(void)
{
    Fyd12864ClearLine(1);
    Fyd12864ClearLine(4);
    Fyd12864DispPrintf(2,1,"  1.������");
    Fyd12864DispPrintf(3,1,"  2.�����");

    TaskAdd(KEY_RET, DispFirst, NULL);
    TaskAdd(KEY_1, FixNum, NULL);
    TaskAdd(KEY_2, FixMoney, NULL);
    if((m1card.cardtype!=0)&&(m1card.existflag==0))//�����γ�
    {
        DispUI=DispFirst;
    }
    globalvar.fixnum=0;
    globalvar.fixmoney=0;
    globalvar.displayonce=1;
    InputInit();
}

/***************************************************************************
**�������ƣ�FixNum
**�������ܣ�������������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void FixNum(void)
{
    char buf[16]= {0};
    ScreenInput.inputmaxlen=5;
    NumKeyHandler();
    globalvar.fix=1;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    globalvar.fixnum=atol(buf);
    Fyd12864ClearLine(3);
    Fyd12864DispPrintf(2,1,"������:%ld.%02d",globalvar.fixnum/100,globalvar.fixnum%100);
    TaskAdd(KEY_RET, DispFirst, NULL);
    if((m1card.existflag==1)&&(m1card.checkresult==0))
    {
        if(sysparas.fuelmode==0)                        //�ֶ�������
        {
            TaskAdd(KEY_OK, StartPrecooling, NULL);
        }
        if(sysparas.fuelmode==2)                        //��ǩ������
        {
            TaskAdd(KEY_OK, WaitBQresults, NULL);
        }

    }
    else
    {
        if((sysparas.fuelmode==0)&&(sysparas.usecard==0)) //�ֶ�������
        {
            TaskAdd(KEY_OK, StartPrecooling,StartFueling);
        }
        if(sysparas.fuelmode==2)                        //��ǩ������
        {
            TaskAdd(KEY_OK, WaitBQresults, NULL);
        }

    }

}

/***************************************************************************
**�������ƣ�FixMoney
**�������ܣ���������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void FixMoney(void)
{
    char buf[16]= {0};
    ScreenInput.inputmaxlen=5;
    NumKeyHandler();
    globalvar.fix=2;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    globalvar.fixmoney=atol(buf);
    Fyd12864ClearLine(3);
    Fyd12864DispPrintf(2,1,"�����:%ld.%02d",globalvar.fixmoney/100,globalvar.fixmoney%100);
    TaskAdd(KEY_RET, DispFirst, NULL);
    if((m1card.existflag==1)&&(m1card.checkresult==0))
    {
        if(sysparas.fuelmode==0)                          //�ֶ�������
        {
            TaskAdd(KEY_OK, StartPrecooling, NULL);
        }
        if(sysparas.fuelmode==2)                          //��ǩ������
        {
            TaskAdd(KEY_OK, WaitBQresults, NULL);
        }

    }
    else
    {
        if((sysparas.fuelmode==0)&&(sysparas.usecard==0)) //�ֶ�������
        {
            TaskAdd(KEY_OK, StartPrecooling,StartFueling);
        }
        if(sysparas.fuelmode==2)                          //��ǩ������
        {
            TaskAdd(KEY_OK, WaitBQresults, NULL);
        }
    }
}

/***************************************************************************
**�������ƣ�DisPlayLock
**�������ܣ����ܰ���ʾ������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void DisPlayLock(void)
{
    Fyd12864ClearLine(1);
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    if(SI_INDATE==0)//����״̬��
    {
        Fyd12864DispPrintf(2,1,"������״̬");
    }
    else
    {
        DispUI=DispFirst;
    }
}

/***************************************************************************
**�������ƣ�SearchBlackList
**�������ܣ���Flash��Ѱ���Ƿ�����Ӧ�ĺڰ��������š�
**��ڲ�����type:1-��ɾ������
                                            2-����������
                                            3-����������
                                            4-������
                                   cardNo:��Ҫ���ҵĿ���
**�� �� ֵ��1-�У�0-�ޡ�
**�� �� �ߣ����
**�������ڣ�2016-11-30 16:03:19
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t SearchBlackList(uint8_t type,uint8_t *cardNo)
{
    uint32_t  addr,value;
    uint8_t   readAarray[8];
    int       low,mid,high,result;
    if(type==0x01)     //��ɾ������
    {
        addr=FlashDelBlkSADDR;
        value=sysparas.blackdelnum;
    }
    else if(type==0x02)//����������
    {
        addr=FlashAddBlkSADDR;
        value=sysparas.blackaddnum;
    }
    else if(type==0x03)//����������
    {
        addr=FlashBlkSADDR;
        value=sysparas.blacknum;
    }
    else if(type==0x04)//������
    {
        addr=FlashWhiteSADDR;
        value=sysparas.whitenum;
    }
    low=0;
    mid=0;
    high=value-1;
    while(low<=high)
    {
        mid=(low+high)/2;
        memset(&readAarray[0],0,sizeof(readAarray));
        ReadFlash(0,addr+mid*8, &readAarray[0], 8);
        result=memcmp(&cardNo[0],&readAarray[0],8);
        if(result>0)
        {
            low=mid+1;
        }
        else if(result<0)
        {
            high=mid-1;
        }
        else
        {
            return 1;
        }
        if(high<0)
        {
            return 0;
        }
    }
    return 0;
}


/***************************************************************************
**�������ƣ�BcdToStr
**�������ܣ���BCD��ת��Ϊ�ַ�����
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-8-18
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void BcdToStr(uint8_t bcd[],uint8_t str[])
{
    str[0]=((bcd[0]>>4)+0x30);
    str[1]=((bcd[0]&0x0f)+0x30);
    str[2]=((bcd[1]>>4)+0x30);
    str[3]=((bcd[1]&0x0f)+0x30);
    str[4]=((bcd[2]>>4)+0x30);
    str[5]=((bcd[2]&0x0f)+0x30);
    str[6]=((bcd[3]>>4)+0x30);
    str[7]=((bcd[3]&0x0f)+0x30);
}


