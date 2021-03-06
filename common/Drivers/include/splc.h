/*************************************************************************************************
--------------File Info--------------------------------------------------------------------------
** 文本：SPLC.H
** 说明：本文档用于定义客显液晶屏的驱动函数和驱动端口。
** 端口：1:LCK--GPIO329
         2:CLK--GPIO330
         3:DAT--GPIO331
** 版本：V1.0
** 作者：杨传坤
** 时间：2016/03/29
**************************************************************************************************/
#ifndef SPLCH
#define SPLCH

#include "stdbool.h"//包含bool类型。
#include "math.h"   //包含fabs函数。

#define OsplcLckOut()   LPC_GPIO3->DIR |= 1<<29 // 方向-输出。
#define OsplcLck_L()    LPC_GPIO3->SET |= 1<<29 // LCK=1
#define OsplcLck_H()   LPC_GPIO3->CLR |= 1<<29 // LCK=0

#define OsplcClkOut()   LPC_GPIO3->DIR |= 1<<30 // 方向-输出。
#define OsplcClk_L()    LPC_GPIO3->SET |= 1<<30 // CLK=1
#define OsplcClk_H()    LPC_GPIO3->CLR |= 1<<30 // CLK=0

#define OsplcDataOut()  LPC_GPIO3->DIR |= (1ul<<31) // 方向-输出。
#define OsplcData_H()   LPC_GPIO3->SET |= (1ul<<31) // DATA=1 
#define OsplcData_L()   LPC_GPIO3->CLR |= (1ul<<31) // DATA=1
   

//气量
#define OsplcKGON()         DispMem[29] &= 0xFB  //S4与S1共用。
#define OsplcKGOFF()        DispMem[29] |= 0x04
//金额
#define OsplcSUMON()        DispMem[29] &= 0xFD  //S5 (U2/Y2)与S2共用。
#define OsplcSUMOFF()       DispMem[29] |= 0x02
//单价
#define OsplcPRICEON()      DispMem[29] &= 0xFE  //S (U2/Y1)与S18共用。
#define OsplcPRICEOFF()     DispMem[29] |= 0x01
//进气KG/MIN
#define OsplcKGMINON()      DispMem[13] &= 0xFB
#define OsplcKGMINOFF()     DispMem[13] |= 0x04  //SS(U5/Y11)
//报警
#define OsplcWARNINGON()    DispMem[21] &= 0xFD
#define OsplcWARNINGOFF()   DispMem[21] |= 0x02
//枪号
#define OsplcGNUMON()       DispMem[21] &= 0xFB
#define OsplcGNUMOFF()      DispMem[21] |= 0x04
//进气流量S9
#define OsplcFLNUMON()      DispMem[18] &= 0x7F
#define OsplcFLNUMOFF()     DispMem[18] |= 0x80
//回气流量S10
#define OsplcSTATEON()      DispMem[17] &= 0xFE
#define OsplcSTATEOFF()     DispMem[17] |= 0x01
//回气KG/MIN
#define OsplcSTATEUPAYON()  DispMem[14] &= 0x7F  //S12 (U5/Y8)
#define OsplcSTATEUPAYOFF() DispMem[14] |= 0x80
//增益
#define OsplcSTATEIDLEON()  DispMem[11] &= 0x7F  //S14 (U5/Y10)
#define OsplcSTATEIDLEOFF() DispMem[11] |= 0x80
//温度
#define OsplcTEMPNON()     DispMem[17] &= 0xFD   //S20 
#define OsplcTEMPNOFF()    DispMem[17] |= (~0xFD)

#define OsplcTEMPON()       DispMem[10]  &= 0xFE //S16 (U5/Y10)
#define OsplcTEMPOFF()      DispMem[10] |= 0x01
#define OsplcTEMPUNITON()   DispMem[7]  &= 0xBF  //S17 (U6/Y23)
#define OsplcTEMPUNITOFF()  DispMem[7] |= 0x40
//压力
#define OsplcPRESSUREON()   DispMem[7]  &= 0x7F  //S18 (U6/Y24)与S8共用。
#define OsplcPRESSUREOFF()  DispMem[7] |= 0x80
//LOGO
#define OsplcLOGOON()       DispMem[7]  &= 0xDF  //S15 (U6/Y22)
#define OsplcLOGOOFF()      DispMem[7] |= 0x20


