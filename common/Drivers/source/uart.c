#include "kernel.h"

volatile uint32_t UART0Status, UART1Status;
volatile uint32_t UART2Status, UART3Status;
volatile uint32_t UART4Status;

volatile uint8_t UART0TxEmpty = 1, UART1TxEmpty = 1;
volatile uint8_t UART2TxEmpty = 1, UART3TxEmpty = 1;
volatile uint8_t UART4TxEmpty = 1;

uint8_t UART0Buffer[BUFSIZE], UART1Buffer[50];
volatile uint8_t UART2Buffer[BUFSIZE], UART3Buffer[BUFSIZE];
volatile uint8_t UART4Buffer[BUFSIZE];

volatile uint32_t UART0Count = 0, UART1Count = 0;
volatile uint32_t UART2Count = 0, UART3Count = 0;
volatile uint32_t UART4Count = 0;


uint8_t UART2_IRQ_FLAG = 0;
uint8_t UART3_IRQ_FLAG = 0;
uint8_t UART4_IRQ_FLAG = 0;





/*********************************************************************************************************
** �������ƣ�UART2_IRQHandler
** ����������UART2�жϴ�����
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void UART2_IRQHandler (void) 
{
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;

    uiIIRValue = LPC_UART2->IIR;

    uiIIRValue >>= 1;                          /* �жϱ�־�Ĵ���ֵ����1λ */
    uiIIRValue &= 0x07;

    if ( uiIIRValue == IIR_RLS ) {             /* ������״̬�ж� */
        uiLSRValue = LPC_UART2->LSR;
        /* 
         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) ) {
            UART2Status = uiLSRValue;
            uiDummy = LPC_UART2->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR ) {          /* ����FIFO��Ϊ���ж�*/
            UART2Buffer[UART2Count] = LPC_UART2->RBR;
            UART2Count++;
            if ( UART2Count == BUFSIZE ) {
                UART2Count = 0;
            }
        }
    }
    else if ( uiIIRValue == IIR_RDA ) {       /* �������ݿ����ж� */
        UART2Buffer[UART2Count] = LPC_UART2->RBR;
        UART2Count++;
        if ( UART2Count == BUFSIZE ) {
            UART2Count = 0;
        }
		if(UART2Count>=(UART2Buffer[0]+1)){
		    UART2_IRQ_FLAG = 1;
		}
    }
    else if ( uiIIRValue == IIR_CTI ) {       /* �ַ���ʱ�ж� */
        UART2Status |= 0x100;
    }
    else if ( uiIIRValue == IIR_THRE ) {      /* ���ͱ��ּĴ������ж�*/
        uiLSRValue = LPC_UART2->LSR;
        if ( uiLSRValue & LSR_THRE ) {
            UART2TxEmpty = 1;
        } else {
            UART2TxEmpty = 0;
        }
    }
}

/*********************************************************************************************************
** �������ƣ�UART3_IRQHandler
** ����������UART3�жϴ�����
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void UART3_IRQHandler (void) 
{
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;

    uiIIRValue = LPC_UART3->IIR;

    uiIIRValue >>= 1;                       /* �жϱ�־�Ĵ���ֵ����1λ*/
    uiIIRValue &= 0x07;

    if ( uiIIRValue == IIR_RLS ) {          /* ������״̬�ж�*/
        uiLSRValue = LPC_UART3->LSR;
        /* 
         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) ) {
            UART3Status = uiLSRValue;
            uiDummy = LPC_UART3->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR ) {       /* ����FIFO��Ϊ���ж� */
			UART3Buffer[UART3Count++] = LPC_UART3->RBR;
			if(UART3Buffer[0]!=0xaa)
			{
				UART3Count=0;
			}
        }
    }
    else if ( uiIIRValue == IIR_RDA ) {     /* �������ݿ����ж�*/
			UART3Buffer[UART3Count++] = LPC_UART3->RBR;
			if(UART3Buffer[0]!=0xaa)
			{
				UART3Count=0;
			}
    }
    else if ( uiIIRValue == IIR_CTI ) {     /* �ַ���ʱ�ж� */
        UART3Status |= 0x100;
    }
    else if ( uiIIRValue == IIR_THRE ) {    /* ���ͱ��ּĴ������ж� */
        uiLSRValue = LPC_UART3->LSR;
        if ( uiLSRValue & LSR_THRE ) {
            UART3TxEmpty = 1;
        } else {
            UART3TxEmpty = 0;
        }
    }
}


