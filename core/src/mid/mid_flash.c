#include <stdio.h>
#include "mb91520.h"
#include "comm_typedef.h"
#include "mid_flash.h"
#include "mid_string.h"

/* 写入超时 */
#define WORKFLASH_WRITE_TIME_OUT	2*HZ	
static U32 mStartTime;

typedef enum{
	FL_STATE_IDLE=0,
	FL_STATE_ERASING,
	FL_STATE_ERASE_DONE,
	FL_STATE_PROGRAMMING,
}STATE_FL;

#define WF_OP_ERASE	1
#define WF_OP_WRITE 2


/* 待处理队列 */
#define WF_FIFO_LEN	32
typedef struct{
	U8 op;	/* 擦除或者写操作 */
	unsigned long offset;
	char buf[WF_BUF_SZ];
	int len;
}WF_FIFO;
static U8 mFifoHead=0, mFifoTail=0;
static WF_FIFO mWfFifo[WF_FIFO_LEN];

static U8 mFlState = FL_STATE_IDLE;
static unsigned long mPgAddr;
static int mOffset, mLen;
static U8 mOp;

static unsigned char wf_buf[WF_PAGE_SZ];	/* 用来保存flash原内容的缓冲 */ 

static void start_erase(unsigned long pg_addr)
{
	mStartTime = hal_timer_get_tick();

	//dbg_string("start erase: 0x%x\n", pg_addr);	
	hal_data_flash_sect_erase(pg_addr);
	
	mFlState = FL_STATE_ERASING;
}

/*******************************************
 
 指定页面的区域是否为脏
 返回：
 	0: 干净区域，可以直接写
 	1: 脏区域，不能直接写，要先擦除
 	
 *******************************************/	
static U8 is_area_dirty(int offset, int len)
{
	unsigned char *p;
	int i;
	
	p = (unsigned char*)(WORK_FLASH_START_ADDRESS + offset);
	for (i = 0; i < len; i++)
	{
		if (*p != 0xff) return 1;
		p++;
	}
	return 0;
}
/************************************

 处理缓冲区中的请求
 
 ************************************/
static U8 operate_fifo(void)
{
	unsigned long pg_addr;
	int offset, len;
	char *buf;
	U8 pg_dirty;
	
	/* 队列空 */
	if (mFifoTail == mFifoHead) return 0;
	
	offset = mWfFifo[mFifoTail].offset;
	len = mWfFifo[mFifoTail].len;
	buf = mWfFifo[mFifoTail].buf;
	mOp = mWfFifo[mFifoTail].op;
	
	mFifoTail ++;
	mFifoTail %= WF_FIFO_LEN;
			
	pg_addr = WORK_FLASH_START_ADDRESS + offset;	
	pg_addr = (pg_addr >> 13) << 13;	/* 计算页起始地址 */
	mPgAddr = pg_addr;
		
	switch(mOp)
	{
		case WF_OP_ERASE:
			start_erase(pg_addr); /* 开始异步擦除 */
		break;
		case WF_OP_WRITE:
			if (!buf)
			{
				dbg_string("Null ptr, %d,%d\n", mFifoHead, mFifoTail);
				return 1;
			}
	
			pg_dirty = is_area_dirty(offset, len);
			offset &= 0x1FFF;
			if (pg_dirty)
			{
				/* 拷贝原内容 */
				data_copy((void*)wf_buf, (void*)pg_addr, sizeof(wf_buf));
			}
			data_copy((U8*)(wf_buf + offset), (U8*)buf, len);
	

			if (pg_dirty)
			{
				start_erase(pg_addr); /* 开始异步擦除，需要整块写入 */
				mOffset = 0;
				//mLen = sizeof(wf_buf);
				mLen = offset + len;	/* 只写0到当前块之间的内容，后面的内容保留0xff */
			}
			else
			{
				mFlState = FL_STATE_ERASE_DONE; /* 跳过擦除步骤, 可以部分写入 */
				mOffset = offset;
				mLen = len;	
				//dbg_string("Program: %x,%x\n", mOffset, len);	
			}
		
		break;
	} /* end of switch */
	return 0;
}

