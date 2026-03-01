/************************************************************/
/*		         	FAT 			 					    */
/*				  											*/
/************************************************************/
#include <stm8s105x6.h>
#include "fat.h"
#include "mmcspi.h"
#include "define.h"

FAT_INFO fat_info;
FileDesc_t file_desc;
MMC_BUF MMC_Buf;
unsigned long MAX_CLUSTER;

static char CheckSignature (unsigned long sector);
static void FAT_searchMP3FilesDir (unsigned long cluster, unsigned char level, unsigned int *cnt);
static FileDesc_t *FAT_searchMP3FileDir (unsigned long cluster, unsigned char level, unsigned int *files);
static FileDesc_t *getMP3Info (void);

static unsigned long endian32(char *np)
{
	char c;
	c = *np; *np = np[3]; np[3] = c;
	c = np[1]; np[1] = np[2]; np[2] = c;
	return *((long*)np);
}

static unsigned int endian16(unsigned int n)
{
	unsigned int num = n >> 8;
	return num | (n << 8);
}

/*-----------------------------------------------------------------------
 查询数据区一个簇开始扇区号
-----------------------------------------------------------------------*/
unsigned long fatClusterToSector(unsigned long clust)
{
	unsigned long sector = fat_info.firstDataSector + (clust-2) * fat_info.sectorsPerCluster;
	return sector;
}

/*-----------------------------------------------------------------------
-----------------------------------------------------------------------*/
unsigned long fatNextCluster (unsigned long cluster)
{
    unsigned long fatOffset = cluster << 1;

	if ( fat_info.fatType != FAT16 )	// FAT32: 一个表项为4bytes(32 bits)
		fatOffset <<= 1;

    unsigned long sector = fat_info.firstFATSector + (fatOffset >> 9);  //计算FAT扇区号

	MMC_ReadSegOffset(sector, fatOffset & 0x1ff);

	if ( fat_info.fatType == FAT16 )
        return endian16(*((unsigned int *)BUFFER));
	else
	    return endian32((unsigned long *)BUFFER);
}

//////////////////////////////////////////////////////////////
//文件系统初始化
unsigned char fatInit(void)
{
    if ( MMC_Init() != MMC_OK || !CheckSignature(0) )
    	return 0;

	fat_info.fatType    = FAT16;
	unsigned long bootSector = 0;
	PARTITION_REC *PartInfo = (PARTITION_REC *)BUFFER;

	// read a partition[0]
	if ( MMC_ReadSegOffset(0, 446) != MMC_OK )
		return 0;

	if ( (PartInfo->isActive & 0x7f) == 0 )
	{
		bootSector = endian32(&PartInfo->startLBA);

		switch ( PartInfo->partitionType )
		{
			case PART_TYPE_DOSFAT16:
			case PART_TYPE_FAT16:
			case PART_TYPE_FAT16LBA:
				MAX_CLUSTER = 0x0000fff8;
				break;

			case PART_TYPE_FAT32LBA:
  			case PART_TYPE_FAT32:
				MAX_CLUSTER = 0x0ffffff8;
				fat_info.fatType = FAT32;
				break;

			default:
				return 0;
		}
	}

	if ( !CheckSignature(bootSector) )
		return 0;

	// 引导扇区号在PartInfo.prStartLBA中
	if ( MMC_ReadSegOffsetLength(bootSector, 11, SEGMENT_SIZE+6) != MMC_OK )
		return 0;

	BOOT_SECTOR *bp = (BOOT_SECTOR *)BUFFER;

	if ( endian16(bp->bytesPerSector) != SECTOR_SIZE || bp->numberOfFATs != 2 )	// sector size must be 512 bytes
	{
		return 0;
	}

	fat_info.sectorsPerCluster = bp->sectorsPerCluster;
	fat_info.firstFATSector    = bootSector + endian16(bp->reservedSectors);

	switch ( fat_info.fatType )
	{
		case FAT16:
			fat_info.FAT.fat16.rootDirEntries= endian16(bp->rootDirEntries);
			fat_info.FAT.fat16.rootDirSector = fat_info.firstFATSector + endian16(bp->sectorsPerFAT16) * 2;
			fat_info.firstDataSector         = fat_info.FAT.fat16.rootDirSector + (fat_info.FAT.fat16.rootDirEntries >> 4);
			break;

		case FAT32:
			fat_info.FAT.fat32.rootDirCluster= endian32(&bp->rootClusterFAT32);
			fat_info.firstDataSector         = fat_info.firstFATSector + endian32(&bp->sectorsPerFAT32) * 2;
	}

	return 1;
}

