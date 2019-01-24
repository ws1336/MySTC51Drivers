#ifndef __SPI_SDCARD_H
#define __SPI_SDCARD_H
#include "define.h"
#include "delay.h"

#define USE_PETIT_FATFS 1
#define USE_WRITE_DISK 1
sbit SD_MISO = P2^0;
sbit SD_MOSI = P2^1;
sbit SD_SCLK = P2^2;
sbit SD_CS   = P2^3;

// SD卡类型  
#define SD_TYPE_ERR     0X00  
#define SD_TYPE_MMC     0X01  
#define SD_TYPE_V1      0X02  
#define SD_TYPE_V2      0X04  
#define SD_TYPE_V2HC    0X06      
  
// SD卡命令    
#define CMD0    0       //卡复位
#define CMD1    1  
#define CMD8    8       //SEND_IF_COND  
#define CMD9    9       //读CSD
#define CMD10   10      //读CID
#define CMD12   12      //
#define CMD16   16      //设置SectorSize 应返回0x00  
#define CMD17   17      //读sector  
#define CMD18   18      //读Multi sector  
#define CMD23   23      //设置多sector写入前预先擦除N个block  
#define CMD24   24      //写sector  
#define CMD25   25      //写Multi sector  
#define CMD41   41      //应返回0x00  
#define CMD55   55      //应返回0x01  
#define CMD58   58      //读OCR
#define CMD59   59      //使能/禁止CRC,应返回0x00  
  
  
#define MSD_RESPONSE_NO_ERROR      0x00    //无错误
#define MSD_IN_IDLE_STATE          0x01    //空闲状态
#define MSD_ERASE_RESET            0x02    //擦除错误
#define MSD_ILLEGAL_COMMAND        0x04    //命令错误
#define MSD_COM_CRC_ERROR          0x08    //CRC通信错误
#define MSD_ERASE_SEQUENCE_ERROR   0x10    //擦除次序错误
#define MSD_ADDRESS_ERROR          0x20    //地址错误
#define MSD_PARAMETER_ERROR        0x40    //参数错误
#define MSD_RESPONSE_FAILURE       0xFF    //这个名根本是失败的，没有任何响应

extern u8  SD_Type;

u8 SD_Initialize(void);

#if USE_PETIT_FATFS
    u8 SD_ReadDisk(u8*buf,u32 sector,u16 offset, u16 count);
    #if USE_WRITE_DISK
        u8 SD_WriteDisk(u8*buf,u32 sector);
    #endif
#else
    u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);
    #if USE_WRITE_DISK
        u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt);
    #endif
#endif

#endif
