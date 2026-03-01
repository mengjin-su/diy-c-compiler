#ifndef _FAT_H
#define _FAT_H
#include "mmcspi.h"

#define PART_TYPE_UNKNOWN		0x00
#define PART_TYPE_FAT12			0x01
#define PART_TYPE_XENIX			0x02
#define PART_TYPE_DOSFAT16		0x04
#define PART_TYPE_EXTDOS		0x05
#define PART_TYPE_FAT16			0x06
#define PART_TYPE_NTFS			0x07
#define PART_TYPE_FAT32			0x0B
#define PART_TYPE_FAT32LBA		0x0C
#define PART_TYPE_FAT16LBA		0x0E
#define PART_TYPE_EXTDOSLBA		0x0F
#define PART_TYPE_ONTRACK		0x33
#define PART_TYPE_NOVELL		0x40
#define PART_TYPE_PCIX			0x4B
#define PART_TYPE_PHOENIXSAVE	0xA0
#define PART_TYPE_CPM			0xDB
#define PART_TYPE_DBFS			0xE0
#define PART_TYPE_BBT			0xFF

typedef struct //  16 字节
{
	unsigned char	isActive;			// - 0x00	0x80代表该分区为缺省分区
	unsigned char	startHead;			// - 0x01	该分区入口的磁头地址
	unsigned int	startCylSector;		// - 0x02	该分区入口的扇区地址和柱面地址
	unsigned char	partitionType;		// - 0x04	该分区类型
	unsigned char	endHead;			// - 0x05	该分区结束的扇区地址
	unsigned int	endCylSector;		// - 0x06	该分区结束的柱面地址
	unsigned long	startLBA;			// - 0x08	该分区内第一个扇区地址
	unsigned long	size;				// - 0x0c	该分区所包含的扇区总数
} PARTITION_REC;


#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa


typedef struct{
	unsigned char	code[512-64-2];		// 启动代码 (446 unsigned chars)
	PARTITION_REC	partitions[4];		// 四个分区表(64 unsigned chars)
	unsigned char	bootSectSig0;		// 两个签名(2 unsigned chars)
	unsigned char	bootSectSig1;
} PARTITION_SECTOR;


typedef struct {
//	unsigned char	jumpInst[3];		// - 0x00
//	unsigned char	oemName[8];			// - 0x03
	unsigned int	bytesPerSector;		// - 0x0b
	unsigned char	sectorsPerCluster;	// - 0x0d
	unsigned int	reservedSectors;	// - 0x0e 	FAT12/FAT16 = 1; FAT32 = 32
	unsigned char	numberOfFATs;		// - 0x10	always = 2
	unsigned int	rootDirEntries;		// - 0x11	only used for FAT12/FAT16 (0 for FAT32)
	unsigned int	totalSectorsFAT;	// - 0x13	(if zero, use 4 byte value at offset 0x20)
	unsigned char	mediaDescript;		// - 0x15
	unsigned int	sectorsPerFAT16;	// - 0x16	only used for FAT12/FAT16
	unsigned int	sectorsPerTrack;	// - 0x18
	unsigned int	numberOfHeads;		// - 0x1a
	unsigned long	hiddenSectors;		// - 0x1c

	unsigned long	totalSectorsFAT32;	// - 0x20	(if greater than 65535; otherwise, see offset 0x13)
	unsigned long	sectorsPerFAT32;	// - 0x24	only used for FAT32
	unsigned char	_dont_care1[4];		// - 0x28
	unsigned long	rootClusterFAT32;	// - 0x2c

	unsigned char	_dont_care2[470-8];	// - 0x30
	unsigned char	signature0;			// - 0x1fe
	unsigned char	signature1;			// - 0x1ff
} BOOT_SECTOR;

// 目录或文件入口地址结构
typedef struct{
	unsigned char	name[8];          	// - 0x00 	文件名
	unsigned char	ext[3];     		// - 0x08 	扩展名
	unsigned char	attrib;		       	// - 0x0b	文件属性
	unsigned char   lowerCase;       	// - 0x0c 	系统保留
	unsigned char   hundredth;       	// - 0x0d	文件创建时间的10MS
	unsigned char   time[2];         	// - 0x0e	文件创建时间
	unsigned char   date[2];         	// - 0x10	文件创建日期
	unsigned char   aDate[2];         	// - 0x12	文件最近访问日期
	unsigned int    highCluster;     	// - 0x14	文件起始簇号的高16位
	unsigned char   mtime[2];         	// - 0x16	文件最近修改时间
	unsigned char   mDate[2];         	// - 0x18	文件最近修改日期
	unsigned int    startCluster;     	// - 0x1a	文件起始簇号的低16位
	unsigned long   fileSize;  	    	// - 0x1c	文件长度
} FILE_DIR;


enum {FAT16, FAT32};

typedef struct {

	unsigned char fatType;
	unsigned char sectorsPerCluster;
	unsigned long firstFATSector;
	unsigned long firstDataSector;

	union {
		struct {
			unsigned long rootDirSector;
			unsigned int  rootDirEntries;
		} fat16;
		struct {
			unsigned long rootDirCluster;
		} fat32;
	} FAT;
} FAT_INFO;

typedef struct {
//	unsigned char   title[16];
	unsigned long 	size;		// file size (bytes)
	unsigned long	cluster;  	// starting cluster
	unsigned long   startSector;
	unsigned char	sector;
	unsigned char	segment;
} FileDesc_t;

#define SEGMENT_SIZE	32

typedef union {
	unsigned char buffer[SEGMENT_SIZE+6];
	FILE_DIR dir;
} MMC_BUF;

extern MMC_BUF 		MMC_Buf;
extern FileDesc_t	file_desc;
extern FAT_INFO   	fat_info;

extern unsigned long MAX_CLUSTER;
#define BUFFER 		MMC_Buf.buffer

unsigned char fatInit ();
unsigned int FILE_searchMP3Files (void);
FileDesc_t *FILE_searchMP3File (unsigned int files);
char FILE_nextFileSector (FileDesc_t *file);

typedef struct {
	unsigned int  index, dummy;
	unsigned long cluster;
} log_t;

#endif
