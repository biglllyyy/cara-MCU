#include "mb91520.h"
#include "hal_input_capture.h"
#include "KeyPortReg.h"

unsigned int count = 0;

void INPUT0_sample_1(void)
{
    freerun0_initial();
    INPUT4_initial();
    INPUT4_start();
    freerun0_start();
}
void freerun0_initial(void)
{
    FT3_TCCS3 = 0x0041; /* Setting value=0000_0000_0100_0001 */
    /* bit15 = 0 ECKE internal clock source */
    /* bit14 -10 =0 Reserved bit */
    /* bit9 = 0 Interrupt flag clear */
    /* bit8 = 0 Interrupt disabled */
    /* bit7 = 0 Reserved bit */
    /* bit6 = 1 */
    /* bit5 = 0 Reserved bit */
    /* bit4 = 0 */
    /* bit3-0 = 0001 */
    FT3_TCDT3 = 0x0000; /* Initialization of timer data value */
}
void PORT_SETTING_ICU4_IN(void)
{
    WriteKeyProtectedRegister( (unsigned int)&EPFR01, 0x00, BYTE_ACCESS ); // Input from the ICU4_0 pin
    WriteKeyProtectedRegister( (unsigned int)&DDR09, 0x00, BYTE_ACCESS );
    WriteKeyProtectedRegister( (unsigned int)&PORTEN, 0x01, BYTE_ACCESS );
}
void INPUT4_initial(void)
{
    PORT_SETTING_ICU4_IN(); /* Set the ICU0 pin for peripheral input. */
    ICS45 = 0x01; 			/* Setting value=0000_0001 */
    /* bit7 to 6 = 00 ICP5,4, 0 Interrupt request flag clear */
    /* bit5 to 4 = 00 ICE5,4, 0 Interrupt disabled */
    /* bit3 to 2 = 00 EG51, EG50 ch.5 No edge detected */
    /* bit1 to 0 = 01 EG41, EG40 ch.4 Rising edge detected */
    ICR40 = 0x30; 			/* Input capture ch.4 interrupt level setting (any value) */
    count = 0;
}
void INPUT4_start(void)
{
    ICS45_ICE4 = 1; /* bit4 = 1 ICE45 ch.4 Interrupt enabled */
}
void freerun0_start(void)
{
    FT3_TCCS3_STOP= 0; /* bit6 = 0 STOP count enabled */
}
__interrupt void INPUT4_int(void)
{
    ICS45_ICP4 = 0; /* bit6 = 0 Clearing of ICP4 effective edge detection flag */
    count ++ ;
}