static uint8_t DispMem[40] = {0};            //液晶大屏数据存储数组。
/*************************************************************************
** 函数名称:OsplcPortInit
** 函数功能：初始化客显端口。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-28

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcPortInit(void);

/*************************************************************************
** 函数名称:OsplcLatch
** 函数功能：当将所有数据送入LED显示后，使用此函数进行数据的锁存。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-28

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcLatch(void);

/*************************************************************************
** 函数名称:OsplcEnclk
** 函数功能：控制SPLC的一次时钟使能函数。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-28

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcEnclk(void);

/*************************************************************************
** 函数名称:OsplcDot
** 函数功能：向液晶蓝屏写入小数点。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcDot(uint8_t dcode);

/*************************************************************************
** 函数名称:OsplcDispB
** 函数功能：向蓝屏写入一帧数据。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcDispB(uint8_t *pdisp,bool nPoint);

/*************************************************************************
** 函数名称:OsplcShowB
** 函数功能：向蓝屏液晶某行写入数据。
** 入口参数：Row1：第一行要写入的数据；
             Row2：第二行要写入的数据；
             Row3：第三行要写入的数据。
             bStop：负号标志。
             bOverFlag：第一行，第二行第一个字符显示标志。
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcShowB(uint32_t Row1,uint32_t Row2,uint16_t Row3,bool bStop, bool bOverFlag);

/*************************************************************************
** 函数名称:OsplcHardwarechk
** 函数功能：检测液晶是否能正常工作。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcHardwarechk(void);

/*************************************************************************
** 函数名称:OsplcInit
** 函数功能：初始化液晶屏。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcInit(void);

/*************************************************************************
** 函数名称:OsplcDispN
** 函数功能：向大屏液晶写入一帧的数据。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcDispN(void); 

/*************************************************************************
** 函数名称:OsplcShowN
** 函数功能：向大屏写入要显示的所有数据。
** 入口参数：Row1--气量；
             Row2--金额；
             Row3--单价。
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcShowN(uint32_t Row1,uint32_t Row2,uint16_t Row3);

/*************************************************************************
** 函数名称:OsplcTitleOn
** 函数功能：点亮液晶上的名称，单位等位置。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcTitleOn(void);

/*************************************************************************
** 函数名称:OsplcNum
** 函数功能：大屏液晶气量显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
bool OsplcNum(uint32_t vnum);

/*************************************************************************
** 函数名称:OsplcSum
** 函数功能：大屏液晶金额显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
bool OsplcSum(uint32_t vsum);

/*************************************************************************
** 函数名称:OsplcPrice
** 函数功能：大屏液晶价格显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
bool OsplcPrice(uint32_t vprice);

/*************************************************************************
** 函数名称:OsplcGunNum
** 函数功能：大屏液晶枪号显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcGunNum(uint8_t GunNum); //NUM24 NUM25

    /*************************************************************************
** 函数名称:OsplcFlRate
** 函数功能：大屏液晶进气流量显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
bool OsplcFlRate(uint32_t nfrNum);

/*************************************************************************
** 函数名称:OsplcTemperature
** 函数功能：大屏液晶温度显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
bool OsplcTemperature(float vfTemp);

/*************************************************************************
** 函数名称:OsplcWarnning
** 函数功能：大屏液晶异常停机报警显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcWarnning(uint8_t WNO); //NUM22 NUM23

/*************************************************************************
** 函数名称:OsplcDrvgain
** 函数功能：大屏液晶增益显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
bool OsplcDrvgain(int nTnum);

/*************************************************************************
** 函数名称:OsplcFlrateBack
** 函数功能：大屏液晶回气流速显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
bool OsplcFlrateBack(uint32_t nfrNum);

/*************************************************************************
** 函数名称:OsplcPressure
** 函数功能：大屏液晶进气流量显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29

** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcPressure(uint32_t npNum); 

/*************************************************************************
** 函数名称:OsplcDispInit
** 函数功能：大屏液晶初始化显示。
** 入口参数：无
** 返 回 值：无 
** 创 建 者：杨传坤
** 创建时间：2016-03-29
** 修 改 者：
** 修改如期：
** 修改内容：
*************************************************************************/
void OsplcDispInit(void);
void OsplcShow(uint32_t Row1,uint32_t Row2,uint16_t Row3,bool bStop, bool bOverFlag);
uint8_t GetOnePosition(uint32_t num);

#endif 
