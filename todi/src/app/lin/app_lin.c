#include <string.h>
#include "hal_lin.h"
#include "app_lin.h"
#include "_fr.h"
#include "app_led.h"
#include"app_uart_cmd.h"
#include "app_IPconfig.h"

extern U8 g_u8IgnSts;
extern void data_copy(U8 *dest,U8 *src,U8 count);

U16 lin_lost_timecnt				 =0;
U16 lin_pid_0x42_lost_timecnt=0xffff;
U16 lin_pid_0x03_lost_timecnt=0xffff;
U16 lin_pid_0x92_lost_timecnt=0xffff;

car0_lin_msg_t lin0_msg;
car1_lin_msg_t lin1_msg;
car2_lin_msg_t lin2_msg;
car3_lin_msg_t lin3_msg;
car4_lin_msg_t lin4_msg;
car5_lin_msg_t lin5_msg;;

void app_lin_init(void)
{
	lin_lost_timecnt = 0;
 
	memset(&LIN_Data_Send,0,sizeof(LIN_Data_Send));
	memset(&LIN_Data_Rec,0,sizeof(LIN_Data_Rec));
 
	hal_lin_init(app_lin_msg, 4,LIN_SLAVE,19200);
	hal_lin_start(4,LIN_SLAVE);
}

void app_lin_stop(void)
{
	hal_lin_stop(4);
}

