/*************************************************************************************************
--------------File Info--------------------------------------------------------------------------
** �ı���SPLC.H
** ˵�������ĵ����ڶ������Һ���������������������˿ڡ�
** �˿ڣ�1:LCK--GPIO329
         2:CLK--GPIO330
         3:DAT--GPIO331
** �汾��V1.0
** ���ߣ����
** ʱ�䣺2016/03/29
**************************************************************************************************/
#ifndef SPLCH
#define SPLCH

#include "stdbool.h"//����bool���͡�
#include "math.h"   //����fabs������

#define OsplcLckOut()   LPC_GPIO3->DIR |= 1<<29 // ����-�����
#define OsplcLck_L()    LPC_GPIO3->SET |= 1<<29 // LCK=1
#define OsplcLck_H()   LPC_GPIO3->CLR |= 1<<29 // LCK=0

#define OsplcClkOut()   LPC_GPIO3->DIR |= 1<<30 // ����-�����
#define OsplcClk_L()    LPC_GPIO3->SET |= 1<<30 // CLK=1
#define OsplcClk_H()    LPC_GPIO3->CLR |= 1<<30 // CLK=0

#define OsplcDataOut()  LPC_GPIO3->DIR |= (1ul<<31) // ����-�����
#define OsplcData_H()   LPC_GPIO3->SET |= (1ul<<31) // DATA=1 
#define OsplcData_L()   LPC_GPIO3->CLR |= (1ul<<31) // DATA=1
   

//����
#define OsplcKGON()         DispMem[29] &= 0xFB  //S4��S1���á�
#define OsplcKGOFF()        DispMem[29] |= 0x04
//���
#define OsplcSUMON()        DispMem[29] &= 0xFD  //S5 (U2/Y2)��S2���á�
#define OsplcSUMOFF()       DispMem[29] |= 0x02
//����
#define OsplcPRICEON()      DispMem[29] &= 0xFE  //S (U2/Y1)��S18���á�
#define OsplcPRICEOFF()     DispMem[29] |= 0x01
//����KG/MIN
#define OsplcKGMINON()      DispMem[13] &= 0xFB
#define OsplcKGMINOFF()     DispMem[13] |= 0x04  //SS(U5/Y11)
//����
#define OsplcWARNINGON()    DispMem[21] &= 0xFD
#define OsplcWARNINGOFF()   DispMem[21] |= 0x02
//ǹ��
#define OsplcGNUMON()       DispMem[21] &= 0xFB
#define OsplcGNUMOFF()      DispMem[21] |= 0x04
//��������S9
#define OsplcFLNUMON()      DispMem[18] &= 0x7F
#define OsplcFLNUMOFF()     DispMem[18] |= 0x80
//��������S10
#define OsplcSTATEON()      DispMem[17] &= 0xFE
#define OsplcSTATEOFF()     DispMem[17] |= 0x01
//����KG/MIN
#define OsplcSTATEUPAYON()  DispMem[14] &= 0x7F  //S12 (U5/Y8)
#define OsplcSTATEUPAYOFF() DispMem[14] |= 0x80
//����
#define OsplcSTATEIDLEON()  DispMem[11] &= 0x7F  //S14 (U5/Y10)
#define OsplcSTATEIDLEOFF() DispMem[11] |= 0x80
//�¶�
#define OsplcTEMPNON()     DispMem[17] &= 0xFD   //S20 
#define OsplcTEMPNOFF()    DispMem[17] |= (~0xFD)

#define OsplcTEMPON()       DispMem[10]  &= 0xFE //S16 (U5/Y10)
#define OsplcTEMPOFF()      DispMem[10] |= 0x01
#define OsplcTEMPUNITON()   DispMem[7]  &= 0xBF  //S17 (U6/Y23)
#define OsplcTEMPUNITOFF()  DispMem[7] |= 0x40
//ѹ��
#define OsplcPRESSUREON()   DispMem[7]  &= 0x7F  //S18 (U6/Y24)��S8���á�
#define OsplcPRESSUREOFF()  DispMem[7] |= 0x80
//LOGO
#define OsplcLOGOON()       DispMem[7]  &= 0xDF  //S15 (U6/Y22)
#define OsplcLOGOOFF()      DispMem[7] |= 0x20