void task_work_flash(void)
{
	int erase_status;
	U32 tick;
	U8 ret;
	
	switch(mFlState)
	{
		case FL_STATE_IDLE:
			operate_fifo();
		break;
		case FL_STATE_ERASING:
			/* 超时 */
			tick = hal_timer_get_tick();
			if (time_after(tick, mStartTime+WORKFLASH_WRITE_TIME_OUT))
			{
				hal_data_flash_stop_programming();
				dbg_string("Flash Erasing time out\n");
				mFlState = FL_STATE_IDLE;
				break;
			}
			
			erase_status = hal_check_erasing_status(mPgAddr);
			if (erase_status == 0)
				mFlState = FL_STATE_ERASE_DONE;
			else if (erase_status == -2)	/* 擦除错误 */
			{
				dbg_string("Work flash sector %d erase failed\n", mPgAddr);
				mFlState = FL_STATE_IDLE;
			}
				 
		break;
		case FL_STATE_ERASE_DONE:
			if (mOp == WF_OP_WRITE)
			{
				//dbg_string("start programming (%x+%x):%d, ", mPgAddr, mOffset, hal_timer_get_tick());
#ifdef __DEBUG__
				//dbg_string("start programming (%x+%x)\n", mPgAddr, mOffset);
#endif
				ret = hal_data_flash_sector_programing(mPgAddr, (unsigned short*)wf_buf, mOffset, mLen/2);
				if (ret)
					dbg_string("Flash programming failed\n");
				//dbg_string("done %d:%d\n", ret, hal_timer_get_tick());
			}
			mFlState = FL_STATE_IDLE;
		break;
		case FL_STATE_PROGRAMMING:
		break;
	}
}

/*****************************
 
 FLASH是否可写
 返回：
 	1：目前可以写入
 	0：目前不能写入
 *****************************/
U8 mid_flash_ready(void)
{
	if (mFlState == FL_STATE_IDLE)
		return 1;
	else
		return 0;
}

/******************************

 将写入请求压入队列
 返回：
 	0: 成功
 	1: 超出work flash范围
 	2: 队列满
 	3: 太多数据
 	4: 缓冲区指针为空
 
 ******************************/
static U8 push_request(U8 op, unsigned long offset, unsigned char *buf, unsigned short len)
{
	/* 超出Work Flash范围 */
	if (offset+len > WORK_FLASH_END_ADDRESS)
		return 1;
			
	/* 队列满 */
	if ((mFifoHead == mFifoTail-1) || (mFifoHead-mFifoTail == WF_FIFO_LEN -1))
	{
		dbg_string("Fifo full %d,%d\n", mFifoHead, mFifoTail);
		return 2;
	}
	
	mWfFifo[mFifoHead].op = op;
	switch(op)
	{
		case WF_OP_ERASE:
			mWfFifo[mFifoHead].offset = offset;
			mWfFifo[mFifoHead].len = 0;
		break;
		case WF_OP_WRITE:
			/* 太多数据 */
			if (len > WF_BUF_SZ) return 3;
			if (!buf) return 4;
	
			data_copy((U8*)mWfFifo[mFifoHead].buf, (U8*)buf, len);
			mWfFifo[mFifoHead].offset = offset;
			mWfFifo[mFifoHead].len = len;
		break;
	}
	mFifoHead ++;
	mFifoHead %= WF_FIFO_LEN;
	
	return 0;
}

/******************************
 
 异步擦除work flash的一个page
 
 参数：
 	offset：要写入的地址在Work Flash中的偏移量
 	
 返回: 同push_request()
 
*******************************/
U8 mid_erase_flash_async(unsigned long offset)
{
	return (push_request(WF_OP_ERASE, offset, 0, 0));
}
/******************************
 
 异步写work flash
 参数：
 	offset：要写入的地址在Work Flash中的偏移量
 	buf：待写入内容的缓冲区
 	len：待写入内容的长度
 	
 返回: 同push_request()
  
*******************************/
U8 mid_write_flash_async(unsigned long offset, unsigned char *buf, unsigned short len)
{
	U8 ret;
	
	ret = push_request(WF_OP_WRITE, offset, buf, len);
			
	return ret;
}
/*****************************************
 
 将缓冲区中的内容一次性写入
 注意：可能会耗时很久，仅在主动关机前调用
 
*****************************************/
U8 mid_flash_sync(void)
{
	operate_fifo();
	do
	{
		task_work_flash();
	}while (mFifoHead != mFifoTail);
		
	return 0;
}