U8 app_lin_msg(U8 nPortnum,U8 nLINID,U8 current_rec_status)
{
 unsigned char i,response_type=0;
  
 switch(IPconfig.protocol)
 {
		case HAVAL_2013_2P4L_AT_2WD:
			switch(nPortnum)
 			{
 			 case 4:
 			  switch(nLINID)
 			  {
 			   case 0x42:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x42_lost_timecnt=0;
 			    
 			    response_type=RECEPTION;
 			    
 			    MFS4_LINUART_LAMCR_LDL = 3;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin0_msg.pid_0x42,LIN_Data_Rec,3);
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			   
 			    break;
 			   case 0x03:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x03_lost_timecnt=0;
 			    
 			    response_type=RECEPTION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 4;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin0_msg.pid_0x03,LIN_Data_Rec,4);
 			      break;
 			     default:
 			      break;
 			    }
 			    
 			    break;
 			   case 0x92:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x92_lost_timecnt=0;
 			    
 			    response_type=TRANSMISSION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 1;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      data_copy(LIN_Data_Send,(unsigned char *)&lin0_msg.pid_0x92,1);	
 			      break;
 			     case RESPONSE:
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			    break;
 			   default:
 			    response_type =  NULL; 
 			    
 			    break;
 			  }
 			  break;
 			 default:
 			  break;
 			 
 			}
			break;
		case HAVAL_2013_1P5T_MT_2WD:
			 switch(nPortnum)
 			{
 			 case 4:
 			  switch(nLINID)
 			  {
 			   case 0x42:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x42_lost_timecnt=0;
 			    
 			    response_type=RECEPTION;
 			    
 			    MFS4_LINUART_LAMCR_LDL = 3;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin1_msg.pid_0x42,LIN_Data_Rec,3);
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			   
 			    break;
 			   case 0x03:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x03_lost_timecnt=0;
 			    
 			    response_type=RECEPTION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 4;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin1_msg.pid_0x03,LIN_Data_Rec,4);
 			      break;
 			     default:
 			      break;
 			    }
 			    
 			    break;
 			   case 0x92:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x92_lost_timecnt=0;
 			    
 			    response_type=TRANSMISSION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 1;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      data_copy(LIN_Data_Send,(unsigned char *)&lin1_msg.pid_0x92,1);	
 			      break;
 			     case RESPONSE:
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			    break;
 			   default:
 			    response_type =  NULL; 
 			    
 			    break;
 			  }
 			  break;
 			 default:
 			  break;
 			 
 			}
			break;
		case HAVAL_2012_1P5T_MT_2WD:
			switch(nPortnum)
 			{
 			 case 4:
 			  switch(nLINID)
 			  {
 			   case 0x42:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x42_lost_timecnt=0;
 			    
 			    response_type=RECEPTION;
 			    
 			    MFS4_LINUART_LAMCR_LDL = 2;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin2_msg.pid_0x42,LIN_Data_Rec,2);
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			   
 			    break;
 			   case 0x03:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x03_lost_timecnt=0;
 			    
 			    response_type=RECEPTION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 4;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin2_msg.pid_0x03,LIN_Data_Rec,4);
 			      break;
 			     default:
 			      break;
 			    }
 			    
 			    break;
 			   case 0x92:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x92_lost_timecnt=0;
 			    
 			    response_type=TRANSMISSION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 1;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      data_copy(LIN_Data_Send,(unsigned char *)&lin2_msg.pid_0x92,1);	
 			      break;
 			     case RESPONSE:
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			    break;
 			   default:
 			    response_type =  NULL; 
 			    
 			    break;
 			  }
 			  break;
 			 default:
 			  break;
 			 
 			}
			break;
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:
			switch(nPortnum)
 			{
 			 case 4:
 			  switch(nLINID)
 			  {
 			   case 0x42:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x42_lost_timecnt=0;
 			    
 			    response_type=RECEPTION;
 			    
 			    MFS4_LINUART_LAMCR_LDL = 6;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin3_msg.pid_0x42,LIN_Data_Rec,6);
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			   
 			    break;
 			   case 0x03:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x03_lost_timecnt=0;
 			    
 			    response_type=RECEPTION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 4;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin3_msg.pid_0x03,LIN_Data_Rec,4);
 			      break;
 			     default:
 			      break;
 			    }
 			    
 			    break;
 			   case 0x92:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x92_lost_timecnt=0;
 			    
 			    response_type=TRANSMISSION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 1;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      data_copy(LIN_Data_Send,(unsigned char *)&lin3_msg.pid_0x92,1);	
 			      break;
 			     case RESPONSE:
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			    break;
 			   default:
 			    response_type =  NULL; 
 			    
 			    break;
 			  }
 			  break;
 			 default:
 			  break;
 			 
 			}
			break;
		case HAVAL_2011_2P0T_MT_2WD:
			switch(nPortnum)
 			{
 			 case 4:
 			  switch(nLINID)
 			  {
 			   case 0x42:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x42_lost_timecnt=0;
 			    
 			    response_type=RECEPTION;
 			    
 			    MFS4_LINUART_LAMCR_LDL = 2;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin4_msg.pid_0x42,LIN_Data_Rec,2);
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			   
 			    break;
 			   case 0x03:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x03_lost_timecnt=0;
 			    
 			    response_type=RECEPTION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 4;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin4_msg.pid_0x03,LIN_Data_Rec,4);
 			      break;
 			     default:
 			      break;
 			    }
 			    
 			    break;
 			   case 0x92:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x92_lost_timecnt=0;
 			    
 			    response_type=TRANSMISSION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 1;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      data_copy(LIN_Data_Send,(unsigned char *)&lin4_msg.pid_0x92,1);	
 			      break;
 			     case RESPONSE:
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			    break;
 			   default:
 			    response_type =  NULL; 
 			    
 			    break;
 			  }
 			  break;
 			 default:
 			  break;
 			 
 			}
			break;
		case HAVAL_2013_1P5T_MT_4WD:
			 switch(nPortnum)
 			{
 			 case 4:
 			  switch(nLINID)
 			  {
 			   case 0x42:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x42_lost_timecnt=0;
 			    
 			    response_type=RECEPTION;
 			    
 			    MFS4_LINUART_LAMCR_LDL = 3;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin5_msg.pid_0x42,LIN_Data_Rec,3);
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			   
 			    break;
 			   case 0x03:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x03_lost_timecnt=0;
 			    
 			    response_type=RECEPTION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 4;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      
 			      break;
 			     case RESPONSE:
 			      for(i = 0; i<8; i++)
 			      {
 			       LIN_Data_Rec[i] = MFS4_LINUART_RDR0;
 			      }
 			      data_copy((unsigned char *)&lin5_msg.pid_0x03,LIN_Data_Rec,4);
 			      break;
 			     default:
 			      break;
 			    }
 			    
 			    break;
 			   case 0x92:
 			    lin_lost_timecnt=0;
 			    lin_pid_0x92_lost_timecnt=0;
 			    
 			    response_type=TRANSMISSION; 
 			    
 			    MFS4_LINUART_LAMCR_LDL = 1;
 			    switch(current_rec_status)
 			    {
 			     case HEADER:
 			      data_copy(LIN_Data_Send,(unsigned char *)&lin5_msg.pid_0x92,1);	
 			      break;
 			     case RESPONSE:
 			      
 			      break;
 			     default:
 			      break;
 			    }
 			    break;
 			   default:
 			    response_type =  NULL; 
 			    
 			    break;
 			  }
 			  break;
 			 default:
 			  break;
 			 
 			}
			break;
 }
 return response_type;
}