static uint8_t DispMem[40] = {0};            //Һ���������ݴ洢���顣
/*************************************************************************
** ��������:OsplcPortInit
** �������ܣ���ʼ�����Զ˿ڡ�
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-28

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcPortInit(void);

/*************************************************************************
** ��������:OsplcLatch
** �������ܣ�����������������LED��ʾ��ʹ�ô˺����������ݵ����档
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-28

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcLatch(void);

/*************************************************************************
** ��������:OsplcEnclk
** �������ܣ�����SPLC��һ��ʱ��ʹ�ܺ�����
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-28

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcEnclk(void);

/*************************************************************************
** ��������:OsplcDot
** �������ܣ���Һ������д��С���㡣
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcDot(uint8_t dcode);

/*************************************************************************
** ��������:OsplcDispB
** �������ܣ�������д��һ֡���ݡ�
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcDispB(uint8_t *pdisp,bool nPoint);

/*************************************************************************
** ��������:OsplcShowB
** �������ܣ�������Һ��ĳ��д�����ݡ�
** ��ڲ�����Row1����һ��Ҫд������ݣ�
             Row2���ڶ���Ҫд������ݣ�
             Row3��������Ҫд������ݡ�
             bStop�����ű�־��
             bOverFlag����һ�У��ڶ��е�һ���ַ���ʾ��־��
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcShowB(uint32_t Row1,uint32_t Row2,uint16_t Row3,bool bStop, bool bOverFlag);

/*************************************************************************
** ��������:OsplcHardwarechk
** �������ܣ����Һ���Ƿ�������������
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcHardwarechk(void);

/*************************************************************************
** ��������:OsplcInit
** �������ܣ���ʼ��Һ������
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcInit(void);

/*************************************************************************
** ��������:OsplcDispN
** �������ܣ������Һ��д��һ֡�����ݡ�
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcDispN(void); 

/*************************************************************************
** ��������:OsplcShowN
** �������ܣ������д��Ҫ��ʾ���������ݡ�
** ��ڲ�����Row1--������
             Row2--��
             Row3--���ۡ�
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcShowN(uint32_t Row1,uint32_t Row2,uint16_t Row3);

/*************************************************************************
** ��������:OsplcTitleOn
** �������ܣ�����Һ���ϵ����ƣ���λ��λ�á�
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcTitleOn(void);

/*************************************************************************
** ��������:OsplcNum
** �������ܣ�����Һ��������ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
bool OsplcNum(uint32_t vnum);

/*************************************************************************
** ��������:OsplcSum
** �������ܣ�����Һ�������ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
bool OsplcSum(uint32_t vsum);

/*************************************************************************
** ��������:OsplcPrice
** �������ܣ�����Һ���۸���ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
bool OsplcPrice(uint32_t vprice);

/*************************************************************************
** ��������:OsplcGunNum
** �������ܣ�����Һ��ǹ����ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcGunNum(uint8_t GunNum); //NUM24 NUM25

    /*************************************************************************
** ��������:OsplcFlRate
** �������ܣ�����Һ������������ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
bool OsplcFlRate(uint32_t nfrNum);

/*************************************************************************
** ��������:OsplcTemperature
** �������ܣ�����Һ���¶���ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
bool OsplcTemperature(float vfTemp);

/*************************************************************************
** ��������:OsplcWarnning
** �������ܣ�����Һ���쳣ͣ��������ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcWarnning(uint8_t WNO); //NUM22 NUM23

/*************************************************************************
** ��������:OsplcDrvgain
** �������ܣ�����Һ��������ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
bool OsplcDrvgain(int nTnum);

/*************************************************************************
** ��������:OsplcFlrateBack
** �������ܣ�����Һ������������ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
bool OsplcFlrateBack(uint32_t nfrNum);

/*************************************************************************
** ��������:OsplcPressure
** �������ܣ�����Һ������������ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29

** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcPressure(uint32_t npNum); 

/*************************************************************************
** ��������:OsplcDispInit
** �������ܣ�����Һ����ʼ����ʾ��
** ��ڲ�������
** �� �� ֵ���� 
** �� �� �ߣ����
** ����ʱ�䣺2016-03-29
** �� �� �ߣ�
** �޸����ڣ�
** �޸����ݣ�
*************************************************************************/
void OsplcDispInit(void);
void OsplcShow(uint32_t Row1,uint32_t Row2,uint16_t Row3,bool bStop, bool bOverFlag);
uint8_t GetOnePosition(uint32_t num);

#endif 