/*********************************************************************************************************
** �������ƣ�UART4_IRQHandler
** ����������UART4�жϴ�����
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void UART4_IRQHandler (void) 
{
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;

    uiIIRValue = LPC_UART4->IIR;

    uiIIRValue >>= 1;                        /* �жϱ�־�Ĵ���ֵ����1λ*/
    uiIIRValue &= 0x07;

    if ( uiIIRValue == IIR_RLS ) {           /* ������״̬�ж� */
        uiLSRValue = LPC_UART4->LSR;
        /* 
         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) ) {
            UART4Status = uiLSRValue;
            uiDummy = LPC_UART4->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR ) {         /* ����FIFO��Ϊ���ж� */
            UART4Buffer[UART4Count] = LPC_UART4->RBR;
            UART4Count++;
            if ( UART4Count == BUFSIZE ) {
                UART4Count = 0;
            }
        }
    }
    else if ( uiIIRValue == IIR_RDA ) {       /* �������ݿ����ж�*/
        UART4Buffer[UART4Count] = LPC_UART4->RBR;
        UART4Count++;
        if ( UART4Count == BUFSIZE ) {
            UART4Count = 0;
        }
		if(UART4Count>=(UART4Buffer[0]+1)){
			UART4_IRQ_FLAG = 1;
		}
    }
    else if ( uiIIRValue == IIR_CTI ) {       /* �ַ���ʱ�ж� */
        UART4Status |= 0x100;
    }
    else if ( uiIIRValue == IIR_THRE ) {      /* ���ͱ��ּĴ������ж� */
        uiLSRValue = LPC_UART4->LSR;
        if ( uiLSRValue & LSR_THRE ) {
            UART4TxEmpty = 1;
        } else {
            UART4TxEmpty = 0;
        }
    }
}

