#include "kernel.h"
/* �������ں�I2C�жϴ�����Ϣ��  */
/* ȫ�ֱ���                     */

volatile uint8_t     I2C_sla;                                           /* I2C�����ӵ�ַ                */
volatile uint32_t    I2C_suba;                                          /* I2C�����ڲ��ӵ�ַ            */
volatile uint8_t     I2C_suba_num;                                      /* I2C�ӵ�ַ�ֽ���              */
volatile uint8_t     *I2C_buf;                                          /* ���ݻ�����ָ��               */
volatile uint32_t    I2C_num;                                           /* Ҫ��ȡ/д������ݸ���        */
volatile uint8_t     I2C_end;                                           /* I2C���߽�����־������������  */
/* ��1                          */
volatile uint8_t     I2C_suba_en;                                       /*  �ӵ�ַ���ơ�
                                                                         0--�ӵ�ַ�Ѿ�������߲���Ҫ�ӵ�ַ
                                                                         1--��ȡ����
                                                                         2--д����
                                                                         */

/*********************************************************************************************************
** �������ƣ�I2C1_Test()
** ����������I2C1�Ĳ��Գ���:������AT24C128д�����ݣ�Ȼ��������Ƿ���ȷ
** �������:
** ����ֵ  ����
*********************************************************************************************************/
void I2C1_Test(void)
{
    uint16_t i;
    uint8_t data_buf[10]= {0},data_bufaa[10]= {0};

    for(i=0; i <= 10; i++)
    {
        data_buf[i] = (i);
    }

    At24C128WriteNByte(0x00, data_buf, 10);                                                          //������
    MsDelay(100);
    I2C1_ReadNByte(AT24C128, TWO_BYTE_SUBA ,0x00, data_bufaa, 10);        //������

    for(i=0; i < 10; i++)
    {
        // �ж϶��ص������Ƿ���ȷ
        if(data_buf[i] != data_bufaa[i])
        {
            while(1)
            {
                Fyd12864DispPrintf(1,1,"���ݴ洢�������");
                Fyd12864DispPrintf(2,1,"%d",i);
                Fyd12864DispPrintf(2,3,"��ȷ��");
                Fyd12864DispPrintf(2,7,"%d",data_buf[i]);
                Fyd12864DispPrintf(3,7,"%d",data_bufaa[i]);
                Fyd12864DispPrintf(4,7,"%d",data_bufaa[i+1]);
            }
        }
    }


    while(1)
    {
        Fyd12864DispPrintf(1,1,"���ݴ洢��ȷ");
//        Fyd12864DispPrintf(2,3,"%d",data_buf[150]);
//        Fyd12864DispPrintf(3,3,"%d",data_bufaa[150]);
//        Fyd12864DispPrintf(4,3,"%d",data_bufaa[151]);
    }
}

/********************************************************
**��������:At24C128WriteNByte
**��������:��At24C128дnum������  һҳ64�ֽ�
**�������:addr---��ʼ��ַ
                     data---д������ָ��
           num----д�����ݸ���
**���ֵ  :����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��

**������  :����
**��������:2016-04-20
********************************************************/
uint8_t At24C128WriteNByte(uint32_t addr, uint8_t *data, uint16_t num)
{
    uint8_t pageoffset,pagenum;
    uint32_t startaddr;
    uint16_t count;
    startaddr=addr;
    count=0;
    pageoffset=(uint8_t)(addr&0x003f);//7f  128   3f  64
    while(count<num)
    {
        if((startaddr&0x003f)==0)
        {
            if((num-count)>64)
            {
                pagenum=64;
            }
            else
            {
                pagenum=num-count;
            }
        }
        else
        {
            pagenum=64-pageoffset;
            if(num<=pagenum)
            {
                pagenum=num;
            }
        }
        I2C1_WriteNByte(AT24C128,TWO_BYTE_SUBA,startaddr,data+count,pagenum);              //LPC1788 ��I2C1
        MsDelay(50);
        count+=pagenum;
        startaddr+=pagenum;
    }
    return 0x01;
}

