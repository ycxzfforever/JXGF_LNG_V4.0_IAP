#ifndef _TURN_H
#define _TURN_H

typedef union       //������������
{
  float data;
  uint8_t rdcv[4];
}UnionFloat;

typedef union       // 16λ������������
{
  uint16_t data;
  uint8_t rdcv[2];
}UnionU16;

typedef union       // 32λ������������
{
	uint32_t  data;
	uint8_t   rdcv[4];
}UnionU32;

typedef union       // 64λ������������
{
	uint64_t  data;
	uint8_t   rdcv[8];
}UnionU64;


uint8_t     CheckDate(uint8_t * str);
uint8_t     CheckTime(uint8_t * str);

uint8_t     AscToBcd(uint8_t* asc);
void        BcdToAsc(uint8_t* dest,uint8_t bcd);
char*       ltostr (char *str,uint32_t val, uint8_t base) ;
uint32_t    HexToBcd(uint32_t sdt);
uint32_t    BcdToHex(uint32_t sdt);
uint32_t    BcdbufToHex(uint8_t* buf,uint8_t len); 
uint8_t     Cmp2float(double vcmp1, double vcmp2);
void        HexToBcdbuf(uint32_t sdt, char* buf, uint8_t len);
void        FloatToBcdbuf(double fdt, char* buf, uint8_t len);
void        FloatToAsciibuf(double fdt, char* buf, uint8_t len);
void        HexToAsciibuf(uint32_t fdt,char *buf,uint8_t len);


#endif
