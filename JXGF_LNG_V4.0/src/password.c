/*
 * GetPassword.c 
 * ˵�������ĵ��������ڼ��ܰ幦��ʹ�á�
 * Created: 2016-5-11 13:51:16
 *  Author: yck
 */ 
#include "kernel.h"
uint8_t Password[6];
/***************************************************************************
 **name:GetGunNum().
 **���ܣ���ǹ��ת��ΪBCD��-yck20160512��
****************************************************************************/
void GetGunNum(void)
{		
	Password[0]=sysparas.gunnum;
}

/***************************************************************************
 **name:GetTimeSec(void)
 **���ܣ���ʱ����ת��ΪBCD��-yck20160512��
****************************************************************************/
void GetTimeSec(void)
{
    Password[1]=time.second;
}

/***************************************************************************
 **name:GetGunSum(void)
 **���ܣ�ȡǹ�ۼ����ѽ������ֽ�-yck20160512��
****************************************************************************/
void GetGunSum(void)
{
	Password[2]=(uint8_t)sysparas.totmoney; // ǹ�ۼ����ѽ������ֽڡ�
}

/***************************************************************************
 **name:GetGunGas(void)
 **���ܣ�ȡǹ�ۼ�������������ֽ�-yck20160512��
****************************************************************************/
void GetGunGas(void)
{
	Password[3]=(uint8_t)sysparas.totvolume;// ǹ�ۼ�������������ֽڡ�
}

/***************************************************************************
 **name:GetTimeMin(void)
 **���ܣ���ʱ���ת��ΪBCD��-yck20160512��
****************************************************************************/
void GetTimeMin(void)
{
	Password[4]=time.minute;          // ʱ��֡�
}

/***************************************************************************
 **name:GetDataCrc(void)
 **���ܣ��õ���������CRCУ����-yck20160512��
****************************************************************************/
void GetDataCrc(void)
{
	Password[5] = (uint8_t)CRC16Calculate(&Password[0], 5);   // CRCУ�顣
}

/***************************************************************************
 **name:LockPortInit(void)
 **���ܣ����ܰ�˿ڳ�ʼ��-yck20160512��
****************************************************************************/
void LockPortInit(void)
{
	S1_IN;         //S1��������Ϊ���롣
}

/***************************************************************************
 **name:GetLockdata(void)
 **���ܣ��õ���������-yck20160512��
****************************************************************************/
void GetLockdata(void)
{
	GetGunNum(); // ���š�
	GetTimeSec();// �롣
	GetGunSum(); // ǹ�������С�ֽڡ�
	GetGunGas(); // ǹ��������С�ֽڡ�
	GetTimeMin();// �֡�
	GetDataCrc();// CRC��
}

/***************************************************************************
 **name:LockDataStore(void)
 **���ܣ���Flash�ͼ��ܰ�����洢��������-yck20160512��
****************************************************************************/
void LockDataStore(void)
{
    uint8_t i1,i2;
    uint8_t buf[6];
    uint8_t tmp[6];
    GetLockdata(); //�õ��������ݡ�
    
    At24C512WriteNByte(LOCK_ADDR,&Password[0],6);
    MsDelay(100);
    At24C128WriteNByte(0,        &Password[0],6);
    
    if(At24C128ReadNByte(0, buf, 6))
    {
        for(i1=0;i1<6;i1++)
        {
            if(Password[i1]!=buf[i1]) break;
        }
      
    }
    if(At24C512ReadNByte(LOCK_ADDR, tmp, 6))
    {
        for(i2=0;i2<6;i2++)
        {
            if(Password[i2]!=tmp[i2]) break;
        }
      
    }
    if((i1==6)&&(i2==6))
    {
        if(SaveSysparas(SYSPARA_ADDR,(uint8_t*)&sysparas.head,sizeof(Sysparas)))
        {
            SetSuccess();
        } 
		else
	    {
	    	sysparas.usepwdboard=0;
	        SetFault();
	    }
    }
    else
    {
    	sysparas.usepwdboard=0;
        SetFault();
    }
}

/***************************************************************************
 **name:LockDataCompare(void)
 **���ܣ�����ȶԡ�����ֵ����0-�ȶ�ʧ�ܣ�1-�ȶԳɹ�-yck20160512��
****************************************************************************/
unsigned char LockDataCompare(void)
{
	uint8_t temp_lock0[6];
	uint8_t temp_lock1[6];
	uint8_t i;
	At24C512ReadNByte(LOCK_ADDR, temp_lock0, 6);
    MsDelay(100);
    At24C128ReadNByte(0,         temp_lock1, 6);
	
	for (i=0;i<6;i++)                     //�ȶ����롣
	{
		if (temp_lock0[i]!=temp_lock1[i])
		{
			return 0;                     //�ȶ�ʧ�ܡ�
		}
	}
	return 1;                             //�ȶԳɹ���
}

