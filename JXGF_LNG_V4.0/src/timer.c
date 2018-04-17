#include "kernel.h"

Timer0  timer0;

/*********************************************************************************************************
** �������ƣ�timer0Init
** ������������ʱ��0��ʼ������   fighting
** �����������                                  SystemCoreClock
** ����ֵ  ����
*********************************************************************************************************/
void timer0Init(void)
{
	LPC_SC->PCONP |=(0x01<<1); //�򿪶�ʱ��0����ģ��
    LPC_TIM0->TCR  = 0x02;  /*��ʱ�����ƼĴ�����TCR�������ƶ�ʱ���������Ĺ��ܡ���ʱ������������ͨ��TCR�����ܻ�λ��*/
    LPC_TIM0->IR   = 1;  	/*�жϼĴ���������IRд����Ӧ��ֵ������жϡ�Ҳ�ɶ�IR��ȷ���ж�Դ���ĸ��ж�Դ������*/             /*�ж�ʹ��*/
    LPC_TIM0->CTCR = 0;  	/*CTCRѡ���ڶ�ʱ��ģʽ���Ǽ�����ģʽ�¹������ڼ�����ģʽ������ѡ��Ҫ�������źź��ء�*/
    LPC_TIM0->TC   = 0;  	/*��ʱ����������32λ��TCÿ����PR+1����PCLK���ڵ���һ�Ρ�TCͨ��TCR�����ơ�*/
    LPC_TIM0->PR   = 0;  	/*Ԥ��Ƶ�Ĵ����� ��Ԥ��Ƶ�����������£���ֵ������Ĵ�����ֵ���ʱ���¸�ʱ��TC�����TC��PC���㡣*/
    LPC_TIM0->MR0  = 100*(PeripheralClock/1000-1);  /* 100MS�ж�1��                  */
    LPC_TIM0->MCR  = 0x03;                    /*ƥ����ƼĴ�����MCR����������ƥ�����ʱ�Ƿ�����жϺ��Ƿ�λTC��*/       /* ƥ�������ж�               */

    NVIC_EnableIRQ(TIMER0_IRQn);                                          /* �����жϲ�ʹ��               */
    NVIC_SetPriority(TIMER0_IRQn, 3);
    LPC_TIM0->TCR  = 0x01;                                                /* ������ʱ��                   */
}

uint32_t Timer0GetTick(void)
{
	return(timer0.tick_100ms);
}

void Timer0Reset(void)
{
	LPC_SC->PCONP &=~(0x01<<1);
	timer0.tick_100ms=0;
	timer0Init();
}

/*********************************************************************************************************
** �������ƣ�TIMER0_IRQHandler
** ����������TIMER0 �жϴ�����
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void TIMER0_IRQHandler(void)
{  
    //LPC_TIM0->IR = 0x01;                                        /* ����жϱ�־                 */
    timer0.tick_100ms++;
    timer0.flag_100ms=1;
    if(timer0.tick_100ms%5==0)
    {
        timer0.flag_500ms=1;
        if(m1card.debittimes>0)
        {
            m1card.debittimes--;
        }
    }    
    if(timer0.tick_100ms%10==0)
    {
        timer0.flag_1s=1;
        globalvar.comfailcount++;  //��̨������ͨ���Ƿ�������־��
        if(globalvar.comfailcount>200)
        {
            globalvar.comfailcount=200;
        }
        globalvar.timeout++;//���㳬ʱʱ��       
    }
    LPC_TIM0->IR = 0x01;                                        /* ����жϱ�־                 */
    if(timer0.tick_100ms>1000000)  timer0.tick_100ms=0;    
}


void timer1on(void)
{
   LPC_TIM1->TCR  = 0x01; 
}

void timer1off(void)
{
   LPC_TIM1->TCR  = 0x00; 
}

