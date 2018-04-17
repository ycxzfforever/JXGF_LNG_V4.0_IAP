#include "kernel.h"

/********************************************************
**��������:RTCSetTime
**��������:����ʱ��
**�������:Time---RTCTime�ṹ��(��rtc.h,���������ա�ʱ�����)
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void RTCSetTime(RTCTime Time) 
{
  LPC_RTC->SEC   = Time.RTC_Sec;
  LPC_RTC->MIN   = Time.RTC_Min;
  LPC_RTC->HOUR  = Time.RTC_Hour;
  LPC_RTC->DOM   = Time.RTC_Mday;
  LPC_RTC->DOW   = Time.RTC_Wday;
  LPC_RTC->DOY   = Time.RTC_Yday;
  LPC_RTC->MONTH = Time.RTC_Mon;
  LPC_RTC->YEAR  = Time.RTC_Year;     
}


/********************************************************
**��������:RTCGetTime
**��������:��ȡʱ��
**�������:��
**���ֵ  :���ص�ǰʱ�䣬��RTCTime�ṹ���ʾ

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
RTCTime RTCGetTime(void) 
{    
  RTCTime LocalTime;
  LocalTime.RTC_Sec  = LPC_RTC->SEC;
  LocalTime.RTC_Min  = LPC_RTC->MIN;
  LocalTime.RTC_Hour = LPC_RTC->HOUR;
  LocalTime.RTC_Mday = LPC_RTC->DOM;
  LocalTime.RTC_Wday = LPC_RTC->DOW;
  LocalTime.RTC_Yday = LPC_RTC->DOY;
  LocalTime.RTC_Mon  = LPC_RTC->MONTH;
  LocalTime.RTC_Year = LPC_RTC->YEAR;
  return (LocalTime);   
}

/********************************************************
**��������:RTCStart
**��������:�ڲ�RTC��ʼ����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
void RTCStart(void) 
{
                                               /* Start RTC counters        */
  LPC_RTC->ILR = (ILR_RTCCIF | ILR_RTCALF);    /* ��������������ж� ��־   */
  LPC_RTC->CCR |= CCR_CLKEN;                   /* ʹ��                      */
  /*LPC_SC->PCONP &= ~(1 << 9);                   ������� ��ʹ���ⲿʱ��   */
  return;
}

void RTCStop(void)
/********************************************************
**��������:RTCStart
**��������:�ڲ�RTCֹͣ����
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
{    
  LPC_RTC->CCR &= ~CCR_CLKEN;                    /* Stop RTC counters       */
  return;
} 

void RTCInit(void)
/********************************************************
**��������:RTCInit
**��������:�ڲ�RTC��ʼ��
**�������:��
**���ֵ  :��

**������  :�ƺ���
**��������:2016-04-13

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/
{
  LPC_SC->PCONP |=(1<<9);                    /* ���� RTC Ҫ�õ��ⲿʱ��   */
  if(LPC_RTC->RTC_AUX&(0x1<<4))              /* RTC ����ʧЧ̽���־    */
  {
    LPC_RTC->RTC_AUX |=(0x1<<4);               /* ���                      */
  }
  LPC_RTC->CCR =0;                             /* ����                     */
  LPC_RTC->ILR =(ILR_RTCCIF|ILR_RTCALF);     /* ��������������ж� ��־  */
  LPC_RTC->AMR =0xFF;                          /* ȫ���α���               */
  LPC_RTC->CIIR =0;                            /* ������������             */
  LPC_RTC->CALIBRATION =0x0;                   /* У׼�Ĵ�����0            */
  return;
}
