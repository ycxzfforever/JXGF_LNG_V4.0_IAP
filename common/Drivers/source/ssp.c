#include "kernel.h"


/* 
 * �ж�״̬��־
 */
volatile uint32_t interrupt0RxStat = 0;
volatile uint32_t interrupt0OverRunStat = 0;
volatile uint32_t interrupt0RxTimeoutStat = 0;
volatile uint32_t interrupt1RxStat = 0;
volatile uint32_t interrupt1OverRunStat = 0;
volatile uint32_t interrupt1RxTimeoutStat = 0;
volatile uint32_t interrupt2RxStat = 0;
volatile uint32_t interrupt2OverRunStat = 0;
volatile uint32_t interrupt2RxTimeoutStat = 0;


/*********************************************************************************************************
** �������ƣ�SSP0_IRQHandler
** ����������SSP0�жϺ���
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void SSP0_IRQHandler(void)
{
    uint32_t regValue;

    regValue = LPC_SSP0->MIS;
    if (regValue & SSPMIS_RORMIS) {                                     /* ����ж�                     */
        interrupt0OverRunStat++;
        LPC_SSP0->ICR = SSPICR_RORIC;
    }
    if (regValue & SSPMIS_RTMIS) {                                      /* ���ճ�ʱ�ж�                 */
        interrupt0RxTimeoutStat++;
        LPC_SSP0->ICR = SSPICR_RTIC;
    }
    if (regValue & SSPMIS_RXMIS) {                                      /* RX FIFO������һ��Ϊ��        */
        interrupt0RxStat++;
    }
    return;
}


/*********************************************************************************************************
** �������ƣ�SSP1_IRQHandler
** ����������SSP1�жϺ���
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void SSP1_IRQHandler(void) 
{
    uint32_t regValue;

    regValue = LPC_SSP1->MIS;
    if (regValue & SSPMIS_RORMIS) {                                     /* ����ж�                     */
        interrupt1OverRunStat++;
        LPC_SSP1->ICR = SSPICR_RORIC;
    }
    if (regValue & SSPMIS_RTMIS) {                                      /* ���ճ�ʱ�ж�                 */
        interrupt1RxTimeoutStat++;
        LPC_SSP1->ICR = SSPICR_RTIC;
    }
    if (regValue & SSPMIS_RXMIS) {                                      /* RX FIFO������һ��Ϊ��        */
        interrupt1RxStat++;
    }
    return;
}


/*********************************************************************************************************
** �������ƣ�SSP2_IRQHandler
** ����������SSP2�жϺ���
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void SSP2_IRQHandler(void) 
{
    uint32_t regValue;

    regValue = LPC_SSP2->MIS;
    if (regValue & SSPMIS_RORMIS) {                                     /* ����ж�                     */
        interrupt2OverRunStat++;
        LPC_SSP2->ICR = SSPICR_RORIC;
    }
    if (regValue & SSPMIS_RTMIS) {                                      /* ���ճ�ʱ�ж�                 */
        interrupt2RxTimeoutStat++;
        LPC_SSP2->ICR = SSPICR_RTIC;
    }
    if (regValue & SSPMIS_RXMIS) {                                      /* RX FIFO������һ��Ϊ��        */
        interrupt2RxStat++;
    }
    return;
}


