
#ifndef __CTRL_H__
#define __CTRL_H__


/*********************************************************************************************************
  ʹ�ܶ˿ڹܽŶ���
*********************************************************************************************************/
#define BEEP      		        (1ul << 5) 	 //P1.5			 
#define BEEP_ON()     	        LPC_GPIO1->DIR |= BEEP;LPC_GPIO1->SET = BEEP 
#define BEEP_OFF()              LPC_GPIO1->DIR |= BEEP;LPC_GPIO1->CLR = BEEP

#define LED1       		        (1ul << 16)  //P4.16			 
#define LED1_OFF()     	        LPC_GPIO4->DIR |= LED1;LPC_GPIO4->SET = LED1 
#define LED1_ON()      	        LPC_GPIO4->DIR |= LED1;LPC_GPIO4->CLR = LED1
#define LED1STATUS              (LPC_GPIO4->SET & LED1)

#define ZIGBEE_CFG              (1ul << 4)  //p2.4 zigbeeģ��cfg
#define ZIGBEE_CFGH             LPC_GPIO2->DIR |= ZIGBEE_CFG;LPC_GPIO2->SET = ZIGBEE_CFG    
#define ZIGBEE_CFGL             LPC_GPIO2->DIR |= ZIGBEE_CFG;LPC_GPIO2->CLR = ZIGBEE_CFG

#define ZIGBEE_RET              (1ul << 5)  //p2.5 zigbeeģ��ret
#define ZIGBEE_RETH             LPC_GPIO2->DIR |= ZIGBEE_RET;LPC_GPIO2->SET = ZIGBEE_RET    
#define ZIGBEE_RETL             LPC_GPIO2->DIR |= ZIGBEE_RET;LPC_GPIO2->CLR = ZIGBEE_RET

//����ΪP2��P14��·����������
#define SwitchInput1            (1ul << 0)  //p5.0
#define SI1STATUS               (LPC_GPIO5->PIN & SwitchInput1)

#define SwitchInput2            (1ul << 30) //p2.30
#define SI2STATUS               (LPC_GPIO2->PIN & SwitchInput2)


#define WDI				        (1ul << 1) 	 //P5.1
#define	WDI_H()	 		        LPC_GPIO5->SET = WDI
#define	WDI_L()	 		        LPC_GPIO5->CLR = WDI 
#define	WATCH_DOG()	 	        WDI_H();WDI_L()	

#define WDDIR()					(LPC_GPIO5->DIR |= WDI)//���Ź�����
#define FEEDDOG()				(LPC_GPIO5->PIN ^= WDI)//ι����

/********************************************************************
˵����PLC�˿ڷ�����ơ�
*********************************************************************/
#define PUMPCTRL_HIGH            (1ul << 24)//p3.24
#define PUMPCTRL_HIGH_OPEN()     LPC_GPIO3->DIR |= PUMPCTRL_HIGH;LPC_GPIO3->CLR = PUMPCTRL_HIGH
#define PUMPCTRL_HIGH_CLOSE()    LPC_GPIO3->DIR |= PUMPCTRL_HIGH;LPC_GPIO3->SET = PUMPCTRL_HIGH

#define PUMPCTRL_LOW             (1ul << 25)//p3.25
#define PUMPCTRL_LOW_OPEN()      LPC_GPIO3->DIR |= PUMPCTRL_LOW;LPC_GPIO3->CLR = PUMPCTRL_LOW
#define PUMPCTRL_LOW_CLOSE()     LPC_GPIO3->DIR |= PUMPCTRL_LOW;LPC_GPIO3->SET = PUMPCTRL_LOW


/********************************************************************
˵������ŷ��˿ڷ�����ơ�
*********************************************************************/
#define VCTRL_HIGH              (1ul << 22)//p3.22
#define VCTRL_HIGH_OPEN()       LPC_GPIO3->DIR |= VCTRL_HIGH;LPC_GPIO3->CLR = VCTRL_HIGH
#define VCTRL_HIGH_CLOSE()      LPC_GPIO3->DIR |= VCTRL_HIGH;LPC_GPIO3->SET = VCTRL_HIGH

#define VCTRL_MID               (1ul << 21)//p3.21
#define VCTRL_MID_OPEN()        LPC_GPIO3->DIR |= VCTRL_MID;LPC_GPIO3->CLR = VCTRL_MID
#define VCTRL_MID_CLOSE()       LPC_GPIO3->DIR |= VCTRL_MID;LPC_GPIO3->SET = VCTRL_MID

#define VCTRL_LOW               (1ul << 20)//p3.20
#define VCTRL_LOW_OPEN()        LPC_GPIO3->DIR |= VCTRL_LOW;LPC_GPIO3->CLR = VCTRL_LOW
#define VCTRL_LOW_CLOSE()       LPC_GPIO3->DIR |= VCTRL_LOW;LPC_GPIO3->SET = VCTRL_LOW

#define VCTRL_KEY               (1ul << 23)//p3.23
#define VCTRL_OPENKEY()         LPC_GPIO3->DIR |= VCTRL_KEY;LPC_GPIO3->CLR = VCTRL_KEY
#define VCTRL_CLOSEKEY()	    LPC_GPIO3->DIR |= VCTRL_KEY;LPC_GPIO3->SET = VCTRL_KEY


//���ںŶ���
#define CardPort       0
#define PrintPort      0
#define ModbusPort     1
#define LCDPort        2
#define PCPort         3
#define RfidPort       4


#define Ulong unsigned long long 
	

enum    Sys_Err                //ϵͳ�쳣ԭ��
{
    SYSERR_FLASH_A_CHK,         //����Flash���ʧ�� 0
    SYSERR_FLASH_B_CHK,         //����Flash���ʧ�� 1
    SYSERR_W_FLASH_A,           //д�����2
    SYSERR_R_FLASH_A,           //�������3
    SYSERR_W_FLASH_B,           //д�����4
    SYSERR_R_FLASH_B,           //�������5
    SYSERR_W_IN_EEPROM,         //д�ڲ�eeprom���� 6   
    SYSERR_R_IN_EEPROM,         //���ڲ�eeprom���� 7
    SYSERR_W_EX_EEPROM,         //д�ⲿeeprom���� 8
    SYSERR_R_EX_EEPROM,         //���ⲿeeprom���� 9
    SYSERR_INANDEX_EEPROM,      //�ڲ�eeprom���ⲿeeprom�������ݲ�һ�� 10  
    SYSERR_T_FM25L04,           //������Դ��� 11
    SYSERR_MODBUS_G_AUTO_ERR,   //����������ͨѶ���� 12
    SYSERR_MODBUS_L_AUTO_ERR,   //Һ��������ͨѶ���� 13
    SYSERR_PRESSURE,            //ѹ���������쳣14
};


/********************************/
void    GPIOInit(void);
void    BeepOut(uint8_t N_100ms);
void    RunLed1(void);


#endif

