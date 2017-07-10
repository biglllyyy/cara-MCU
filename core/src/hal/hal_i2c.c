#include "_fr.h"
#include "hal_i2c.h"

static S8 hal_i2c_writeSlaveAddr(U8 nPortNum, U8 a_ucSlaveAddress_7Bit, U8 bWrite);


static S32 hal_changeBaudRate(U8 nPortNum, U32 nBaudRate)
{
	S32 l_nError=i2c_Error_NoError;
	U32 l_nRLValue;

	if (nBaudRate > 400*1000)	//>400k is prohibited
		return i2c_Error_InvalidParameter;

	l_nRLValue = I2C_SupplyClk / (nBaudRate);//-1;

	if (l_nRLValue < 1 || l_nRLValue > 32767)
		return i2c_Error_InvalidParameter;

	switch(nPortNum)
	{
	case 3:
		if (MFS3_I2C_ISMK_EN == 1)
		{
			l_nError = i2c_Error_InvalidSettingStep;
		}
		else
		{
			I2C_BGR3 = l_nRLValue;
		}
		break;
	default:
		l_nError=i2c_Error_InvalidParameter;
		break;
	}
	return l_nError;
}




S32 hal_i2c_open(U8 nPortNum, U32 nBaudRate)
{
	S32 l_nError = i2c_Error_NoError;

	switch(nPortNum)
	{
	case 3://Set SOT and SCK as input to read the bus state
	{
		WriteKeyProtectedRegister( (U32)&DDR07, 0xBF, BYTE_ACCESS );
		if((PDR07_P6 == 0) && (PDR07_P7 == 0))
		{
			//SOT is always LOW or SCK is always LOW
			return i2c_Error_DeviceBusy;
		}
	}

	WriteKeyProtectedRegister( (U32)&DDR07, 0x3F, BYTE_ACCESS );
	WriteKeyProtectedRegister( (U32)&PFR07, 0xC0, BYTE_ACCESS );
	__wait_nop();
	//WriteKeyProtectedRegister( (U32)&EPFR07, 0x00, BYTE_ACCESS );
	WriteKeyProtectedRegister( (U32)&EPFR07, 0xEA, BYTE_ACCESS );
	//WriteKeyProtectedRegister( (U32)&PPER07, 0xC0, BYTE_ACCESS );
	//SMR
	MFS3_I2C_ISMK_EN= 0;
	MFS3_I2C_SMR_MD = 4;	//set as i2c mode
	//			MFS3_I2C_SMR_TIE= 1;
	break;


	default:
		l_nError= i2c_Error_InvalidParameter;
		break;
	}


	if (hal_changeBaudRate(nPortNum,nBaudRate) != i2c_Error_NoError)
	{
		l_nError= i2c_Error_InvalidParameter;
	}
	else
	{
		//SSR
		MFS3_I2C_SSR_REC = 1;	//Clear error flag for reception , SCR4_CRE=1;
		MFS3_I2C_IBCR_INT= 0;	//Clear INT
	}
	switch(nPortNum)
	{
	case 3:
		MFS3_I2C_IBCR_MSS= 0;
		MFS3_I2C_ISMK_EN = 1;		//enable I2C1
		break;
	}
	return l_nError;
}