/*********************************************************************************************************
** �������ƣ�SSP_SSELSetDirection
** ����������ѡ��SSP��CS�ܽţ�ʵ�ֶ����ƽ�ɿ�
** ���������uiPortNum      SSP�˿ں�
**           uiLocation   CS�ܽ�λ��
** ����ֵ  ����
*********************************************************************************************************/
void SSP_SSELSetDirection(uint32_t uiPortNum, uint32_t uiLocation)
{
    if (uiPortNum == 0) {
        switch ( uiLocation ) {
        case 0:
            LPC_IOCON->P2_23 &= ~0x07;                                  /* P2.23ΪSSP0��CS�ܽ�          */
            LPC_GPIO2->DIR |= (0x1<<23);
            LPC_GPIO2->SET |= (0x1<<23);
            break;
        case 1:
            LPC_IOCON->P1_21 &= ~0x07;                                  /* P1.21ΪSSP0��CS�ܽ�          */
            LPC_GPIO1->DIR |= (0x1<<21);
            LPC_GPIO1->SET |= (0x1<<21);
            break;
        case 2:
            LPC_IOCON->P1_28 &= ~0x07;                                  /* P1.28ΪSSP0��CS�ܽ�          */
            LPC_GPIO1->DIR |= (0x1<<28);
            LPC_GPIO1->SET |= (0x1<<28);
            break;
        case 3:
            LPC_IOCON->P0_16 &= ~0x07;                                  /* P0.16ΪSSP0��CS�ܽ�          */
            LPC_GPIO0->DIR |= (0x1<<16);
            LPC_GPIO0->SET |= (0x1<<16);
            break;
        }
    }
    else if (uiPortNum == 1) {
        switch (uiLocation) {
        case 0:
            LPC_IOCON->P0_14 &= ~0x07;                                  /* P0.14ΪSSP1��CS�ܽ�          */
            LPC_GPIO0->DIR |= (0x1<<14);
            LPC_GPIO0->SET |= (0x1<<14);
            break;
        case 1:
            LPC_IOCON->P1_26 &= ~0x07;                                  /* P1.26ΪSSP1��CS�ܽ�          */
            LPC_GPIO1->DIR |= (0x1<<26);
            LPC_GPIO1->SET |= (0x1<<26);
            break;
        case 2:
            LPC_IOCON->P4_21 &= ~0x07;                                  /* P4.21ΪSSP1��CS�ܽ�          */
            LPC_GPIO4->DIR |= (0x1<<21);
            LPC_GPIO4->SET |= (0x1<<21);
            break;
        case 3:
            LPC_IOCON->P0_6 &= ~0x07;                                  /* P0.6ΪSSP1��CS�ܽ�           */
            LPC_GPIO0->DIR |= (0x1<<6);
            LPC_GPIO0->SET |= (0x1<<6);
            break;
       }
    }
    else if ( uiPortNum == 2 ) {
        switch ( uiLocation ) {
        case 0:
            LPC_IOCON->P5_3 &= ~0x07;                                  /* P5.3ΪSSP2��CS�ܽ�           */
            LPC_GPIO5->DIR |= (0x1<<3);
            LPC_GPIO5->SET |= (0x1<<3);
            break;
        case 1:
            LPC_IOCON->P1_8 &= ~0x07;                                  /* P1.8ΪSSP2��CS�ܽ�           */
            LPC_GPIO1->DIR |= (0x1<<8);
            LPC_GPIO1->SET |= (0x1<<8);
            break;
        default:
            break;
        }
    }
    return;
}


