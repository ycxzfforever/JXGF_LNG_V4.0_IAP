#ifndef __KEY_H__
#define __KEY_H__

#define KEY_0       0x000FEFFF
#define KEY_1       0x000FFFFE
#define KEY_2       0x000FFFFD
#define KEY_3       0x000FFFFB
#define KEY_4       0x000FFFEF
#define KEY_5       0x000FFFDF
#define KEY_6       0x000FFFBF
#define KEY_7       0x000EFFFF
#define KEY_8       0x000DFFFF
#define KEY_9       0x000BFFFF

#define KEY_UP      KEY_6
#define KEY_DOWN    KEY_9
    
#define KEY_QRY     0x000FFFF7  //��ѯ
#define KEY_SET     0x000FFF7F  //�˵�
#define KEY_FIX     0x0007FFFF  //����
#define KEY_BACK    0x000FBFFF  //����(CNG)��Ԥ�� (LNG)
#define KEY_CLR     0x000F7FFF  //��� 
#define KEY_START   0x000FFEFF  //����
#define KEY_STOP    0x000FFDFF  //ֹͣ 
#define KEY_OK      0x000FFBFF  //ȷ�� 
#define KEY_RET     0x000FF7FF  //����
#define KEY_PT      0x000FDFFF  //С����   
    
#define KEY_COMB_SSET   0x000FFD7F /*STOP+SET����*/
#define KEY_COMB_SQRY   0x000FFDF7 /*STOP+QRY��ѯ*/
#define KEY_COMB_SFIX   0x0007FDFF /*STOP+FIX����*/
#define KEY_COMB_SBACK  0x000FBDFF /*STOP+BACK����*/
#define KEY_COMB_SCLR   0x000F7DFF /*STOP+CLR���*/
#define KEY_COMB_SRET   0x000FF5FF /*STOP+RET����*/ 
#define KEY_COMB_7KEY   0x000EFDFF /*STOP+7*/ 
#define KEY_COMB_8KEY   0x000DFDFF /*STOP+8*/ 

typedef struct def_Screeninput//��������
{
   uint8_t  inputlen;       //ʵʱ����仯���ȣ����ȴ�0��ʼ����
   uint8_t  inputmaxlen;    //������볤�ȣ����ȴ�0��ʼ����
   uint8_t  pointnum;       //С�������
   uint8_t  array[16];      //�洢������������   
}Screeninput;


extern  Screeninput ScreenInput;
void KeyPinInit(void);
void KeyGetValue(void);
void NumKeyHandler(void);


#endif
