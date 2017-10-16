#include "comm_typedef.h"

#if 0
typedef struct
{
		struct {
			U8 IN_KL01to08;
			U8 IN_KL09to16;
			U8 IN_KL17to22;
			U8 AddressError;  //这个存在不对齐的风险要改，将其向上移动两个位置
			U16 Frequence1;
			U16 Frequence2;
		} ID_700;

		struct {
			U8 ADCChannel1Value;
			U8 ADCChannel2Value;
			U8 ADCChannel3Value;
			U8 ADCChannel4Value;
			U8 ADCChannel5Value;
			U8 ADCChannel6Value;
			U16 Reversed;
		} ID_710;

		struct {
			U16 Ctrl_Out01;
			U16 Ctrl_Out02;
			U16 Ctrl_Out03;
			U16 Ctrl_Out04;
		} ID_730;
		struct {
			U16 Ctrl_Out05;
			U16 Ctrl_Out06;
			U16 Ctrl_Out07;
			U16 Ctrl_Out08;
		} ID_740;
		struct {
			U16 Ctrl_Out09;
			U16 Ctrl_Out10;
			U16 Ctrl_Out11;
			U16 Ctrl_Out12;
		} ID_750;
		struct {
			U16 Ctrl_Out13;
			U16 Ctrl_Out14;
			U16 Ctrl_Out15;
			U16 Ctrl_Out16;
		} ID_760;
		struct {
			U16 Ctrl_Out17;
			U16 Ctrl_Out18;
			U16 Ctrl_Out19;
			U16 Ctrl_Out20;
		} ID_770;
		struct {
			U16 Ctrl_Out21;
			U16 Ctrl_Out22;
			U16 Ctrl_Out23;
			U16 Ctrl_Out24;
		} ID_780;
		struct {
			U16 Ctrl_Out25;
			U16 Ctrl_Out26;
			U16 Ctrl_Out27;
			U16 Ctrl_Out28;
		} ID_790;
		struct {
			U16 Ctrl_Out29;
			U16 Ctrl_Out30;
			U16 Ctrl_Out31;
			U16 Ctrl_Out32;
		} ID_7A0;
		struct {
			U16 Ctrl_Out33;
			U16 Ctrl_Out34;
			U16 Ctrl_Out35;
			U16 Ctrl_Out36;
		} ID_7B0;

} Big_Moudle_Data_Struct;



void big_moudle_front_send_data(void);
void big_moudle_middle_send_data(void);
void big_moudle_back_send_data(void);
#endif
