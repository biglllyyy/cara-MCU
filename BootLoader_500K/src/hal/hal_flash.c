#include "Mb91520.h"
#include "Comm_typedef.h"
#include "hal_wdg.h"

static void hal_data_flash_start_programming(void);
static void hal_data_flash_stop_programming(void);

#pragma segment CODE=CODE_CAN_Boot, attr=CODE /* ½«ÒÔÏÂ´úÂë·Åµ½RAMÇø£¬·ÀÖ¹·ÅÔÚROMÇø±»²Á³ý */
unsigned char hal_data_flash_sector_programing(long addr, unsigned short* src_data, int offset, unsigned long cnt)
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

		*((short *)(((tgt_add+times*2)&0x00FFF000)+0x00000AA8))=0x00AA; //1st cycle
		*((short *)(((tgt_add+times*2)&0x00FFF000)+0x00000554))=0x0055; //2st cycle
		*((short *)(((tgt_add+times*2)&0x00FFF000)+0x00000AA8))=0x00A0; //3st cycle
		*((short *)(tgt_add+times*2))=ptr[times]; //4st cycle

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

static void hal_data_flash_start_programming(void)
{
	//FCTLR   = 0x4400;	/* enable flash programming */
    DFCTLR_FWE = 1;	/* Work flash write enabled */
}
static void hal_data_flash_stop_programming(void)
{
    DFCTLR_FWE = 0;	/* Work flash write disabled */
}

unsigned char hal_Flash_Sect_Erasing(unsigned long tgt_add)
{
    unsigned short read0, read1, err_flag=0;
    unsigned long tgt_sect_add;

    __DI();


    IO_WDTCPR1 = 0xa5;  /* H/W WDT Clear */
    FCTLR_FWE = 0;   /* flash write disable */
    FCTLR_FSZ = 1;   /* 16-bit */
    FCTLR_FWE = 1;   /* flash write enable */


    //IO_FCTLR.hword=0x4400;  //enable flash programming
    //IO_DFCTLR.hword=0x4000;  //enable data flash programming

    /****************************** main flash **********************************/
    /* SA2---SA7:8K  */
    /* SA8---SA23:64K */
    /* even sector: last address is:0,1,2,3,8,9,A,B */
    /* odd  sector: last address is:4,5,6,7,C,D,E,F */
    tgt_sect_add= tgt_add & 0xFFFFFFFE;//must use even address
    
    IO_WDTCPR1 = 0xa5;//clear WCDOG

    *((unsigned short *)((tgt_sect_add&0xFFFF0000)+0x00001554))=0x00AA; //1st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFF0000)+0x00000AA8))=0x0055; //2st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFF0000)+0x00001554))=0x0080; //3st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFF0000)+0x00001554))=0x00AA; //4st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFF0000)+0x00000AA8))=0x0055; //5st cycle
    *((unsigned short *)tgt_sect_add)=0x30; //6st cycle
    IO_WDTCPR1 = 0xa5;//clear WCDOG

    read0=*((unsigned char *)tgt_sect_add); //read flash data
    read1=*((unsigned char *)tgt_sect_add); //read flash data

    IO_WDTCPR1 = 0xa5;//clear WCDOG

    while(read0!=read1) //if DQ6 is toggling
    {
        if((read1 & 0x20) == 0) //bit5=0 means flash is processing
        {
            read0=*((unsigned char *)tgt_sect_add); //read flash data
            read1=*((unsigned char *)tgt_sect_add); //read flash data
        }
        else
        {

            read0=*((unsigned char *)tgt_sect_add); //read flash data
            read1=*((unsigned char *)tgt_sect_add); //read flash data
            if(read0!=read1)
            {
                err_flag=1;
                while(1)
                {
					//dbg_string("reset.\n");
                    /*
                    IO_KEYCDR.hword =0x0009;    //PDR09: 0009H
                    IO_KEYCDR.hword =0x4009;    //PDR09: 0009H
                    IO_KEYCDR.hword =0x8009;    //PDR09: 0009H
                    IO_KEYCDR.hword =0xC009;    //PDR09: 0009H
                    IO_PDR09.byte   =0x01;      //P09
                    */
                    IO_WDTCPR1 = 0xa5;//clear WCDOG
                }
            }
            else
            {
            }
        }

        IO_WDTCPR1 = 0xa5;//clear WCDOG
    }
    /********************************/
    __EI();
    return err_flag;
}

