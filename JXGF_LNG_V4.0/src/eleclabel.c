#include "kernel.h"
/***************************************************************************
**�������ƣ�BQresults(void)
**�������ܣ���̨���͵ĵ��ӱ�ǩ�����
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-11-22
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void BQresults(void)
{
	Fyd12864ClearLine(1); 
    Fyd12864ClearLine(3);
	Fyd12864ClearLine(4); 
	if(globalvar.startstate==0)
	{
		 Fyd12864DispPrintf(2,1,"�Ѿ���Ȩ����ȴ�");
		 TaskMessage(20, StartPrecooling);
		 DispNextUI=StartFueling;
	}
	else if(globalvar.startstate==1)
	{
		Fyd12864DispPrintf(2,1,"  �Ƿ���ǩ...");
		globalvar.startstate=0xff;          			//���ǩ���
	}
	else if(globalvar.startstate==2)
	{
		Fyd12864DispPrintf(2,1,"  ��ƿ����...");
		globalvar.startstate=0xff;          			//���ǩ���
	}
	else if(globalvar.startstate==3)
	{
		Fyd12864DispPrintf(2,1,"  ��ƿ����...");
		globalvar.startstate=0xff;          			//���ǩ���
	}
	if(globalvar.startstate!=0)
	{
		TaskMessage(20, DispFirst);
	}
}

/***************************************************************************
**�������ƣ�WaitBQresults(void)
**�������ܣ���������ʱ���ȴ���Ȩ��
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-11-22
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void WaitBQresults(void)
{
	Fyd12864ClearLine(1); 
    Fyd12864ClearLine(3);
	Fyd12864ClearLine(4);
	
	Fyd12864DispPrintf(2,1,"��δ��Ȩ����ȴ�");
	if(globalvar.startstate!=0xff)
	{
		DispUI=BQresults;
	}
	TaskAdd(KEY_RET, DispFirst,NULL);
}
