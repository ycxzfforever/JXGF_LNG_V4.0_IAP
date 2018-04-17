#ifndef __SST25VF064C_H 
#define __SST25VF064C_H                                                                /* __cplusplus                 */

/*********************************************************************************************************
  �궨��
*********************************************************************************************************/
#define     SPI_FLASH_A_ENABLE()         LPC_GPIO4->CLR |= 1<<9
#define     SPI_FLASH_A_DISABLE()        LPC_GPIO4->SET |= 1<<9

#define     SPI_FLASH_B_ENABLE()         LPC_GPIO2->CLR |= 1<<9
#define     SPI_FLASH_B_DISABLE()        LPC_GPIO2->SET |= 1<<9

/*  
** SPI Flash����(���SST25VF064C)  
 */
#define   SPI_FLASH_WREN          0x06        /* write enable                 */
#define   SPI_FLASH_WRDI          0x04        /* write disable                */
#define   SPI_FLASH_JEDEC         0x9F        /* JEDEC ID����                 */
#define   SPI_FLASH_RDSR          0x05        /* ��ȡ״̬�Ĵ�����              */
#define   SPI_FLASH_WRSR          0x01        /* write status register        */
#define   SPI_FLASH_READ          0x03        /* read data                    */
#define   SPI_FLASH_FAST_READ     0x0B        /* fast read data               */
#define   SPI_FLASH_SE            0x20        /* ����4k�Ĵ洢�����С�          */
#define   SPI_FLASH_BE            0xD8        /* ����64k�Ĵ洢�����С�         */
#define   SPI_FLASH_CE            0x60        /* ������Ƭ�洢�����С�          */
#define   SPI_FLASH_PP            0x02        /* ҳ��̡�                     */
#define   SPI_FLASH_RDID          0x90        /* read electronic ID           */
#define   SPI_FLASH_EWSR          0x50        /* ʹ��д״̬�Ĵ�����            */
                                                                  

#define   MAX_ADDR                0x7FFFFF    /* оƬ�ڲ�����ַ             */
#define   SEC_SIZE                0x1000      /* ������С 4096                  */
#define   PAGE_SIZE               256         /* ҳ��С                     */
#define   MAX_BLOCK_NUM           127         /* �����                     */
#define   MAX_SECTOR_NUM	      2047        /* ���������                   */ 


/****************��ˮ��¼******************/
#define  FlashRecSADDR		      0x0         //��ˮ��¼��ʼ��ַ
#define  FlashRecLen		      256         //ÿ����ˮ��¼��ռ���ֽ���
#define  FlashRecMaxID		      20000       //���20000����ˮ��¼

/****************���ۼ�¼******************/
#define  FlashShiftSADDR	      0x4E2000    //���ۼ�¼��ʼ��ַ
#define  FlashShiftLen		      128         //ÿ�����ۼ�¼��ռ���ֽ���
#define  FlashShiftMaxID		  1000        //���1000�����ۼ�¼(ע:������Ϊ1024)

/*************�����޸ļ�¼***************/
#define  FlashModSADDR	          0x502000    //�����޸ļ�¼��ʼ��ַ
#define  FlashModLen		      128         //ÿ�������޸ļ�¼��ռ���ֽ���
#define  FlashModMaxID		      1000        //���1000�������޸ļ�¼(ע:������Ϊ1024)

/*****************������********************/
#define  FlashBlkSADDR	          0x522000    //��������¼��ʼ��ַ
#define  FlashBlkLen		      8           //ÿ����������¼��ռ���ֽ���
#define  FlashBlkMaxID		      20000       //���20000����������¼(ע:������Ϊ20480)

/**************����������*****************/
#define  FlashAddBlkSADDR	      0x54A000    //������������¼��ʼ��ַ
#define  FlashAddBlkLen		      8           //ÿ��������������¼��ռ���ֽ���
#define  FlashAddBlkMaxID		  200         //���200��������������¼(ע:������Ϊ512)

/**************��ɾ������*****************/
#define  FlashDelBlkSADDR	      0x54B000    //��ɾ��������¼��ʼ��ַ
#define  FlashDelBlkLen		      8           //ÿ����ɾ��������¼��ռ���ֽ���
#define  FlashDelBlkMaxID		  200         //���200����ɾ��������¼(ע:������Ϊ512)

/*****************������********************/
#define  FlashWhiteSADDR	      0x54C000    //��������¼��ʼ��ַ
#define  FlashWhiteLen		      8           //ÿ����������¼��ռ���ֽ���
#define  FlashWhiteMaxID		  200         //���200����������¼(ע:������Ϊ512)

/*****************��������********************/
#define  FlashTestSADDR	          0x54D000    //flash���Ե�ַ
#define  FlashTestSector          1357        //0x54D000/4096

void    SpiSimpleCmd(uint8_t flashnum,uint8_t cmd);
uint8_t SSTReadID(uint8_t flashnum);
uint8_t SSTReadFlashStatus(uint8_t flashnum);
void    WriteSpiFlashStatus(uint8_t flashnum,uint8_t status);
uint8_t SSTEraseSector(uint8_t flashnum,uint32_t addr);
uint8_t SSTEraseBlock(uint8_t flashnum,uint32_t addr);
void    SSTEraseChip(uint8_t flashnum);
uint8_t SSTReadNbyte(uint8_t flashnum,
                   uint32_t addr, 
                   uint8_t *dst, 
                   uint32_t num, 
                   uint8_t readmode);
uint8_t SSTWriteNbyte( uint8_t  flashnum,
                      uint32_t addr, 
                      uint8_t  *src, 
                      uint32_t num );
uint8_t SSTFlashTest(uint8_t  flashnum);

uint8_t SaveFuelRecord(uint8_t select);
uint8_t ReadFuelRecord(uint32_t id,uint8_t select);
uint8_t SaveFlash(uint8_t  flashnum,uint32_t addr, uint8_t  *src, uint32_t num );
uint8_t ReadFlash(uint8_t flashnum,uint32_t addr,uint8_t *pt,uint32_t num);
                                                             
#endif
/*********************************************************************************************************
**                            End Of File
*********************************************************************************************************/