/*********************************************************************************************************
** �������ƣ�timer1Init
** ������������ʱ��1��ʼ������
** �����������                                  
** ����ֵ  ����
*********************************************************************************************************/
void timer1Init(void)
{
	LPC_SC->PCONP |=(0x01<<2); //�򿪶�ʱ��1����ģ��
    LPC_TIM1->TCR  = 0x02;  /*��ʱ�����ƼĴ�����TCR�������ƶ�ʱ���������Ĺ��ܡ���ʱ������������ͨ��TCR�����ܻ�λ��*/
    LPC_TIM1->IR   = 1;  	/*�жϼĴ���������IRд����Ӧ��ֵ������жϡ�Ҳ�ɶ�IR��ȷ���ж�Դ���ĸ��ж�Դ������*/             /*�ж�ʹ��*/
    LPC_TIM1->CTCR = 0;  	/*CTCRѡ���ڶ�ʱ��ģʽ���Ǽ�����ģʽ�¹������ڼ�����ģʽ������ѡ��Ҫ�������źź��ء�*/
    LPC_TIM1->TC   = 0;  	/*��ʱ����������32λ��TCÿ����PR+1����PCLK���ڵ���һ�Ρ�TCͨ��TCR�����ơ�*/
    LPC_TIM1->PR   = 0;  	/*Ԥ��Ƶ�Ĵ����� ��Ԥ��Ƶ�����������£���ֵ������Ĵ�����ֵ���ʱ���¸�ʱ��TC�����TC��PC���㡣*/
    LPC_TIM1->MR0  = 100*(PeripheralClock/1000-1);  /* 100MS�ж�1��                  */
    LPC_TIM1->MCR  = 0x03;                    /*ƥ����ƼĴ�����MCR����������ƥ�����ʱ�Ƿ�����жϺ��Ƿ�λTC��*/       /* ƥ�������ж�               */
    NVIC_EnableIRQ(TIMER1_IRQn);                                          /* �����жϲ�ʹ��               */
    NVIC_SetPriority(TIMER1_IRQn, 4);
    LPC_TIM1->TCR  = 0x0;                                                /* ������ʱ��                   */
}

//�������ƺ�ѹ�������� ycx20170329
void ReadModbusAndPressure(void)
{
    if(sysparas.simulation==0)
    {                
        if((globalvar.readLorG==0)||(sysparas.backgascalc==0))//����������
        {
            ModbusReadScanBuffer(_MODBUS_L_);
            globalvar.modbus_l_error++;
            if(globalvar.modbus_l_error>5)
            {
                SetBit(syserr, SYSERR_MODBUS_L_AUTO_ERR);//Һ��������ͨѶ����
                globalvar.stoptimes=1000;//Һ���������쳣ֱ��ͣ��
                stopreason=2;
            }
        }
        if(((sysparas.backgascalc==1)||(GetBit(syserr, SYSERR_MODBUS_G_AUTO_ERR)==1))
            &&(globalvar.readLorG>0))//�����������������쳣ʱ��
        {
            ModbusReadScanBuffer(_MODBUS_G_);            
            globalvar.modbus_g_error++;
            if(globalvar.modbus_g_error>5)
            {
                SetBit(syserr, SYSERR_MODBUS_G_AUTO_ERR);//����������ͨѶ����
                globalvar.stoptimes=1000;//�����������쳣ֱ��ͣ��
                stopreason=1;
            }          
        }
        if(!PressureGetValue())
		{
		    globalvar.pressure_error++;
            if(globalvar.pressure_error>10)
            {
    			SetBit(syserr,SYSERR_PRESSURE);
                globalvar.stoptimes=1000;//ѹ���������쳣ֱ��ͣ��
                stopreason=4;
            }
		}
		else
		{
			globalvar.pressure_error=0;
            ClrBit(syserr,SYSERR_PRESSURE);
		}        
    }
}

/*********************************************************************************************************
** �������ƣ�TIMER1_IRQHandler
** ����������TIMER1 �жϴ�����,���ڶ�ȡ����������
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void TIMER1_IRQHandler(void)
{      
    ReadModbusAndPressure();
    LPC_TIM1->IR = 0x01; 
}