/********************************************************
**��������:At24C128ReadNByte
**��������:��At24C128��num������  һҳ64�ֽ�
**�������:addr---��ʼ��ַ
                            data---��������ָ��
                            num---�������ݸ���
**���ֵ  :����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��

**������  :����
**��������:2016-04-20
********************************************************/
uint8_t At24C128ReadNByte(uint32_t addr, uint8_t *data, uint16_t num)
{
    if(I2C1_ReadNByte(AT24C128,TWO_BYTE_SUBA,addr,data,num))
        return 1;
    else
        return 0;
}


/*********************************************************************************************************
** �������ƣ�I2C1Init
** ����������I2C1��ʼ��
** ���������Fi2c1    I2C1����Ƶ��(���400K)��400 000
** ����ֵ  ����
*********************************************************************************************************/
void I2C1Init(uint32_t ulFi2c1)
{
    LPC_SC->PCONP |= (0x01 << 19);                                      /* ��I2C1ģ�鹦�ʿ���λ       */
    LPC_SC->PCONP |= (0x01 << 15);                                      /* ��GPIOģ�鹦�ʿ���λ       */
    LPC_IOCON->P0_19 = 0x03 | (0x00 << 3) | (0x01 << 10);               /* P0.19����ΪSDA1����          */
    LPC_IOCON->P0_20 = 0x03 | (0x00 << 3) | (0x01 << 10);               /* P0.20����ΪSCL1����          */
    if(ulFi2c1 > 400000)
    {
        ulFi2c1 = 400000;
    }
    LPC_I2C1->SCLH = (PeripheralClock / ulFi2c1) / 2;                   /* ����I2Cʱ��Ϊfi2c            */
    LPC_I2C1->SCLL = (PeripheralClock / ulFi2c1) / 2;
    NVIC_EnableIRQ(I2C1_IRQn);                                          /* ʹ��I2C�ж�                  */
    NVIC_SetPriority(I2C1_IRQn, 2);                                     /* ����I2C1�ж����ȼ���ʹ���ж� */
    LPC_I2C1->CONCLR = 0x2C;
    LPC_I2C1->CONSET = 0x40;                                            /* ʹ����I2C                    */
}

