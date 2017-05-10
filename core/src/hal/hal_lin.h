#ifndef _HAL_LIN_H
#define _HAL_LIN_H

#define LIN_MASTER  0
#define LIN_SLAVE   1


#define HEADER       0
#define RESPONSE     1


#define TRANSMISSION  1
#define RECEPTION     2


extern unsigned char LIN_Data_Send[8],LIN_Data_Rec[8];

void hal_lin_init(void (*ptr), unsigned char nPortNum,unsigned char LIN_mode,unsigned long nBaudRate);
void hal_lin_start(unsigned char nPortNum,unsigned char nLINID);
void hal_lin_stop(unsigned char nPortNum);

__interrupt void ISR_LIN0_Reception(void);
__interrupt void ISR_LIN0_Transmission(void);
__interrupt void ISR_LIN1_Reception(void);
__interrupt void ISR_LIN1_Transmission(void);
__interrupt void ISR_LIN4_Transmission(void);
__interrupt void ISR_LIN4_Reception(void);

#endif
