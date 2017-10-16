/***************************************************************************
 
 最终保存的数据：CRC16(2 bytes), ID(4 bytes), data(DATA_ITEM_LEN - 6 bytes)
 
 ***************************************************************************/
#include "mid_data_store.h"
#include "crc16.h"

#define DATA_OFFSET		0	/* 在Work flash中的起始偏移 */
#define DATA_ITEMS	64	/* 最多保存的记录数 */
#define DATA_ITEM_LEN	WF_BUF_SZ	/* 单条记录占用的字节数 */

#define ID_OFFSET	4	/* 存放ID的偏移量 */
#define ITEMS_IN_ONE_PAGE	(WF_PAGE_SZ/DATA_ITEM_LEN)	/* 一个页面能存放的记录数 */


/* 记录最新一个有效区块的位置 */
static short mPos = -1;
/* 记录最新一个有效区块的id */
static int mID = -1;

/* 单条记录缓冲 */
static U8 mBuf[DATA_ITEM_LEN];

/* AES解密缓冲 */
static char aesBuf[AES_BLOCKSIZE];

char *mid_aes_decrypt(char *src)
{
	memcpy(aesBuf, src, sizeof(aesBuf));
	aes_decrypt((U8*)aesBuf);
	return aesBuf;
}
/**********************************
 
 获取最后一个有效项目的id
 返回
 	>=0: 位置
 	-1: 无任何有效数据
 **********************************/
static short get_last_valid_id(void)
{
	short pos, i;
	int *ptr;
	unsigned short crc16, *cp;
		
	pos = -1;
	for (i = 0; i < DATA_ITEMS; i++)
	{
		cp = (unsigned short*)(WORK_FLASH_START_ADDRESS + i * DATA_ITEM_LEN);
		ptr = (int*)(WORK_FLASH_START_ADDRESS + i * DATA_ITEM_LEN + ID_OFFSET);
		if ((unsigned int)(*ptr) != 0xFFFFFFFF)
		{
			crc16 = api_cal_crc16((U8*)ptr, sizeof(mBuf)-ID_OFFSET);
			if (crc16 != *cp)
			{
				dbg_string("CRC error, ID=%d, pos=%d\n", *ptr, i);
				continue;
			}
			if (*ptr > mID)
			{
				pos = i;
				mID = *ptr;
			}
		}
	}
	
	return pos;
}


/********************************************
 
 保存指定数据到work flash，封装自增长ID等特性
 
 返回:
 	0: 成功
 	1: 数据无效

*********************************************/
U8 mid_save_data(U8 *buf, short len)
{
	unsigned short crc16;
	unsigned short *p;
	int *ptr;
	
	if ((!buf) || (!len)) return 1;
	if (len > WF_BUF_SZ - 8) return 1;
	
	if (mPos == -1)
	{
		mPos = get_last_valid_id();
		/* 没找到任何合法记录 */
		if (mPos == -1)
		{
			mPos = 0;
			mID = 0;
			mid_erase_flash_async(DATA_OFFSET);
		}
		else
		{
			mPos += 1;
			mPos %= DATA_ITEMS;
			mID += 1;
		}
	}
	else
	{
		mPos ++;
		mPos %= DATA_ITEMS;
		mID++;
	}
	
	/* 写入 */
	memset(mBuf, 0x00, sizeof(mBuf));
	ptr = (int*)(mBuf+ID_OFFSET);
	*ptr = mID;
	memcpy(mBuf+8, buf, len);
	crc16 = api_cal_crc16(mBuf+ID_OFFSET, sizeof(mBuf)-ID_OFFSET);	/* 计算CRC16 */
	p = (unsigned short*)mBuf;
	*p = crc16;
	p++;
	*p = 0;	/* 必须填0，否则crc会保存不正确 */
#ifdef __DEBUG__
	//dbg_string("ID:%d(%x, %dB), pos:%d\n", mID, crc16, sizeof(mBuf)-ID_OFFSET, mPos);
#endif
	mid_write_flash_async(mPos * DATA_ITEM_LEN, mBuf, sizeof(mBuf));
	
	
	/* 如果当前page已经占用一半， 擦除下一个page */
	if (mPos == ITEMS_IN_ONE_PAGE/2) 
		mid_erase_flash_async(ITEMS_IN_ONE_PAGE * DATA_ITEM_LEN + DATA_OFFSET);
	else
		if (mPos == ITEMS_IN_ONE_PAGE + ITEMS_IN_ONE_PAGE/2)
			mid_erase_flash_async(DATA_OFFSET);
		
	return 0;
}

/********************************************
 
 从work flash读取最后一个有效数据
 
 返回:
 	0: 成功
 	1: 无有效数据

*********************************************/
U8 mid_load_data(U8 *buf, short len)
{
	char *ptr;
	if ((!buf) || (!len)) return 1;
	
	if (mPos == -1) mPos = get_last_valid_id();
	if (mPos == -1) return 1;
	
	/* 拷贝数据 */
	ptr = (char*)(WORK_FLASH_START_ADDRESS + mPos * DATA_ITEM_LEN + ID_OFFSET + 4);
	memcpy(buf,ptr,len);
	return 0;
}

