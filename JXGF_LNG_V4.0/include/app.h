#ifndef APP_H_
#define APP_H_

extern uint8_t    LoadSuccess ;      //���������Ƿ���ɡ�
extern void StrReverse(uint8_t *buf,uint8_t num,uint8_t n);
extern void BufReverse2(uint8_t *buf, uint8_t num);
extern uint8_t SendBack485Buf[BUFSIZE];
extern uint8_t RecBack485Buf[BUFSIZE];
void ClearBuf(volatile uint8_t *buf,uint8_t num,uint16_t len);
bool RecBlckDataCheck(void);
void GetRep51Data(void);
void UpLoad51H(void);
void UpLoad52H(void);
void UpLoad53H(void);
void UpLoad54H(void);
void UpLoad55H(uint8_t type);
void UpLoad56H(uint8_t type);
void UpLoad5AH(void);
void UpLoad5BH(void);
void UpLoad5CH(void);
void UpLoad5EH(void);
void BGCommunication(uint8_t data);
/********************************************************
**�ṹ����:Reply51
**�ṹ����:��̨51Ѳ����������ʱ����������Ӧ��̨�����ݽṹ��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/  
typedef struct 
{
    uint8_t   LCNG;                   //CNG/LNG���
    uint8_t   status;                 //������״̬,   1�ֽڡ�
    uint16_t  price;                  //���׵��ۣ�    2�ֽڡ�
    uint32_t  gasnum;                 //��ǰ����������4�ֽڡ�
    uint32_t  gasmoney;               //��ǰ������4�ֽڡ�
    uint8_t   classnum;               //��ţ�    1�ֽڡ�
    uint8_t   employeenum[8];         //Ա�����룬8�ֽڣ�BCD�롣
    uint8_t   cardnum[8];             //�û����룬8�ֽڣ�BCD�롣
    uint8_t   carnum[9];              //���ƺ��룬7�ֽڣ�ASCII�롣
    uint16_t  curpressure;            //��ǰѹ����2�ֽڡ�
    float     inflowrate;             //����������4�ֽڡ�
    float     intemperature;          //�����¶ȣ�4�ֽڡ�
    float     inamount;               //����������4�ֽڡ�
    float     indensity;              //�����ܶȣ�4�ֽڡ�
    float     ingain;                 //�������棬4�ֽڡ�
    float     returnflowrate;         //����������4�ֽڡ�
    float     returntemperature;      //�����¶ȣ�4�ֽڡ�
    float     returnamount;           //����������4�ֽڡ�
    float     returndensity;          //�����ܶȣ�4�ֽڡ�
    float     returngain;             //�������棬4�ֽڡ�
    uint32_t  systemalarm;            //ϵͳ������4�ֽڡ�
}Reply51;

extern Reply51 UpLoad51Data; 

/********************************************************
**�ṹ����:Reply52
**�ṹ����:52����������ϴ����׼�¼��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint32_t  transactionid;          //������ˮ�ţ�4�ֽڡ�
    uint8_t   classnum;               //��ţ�1�ֽڡ�
    uint8_t   employeenum[8];         //Ա�����룬10�ֽڣ�BCD�롣
    uint8_t   transactiontype;        //�������ͣ�1�ֽڡ�
    uint8_t   usernum[8];             //�û����ţ�10�ֽ�,BCD�롣
    uint8_t   cardtype;               //������ �� 1�ֽڡ�
    uint8_t   carnum[9];              //���ƺ��룬7�ֽڣ�ASCII�롣
    uint8_t   stationnum[4];          //����վ���룬4�ֽڣ�BCD�롣
    uint8_t   starttime[7];           //������ʼʱ�䣬7�ֽڣ�BCD�롣
    uint8_t   endtime[7];             //��������ʱ�䣬7�ֽڣ�BCD�롣
    uint16_t  price;                  //���μ�������,2�ֽڡ�
    uint32_t  gasnum;                 //��ǰ����������4�ֽڡ�
    uint32_t  gasmoney;               //��ǰ������4�ֽڡ�
    uint32_t  reallymoney;            //����ʵ�ʿۿ��
    uint32_t  beforebalance;          //����ǰ��4�ֽڡ�
    uint32_t  afterbalance;           //��������4�ֽڡ�
    uint32_t  allrechargetimes;       //�ۼƳ�ֵ������4�ֽڡ�
    uint32_t  allrechargemoney;       //�ۼƳ�ֵ��4�ֽڡ�
    uint32_t  allconsumptiontimes;    //�ۼ����Ѵ�����4�ֽڡ�
    uint32_t  allconsumptionnum;      //�ۼ�����������4�ֽڡ�
    uint32_t  allconsumptionmoney;    //�ۼ����ѽ�4�ֽڡ�
    uint32_t  allgunnum;              //ǹ�����ۼƣ�4�ֽڡ�
    uint32_t  allgunmoney;            //ǹ����ۼ�,4�ֽڡ�
    uint16_t  beforepressure;         //����ǰѹ����2�ֽڡ�
    uint16_t  afterpressure;          //������ѹ����2�ֽڡ�
    uint32_t  density;                //�ܶȣ�4�ֽڡ�
    uint32_t  curnum;                 //������4�ֽڡ�
    uint8_t   devicetype;             //�豸����0�� LNG ������1�� CNG ������2�� ������3�� ж����
    uint8_t   measuretype;            //������λ��1�ֽڣ�1-�귽��0-Kg��
    uint8_t   stopreason;             //ͣ��ԭ�� 
    uint32_t  jhid;                   //��������ˮ�š�
    uint32_t  classid;                //����ˮ�š�
}Reply52;

extern Reply52 UpLoad52Data; 

/********************************************************
**�ṹ����:Reply53
**�ṹ����:53��������������������ݡ�

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct
{
    uint8_t  loadtype;               //��Ҫ�������ݵ����ͣ�1�ֽڡ�
    uint8_t  presurve[6];            //������6�ֽڡ�
}Reply53;

extern Reply53 UpLoad53Data; 

/********************************************************
**�ṹ����:Reply54
**�ṹ����:54������������������������ݡ�

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct
{
    uint8_t   loadtype;               //��Ҫ�������ݵ����ͣ�1�ֽڡ�
    uint16_t  partoffset;             //�����ؿ��Ÿ�����
    uint8_t   partnum;                //�������ؿ��Ÿ�����
}Reply54;

extern Reply54 UpLoad54Data; 

/********************************************************
**�ṹ����:Reply55
**�ṹ����:55��������������ҡ�

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct
{
    uint8_t  type;                   //���͡�
    uint8_t  cardtype;               //�����͡��¼ӵģ�20160728��
    uint8_t  cardnum[8];             //���ţ�BCD�룬8�ֽڡ�
    uint32_t gasnum;                 //��ǰ����������
    uint32_t money;                  //��ǰ������
    uint32_t cardover;               //����
    uint8_t  stationnum[4];          //վ���룬BCD�롣
    uint8_t  gunnum;                 //ǹ���һ�μ���ǹ�š�
    uint32_t gunnumber;              //�����һ�μ���ǹ�ļ�����¼��ˮ�š�
   
}Reply55;

extern Reply55 UpLoad55Data; 

typedef struct	
{
	uint8_t		greyrecord;		//�Ƿ��ҵ��Ҽ�¼0��δ�ҵ� 1���ҵ�
	uint8_t		type;			//����0������ۿ� 1��������
	uint8_t		cardnum[8];		//����BCD
	uint32_t	realmoney;		//ʵ�ʼ������
	uint32_t	realtrademoney;	//ʵ�ʿۿ���
	uint32_t	beforemoney;	//����ǰ���
	
	uint32_t	aftermoney;		//���������
	uint16_t	price;			//����
	
	uint32_t	volume;			//����
	uint32_t	totaltimes;		//�ۼ����Ѵ���
	uint32_t	totalmoney;		//�ۼ����ѽ��
	
	uint32_t	totalvolume;	//�ۼ���������
	uint32_t	removegrey;		//��������ˮ����
}	Receive55data;

extern Receive55data receive55data;

/********************************************************
**�ṹ����:Reply56
**�ṹ����:56��������������ѯ�ڰ�������

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint8_t  cardnum[8];             //���ţ�BCD�룬8�ֽڡ�
    uint8_t  seekname;               //������/�ף�0-��ڣ�1-��ס�
}Reply56;

extern Reply56 UpLoad56Data;  

/********************************************************
**�ṹ����:Reply57
**�ṹ����:57��������������ۼ�������̨ϵͳ��

**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint32_t  transactionid;          //������ˮ�ţ�4�ֽڡ�
    uint32_t  allgasnum;              //�����������ۼơ�
    uint32_t  allgasmoney;            //����������ۼơ�
    uint32_t  allclassnum;            //�������ۼơ�
    uint32_t  allclassmoney;          //�����ۼơ�
}Reply57;

extern Reply57 UpLoad57Data; 

/********************************************************
**�ṹ����:Reply58
**�ṹ����:58�����̨ϵͳ��������ָ����ˮ��¼��
          ������������ˮ��¼����̨ϵͳ�����������������
          a. ���иü�¼�����������֡�52H����Ϣ�����ݻظ���
          b. ��û�иü�¼����ظ����½ṹ���ݡ�
**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint32_t  transactionid;          //������ˮ�ţ�4�ֽڡ�
    uint8_t   ifid;                   //�Ƿ��иñ���ˮ��¼�� 
}Reply58;

extern Reply58 UpLoad58Data;
/********************************************************
**�ṹ����:Reply59
**�ṹ����:59������������ؼ�������Ϣ����̨ϵͳ��
       
**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint8_t  gunnum;                 //ǹ��,BCD�롣
    uint32_t transactionid;          //������ˮ�ţ�4�ֽڡ�
    uint8_t  baseblackversion;       //�����������汾�š�
    uint8_t  newaddblackversion;     //�����������汾�š�
    uint8_t  newdelblackversion;     //��ɾ�������汾�š�
    uint8_t  whitelistversion;       //�������汾�š�
    uint8_t  priceversion;           //���۰汾�š�
    uint8_t  baseinfoversion;        //ͨ����Ϣ�汾�š�
    uint8_t  stationnum[4];          //վ���룬BCD�롣
    uint8_t  provincenum[2];         //ʡ���룬BCD�롣
    uint8_t  citynum[2];             //���д��룬BCD�롣
    uint8_t  parentunitnum[4];       //�ϼ���λ���룬BCD�롣
    uint8_t  stationname[40];        //����վ���ƣ�ASCII�롣
    uint8_t  hardsoftwareversion[16];//Ƕ��ʽ����汾��,ASCII�롣
    uint8_t  manufacturerinfo[40];   //������������Ϣ��ASCII�롣
}Reply59;

extern Reply59 UpLoad59Data; 


/********************************************************
**�ṹ����:Reply5A 
**�ṹ����:5A����������ϴ����°��¼��
       
**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint32_t  classid;                //���°��¼��ˮ�ţ�4�ֽڡ�
    uint8_t   transactiontype;        //�������ͣ�1�ֽڡ�
    uint8_t   classnum;               //��ţ�1�ֽڡ�
    uint8_t   cardstaffnum[8];        //Ա�����ţ�BCD�롣
    uint8_t   classstarttime[7];      //�࿪ʼʱ�䣬7�ֽڣ�BCD�롣
    uint8_t   classendtime[7];        //�����ʱ�䣬7�ֽڣ�BCD�롣
    uint32_t  allgunnum;              //ǹ�����ۼƣ�4�ֽڡ�
    uint32_t  allgunmoney;            //ǹ����ۼ�,4�ֽڡ�
    uint32_t  classgasnum;            //��������4�ֽڡ�
    uint32_t  classgasmoney;          //����,4�ֽڡ�
    uint32_t  classtimes;             //�����������4�ֽڡ�
    uint8_t   devicetype;             //�豸����0�� LNG ������1�� CNG ������2�� ������3�� ж����
    uint8_t   measuretype;            //������λ��1�ֽڣ�1-�귽��0-Kg��
    uint32_t  classiding;             //�ϰ��¼��ˮ�š�
}Reply5A;

extern Reply5A UpLoad5AData; 

/********************************************************
**�ṹ����:Reply5B 
**�ṹ����:5B����������ϴ������޸ļ�¼��
       
**�� �� ��:���
**��������:2016-05-17

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint32_t  modifyid;               //�����޸ļ�¼��ˮ�ţ�4�ֽڡ�
    uint8_t   classnum;               //��ţ�1�ֽڡ�
    uint8_t   cardstaffnum[8];        //ά�������ʼ쿨�ȣ�BCD�롣
    uint16_t  paraindex;              //���������š�
    uint8_t   dexnum;                 //С��λ����
    uint32_t  beforeparameter;        //�޸�ǰ����ֵ�� 
    uint32_t  afterparameter;         //�޸ĺ����ֵ��    
    uint8_t   modifytime[7];          //�޸�ʱ�䣬7�ֽڣ�BCD�롣   
}Reply5B;

extern Reply5B UpLoad5BData; 

/********************************************************
**�ṹ����:Reply5C 
**�ṹ����:5C�����������֤IC�������ԡ�
       
**�� �� ��:���
**��������:2016-06-8

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint8_t   cardnum[8];       //IC�����ţ�BCD�롣
    uint32_t  balance;          //��
    uint8_t   endtime[7];       //�����ʱ�䡣
}Reply5C;

extern Reply5C UpLoad5CData; 

/********************************************************
**�ṹ����:Reply5D 
**�ṹ����:5D��������������°�����
       
**�� �� ��:���
**��������:2016-06-8

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint8_t OffWork;        //�°��Ƿ�ɹ���
}Reply5D;

extern Reply5D UpLoad5DData;

/********************************************************
**�ṹ����:Reply5E 
**�ṹ����:5E��������������·����ۡ�
       
**�� �� ��:���
**��������:2016-06-8

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint8_t lcng;        //��Ʒ���롣1��lng��0��cng��
}Reply5E;

extern Reply5E UpLoad5EData;


/********************************************************
**�ṹ����:Reply5F 
**�ṹ����:5F��������������Ƿ����������
       
**�� �� ��:���
**��������:2016-06-8

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint8_t commdwork;        //�Ƿ�������������
}Reply5F;

extern Reply5F UpLoad5FData;
/********************************************************
**�ṹ����:BWInfo 
**�ṹ����:�ڰ������汾��Ϣ��
       
**�� �� ��:���
**��������:2016-07-8

**�޸���  :
**�޸�����:
**�޸�����:
********************************************************/ 
typedef struct 
{
    uint8_t version;      //�汾�š�   
    uint8_t starttime[4]; //��Ч���ڡ�
    uint8_t stoptime[4];  //��ֹ���ڡ�
    uint8_t effectarea[2];//�ڰ�������Ч����
    uint32_t namenum;     //����������
    uint8_t reserve;      //������
}BWInfo;

extern BWInfo bwinfo;
#endif