S32 hal_i2c_WriteData(U8 nPortNum, U8 a_ucSlaveAddress_7Bit,
		U8 a_ucNumOfData, U8 *a_pDataArray)
{
	if (a_pDataArray == 0)
		return 0;

	hal_i2c_writeSlaveAddr(nPortNum, a_ucSlaveAddress_7Bit, 1);

#if 0
	//write data
	for(u8CharIndex=0;u8CharIndex<a_ucNumOfData;u8CharIndex++)
	{
		switch (nPortNum)
		{
			case 3:
			{
				//wait for ready
				while(!((MFS3_I2C_IBCR_INT==1)&&(MFS3_I2C_SSR_TDRE==1)));
				MFS3_I2C_TDR0 = a_pDataArray[u8CharIndex];

				MFS3_I2C_IBCR_WSEL=0;
				MFS3_I2C_IBCR_ACKE=0;
				MFS3_I2C_IBCR_INT=0;
				break;
			}
			default:
				break;
		}
	}

	switch (nPortNum)
	{
		case 3:
		{
			while (!(MFS3_I2C_IBCR_INT==1));
			//stop condition
			//Clear interrupt flag
			MFS3_I2C_IBCR_WSEL=0;
			MFS3_I2C_IBCR_MSS=0;
			MFS3_I2C_IBCR_ACKE=0;
			MFS3_I2C_IBCR_INT=0;

			//check if stopped
			while (MFS3_I2C_IBSR_BB==1);
			break;
		}
		default:
			break;
	}
#endif
	return i2c_Error_NoError;
}

S32 hal_i2c_ReadData(U8 nPortNum, U8 a_ucSlaveAddress_7Bit, U8 a_ucNumOfData, U8 *a_pDataArray)
{
	S32 s32CharIndex;

	if(a_pDataArray == 0)
		return 0;

	hal_i2c_writeSlaveAddr(nPortNum, a_ucSlaveAddress_7Bit, 0);

	switch (nPortNum)
	{
		case 3:
		{
			//Clear interrupt flag
			MFS3_I2C_IBCR_INT=0;

			for(s32CharIndex=0;s32CharIndex<a_ucNumOfData;)
			{
				//wait for ready
				while(!(MFS3_I2C_IBCR_INT==1));
				if ((MFS3_I2C_IBSR_FBT==0) && (MFS3_I2C_SSR_RDRF==1))
				{
					a_pDataArray[s32CharIndex] = MFS3_I2C_RDR0;
					s32CharIndex++;
				}
				//ACK setting
				MFS3_I2C_IBCR_WSEL = 1;
				MFS3_I2C_IBCR_ACKE = 1;
				MFS3_I2C_IBCR_INT = 0;

			}

			while(!(MFS3_I2C_IBCR_INT==1));
			//stop condition
			//Clear interrupt flag
			MFS3_I2C_IBCR_WSEL=0;
			MFS3_I2C_IBCR_MSS=0;
			MFS3_I2C_IBCR_ACKE=0;
			MFS3_I2C_IBCR_INT=0;

			//check if stopped
			while(MFS3_I2C_IBSR_BB==1);
			break;
		}
		default:
			break;
	}

	return i2c_Error_NoError;
}


static S8 hal_i2c_writeSlaveAddr(U8 nPortNum, U8 a_ucSlaveAddress_7Bit, U8 bWrite)
{
	S8 ret = -1;

	//write slave address
	switch(nPortNum)
	{
		case 3:
			MFS3_I2C_IBSR_BB = 0;
			while( MFS3_I2C_IBSR_BB);    /* wait for bus idle state */
			while( MFS3_I2C_IBCR_MSS );   /*  wait for slave state */
			if (MFS3_I2C_SSR_TDRE == 1)
			{
				if(bWrite)
					MFS3_I2C_TDR0 = (a_ucSlaveAddress_7Bit<<1)&~0x01;//write
				else
					MFS3_I2C_TDR0 = (a_ucSlaveAddress_7Bit<<1)|0x01;//read
			}
			MFS3_I2C_IBCR_MSS = 1;

			//retry if needed
			while(1)
			{
				if (MFS3_I2C_IBCR_INT == 1)
				{
					//No Ack
					if (MFS3_I2C_IBSR_RACK == 1)
					{
						//! Ask to resend data or stop
						MFS3_I2C_IBCR_MSS		= 1;
						MFS3_I2C_IBCR_ACT_SCC	= 1;
						MFS3_I2C_IBCR_INT 		= 0;
					}
					else
					{
						ret = 0;
						break;
					}
				}
				__wait_nop();
			}
			break;
		default:
			break;
	}

	return ret;
}



