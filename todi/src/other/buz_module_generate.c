#include "string.h"
#include "stdio.h"
#include "stdlib.h"

typedef unsigned char U8;
typedef unsigned short U16;

int main()
{
	U8 str[100] = {0};
	U8 buz_name[100]= {0};
	U8  ch;
	FILE *fp;
	fp = fopen("buz.txt","w+");
	if(fp == NULL)
	{
		printf("open file err\n");
	}
	else
	{
	 do
	 {
        printf("please input buz fuc name:\n");
		gets(str);
		printf("please input buz enum name:\n");
	    gets(buz_name);
	    fprintf(fp,"/*\n================================================================================\n");
	    fprintf(fp,"**\tname   \t%s\n",str);
	    fprintf(fp,"**\tbrief\n");
	    fprintf(fp,"**\tinput\n");
        fprintf(fp,"**\toutput\n");
        fprintf(fp,"**\tversion\t 0.1\n");
        fprintf(fp,"**\tdata\n");
        fprintf(fp,"**\tnotes\n");
        fprintf(fp,"================================================================================\n");
		fprintf(fp,"*/\n");
        if(str != NULL)
		{
            fprintf(fp,"void %s(U16 action)\n",str);
            fprintf(fp,"{\n");
			fprintf(fp,"\tstatic U16 state = 0;\n");
			fprintf(fp,"\tU16 new_state = 0;\n\n");
			fprintf(fp,"\tswitch(action)\n");
			fprintf(fp,"\t{\n");
			fprintf(fp,"\t\tcase ACT_RESET:\n");
			fprintf(fp,"\t\t\tstate = 0;\n");
			fprintf(fp,"\t\t\tbreak;\n");
			fprintf(fp,"\t\tcase ACT_TOUCH:\n");
			fprintf(fp,"\t\t\tnew_state = buz_sts[%s];\n",buz_name);
			fprintf(fp,"\t\t\ttouch_switch_trigger(%s, &state, new_state);\n",str);
			fprintf(fp,"\t\t\tbreak;\n");
			fprintf(fp,"\t\tcase ACT_TRUE:\n");
			fprintf(fp,"\t\t\tstart_buzzer(%s);\n",buz_name);
			fprintf(fp,"\t\t\tbreak;\n");
			fprintf(fp,"\t\tcase ACT_FALSE:\n");
			fprintf(fp,"\t\t\tstop_buzzer(%s);\n",buz_name);
			fprintf(fp,"\t\t\tbreak;\n");
			fprintf(fp,"\t\tcase ACT_TIMER:\n");
			fprintf(fp,"\t\t\t/* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,\n\t\t\t this will handle the MSG_TIMER every 1000ms interval */\n"); 
			fprintf(fp,"\t\t\t break;\n");
			fprintf(fp,"\t\tdefault:break;\n");
			fprintf(fp,"\t}\n");
			fprintf(fp,"}\n");
			printf(" if you want to continue, please precss y\n");
            if('y' == getch())
            {
               break;
            }			
		}	
       }while(1);
	}
	fclose(fp);
	printf("code geberate finish,please check buz.txt\n");
	system("PAUSE");
}
