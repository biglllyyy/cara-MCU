#include <stdlib.h>
#include <string.h>
#include "mid_buzzer.h"
#include "hal_ppg.h"
#include "hal_free_run_timer.h"
#include "hal_reload_timer.h"
#include "mid_dbg.h"

#ifdef WIN32
#include <windows.h>
#include "hal_reload_timer.h"
#endif

static  PPGUtil BuzUnit= {0};

S16 top_priority_bit(U32 priority)
{
	S16 top = 0;
	U8 index;
		
	if(priority == 0)
		return 0;
	for(index = 0; index < 32; index++)
	{
		if((priority >> index) & 0x00000001)
			break;
	}
	top = ++index;

	return top; 
}



static const U32 MaskBits[] =
{
    0x00,     
	0x01,     0x02,     0x04,     0x08,     0x10,      0x20,      0x40,      0x80,
    0x100,    0x200,    0x400,    0x800,    0x1000,    0x2000,    0x4000,    0x8000,
    0x10000,  0x20000,  0x40000,  0x80000,  0x100000,  0x200000,  0x400000,  0x800000,
    0x1000000,0x2000000,0x4000000,0x8000000,0x10000000,0x20000000,0x40000000,0x80000000,
};

static void switch_buzzer(const buzzer * buz)
{
    if(buz == NULL)
    {
        /*printf("switch_buzzer(%s)\n", buzzer_string[0]);*/

        /*/ do buzzer off*/
        g_buzzer.current = 0;

        /*buzzer off*/
        g_buzzer.shutdown = 1; /* lazy shutdown to process_buzzer*/
    }
    else
    {
        /*printf("switch_buzzer(%s)\n", buzzer_string[buz->priority]);*/

        /* do buzzer on*/
        g_buzzer.current = buz->priority;

        /*buzzer on*/
        g_buzzer.startup = 1; /* lazy startup to process_buzzer*/
    }
}

void start_buzzer(U32 priority)
{
    U16 top;

    /*printf("start_buzzer (%s)\n", buzzer_string[priority]);*/

    /* zero, test if already started*/
    do
    {
        if(priority >= BEEPCOUNT)
            break;
        if(!(g_buzzer.priority_bits & MaskBits[priority]))//如果当前需要触发的报警音处于OFF状态
        {
            /*first, get top priority bit*/
            top = top_priority_bit(g_buzzer.priority_bits);
            /* second, set priority bit*/
            g_buzzer.priority_bits |= MaskBits[priority];
			//dbg_string("start buz top:%d, priority:%d g_buzzer.priority_bits:0x%x\n",top,priority,g_buzzer.priority_bits);
            /*third, switch to higher priority beep on/off*/
            if((priority < top) || (top == 0))
            {
                switch_buzzer(g_buzzer.list[priority]);
            }
        }
    }
    while (0);
}

void restart_buzzer(U32 priority)
{
    U16 top;

    /*first, get top priority bit*/
    top = top_priority_bit(g_buzzer.priority_bits);

    /* second, set priority bit*/
    g_buzzer.priority_bits |= MaskBits[priority];

    /*third, switch to higher priority beep on/off*/
    if((priority <= top) || (top == 0))
    {
        switch_buzzer(g_buzzer.list[priority]);
    }
}


void stop_buzzer(U32  priority)
{
    U16 top;
	
    switch(priority)
    {
    default:
        break;
    }
    /*printf("stop_buzzer  (%s)\n", buzzer_string[priority]);*/
    /* zero, test if already stoped*/
    if((g_buzzer.priority_bits & MaskBits[priority]))
    {
        /* first, clear priority bit*/
        g_buzzer.priority_bits &= (U32)(~MaskBits[priority]);
        /* second, get top priority bit*/
        top = top_priority_bit(g_buzzer.priority_bits);
		//dbg_string("stop buz top:%d, priority:%d g_buzzer.priority_bits:0x%x\n",top,priority,g_buzzer.priority_bits);
        /* third, switch to lower priority beep on/off*/
        if((top > priority) || top == 0)
        {
            switch_buzzer(g_buzzer.list[top]);
        }
    }
}