/*********************************************************************************************************
** �������ƣ�UARTInit
** �������������ڳ�ʼ��
** ���������PortNum    UART�˿ں�
**           bandrate   �趨�Ĵ��ڲ�����
** ����ֵ  ����
*********************************************************************************************************/
uint32_t UARTInit( uint32_t PortNum, uint32_t baudrate )
{
    uint32_t Fdiv;

    if ( PortNum == 0 ) {
        //SELECT_CARD();   
        //���蹦�ʿ��ƼĴ��� 32λ
		LPC_SC->PCONP |= 0x00000008;				 

        LPC_IOCON->P0_2  &= ~0x07;
        LPC_IOCON->P0_2  |= 1;                   /* P0.2ΪU0_TXD  */
        LPC_IOCON->P0_3  &= ~0x07;
        LPC_IOCON->P0_3  |= 1;                   /* P0.3ΪU0_RXD  */
        
        /*
                        * 8Ϊ����λ������ż����λ��1λֹͣλ
                         */
        LPC_UART0->LCR = 0x83;
        Fdiv = ( PeripheralClock / 16 ) / baudrate ;     /*�����ֵ  */
        LPC_UART0->DLM = Fdiv / 256;
        LPC_UART0->DLL = Fdiv % 256;
        LPC_UART0->LCR = 0x03;                   /* ������������*/
        LPC_UART0->FCR = 0x07;                   /* ʹ�ܲ���λFIFO*/
        NVIC_SetPriority(UART0_IRQn,4); 
        NVIC_EnableIRQ(UART0_IRQn);
        /* 
                        * ʹ�ܽ���FIFO��Ϊ���жϡ����ͱ��ּĴ������жϡ�������״̬�ж�
                         */
        LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;
        return (TRUE);
    }
    else if ( PortNum == 1 ) {
        LPC_SC->PCONP |= 0x00000010;

        LPC_IOCON->P2_0  &= ~0x07;
        LPC_IOCON->P2_0  |= 2;                   /* P2.0ΪU1_TXD */
        LPC_IOCON->P2_1  &= ~0x07;
        LPC_IOCON->P2_1  |= 2;                   /* P2.1ΪU1_TXD */

        /*
                         * 8λ����λ��ż����λ��1λֹͣλ
                        */
//       LPC_UART1->LCR = 0x83;
        LPC_UART1->LCR = 0x03<<0|0x01<<3|0x01<<4|0x01<<7;
        Fdiv = ( PeripheralClock / 16 ) / baudrate ;   /*�����ֵ */
        LPC_UART1->DLM = Fdiv / 256;
        LPC_UART1->DLL = Fdiv % 256;
//        LPC_UART1->LCR = 0x03;                                                                 /* ������������ */
		LPC_UART1->LCR = 0x03<<0|0x01<<3|0x01<<4;
        LPC_UART1->FCR = 0x07;                   /* ʹ�ܲ���λFIFO  */

        LPC_IOCON->P2_5 &=~0x07;
        LPC_IOCON->P2_5 |= 0x02;

        LPC_UART1->RS485CTRL = 0x38;    //��·������������ڶ���������
        LPC_UART1->RS485DLY = 0;
        NVIC_SetPriority(UART0_IRQn,3); 
        NVIC_EnableIRQ(UART1_IRQn);
        /* 
                        * ʹ�ܽ���FIFO��Ϊ���жϡ����ͱ��ּĴ������жϡ�������״̬�ж�
                         */
//        LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;
        LPC_UART1->IER = IER_RBR  | IER_RLS;				
        return (TRUE);
    }
    else if ( PortNum == 2 ) {
        LPC_SC->PCONP |= 0x01000000;

        LPC_IOCON->P0_10 &= ~0x07;
        LPC_IOCON->P0_10 |= 1;                   /* P0.10ΪU2_TXD  */
        LPC_IOCON->P0_11 &= ~0x07;
        LPC_IOCON->P0_11 |= 1;                   /* P0.11ΪU2_RXD  */
        LPC_IOCON->P1_19 &= ~0x07;
        LPC_IOCON->P1_19 |= 6;                   /* P1.19ΪU2_485�Զ����� */
        /*
                         * 8Ϊ����λ������ż����λ��1λֹͣλ
                        */
        LPC_UART2->LCR = 0x83;        
        Fdiv = ( PeripheralClock / 16 ) / baudrate ; /*�����ֵ */
        LPC_UART2->DLM = Fdiv / 256;
        LPC_UART2->DLL = Fdiv % 256;
        LPC_UART2->LCR = 0x03;                   /* ������������ */
        LPC_UART2->FCR = 0x07;                   /* ʹ�ܲ���λFIFO*/
        LPC_UART2->RS485CTRL = 0x10;
        LPC_UART2->RS485DLY = 0;
        NVIC_EnableIRQ(UART2_IRQn);
        /* 
                         * ʹ�ܽ���FIFO��Ϊ���жϡ����ͱ��ּĴ������жϡ�������״̬�ж�
                         */
        LPC_UART2->IER = IER_RBR | IER_THRE | IER_RLS;
				
        return (TRUE);
    }
    else if ( PortNum == 3 ) {
        LPC_SC->PCONP |= 0x02000000;

        LPC_IOCON->P4_28 &= ~0x07;
        LPC_IOCON->P4_28 |= 2;                   /* P4.28ΪU3_TXD */
        LPC_IOCON->P4_29 &= ~0x07;
        LPC_IOCON->P4_29 |= 2;                   /* P4.29ΪU3_RXD*/

        /*
                        * 8Ϊ����λ������ż����λ��1λֹͣλ
                          */
        LPC_UART3->LCR = 0x83;
        Fdiv = ( PeripheralClock / 16 ) / baudrate ; /*�����ֵ */
        LPC_UART3->DLM = Fdiv / 256;
        LPC_UART3->DLL = Fdiv % 256;
        LPC_UART3->LCR = 0x03;                   /* ������������ */
        LPC_UART3->FCR = 0x07;                   /* ʹ�ܲ���λFIFO*/
        
        NVIC_EnableIRQ(UART3_IRQn);
        NVIC_SetPriority(UART3_IRQn, 5);
        /* 
                         * ʹ�ܽ���FIFO��Ϊ���жϡ����ͱ��ּĴ������жϡ�������״̬�ж�
                        */
        LPC_UART3->IER = IER_RBR | IER_THRE | IER_RLS;				
        return (TRUE);
    }
    else if ( PortNum == 4 ) {
        LPC_SC->PCONP |= 0x00000100;

        LPC_IOCON->P1_29 &= ~0x07;
        LPC_IOCON->P1_29 |= 5;                   /* P1.29ΪU4_TXD */
		LPC_IOCON->P5_3  &= ~0x07;
        LPC_IOCON->P5_3  |= 4;                   /* P5.3ΪU4_RXD  */            
        LPC_IOCON->P0_21 &= ~0x07;
        LPC_IOCON->P0_21 |= 3;                   /* P0.21ΪU4_OE  */ 
        /*
                        * 8Ϊ����λ������ż����λ��1λֹͣλ
                        */
        LPC_UART4->LCR = 0x83;                          
        Fdiv = ( PeripheralClock / 16 ) / baudrate ; /*�����ֵ */
        LPC_UART4->DLM = Fdiv / 256;
        LPC_UART4->DLL = Fdiv % 256;
        LPC_UART4->LCR = 0x03;                   /* ������������ */
        LPC_UART4->FCR = 0x07;                   /* ʹ�ܲ���λFIFO */
        LPC_UART4->RS485CTRL = 0x10;
        LPC_UART4->RS485DLY = 0;
        NVIC_EnableIRQ(UART4_IRQn);
        /* 
                        * ʹ�ܽ���FIFO��Ϊ���жϡ����ͱ��ּĴ������жϡ�������״̬�ж�
                         */
        LPC_UART4->IER = IER_RBR | IER_THRE | IER_RLS;
        return (TRUE);
    }
    return( FALSE );
}