enum {MP3_FILE, SUB_DIR, NULL_ENTRY, UNKNOWN_ENTRY};	// directory types

///////////////////////////////////////////////////////////////////////////////
char FAT_EntryType (void)
{
	unsigned char c = MMC_Buf.dir.name[0];

    if ( c == 0xe5 || c == 0x00 || c == '.' )
		return NULL_ENTRY;

	unsigned char attr = MMC_Buf.dir.attrib & 0x3f;

	if ( MEMCMP_ROM(MMC_Buf.dir.ext, "MP3", 3) && (attr == 0x01 || attr == 0x20) )
		return MP3_FILE;

	if ( attr & 0x10 )
		return SUB_DIR;

	return UNKNOWN_ENTRY;
}
#if 1
//////////////////////////////////////////////////////////////////////////////
unsigned int FILE_searchMP3Files (void)
{
	unsigned int cnt = 0;
	if ( fat_info.fatType == FAT16 )
	{
		unsigned long sector  = fat_info.FAT.fat16.rootDirSector;
		unsigned char segment = 0;
		unsigned int  dir_cnt = fat_info.FAT.fat16.rootDirEntries;

		while ( dir_cnt-- )
		{
			if ( MMC_ReadSegment (sector, segment) != MMC_OK )
				return cnt;

			segment++;

			switch ( FAT_EntryType() )
			{
				case MP3_FILE:
					cnt++;
					break;

				case SUB_DIR:
					MMC_flushSector (segment);
					FAT_searchMP3FilesDir (endian16(MMC_Buf.dir.startCluster), 0, &cnt);
					MMC_SkipSegment (sector, segment);
					break;
			}

			if ( segment >= (SECTOR_SIZE/SEGMENT_SIZE) )
			{
				sector++;
				segment = 0;
			}
		}
	}
	else
		FAT_searchMP3FilesDir (fat_info.FAT.fat32.rootDirCluster, 1, &cnt);

	return cnt;
}

//////////////////////////////////////////////////////////////////////////////
static void FAT_searchMP3FilesDir (unsigned long cluster, unsigned char level, unsigned int *cnt)
{
	unsigned long startSector;
	unsigned char sector  = 0;
	unsigned char segment = 0;

    while ( cluster >= 2 && cluster < MAX_CLUSTER )
	{
		if ( sector == 0 && segment == 0 )
			startSector = fatClusterToSector (cluster);

		if ( MMC_ReadSegment (startSector + sector, segment++) != MMC_OK )
			return;

        switch ( FAT_EntryType () )
        {
            case MP3_FILE:	// it's an MP3 file
               	(*cnt)++;
                break;

            case SUB_DIR:	// it's a sub-directory
                if ( level ) //&& fat_info.fatType != FAT16 )	// only recursive scan for FAT32
                {
					MMC_flushSector (segment);

                    unsigned long clust = endian16(MMC_Buf.dir.startCluster);
                    clust |= endian16(MMC_Buf.dir.highCluster) << 16;

                    FAT_searchMP3FilesDir (clust, 0, cnt);

                    MMC_SkipSegment (startSector + sector, segment);
            	}
		}

		if ( segment >= (SECTOR_SIZE/SEGMENT_SIZE) )
		{
			segment = 0;
			sector++;

	    	if ( sector >= fat_info.sectorsPerCluster )	// end of the cluster, go to next cluster
			{
			    cluster = fatNextCluster (cluster);
				sector = 0;
			}
		}
    }
}