/*********************************************************************************************************
** Function name:           Wait_I2c_End
** Descriptions:            �����ʱ���ṩ��I2C���ߵȴ���ʱʹ��
** input parameters:        dly        ��ʱ������ֵԽ����ʱԽ��
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
uint8_t  Wait_I2c_End(uint32_t  dly)
{
    uint32_t  i;
    if(I2C_end==1) return (1);
    for(; dly>0; dly--)
        for(i=0; i<5000; i++)
        {
            if(I2C_end==1) return (1);
        }
    return (0);
}

/*********************************************************************************************************
** Function name:       ISendByte
** Descriptions:        �����ӵ�ַ��������1�ֽ����ݡ�
** input parameters:    sla     ������ַ
**                      dat     Ҫ���͵�����
** Output parameters:   ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** Returned value:      NONE
*********************************************************************************************************/
uint8_t  I2C1_ISendByte(uint8_t sla, uint8_t dat)
{
    /* ��������                     */
    I2C_sla     = sla;                                                  /* д������������ַ             */
    I2C_buf     = &dat;                                                 /* �����͵�����                 */
    I2C_num     = 1;                                                    /* ����1�ֽ�����                */
    I2C_suba_en = 0;                                                    /* ���ӵ�ַ                     */
    I2C_end     = 0;
    LPC_I2C1->CONCLR = 0x2C;
    LPC_I2C1->CONSET = 0x60;                                            /* ����Ϊ����������������       */
    return(Wait_I2c_End(20));
}
/*********************************************************************************************************
** Function name:       IRcvByte
** Descriptions:        �����ӵ�ַ������ȡ1�ֽ����ݡ�
** input parameters:    sla     ������ַ
**                      dat     Ҫ���͵�����
** Output parameters:   ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** Returned value:      NONE
*********************************************************************************************************/
uint8_t  I2C1_IRcvByte(uint8_t sla, uint8_t *dat)
{
    /* ��������                     */
    I2C_sla     = sla+1;                                                /* ��������������ַ             */
    I2C_buf     = dat;
    I2C_num     = 1;
    I2C_suba_en = 0;                                                    /* ���ӵ�ַ                     */
    I2C_end     = 0;
    LPC_I2C1->CONCLR = 0x2C;
    LPC_I2C1->CONSET = 0x60;                                            /* ����Ϊ����������������       */
    return(Wait_I2c_End(20));
}
/*********************************************************************************************************
** Function name:       I2C_ReadNByte
** Descriptions:        �����ӵ�ַ���������ַ��ʼ��ȡN�ֽ�����
** input parameters:    sla         ������ַ
**                      suba_type   �ӵ�ַ�ṹ    1�����ֽڵ�ַ    2��8+X�ṹ    2��˫�ֽڵ�ַ
**                      suba        �����ӵ�ַ
**                      s           ���ݽ��ջ�����ָ��
**                      num         ��ȡ�ĸ���
** Output parameters:   ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** Returned value:      NONE
*********************************************************************************************************/
uint8_t I2C1_ReadNByte(uint8_t sla, uint32_t suba_type, uint32_t suba, uint8_t *s, uint32_t num)
{
    if(num > 0)                                                         /* �ж�num�����ĺϷ���          */
    {
        /* ��������                     */
        if(suba_type == 1)
        {
            /* �ӵ�ַΪ���ֽ�               */
            I2C_sla         = sla + 1;                                  /* �������Ĵӵ�ַ��R=1          */
            I2C_suba        = suba;                                     /* �����ӵ�ַ                   */
            I2C_suba_num    = 1;                                        /* �����ӵ�ַΪ1�ֽ�            */
        }
        if(suba_type == 2)
        {
            /* �ӵ�ַΪ2�ֽ�                */
            I2C_sla         = sla + 1;                                  /* �������Ĵӵ�ַ��R=1          */
            I2C_suba        = suba;                                     /* �����ӵ�ַ                   */
            I2C_suba_num    = 2;                                        /* �����ӵ�ַΪ2�ֽ�            */
        }
        if(suba_type == 3)
        {
            /* �ӵ�ַ�ṹΪ8+X              */
            I2C_sla         = sla + ((suba >> 7)& 0x0e) + 1;            /* �������Ĵӵ�ַ��R=1          */
            I2C_suba        = suba & 0x0ff;                             /* �����ӵ�ַ                   */
            I2C_suba_num    = 1;                                        /* �����ӵ�ַΪ8+x              */
        }
        I2C_buf     = s;                                                /* ���ݽ��ջ�����ָ��           */
        I2C_num     = num;                                              /* Ҫ��ȡ�ĸ���                 */
        I2C_suba_en = 1;                                                /* ���ӵ�ַ��                   */
        I2C_end     = 0;
        /* ���STA,SI,AA��־λ          */
        LPC_I2C1->CONCLR = (1 << 2)|                                    /* AA                           */
                           (1 << 3)|                                    /* SI                           */
                           (1 << 5);                                    /* STA                          */
        /* ��λSTA,����I2C����          */
        LPC_I2C1->CONSET = (1 << 5)|                                    /* STA                          */
                           (1 << 6);                                    /* I2CEN                        */
        /* �ȴ�I2C�������              */
        return(Wait_I2c_End(20));
    }
    return (FALSE);
}
/*********************************************************************************************************
** Function name:       I2C_WriteNByte
** Descriptions:        �����ӵ�ַ����д��N�ֽ�����
** input parameters:    sla         ������ַ
**                      suba_type   �ӵ�ַ�ṹ    1�����ֽڵ�ַ    2��8+X�ṹ    2��˫�ֽڵ�ַ
**                      suba        �����ӵ�ַ
**                      s           ��Ҫд������ݵ�ָ��
**                      num         ��Ҫд������ݵĸ���
** Output parameters:   ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** Returned value:      NONE
*********************************************************************************************************/
uint8_t I2C1_WriteNByte(uint8_t sla, uint8_t suba_type, uint32_t suba, uint8_t *s, uint32_t num)
{
    if(num > 0)                                                         /* �����ȡ�ĸ���Ϊ0���򷵻ش���*/
    {
        /* ���ò���                     */
        if(suba_type == 1)                                              /* �ӵ�ַΪ���ֽ�               */
        {
            I2C_sla         = sla;                                      /* �������Ĵӵ�ַ               */
            I2C_suba        = suba;                                     /* �����ӵ�ַ                   */
            I2C_suba_num    = 1;                                        /* �����ӵ�ַΪ1�ֽ�            */
        }
        if(suba_type == 2)                                              /* �ӵ�ַΪ2�ֽ�                */
        {
            I2C_sla         = sla;                                      /* �������Ĵӵ�ַ               */
            I2C_suba        = suba;                                     /* �����ӵ�ַ                   */
            I2C_suba_num    = 2;                                        /* �����ӵ�ַΪ2�ֽ�            */
        }
        if(suba_type == 3)                                              /* �ӵ�ַ�ṹΪ8+X              */
        {
            I2C_sla         = sla + ((suba >> 7)& 0x0e);                /* �������Ĵӵ�ַ               */
            I2C_suba        = suba & 0x0ff;                             /* �����ӵ�ַ                   */
            I2C_suba_num    = 1;                                        /* �����ӵ�ַΪ8+X              */
        }
        I2C_buf     = s;                                                /* ����                         */
        I2C_num     = num;                                              /* ���ݸ���                     */
        I2C_suba_en = 2;                                                /* ���ӵ�ַ��д����             */
        I2C_end     = 0;
        LPC_I2C1->CONCLR = (1 << 2)|                                    /* ���STA,SI,AA��־λ          */
                           (1 << 3)|
                           (1 << 5);
        LPC_I2C1->CONSET = (1 << 5)|                                    /* ��λSTA,����I2C����          */
                           (1 << 6);                                    /* I2CEN                        */
        return(Wait_I2c_End(20));                                       /* �ȴ�I2C�������              */
    }
    return (FALSE);
}
/*********************************************************************************************************
** Function name:       I2C1_IRQHandler
** Descriptions:        Ӳ��I2C�жϷ������
** input parameters:    ��
** Output parameters:   ��
** Returned value:      ע�⴦���ӵ�ַΪ2�ֽڵ������
*********************************************************************************************************/
void  I2C1_IRQHandler(void)
{
    /*
     *��ȡI2C״̬�Ĵ���I2DAT,����ȫ�ֱ��������ý��в��������������־,����ж��߼�,�жϷ���
     */
    switch(LPC_I2C1->STAT & 0xF8)
    {
        /*
         *  ����״̬�������Ӧ�Ĵ���
         */
        case 0x08:                                                      /* �ѷ�����ʼ����,�����ͺ�����  */
            /* �ն���,װ��SLA+W����SLA+R    */
            if(I2C_suba_en == 1)/* SLA+R */                                 /* ָ���ӵ�ַ��                 */
            {
                LPC_I2C1->DAT = I2C_sla & 0xFE;                             /* ��д���ַ                   */
            }
            else                                                            /* SLA+W                        */
            {
                LPC_I2C1->DAT = I2C_sla;                                    /* ����ֱ�ӷ��ʹӻ���ַ         */
            }
            /* ����SIλ                     */
            LPC_I2C1->CONCLR = (1 << 3)|                                    /* SI                           */
                               (1 << 5);                                          /* STA                          */
            break;
        case 0x10:                                                      /*�ѷ����ظ���ʼ����            */
            /* �����ͺ������ն���           */
            /* װ��SLA+W����SLA+R           */
            LPC_I2C1->DAT = I2C_sla;                                        /* �������ߺ��ط��ӵ�ַ       */
            LPC_I2C1->CONCLR = 0x28;                                        /* ����SI,STA                   */
            break;
        case 0x18:
        case 0x28:                                                      /* �ѷ���I2DAT�е�����,�ѽ���ACK*/
            if(I2C_suba_en == 0)
            {
                if(I2C_num > 0)
                {
                    LPC_I2C1->DAT = *I2C_buf++;
                    LPC_I2C1->CONCLR = 0x28;                                /* ����SI,STA                   */
                    I2C_num--;
                }
                else                                                        /* û�����ݷ�����               */
                {
                    /* ֹͣ����                     */
                    LPC_I2C1->CONSET = (1 << 4);                            /* STO                          */
                    LPC_I2C1->CONCLR = 0x28;                                /* ����SI,STA                   */
                    I2C_end = 1;                                            /* �����Ѿ�ֹͣ                 */
                }
            }
            if(I2C_suba_en == 1)                                            /* ����ָ����ַ��,��������������*/
            {
                if(I2C_suba_num == 2)
                {
                    LPC_I2C1->DAT = ((I2C_suba >> 8) & 0xff);
                    LPC_I2C1->CONCLR = 0x28;                                /* ����SI,STA                   */
                    I2C_suba_num--;
                    break;
                }
                if(I2C_suba_num == 1)
                {
                    LPC_I2C1->DAT = (I2C_suba & 0xff);
                    LPC_I2C1->CONCLR = 0x28;                                /* ����SI,STA                   */
                    I2C_suba_num--;
                    break;
                }
                if(I2C_suba_num == 0)
                {
                    LPC_I2C1->CONCLR = 0x08;
                    LPC_I2C1->CONSET = 0x20;
                    I2C_suba_en = 0;                                        /* �ӵ�ַ������                 */
                    break;
                }
            }
            if(I2C_suba_en == 2)                                            /* ָ���ӵ�ַд,�ӵ�ַ��δָ��, */
            {
                /* �����ӵ�ַ                 */
                if(I2C_suba_num > 0)
                {
                    if(I2C_suba_num == 2)
                    {
                        LPC_I2C1->DAT = ((I2C_suba >> 8) & 0xff);
                        LPC_I2C1->CONCLR = 0x28;
                        I2C_suba_num--;
                        break;
                    }
                    if(I2C_suba_num == 1)
                    {
                        LPC_I2C1->DAT    = (I2C_suba & 0xff);
                        LPC_I2C1->CONCLR = 0x28;
                        I2C_suba_num--;
                        I2C_suba_en  = 0;
                        break;
                    }
                }
            }
            break;
        case 0x40:                                                      /* �ѷ���SLA+R,�ѽ���ACK        */
            if(I2C_num <= 1)                                                /* ��������һ���ֽ�           */
            {
                LPC_I2C1->CONCLR = 1 << 2;                                 /* �´η��ͷ�Ӧ���ź�           */
            }
            else
            {
                LPC_I2C1->CONSET = 1 << 2;                                 /* �´η���Ӧ���ź�             */
            }
            LPC_I2C1->CONCLR = 0x28;                                        /* ����SI,STA                   */
            break;
        case 0x20:                                                      /* �ѷ���SLA+W,�ѽ��շ�Ӧ��     */
        case 0x30:                                                      /* �ѷ���I2DAT�е����ݣ��ѽ���  */
        /* ��Ӧ��                       */
        case 0x38:                                                      /* ��SLA+R/W�������ֽ��ж�ʧ��  */
        /* ��                           */
        case 0x48:                                                      /* �ѷ���SLA+R,�ѽ��շ�Ӧ��     */
            LPC_I2C1->CONCLR = 0x28;
            I2C_end = 0xFF;
            break;
        case 0x50:                                                      /* �ѽ��������ֽڣ��ѷ���ACK    */
            *I2C_buf++ = LPC_I2C1->DAT;
            I2C_num--;
            if(I2C_num == 1)                                                /* �������һ���ֽ�             */
            {
                LPC_I2C1->CONCLR = 0x2C;                                    /* STA,SI,AA = 0                */
            }
            else
            {
                LPC_I2C1->CONSET = 0x04;                                    /* AA=1                         */
                LPC_I2C1->CONCLR = 0x28;
            }
            break;
        case 0x58:                                                      /* �ѽ��������ֽڣ��ѷ��ط�Ӧ�� */
            *I2C_buf++ = LPC_I2C1->DAT;                                     /* ��ȡ���һ�ֽ�����           */
            LPC_I2C1->CONSET = 0x10;                                        /* ��������                     */
            LPC_I2C1->CONCLR = 0x28;
            I2C_end = 1;
            break;
        default:
            break;
    }
}