/*********************************************************************************************************
** �������ƣ�SSP_SSELToggle
** ��������������SSP��CS�ܽŵ�ƽ
** ���������uiPortNum    SSP�˿ں�
**           uiLocation   SSEL�ܽ�λ��
**           toggle       SSEL�ܽŵ�ƽ���ƣ�0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
** ����ֵ  ����
*********************************************************************************************************/
void SSP_SSELToggle( uint32_t uiPortNum, uint32_t uiLocation, uint32_t toggle )
{
    if ( uiPortNum == 0 ) {
        switch ( uiLocation ) {
        case 0:
            if ( !toggle )
                LPC_GPIO2->CLR |= (0x1<<23);                            /* P2.23ΪCS����Ϊ�͵�ƽ        */
            else
                LPC_GPIO2->SET |= (0x1<<23);                            /* P2.23ΪCS����Ϊ�ߵ�ƽ        */
            break;
        case 1:
            if ( !toggle )
                LPC_GPIO1->CLR |= (0x1<<21);                            /* P1.21ΪCS����Ϊ�͵�ƽ        */
            else
                LPC_GPIO1->SET |= (0x1<<21);                            /* P1.21ΪCS����Ϊ�ߵ�ƽ        */
            break;
        case 2:
            if ( !toggle )
                LPC_GPIO1->CLR |= (0x1<<28);                            /* P1.28ΪCS����Ϊ�͵�ƽ        */
            else
                LPC_GPIO1->SET |= (0x1<<28);                            /* P1.28ΪCS����Ϊ�ߵ�ƽ        */
            break;
        case 3:
            if ( !toggle )
                LPC_GPIO0->CLR |= (0x1<<16);                            /* P0.16ΪCS����Ϊ�͵�ƽ        */
            else
                LPC_GPIO0->SET |= (0x1<<16);                            /* P0.16ΪCS����Ϊ�ߵ�ƽ        */
            break;
        }
    }
    else if ( uiPortNum == 1 ) {
        switch ( uiLocation ) {
        case 0:
            if ( !toggle )
                LPC_GPIO0->CLR |= (0x1<<14);                            /* P0.14ΪCS����Ϊ�͵�ƽ        */
            else
                LPC_GPIO0->SET |= (0x1<<14);                            /* P0.17ΪCS����Ϊ�ߵ�ƽ        */
            break;
        case 1:
            if ( !toggle )
                LPC_GPIO1->CLR |= (0x1<<26);                            /* P1.26ΪCS����Ϊ�͵�ƽ        */
            else
                LPC_GPIO1->SET |= (0x1<<26);                            /* P1.26ΪCS����Ϊ�ߵ�ƽ        */
            break;
        case 2:
            if ( !toggle )
                LPC_GPIO4->CLR |= (0x1<<21);                            /* P4.21ΪCS����Ϊ�͵�ƽ        */
            else
                LPC_GPIO4->SET |= (0x1<<21);                            /* P4.21ΪCS����Ϊ�ߵ�ƽ        */
            break;
        case 3:
            if ( !toggle )
                LPC_GPIO0->CLR |= (0x1<<6);                            /* P0.6ΪCS����Ϊ�͵�ƽ         */
            else
                LPC_GPIO0->SET |= (0x1<<6);                            /* P0.6ΪCS����Ϊ�ߵ�ƽ         */
            break;
        }
    }
    else if ( uiPortNum == 2 ) {
        switch ( uiLocation ) {
        case 0:
            if ( !toggle )
                LPC_GPIO5->CLR |= (0x1<<3);                            /* P5.3ΪCS����Ϊ�͵�ƽ         */
            else
                LPC_GPIO5->SET |= (0x1<<3);                            /* P5.3ΪCS����Ϊ�ߵ�ƽ         */
            break;
        case 1:
            if ( !toggle )
                LPC_GPIO1->CLR |= (0x1<<8);                            /* P1.8ΪCS����Ϊ�͵�ƽ         */
            else
                LPC_GPIO1->SET |= (0x1<<8);                            /* P1.8ΪCS����Ϊ�ߵ�ƽ         */
            break;
        default:
            break;
        }
    }
    return;
}