//////////////////////////////////////////////////////////////////////////////
FileDesc_t *FILE_searchMP3File (unsigned int files)
{
	unsigned char level;
	FileDesc_t *fh;

	if ( files == 0 )
		return NULL;

	for (level = 0; level != 2; level++)
	{
		if ( fat_info.fatType == FAT16 )
		{
			unsigned long sector  = fat_info.FAT.fat16.rootDirSector;
			unsigned char segment = 0;
			unsigned int  dir_cnt = fat_info.FAT.fat16.rootDirEntries;

			while ( dir_cnt-- )
			{
				MMC_ReadSegment (sector, segment++);

				switch ( FAT_EntryType() )
				{
					case MP3_FILE:
						if ( !level )
						{
							if ( --files == 0 )
							{
								MMC_flushSector (segment);
								return getMP3Info ();
							}
						}
						break;

					case SUB_DIR:
						if ( level )
						{
							MMC_flushSector (segment);

							fh = FAT_searchMP3FileDir (endian16(MMC_Buf.dir.startCluster), 0, &files);

							if ( fh )
								return fh;

							MMC_SkipSegment (sector, segment);
						}
						break;
				}

				if ( segment >= (SECTOR_SIZE/SEGMENT_SIZE) )
				{
					sector++;
					segment = 0;
				}
			}
		}
		else
		{
			fh = FAT_searchMP3FileDir (fat_info.FAT.fat32.rootDirCluster, level, &files);

			if ( fh )
				return fh;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
static FileDesc_t *FAT_searchMP3FileDir (unsigned long cluster, unsigned char level, unsigned int *files)
{
	unsigned long startSector = startSector;
	unsigned char sector  = 0;
	unsigned char segment = 0;

    while ( cluster >= 2 && cluster < MAX_CLUSTER )
	{
		if ( sector == 0 && segment == 0 )
			startSector = fatClusterToSector (cluster);

		if ( MMC_ReadSegment (startSector + sector, segment++) != MMC_OK )
			return NULL;

        switch ( FAT_EntryType () )
		{
			case MP3_FILE:	// it's an MP3 file
				if ( !level )
				{
					if ( --(*files) == 0 )
					{
						MMC_flushSector (segment+1);
						return getMP3Info ();
					}
				}
				break;

			case SUB_DIR:
                if ( level )    // only recursive scan for FAT32
                {
					MMC_flushSector (segment);

                    unsigned long clust  = endian16(MMC_Buf.dir.startCluster);
                   	clust |= endian16(MMC_Buf.dir.highCluster) << 16;

                    FileDesc_t *fh = FAT_searchMP3FileDir (clust, 0, files);
                    if ( fh )
                    	return fh;

                    MMC_SkipSegment (startSector+sector, segment);
            	}
				break;
		}

		if ( segment >= (SECTOR_SIZE/SEGMENT_SIZE) )
		{
			segment = 0;

	    	if ( ++sector >= fat_info.sectorsPerCluster )
			{
				cluster = fatNextCluster (cluster);
				sector  = 0;
			}
		}
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////
static FileDesc_t *getMP3Info (void)
{
//	MEMCPY (file_desc.title, MMC_Buf.dir.name, 11);
	file_desc.size    = endian32(&MMC_Buf.dir.fileSize);
	file_desc.cluster = endian16(MMC_Buf.dir.startCluster);

	if ( fat_info.fatType != FAT16 )
        file_desc.cluster |= endian16(MMC_Buf.dir.highCluster) << 16;

	file_desc.sector  = 0;
	file_desc.segment = 0;
	return &file_desc;
}

///////////////////////////////////////////////////////////////////////////
static char CheckSignature (unsigned long sector)
{
	if ( MMC_ReadSegOffset(sector, 510) == MMC_OK )
	{
		if ( BUFFER[0] == BOOTSIG0 && BUFFER[1] == BOOTSIG1 )
			return 1;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
char FILE_nextFileSector (FileDesc_t *file)
{
    if ( file->sector >= fat_info.sectorsPerCluster )
	{
        file->cluster = fatNextCluster (file->cluster);
        file->sector  = 0;
	}

    if ( file->cluster >= MAX_CLUSTER )
		return 0;

	if ( file->sector == 0 )
		file->startSector = fatClusterToSector (file->cluster);

  	if ( MMC_NextSector (file->startSector + file->sector++) != MMC_OK )
        return 0;

	return -1;
}
#endif

