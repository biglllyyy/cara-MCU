#ifndef APP_WARNING_H
#define APP_WARNING_H
#include "comm_typedef.h"

typedef struct
{
    uint8_t  ID_180689f4_byte7_bit0:1;   //模块电压过高
    uint8_t  ID_180689f4_byte7_bit1:1;   //模块电压过低
    uint8_t  ID_180689f4_byte7_bit2:1;   //SOC过高
    uint8_t  ID_180689f4_byte7_bit3:1;   //SOC过低
    uint8_t  ID_180689f4_byte7_bit4:1;   //过电流
    uint8_t  ID_180689f4_byte7_bit5:1;   //温度过高
    uint8_t  ID_180689f4_byte7_bit6:1;   //SOC太低停车
    uint8_t  ID_180689f4_byte7_bit7:1;   //温度过低
    uint8_t  ID_180689f4_byte7;
	
    uint8_t  ID_181B80f4_byte5_bit01:2;  //单体过压报警 				    00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte5_bit23:2;  //整组过压报警 （总线电压过压报警） 00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte5_bit45:2;  //单体欠压报警  				    00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte5_bit67:2;  //整组欠压报警 （总线电压欠压报警） 00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte5;

	uint8_t  ID_181B80f4_byte6_bit01:2;  //电池低温报警 				    00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte6_bit23:2;  //电池高温报警                   00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte6_bit45:2;  //单体压差报警  				    00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte6_bit67:2;  //模块温差报警                   00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte6;

	uint8_t  ID_181B80f4_byte7_bit01:2;  //电池放电电流过大报警      	    00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte7_bit23:2;  //电池充电电流过大报警            00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte7_bit45:2;  //绝缘检测报警  				    00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte7_bit67:2;  //电池 SOC 低报警                00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte7;

	uint8_t  ID_181B80f4_byte8_bit01:2;  //内网通讯故障 				    00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte8_bit23:2;  //外网通讯故障 （总线电压过压报警）  00：无报警. 01：一级报警. 10：二级报警
    uint8_t  ID_181B80f4_byte8_bit45:2;  //预留
    uint8_t  ID_181B80f4_byte8_bit67:2;  //预留
    uint8_t  ID_181B80f4_byte8;

	uint8_t  ID_0C068980_byte8_bit0:1;  //气泵DC/DC 状态及故障.过流
    uint8_t  ID_0C068980_byte8_bit1:1;  //气泵DC/DC 状态及故障.过热
    uint8_t  ID_0C068980_byte8_bit2:1;  //气泵DC/DC 状态及故障.欠压
    uint8_t  ID_0C068980_byte8_bit3:1;  //气泵DC/DC 状态及故障.过压
    uint8_t  ID_0C068980_byte8_bit4:1;  //气泵DC/DC 状态及故障.短相
    uint8_t  ID_0C068980_byte8_bit5:1;  //气泵DC/DC 状态及故障.缺相
    uint8_t  ID_0C068980_byte8_bit6:1;  //气泵DC/DC 状态及故障.气压信号
    uint8_t  ID_0C068980_byte8_bit7:1;  //气泵DC/DC 状态及故障.1 运行 0 停机
    uint8_t  ID_0C068980_byte8;         //气泵DC/DC 状态及故障

	uint8_t  ID_0C058980_byte8_bit0:1;  //油泵DC/DC 状态及故障.过流
    uint8_t  ID_0C058980_byte8_bit1:1;  //油泵DC/DC 状态及故障.过热
    uint8_t  ID_0C058980_byte8_bit2:1;  //油泵DC/DC 状态及故障.欠压
    uint8_t  ID_0C058980_byte8_bit3:1;  //油泵DC/DC 状态及故障.过压
    uint8_t  ID_0C058980_byte8_bit4:1;  //油泵DC/DC 状态及故障.短相
    uint8_t  ID_0C058980_byte8_bit5:1;  //油泵DC/DC 状态及故障.缺相
    uint8_t  ID_0C058980_byte8_bit6:1;  //油泵DC/DC 状态及故障.预留
    uint8_t  ID_0C058980_byte8_bit7:1;  //油泵DC/DC 状态及故障.1 运行 0 停机
    uint8_t  ID_0C058980_byte8;         //油泵DC/DC 状态及故障

	uint8_t  ID_0C048980_byte8_bit0:1;  //DC/DC 状态及故障.过流
    uint8_t  ID_0C048980_byte8_bit1:1;  //DC/DC 状态及故障.过热
    uint8_t  ID_0C048980_byte8_bit2:1;  //DC/DC 状态及故障.欠压
    uint8_t  ID_0C048980_byte8_bit3:1;  //DC/DC 状态及故障.过压
    uint8_t  ID_0C048980_byte8_bit4:1;  //DC/DC 状态及故障.预留
    uint8_t  ID_0C048980_byte8_bit5:1;  //DC/DC 状态及故障.预留
    uint8_t  ID_0C048980_byte8_bit6:1;  //DC/DC 状态及故障.预留
    uint8_t  ID_0C048980_byte8_bit7:1;  //DC/DC 状态及故障.1 运行 0 停机
    uint8_t  ID_0C048980_byte8;         //DC/DC 状态及故障
} WarningDataStruct;




extern WarningDataStruct warning_data;
void warning_get_data(void);
void warning_send_data(void);
#endif
