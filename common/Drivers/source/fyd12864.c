#include "kernel.h"

/********************************************************
**函数名称:Fyd12864SendByte
**函数功能:模拟时序，发送一个字节
**输入参数:ch---即将发送一个字节
**输出值  :无

**创建者  :黄豪民
**创建日期:2016-03-28

**修改者  :
**修改日期:
**修改内容:
********************************************************/
void Fyd12864SendByte(char ch)
{
 uint8_t i;
 Fyd12864_SDI_Out; 	 
 for(i=0;i<8;i++)
 {
    Fyd12864_CLK_Low;
    if(ch&0x80)	
    {
      Fyd12864_SDI_High;
    } 
    else     	        
    {
      Fyd12864_SDI_Low; 
    }
    ch=ch<<1;
    Fyd12864_CLK_High; 
    UsDelay(30); 
 }
 Fyd12864_SDI_Out; 
}

/********************************************************
**函数名称:Fyd12864WriteData
**函数功能:根据发送模式，向FYD12864发送一个命令或数据
**输入参数:data---即将发送一个数据或命令
           mode---发送模式 0x01：向FYD12864发送一个命令  0：向FYD12864发送一个数据
**输出值  :无

**创建者  :黄豪民
**创建日期:2016-03-28

**修改者  :
**修改日期:
**修改内容:
********************************************************/
void Fyd12864WriteData(char data, int8_t mode)
{
 Fyd12864_CS_High;
 if(mode)        
 {
    Fyd12864SendByte(0xF8);      
 }  	
 else        
 {
    Fyd12864SendByte(0xFA);     
 }
 Fyd12864SendByte(0xF0&data);		     
 Fyd12864SendByte(0xF0&(data<<4));	     
 Fyd12864_CS_Low;
}

/********************************************************
**函数名称:Fyd12864ClearScreen
**函数功能:清屏
**输入参数:无
**输出值  :无

**创建者  :黄豪民
**创建日期:2016-03-28

**修改者  :
**修改日期:
**修改内容:
********************************************************/
void Fyd12864ClearScreen(void)
{
    Fyd12864WriteData(0,0x01);
    UsDelay(100);
//    Fyd12864DispPrintf(1,1,"                ");
//    Fyd12864DispPrintf(2,1,"                ");
//    Fyd12864DispPrintf(3,1,"                ");
//    Fyd12864DispPrintf(4,1,"                ");

}

/********************************************************
**函数名称:Fyd12864Locate
**函数功能:定位主显显示位置
**输入参数:row:行坐标(注：针对FYD12864,范围为1<=row<=4)
           col:列坐标(注：针对FYD12864,范围为1<=col<=8))
**输出值  :无

**创建者  :黄豪民
**创建日期:2016-03-28

**修改者  :
**修改日期:
**修改内容:
********************************************************/
void Fyd12864Locate(uint8_t row,uint8_t col)
{
  uint8_t addr[4] ={0x80,0x90,0x88,0x98};
  Fyd12864WriteData(addr[row-1]+col-1,0x01);  
}

/********************************************************
**函数名称:FYD12864WriteString
**函数功能:在主显的row行col列开始显示字符串
**输入参数:row:行坐标(注：针对FYD12864,范围为1<=row<=4)
           col:列坐标(注：针对FYD12864,范围为1<=col<=8))
					 str:显实木字符串
**输出值  :无

**创建者  :黄豪民
**创建日期:2016-03-28

**修改者  :
**修改日期:
**修改内容:
********************************************************/
void FYD12864WriteString(uint8_t row,uint8_t col,char *str)
{
  uint8_t i,length;
  length=strlen((char *)str);
  length=((9-col)*2 < length) ? (9-col)*2 :length; //截取显示长度
  Fyd12864Locate(row,col);
  for(i=0;i<length;i++)
     Fyd12864WriteData(*str++,0);//显示字符串
	for(i=(col-1)*2+length;i<16;i++)
	{
		Fyd12864WriteData(0x20,0);//如果字符串没有占满一行，则后面显示空
	} 
}

/********************************************************
**函数名称:Fyd12864DisChar
**函数功能:在主显的row行col列开始显示一个字符
**输入参数:row:行坐标(注：针对FYD12864,范围为1<=row<=4)
           col:列坐标(注：针对FYD12864,范围为1<=col<=8))
					 ch:显实字节
**输出值  :无

**创建者  :黄豪民
**创建日期:2016-03-28

**修改者  :
**修改日期:
**修改内容:
********************************************************/
void Fyd12864DisChar(uint8_t row,uint8_t col,char ch)
{
     Fyd12864Locate(row, col);
     Fyd12864WriteData(ch,0);
}

void Fyd12864DispPrintf(uint8_t row, uint8_t col, char *fmt,...)
/********************************************************
**函数名称:FYD12864DispPrintf
**函数功能:在主显的row行col列开始显示格式化后一个字符串
**输入参数:row:行坐标(注：针对FYD12864,范围为1<=row<=4)
           col:列坐标(注：针对FYD12864,范围为1<=col<=8))
					 fmt:格式化字符串
**输出值  :无

**创建者  :黄豪民
**创建日期:2016-03-28

**修改者  :
**修改日期:
**修改内容:
********************************************************/
{
    va_list ap;
    char str[100];
    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    FYD12864WriteString(row,col,str);
    va_end(ap);
}

void Fyd12864ClearLine(uint8_t row)
/********************************************************
**函数名称:FYD12864ClearLine
**函数功能:清空一行字符
**输入参数:row:行坐标(注：针对FYD12864,范围为1<=row<=4)
**输出值  :无

**创建者  :黄豪民
**创建日期:2016-03-28

**修改者  :
**修改日期:
**修改内容:
********************************************************/
{
  unsigned char i;
  Fyd12864Locate(row,1);
  for(i=0; i<16; i++)
  {
    Fyd12864WriteData(0x20,0);
  }
}

/********************************************************
**函数名称:Fyd12864Init
**函数功能:FYD12864初始化，包括引脚方向等
**输入参数:无
**输出值  :无

**创建者  :黄豪民
**创建日期:2016-03-28

**修改者  :
**修改日期:
**修改内容:
********************************************************/
void Fyd12864Init(void)
{  
  Fyd12864_CS_Out;
  UsDelay(10);
  Fyd12864_SDI_Out;
  UsDelay(10);;
  Fyd12864_CLK_Out;
  UsDelay(10);
  Fyd12864_CS_High; 
  UsDelay(10);
  Fyd12864_SDI_High; 
  UsDelay(10);
  Fyd12864_CLK_High; 
  UsDelay(10); 
  Fyd12864WriteData(0x03,0x01);//设定DDRAM的地址计数器到"00"
  Fyd12864WriteData(0x0C,0x01);//整体显示
  Fyd12864WriteData(0x06,0x01);//光标右移
  Fyd12864WriteData(0x01,0x01);//消除显示
  UsDelay(100);
  Fyd12864ClearScreen();
}