U8 IsAnyBeepWarnigOn(void)
{
	if(g_buzzer.priority_bits)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void process_buzzer(void) /* 10ms interval*/
{
    buzzer * buz = g_buzzer.list[g_buzzer.current];

    /* if null buzzer, ignore*/
    if(buz == NULL)
    {
        if(g_buzzer.shutdown)
        {
            g_buzzer.shutdown = 0;
	     /*如果上一个播放的不是和弦*/
	     if(BuzUnit.nFrq!=BUZZER_CHORD_FREQUENCY)
	     {
	     		/*关闭PWM*/
            		HalPwmStop(&BuzUnit,PPG_PIN_LOW);
            		/*printf("\n");*/
	     }
	     /*如不是，则逐步减小占空比*/
	     else
	     {
	    		if(BuzUnit.nPDUT>1)
	    		{
	     			BuzUnit.nPDUT-=2;
	    		}
			else
			{
				BuzUnit.nPDUT=0;
			}
			/*占空比到0则关闭PWM*/
			if(BuzUnit.nPDUT==0)
			{
				HalPwmStop(&BuzUnit,PPG_PIN_LOW);
			}
			else
			{
				buzzer_change_pwm_duty();
			}
	     }
	     //dbg_string("buz shutdown \n");
        }
        return;
    }

    /* check first time start beep*/
    if(g_buzzer.startup)
    {
        g_buzzer.startup = 0;
        buz->cycle_index = 0;
        buz->onoff_index = 0;
        g_buzzer.alarm_tck = ms2tck(buz->onoff_data[buz->onoff_index]);
    }
    else
    {
        /* if not timeout, just reduce tck*/
        /* if timeout, switch to next data, do cycle check*/
        if(g_buzzer.alarm_tck > 1)
        {
            g_buzzer.alarm_tck--;
            return;
        }

        /* increase onoff.index*/
        do
        {
        	if(buz->onoff_total!=0)
        	{
            		buz->onoff_index = (buz->onoff_index + 1) % buz->onoff_total;
            	}
		else
		{
			//除法错误：被除数为0
			buz->onoff_index=0;
		}
		g_buzzer.alarm_tck = ms2tck(buz->onoff_data[buz->onoff_index]);
        }
        while(g_buzzer.alarm_tck == 0);

        /* increase cycle.index, except endness cycle*/
        if((buz->onoff_index == 0) && (buz->cycle_total > 0))
        {
            buz->cycle_index++;
            if(buz->cycle_index >= buz->cycle_total)
            {
                /*printf("\n");*/
                stop_buzzer(buz->priority);
                return;
            }
        }
    }

    /* switch buzzer from on/off to off/on */
    if(buz->onoff_index % 2 == 0)   /* 偶数次 */
    {
	 /* pwm on */
	 //buz->frequency+=10;//for test
	 if(buz->frequency!=BUZZER_CHORD_FREQUENCY)
	 {
	 	buzzer_chord_player_stop();
	 	HalPwmStart(&BuzUnit,(U8)BUZZER_CHN, buz->frequency, 93*0xff/100);
	 }
	 else
	 {
		if(buz->tone!=NULL)
		{
			if(g_buzzer.alarm_tck == ms2tck(buz->onoff_data[buz->onoff_index]))
			{
				HalPwmStop(&BuzUnit,PPG_PIN_LOW);
				buzzer_chord_player_play(buz->tone, 0);
			}
		}
	}
    }
    else
    {
    	/* pwm off */
    	if(buz->frequency!=BUZZER_CHORD_FREQUENCY)
    	{
    		HalPwmStop(&BuzUnit,PPG_PIN_LOW);
    	}
    	else
    	{
    		buzzer_chord_player_stop();
    	}
    }
}



void init_buzzer(void)
{
	g_buzzer.startup = 0;
	g_buzzer.shutdown = 0;
	g_buzzer.current = 0;
	g_buzzer.alarm_tck = 0;
	g_buzzer.priority_bits = 0;
	memset(g_buzzer.list, 0, sizeof(g_buzzer.list));
	HalPwmInit(&BuzUnit,BUZZER_CHN);
}

/* init the process function */
void buz_init_beep_val(short buz_name,buzzer* buz)
{
	if (buz_name >= BEEPCOUNT)
	{
		dbg_string("Too many beeps than expect\n");
		return;
	}
	if(buz != NULL)
	{
		g_buzzer.list[buz_name] = buz;
	}
}

void buzzer_start_test(U16 frequency,U8 duty)
{
	HalPwmStart(&BuzUnit,(U8)BUZZER_CHN,frequency , duty);	
}

void buzzer_start_fine_test(U16 frequency,U16 duty)
{
	HalPwmFineStart(&BuzUnit,(U8)BUZZER_CHN,frequency , duty);	
}

void buzzer_start_pwm(void)
{
	PPGConfig_ChannelControl_Start(&BuzUnit);	//启动PWM
}

void buzzer_change_pwm_duty(void)
{
	PPGConfig_ChannelPCSR(&BuzUnit,BuzUnit.nPCSR);		//设置PCSR（周期）
	PPGConfig_ChannelPDUT(&BuzUnit,BuzUnit.nPDUT);		//设置PDUT（占空比）
}

void buzzer_stop_test(void)
{
	HalPwmStop(&BuzUnit,PPG_PIN_LOW);
}

//chord part
#define CHORD_NUM 4								//number of chord
#define CHORD_WAVE_TABLE_LENGTH 1024				//tone wave table length 
#define CHORD_WAVE_TABLE_SUSTRAIN_LENGTH 128	//tone wave table sustrain part length
#define CHORD_DELAY 10							//chord delay factor, the bigger of this value, the slower will tone decay
#define CHORD_VALUE_LIMIT 120					//chord PWM output limit +-CHORD_VALUE_LIMIT			


//chord structure
typedef struct 
{
  	U16 step_value;      		//phase shift per tick
  	U32 phase_sum;          	//accumulate phase shift
  	U8 loop;	  				//frequency circle count
  	U8 envelop_count;			//envelop circle count
 }Chord;

//chord player structure
typedef struct 
{
	Chord chords[CHORD_NUM];		//chord collection
	const U8 *tone;				//tone 
	U32 time;					//player timer, tick as unit
	U8 chord_index;				//next chord index
	U16 tone_index;				//next tone index
	U32 note_time_update;			//next note update tick
	Bool is_running;			//player running status
 }Chord_Player;

//tone library (music box)
static const  S8 wave_table[]= 
{
//attack area
  0, 0, 0, 0, 0, 0, -1, -2, -2, -3, -2, -2, -1, 0, 0, 0,    
  0, 0, -1, -2, -3, -3, -4, -4, -3, -2, -1, 0, 0, 1, 0, 0,
  0, -1, -2, -3, -3, -2, 0, 0, 2, 4, 5, 5, 5, 3, 1, 0,
  -2, -3, -4, -3, -2, 0, 1, 2, 3, 2, 0, -2, -6, -11, -15, -18,
  -19, -19, -16, -11, -5, 1, 8, 15, 20, 23, 24, 23, 20, 17, 13, 10,
  7, 6, 6, 8, 10, 13, 16, 18, 20, 20, 20, 19, 18, 18, 18, 18,
  19, 21, 24, 26, 27, 28, 27, 25, 22, 17, 11, 5, 0, -5, -9, -12,
  -13, -13, -11, -9, -5, -1, 1, 4, 6, 6, 4, 0, -5, -12, -21, -30,
  -39, -48, -56, -62, -66, -69, -70, -71, -70, -70, -70, -71, -72, -75, -77, -79,
  -80, -78, -75, -69, -61, -50, -38, -26, -13, -2, 7, 15, 21, 25, 28, 30,
  33, 36, 40, 44, 49, 55, 59, 61, 62, 60, 56, 49, 42, 34, 27, 22,
  20, 21, 25, 33, 42, 52, 63, 72, 80, 86, 89, 90, 89, 87, 84, 81,
  79, 77, 75, 73, 72, 69, 66, 61, 55, 48, 39, 31, 22, 14, 6, 0,
  -5, -10, -14, -17, -20, -22, -24, -26, -28, -29, -30, -31, -32, -33, -34, -35,
  -36, -37, -39, -41, -44, -48, -52, -57, -63, -70, -77, -85, -94, -102, -110, -116,
  -121, -124, -124, -121, -116, -109, -99, -89, -78, -68, -60, -53, -48, -45, -43, -43,
  -43, -42, -41, -40, -37, -33, -29, -25, -22, -19, -17, -16, -15, -14, -12, -9,
  -4, 2, 11, 22, 34, 46, 57, 67, 75, 81, 84, 86, 87, 86, 86, 85,
  85, 85, 86, 86, 87, 86, 84, 82, 78, 73, 69, 64, 60, 58, 56, 57,
  59, 62, 66, 70, 75, 79, 83, 86, 89, 91, 92, 92, 92, 90, 86, 81,
  73, 63, 52, 38, 24, 9, -5, -19, -31, -42, -50, -57, -61, -64, -66, -67,
  -68, -68, -68, -67, -66, -64, -61, -56, -52, -46, -41, -36, -32, -29, -28, -28,
  -30, -34, -39, -44, -50, -56, -62, -68, -74, -79, -84, -90, -95, -100, -104, -107,
  -108, -108, -107, -103, -98, -92, -84, -76, -67, -58, -49, -40, -32, -24, -16, -8,
  -1, 5, 11, 16, 20, 22, 22, 21, 18, 14, 10, 5, 1, -1, -3, -3,
  -2, 0, 1, 5, 9, 12, 16, 20, 24, 28, 34, 40, 47, 55, 64, 72,
  81, 89, 96, 101, 105, 108, 108, 108, 107, 105, 103, 101, 100, 98, 96, 93,
  90, 87, 83, 79, 75, 70, 66, 62, 59, 56, 55, 53, 52, 51, 50, 48,
  46, 44, 42, 39, 37, 34, 31, 28, 24, 20, 15, 10, 3, -3, -12, -21,
  -30, -39, -48, -57, -65, -72, -79, -85, -90, -95, -99, -103, -106, -109, -110, -112,
  -112, -112, -111, -110, -109, -107, -106, -105, -104, -104, -103, -102, -101, -99, -97, -94,
  -90, -86, -82, -78, -75, -72, -69, -67, -65, -63, -61, -58, -54, -50, -44, -38,
  -31, -23, -16, -10, -4, 1, 6, 10, 14, 18, 23, 27, 32, 38, 43, 49,
  54, 59, 63, 67, 70, 73, 75, 77, 79, 81, 83, 85, 88, 91, 93, 96,
  99, 101, 103, 105, 107, 109, 110, 111, 112, 112, 112, 111, 109, 107, 104, 100,
  96, 92, 88, 83, 79, 75, 70, 65, 60, 55, 49, 43, 36, 30, 24, 18,
  12, 7, 3, 0, -3, -6, -8, -11, -14, -17, -20, -24, -28, -32, -37, -42,
  -46, -51, -55, -59, -63, -67, -71, -75, -79, -82, -85, -88, -91, -93, -95, -97,
  -98, -100, -101, -102, -103, -104, -104, -104, -103, -101, -99, -96, -93, -89, -86, -83,
  -80, -78, -76, -75, -74, -73, -72, -71, -70, -68, -65, -63, -59, -56, -52, -48,
  -44, -40, -35, -30, -24, -18, -11, -4, 3, 11, 19, 27, 34, 41, 48, 53,
  58, 63, 67, 71, 75, 79, 82, 86, 89, 92, 94, 95, 96, 97, 98, 98,
  99, 100, 101, 103, 105, 108, 110, 112, 114, 115, 115, 114, 113, 110, 107, 103,
  99, 95, 91, 87, 83, 79, 74, 70, 65, 59, 53, 47, 41, 35, 29, 24,
  19, 14, 11, 7, 4, 1, -1, -5, -9, -14, -19, -25, -31, -37, -43, -50,
  -56, -62, -68, -73, -78, -83, -88, -92, -97, -101, -105, -109, -112, -115, -117, -119,
  -120, -121, -120, -119, -118, -116, -114, -112, -109, -106, -104, -101, -99, -96, -94, -91,
  -88, -85, -81, -78, -74, -70, -66, -62, -57, -53, -49, -45, -41, -37, -32, -27,
  -22, -16, -10, -4, 1, 8, 14, 20, 25, 31, 36, 41, 46, 50, 55, 60,
  64, 69, 73, 77, 81, 85, 88, 90, 93, 95, 97, 99, 101, 103, 106, 109,
  112, 115, 118, 121, 123, 125, 126, 126, 125, 125, 123, 121, 119, 117, 115, 112,
  110, 107, 103, 99, 94, 89, 83, 76, 69, 62, 54, 47, 39, 31, 24, 16,
  9, 2, -4, -11, -18, -25, -31, -38, -44, -51, -57, -62, -68, -73, -79, -84,
  -88, -93, -97, -102, -106, -109, -113, -116, -119, -121, -123, -124, -125, -126, -127, -127,
  -127, -127, -126, -125, -125, -123, -122, -120, -118, -116, -114, -111, -107, -104, -100, -95,
  -91, -86, -80, -75, -69, -63, -58, -52, -46, -40, -34, -28, -22, -16, -10, -4,
//sustain area
  0, 5, 11, 17, 23, 28, 34, 39, 45, 50, 55, 60, 65, 69, 74, 78,        
  82, 85, 89, 92, 95, 98, 100, 102, 104, 106, 107, 109, 109, 110, 110, 111,
  110, 110, 109, 108, 107, 106, 104, 102, 100, 98, 95, 93, 90, 87, 83, 80,
  76, 72, 68, 64, 59, 55, 50, 46, 41, 36, 31, 26, 21, 15, 10, 5,
  0, -5, -10, -15, -21, -26, -31, -36, -41, -46, -50, -55, -59, -64, -68, -72,
  -76, -80, -83, -87, -90, -93, -95, -98, -100, -102, -104, -106, -107, -108, -109, -110,
  -110, -111, -110, -110, -109, -109, -107, -106, -104, -102, -100, -98, -95, -92, -89, -85,
  -82, -78, -74, -69, -65, -60, -55, -50, -45, -39, -34, -28, -23, -17, -11, -5
};

//envelop library
static const  U8 envelope[]= 
{
   255,252,250,247,245,243,240,238,235,233,231,228,226,224,222,219,
   217,215,213,211,209,207,205,203,201,199,197,195,193,191,189,187,
   185,183,182,180,178,176,174,173,171,169,168,166,164,163,161,159,
   158,156,155,153,152,150,149,147,146,144,143,141,140,139,137,136,
   134,133,132,130,129,128,127,125,124,123,122,120,119,118,117,116,
   115,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,
   98,97,96,95,94,93,92,91,90,89,88,87,87,86,85,84,
   83,82,82,81,80,79,78,78,77,76,75,75,74,73,72,72,
   71,70,69,69,68,67,67,66,65,65,64,64,63,62,62,61,
   60,60,59,59,58,57,57,56,56,55,55,54,54,53,53,52,
   51,51,50,50,49,49,48,48,48,47,47,46,46,45,45,44,
   44,43,43,43,42,42,41,40,40,39,39,38,38,37,37,36,
   35,35,34,34,33,33,32,31,31,30,30,29,29,28,28,27,
   26,26,25,25,24,24,23,22,22,21,21,20,20,19,19,18,
   17,17,16,16,15,15,14,13,13,12,12,11,11,10,10,9,
   8,8,7,7,6,6,5,4,4,3,3,2,2,1,1,0
};

//8K Hz phase shift table
/*
static const U16 pitch_table[] =
{
//  A     B     H     C    Cis    D    Dis    E     F    Fis    G    Gis 
   899,  952,  1010, 1071, 1133, 1202, 1276, 1350, 1432, 1513, 1604, 1698,    //220Hz..
   1800, 1907, 2021, 2140, 2271, 2402, 2546, 2697, 2857, 3029, 3209, 3402,    //440Hz..
   3602, 3820, 4041, 4286, 4536, 4811, 5093, 5396, 5716, 6056, 6420, 6801,     //880Hz..
   7207, 7633, 8092, 8571, 9083, 9624, 10193,10799,11442,12122,12843,13605,    //1760Hz..
   14416,15272,16181,17144,18168,19245,20392, 21600			       //3520Hz..
};
*/

//16K Hz phase shift table
/*
const U16 pitch_table[] =
{
//  A     B     H     C    Cis    D    Dis    E     F    Fis    G    Gis 
   451,  477,  506,  536,  568,  601,  637,  675,  715,  758,  803,  851,    		//220Hz..
   899,  952,  1010, 1071, 1133, 1202, 1276, 1350, 1432, 1513, 1604, 1698,    //440Hz..
   1800, 1907, 2021, 2140, 2271, 2402, 2546, 2697, 2857, 3029, 3209, 3402,    //880Hz..
   3602, 3820, 4041, 4286, 4536, 4811, 5093, 5396, 5716, 6056, 6420, 6801,     //1760Hz..
   7207, 7633, 8092, 8571, 9083, 9624, 10193,10799,11442,12122,12843,13605    //3520Hz..
};
*/

//32K Hz phase shift table

const U16 pitch_table[] =
{
//  A     B     H     C    Cis    D    Dis    E     F    Fis    G    Gis 
   225,  239,  253,  268,  284,  301,  319,  338,  358,  379,  401,  425,    //220Hz..
   451,  477,  506,  536,  568,  601,  637,  675,  715,  758,  803,  851,    //440Hz..
   901,  955, 1011, 1072, 1135, 1203, 1274, 1350, 1430, 1515, 1606, 1701,    //880Hz..
   1802, 1909, 2023, 2143, 2271, 2406, 2549, 2700, 2861, 3031, 3211, 3402,   //1760Hz..
   3604, 3818, 4046, 4286, 4542, 4812, 5098, 5400			                 //3520Hz..
};


static Chord_Player chord_player;	//播放器实例

static void buzzer_chord_init(Chord *note,U16 step);
static Bool buzzer_chord_update_beat(void);
static Bool buzzer_chord_update_next_note(void);
static void buzzer_chord_update_chords_with_note(U8 note_number);
static void buzzer_chord_update_beat_time(void);
static Bool buzzer_chord_player_pause(void);
static Bool buzzer_chord_player_resume(void);

//chord initializer
//chord: pointer to the chord need to be initialized
//step: initial value for chord
static void buzzer_chord_init(Chord *chord,U16 step)
{
	if(chord==NULL)
	{
		return;
	}
		
	chord->step_value=step;
 	chord->phase_sum=0;
 	chord->loop=0;
 	chord->envelop_count=0;
}

//chord player clear
static void buzzer_chord_player_clear(void)
{
	U32 i;
	for(i=0;i<CHORD_NUM;i++)
	{
     		buzzer_chord_init(&chord_player.chords[i],0);
   	}
	chord_player.time=0;
	chord_player.chord_index=0;
	chord_player.tone_index=0;
	chord_player.note_time_update=0;
	chord_player.is_running=FALSE;
}

//chord player initializer
void buzzer_chord_player_init(void)
{
	buzzer_chord_player_clear();
 	chord_player.tone=NULL;
}

//check if chord player is running
//if chord player is running,TRUE will be return
Bool buzzer_chord_player_is_running(void)
{
	return chord_player.is_running;
}

//chord player stop
void buzzer_chord_player_stop(void)
{
	hal_freeruntimer_stop(BUZZER_CHN);
	buzzer_chord_player_clear();
}

//chord player pause
//if chord player is not running,FALSE will be return
static Bool buzzer_chord_player_pause(void)
{
	if(!chord_player.is_running)
	{
		return FALSE;
	}
	hal_freeruntimer_stop(BUZZER_CHN);
	chord_player.is_running=FALSE;
	
	return TRUE;
}

//chord player resume
//if chord player is already running or there is no tone , FALSE will be return
static Bool buzzer_chord_player_resume(void)
{
	if((chord_player.is_running)||(chord_player.tone==NULL))
	{
		return FALSE;
	}
	chord_player.is_running=TRUE;
	hal_freeruntimer_start(BUZZER_CHN);
	return TRUE;
}

//chord player play
//tone: the melody want to play
//if chord player is already running or tone is NULL, FALSE will be return
Bool buzzer_chord_player_play(const U8 *tone,U32 start_time)
{
	if(tone!=chord_player.tone)
	{
		buzzer_chord_player_stop() ;
	}

	buzzer_chord_player_init();
	chord_player.tone=tone;
	chord_player.time=start_time;
	chord_player.is_running=TRUE;
	hal_freeruntimer_Init(BUZZER_CHN, 31);
	buzzer_start_fine_test(BUZZER_CHORD_FREQUENCY,BUZZER_CHORD_MIDDLE);
	buzzer_change_pwm_duty();
	buzzer_start_pwm();
	hal_freeruntimer_start(BUZZER_CHN);
	return TRUE;
	
}

//chord player step play
//result:PWM ratio transfer to PPG
//if chord player is not running or there is no tone or the end of tone reached, FALSE will be return
void buzzer_chord_player_play_one_step(void)
{
	U32 i,tempU32;
	S32 tempS32;
	S8 tempS8;
	U16 tempU16;
	if((!chord_player.is_running)||(chord_player.tone==NULL))
	{
		buzzer_chord_player_stop();
		return;
	}

	buzzer_change_pwm_duty();
	
	chord_player.time++;
	if(chord_player.time>chord_player.note_time_update)
	{
		if(!buzzer_chord_update_beat())
		{
			buzzer_chord_player_stop();
			return;
		}
	}
	
	tempS32=0;
	for(i=0;i<CHORD_NUM;i++)
	{
		Chord *note=&chord_player.chords[i];
		note->phase_sum+=note->step_value;
		
		if((note->phase_sum)>=(CHORD_WAVE_TABLE_LENGTH<<8))
		{
			note->phase_sum-=CHORD_WAVE_TABLE_SUSTRAIN_LENGTH<<8;
			note->loop++;
			if(note->loop==CHORD_DELAY)
			{
				note->loop=0;
				if(note->envelop_count<255)
				{
					note->envelop_count++;
				}
			}
		}
		
		tempU32=(note->phase_sum)>>8;
		tempS8=wave_table[tempU32];
		tempS32+=tempS8*envelope[note->envelop_count];
	}
	tempS32=tempS32>>2;
	if(tempS32>=32000)
	{
		tempS32=32000;
	}
	else if(tempS32<=-32000)
	{
		tempS32=-32000;
	}
	

	tempS32+=32768;
	tempU16=(U16)tempS32;

	buzzer_start_fine_test(BUZZER_CHORD_FREQUENCY,tempU16);
	
	return;
}

//update next beat
//if the end of tone reached, return FALSE
static Bool buzzer_chord_update_beat(void)
{
	buzzer_chord_update_beat_time();
	if(!buzzer_chord_update_next_note())
	{
		return FALSE;
	}
	return TRUE;
}

//update next note
//if the end of tone reached, return FALSE
static Bool buzzer_chord_update_next_note(void)
{
	U8 next_note;
	do
	{
		next_note=chord_player.tone[chord_player.tone_index++];
	}while(!(next_note&CHORD_EN));

	if(next_note==CHORD_EoS)
	{
		return FALSE;
	}

	next_note-=CHORD_EN;
	buzzer_chord_update_chords_with_note(next_note);
	return TRUE;
}

//update chords in chord player with note
static void buzzer_chord_update_chords_with_note(U8 note)
{
  	U16 step_value;
  	Chord *chord;
  	step_value=pitch_table[note];
	chord=&chord_player.chords[chord_player.chord_index];

	buzzer_chord_init(chord, step_value);
  	
  	chord_player.chord_index++;
 	if(chord_player.chord_index==CHORD_NUM)
 	{
   		chord_player.chord_index=0;
 	}
}

//update beat time
static void buzzer_chord_update_beat_time(void)
{
	U32 next_time;
	next_time=chord_player.tone[chord_player.tone_index++]*256;
	next_time+=chord_player.tone[chord_player.tone_index++]*256*256;
	chord_player.note_time_update=next_time;
}