/*********************************************************************************************************
** �������ƣ�UARTSend
** �������������ڷ�������
** ���������PortNum    UART�˿ں�
**           BufferPtr  �������ݻ�����
**           Length     �������ݳ���
** ����ֵ  ����
*********************************************************************************************************/
void UARTSend( uint32_t portNum, uint8_t *BufferPtr, uint32_t Length )
{
    if ( portNum == 0 ) {
        while ( Length != 0 ) {
            while ( !(UART0TxEmpty & 0x01) );   /* �ȴ����ͱ�־Ϊ1  */
            LPC_UART0->THR = *BufferPtr;        /* ��������*/
            UART0TxEmpty = 0;                   /* ���ͱ�־��0 */
            BufferPtr++;                        /* ָ����� */
            Length--;                           /* ������Ŀ��1 */
        }
    }
    else if ( portNum == 1 ) {
        while ( Length != 0 ) {
            while ( !(UART1TxEmpty & 0x01) );   /* �ȴ����ͱ�־Ϊ1 */
            LPC_UART1->THR = *BufferPtr;        /* �������� */
            UART1TxEmpty = 0;                   /* ���ͱ�־��0 */
            BufferPtr++;                        /* ָ����� */
            Length--;                           /* ������Ŀ��1  */
        }
    }
    else if ( portNum == 2 ) {
        while ( Length != 0 ) {
            while ( !(UART2TxEmpty & 0x01) );   /* �ȴ����ͱ�־Ϊ1 */
            LPC_UART2->THR = *BufferPtr;        /* �������� */
            UART2TxEmpty = 0;                   /* ���ͱ�־��0 */
            BufferPtr++;                        /* ָ�����*/
            Length--;                           /* ������Ŀ��1*/
        }
    }
    else if ( portNum == 3 ) {
        while ( Length != 0 ) {
            while ( !(UART3TxEmpty & 0x01) );   /* �ȴ����ͱ�־Ϊ1 */
            LPC_UART3->THR = *BufferPtr;        /* ��������*/
            UART3TxEmpty = 0;                   /* ���ͱ�־��0*/
            BufferPtr++;                        /* ָ����� */
            Length--;                           /* ������Ŀ��1 */
        }
    }
    else if ( portNum == 4 ) {
        while ( Length != 0 ) {
            while ( !(UART4TxEmpty & 0x01) );  /* �ȴ����ͱ�־Ϊ1  */
            LPC_UART4->THR = *BufferPtr;        /* �������� */
            UART4TxEmpty = 0;                   /* ���ͱ�־��0 */
            BufferPtr++;                        /* ָ�����*/
            Length--;                           /* ������Ŀ��1*/
        }
    }
    return;
}

/*********************************************************************************************************
** �������ƣ�uart0SendByte
** �����������򴮿�0�����ֽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** ���������ucDat:   Ҫ���͵�����
** ����ֵ  ����
*********************************************************************************************************/
void uart0SendByte(uint8_t ucDat)
{
    LPC_UART0->THR = ucDat;                 /* д������*/
    while((LPC_UART0->LSR & 0x40) == 0);    /* �ȴ����ݷ������*/
}

/*********************************************************************************************************
** �������ƣ�uart0SendStr
** �����������򴮿�0�����ַ���
** ���������puiStr:  Ҫ���͵��ַ���ָ��
** ����ֵ  ����
*********************************************************************************************************/
void uart0SendStr(uint8_t *puiStr, uint32_t len)
{
    while(len -- > 0)                       /* �������� */
    {
        uart0SendByte(*puiStr++);           /* �����ֽ�����*/
    }
}
void uart1SendByte(uint8_t ucDat)
{
    LPC_UART1->THR = ucDat;               /* д������*/
    while((LPC_UART1->LSR & 0x40) == 0);  /* �ȴ����ݷ������*/
}

void uart1SendStr(uint8_t *puiStr, uint32_t len)
{
    while(len -- > 0)                     /* ��������*/
    {
        uart1SendByte(*puiStr++);         /* �����ֽ�����*/
    }  
}

