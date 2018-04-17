#ifndef __MENU_H
#define __MENU_H

typedef void(*fun)(void);  

typedef void(*fun1)(fun);  


void TaskAdd(uint32_t keyvalue,fun funname1,fun funname2);

extern fun         DispUI;          //����ˢ�´���
extern fun         DispNextUI;      //������Ϣ��ʾ�����һ������


void DispFirst(void);

// ������λ���ʼ�ģʽ���ָ���������
void DispUnitQcReset(void);

//����ָ���������
void DispInputResetPwd(void);

//����ά������
void DispInputTestPwd(void);

//ά��������֤
void DispJudgeTestPwd(void);

//�����ʼ�����
void DispInputQcPwd(void);

//�ʼ�������֤
void DispJudgeQcPwd(void);

//����˵�����
void DispInputMenuPwd(void);

//�����������
void DispInputPwd(char* pwdbuf,fun funname);

//��ʼ����������buf
void InputInit(void);

//��������
void DispSetPrice(void);

//�Żݵ�������1
void DispSetOthPrice1(void);


//�˵�����1��ʾ
void DispMenu_1(void);

//ͨ����Ϣ����
void DispGeneralInfo(void);
//ͨѶ��������
void DispCommPara(void);

//���ò�������
void DispCommonOpt(void);

//�˵�����2��ʾ
void DispMenu_2(void);

//�汾�������1
void DispVerNum1(void);

//�汾�������2
void DispVerNum2(void);

//������Ϣ����1
void DispOthMsg1(void);

//������Ϣ����2
void DispOthMsg2(void);


//�˵�����3��ʾ
void DispMenu_3(void);

//ϵͳ��������
void DispSysSet1(void);

//ϵͳ��������2
void DispSysSet2(void);

//������Ϣ����1
void DispSetShiftInfo1(void);

//������Ϣ����2
void DispSetShiftInfo2(void);

//������Ϣ����3
void DispSetShiftInfo3(void);

void DispSetPressure(void);


//IC����Ϣ����1
void DispSetICInfo1(void);

//IC����Ϣ����2
void DispSetICInfo2(void);

//��Ϣ��ʾ��ms_100:Ϊ��ʾʱ�䣬100msΪ��λ��funname:ʱ�䵽�˹�����Ҫִ�еĺ���
void TaskMessage(uint32_t ms_100,fun funname);


//ȷ�ϴ���
void DispOK(void);

//���ò���ʱ��ʾ numpoint:С�������λ��,0,2,4��Ч��������4ʱΪѡ������
void DispSet(uint8_t numpoint,uint32_t data,char*buf);

//����������
void DispInputOldPwd(void);

//�˵���������
void DispSetMenuPwd(void);
//�ʼ���������
void DispSetQcPwd(void);

//ά����������
void DispSetTestPwd(void);
//���°���������
void DispSetonoffworkpwd(void);

//�ָ�������������
void DispSetResetPwd(void);

//�ָ���������������֤
void DispJudgeResetPwd(void);

//�ָ���������
void DispFactoryReset(void);

//�ʼ��������
void DispQcPara(void);

void SimulatorSet(void);

//��ʾ���洦��
extern fun DispUI;

//�ϰࡣ
void OnWork(void);
//�°ࡣ
void OffWork(void);

void Diswork(void);

void WriteICcard(void);

uint16_t memcmpp(uint8_t *tr1,uint8_t *tr2);
//���óɹ���ʾ
void SetSuccess(void);
//����ʧ����ʾ
void SetFault(void);

//��Կ��Ϣ�汾������
void DispSetLockVer(void);

//���ò�������1��
void DispCommonOpt1(void);

//��Ļѡ������
void DispSetPing(void);
//�������°�
void OnWorkWithPwd(void);
void OffWorkWithPwd(void);
void DispJudgeOnWorkPwd(void);
void DispJudgeOffWorkPwd(void);
//�ѻ�ģʽ
void DispSetOffline(void);

void SystemReStart(void);
//Flash�洢оƬ����
void DispEraseChip(void);

#endif

