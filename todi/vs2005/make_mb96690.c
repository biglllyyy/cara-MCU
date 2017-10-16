#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *fpIN = NULL;
	FILE *fpOUT = NULL;
	char rowbuf[80];
	
	do
	{
		fpIN = fopen("mb91520.h", "r");
		if(fpIN == NULL) 
       {
            printf("open mb91520.h err\n");
            break;
       }
		
		fpOUT = fopen("mb91520.c", "w");
		if(fpOUT == NULL) break;
		
		fprintf(fpOUT,
			"#include \"mb91520.h\"\n"
			"\n"
			"void __DI(void) {}\n"
			"void __EI(void) {}\n"
			"void __wait_nop(void) {}\n"
			"void __set_il(int il) {(void)il;}\n"
			"\n"
		);
		
		while(fgets(rowbuf, 80, fpIN))
		{
			if(strncmp(rowbuf, "__IO_EXTERN", 11) == 0)
			{
				fprintf(fpOUT, "/*__IO_EXTERN*/ %s", rowbuf + 12);
			}
			else if(strncmp(rowbuf, "__IO_EXTENDED", 13) == 0)
			{
				fprintf(fpOUT, "/*__IO_EXTENDED*/ %s", rowbuf + 14);
			}
		}
		
	} while(0);
	
	if(fpIN) fclose(fpIN);
	if(fpOUT) fclose(fpOUT);
    system("PAUSE");
	return 0;
}
