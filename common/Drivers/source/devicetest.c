#include "kernel.h"

void ValveTestLow(void);
void ValveTestMid(void);
void ValveTestHigh(void);
void ValveTestAll(void);
void MassmeterTest(void);

/********************************************************
**��������:KeyBoardTest0
**��������:����0����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest0(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���:0");
}
/********************************************************
**��������:KeyBoardTest0
**��������:����1����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest1(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���:1");
}
/********************************************************
**��������:KeyBoardTest0
**��������:����2����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest2(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���:2");
}
/********************************************************
**��������:KeyBoardTest0
**��������:����3����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest3(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���:3");
}
/********************************************************
**��������:KeyBoardTest0
**��������:����4����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest4(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���:4");
}
/********************************************************
**��������:KeyBoardTest0
**��������:����5����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest5(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���:5");
}
/********************************************************
**��������:KeyBoardTest0
**��������:����6����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest6(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���:6");
}
/********************************************************
**��������:KeyBoardTest0
**��������:����7����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest7(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���:7");
}
/********************************************************
**��������:KeyBoardTest0
**��������:����8����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest8(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���:8");
}
/********************************************************
**��������:KeyBoardTest0
**��������:����9����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest9(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���:9");
}
/********************************************************
**��������:KeyBoardTestPoint
**��������:С�������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTestPoint(void)
{
    Fyd12864DispPrintf(3,1,"���ְ���: С����");
}
/********************************************************
**��������:KeyBoardTestQuery
**��������:��ѯ��������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTestQuery(void)
{
    Fyd12864DispPrintf(3,1,"���ܰ���: ��ѯ");
}
/********************************************************
**��������:KeyBoardTestMenu
**��������:�˵���������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTestMenu(void)
{
    Fyd12864DispPrintf(3,1,"���ܰ���: �˵�");
}
/********************************************************
**��������:KeyBoardTestRation
**��������:������������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTestRation(void)
{
    Fyd12864DispPrintf(3,1,"���ܰ���: ����");
}
/********************************************************
**��������:KeyBoardTestClear
**��������:�����������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTestClear(void)
{
    Fyd12864DispPrintf(3,1,"���ܰ��������");
}
/********************************************************
**��������:KeyBoardTestStop
**��������:ֹͣ��������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTestStop(void)
{
    Fyd12864DispPrintf(3,1,"���ܰ�����ֹͣ");
}
/********************************************************
**��������:KeyBoardTestReplay
**��������:���԰�������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTestReplay(void)
{
    Fyd12864DispPrintf(3,1,"���ܰ���: ����");
}
/********************************************************
**��������:KeyBoardTestEnter
**��������:ȷ�ϰ�������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTestEnter(void)
{
    Fyd12864DispPrintf(3,1,"���ܰ�����ȷ��");
}
/********************************************************
**��������:KeyBoardTestStart
**��������:������������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTestStart(void)
{
    Fyd12864DispPrintf(3,1,"���ܰ���������");
}
/********************************************************
**��������:KeyBoardTestSub
**��������:��������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTestSub(void)
{
    Fyd12864DispPrintf(1,1,"    ��������");
    Fyd12864DispPrintf(2,1,"    �밴����");
    Fyd12864DispPrintf(4,1,"�����ء�ֹͣ����");
    TaskAdd(KEY_RET,DeviceTestMenu,NULL);
    switch(globalvar.KeyValue)
    {
        case KEY_0:
            KeyBoardTest0();
            break;
        case KEY_1:
            KeyBoardTest1();
            break;
        case KEY_2:
            KeyBoardTest2();
            break;
        case KEY_3:
            KeyBoardTest3();
            break;
        case KEY_4:
            KeyBoardTest4();
            break;
        case KEY_5:
            KeyBoardTest5();
            break;
        case KEY_6:
            KeyBoardTest6();
            break;
        case KEY_7:
            KeyBoardTest7();
            break;
        case KEY_8:
            KeyBoardTest8();
            break;
        case KEY_9:
            KeyBoardTest9();
            break;
        case KEY_PT:
            KeyBoardTestPoint();
            break;
        case KEY_QRY:
            KeyBoardTestQuery();
            break;
        case KEY_SET:
            KeyBoardTestMenu();
            break;
        case KEY_FIX:
            KeyBoardTestRation();
            break;
        case KEY_BACK:
            KeyBoardTestReplay();
            break;
        case KEY_CLR:
            KeyBoardTestClear();
            break;
        case KEY_START:
            KeyBoardTestStart();
            break;
        case KEY_STOP:
            KeyBoardTestStop();
            break;
        case KEY_OK:
            KeyBoardTestEnter();
            break;
        default:
            break;
    }
}
/********************************************************
**��������:KeyBoardTest
**��������:��������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void KeyBoardTest(void)
{
    Fyd12864ClearScreen();
    DispUI=KeyBoardTestSub;
}

/********************************************************
**��������:ValveTestStop
**��������:��ȫ���ر�
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void ValveTestStop(void)
{
    VCTRL_MID_CLOSE();
    UsDelay(10);
    VCTRL_HIGH_CLOSE();
    UsDelay(10);
    VCTRL_LOW_CLOSE();
    UsDelay(10);
    VCTRL_CLOSEKEY();
    UsDelay(10);
    DispUI=DeviceTestMenu;
}

/********************************************************
**��������:ValveTestLow
**��������:��ѹ����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void ValveTestLow(void)
{
    Fyd12864DispPrintf(2,1,"��  ��  ��  ȫ");
    Fyd12864DispPrintf(3,1,"��  ��  ��  ��");
    TaskAdd(KEY_2,ValveTestMid,NULL);
    TaskAdd(KEY_3,ValveTestHigh,NULL);
    TaskAdd(KEY_4,ValveTestAll,NULL);
    TaskAdd(KEY_STOP,ValveTestStop,NULL);
    VCTRL_OPENKEY();
    UsDelay(10);
    VCTRL_MID_CLOSE();
    UsDelay(10);
    VCTRL_HIGH_CLOSE();
    UsDelay(10);
    VCTRL_LOW_OPEN();
    UsDelay(10);
}
/********************************************************
**��������:ValveTestMid
**��������:��ѹ����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void ValveTestMid(void)
{
    Fyd12864DispPrintf(2,1,"��  ��  ��  ȫ");
    Fyd12864DispPrintf(3,1,"��  ��  ��  ��");
    TaskAdd(KEY_1,ValveTestLow,NULL);
    TaskAdd(KEY_3,ValveTestHigh,NULL);
    TaskAdd(KEY_4,ValveTestAll,NULL);
    TaskAdd(KEY_STOP,ValveTestStop,NULL);
    VCTRL_OPENKEY();
    UsDelay(10);
    VCTRL_LOW_CLOSE();
    UsDelay(10);
    VCTRL_HIGH_CLOSE();
    UsDelay(10);
    VCTRL_MID_OPEN();
    UsDelay(10);
}
/********************************************************
**��������:ValveTestHigh
**��������:��ѹ����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void ValveTestHigh(void)
{
    Fyd12864DispPrintf(2,1,"��  ��  ��  ȫ");
    Fyd12864DispPrintf(3,1,"��  ��  ��  ��");
    TaskAdd(KEY_1,ValveTestLow,NULL);
    TaskAdd(KEY_2,ValveTestMid,NULL);
    TaskAdd(KEY_4,ValveTestAll,NULL);
    TaskAdd(KEY_STOP,ValveTestStop,NULL);
    VCTRL_OPENKEY();
    UsDelay(10);
    VCTRL_MID_CLOSE();
    UsDelay(10);
    VCTRL_HIGH_OPEN();
    UsDelay(10);
    VCTRL_LOW_CLOSE();
    UsDelay(10);
}
/********************************************************
**��������:ValveTestAll
**��������:��ȫ����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void ValveTestAll(void)
{
    Fyd12864DispPrintf(2,1,"��  ��  ��  ȫ");
    Fyd12864DispPrintf(3,1,"��  ��  ��  ��");
    TaskAdd(KEY_1,ValveTestLow,NULL);
    TaskAdd(KEY_2,ValveTestMid,NULL);
    TaskAdd(KEY_3,ValveTestHigh,NULL);
    TaskAdd(KEY_STOP,ValveTestStop,NULL);
    VCTRL_OPENKEY();
    UsDelay(10);
    VCTRL_LOW_OPEN();
    UsDelay(10);
    VCTRL_MID_OPEN();
    UsDelay(10);
    VCTRL_HIGH_OPEN();
    UsDelay(10);
}

/********************************************************
**��������:ValveTest
**��������:��ŷ�����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void ValveTest(void)
{
    globalvar.devicetest=1;
    Fyd12864DispPrintf(1,1,"  ��ŷ�����");
    Fyd12864DispPrintf(2,1,"��  ��  ��  ȫ");
    Fyd12864DispPrintf(3,1,"��  ��  ��  �� ");
    Fyd12864DispPrintf(4,1,"��ֹͣ��ֹͣ����");
    TaskAdd(KEY_1,ValveTestLow,NULL);
    TaskAdd(KEY_2,ValveTestMid,NULL);
    TaskAdd(KEY_3,ValveTestHigh,NULL);
    TaskAdd(KEY_4,ValveTestAll,NULL);
    TaskAdd(KEY_STOP,ValveTestStop,NULL);
}
/********************************************************
**��������:PressureWork
**��������:ѹ������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void PressureWork(void)
{
    uint16_t pzero;
    uint16_t pratio;
    uint32_t curad;
    pzero=sysparas.pzero;
    pratio=sysparas.pcoeff;
    if(pzero==0)
    {
        Fyd12864DispPrintf(2,1,"ѹ������");
        Fyd12864DispPrintf(4,1,"�밴�����ء�����");
    }
    else
    {
        Fyd12864DispPrintf(2,1,"���:%d",pzero);
        curad=PlADCGetValue();
        Fyd12864DispPrintf(3,1,"��ǰADֵ:%d",curad);
        curad=curad;
        pzero=pzero;
        if(curad<=pzero)
        {
            curad=0;
        }
        else
        {
            curad=curad-pzero;
        }
        Fyd12864DispPrintf(4,1,"��ǰѹ��:%.02f",(double)curad/pratio);
    }
    TaskAdd(KEY_RET,DeviceTestMenu,NULL);
}
/********************************************************
**��������:PressureTest
**��������:ѹ������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void PressureTest(void)
{
    Fyd12864ClearScreen();
    Fyd12864DispPrintf(1,1,"    ѹ������");
    DispUI=PressureWork;
}
/********************************************************
**��������:GasMassmeterTest
**��������:���������Ʋ���
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void GasMassmeterTest(void)
{
    uint8_t  result;
    uint16_t deviceadd;
    Fyd12864DispPrintf(1,1,"  ����������");
    Fyd12864ClearLine(2);
    result=ModbusReadDeviceAddress(_MODBUS_G_,&deviceadd);
    if(result==TRUE)
    {
        if(deviceadd==0x01)
        {
            Fyd12864DispPrintf(3,1,"  ��������������");
        }
        else
        {
            Fyd12864DispPrintf(3,1,"  �����������쳣");
        }
    }
    else
    {
        Fyd12864DispPrintf(3,1,"  ��������ʧ��");
    }
    Fyd12864DispPrintf(4,1,"�밴�����ء�����");
    TaskAdd(KEY_RET, MassmeterTest,NULL);
}
/********************************************************
**��������:LiquidMassmeterTest
**��������:Һ�������Ʋ���
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void LiquidMassmeterTest(void)
{
    uint8_t  result;
    uint16_t deviceadd;
    Fyd12864DispPrintf(1,1,"  Һ��������");
    Fyd12864ClearLine(2);
    result=ModbusReadDeviceAddress(_MODBUS_L_,&deviceadd);
    MsDelay(100);
    if(result==TRUE)
    {
        if(deviceadd==0x02)
        {
            Fyd12864DispPrintf(3,1,"  ��������������");
        }
        else
        {
            Fyd12864DispPrintf(3,1,"  �����������쳣");
        }
    }
    else
    {
        Fyd12864DispPrintf(3,1,"  ��������ʧ��");
    }
    Fyd12864DispPrintf(4,1,"�밴�����ء�����");
    TaskAdd(KEY_RET, MassmeterTest,NULL);
}
/********************************************************
**��������:MassmeterTest
**��������:�����Ʋ���
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void MassmeterTest(void)
{
    Fyd12864DispPrintf(1,1,"  �����Ʋ���");
    Fyd12864DispPrintf(2,1,"1.����������");
    Fyd12864DispPrintf(3,1,"2.Һ��������");
    Fyd12864DispPrintf(4,1,"�밴�����ء�����");
    TaskAdd(KEY_RET, DeviceTestMenu,NULL);
    TaskAdd(KEY_1,GasMassmeterTest,NULL);
    TaskAdd(KEY_2,LiquidMassmeterTest,NULL);
}
/********************************************************
**��������:PrinterTest
**��������:��ӡ������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void PrinterTest(void)
{
    SELECT_PRINT();
    WHPrinterHead();
    WHPrintf("       ��ӡ������\r\n");
    WHPrinterFreeRow(1);
    WHPrintf("---------------------------\r\n");
    WHPrinterNormalCmd();
    WHPrintf("   ллʹ�ý��ǲ�Ʒ\r\n");
    WHPrinterFreeRow(6);
    SELECT_CARD();
    DispUI=DeviceTestMenu;
}

//�������
void SoftReset(void)
{
    (*(void(*)(void))0x00)();
}

//ִ���ⲿeeprom�����ڲ�
void CopyExToIn(void)
{
    uint32_t    addr=0,num,i;
    uint8_t     tmp1[100],tmp2[100],ret=FALSE;
    num=sizeof(tmp1);
    if(globalvar.displayonce==1)
    {
        while(1)
        {   
            WATCH_DOG();
            if(At24C512ReadNByte(addr, tmp1, num))
            {
                if(EepromInterWriteNByte(addr, tmp1, num))
                {
                    if(EepromInterReadNByte(addr, tmp2, num))
                    {
                        for(i=0;i<num;i++)
                        {
                            if(tmp1[i]!=tmp2[i]) break;
                        }
                        if(i==num) ret=TRUE;
                        else ret=FALSE;
                    }
                    else
                    {
                        ret=FALSE;
                    }                    
                }
                else
                {
                    ret=FALSE;
                }
            }
            else
            {
                ret=FALSE;
            }
            addr+=num;
            if(addr>=4000) break;
            if(ret==FALSE) break;
        }
        globalvar.displayonce=0;
        if(ret==TRUE)   SetSuccess();  
        else    SetFault();
    }    
    TaskMessage(10,DispEepromCopy);
}

//�ⲿ�����ڲ�ѯ��
void DispCopyExToIn(void)
{
    Fyd12864DispPrintf(1,1,"�Ƿ񿽱�  ��->��");
    Fyd12864DispPrintf(2,1,"��ȷ�ϡ�  ����");
    Fyd12864DispPrintf(3,1,"�����ء�  ȡ��");
    Fyd12864ClearLine(4); 
    TaskAdd(KEY_OK,CopyExToIn,NULL);
    TaskAdd(KEY_RET,DispEepromCopy ,NULL);
}


//ִ���ڲ�eeprom�����ⲿ
void CopyInToEx(void)
{
    uint32_t    addr=0,num,i;
    uint8_t     tmp1[100],tmp2[100],ret=FALSE;
    num=sizeof(tmp1);
    if(globalvar.displayonce==1)
    {
        while(1)
        {         
            WATCH_DOG();
            if(EepromInterReadNByte(addr, tmp1, num))
            {
                if(At24C512WriteNByte(addr, tmp1, num))
                {
                    if(At24C512ReadNByte(addr, tmp2, num))
                    {
                        for(i=0;i<num;i++)
                        {
                            if(tmp1[i]!=tmp2[i]) break;
                        }
                        if(i==num) ret=TRUE;
                        else ret=FALSE;
                    }
                    else
                    {
                        ret=FALSE;
                    }                    
                }
                else
                {
                    ret=FALSE;
                }
            }
            else
            {
                ret=FALSE;
            }
            addr+=num;
            if(addr>=4000) break;
            if(ret==FALSE) break;
        }
        globalvar.displayonce=0;
		if(ret==TRUE)   SetSuccess();  
		else    SetFault();
    }    
    TaskMessage(10,DispEepromCopy);
}

//�ڲ������ⲿѯ��
void DispCopyInToEx(void)
{
    Fyd12864DispPrintf(1,1,"�Ƿ񿽱�  ��->��");
    Fyd12864DispPrintf(2,1,"��ȷ�ϡ�  ����");
    Fyd12864DispPrintf(3,1,"�����ء�  ȡ��");
    Fyd12864ClearLine(4); 
    TaskAdd(KEY_OK,CopyInToEx,NULL);
    TaskAdd(KEY_RET,DispEepromCopy ,NULL);
}

//eeprom ����
void DispEepromCopy(void)
{
    globalvar.displayonce=1;
    Fyd12864DispPrintf(1,1,"    EEPROM����  ");
    Fyd12864DispPrintf(2,1,"1.���ڲ������ⲿ");
    Fyd12864DispPrintf(3,1,"2.���ⲿ�����ڲ�");
    Fyd12864ClearLine(4);    
    TaskAdd(KEY_1,DispCopyInToEx,NULL);
    TaskAdd(KEY_2,DispCopyExToIn,NULL);
    TaskAdd(KEY_RET, DeviceTestMenu,NULL);
}

//���Բ���
void DispLCDTest(void)
{
    Fyd12864ClearLine(1);
    Fyd12864DispPrintf(2,1,"    ���Բ���  ");
    Fyd12864DispPrintf(3,1,"ֹͣ�밴�����ء�");
    Fyd12864DispPrintf(4,1,"�����ţ�ֱ���˳�"); 
    OsplcHardwarechk();
    TaskAdd(KEY_RET, DeviceTestMenu,NULL);
}

/********************************************************
**��������:DeviceTestMenu
**��������:�豸Ӳ������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-25

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void DeviceTestMenu(void)
{
    Fyd12864DispPrintf(1,1,"  Ӳ��ά������");
    Fyd12864DispPrintf(2,1,"1.����  2.��ŷ�");
    Fyd12864DispPrintf(3,1,"3.ѹ��  4.������");
    Fyd12864DispPrintf(4,1,"5.����  6.��ӡ��");
    TaskAdd(KEY_RET, DispFirst,NULL);
    TaskAdd(KEY_1,KeyBoardTest,NULL);
    TaskAdd(KEY_2,ValveTest,NULL);
    TaskAdd(KEY_3,PressureTest,NULL);
    TaskAdd(KEY_4,MassmeterTest,NULL);
    TaskAdd(KEY_5,DispLCDTest,NULL);
    TaskAdd(KEY_6,PrinterTest,NULL);
    TaskAdd(KEY_8,DispEepromCopy,NULL);
    TaskAdd(KEY_COMB_SFIX, SoftReset, NULL);//�������
}


