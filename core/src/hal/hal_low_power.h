typedef enum
{
    NORMAL = 0,
    SLEEP = 1,
    STANDBY_WATCH = 2,
    STANDBY_WATCH_POWERDOWN = 3,
    STANDBY_STOP = 4,
    STANDBY_STOP_POWERDOWN = 5,
    MODE_ALL
} t_low_power_mode;

/* power consumption
   NORMAL> SLEEP > STABDBY_WATCH > STANDBY_WATCH_POWERDOWN >STANDBY_STOP > STANDBY_STOP_POWERDOWN
*/
void hal_entry_low_power_mode(void);
void hal_exit_low_power_mode(void);

