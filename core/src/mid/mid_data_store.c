/***************************************************************************
 
 ���ձ�������ݣ�CRC16(2 bytes), ID(4 bytes), data(DATA_ITEM_LEN - 6 bytes)
 
 ***************************************************************************/
#include "mid_data_store.h"
#include "crc16.h"

#define DATA_OFFSET		0	/* ��Work flash�е���ʼƫ�� */
#define DATA_ITEMS	64	/* ��ౣ��ļ�¼�� */
#define DATA_ITEM_LEN	WF_BUF_SZ	/* ������¼ռ�õ��ֽ��� */

#define ID_OFFSET	4	/* ���ID��ƫ���� */
#define ITEMS_IN_ONE_PAGE	(WF_PAGE_SZ/DATA_ITEM_LEN)	/* һ��ҳ���ܴ�ŵļ�¼�� */


/* ��¼����һ����Ч�����λ�� */
static short mPos = -1;
/* ��¼����һ����Ч�����id */
static int mID = -1;

/* ������¼���� */
static U8 mBuf[DATA_ITEM_LEN];

/* AES���ܻ��� */
static char aesBuf[AES_BLOCKSIZE];

char *mid_aes_decrypt(char *src)
{
	memcpy(aesBuf, src, sizeof(aesBuf));
	aes_decrypt((U8*)aesBuf);
	return aesBuf;
}
/**********************************
 
 ��ȡ���һ����Ч��Ŀ��id
 ����
 	>=0: λ��
 	-1: ���κ���Ч����
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
 
 ����ָ�����ݵ�work flash����װ������ID������
 
 ����:
 	0: �ɹ�
 	1: ������Ч

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
		/* û�ҵ��κκϷ���¼ */
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
	
	/* д�� */
	memset(mBuf, 0x00, sizeof(mBuf));
	ptr = (int*)(mBuf+ID_OFFSET);
	*ptr = mID;
	memcpy(mBuf+8, buf, len);
	crc16 = api_cal_crc16(mBuf+ID_OFFSET, sizeof(mBuf)-ID_OFFSET);	/* ����CRC16 */
	p = (unsigned short*)mBuf;
	*p = crc16;
	p++;
	*p = 0;	/* ������0������crc�ᱣ�治��ȷ */
#ifdef __DEBUG__
	//dbg_string("ID:%d(%x, %dB), pos:%d\n", mID, crc16, sizeof(mBuf)-ID_OFFSET, mPos);
#endif
	mid_write_flash_async(mPos * DATA_ITEM_LEN, mBuf, sizeof(mBuf));
	
	
	/* �����ǰpage�Ѿ�ռ��һ�룬 ������һ��page */
	if (mPos == ITEMS_IN_ONE_PAGE/2) 
		mid_erase_flash_async(ITEMS_IN_ONE_PAGE * DATA_ITEM_LEN + DATA_OFFSET);
	else
		if (mPos == ITEMS_IN_ONE_PAGE + ITEMS_IN_ONE_PAGE/2)
			mid_erase_flash_async(DATA_OFFSET);
		
	return 0;
}

/********************************************
 
 ��work flash��ȡ���һ����Ч����
 
 ����:
 	0: �ɹ�
 	1: ����Ч����

*********************************************/
U8 mid_load_data(U8 *buf, short len)
{
	char *ptr;
	if ((!buf) || (!len)) return 1;
	
	if (mPos == -1) mPos = get_last_valid_id();
	if (mPos == -1) return 1;
	
	/* �������� */
	ptr = (char*)(WORK_FLASH_START_ADDRESS + mPos * DATA_ITEM_LEN + ID_OFFSET + 4);
	memcpy(buf,ptr,len);
	return 0;
}