void uart2SendByte (uint8_t ucDat)
{
    LPC_UART2->THR = ucDat;                 /* д������ */
    while ((LPC_UART2->LSR & 0x40) == 0);   /* �ȴ����ݷ������ */
}
void uart2SendStr (uint8_t *puiStr, uint32_t len)
{
	while (len -- > 0) {                                                  /* ��������                     */
        uart2SendByte (*puiStr++);          /* �����ֽ�����*/
    }
}
void uart3SendByte (uint8_t ucDat)
{
    LPC_UART3->THR = ucDat;                 /* д������ */
    while ((LPC_UART3->LSR & 0x40) == 0);   /* �ȴ����ݷ������ */
}
void uart3SendStr (uint8_t *puiStr, uint32_t len)
{
	while (len -- > 0) {                                                  /* ��������                     */
        uart3SendByte (*puiStr++);          /* �����ֽ�����*/
    }
}
void uart4SendByte (uint8_t ucDat)
{
    LPC_UART4->THR = ucDat;                 /* д������ */
    while ((LPC_UART4->LSR & 0x40) == 0);   /* �ȴ����ݷ������ */
}

void uart4SendStr (uint8_t *puiStr, uint32_t len)
{
	while (len -- > 0) {                                                  /* ��������                     */
        uart4SendByte (*puiStr++);          /* �����ֽ�����*/
    }
}

void UartSendByte( uint32_t portNum,uint8_t ucDat)
{
    if((portNum==CardPort)||(portNum==PrintPort)){uart0SendByte(ucDat);}
    else if(portNum==ModbusPort){uart1SendByte(ucDat);}
    else if(portNum==LCDPort){uart2SendByte(ucDat);}
    else if(portNum==PCPort){uart3SendByte(ucDat);}
    else if(portNum==RfidPort){uart4SendByte(ucDat);}
}

/*********************************************************************************************************
** �������ƣ�UartSendStr
** �������������ڷ�������
** ���������
**           PortNum    UART�˿ں�
**           BufferPtr  �������ݻ�����
**           Length     �������ݳ���
** ����ֵ  ����
*********************************************************************************************************/
void UartSendStr( uint32_t portNum, uint8_t *BufferPtr, uint32_t Length )
{
    if((portNum==CardPort)||(portNum==PrintPort)){uart0SendStr(BufferPtr, Length);}
    else if(portNum==ModbusPort){uart1SendStr(BufferPtr, Length);}
    else if(portNum==LCDPort){uart2SendStr(BufferPtr, Length);}
    else if(portNum==PCPort){uart3SendStr(BufferPtr, Length);}
    else if(portNum==RfidPort){uart4SendStr(BufferPtr, Length);}
}


/***************************************************************************
   **�������ƣ� OpenUartRcvIRQ
   **�������ܣ��򿪴��ڽ����ж�
   **��ڲ�����portnum:���ں�
   **����ֵ��    ��
   **�����ߣ�    ���
   **�������ڣ�2016-3-31 09:47:41

   **�޸��ߣ�
   **�޸����ڣ�
   **�޸����ݣ�
   ***************************************************************************/
void OpenUartRcvIRQ(uint8_t  portnum)
{   
    if(portnum==0)
        LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;
    else if(portnum==1)
        LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;
    else if(portnum==2)
        LPC_UART2->IER = IER_RBR | IER_THRE | IER_RLS;
    else if(portnum==3)
        LPC_UART3->IER = IER_RBR | IER_THRE | IER_RLS;
    else if(portnum==4)
        LPC_UART4->IER = IER_RBR | IER_THRE | IER_RLS;    
}

/***************************************************************************
   **�������ƣ� CloseUartRcvIRQ
   **�������ܣ��رմ��ڽ����ж�
   **��ڲ�����portnum:���ں�
   **����ֵ��    ��
   **�����ߣ�    ���
   **�������ڣ�2016-3-31 09:47:41

   **�޸��ߣ�
   **�޸����ڣ�
   **�޸����ݣ�
   ***************************************************************************/
void CloseUartRcvIRQ(uint8_t  portnum)
{   
    if(portnum==0)
        LPC_UART0->IER = 0;
    else if(portnum==1)
        LPC_UART1->IER = 0;
    else if(portnum==2)
        LPC_UART2->IER = 0;
    else if(portnum==3)
        LPC_UART3->IER = 0;
    else if(portnum==4)
        LPC_UART4->IER = 0;
}

/*********************************************************************************************************
**                            End Of File
*********************************************************************************************************/