/***************************************************************************
**�������ƣ�Fm25L04AndFlashSpiInit
**�������ܣ���ʼ��Fm25L04��Flash�Ĺ���Spi0�˿ڡ�
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void Fm25L04AndFlashSpiInit(void)
{
    uint8_t i = 0; 
    uint8_t uiDummy = uiDummy = 0;

    LPC_SC->PCONP |= (0x1 << 21);                                       /* ����SSP0����                 */


    LPC_IOCON->P0_15 &= ~0x07;
    LPC_IOCON->P0_15 |=  0x02;                                           /* SSP CLK                      */
    LPC_IOCON->P0_16 &= ~0x07;    
    LPC_IOCON->P0_16 |=  0x02;                                           /* SSP SSEL                     */
    LPC_IOCON->P0_17 &= ~0x07;
    LPC_IOCON->P0_17 |=  0x02;                                           /* SSP MISO                     */
    LPC_IOCON->P0_18 &= ~0x07;    
    LPC_IOCON->P0_18 |=  0x02;                                           /* SSP MOSI                     */


    /* 
     * ��ʹ�ùܽŵ�CS����ʱ���˹�����ʵ��ʹ�ܿ���
     */
    #if USE_CS
        SSP_SSELSetDirection( 0,3 );
    #endif

    LPC_SSP0->CR0 = 0x07 << 0 |                                         /* ���ݳ���Ϊ8λ                */
                    0x00 << 4 |                                         /* ֡��ʽΪSPI                  */
                    0x01 << 6 |                                         /* CPOLΪ1                      */
                    0x01 << 7 |                                         /* CPHAΪ1  ZLG9518S������ģʽ3 */
                    0x07 << 8;                                          /* ����ʱ������Ϊ7              */

    LPC_SSP0->CPSR = 0x02;                                              /* ʱ��Ԥ��ƵֵΪ2              */
    
    for (i = 0; i < FIFOSIZE; i++) {
        uiDummy = LPC_SSP0->DR;                                         /* ��0����FIFO                  */
    }

	NVIC_EnableIRQ(SSP0_IRQn);                                          /* ʹ��SSP0�ж�                 */
	  
                                                                        /* ����ģʽ                     */
    LPC_SSP0->CR1 = (0x00 << 3) |                                       /*	�ӻ�������ܡ���λֻ��ӻ�ģʽ�йأ�MS=1���������λΪ1����ֹSSP�������������������ߣ�MISO��*/                                        
                    (0x00 << 2) |                                       /*	SSP����������һ����������������SCLK��MOSI��SSEL�߲�����MISO��	*/                                        
                    (0x01 << 1) |                                       /*	SSP������ʹ��	*/                                        
                    (0x00 << 0);                                        /*	��������ģʽ	*/                                       
    LPC_SSP0->ICR  = 0x03;        	                                    /* SSPICR�ж�����Ĵ���  */                                           
   

    LPC_SSP0->IMSC = SSPIMSC_RORIM | SSPIMSC_RTIM;                      /* ʹ������жϺͽ��ճ�ʱ�ж�   */
    return;
}


