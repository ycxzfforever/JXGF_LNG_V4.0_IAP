#include "kernel.h"
/* �������ں�I2C�жϴ�����Ϣ��  */
/* ȫ�ֱ���                     */
volatile uint8_t     I2C0_SlaveAddr;                  /* I2C������ַ                */
volatile uint32_t    I2C0_SlaveSubAddr;               /* I2C�����洢�ֽڵ�ַ            */
volatile uint8_t     I2C0_SlaveSubAddrNum;            /* I2C�ӵ�ַ�ֽ���              */
volatile uint8_t     *I2C0_DataBuf;                   /* ���ݻ�����ָ��               */
volatile uint32_t    I2C0_DataNum;                    /* Ҫ��ȡ/д������ݸ���        */
volatile uint8_t     I2C0_EndFlag;                    /* I2C���߽�����־����������������1  */
volatile uint8_t     I2C0_OP_Type;                    /* �ӵ�ַ���ơ�
																							           0--�ӵ�ַ�Ѿ�������߲���Ҫ�ӵ�ַ
																												 1--��ȡ����
																												 2--д����
																											*/

/********************************************************
**��������:I2C0WaitEndFlag
**��������:�ȴ�������־
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t  I2C0WaitEndFlag(uint32_t  dly)
{
    uint32_t  i;
    if(I2C0_EndFlag==1) return (1);
    for(; dly>0; dly--)
    for(i=0;i<5000; i++)
    {
        if(I2C0_EndFlag==1) return (1);
    }
    return (0);
}

/********************************************************
**��������:I2C0WriteByte
**��������:�����ӵ�ַ��������1�ֽ����ݡ�
**�������:sla     ������ַ
**         dat     Ҫ���͵�����
**���ֵ  :����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/

uint8_t  I2C0WriteByte(uint8_t slaveaddr, uint8_t data)
{                                                                       /* ��������                     */
    I2C0_SlaveAddr   = slaveaddr;                                       /* д������������ַ             */
    I2C0_DataBuf     = &data;                                           /* �����͵�����                 */
    I2C0_DataNum     = 1;                                               /* ����1�ֽ�����                */
    I2C0_OP_Type = 0;                                                   /* ���ӵ�ַ                     */
    I2C0_EndFlag     = 0;
    LPC_I2C0->CONCLR = 0x2C;
    LPC_I2C0->CONSET = 0x60;                                            /* ����Ϊ����������������       */
    return( I2C0WaitEndFlag(20));
}

/********************************************************
**��������:I2C0ReadByte
**��������:�����ӵ�ַ������ȡ1�ֽ����ݡ�
**�������:sla     ������ַ
**         dat     ���ܵ�����
**���ֵ  :����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t  I2C0ReadByte(uint8_t slaveaddr, uint8_t *data)
{                                                                       /* ��������                     */
    I2C0_SlaveAddr     = slaveaddr+1;                                   /* ��������������ַ             */
    I2C0_DataBuf     = data;
    I2C0_DataNum     = 1;
    I2C0_OP_Type = 0;                                                    /* ���ӵ�ַ                     */
    I2C0_EndFlag     = 0;
    LPC_I2C0->CONCLR = 0x2C;
    LPC_I2C0->CONSET = 0x60;                                            /* ����Ϊ����������������       */
    return( I2C0WaitEndFlag(20));
}

