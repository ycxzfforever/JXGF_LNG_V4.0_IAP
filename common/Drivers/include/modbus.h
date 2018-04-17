#ifndef __MODBUS_H
#define __MODBUS_H

#define  _MODBUS_G_AUTO_BUF_LEN		5 	
#define  _MODBUS_L_AUTO_BUF_LEN		5 

#define  MODBUS_OK			    	1


#define  MODBUS_TIMEOUT             500000

enum    e_modbus_type   //����������
{
	MODBUS_TYPE_NO = 0,
	MODBUS_TYPE_RM,       // 1Ϊ��˹����
	MODBUS_TYPE_EH,       // 2ΪE+H
};

enum    e_modbus_addr   //�����Ƶ�ַ
{
    _MODBUS_G_ = 1, // 1����
    _MODBUS_L_=2,     // 2Һ��
};


typedef struct MassFlowModbus
{
  uint8_t   sendbuf[10];    //�������Ʒ�������buffer
  uint8_t   recbuf[40];     //��������������buffer
  uint8_t   flag;           //����1ʱ��ʾ���ݽ�����ȷ
}MassFlowModbus;
extern  MassFlowModbus      modbus;


typedef struct RealTimeData //�������ƺ�ѹ��������ʵʱ����
{    
    uint8_t           lcng;                  //CNG/LNG��ǡ�
    uint32_t          transnum;              //������ˮ�š�
    uint8_t           workstatus;            //����������״̬��
    uint16_t          price;                 //��ǰ���׵��ۡ�
    uint32_t          backgas;               //������
    uint32_t          volume;                //��ǰ��������
    uint32_t          money;                 //��ǰ������
    uint8_t           shiftnum;              //��ǰ��š�
    uint8_t           employeenum[8];        //��ǰԱ�����š�
    uint8_t           cardnum[8];            //��ǰ�û������š�
    uint8_t           carnum[9];             //���ƺš�
    uint8_t           cardtype;              //�����͡�
    uint8_t           transactiontype;       //�������ͣ�1�ֽڡ�
    uint8_t           starttime[7];          //������ʼʱ�䡣
    uint8_t           endtime[7];            //��������ʱ�䡣
    uint32_t          reallymoney;           //����ʵ�ʿۿ��
    uint32_t          beforebalance;         //����ǰ��4�ֽڡ�
    uint32_t          afterbalance;          //��������4�ֽڡ�
    uint32_t          allrechargetimes;      //�ۼƳ�ֵ������4�ֽڡ�
    uint32_t          allrechargemoney;      //�ۼƳ�ֵ��4�ֽڡ�
    uint32_t          allconsumptiontimes;   //�ۼ����Ѵ�����4�ֽڡ�
    uint32_t          allconsumptionnum;     //�ۼ�����������4�ֽڡ�
    uint32_t          allconsumptionmoney;   //�ۼ����ѽ�4�ֽڡ�
    uint16_t          beforepressure;        //����ǰѹ����2�ֽڡ�
    uint16_t          afterpressure;         //������ѹ����2�ֽڡ�
    
    uint32_t          curnum;                //������4�ֽڡ�����ϵ����
    uint8_t           measuretype;           //������λ��1�ֽڣ�1-�귽��0-Kg��
    uint8_t           stopreason;            //ͣ��ԭ�� 
    uint32_t          jhid;                  //��������ˮ�š�
    uint32_t          classid;               //����ˮ�š�
    uint8_t           fuelmode;              //������ʽ
    uint32_t          cur_pressure;          //��ǰѹ����ѹ������ʱ���Ѿ�������100����
    uint8_t           powerdown;             //�Ƿ���磬0-������1-���硣
    float             cur_flowrate_l;        //��ǰҺ������
    float             cur_temperature_l;     //��ǰҺ���¶�
    float             cur_totalize_l;        //��ǰҺ������
    float             cur_density_l;         //��ǰҺ���ܶ�
    float             cur_gain_l;            //��ǰҺ������
    
    float             cur_flowrate_g;        //��ǰ��������
    float             cur_temperature_g;     //��ǰ�����¶�    
    float             cur_totalize_g;        //��ǰ��������      
    float             cur_density_g;         //��ǰ�����ܶ�    
    float             cur_gain_g;            //��ǰ��������        
}RealTimeData;


uint16_t ModbusCrc16(uint8_t *buf, uint16_t Len);
uint8_t  ModbusReadReg(uint8_t modbus_addr,uint16_t RegAddr, uint8_t regnum );
void     ModbusReadScanReg(uint8_t modbus_addr,uint16_t RegAddr, uint8_t regnum);
uint8_t  ModbusWriteReg(uint8_t modbus_addr, uint8_t modbus_coil, uint16_t reg_id, uint16_t v_value);
uint8_t  ModbusReadDeviceAddress(uint8_t modbus_addr,uint16_t *rdaddress);
uint8_t  ModbusReadTotal(uint8_t modbus_addr,float *total);
uint8_t  ModbusReadFlow(uint8_t modbus_addr,float *flow);
uint8_t  ModbusClearTotal(uint8_t modbus_addr);
uint8_t  ModbusInitReg(void);
void     ModbusReadScanBuffer(uint8_t modbus_addr);
uint8_t  ModbusComTest(uint8_t modbus_addr);
void     ModbusDataDisposs(void);

void     UART1_IRQHandler( void );
void     ModbusRecDataCheck(void);


#endif