static void lin_pid_0x42_analyse(void)
{
	if(ON == g_u8IgnSts)
	{
		
	}
	else
	{
		if(lin_pid_0x42_lost_timecnt<50)
		{
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					break;
			}
		}
		else
		{  
		}
	}
}
static void lin_pid_0x03_analyse(void)
{
	if(ON == g_u8IgnSts)
	{
		
	}
	else
	{
		if(lin_pid_0x03_lost_timecnt<50)
		{
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					break;
			}
		}
		else
		{
			
		}
	}
}
static void lin_pid_0x92_analyse(void)
{
	if(ON == g_u8IgnSts)
	{
		
	}
	else
	{
		if(lin_pid_0x92_lost_timecnt<50)
		{
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					break;
			}
		}
		else
		{
			
		}
	}
}
static void app_lin_process(void)
{
	lin_pid_0x42_analyse();
	lin_pid_0x03_analyse();
	lin_pid_0x92_analyse();
}

void app_lin_lost_time_cnt_100ms(void)
{
	static unsigned char g_u8IgnSts_bak=OFF;
	
	if((ON == g_u8IgnSts)&&(OFF==g_u8IgnSts_bak))
	{
		lin_lost_timecnt = 0;
		lin_pid_0x42_lost_timecnt=0xffff;
		lin_pid_0x03_lost_timecnt=0xffff;
		lin_pid_0x92_lost_timecnt=0xffff;
		
		memset(&lin0_msg,0,sizeof(lin0_msg));
		memset(&lin1_msg,0,sizeof(lin1_msg));
		memset(&lin2_msg,0,sizeof(lin2_msg));
		memset(&lin3_msg,0,sizeof(lin3_msg));
		memset(&lin4_msg,0,sizeof(lin4_msg));
		memset(&lin5_msg,0,sizeof(lin5_msg));
		
		g_u8IgnSts_bak=g_u8IgnSts;
	}
	else if((OFF == g_u8IgnSts)&&(ON==g_u8IgnSts_bak))
	{
		lin_lost_timecnt = 0;
		lin_pid_0x42_lost_timecnt=0xffff;
		lin_pid_0x03_lost_timecnt=0xffff;
		lin_pid_0x92_lost_timecnt=0xffff;
		
		memset(&lin0_msg,0,sizeof(lin0_msg));
		memset(&lin1_msg,0,sizeof(lin1_msg));
		memset(&lin2_msg,0,sizeof(lin2_msg));
		memset(&lin3_msg,0,sizeof(lin3_msg));
		memset(&lin4_msg,0,sizeof(lin4_msg));
		memset(&lin5_msg,0,sizeof(lin5_msg));
		
		g_u8IgnSts_bak=g_u8IgnSts;
	}
	else
	{
		if(lin_lost_timecnt<0xffff)
		{
			lin_lost_timecnt++;
		}
		if(lin_pid_0x42_lost_timecnt<0xffff)
		{
			lin_pid_0x42_lost_timecnt++;
		}
		if(lin_pid_0x03_lost_timecnt<0xffff)
		{
			lin_pid_0x03_lost_timecnt++;
		}
		if(lin_pid_0x92_lost_timecnt<0xffff)
		{
			lin_pid_0x92_lost_timecnt++;
		}
	}
}
void app_lin_task10(void)
{
	app_lin_process();
}