/*********************************************************************************************************
** �������ƣ�SSP1Init
** ����������SSP1��ʼ��
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void SSP1Init( uint32_t uiLocation )
{
    uint8_t i = 0, uiRegVal = 0;
    uint8_t uiDummy = uiDummy = 0; 
    
    LPC_SC->PCONP |= (0x1<<10);                                         /* ����SSP1����                 */
    
    /* 
     * �趨�����Ƶֵ
     */
    uiRegVal = LPC_SC->PCLKSEL;
    if ( uiRegVal < 4 ) {
        LPC_SC->PCLKSEL = 4;
    }

    /* 
     * ѡ���ض�λ�ù��ܹܽ�
     */
    switch (uiLocation) {
    case 0:
        LPC_IOCON->P0_7  &= ~0x07;
        LPC_IOCON->P0_7  |=  0x02;                                      /* SSP CLK                      */
        LPC_IOCON->P0_14 &= ~0x07;    
        LPC_IOCON->P0_14 |=  0x02;                                      /* SSP SSEL                     */
        LPC_IOCON->P0_12 &= ~0x07;
        LPC_IOCON->P0_12 |=  0x02;                                      /* SSP MISO                     */
        LPC_IOCON->P0_13 &= ~0x07;    
        LPC_IOCON->P0_13 |=  0x02;                                      /* SSP MOSI                     */
        break;

    case 1:
        LPC_IOCON->P1_19 &= ~0x07;
        LPC_IOCON->P1_19 |=  0x05;                                      /* SSP CLK                      */
        LPC_IOCON->P1_26 &= ~0x07;    
        LPC_IOCON->P1_26 |=  0x05;                                      /* SSP SSEL                     */
        LPC_IOCON->P1_18 &= ~0x07;
        LPC_IOCON->P1_18 |=  0x05;                                      /* SSP MISO                     */
        LPC_IOCON->P1_22 &= ~0x07;    
        LPC_IOCON->P1_22 |=  0x05;                                      /* SSP MOSI                     */
        break;

    case 2:
        LPC_IOCON->P4_20 &= ~0x07;
        LPC_IOCON->P4_20 |=  0x03;                                      /* SSP CLK                      */
        LPC_IOCON->P4_21 &= ~0x07;    
        LPC_IOCON->P4_21 |=  0x03;                                      /* SSP SSEL                     */
        LPC_IOCON->P4_22 &= ~0x07;
        LPC_IOCON->P4_22 |=  0x03;                                      /* SSP MISO                     */
        LPC_IOCON->P4_23 &= ~0x07;    
        LPC_IOCON->P4_23 |=  0x03;                                      /* SSP MOSI                     */
        break;

    case 3:
        LPC_IOCON->P0_7 &= ~0x07;
        LPC_IOCON->P0_7 |=  0x02;                                       /* SSP CLK                      */
        LPC_IOCON->P0_6 &= ~0x07;    
        LPC_IOCON->P0_6 |=  0x02;                                       /* SSP SSEL                     */
        LPC_IOCON->P0_8 &= ~0x07;
        LPC_IOCON->P0_8 |=  0x02;                                       /* SSP MISO                     */
        LPC_IOCON->P0_9 &= ~0x07;    
        LPC_IOCON->P0_9 |=  0x02;                                       /* SSP MOSI                     */
        break;
    default :
        break;
    }
    
    /* 
     * ��ʹ�ùܽŵ�CS����ʱ���˹�����ʵ��ʹ�ܿ���
     */
    #if USE_CS
        SSP_SSELSetDirection( 1, uiLocation );
    #endif

    LPC_SSP1->CR0 = 0x07 << 0 |                                         /* ���ݳ���Ϊ8λ                */
                    0x00 << 4 |                                         /* ֡��ʽΪSPI                  */
                    0x00 << 6 |                                         /* CPOLΪ0                      */
                    0x00 << 7 |                                         /* CPHAΪ0                      */
                    0x07 << 8;                                          /* ����ʱ������Ϊ7              */

    LPC_SSP1->CPSR = 0x2;                                               /* ʱ��Ԥ��ƵֵΪ2              */
    
    for (i = 0; i < FIFOSIZE; i++) {
        uiDummy = LPC_SSP1->DR;                                         /* ��0����FIFO                  */
    }
    

    NVIC_EnableIRQ(SSP1_IRQn);                                          /* ʹ��SSP1�ж�                 */
//    NVIC_DisableIRQ(SSP1_IRQn);                                          /* ʹ��SSP0�ж�                 */

    #if LOOPBACK_MODE                                                   /* ��·ģʽ                     */
        LPC_SSP1->CR1 = SSPCR1_LBM | SSPCR1_SSE;
    #elif SSP_SLAVE1                                                     /* �ӻ�ģʽ                     */
        if ( LPC_SSP1->CR1 & SSPCR1_SSE ) {
            LPC_SSP1->CR1 &= ~SSPCR1_SSE;                               /* ����SSP1������               */
        }
        LPC_SSP1->CR1 = SSPCR1_MS;                                      /* ѡ��SSP1���ߴӻ�             */
        LPC_SSP1->CR1 |= SSPCR1_SSE;                                    /* ʹ��SSP1������               */
    #else                                                               /* ����ģʽ                     */
        LPC_SSP1->CR1 = SSPCR1_SSE;

//		LPC_SSP1->CR1 = (0x00 << 3) |    /*	�ӻ�������ܡ���λֻ��ӻ�ģʽ�йأ�MS=1���������λΪ1����ֹSSP�������������������ߣ�MISO��*/                                        
//						(0x00 << 2) |    /*	SSP����������һ����������������SCLK��MOSI��SSEL�߲�����MISO��	*/                                        
//						(0x01 << 1) |    /*	SSP������ʹ��	*/                                        
//						(0x00 << 0);     /*	��������ģʽ	*/                                       
////    	LPC_SSP1->ICR  = 0x03;        	 /* SSPICR�ж�����Ĵ���  */                                           

    #endif

    LPC_SSP1->IMSC = SSPIMSC_RORIM | SSPIMSC_RTIM;                      /* ʹ������жϺͽ��ճ�ʱ�ж�   */
    return;
}


