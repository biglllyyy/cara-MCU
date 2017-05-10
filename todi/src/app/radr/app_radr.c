#include <string.h>
#include "app_radr.h"
#include "app_uart_cmd.h"
#include "app_can.h"
#include "app_IPconfig.h"

void app_radar_init(void)
{
	memset(&uart_special_data.specialInfo.Radar,0,sizeof(uart_special_data.specialInfo.Radar));
}

void app_radar_ctl(void)
{
	if(ON == g_u8IgnSts)
	{
		if(can_id_3df_lost_timecnt<CAN_LOST_TIME)
		{
			switch(IPconfig.protocol)                               
			{                                                     
				case HAVAL_2013_2P4L_AT_2WD:                            
					uart_special_data.specialInfo.Radar.fifth.bit.RadarFailure=can0.id_3df.RadarFailure;
					uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarDiag=can0.id_3df.FrontRadarDiag;
					uart_special_data.specialInfo.Radar.fifth.bit.RearRadarDiag=can0.id_3df.RearRadarDiag;
					
					if(uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarDiag==1)//前车雷达诊断
					{
						uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarWarning=can0.id_3df.FrontRadarWarning;
					}
					else
					{
						uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarWarning=0;
						
						if(can0.id_3df.FrontRadarWarning==1)
						{
							//前雷达
							//左传感器
							if(can0.id_3df.FrontRadarLeftSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=5;
							}
							else
							{
								switch(can0.id_3df.FrontRadarLeftSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
										break;
								}
							}
							//左中传感器
							if(can0.id_3df.FrontRadarLeftMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=5;
							}
							else
							{
								switch(can0.id_3df.FrontRadarLeftMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
										break;
								}
							}
							//右中传感器
							if(can0.id_3df.FrontRadarRightMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=5;
							}
							else
							{
								switch(can0.id_3df.FrontRadarRightMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
										break;
								}
							}
							//右传感器
							if(can0.id_3df.FrontRadarRightSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=5;
							}
							else
							{
								switch(can0.id_3df.FrontRadarRightSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
										break;
								}
							} 
						}
						else
						{
							uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
						} 
					}
					
					if(uart_special_data.specialInfo.Radar.fifth.bit.RearRadarDiag==1)//后车雷达诊断
					{
						uart_special_data.specialInfo.Radar.fifth.bit.RearRadarWarning=can0.id_3df.RearRadarWarning;
					}
					else
					{
						uart_special_data.specialInfo.Radar.fifth.bit.RearRadarWarning=0;
   			
						if(can0.id_3df.RearRadarWarning==1)
						{
							//后雷达
							//左传感器
							if(can0.id_3df.RearRadarLeftSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=5;
							}
							else
							{
								switch(can0.id_3df.RearRadarLeftSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
										break;
								}
							}
							//左中传感器
							if(can0.id_3df.RearRadarLeftMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=5;
							}
							else
							{
								switch(can0.id_3df.RearRadarLeftMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
										break;
								}
							}
							//右中传感器
							if(can0.id_3df.RearRadarRightMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=5;
							}
							else
							{
								switch(can0.id_3df.RearRadarRightMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
										break;
								}
							}
							//右传感器
							if(can0.id_3df.RearRadarRightSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=5;
							}
							else
							{
								switch(can0.id_3df.RearRadarRightSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
										break;
								}
							} 
						}
						else
						{
							uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
						}
					}
					break;                                            
				case HAVAL_2013_1P5T_MT_2WD:                            
					uart_special_data.specialInfo.Radar.fifth.bit.RadarFailure=can1.id_3df.RadarFailure;
					uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarDiag=can1.id_3df.FrontRadarDiag;
					uart_special_data.specialInfo.Radar.fifth.bit.RearRadarDiag=can1.id_3df.RearRadarDiag;
					
					if(uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarDiag==1)//前车雷达诊断
					{
						uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarWarning=can1.id_3df.FrontRadarWarning;
					}
					else
					{
						uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarWarning=0;
						
						if(can1.id_3df.FrontRadarWarning==1)
						{
							//前雷达
							//左传感器
							if(can1.id_3df.FrontRadarLeftSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=5;
							}
							else
							{
								switch(can1.id_3df.FrontRadarLeftSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
										break;
								}
							}
							//左中传感器
							if(can1.id_3df.FrontRadarLeftMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=5;
							}
							else
							{
								switch(can1.id_3df.FrontRadarLeftMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
										break;
								}
							}
							//右中传感器
							if(can1.id_3df.FrontRadarRightMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=5;
							}
							else
							{
								switch(can1.id_3df.FrontRadarRightMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
										break;
								}
							}
							//右传感器
							if(can1.id_3df.FrontRadarRightSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=5;
							}
							else
							{
								switch(can1.id_3df.FrontRadarRightSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
										break;
								}
							} 
						}
						else
						{
							uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
						} 
					}
					
					if(uart_special_data.specialInfo.Radar.fifth.bit.RearRadarDiag==1)//后车雷达诊断
					{
						uart_special_data.specialInfo.Radar.fifth.bit.RearRadarWarning=can1.id_3df.RearRadarWarning;
					}
					else
					{
						uart_special_data.specialInfo.Radar.fifth.bit.RearRadarWarning=0;
   			
						if(can1.id_3df.RearRadarWarning==1)
						{
							//后雷达
							//左传感器
							if(can1.id_3df.RearRadarLeftSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=5;
							}
							else
							{
								switch(can1.id_3df.RearRadarLeftSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
										break;
								}
							}
							//左中传感器
							if(can1.id_3df.RearRadarLeftMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=5;
							}
							else
							{
								switch(can1.id_3df.RearRadarLeftMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
										break;
								}
							}
							//右中传感器
							if(can1.id_3df.RearRadarRightMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=5;
							}
							else
							{
								switch(can1.id_3df.RearRadarRightMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
										break;
								}
							}
							//右传感器
							if(can1.id_3df.RearRadarRightSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=5;
							}
							else
							{
								switch(can1.id_3df.RearRadarRightSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
										break;
								}
							} 
						}
						else
						{
							uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
						}
					}                                             
					break;                                            
				case HAVAL_2012_1P5T_MT_2WD:                            
					                                             
					break;   
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:                           
					uart_special_data.specialInfo.Radar.fifth.bit.RadarFailure=can3.id_3df.RadarFailure;
					uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarDiag=can3.id_3df.FrontRadarDiag;
					uart_special_data.specialInfo.Radar.fifth.bit.RearRadarDiag=can3.id_3df.RearRadarDiag;
					
					if(uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarDiag==1)//前车雷达诊断
					{
						uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarWarning=can3.id_3df.FrontRadarWarning;
					}
					else
					{
						uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarWarning=0;
						
						if(can3.id_3df.FrontRadarWarning==1)
						{
							//前雷达
							//左传感器
							if(can3.id_3df.FrontRadarLeftSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=5;
							}
							else
							{
								switch(can3.id_3df.FrontRadarLeftSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
										break;
								}
							}
							//左中传感器
							if(can3.id_3df.FrontRadarLeftMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=5;
							}
							else
							{
								switch(can3.id_3df.FrontRadarLeftMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
										break;
								}
							}
							//右中传感器
							if(can3.id_3df.FrontRadarRightMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=5;
							}
							else
							{
								switch(can3.id_3df.FrontRadarRightMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
										break;
								}
							}
							//右传感器
							if(can3.id_3df.FrontRadarRightSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=5;
							}
							else
							{
								switch(can3.id_3df.FrontRadarRightSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
										break;
								}
							} 
						}
						else
						{
							uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
						} 
					}
					
					if(uart_special_data.specialInfo.Radar.fifth.bit.RearRadarDiag==1)//后车雷达诊断
					{
						uart_special_data.specialInfo.Radar.fifth.bit.RearRadarWarning=can3.id_3df.RearRadarWarning;
					}
					else
					{
						uart_special_data.specialInfo.Radar.fifth.bit.RearRadarWarning=0;
   			
						if(can3.id_3df.RearRadarWarning==1)
						{
							//后雷达
							//左传感器
							if(can3.id_3df.RearRadarLeftSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=5;
							}
							else
							{
								switch(can3.id_3df.RearRadarLeftSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
										break;
								}
							}
							//左中传感器
							if(can3.id_3df.RearRadarLeftMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=5;
							}
							else
							{
								switch(can3.id_3df.RearRadarLeftMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
										break;
								}
							}
							//右中传感器
							if(can3.id_3df.RearRadarRightMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=5;
							}
							else
							{
								switch(can3.id_3df.RearRadarRightMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
										break;
								}
							}
							//右传感器
							if(can3.id_3df.RearRadarRightSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=5;
							}
							else
							{
								switch(can3.id_3df.RearRadarRightSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
										break;
								}
							} 
						}
						else
						{
							uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
						}
					}                                        
					break;
				case HAVAL_2011_2P0T_MT_2WD:                            
					                                             
					break;     
				case HAVAL_2013_1P5T_MT_4WD:                            
					uart_special_data.specialInfo.Radar.fifth.bit.RadarFailure=can5.id_3df.RadarFailure;
					uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarDiag=can5.id_3df.FrontRadarDiag;
					uart_special_data.specialInfo.Radar.fifth.bit.RearRadarDiag=can5.id_3df.RearRadarDiag;
					
					if(uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarDiag==1)//前车雷达诊断
					{
						uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarWarning=can5.id_3df.FrontRadarWarning;
					}
					else
					{
						uart_special_data.specialInfo.Radar.fifth.bit.FrontRadarWarning=0;
						
						if(can5.id_3df.FrontRadarWarning==1)
						{
							//前雷达
							//左传感器
							if(can5.id_3df.FrontRadarLeftSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=5;
							}
							else
							{
								switch(can5.id_3df.FrontRadarLeftSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
										break;
								}
							}
							//左中传感器
							if(can5.id_3df.FrontRadarLeftMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=5;
							}
							else
							{
								switch(can5.id_3df.FrontRadarLeftMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
										break;
								}
							}
							//右中传感器
							if(can5.id_3df.FrontRadarRightMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=5;
							}
							else
							{
								switch(can5.id_3df.FrontRadarRightMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
										break;
								}
							}
							//右传感器
							if(can5.id_3df.FrontRadarRightSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=5;
							}
							else
							{
								switch(can5.id_3df.FrontRadarRightSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
										break;
								}
							} 
						}
						else
						{
							uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.first.bit.FrontRadarLeftMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.second.bit.FrontRadarRightSensorWarningLevel=0;
						} 
					}
					
					if(uart_special_data.specialInfo.Radar.fifth.bit.RearRadarDiag==1)//后车雷达诊断
					{
						uart_special_data.specialInfo.Radar.fifth.bit.RearRadarWarning=can5.id_3df.RearRadarWarning;
					}
					else
					{
						uart_special_data.specialInfo.Radar.fifth.bit.RearRadarWarning=0;
   			
						if(can5.id_3df.RearRadarWarning==1)
						{
							//后雷达
							//左传感器
							if(can5.id_3df.RearRadarLeftSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=5;
							}
							else
							{
								switch(can5.id_3df.RearRadarLeftSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
										break;
								}
							}
							//左中传感器
							if(can5.id_3df.RearRadarLeftMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=5;
							}
							else
							{
								switch(can5.id_3df.RearRadarLeftMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
										break;
								}
							}
							//右中传感器
							if(can5.id_3df.RearRadarRightMidSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=5;
							}
							else
							{
								switch(can5.id_3df.RearRadarRightMidSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=2;
										break;
									case 4:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=3;
										break;
									case 5:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=3;
										break;
									case 6:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=4;
										break;
									case 7:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=4;
										break;
									default:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
										break;
								}
							}
							//右传感器
							if(can5.id_3df.RearRadarRightSensorSegFlash==1)
							{
								uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=5;
							}
							else
							{
								switch(can5.id_3df.RearRadarRightSensorSegNum)
								{
									case 0:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
										break;
									case 1:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=1;
										break;
									case 2:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=2;
										break;
									case 3:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=3;
										break;
									default:
										uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
										break;
								}
							} 
						}
						else
						{
							uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.third.bit.RearRadarLeftMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightMidSensorWarningLevel=0;
							uart_special_data.specialInfo.Radar.fourth.bit.RearRadarRightSensorWarningLevel=0;
						}
					}                                             
					break;                                       
			}
		}
		else
		{
			memset(&uart_special_data.specialInfo.Radar,0,sizeof(uart_special_data.specialInfo.Radar));
		}      
	}
	else
	{
		memset(&uart_special_data.specialInfo.Radar,0,sizeof(uart_special_data.specialInfo.Radar));
	}
}