/********************************************************
**��������:I2C0ReadNByte
**��������:�����ӵ�ַ���������ַ��ʼ��ȡN�ֽ�����
**�������:slaveaddr      ������ַ
**         subaddr_type   �ӵ�ַ�ṹ    1�����ֽڵ�ַ    2��8+X�ṹ    2��˫�ֽڵ�ַ
**         subaddr        �����ӵ�ַ
**         s           ���ݽ��ջ�����ָ��
**        num         ��ȡ�ĸ���
**���ֵ  :����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t I2C0ReadNByte(uint8_t slaveaddr, uint32_t subaddr_type, uint32_t subaddr, uint8_t *s, uint32_t num)
{
    if(num>0)                                                        /* �ж�num�����ĺϷ���          */
    {                                                                   /* ��������                     */
        if(subaddr_type == 1)
        {                                                               /* �ӵ�ַΪ���ֽ�               */
            I2C0_SlaveAddr         = slaveaddr + 1;                                  /* �������Ĵӵ�ַ��R=1          */
            I2C0_SlaveSubAddr      = subaddr;                                     /* �����ӵ�ַ                   */
            I2C0_SlaveSubAddrNum   = 1;                                        /* �����ӵ�ַΪ1�ֽ�            */
        }
        if(subaddr_type == 2)
        {                                                               /* �ӵ�ַΪ2�ֽ�                */
            I2C0_SlaveAddr         = slaveaddr + 1;                                  /* �������Ĵӵ�ַ��R=1          */
            I2C0_SlaveSubAddr      = subaddr;                                     /* �����ӵ�ַ                   */
            I2C0_SlaveSubAddrNum   = 2;                                        /* �����ӵ�ַΪ2�ֽ�            */
        }
        if(subaddr_type == 3)
        {                                                               /* �ӵ�ַ�ṹΪ8+X              */
            I2C0_SlaveAddr         = slaveaddr + ((subaddr >> 7 )& 0x0e) + 1;           /* �������Ĵӵ�ַ��R=1          */
            I2C0_SlaveSubAddr      = subaddr & 0x0ff;                             /* �����ӵ�ַ                   */
            I2C0_SlaveSubAddrNum   = 1;                                        /* �����ӵ�ַΪ8+x              */
        }
        I2C0_DataBuf     = s;                                                /* ���ݽ��ջ�����ָ��           */
        I2C0_DataNum     = num;                                              /* Ҫ��ȡ�ĸ���                 */
        I2C0_OP_Type = 1;                                                /* ���ӵ�ַ��                   */
        I2C0_EndFlag     = 0;
                                                                        /* ���STA,SI,AA��־λ          */
        LPC_I2C0->CONCLR = (1 << 2)|                                    /* AA                           */
                           (1 << 3)|                                    /* SI                           */
                           (1 << 5);                                    /* STA                          */
                                                                        /* ��λSTA,����I2C����          */
        LPC_I2C0->CONSET = (1 << 5)|                                    /* STA                          */
                           (1 << 6);                                    /* I2CEN                        */
                                                                        /* �ȴ�I2C�������              */
        return(I2C0WaitEndFlag(20));
    }
    return (FALSE);
}
/********************************************************
**��������:I2C0WriteNByte
**��������:�����ӵ�ַ����д��N�ֽ�����
**�������:slaveaddr      ������ַ
**         subaddr_type   �ӵ�ַ�ṹ    1�����ֽڵ�ַ    2��8+X�ṹ    2��˫�ֽڵ�ַ
**         subaddr        �����ӵ�ַ
**         s              ��Ҫд������ݵ�ָ��
**        num             ��Ҫд������ݵĸ���
**���ֵ  :����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
uint8_t I2C0WriteNByte(uint8_t slaveaddr, uint8_t subaddr_type, uint32_t subaddr, uint8_t *s, uint32_t num)
{
    if (num > 0)                                                        /* �����ȡ�ĸ���Ϊ0���򷵻ش���*/
    {                                                                   /* ���ò���                     */
        if (subaddr_type == 1){                                            /* �ӵ�ַΪ���ֽ�               */
            I2C0_SlaveAddr         = slaveaddr;                                      /* �������Ĵӵ�ַ               */
            I2C0_SlaveSubAddr      = subaddr;                                     /* �����ӵ�ַ                   */
            I2C0_SlaveSubAddrNum   = 1;                                        /* �����ӵ�ַΪ1�ֽ�            */
        }
        if (subaddr_type == 2){                                            /* �ӵ�ַΪ2�ֽ�                */
            I2C0_SlaveAddr         = slaveaddr;                                      /* �������Ĵӵ�ַ               */
            I2C0_SlaveSubAddr      = subaddr;                                     /* �����ӵ�ַ                   */
            I2C0_SlaveSubAddrNum   = 2;                                        /* �����ӵ�ַΪ2�ֽ�            */
        }
        if (subaddr_type == 3){                                            /* �ӵ�ַ�ṹΪ8+X              */
            I2C0_SlaveAddr         = slaveaddr + ((subaddr >> 7 )& 0x0e);               /* �������Ĵӵ�ַ               */
            I2C0_SlaveSubAddr      = subaddr & 0x0ff;                             /* �����ӵ�ַ                   */
            I2C0_SlaveSubAddrNum   = 1;                                        /* �����ӵ�ַΪ8+X              */
        }
        I2C0_DataBuf     = s;                                                /* ����                         */
        I2C0_DataNum     = num;                                              /* ���ݸ���                     */
        I2C0_OP_Type = 2;                                                /* ���ӵ�ַ��д����             */
        I2C0_EndFlag     = 0;                                                                        
        LPC_I2C0->CONCLR = (1 << 2)|                                    /* ���STA,SI,AA��־λ          */
                           (1 << 3)|
                           (1 << 5);
        LPC_I2C0->CONSET = (1 << 5)|                                    /* ��λSTA,����I2C����          */
                           (1 << 6);                                    /* I2CEN                        */
        return( I2C0WaitEndFlag(20));                                      /* �ȴ�I2C�������              */
    }
    return (FALSE);
}