/*********************************************************************************************************
** �������ƣ�SSP2Init
** ����������SSP2��ʼ��
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void SSP2Init( uint32_t uiLocation )
{
    uint8_t i = 0, uiRegVal = 0;
    uint8_t uiDummy = uiDummy = 0;
    
    LPC_SC->PCONP |= (0x1<<20);                                         /* ����SSP2����                 */
    
    /* 
     * �趨�����Ƶֵ
     */
    uiRegVal = LPC_SC->PCLKSEL;
    if (uiRegVal < 4) {
        LPC_SC->PCLKSEL = 4;
    }
    
    /* 
     * ѡ���ض�λ�ù��ܹܽ�
     */
    switch (uiLocation) {
    case 0:
        LPC_IOCON->P5_2 &= ~0x07;
        LPC_IOCON->P5_2 |=  0x02;                                       /* SSP CLK                      */
        LPC_IOCON->P5_3 &= ~0x07;    
        LPC_IOCON->P5_3 |=  0x02;                                       /* SSP SSEL                     */
        LPC_IOCON->P5_1 &= ~0x07;
        LPC_IOCON->P5_1 |=  0x02;                                       /* SSP MISO                     */
        LPC_IOCON->P5_0 &= ~0x07;    
        LPC_IOCON->P5_0 |=  0x02;                                       /* SSP MOSI                     */
        break;

    case 1:
        LPC_IOCON->P1_0 &= ~0x07;
        LPC_IOCON->P1_0 |=  0x04;                                       /* SSP CLK                      */
        LPC_IOCON->P1_8 &= ~0x07;    
        LPC_IOCON->P1_8 |=  0x04;                                       /* SSP SSEL                     */
        LPC_IOCON->P1_4 &= ~0x07;
        LPC_IOCON->P1_4 |=  0x04;                                       /* SSP MISO                     */
        LPC_IOCON->P1_1 &= ~0x07;    
        LPC_IOCON->P1_1 |=  0x04;                                       /* SSP MOSI                     */
        break;
        
    default:
        break;
    }

    /* 
     * ��ʹ�ùܽŵ�CS����ʱ���˹�����ʵ��ʹ�ܿ���
     */
    #if !USE_CS
        SSP_SSELSetDirection( 2, uiLocation );
    #endif

    LPC_SSP2->CR0 = 0x07 << 0 |                                         /* ���ݳ���Ϊ8λ                */
                    0x00 << 4 |                                         /* ֡��ʽΪSPI                  */
                    0x00 << 6 |                                         /* CPOLΪ0                      */
                    0x00 << 7 |                                         /* CPHAΪ0                      */
                    0x07 << 8;                                          /* ����ʱ������Ϊ7              */

    LPC_SSP2->CPSR = 0x2;                                               /* ʱ��Ԥ��ƵֵΪ2              */
    
    for (i = 0; i < FIFOSIZE; i++) {
        uiDummy = LPC_SSP2->DR;                                         /* ��0����FIFO                  */
    }

    NVIC_EnableIRQ(SSP2_IRQn);                                          /* ʹ��SSP2�ж�                 */

    #if LOOPBACK_MODE                                                   /* ��·ģʽ                     */
        LPC_SSP2->CR1 = SSPCR1_LBM | SSPCR1_SSE;
    #elif SSP_SLAVE2                                                    /* �ӻ�ģʽ                     */
        if ( LPC_SSP2->CR1 & SSPCR1_SSE ) {
            LPC_SSP2->CR1 &= ~SSPCR1_SSE;                               /* ����SSP1������               */
        }
        LPC_SSP2->CR1 = SSPCR1_MS;                                      /* ѡ��SSP2���ߴӻ�             */
        LPC_SSP2->CR1 |= SSPCR1_SSE;                                    /* ʹ��SSP2������               */
    #else                                                               /* ����ģʽ                     */
        LPC_SSP2->CR1 = SSPCR1_SSE;
    #endif
    LPC_SSP2->IMSC = SSPIMSC_RORIM | SSPIMSC_RTIM;                      /* ʹ������жϺͽ��ճ�ʱ�ж�   */
    return;
}


