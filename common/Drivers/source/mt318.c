#include "kernel.h"

Mt318Reader mt318reader;
//M1Card m1card;    //定义M1卡块结构体。//huanghao20161026

/***************************************************************************
**函数名称： Mt318CrcVerify
**函数功能：MT318读卡器校验算法
**入口参数：array:需要校验的数据
                                    length:校验数据的长度
**返回值：计算后的BCC校验值
**创建者：杨朝旭
**创建日期：2016-3-31 16:27:23
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
uint8_t Mt318CrcVerify(uint8_t *array,uint8_t length)
{
    uint8_t crcresult=0;
    uint8_t tmp;
    crcresult=*array;
    for(tmp=1; tmp<length; tmp++)
        crcresult^=*(array+tmp);
    return(crcresult);
}

/***************************************************************************
**函数名称： Mt318GetCardStatus
**函数功能：通过MT318获取卡的状态
**入口参数：无
**返回值：TRUE:成功，FALSE:失败
**创建者：杨朝旭
**创建日期：2016-4-7 14:35:24
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
uint8_t Mt318GetCardStatus(void)
{
    //0x02 0x00 0x02 0x31 0x41 0x03 BCC
    uint8_t     str[7] = {0x02, 0x00, 0x02, 0x31,0x41,0x03};
    uint32_t    timeout =  CARD_RESPONSE_MAX_TIME;
    uint8_t     ret = FALSE;

    memset((uint8_t*)mt318reader.recbuf,0,sizeof(mt318reader.recbuf));
    str[6]=Mt318CrcVerify(str,6);
    mt318reader.err_code = 0;

    SELECT_CARD();
    OpenUartRcvIRQ(CardPort);
    UARTSend(CardPort, str, 7);

    timeout =  CARD_RESPONSE_MAX_TIME;
    while(--timeout)
    {
        if(mt318reader.recbuf[5] == 0x31)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(2,1,"机内无卡");
            break;
        }
        else if(mt318reader.recbuf[6] == 0x30)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(2,1,"寻不到射频卡");
            break;
        }
        else if(mt318reader.recbuf[6] == 0x32)
        {
            ret = TRUE;
            //Fyd12864DispPrintf(2,1,"CPU B");
            break;
        }
        else if(mt318reader.recbuf[6] == 0x33)
        {
            ret = TRUE;
            //Fyd12864DispPrintf(2,1,"CPU A");
            break;
        }
        else if(mt318reader.recbuf[6] == 0x34)
        {
            ret = TRUE;
            //Fyd12864DispPrintf(2,1,"M1卡");
            break;
        }
        mt318reader.err_code = mt318reader.recbuf[6];
    }
    if(timeout == 0) ret = FALSE;
    return  ret;
}

/***************************************************************************
**函数名称： Mt318GetCardSerialNum
**函数功能：通过MT318获取卡的序列号
**入口参数：无
**返回值：TRUE:成功，FALSE:失败
**创建者：杨朝旭
**创建日期：2016-4-7 14:36:07
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
uint8_t Mt318GetCardSerialNum(void)  //寻卡获取卡序列号
{
    uint8_t     str[7] = {0x02, 0x00, 0x02, 0x34,0x31,0x03};
    uint32_t    timeout =  CARD_RESPONSE_MAX_TIME;
    uint8_t     ret = FALSE;

    memset((uint8_t*)mt318reader.recbuf,0,sizeof(mt318reader.recbuf));
    str[6]=Mt318CrcVerify(str,6);
    mt318reader.err_code = 0;
    OpenUartRcvIRQ(CardPort);
    SELECT_CARD();
    UARTSend(CardPort, str, 7);
    while(--timeout)
    {
        if(mt318reader.recbuf[5] == 0x59 &&mt318reader.recbuf[10] == 0x03)
        {
            ret = TRUE;
            //Fyd12864DispPrintf(2,1,"卡序列号");
           // Fyd12864DispPrintf(3,1,"%02X%02X%02X%02X",mt318reader.recbuf[6],mt318reader.recbuf[7],
                               //mt318reader.recbuf[8],mt318reader.recbuf[9]);
            break;
        }
        else if(mt318reader.recbuf[5] == 0x4e)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(2,1,"寻卡不成功");
            break;
        }
        else if(mt318reader.recbuf[5] == 0x45)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(2,1,"机内无卡");
            break;
        }
        mt318reader.err_code = mt318reader.recbuf[5];
    }
    if(timeout == 0) ret = FALSE;
    return  ret;
}

/***************************************************************************
**函数名称： Mt318VerifyKeyB
**函数功能：验证卡的B密
**入口参数：key:密码
                                secter_num:扇区号
**返回值：TRUE:成功，FALSE:失败
**创建者：杨朝旭
**创建日期：2016-4-7 14:37:19
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
uint8_t Mt318VerifyKey(uint8_t keyAB,uint8_t * key,uint8_t secter_num)
{
    uint8_t     i,str[14] = {0x02, 0x00, 0x09, 0x34,0x39,0xFF,'2','2','2','2','2','2'};
    uint32_t    timeout =  CARD_RESPONSE_MAX_TIME;
    uint8_t     ret = FALSE;

    memset((uint8_t*)mt318reader.recbuf,0,sizeof(mt318reader.recbuf));
    str[4] = keyAB;
    str[5] = secter_num;
    for(i=0; i<6; i++)
    {
        str[6+i]=key[i];
    }
    str[12]= 0x03;
    str[13]=Mt318CrcVerify(str,13);

    mt318reader.err_code = 0;
    SELECT_CARD();
    OpenUartRcvIRQ(CardPort);
    UARTSend(CardPort, str, 14);
    while(--timeout)
    {
        if(mt318reader.recbuf[6] == 0x59)
        {
            ret = TRUE;
            //Fyd12864DispPrintf(2,1,"密码验证成功");
            break;
        }
        else if(mt318reader.recbuf[6] == 0x30)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(2,1,"寻不到射频卡");
            break;
        }
        else if(mt318reader.recbuf[6] == 0x33)
        {
            ret = FALSE;
            if(mt318reader.recbuf[4]==KEYA);
                //Fyd12864DispPrintf(3,1,"A 密验证失败");
            else if(mt318reader.recbuf[4]==KEYB);
                //Fyd12864DispPrintf(3,1,"B 密验证失败");
            break;
        }
        else if(mt318reader.recbuf[6] == 0x45)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(2,1,"卡机内无卡");
            break;
        }
        mt318reader.err_code = mt318reader.recbuf[6];
    }
    if(timeout == 0) ret = FALSE;
    return  ret;
}

/***************************************************************************
**函数名称： Mt318ReadM1Card
**函数功能：读取M1卡中的内容
**入口参数：keyAB:验证密码时采用A密验证还是B密验证
                                key:密码
                                blocks:块号
                                rdbuf:将读取的数据内容装入该buf
**返回值：TRUE:成功，FALSE:失败
**创建者：杨朝旭
**创建日期：2016-4-7 14:41:23
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
uint8_t Mt318ReadM1Card(uint8_t keyAB,uint8_t * key,uint8_t blocks, uint8_t* rdbuf)
{
    uint8_t     secter = 0;
    uint8_t     str[10] = {0x02, 0x00, 0x04, 0x34,0x33};
    uint32_t    timeout =  CARD_RESPONSE_MAX_TIME;
    uint8_t     ret = FALSE;
    WATCH_DOG();
    secter = blocks/4;
    if(Mt318VerifyKey(keyAB,key,secter) == FALSE)
    {
        return FALSE;
    }

    str[5] = secter;
    str[6] = blocks-4*secter;
    str[7] = 0x03;
    str[8]=Mt318CrcVerify(str,8);

    memset((uint8_t*)mt318reader.recbuf,0,sizeof(mt318reader.recbuf));
    mt318reader.err_code = 0;
    SELECT_CARD();
    OpenUartRcvIRQ(CardPort);
    UARTSend(CardPort, str, 9);
    while(--timeout)
    {
        if((mt318reader.recbuf[7] == 0x59)&&(mt318reader.recbuf[24] == 0x03))
        {
            ret = TRUE;
            memcpy(rdbuf, (uint8_t*)&mt318reader.recbuf[8], 16);
            //Fyd12864DispPrintf(4,1,"读块成功:%d,%x",blocks,mt318reader.recbuf[11]);
            break;
        }
        else if(mt318reader.recbuf[7] == 0x30)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(4,1,"寻不到RF卡");
            break;
        }
        else if(mt318reader.recbuf[7] == 0x33)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(4,1,"密码验证错误%x,%x",mt318reader.recbuf[5],mt318reader.recbuf[6]);
            break;
        }
        else if(mt318reader.recbuf[7] == 0x34)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(4,1,"读数据错");
            break;
        }
        else if(mt318reader.recbuf[7] == 0x45)
        {
            ret = FALSE;
           // Fyd12864DispPrintf(4,1,"卡机内无卡");
            break;
        }
        mt318reader.err_code = mt318reader.recbuf[7];
    }
    if(timeout == 0) ret = FALSE;
    return  ret;
}


/***************************************************************************
**函数名称： Mt318WriteM1Card
**函数功能：向M1卡中写入数据
**入口参数:   keyAB:验证密码时采用A密验证还是B密验证�
                                key:密码
                                blocks:块号
                                wtbuf:将wtbuf的数据写入m1卡中
**返回值：TRUE:成功，FALSE:失败
**创建者：杨朝旭
**创建日期：2016-4-7 14:43:34
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
uint8_t Mt318WriteM1Card(uint8_t keyAB,uint8_t * key,uint8_t blocks, uint8_t *wtbuf)
{
    uint8_t     secter = 0;
    uint8_t     str[26] = {0x02, 0x00, 0x14, 0x34,0x34};
    uint32_t    timeout =  CARD_RESPONSE_MAX_TIME;
    uint8_t     ret = FALSE;
    WATCH_DOG();
    secter = blocks/4;
    if(Mt318VerifyKey(keyAB,key,secter) == FALSE)
    {
        return FALSE;
    }
    str[5] = secter;
    str[6] = blocks-4*secter;
    memcpy((uint8_t *)&str[7], wtbuf, 16);
    str[23] =  0x03;
    str[24]=Mt318CrcVerify(str,24);

    memset((uint8_t*)mt318reader.recbuf,0,sizeof(mt318reader.recbuf));
    mt318reader.err_code = 0;
    SELECT_CARD();
    OpenUartRcvIRQ(CardPort);
    UARTSend(CardPort, str, 25);

    while(--timeout)
    {
        if(mt318reader.recbuf[7] == 0x59)
        {
            ret = TRUE;
            Fyd12864DispPrintf(4,1,"写块成功:%d",blocks);
            break;
        }
        else if(mt318reader.recbuf[7] == 0x30)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(4,1,"寻不到RF卡");
            break;
        }
        else if(mt318reader.recbuf[7] == 0x31)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(4,1,"操作扇区号错");
            break;
        }
        else if(mt318reader.recbuf[7] == 0x33)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(4,1,"密码验证错%x,%x",mt318reader.recbuf[5],mt318reader.recbuf[6]);
            break;
        }
        else if(mt318reader.recbuf[7] == 0x34)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(4,1,"校验写入块数据错");
            break;
        }
        else if(mt318reader.recbuf[7] == 0x45)
        {
            ret = FALSE;
            //Fyd12864DispPrintf(4,1,"卡机内无卡");
            break;
        }
        mt318reader.err_code = mt318reader.recbuf[7];
    }
    if(timeout == 0) ret = FALSE;
    return  ret;
}


/***************************************************************************
**函数名称： Mt318RecDataCheck
**函数功能：中断处理函数中检测接收到读卡器的数据是否正确
**入口参数:   无
**返回值：无
**创建者：杨朝旭
**创建日期：2016-4-7 14:46:34
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void Mt318RecDataCheck(void)
{
    uint32_t    len,i;
    uint32_t    datalen;//接收mt318数据帧中的数据长度，除开帧头、帧尾、BCC
    uint8_t     crc;
    if(UART0Buffer[0]==0x02)//帧头
    {
        if((UART0Count>2)&&((UART0Buffer[1]!=0)||(UART0Buffer[2]!=0)))
        {
            datalen=(uint32_t)UART0Buffer[1]<<8|UART0Buffer[2];
            len=5+datalen;

        }
        else if((UART0Buffer[1]==0)&&(UART0Buffer[2]==0))
        {//可能是MT318bug，当获取序列号时，若机内无卡，则返回的长度为0            
            len=12;
        }
        if((UART0Count>=len)&&(UART0Count>2))
        {
            crc=Mt318CrcVerify((uint8_t*)UART0Buffer,len-1);
            if(crc==UART0Buffer[len-1])
            {
                CloseUartRcvIRQ(CardPort);
                for(i=0; i<len; i++)
                {
                    mt318reader.recbuf[i]=UART0Buffer[i] ;
                }
                mt318reader.flag = 1;
            }
            UART0Count=0;
        }
    }
    else if(UART0Buffer[0]==0x06)
    {
        UartSendByte(CardPort,0x05);
        UART0Count=0;
    }
    else
    {
        OpenUartRcvIRQ(CardPort);
        UART0Count=0;
    }
}



/*********************************************************************************************************
** 函数名称：UART0_IRQHandler
** 函数描述：UART0中断处理函数
** 输入参数：无
** 返回值  ：无
*********************************************************************************************************/
void UART0_IRQHandler(void)
{
    uint8_t uiIIRValue = 0, uiLSRValue = 0;
    uint8_t uiDummy = uiDummy;

    uiIIRValue = LPC_UART0->IIR;

    uiIIRValue >>= 1;                     /* 中断标志寄存器值右移1位*/

    uiIIRValue &= 0x07;
    if(uiIIRValue == IIR_RLS)             /* 接收线状态中断 */
    {
        uiLSRValue = LPC_UART0->LSR;      /* 读取接收线状态寄存器*/
        /*
                         * 溢出中断、奇偶中断、帧中断、UART RBR中包含错误数据和帧间隔中断处理
                         */
        if(uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI))
        {
            UART0Status = uiLSRValue;
            uiDummy = LPC_UART0->RBR;
            return;
        }
        if(uiLSRValue & LSR_RDR)          /* 接收FIFO不为空中断*/
        {
            UART0Buffer[UART0Count] = LPC_UART0->RBR;
            UART0Count++;
            if(UART0Count == BUFSIZE)
            {
                UART0Count = 0;
            }
            Mt318RecDataCheck();
        }
    }
    else if(uiIIRValue == IIR_RDA)        /* 接收数据可用中断 */
    {
        UART0Buffer[UART0Count] = LPC_UART0->RBR;
        UART0Count++;
        if(UART0Count == BUFSIZE)
        {
            UART0Count = 0;
        }
        Mt318RecDataCheck();
    }
    else if(uiIIRValue == IIR_CTI)                                        /* 字符超时中断               */
    {
        UART0Status |= 0x100;
    }
    else if(uiIIRValue == IIR_THRE)                                       /* 发送保持寄存器空中断       */
    {
        uiLSRValue = LPC_UART0->LSR;
        if(uiLSRValue & LSR_THRE)
        {
            UART0TxEmpty = 1;
        }
        else
        {
            UART0TxEmpty = 0;
        }
    }
}

