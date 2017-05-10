#ifndef _FUEL_METHOD2_
#define _FUEL_METHOD2_

unsigned int get_this_trip_used_fuel();
void set_this_trip_used_fuel(unsigned int val);
void set_this_trip_distance(unsigned int val);
unsigned int get_this_trip_distance();


void app_clac_fuel_method2_task();
void app_calc_this_trip_AFE_task();

void save_fuel_ad(void);
unsigned short get_fuel_volum();
unsigned char get_fuel_percent3();

unsigned short get_this_trip_AFE();
unsigned int get_total_used_fuel();
unsigned short get_drive_time();
void clear_total_used_fuel();

unsigned char get_test_data_U8_ad_saved();

unsigned short get_actual_fuel_ad();
unsigned short get_disp_fuel_ad();


#endif
