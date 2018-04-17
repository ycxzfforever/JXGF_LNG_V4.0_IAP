#include "kernel.h"

Screeninput ScreenInput;

/***************************************************************************
**�������ƣ� KeyPinInit
**�������ܣ�����IO �ڳ�ʼ��
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-3-25 14:26:32
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void KeyPinInit(void)
{
    LPC_GPIO3->DIR &= 0xFFF00000;     //����P3.0~P3.19Ϊ����
}

/***************************************************************************
**�������ƣ� GetKeyValue
**�������ܣ���ȡ���̰���ֵ
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-3-25 14:27:52
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void KeyGetValue(void)
{
    uint32_t rdkey,rdkey1;
    rdkey = (LPC_GPIO3->PIN & 0x000FFFFF);
    if(globalvar.KeyPreValue==rdkey)//����δ�ͷ�
    {
        return;
    }    
    if(rdkey!=0x000FFFFF)//��������
    {
        MsDelay(5);
        rdkey1=(LPC_GPIO3->PIN & 0x000FFFFF);
        if(rdkey==rdkey1)
        {
            globalvar.KeyValue=rdkey;
            BeepOut(1);
        }
    } 
    globalvar.KeyPreValue=rdkey;
}




/***************************************************************************
**�������ƣ� NumKeyHandler
**�������ܣ����ְ�������
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-3-28 16:51:49
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void NumKeyHandler(void)
{
    if(ScreenInput.inputlen<ScreenInput.inputmaxlen)
    {
        switch(globalvar.KeyValue)
        {
            case KEY_0:
                globalvar.KeyValue=0;
                ScreenInput.array[ScreenInput.inputlen++]=0x30;
            break;
            case KEY_1:
                globalvar.KeyValue=0;
                ScreenInput.array[ScreenInput.inputlen++]=0x31;
            break;
            case KEY_2:
                globalvar.KeyValue=0;
                ScreenInput.array[ScreenInput.inputlen++]=0x32;
            break;
            case KEY_3:
                globalvar.KeyValue=0;
                ScreenInput.array[ScreenInput.inputlen++]=0x33;
            break;
            case KEY_4:
                globalvar.KeyValue=0;
                ScreenInput.array[ScreenInput.inputlen++]=0x34;
            break;
            case KEY_5:
                globalvar.KeyValue=0;
                ScreenInput.array[ScreenInput.inputlen++]=0x35;
            break;
            case KEY_6:
                globalvar.KeyValue=0;
                ScreenInput.array[ScreenInput.inputlen++]=0x36;
            break;
            case KEY_7:
                globalvar.KeyValue=0;
                ScreenInput.array[ScreenInput.inputlen++]=0x37;
            break;
            case KEY_8:
                globalvar.KeyValue=0;
                ScreenInput.array[ScreenInput.inputlen++]=0x38;
            break;
            case KEY_9:
                globalvar.KeyValue=0;
                ScreenInput.array[ScreenInput.inputlen++]=0x39;
            break;
            case KEY_PT:
                if(ScreenInput.pointnum==0)
                {
                    ScreenInput.pointnum=1;
                    globalvar.KeyValue=0;
                    ScreenInput.array[ScreenInput.inputlen++]=0x2E;
                }
            break;
            case KEY_CLR:
                globalvar.KeyValue=0;
                if(ScreenInput.inputlen>0)  ScreenInput.inputlen--;
                if(ScreenInput.array[ScreenInput.inputlen]==0x2E) ScreenInput.pointnum=0;//0x2EС���㡣
    			ScreenInput.array[ScreenInput.inputlen]=0x20;                            //0x20�ա�
            break;
        }        
    }
    else
    {
        switch(globalvar.KeyValue)
        {
//            case KEY_RET:
            case KEY_CLR:
                globalvar.KeyValue=0;                
                if(ScreenInput.inputlen>0)  ScreenInput.inputlen--;
                if(ScreenInput.array[ScreenInput.inputlen]==0x2E) ScreenInput.pointnum=0;
    			ScreenInput.array[ScreenInput.inputlen]=0x20;
            break;
        }     
    }
}