unsigned char hal_Work_Flash_Sect_Erasing(U32 tgt_add)
{
    unsigned short read0, read1, err_flag=0;
    U32 tgt_sect_add=tgt_add;//must use even address
    //U32 Delay1,Delay2;
    //unsigned char PinTog;

    __DI();

    IO_WDTCPR1 = 0xa5;    /* H/W WDT Clear */
    IO_FCTLR.hword=0x4400;  //enable flash programming
    IO_DFCTLR.hword=0x4000;  //enable data flash programming

    while(FSTR_FRDY !=1);       // not set success;

    //IO_FLIFCTLR.byte=0x03;
    IO_WDTCPR1 = 0xa5;//clear WCDOG

    *((unsigned short *)((tgt_sect_add&0xFFFFF000)+0x00000AA8))=0x00AA; //1st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFFF000)+0x00000554))=0x0055; //2st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFFF000)+0x00000AA8))=0x0080; //3st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFFF000)+0x00000AA8))=0x00AA; //4st cycle
    *((unsigned short *)((tgt_sect_add&0xFFFFF000)+0x00000554))=0x0055; //5st cycle
    *((unsigned short *)tgt_sect_add)=0x0030; //6st cycle

    read0=*((unsigned char *)tgt_sect_add); //read flash data
    read1=*((unsigned char *)tgt_sect_add); //read flash data

    while(read0!=read1) //if DQ6 is toggling
    {
        if((read1 & 0x20) == 0) //bit5=0 means flash is processing
        {
            read0=*((unsigned char *)tgt_sect_add); //read flash data
            read1=*((unsigned char *)tgt_sect_add); //read flash data
        }
        else
        {
            read0=*((unsigned char *)tgt_sect_add); //read flash data
            read1=*((unsigned char *)tgt_sect_add); //read flash data
            if(read0!=read1)
            {
                err_flag=1;
                while(1)
                {
                    /*
                    IO_KEYCDR.hword =0x0009;    //PDR09: 0009H
                    IO_KEYCDR.hword =0x4009;    //PDR09: 0009H
                    IO_KEYCDR.hword =0x8009;    //PDR09: 0009H
                    IO_KEYCDR.hword =0xC009;    //PDR09: 0009H
                    IO_PDR09.byte   =0x01;      //P09
                    */
                    IO_WDTCPR1 = 0xa5;//clear WCDOG
                }
            }
            else
            {
            }
        }
        IO_WDTCPR1 = 0xa5;//clear WCDOG
    }
    __EI();
    return err_flag;
}

unsigned char hal_Flash_Sector_Programing(long tgt_add, unsigned short* src_data, U32 cnt)
{
    unsigned short read0;
    unsigned char err_flag=0;
    U32 times;
    U8  temp = 0;
    __DI();

    for(times=0; times<cnt; times++)
    {
        IO_WDTCPR1 = 0xa5;    /* H/W WDT Clear */
        __wait_nop();
        __wait_nop();
        *((U16 *)(((tgt_add+times*2)&0x00FF0000)+0x00001554))=0x00AA; //1st cycle
        __wait_nop();
        *((U16 *)(((tgt_add+times*2)&0x00FF0000)+0x00000AA8))=0x0055; //2st cycle
        __wait_nop();
        *((U16 *)(((tgt_add+times*2)&0x00FF0000)+0x00001554))=0x00A0; //3st cycle
        __wait_nop();
        *((U16 *)(tgt_add+times*2))=src_data[times]; //4st cycle
        __wait_nop();

        temp = FSTR_FRDY;
        do
        {
            temp = FSTR_FRDY;      //read atleast twice
            IO_WDTCPR1 = 0xa5;     /* H/W WDT Clear */
        }
        while(!temp);


        while(read0!=src_data[times])
        {
            IO_WDTCPR1 = 0xa5;    /* H/W WDT Clear */
            if((read0 & 0x20) == 0) //bit5=0 means flash is processing
            {
                read0=*((unsigned short *)(tgt_add+times*2)); //read flash data
            }
            else
            {
                read0=*((unsigned short *)(tgt_add+times*2)); //read flash data
                if(read0 != src_data[times])
                {
                    err_flag = 1;
                    __wait_nop();
                    break;
                }
            }
        }

    }

    //  FCTLR_FWE = 0;     //set CPU_ROM mode£¬Flash write disabled
    __EI();
    return err_flag;
}

//function name can be used as a address point
//programing_adds must be even value if using word accessing(2 byte long)
unsigned char hal_Work_Flash_Sector_Programing(long tgt_add, unsigned short* src_data, U32 cnt)
{
    unsigned short read0;
    unsigned char err_flag=0;
    U32 times;

    __DI();
    for(times=0; times<cnt; times++)
    {
        IO_WDTCPR1 = 0xa5;  /* H/W WDT Clear */
        *((short *)(((tgt_add+times*2)&0x00FFF000)+0x00000AA8))=0x00AA; //1st cycle
        *((short *)(((tgt_add+times*2)&0x00FFF000)+0x00000554))=0x0055; //2st cycle
        *((short *)(((tgt_add+times*2)&0x00FFF000)+0x00000AA8))=0x00A0; //3st cycle
        *((short *)(tgt_add+times*2))=src_data[times]; //4st cycle

        read0=*((short *)(tgt_add+times*2));

        while(read0 != src_data[times])
        {
            if((read0 & 0x20) == 0) //bit5=0 means flash is processing
            {
                read0=*((short *)(tgt_add+times*2)); //read flash data
            }
            else
            {
                read0=*((short *)(tgt_add+times*2)); //read flash data

                if(read0 != src_data[times])
                {
                    err_flag=1;
                    while(1)
                    {
                        //IO_PDR05.bit.P1=~IO_PDR05.bit.P1;
                        IO_WDTCPR1 = 0xa5;    /* H/W WDT Clear */
                    }
                }
                else
                {
                }
            }
        }
    }
    __EI();
    //dbg_string("Data_Flash_Sector_Programing:%x, %d\n", tgt_add, cnt);
    return err_flag;
}