/********************************************************
**��������:I2C0_IRQHandler
**��������:Ӳ��I2C�жϷ������
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void  I2C0_IRQHandler(void)
{       /*
	 *��ȡI2C״̬�Ĵ���I2DAT,����ȫ�ֱ��������ý��в��������������־,����ж��߼�,�жϷ���
	 */
	switch (LPC_I2C0->STAT & 0xF8)
	{   /* 
		 *  ����״̬�������Ӧ�Ĵ���     
		 */
			case 0x08:                                                      /* �ѷ�����ʼ����,�����ͺ�����  */
																																			/* �ն���,װ��SLA+W����SLA+R    */
			if(I2C0_OP_Type == 1)/* SLA+R */                                 /* ָ���ӵ�ַ��                 */
			{   LPC_I2C0->DAT = I2C0_SlaveAddr & 0xFE;                             /* ��д���ַ                   */
			}
			else                                                            /* SLA+W                        */
			{   LPC_I2C0->DAT = I2C0_SlaveAddr;                                    /* ����ֱ�ӷ��ʹӻ���ַ         */
			}
																																			/* ����SIλ                     */
			LPC_I2C0->CONCLR = (1 << 3)|                                    /* SI                           */
									 (1 << 5);                                          /* STA                          */
			break;
			case 0x10:                                                      /*�ѷ����ظ���ʼ����            */
																																			/* �����ͺ������ն���           */
																																			/* װ��SLA+W����SLA+R           */
			LPC_I2C0->DAT = I2C0_SlaveAddr;                                        /* �������ߺ��ط��ӵ�ַ       */
			LPC_I2C0->CONCLR = 0x28;                                        /* ����SI,STA                   */
			break;
			case 0x18:
			case 0x28:                                                      /* �ѷ���I2DAT�е�����,�ѽ���ACK*/
			if (I2C0_OP_Type == 0){
					if (I2C0_DataNum > 0){
							LPC_I2C0->DAT = *I2C0_DataBuf++;
							LPC_I2C0->CONCLR = 0x28;                                /* ����SI,STA                   */
							I2C0_DataNum--;
					}
					else                                                        /* û�����ݷ�����               */
					{                                                           /* ֹͣ����                     */
							LPC_I2C0->CONSET = (1 << 4);                            /* STO                          */
							LPC_I2C0->CONCLR = 0x28;                                /* ����SI,STA                   */
							I2C0_EndFlag = 1;                                            /* �����Ѿ�ֹͣ                 */
					}
			}
			if(I2C0_OP_Type == 1) {                                          /* ����ָ����ַ��,��������������*/
					if (I2C0_SlaveSubAddrNum == 2) {
							LPC_I2C0->DAT = ((I2C0_SlaveSubAddr >> 8) & 0xff);
							LPC_I2C0->CONCLR = 0x28;                                /* ����SI,STA                   */
							I2C0_SlaveSubAddrNum--;
							break;
					}
					if(I2C0_SlaveSubAddrNum == 1) {
							LPC_I2C0->DAT = (I2C0_SlaveSubAddr & 0xff);
							LPC_I2C0->CONCLR = 0x28;                                /* ����SI,STA                   */
							I2C0_SlaveSubAddrNum--;
							break;
					}
					if (I2C0_SlaveSubAddrNum == 0) {
							LPC_I2C0->CONCLR = 0x08;
							LPC_I2C0->CONSET = 0x20;
							I2C0_OP_Type = 0;                                        /* �ӵ�ַ������                 */
							break;
					}
			}
			if (I2C0_OP_Type == 2) {                                         /* ָ���ӵ�ַд,�ӵ�ַ��δָ��, */
																																			/* �����ӵ�ַ                 */
					if (I2C0_SlaveSubAddrNum > 0) {
							if (I2C0_SlaveSubAddrNum == 2) {
									LPC_I2C0->DAT = ((I2C0_SlaveSubAddr >> 8) & 0xff);
									LPC_I2C0->CONCLR = 0x28;
									I2C0_SlaveSubAddrNum--;
									break;
							}
							if (I2C0_SlaveSubAddrNum == 1) {
									LPC_I2C0->DAT    = (I2C0_SlaveSubAddr & 0xff);
									LPC_I2C0->CONCLR = 0x28;
									I2C0_SlaveSubAddrNum--;
									I2C0_OP_Type  = 0;
									break;
							}
					}
			}
			break;
			case 0x40:                                                      /* �ѷ���SLA+R,�ѽ���ACK        */
			if (I2C0_DataNum <= 1) {                                             /* ��������һ���ֽ�           */
					 LPC_I2C0->CONCLR = 1 << 2;                                 /* �´η��ͷ�Ӧ���ź�           */
			}else {
					 LPC_I2C0->CONSET = 1 << 2;                                 /* �´η���Ӧ���ź�             */
			}
			LPC_I2C0->CONCLR = 0x28;                                        /* ����SI,STA                   */
			break;
			case 0x20:                                                      /* �ѷ���SLA+W,�ѽ��շ�Ӧ��     */
			case 0x30:                                                      /* �ѷ���I2DAT�е����ݣ��ѽ���  */
																																			/* ��Ӧ��                       */
			case 0x38:                                                      /* ��SLA+R/W�������ֽ��ж�ʧ��  */
																																			/* ��                           */
			case 0x48:                                                      /* �ѷ���SLA+R,�ѽ��շ�Ӧ��     */
			LPC_I2C0->CONCLR = 0x28;
			I2C0_EndFlag = 0xFF;
			break;
			case 0x50:                                                      /* �ѽ��������ֽڣ��ѷ���ACK    */
			*I2C0_DataBuf++ = LPC_I2C0->DAT;
			I2C0_DataNum--;
			if (I2C0_DataNum == 1)                                               /* �������һ���ֽ�             */
			{   LPC_I2C0->CONCLR = 0x2C;                                    /* STA,SI,AA = 0                */
			} else
			{   
					LPC_I2C0->CONSET = 0x04;                                    /* AA=1                         */
					LPC_I2C0->CONCLR = 0x28;
			}
			break;
			case 0x58:                                                      /* �ѽ��������ֽڣ��ѷ��ط�Ӧ�� */
			*I2C0_DataBuf++ = LPC_I2C0->DAT;                                     /* ��ȡ���һ�ֽ�����           */
			LPC_I2C0->CONSET = 0x10;                                        /* ��������                     */
			LPC_I2C0->CONCLR = 0x28;
			I2C0_EndFlag = 1;
			break;
			default:
			break;
	}
}

