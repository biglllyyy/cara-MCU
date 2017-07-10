#include "mb91520.h"
#include "hal_flash.h"

/***********************************************
 
 检查指定页面的擦除状态，
 
 返回值：
 	 0：完毕
 	-1：擦除进行中
 	-2：擦除错误
  
 ***********************************************/
int hal_check_erasing_status(unsigned long addr)
{
	unsigned short read0, read1;
    
    wdg_feed();	
    read0=*((unsigned char *)addr); //read flash data
    read1=*((unsigned char *)addr); //read flash data
    
    if((read1 & 0x20) == 0) //bit5=0 means flash is processing
    	return -1;
    else
    {
		read0=*((unsigned char *)addr); //read flash data
		read1=*((unsigned char *)addr); //read flash data
		if(read0!=read1)
			return -2;
	}
	
	return 0;
}

/*!
 ******************************************************************************
 ** 开始擦除指定的页面，不等待擦除完毕
 **
 ** \param tgt_add Target address.
 **        This parameter can be one of the following values:
 ** \arg   Physical address of data flash area. e.g. 0x330000
 **
 ** \retval err_flag Error flag.
 **         This parameter can be one of the following values:
 ** \arg    0:no error
 ** \arg    1:error
 ******************************************************************************
 */
unsigned char hal_data_flash_sect_erase(unsigned long tgt_add)
{
    unsigned long tgt_sect_add;
    
    //__DI();
	wdg_feed();		/* H/W WDT Clear */
	FCTLR   = 0x4400;	/* enable flash programming */
	DFCTLR_FWE = 1;		/* enable data flash programming */
    
    /************************************************
    erase SA0, 8K, 33:0000 - 33:1FFF Bank:A Flash:A
    *************************************************/
    /************************************************
    erase SA1, 8K, 33:2000 - 33:3FFF  Bank:B Flash:A
    *************************************************/
    /************************************************
    erase SA2, 8K, 33:4000 - 33:5FFF  Bank:B Flash:A
    *************************************************/
    /************************************************
    erase SA3, 8K, 33:6000 - 33:7FFF  Bank:B Flash:A
    *************************************************/
    /************************************************
    erase SA4, 8K, 33:8000 - 33:9FFF  Bank:B Flash:A
    *************************************************/
    /************************************************
    erase SA5, 8K, 33:A000 - 33:BFFF  Bank:B Flash:A
    *************************************************/
    /************************************************
    erase SA6, 8K, 33:C000 - 33:DFFF  Bank:B Flash:A
    *************************************************/
    /************************************************
    erase SA7, 8K, 33:E000 - 33:FFFF  Bank:B Flash:A
    *************************************************/
    
    tgt_sect_add=tgt_add;//must use even address
    
    wdg_feed();

    *((unsigned short *)((tgt_sect_add&0xFFFFF000)+0x00000AA8))=0x00AA; //1st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFFF000)+0x00000554))=0x0055; //2st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFFF000)+0x00000AA8))=0x0080; //3st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFFF000)+0x00000AA8))=0x00AA; //4st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFFF000)+0x00000554))=0x0055; //5st cycle
    *((unsigned short *)tgt_sect_add)=0x0030; //6st cycle
            
    return 0;
}

/*!
 ******************************************************************************
 ** \brief Data Flash programming algorithm.
 **
 ** \param tgt_add Target address.
 **        This parameter can be one of the following values:
 ** \arg   Physical address e.g. 0xFFFE00

 ** \param src_data Source data. Data type is 'int' long.
 **        This parameter can be one of the following values:
 ** \arg   Source data for flash programming algorithm e.g. &data[0]

 ** \param cnt Count of data. Count of that how many data should be programed to target area.
 **        This parameter can be one of the following values:
 ** \arg   Count of source data e.g. 256
 **
 ** \retval err_flag Error flag.
 **         This parameter can be one of the following values:
 ** \arg    0:no error
 ** \arg    1:error
 **
 ******************************************************************************
 */
unsigned char hal_data_flash_sector_programing(long addr, unsigned short* src_data, int offset, unsigned long cnt)
//function name can be used as a address point
//programing_adds must be even value if using word accessing(2 byte long)
{
    unsigned short read0;
    unsigned char err_flag=0;
    unsigned long times;
    unsigned short *ptr;
    long tgt_add;
    
    hal_data_flash_start_programming();
    tgt_add = addr + offset;
    ptr = src_data + offset/2;
    
    for(times = 0; times < cnt; times++)
    {
		wdg_feed();
		/* Erase之后，如果写入0xFF，下次在未erase状态下，该单元无法再次写入*/
		if ((ptr[times] == 0xffff) && (*((unsigned short *)(tgt_add+times*2)) == 0xffff))
		{
			;
		}
		//else
		{
			*((short *)(((tgt_add+times*2)&0x00FFF000)+0x00000AA8))=0x00AA; //1st cycle
			*((short *)(((tgt_add+times*2)&0x00FFF000)+0x00000554))=0x0055; //2st cycle
			*((short *)(((tgt_add+times*2)&0x00FFF000)+0x00000AA8))=0x00A0; //3st cycle
			*((short *)(tgt_add+times*2))=ptr[times]; //4st cycle
		}
		read0=*((short *)(tgt_add+times*2));
        
        while(read0 != ptr[times])
        {
        	wdg_feed();
            if((read0 & 0x20) == 0) /* TLOV, bit5=0 means flash is processing */
            {
                read0=*((short *)(tgt_add+times*2)); //read flash data
            }
            else
            {
                read0=*((short *)(tgt_add+times*2)); //read flash data
                
                if(read0 != ptr[times])
                {
                    err_flag=1;
                    break;
                 }
                else
                {
                }
            }
        }
    }
    
    hal_data_flash_stop_programming();
    return err_flag;
}

void hal_data_flash_start_programming(void)
{
	//FCTLR   = 0x4400;	/* enable flash programming */
    DFCTLR_FWE = 1;	/* Work flash write enabled */
}
void hal_data_flash_stop_programming(void)
{
    DFCTLR_FWE = 0;	/* Work flash write disabled */
}

