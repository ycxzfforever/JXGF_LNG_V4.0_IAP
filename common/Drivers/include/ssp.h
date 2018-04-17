#ifndef __SSP_H__
#define __SSP_H__

#include "type.h"

/*
** 1 is loopback, 0 is normal operation. 
*/
#define LOOPBACK_MODE    0
/*
** 1 is SLAVE mode, 0 is master mode
*/
#define SSP_SLAVE0        0
#define SSP_SLAVE1        0
#define SSP_SLAVE2        0
/* 1 is TX or RX only depending on SSP_SLAVE flag, 0 is either loopback mode or communicate
** with a serial EEPROM
*/
#define TX_RX_ONLY       0

/* 
** if USE_CS is zero, set SSEL as GPIO that you have total control of the sequence 
*/
#define USE_CS            1

/*
** SPI read and write buffer size 
*/
#define SSP_BUFSIZE        16
#define FIFOSIZE        8

#define DELAY_COUNT        10
#define MAX_TIMEOUT        0xFF

/*
** Port0.2 is the SSP select pin 
*/
#define SSP0_SEL        (1 << 2)
    
/*
** SSP Status register 
*/
#define SSPSR_TFE        (1 << 0)
#define SSPSR_TNF        (1 << 1) 
#define SSPSR_RNE        (1 << 2)
#define SSPSR_RFF        (1 << 3) 
#define SSPSR_BSY        (1 << 4)

/*
** SSP CR0 register 
*/
#define SSPCR0_DSS        (1 << 0)
#define SSPCR0_FRF        (1 << 4)
#define SSPCR0_SPO        (1 << 6)
#define SSPCR0_SPH        (1 << 7)
#define SSPCR0_SCR        (1 << 8)

/*
** SSP CR1 register 
*/
#define SSPCR1_LBM        (1 << 0)
#define SSPCR1_SSE        (1 << 1)
#define SSPCR1_MS         (1 << 2)
#define SSPCR1_SOD        (1 << 3)

/*
** SSP Interrupt Mask Set/Clear register 
*/
#define SSPIMSC_RORIM   (1 << 0)
#define SSPIMSC_RTIM    (1 << 1)
#define SSPIMSC_RXIM    (1 << 2)
#define SSPIMSC_TXIM    (1 << 3)

/*
** SSP0 Interrupt Status register 
*/
#define SSPRIS_RORRIS    (1 << 0)
#define SSPRIS_RTRIS    (1 << 1)
#define SSPRIS_RXRIS    (1 << 2)
#define SSPRIS_TXRIS    (1 << 3)

/*
** SSP0 Masked Interrupt register 
*/
#define SSPMIS_RORMIS    (1 << 0)
#define SSPMIS_RTMIS    (1 << 1)
#define SSPMIS_RXMIS    (1 << 2)
#define SSPMIS_TXMIS    (1 << 3)

/*
** SSP0 Interrupt clear register 
*/
#define SSPICR_RORIC    (1 << 0)
#define SSPICR_RTIC        (1 << 1)

/*
** ATMEL SEEPROM command set 
*/
#define WREN        0x06
#define WRDI        0x04
#define RDSR        0x05
#define WRSR        0x01
#define READ        0x03
#define WRITE       0x02

/*
** RDSR status bit definition 
*/
#define RDSR_RDY    0x01
#define RDSR_WEN    0x02

/* If RX_INTERRUPT is enabled, the SSP RX will be handled in the ISR
** SSPReceive() will not be needed. 
*/

/*********************************************************************************************************
** �������ƣ�SSP1_IRQHandler
** ����������SSP1�жϺ���
** �����������
** ����ֵ  ����
*********************************************************************************************************/
extern void SSP0_IRQHandler (void);

/*********************************************************************************************************
** �������ƣ�SSP1_IRQHandler
** ����������SSP1�жϺ���
** �����������
** ����ֵ  ����
*********************************************************************************************************/
extern void SSP1_IRQHandler (void);

/*********************************************************************************************************
** �������ƣ�SSP2_IRQHandler
** ����������SSP2�жϺ���
** �����������
** ����ֵ  ����
*********************************************************************************************************/
extern void SSP2_IRQHandler (void);

/*********************************************************************************************************
** �������ƣ�SSP_SSELSetDirection
** ����������ѡ��SSP��CS�ܽţ�ʵ�ֶ����ƽ�ɿ�
** ���������uiPortNum      SSP�˿ں�
**           uiLocation     CS�ܽ�λ��
** ����ֵ  ����
*********************************************************************************************************/
extern void SSP_SSELSetDirection( uint32_t portnum, uint32_t location );

/*********************************************************************************************************
** �������ƣ�SSP_SSELToggle
** ��������������SSP��CS�ܽŵ�ƽ
** ���������uiPortNum    SSP�˿ں�
**           uiLocation   SSEL�ܽ�λ��
**           toggle       SSEL�ܽŵ�ƽ���ƣ�0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
** ����ֵ  ����
*********************************************************************************************************/
extern void SSP_SSELToggle( uint32_t portnum, uint32_t location, uint32_t toggle );

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
extern void Fm25L04AndFlashSpiInit(void);

/*********************************************************************************************************
** �������ƣ�SSP1Init
** ����������SSP1��ʼ��
** �����������
** ����ֵ  ����
*********************************************************************************************************/
extern void SSP1Init( uint32_t location );

/*********************************************************************************************************
** �������ƣ�SSP2Init
** ����������SSP2��ʼ��
** �����������
** ����ֵ  ����
*********************************************************************************************************/
extern void SSP2Init( uint32_t location );

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
extern void Fm25L04AndFlashSpiSend(uint8_t *puiBuf, uint16_t uiLength);
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
extern void Fm25L04AndFlashSpiReceive(uint8_t *puiBuf, uint16_t uiLength );

extern void SSPSend( uint32_t uiPortNum, uint8_t *puiBuf, uint32_t uiLength );
extern void SSPRec( uint32_t uiPortNum, uint8_t *puiBuf, uint32_t uiLength );

#endif                                                                   /* __SSP_H__                   */
/*********************************************************************************************************
**                            End Of File
*********************************************************************************************************/