/***************************************************************************
**�������ƣ�Fm25L04AndFlashSpiSend
**�������ܣ�Fm25l04��Flash��Spi���ݷ��ͺ�����
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void Fm25L04AndFlashSpiSend(uint8_t *puiBuf, uint16_t uiLength)
{
    uint32_t i = 0;
    uint8_t uiDummy = uiDummy = 0;

    for (i = 0; i < uiLength; i++) 
    {
        while ( (LPC_SSP0->SR &                                     /* SSP0��TX FIFOδ����SSP0����  */
                (SSPSR_TNF | SSPSR_BSY))!=
                 SSPSR_TNF );

        LPC_SSP0->DR = *puiBuf;                                     /* ��������                     */
        puiBuf++;                                                   /* ָ����һ����������           */

  
        while ( (LPC_SSP0->SR &                                     /* SSP0��RX FIFOδ����SSP0����  */
                (SSPSR_BSY | SSPSR_RNE)) !=
                 SSPSR_RNE );

        uiDummy = LPC_SSP0->DR;                                     /* ���TX FIFO                  */
    }
    return;
}


/***************************************************************************
**�������ƣ�Fm25L04AndFlashSpiReceive
**�������ܣ�Fm25l04��Flash��Spi���ݽ��ܺ�����
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void Fm25L04AndFlashSpiReceive(uint8_t *puiBuf, uint16_t uiLength )
{
    uint16_t i;
    for (i = 0; i < uiLength; i++)  // ����ģʽ��
    {                                               
        LPC_SSP0->DR = 0xFF;
        while ( (LPC_SSP0->SR & 
                (SSPSR_BSY | SSPSR_RNE)) !=
                 SSPSR_RNE );
   
        *puiBuf++ = LPC_SSP0->DR; 
    }
    return;
}
/*********************************************************************************************************
** ��������: SPSend
** ����������SSP�������ݺ�����
** ���������uiPortNum    SSP�˿ں�
**           puiBuf       ��������ָ��
**           uiLength     �������ݳ���
** ����ֵ  ����
*********************************************************************************************************/
void SSPSend( uint32_t uiPortNum, uint8_t *puiBuf, uint32_t uiLength )
{
    uint32_t i = 0;
    uint8_t uiDummy = uiDummy = 0;

    for (i = 0; i < uiLength; i++) {
        if ( uiPortNum == 0 ) {
            while ( (LPC_SSP0->SR &                                     /* SSP0��TX FIFOδ����SSP0����  */
                    (SSPSR_TNF | SSPSR_BSY))!=
                     SSPSR_TNF );

            LPC_SSP0->DR = *puiBuf;                                     /* ��������                     */
            puiBuf++;                                                   /* ָ����һ����������           */

            #if !LOOPBACK_MODE                                          /* ������ӻ�ģʽ               */
                while ( (LPC_SSP0->SR &                                 /* SSP0��RX FIFOδ����SSP0����  */
                        (SSPSR_BSY | SSPSR_RNE)) !=
                         SSPSR_RNE );

                uiDummy = LPC_SSP0->DR;                                 /* ���TX FIFO                  */
            #else                                                       /* ��·ģʽ                     */
                while ( LPC_SSP0->SR & SSPSR_BSY );
            #endif
        }
        else if ( uiPortNum == 1 ) {
            while ( (LPC_SSP1->SR &                                     /* SSP0��TX FIFOδ����SSP0����  */
                    (SSPSR_TNF | SSPSR_BSY)) != 
                	SSPSR_TNF );

            LPC_SSP1->DR = *puiBuf;                                     /* ��������                     */
            puiBuf++;                                                   /* ָ����һ����������           */

            #if !LOOPBACK_MODE                                          /* ������ӻ�ģʽ               */
                while ( (LPC_SSP1->SR &                                 /* SSP0��RX FIFOδ����SSP0����  */
                        (SSPSR_BSY | SSPSR_RNE)) != 
                         SSPSR_RNE );

                uiDummy = LPC_SSP1->DR;                                 /* ���TX FIFO                  */
            #else                                                       /* ��·ģʽ                     */
                while ( LPC_SSP1->SR & SSPSR_BSY );
            #endif
        }
        else if ( uiPortNum == 2 ) {
            while ( (LPC_SSP2->SR &                                     /* SSP0��TX FIFOδ����SSP0����  */
                    (SSPSR_TNF|SSPSR_BSY)) !=
                     SSPSR_TNF );
            LPC_SSP2->DR = *puiBuf;                                       /* ��������                     */
            puiBuf++;                                                     /* ָ����һ����������           */
            #if !LOOPBACK_MODE                                          /* ������ӻ�ģʽ               */
                while ( (LPC_SSP2->SR &                                 /* SSP0��RX FIFOδ����SSP0����  */
                        (SSPSR_BSY | SSPSR_RNE)) !=
                         SSPSR_RNE );

                uiDummy = LPC_SSP2->DR;                                 /* ���TX FIFO                  */
            #else
                while ( LPC_SSP2->SR & SSPSR_BSY );
            #endif
        }
    }
    return;
}

