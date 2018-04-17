#ifndef  __RTC_H
#define  __RTC_H

#define ILR_RTCCIF    0x01
#define ILR_RTCALF    0x02

#define CCR_CLKEN     0x01
#define CCR_CTCRST    0x02
#define CCR_CCALEN    0x10

typedef struct {
  uint32_t RTC_Sec;                        /* �� - [0,59]           */
	uint32_t RTC_Min;                        /* ����- [0,59]           */
	uint32_t RTC_Hour;                       /* Сʱ - [0,23]             */
	uint32_t RTC_Mday;                       /* ���µĵڼ��� - [1,31] */
	uint32_t RTC_Mon;                        /* �� - [1,12]            */
	uint32_t RTC_Year;                       /* �� - [0,4095]           */
	uint32_t RTC_Wday;                       /* �ܵĵڼ���(���Բ���) - [0,6]       */
	uint32_t RTC_Yday;                       /* ��ĵڼ���(���Բ���) - [1,365]     */
}RTCTime;

void RTCSetTime(RTCTime Time);
RTCTime RTCGetTime(void);
void RTCInit(void);
void RTCStart(void);
void RTCStop(void);
#endif