/********************************************************
**��������:I2C0Init
**��������:I2C0��ʼ��
**�������:ulFi2c0    I2C0����Ƶ��(���400K)
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void I2C0Init(uint32_t ulFi2c0)
{
    LPC_SC->PCONP |=(0x01<<7);               /* ��I2C0ģ�鹦�ʿ���λ       */
    LPC_SC->PCONP |=(0x01<<15);              /* ��GPIOģ�鹦�ʿ���λ       */
    LPC_IOCON->P0_27 &=~0x07 ;               /* P0.27����ΪSDA0����          */
	  LPC_IOCON->P0_27 |=0x01;
    LPC_IOCON->P0_28 &=~0x07 ;               /* P0.28����ΪSCL0����          */
	  LPC_IOCON->P0_28 |=0x01;
    if (ulFi2c0 > 400000) {
        ulFi2c0 = 400000;
    }
    LPC_I2C0->SCLH = (PeripheralClock/ulFi2c0)/2;                   /* ����I2Cʱ��Ϊfi2c            */
    LPC_I2C0->SCLL = (PeripheralClock/ulFi2c0)/2;
    NVIC_EnableIRQ(I2C0_IRQn);                                      /* ʹ��I2C�ж�                  */
    NVIC_SetPriority(I2C0_IRQn,1);                                  /* ����I2C1�ж����ȼ���ʹ���ж� */
    LPC_I2C0->CONCLR = 0x2C;
    LPC_I2C0->CONSET = 0x40;                                        /* ʹ����I2C0                  */
}


