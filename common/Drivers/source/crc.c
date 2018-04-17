#include "kernel.h"

/********************************************************
**��������:CRCInit
**��������:CRC��ʼ��
**�������:CRC_mode:CRC����ز���
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-21

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void CRCInit( CRC_CFG_Type* CRC_mode )
{
    LPC_CRC->MODE = 0;
    LPC_CRC->SEED = 0;

    LPC_CRC->MODE = ((CRC_mode->poly)|(CRC_mode->bit_wr)|( CRC_mode->cmpl_wr)|\
                    (CRC_mode->bit_sum)|(CRC_mode->cmpl_sum));
    LPC_CRC->SEED = CRC_mode->seed;
    return;
}

/********************************************************
**��������:CRCWriteData
**��������:��CRC�ļĴ���д��Ҫ���������
**�������:data:����CRC������
					 length:����CRC�����ݳ���
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-21

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void CRCWriteData(uint8_t *data, uint8_t length)
{
  uint16_t * data_word = (uint16_t *)data;
  uint32_t * data_dword = (uint32_t *)data;

  switch ( length )
  {
     case 4:                                                            /* ���ݿ��Ϊ32λ               */
       LPC_CRC->WR_DATA_DWORD = *data_dword;
     break;
     case 2:                                                            /* ���ݿ��Ϊ16λ               */
       LPC_CRC->WR_DATA_WORD = *data_word;
     break;
     case 1:                                                            /* ���ݿ��Ϊ8λ                */
       LPC_CRC->WR_DATA_BYTE = *data;
     break;
     default:
     break;
  }
  return;
}

/********************************************************
**��������:CRCGetResult
**��������:��ȡCRC������
**�������:��
**���ֵ  :CRC������

**������  :�ƺ���
**��������:2016-04-21

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint32_t CRCGetResult(void)
{
  uint32_t regVal;
  regVal = LPC_CRC->SUM;                                                /* ��ȡ�����У�����ֵ         */
  return regVal;
}

/********************************************************
**��������:CRC16Calculate
**��������:CRC����
**�������:data������CRC������
           len ������CRC���ݵĳ���
**���ֵ  :CRC������

**������  :�ƺ���
**��������:2016-04-21

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint16_t CRC16Calculate(uint8_t *data, uint32_t len)
{
    CRC_CFG_Type crc_mode;
    uint16_t usCRCResult;                                               /* У��ͽ������               */
    uint16_t i;

    crc_mode.poly     = CRC_POLY_16;                                    /* ѡ��CRC-16����ʽ             */
    crc_mode.bit_wr   = CRC_BIT_RVS_WR_EN;                              /* ��������λ��ת             */
    crc_mode.cmpl_wr  = CRC_CMPL_WR_NO;
    crc_mode.bit_sum  = CRC_BIT_RVS_SUM_EN;                             /* У���λ��ת               */
    crc_mode.cmpl_sum = CRC_CMPL_SUM_NO;
    crc_mode.seed     = 0xffff;                                         /* ���ó�ֵΪ 0                 */
    CRCInit(&crc_mode);                                                /* ��ʼ��CRC����                */

    for(i = 0; i < len; i++) {
        CRCWriteData(data, 1);                                       /* д���У������               */
        data++;
    }

    usCRCResult=CRCGetResult();                                         /* ��ȡУ���                   */
    return(usCRCResult);                                                /* ����У���                   */
}