/*********************************************************************************************************
** �������ƣ�SSPRec
** ����������SSP�������ݺ�����
** ���������uiPortNum    SSP�˿ں�
**           puiBuf       ��������ָ��
**           uiLength     �������ݳ���
** ����ֵ  ����
*********************************************************************************************************/
void SSPRec( uint32_t uiPortNum, uint8_t *puiBuf, uint32_t uiLength )
{
    uint32_t i;

    for (i = 0; i < uiLength; i++) {
        if ( uiPortNum == 0 ) {
            #if !LOOPBACK_MODE
            #if SSP_SLAVE0                                              /* �ӻ�ģʽ                     */
                while ( !(LPC_SSP0->SR & SSPSR_RNE) );
            #else                                                       /* ����ģʽ                     */
                LPC_SSP0->DR = 0xFF;
                while ( (LPC_SSP0->SR & 
                        (SSPSR_BSY | SSPSR_RNE)) !=
                         SSPSR_RNE );
            #endif
            #else                                                       /* ��·ģʽ                     */
                while ( !(LPC_SSP0->SR & SSPSR_RNE) );
            #endif
            *puiBuf++ = LPC_SSP0->DR;
        }
        else if ( uiPortNum == 1 ) {
            #if !LOOPBACK_MODE
            #if SSP_SLAVE1                                              /* �ӻ�ģʽ                     */
                while ( !(LPC_SSP1->SR & SSPSR_RNE) );
            #else                                                       /* ����ģʽ                     */
                LPC_SSP1->DR = 0xCC;
                while ( (LPC_SSP1->SR & 
                        (SSPSR_BSY | SSPSR_RNE)) !=
                         SSPSR_RNE );
            #endif
            #else                                                       /* ��·ģʽ                     */
                while ( !(LPC_SSP1->SR & SSPSR_RNE) );
            #endif
            *puiBuf++ = LPC_SSP1->DR;
        }
        else if ( uiPortNum == 2 ) {
            #if !LOOPBACK_MODE
            #if SSP_SLAVE2                                              /* �ӻ�ģʽ                     */
                while (!(LPC_SSP2->SR & SSPSR_RNE));
            #else                                                       /* ����ģʽ                     */
                LPC_SSP2->DR = 0xFF;
                while ( (LPC_SSP2->SR & 
                        (SSPSR_BSY | SSPSR_RNE)) != 
                         SSPSR_RNE );
            #endif
            #else                                                       /* ��·ģʽ                     */
                while (!(LPC_SSP2->SR & SSPSR_RNE));
            #endif
            *puiBuf++ = LPC_SSP2->DR;
        }
    }
    return;
}

/*********************************************************************************************************
**                            End Of File
*********************************************************************************************************/

