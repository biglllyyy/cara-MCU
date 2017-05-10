;**************************************************************************
;
;    Startup file for memory and basic controller initialisation
;
;**************************************************************************
; 1  Copyright
;**************************************************************************
;  FR81S Family Template Project V01L02
;  ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU SEMICONDUCTOR LIMITED 2012
;  LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU SEMICONDUCTOR LIMITED
;--------------------------------------------------------------------------

          .PROGRAM  STARTUP
          .TITLE    "STARTUP FILE FOR MEMORY INITIALISATION"

;**************************************************************************
; 2  Contents
;**************************************************************************
;
; 1  Copyright
;
; 2  Contents
;
; 3  Information
;
; 4  Settings
; 4.1  Stack Type and Stack Size
; 4.2  Clock Selection and clock Stabilization Time
; 4.3  Flash Security
; 4.4  Watchdog timer
; 4.5  Initialization of processor status
; 4.6  NMI activation
; 4.7  Enable RAMCODE Copying
; 4.8  C++ Language Use
;
; 5  Section and Data Declaration
; 5.1  Set Flash Security
; 5.2  Stack area and stack top definition/declaration
; 5.3  Set _INROM2(Work FLASH)
;
; 6  Start-Up Code
; 6.1  Import external symbols
; 6.2  Program start (the reset vector should point here)
; 6.3  Initialization of processor status
; 6.4  Prepare stacks and set the default stack type
; 6.5  NMI activation(SET ILM)
; 6.6  The setting of Flash is initialized.
; 6.7  activate MDI port ( for Extarnal OCD tools)
; 6.8  Set clock ratio (ignore subclock) and wait for clocks to stabilize
; 6.9  Copy initial values to data areas.
; 6.10 Clear uninitialized data areas to zero
; 6.11 Watchdock timer setting and reset
; 6.12 Call C-language main function
; 6.13 Program end loop
;
; 7  Revision and History

;**************************************************************************
; 3  Information
;**************************************************************************
; @file   : startup.asm
; @target : FR81S Family MB915xx series

;**************************************************************************
; 4  Settings
;**************************************************************************
;
; CHECK ALL OPTIONS WHETHER THEY FIT TO THE APPLICATION
;
; Configure this startup file in the "Settings" section. Search for
; comments with leading "; <<<". This points to the items to be set.
;--------------------------------------------------------------------------
#define OFF	0
#define ON	1

;**************************************************************************
; 4.1 Stack Type and Stack Size
;**************************************************************************
#define STACK_SIZE		4*1024-4	; <<< byte size of System stack

;**************************************************************************
; 4.2  Clock Selection and clock Stabilization Time
;**************************************************************************
;--------------------------------------------------------------------------
; Definitions related to the setting of the clock
;--------------------------------------------------------------------------
;--------------------------------------------------------------------------
; Address related to the setting of the clock
;--------------------------------------------------------------------------
#define DIVR0_ADDR			0x488		; DIVide clock configuration Register 0
#define CSELR_ADDR			0x510		; Clock source SELect Register
#define PLLCR_ADDR			0x514		; PLLCR register address
#define CCPSSELR_ADDR		0x520		; CCtl Pll/Sscg clock SELect Register
#define CCPSDIVR_ADDR		0x523		; CCtl Pll/Sscg clock DIVide Register
#define CCPLLFBR_ADDR		0x525		; CCtl PLL/FB clock divide Register
#define CCCGRCR0_ADDR		0x52D		; CCtl Clock Gear Config. Register 0
#define CCCGRCR1_ADDR		0x52E		; CCtl Clock Gear Config. Register 1
#define CCCGRCR2_ADDR		0x52F		; CCtl Clock Gear Config. Register 2
#define SACR_ADDR			0x1000		; Sync/Async Control Register
;--------------------------------------------------------------------------
; Set mask/value of the register
;--------------------------------------------------------------------------
#define DIVR_DIVB_DIVP_MASK	0x1FFF0FFF	; Source Clock => PLL Clock
#define CSELR_CKS_MASK		0xFCFFFFFF	; PLL CLK
#define CSELR_PCEN_SET		0x40000000	; Source Clock => PLL Clock
#define CSELR_CKS_PLL		0x02000000	; Check Main Clock/2
#define CMONR_MCRDY_ON		0x00200000	; Check PLL Clock
#define CMONR_PCRDY_ON		0x00400000	; Source Clock => PLL Clock (wait transition)
#define CMONR_CKM_PLL		0x00020000	; Source Clock => Main Clock/2 (wait transition)
#define CMONR_CKM_MCLK		0x00030000	; Source Clock => Main Clock/2 (wait transition)
#define PLLCR_POSW_MASK		0xFF0FFFFF	; wait for clocks to stabilize
#define CCPSSELR_PCSEL_PLL	0xFE		; use PLL Clock
#define CCCGRCR0_GREN_SET	0x01		; Clock Gear CCGCRCR0_GREN
#define CCCGRCR0_GRSTR_SET	0x02		; Clock Gear CCGCRCR0_GRSTR
#define CCCGRCR0_GRSTS00_MASK	0xC0	; Clock Gear CCGCRCR0_GRSTS00
#define CCCGRCR0_GRSTS10_MASK	0x80	; Clock Gear CCGCRCR0_GRSTS10
#define CCCGRCR1_STEP_SET	0x00		; GRSTP:1(b'00)+SRSTN:0(b'00_0000)
#define CCCGRCR2_STEP_SET	0xFF		; GRLP:256(b'1111_1111)
#define SACR_M_MASK			0x01		; PCLK2 M (syncronous)
;--------------------------------------------------------------------------
; Clock Stabilization Time
;--------------------------------------------------------------------------
#define PLLCR_POSW_09_POW_OF_2_CLK	0x00800000	; 2^9  Clock
#define PLLCR_POSW_10_POW_OF_2_CLK	0x00900000	; 2^10 Clock
#define PLLCR_POSW_11_POW_OF_2_CLK	0x00A00000	; 2^11 Clock
#define PLLCR_POSW_12_POW_OF_2_CLK	0x00B00000	; 2^12 Clock
#define PLLCR_POSW_13_POW_OF_2_CLK	0x00C00000	; 2^13 Clock
#define PLLCR_POSW_14_POW_OF_2_CLK	0x00D00000	; 2^14 Clock
#define PLLCR_POSW_15_POW_OF_2_CLK	0x00E00000	; 2^15 Clock
#define PLLCR_POSW_16_POW_OF_2_CLK	0x00F00000	; 2^16 Clock
;--------------------------------------------------------------------------
; Multiplication ratio
;--------------------------------------------------------------------------
#define CCPLLFBR_DIVIDER_RATIO_20	0x13	; Multiplication ratio(x20)
#define CCPLLFBR_DIVIDER_RATIO_32	0x1F	; Multiplication ratio(x32)
#define CCPLLFBR_DIVIDER_RATIO_40	0x27	; Multiplication ratio(x40)
;--------------------------------------------------------------------------
; PLL Macro Clock DIVide ratio
;--------------------------------------------------------------------------
#define PODS_PLL_MACRO_DIV_02 0x00	;DIVide ratio(1/2)
#define PODS_PLL_MACRO_DIV_04 0x11	;DIVide ratio(1/4)
;--------------------------------------------------------------------------
; PCLK DIVide ratio
; CPU Clock -> Dividing Ratio for PCLK
;--------------------------------------------------------------------------
#define DIVP_PLL_DIV_02		0x00001000	; DIVide ratio(1/2)
#define DIVP_PLL_DIV_04		0x00003000	; DIVide ratio(1/4)
;--------------------------------------------------------------------------
; Clock setting
; Difinition of CPU Clock, PCLK, and Clock Stabilization Time
; An appropriate parameter is selected from "Definition related to the setting of the clock" according to the following expressions.
; CPU Clock = Main Clock(MCLK) * Multiplication ratio / DIVide ratio
;
; Clock setting summary
; +----+                  +---------+                                      +----+
; |MCLK|-->PLLCR:PDS--+-->|PLL Macro|-->CCPSDIVR:PODS--+------------------>|CCLK| CPU Clock
; +----+     (1/1)    |   +---------+      (1/n)       |                   +----+
; MCLK           CCPLLFBR:IDIV          Divide Ratio   |                   +-----+
;                     n                                +-->DIVR2.DIVP--+-->|PCLK1| PCLK
;                Multiplication ratio                        (1/n)     |   +-----+
;                                                 Divide Ratio for PCK |   +-----+
;                                                                      +-->|PCLK2|
;                                                                     sync +-----+
;--------------------------------------------------------------------------
; Clock Setting
;--------------------------------------------------------------------------
; MCLK selection ON:enable, OFF:disable
#define MCLK_4MHZ				ON	; <<< Turn ON this parameter when CLK is 4MHz.
#define MCLK_8MHZ				OFF	; <<< Turn ON this parameter when CLK is 8MHz.
; CPU & Sub Clock selection ON:enable, OFF:disable
#define CLOCK_SETTING_THROUGH	ON	; <<< When this is turned on, four of the following are invalid.
#define CPU_80MHZ_PCLK_40MHZ	OFF 	; <<< Turn ON this parameter when CPU clock is 80MHz and sub click is 40MHz.
#define CPU_80MHZ_PCLK_20MHZ	OFF	; <<< Turn ON this parameter when CPU clock is 80MHz and sub click is 20MHz.
#define CPU_64MHZ_PCLK_32MHZ	OFF	; <<< Turn ON this parameter when CPU clock is 64MHz and sub click is 32MHz.
#define CPU_40MHZ_PCLK_20MHZ	OFF	; <<< Turn ON this parameter when CPU clock is 40MHz and sub click is 20MHz.
#define CLOCK_OSC_WAIT		PLLCR_POSW_15_POW_OF_2_CLK			; <<< Clock Stabilization Time

;--------------------------------------------------------------------------
; Clock pattern setting
;--------------------------------------------------------------------------
#if MCLK_4MHZ == ON
#if CPU_80MHZ_PCLK_40MHZ == ON
#define CLOCK_MULTIPLY			CCPLLFBR_DIVIDER_RATIO_40	; Multiplication ratio(x40)
#define CLOCK_PLL_DIVIDE		PODS_PLL_MACRO_DIV_02		; DIVide ratio(1/2)
#define CLOCK_PCLK1_DIVIDE		DIVP_PLL_DIV_02				; DIVide ratio of PCLK(1/2)
#elif CPU_80MHZ_PCLK_20MHZ == ON
#define CLOCK_MULTIPLY			CCPLLFBR_DIVIDER_RATIO_40	; Multiplication ratio(x40)
#define CLOCK_PLL_DIVIDE		PODS_PLL_MACRO_DIV_02		; DIVide ratio(1/2)
#define CLOCK_PCLK1_DIVIDE		DIVP_PLL_DIV_04				; DIVide ratio of PCLK(1/2)
#elif CPU_64MHZ_PCLK_32MHZ == ON
#define CLOCK_MULTIPLY			CCPLLFBR_DIVIDER_RATIO_32	; Multiplication ratio(x32)
#define CLOCK_PLL_DIVIDE		PODS_PLL_MACRO_DIV_02		; DIVide ratio(1/2)
#define CLOCK_PCLK1_DIVIDE		DIVP_PLL_DIV_02				; DIVide ratio of PCLK(1/2)
#else ; CPU_40MHZ_PCLK_20MHZ == ON
#define CLOCK_MULTIPLY			CCPLLFBR_DIVIDER_RATIO_20	; Multiplication ratio(x20)
#define CLOCK_PLL_DIVIDE		PODS_PLL_MACRO_DIV_02		; DIVide ratio(1/2)
#define CLOCK_PCLK1_DIVIDE		DIVP_PLL_DIV_02				; DIVide ratio of PCLK(1/2)
#endif
#else ; MCLK_8MHZ == ON
#if CPU_80MHZ_PCLK_40MHZ == ON
#define CLOCK_MULTIPLY			CCPLLFBR_DIVIDER_RATIO_20	; Multiplication ratio(x20)
#define CLOCK_PLL_DIVIDE		PODS_PLL_MACRO_DIV_02		; DIVide ratio(1/2)
#define CLOCK_PCLK1_DIVIDE		DIVP_PLL_DIV_02				; DIVide ratio of PCLK(1/2)
#elif CPU_80MHZ_PCLK_20MHZ == ON
#define CLOCK_MULTIPLY			CCPLLFBR_DIVIDER_RATIO_20	; Multiplication ratio(x20)
#define CLOCK_PLL_DIVIDE		PODS_PLL_MACRO_DIV_02		; DIVide ratio(1/2)
#define CLOCK_PCLK1_DIVIDE		DIVP_PLL_DIV_04				; DIVide ratio of PCLK(1/4)
#elif CPU_64MHZ_PCLK_32MHZ == ON
#define CLOCK_MULTIPLY			CCPLLFBR_DIVIDER_RATIO_32	; Multiplication ratio(x32)
#define CLOCK_PLL_DIVIDE		PODS_PLL_MACRO_DIV_04		; DIVide ratio(1/4)
#define CLOCK_PCLK1_DIVIDE		DIVP_PLL_DIV_02				; DIVide ratio of PCLK(1/2)
#else ; CPU_40MHZ_PCLK_20MHZ == ON
#define CLOCK_MULTIPLY			CCPLLFBR_DIVIDER_RATIO_20	; Multiplication ratio(x20)
#define CLOCK_PLL_DIVIDE		PODS_PLL_MACRO_DIV_04		; DIVide ratio(1/2)
#define CLOCK_PCLK1_DIVIDE		DIVP_PLL_DIV_02				; DIVide ratio of PCLK(1/2)
#endif
#endif

;**************************************************************************
; 4.3  Flash Security
;**************************************************************************
#define FLASH_SECURITY_ADDRESS	0x080000				; FLASH Security Area(Top of FLASH Area) Address
#define FLASH_SECURITY_ON		0x0001  			; FLASH Security ON
#define FLASH_SECURITY_OFF		0xFFFF  			; FLASH Security OFF

#define FLASH_SECURITY_SW		FLASH_SECURITY_OFF	; <<< FLASH security
;--------------------------------------------------------------------------
; OCD password (default OFF)
;--------------------------------------------------------------------------
#define PASS_WORD_LENGTH		0xFFFF	; <<< OCD password length
                         		      	; The password length is set to the lower 4 bits. When the lower 4 bits is 0x0 or 0xF, the security is disabled.
#define PASS_WORD_01			0xFFFF  ; <<< OCD password
#define PASS_WORD_02			0xFFFF  ; <<< OCD password
#define PASS_WORD_03			0xFFFF  ; <<< OCD password
#define PASS_WORD_04			0xFFFF  ; <<< OCD password
#define PASS_WORD_05			0xFFFF  ; <<< OCD password
#define PASS_WORD_06			0xFFFF  ; <<< OCD password
#define PASS_WORD_07			0xFFFF  ; <<< OCD password
#define PASS_WORD_08			0xFFFF  ; <<< OCD password
#define PASS_WORD_09			0xFFFF  ; <<< OCD password
#define PASS_WORD_10			0xFFFF  ; <<< OCD password
#define PASS_WORD_11			0xFFFF  ; <<< OCD password
#define PASS_WORD_12			0xFFFF  ; <<< OCD password
#define PASS_WORD_13			0xFFFF  ; <<< OCD password
#define PASS_WORD_14			0xFFFF  ; <<< OCD password

;**************************************************************************
; 4.4  Watchdog timer
;**************************************************************************
;--------------------------------------------------------------------------
; Watchdog timer setting register address
;--------------------------------------------------------------------------
#define WDTCR0_ADDR					0x003C	; WatchDog Timer Configuration Register 0
#define WDTCPR1_ADDR				0x003F	; WatchDog Timer Clear Pattern Register 1

#define WDTCPR1_CLEAR				0xA5	; <<< Watchdog timer 1 reset

;**************************************************************************
; 4.5  Initialization of processor status
;**************************************************************************
;--------------------------------------------------------------------------
; Base Pointer(BP) relative access
; If you specify a compiler option(-KBP), the following value is set to ON.
;--------------------------------------------------------------------------
;#define USE_BASE_POINTER_ACCESS	OFF	; <<< use base pointer(default OFF)

;--------------------------------------------------------------------------
; FPU Control Register exception permission flag setting
; The FPU exception to be detected is set to FCR_SET by the OR.
; For the device without FPU, set USE_FPU OFF.
; It is not necessary to set it to OFF.
; (FCR register is also equipped in the device without FPU.)
;--------------------------------------------------------------------------
#define FCR_EEF_D	0x00020000	; Denormalized number input exception detection
#define FCR_EEF_X	0x00010000	; Inaccurate exception detection
#define FCR_EEF_U	0x00008000	; Underflow exception detection
#define FCR_EEF_O	0x00004000	; Overflow exception detection
#define FCR_EEF_Z	0x00002000	; Zero division exception detection
#define FCR_EEF_V	0x00001000	; Invalid operation exception detection

#define USE_FPU		ON          ; <<< use FPU
#define FCR_SET		0x00000000	; <<< FPU exception permission flag
; All exceptions are permitted on (An alternative value is put when the exception is generated and operate).
; if you turn on all exceptions,
; #define FCR_SET		FCR_EEF_D | FCR_EEF_X | FCR_EEF_U | FCR_EEF_O | FCR_EEF_Z | FCR_EEF_V

;--------------------------------------------------------------------------
; Flash memory ECC flag clear
;--------------------------------------------------------------------------
#define FCTLR_ADDR			0x840			; Flash ConTroL Register
#define FSTR_ADDR			0x843			; Flash STatus Register
#define FCTLR_FAW_MASK		0xFCFFFFFF		; FLASH Access Wait(Mask)
#define FCTLR_FAW_0_CYC		0x00000000		; FLASH Access Wait(No wait)
#define FCTLR_FAW_1_CYC		0x01000000		; FLASH Access Wait(Wait 1 cycle)
#define FSTR_FECCER_CLEAR	0x01			; Flash ECC Error Clear

#define FLASH_WRITE_WAIT	FCTLR_FAW_0_CYC	; <<< FLASH Accrss Wait

;--------------------------------------------------------------------------
; activate MDI port ( for Extarnal OCD tools)
;--------------------------------------------------------------------------
#define DSUCR_ADDR		0xFF00		; DSU Control Register
#define DSUCR_DSU_MASK	0x0000FFFF	; DSU Flag mask
#define DSUCR_DSU_ON	0x00010000	; DSU Flag ON

;**************************************************************************
; 4.6  NMI activation
;**************************************************************************
;--------------------------------------------------------------------------
; Interupt Level
; In the case of FR81S, if even level interrupt is set, NMI is enabled.
;--------------------------------------------------------------------------
#define PS_ILM_16  		0x10		; Interrupt Level 16
#define PS_ILM_MIN 		0x1E		; Interrupt Level 31
#define PS_ILM_IMP		0x1F		; Interrupt impossible
#define PS_ILM_LEVEL	PS_ILM_16 	; <<< Interrup level

;**************************************************************************
; 4.7  Enable RAMCODE Copying
;**************************************************************************
;--------------------------------------------------------------------------
; If you transfer the code and run it in RAM,
; please rename the target code section to RAMCODE by writing #pragma section, #pragma segment.
;--------------------------------------------------------------------------
#define COPY_RAMCODE     OFF; <<< enable RAMCODE section to be copied from ROM to RAM

;**************************************************************************
; 4.8  C++ Language Use
;**************************************************************************
;--------------------------------------------------------------------------
; ON:C++ Language use; OFF:C++ Language not use, C Language use
;--------------------------------------------------------------------------
#define CPP_LANG     OFF	; <<< select C++ Language use or not

;**************************************************************************
; 5  Section and Data Declaration
; 5.1  Set Flash Security
;**************************************************************************
;--------------------------------------------------------------------------
; definition for flash ROM security area
;--------------------------------------------------------------------------
	.SECTION	FLASH_SECURITY, LOCATE=FLASH_SECURITY_ADDRESS
	.DATA.H		0x0000				; Because the FLASH security is from the third byte, any value is acceptable here.
	.DATA.H		FLASH_SECURITY_SW	; FLASH Security
	.DATA.H		PASS_WORD_LENGTH	; OCD Security Length
	.DATA.H		PASS_WORD_01		; OCD password 1
	.DATA.H		PASS_WORD_02		; OCD password 2
	.DATA.H		PASS_WORD_03		; OCD password 3
	.DATA.H		PASS_WORD_04		; OCD password 4
	.DATA.H		PASS_WORD_05		; OCD password 5
	.DATA.H		PASS_WORD_06		; OCD password 6
	.DATA.H		PASS_WORD_07		; OCD password 7
	.DATA.H		PASS_WORD_08		; OCD password 8
	.DATA.H		PASS_WORD_09		; OCD password 9
	.DATA.H		PASS_WORD_10		; OCD password 10
	.DATA.H		PASS_WORD_11		; OCD password 11
	.DATA.H		PASS_WORD_12		; OCD password 12
	.DATA.H		PASS_WORD_13		; OCD password 13
	.DATA.H		PASS_WORD_14		; OCD password 14

;**************************************************************************
; 5.2  Stack area and stack top definition/declaration
;**************************************************************************
	.SECTION	STACK, STACK, ALIGN=4
	.RES.B		STACK_SIZE
stack_top:
	.RES.W	1

;**************************************************************************
; 5.3  Set _INROM2(Work FLASH)
;**************************************************************************
	.SECTION	WORK_FLASH, CONST

;**************************************************************************
; 6  Start-Up Code
; 6.1  Import external symbols
;**************************************************************************
	.EXPORT	__start
	.EXPORT	__exit
	.IMPORT	_main
	.IMPORT	_CAN_Boot_Loader
	.IMPORT _exit
	.IMPORT	__stream_init
	.IMPORT	_main
	.IMPORT ___call_dtors       ; for C++
	.IMPORT	_atexit             ; for C++
	.IMPORT	_RAM_INIT	; Data Section with Initial Value(RAM side)
	.IMPORT	_ROM_INIT	; Data Section with Initial Value(ROM side)
#if COPY_RAMCODE == ON
	.IMPORT	_RAM_RAMCODE	; CODE Section that is transferred to the RAM and run(RAM side)
	.IMPORT	_ROM_RAMCODE	; CODE Section that is transferred to the RAM and run(ROM side)
#endif

;**************************************************************************
; 6.2  Program start (the reset vector should point here)
;**************************************************************************
;--------------------------------------------------------------------------
; definition for start address of data, const and code section
;--------------------------------------------------------------------------
;#if USE_BASE_POINTER_ACCESS == ON
;--------------------------------------------------------------------------
; Data section for base pointer relative access
; and Definition of symbol that indicates address of base pointer
;
; When -KBP is appointed to a compiler, it is used.
; <<note>>
; When BP relative access is used, it is necessary to link the start-up most first.
; definition for -KBP option
;--------------------------------------------------------------------------
    .EXPORT	__bpstart	; The top of data that accesses BP(base pointer)
    .SECTION	IO_DUMMY, IO,  locate=0x400
__bpstart:
;#endif
#if COPY_RAMCODE == ON
	.SECTION	RAMCODE,	CODE,	ALIGN=2
#endif
	.SECTION	DATA,	DATA,	ALIGN=4
	.SECTION	INIT,	DATA,	ALIGN=4
	.SECTION	CONST,	CONST,	ALIGN=4
	.SECTION	EXT_CTOR_DTOR, CONST, ALIGN=4 ; for C++
	.SECTION	CODE,	CODE,	ALIGN=2
	.SECTION	INTVECT,	CONST,	ALIGN=4
	.SECTION	CODE_START, CODE, LOCATE= 0x080024;
	//.SECTION	CODE_START, CODE, ALIGN=4

__start:

;**************************************************************************
; 6.3  Initialization of processor status
;**************************************************************************
;--------------------------------------------------------------------------
; TBR initialization
;--------------------------------------------------------------------------
	LDI	#INTVECT, R0
	MOV	R0, TBR

;--------------------------------------------------------------------------
; Initialization of FPU control register
;
; It is necessary in the device with FPU.
; This description causes no problem for the device without FPU.
; (FCR register is also equipped in the device without FPU.)
; For the device without FPU, if it is not appropriate, set USE_FPU OFF.
;--------------------------------------------------------------------------
#if USE_FPU == ON
	LDI:20	#FCR_SET, R0
	MOV		R0, FCR
#endif
;--------------------------------------------------------------------------
; BP(Base Pointer register) is set.
; The address value of __bpstart is stored in BP.
;--------------------------------------------------------------------------
;#if USE_BASE_POINTER_ACCESS == ON
	LDI	#__bpstart, R0
	MOV	R0, BP
;#endif

;**************************************************************************
; 6.4  Prepare stacks and set the default stack type
;**************************************************************************
;--------------------------------------------------------------------------
; set stack pointer register
;--------------------------------------------------------------------------
	LDI:32	#stack_top, SP

;**************************************************************************
; 6.5  NMI activation(SET ILM)
;**************************************************************************
	STILM	#PS_ILM_LEVEL		;ILM=15
	NOP

;**************************************************************************
; 6.6  The setting of Flash is initialized.
;**************************************************************************
; Setting of FCTLR access wait
	LDI:32	#FCTLR_ADDR,R3			;FCTLR(0x0840) load
	LD		@R3,R1
	LDI:32	#FCTLR_FAW_MASK,R2		;
	AND		R2,R1
	LDI:32	#FLASH_WRITE_WAIT,R2	;FCW wait setting
	OR		R2,R1
	LDI:8	#16,R4
	LSR		R4,R1  ; To write it in the halfword, the right shifting by 16 bits.
	STH		r1,@r3
; FSTR Flash Status Register
	LDI:32	#FSTR_ADDR,R0			;FSTR(0x843) load
	LDI:32	#FSTR_FECCER_CLEAR,R1
	STB		R1,@R0

;**************************************************************************
; 6.7  activate MDI port ( for Extarnal OCD tools)
;**************************************************************************
#if 0
ACTIVATE_MDI:
	LDI:32	#DSUCR_ADDR,R0			;DSUCR(0xff00) load
	LD		@R0,R1
	LDI:32	#DSUCR_DSU_MASK,R2
	AND		R2,R1
	LDI:32	#DSUCR_DSU_ON,R2
	OR		R2,R1
	ST		R1,@R0
#endif
;**************************************************************************
; 6.8  Set clock ratio (ignore subclock) and wait for clocks to stabilize
;**************************************************************************
#if CLOCK_SETTING_THROUGH != ON
PLL_setting:
	LDI:32	#CSELR_ADDR,R0			;CSELR(0x0510)
	LDI:32	#CMONR_MCRDY_ON,R2
; Check Oscillation stable state
MCRDY_check:
	LD		@R0,R1
	AND		R2,R1					;If MCRDY is 1, it is stabilized.
	BEQ		MCRDY_check
; Source Clock => Main Clock/2 (for PLL/SSCG Clock setting)
	LDI:32	#CSELR_CKS_MASK,R2
	LD		@R0,R1
	AND		R2,R1
	ST		R1,@R0
; Source Clocck => Main Clock/2 (wait transition)
	LDI:32	#CMONR_CKM_MCLK,R2
CKM_check:
	LD		@R0,R1
	AND		R2,R1					;If CKM is 0, it is finished the transition to Main Clock/2.
	BNE		CKM_check
; PLL/SSCG setting (Clock Stabilization Time)
	LDI:32	#PLLCR_ADDR,R3			;PLLCR(0x0514)
	LD		@R3,R1
	LDI:32	#PLLCR_POSW_MASK,R2
	AND		R2,R1
	LDI:32	#CLOCK_OSC_WAIT,R2
	OR		R2,R1
	ST		R1,@R3
; Source Clock => PLL Clock
	LDI:32	#CCPSSELR_ADDR,R3		;CCPSSELR(0x0520)
	LDUB	@R3,R1
	LDI:8	#CCPSSELR_PCSEL_PLL,R2	;Source Clock = PLL Clock
	AND		R2,R1
	STB	R1,@R3
; Multiplication ratio setting
	LDI:32	#CCPLLFBR_ADDR,R3		;CCPLLFBR(0x0525)
	LDI:8	#CLOCK_MULTIPLY,R2		;Multiplication ratio
	STB	R2,@R3
; PLL Divide setting (normaly it is 1/2, only MCLK:8MHz-SRCCLK:40MHz it is 1/4)
	LDI:32	#CCPSDIVR_ADDR,R2		;CCPSDIVR(0x0523)
	LDI:8   #CLOCK_PLL_DIVIDE, R1
	STB    	R1, @R2
;
; Clock Gear ON
; CCCGRCR0.GREN=1'b1
	LDI:32	#CCCGRCR0_ADDR,R3		;CCCGRCR0(0x052D)
	LDI:8	#CCCGRCR0_GREN_SET,R2   ;0x01
	ORB		R2,@R3
; Clock Gear CCCGRCR1 STEP Set
; CCCGRCR1.GRSTP=2'b00,CCCGRCR1.GRSTN=6'b00_0000
	LDI:32	#CCCGRCR1_ADDR,R3		;CCCGRCR1(0x052E)
	LDI:8	#CCCGRCR1_STEP_SET,R1   ;0x00
	STB		R1,@R3
; Clock Gear CCCGRCR2 STEP Set
; CCCGRCR2.GRLP=8'b1111_1111
	LDI:32	#CCCGRCR2_ADDR,R3		;CCCGRCR1(0x052F)
	LDI:8	#CCCGRCR2_STEP_SET,R1   ;0xFF
	STB		R1,@R3
;
; PLL Clock start
	LD		@R0,R1					;CSELR(0x0510)
	LDI:32	#CSELR_PCEN_SET,R2
	OR		R2,R1
	ST		R1,@R0
; Source Clock => PLL Clock (wait transition)
	LDI:32	#CMONR_PCRDY_ON,R2
PCRDY_check:
	LD		@R0,R1
	AND		R2,R1
	BEQ		PCRDY_check
; Divide Ratio Setting for PCLK
	LDI:32	#DIVR0_ADDR,R3			;DIV0/DIV2(0x0488/0x048a)
	LD		@R3,R1
	LDI:32	#DIVR_DIVB_DIVP_MASK,R2	;
	AND		R2,R1
	LDI:32	#CLOCK_PCLK1_DIVIDE,R2	;DIV0/DIV2
	OR		R2,R1
	ST		R1,@R3
; PCLK2 synchronization setting
	LDI:32	#SACR_ADDR,R3			;SACR(0x1000) load
	LDUB	@R3,R1
	LDI:8	#SACR_M_MASK,R2			;M setting
	AND		R2,R1
	STB		R1,@R3
; Source Clock = PLL Clock
	LD		@R0,R1					;CSELR(0x0510) load
	LDI:32	#CSELR_CKS_MASK,r2		;
	AND		R2,R1
	LDI:32	#CSELR_CKS_PLL,r2		;PLL select
	OR		R2,R1
	ST		R1,@R0
; Check if Source Clock = PLL Clock
	LDI:32	#CMONR_CKM_PLL,r2
CKM_check2:
	LD		@R0,R1
	AND		R2,R1				;CKM=2 check
	CMP		R2,R1
	BNE		CKM_check2
;
; ClockGear start
	LDI:32	#CCCGRCR0_ADDR,R3		;CCCGRCR0(0x052D)
; Check CCCGR0.GRSTS==2'b00?
	LDI:8	#CCCGRCR0_GRSTS00_MASK,R2 ;0xC0
GRSTS_00_check:
	LDUB	@R3,R2
	AND		R2,R1                   ;(GRSTS==00) (GRSTS!=00)
	BNE		GRSTS_00_check          ;if(Z)next;  if(!Z):check;
; Set CCCGR0.GRSTR=1
	LDI:8	#CCCGRCR0_GRSTR_SET,R2 ;0x02
	ORB		R2,@R3
; Check GRSTS==2'b10?
	LDI:8	#CCCGRCR0_GRSTS10_MASK,R2 ;0x80
GRSTS_10_check:
	LDUB	@R3,R1
	AND		R2,R1                   ;(GRSTS!=1x)GearUP?  (GRSTS==10)
	BEQ		GRSTS_10_check          ;if(Z)check;         if(!Z):next;
; Check CCCGR0.GRSTR=0(Clear)?
	LDI:8	#CCCGRCR0_GRSTR_SET,R2  ;0x02
GRSTR_Clear_check:
	LDUB	@R3,R1
	AND		R2,R1                   ;(GRSTS==0)  (GRSTR==1)
	BNE		GRSTR_Clear_check       ;if(Z):next  ;if(!Z)check;
#endif

;**************************************************************************
; 6.9  Copy initial values to data areas.
;**************************************************************************
#macro	TRANS_ROM_RAM	sec_rom_addr, sec_ram_addr, sec_name
#local	copy_rom1, copy_rom2, copy_rom_end
	LDI	#sec_ram_addr, r0
	LDI	#sec_rom_addr, r1
	LDI	#sizeof(sec_name), r2
	CMP	#0, R2
	BEQ:D	copy_rom_end
	LDI	#3, R12
	AND	R2, R12
	BEQ:D	copy_rom2
	MOV	R2, R13
	MOV	R2, R3
	SUB	R12, R3
copy_rom1:
	ADD	#-1, r13
	LDUB	@(R13, R1), R12
	CMP	R3, R13
	BHI:D	copy_rom1
	STB	R12, @(R13, R0)
	CMP	#0, R3
	BEQ:D	copy_rom_end
copy_rom2:
	ADD	#-4, R13
	LD	@(R13, R1), R12
	BGT:D	copy_rom2
	ST	R12, @(R13, R0)
copy_rom_end:
#endm

	TRANS_ROM_RAM	_ROM_INIT, _RAM_INIT, INIT

; When you transfer RAMCODE to RAM at the user program initiating,
; it is transfered here.
; When you transfer it to RAM in user program,
; it is necessary to prepare the code that you transfer separately and to do it.
;#if COPY_RAMCODE == ON
;	TRANS_ROM_RAM	_ROM_RAMCODE, _RAM_RAMCODE, RAMCODE
;#endif

;**************************************************************************
; 6.10 Clear uninitialized data areas to zero
;**************************************************************************
#macro	FILL04	src_addr
#local	L0, L1, L2, LE
	LDI:8	#0, R0
	LDI	#sizeof src_addr &~0x3, R1
	LDI	#src_addr, R13
	CMP	#0, R1
	BEQ	L1
L0:
	ADD2	#-4, R1
	BNE:D	L0
	ST	R0, @(R13, R1)
L1:
	LDI:8	#sizeof src_addr & 0x3, R1
	LDI	#src_addr + (sizeof src_addr & ~0x3), R13
	CMP	#0, R1
	BEQ	LE
L2:
	ADD2	#-1, R1
	BNE:D	L2
	STB	R0, @(R13, R1)
LE:
#endm

	FILL04	DATA
	FILL04	STACK

;**************************************************************************
; 6.11 Watchdock timer setting and reset
;**************************************************************************
	LDI:32	#WDTCPR1_ADDR,R0
	LDI:32	#WDTCPR1_CLEAR,R1
	STB		R1,@R0

;**************************************************************************
; 6.12 Call C-language main function
;**************************************************************************
;--------------------------------------------------------------------------
; C library initialize
;--------------------------------------------------------------------------
	CALL32	__stream_init, R12

#if CPP_LANG == ON
;--------------------------------------------------------------------------
; C++ global objects initialize
;--------------------------------------------------------------------------
	ldi	#___call_dtors, r4
	call32	_atexit, r12

	ldi	#EXT_CTOR_DTOR, r8
	ldi	#EXT_CTOR_DTOR + sizeof(EXT_CTOR_DTOR), r9
	cmp	r9, r8
	beq	L1
L0:
	ld	@r8, r10
	call:d	@r10
	add	#4, r8
	cmp	r9, r8
	bc	L0
L1:
#endif

start_main:
;--------------------------------------------------------------------------
; call main routine
;--------------------------------------------------------------------------
	LDI:8	#0, R4		; Set the 1st parameter for main to 0.
	CALL32:D _main, R12
;   call32	_CAN_Boot_Loader,	r12
	LDI:8	#0, R5		; Set the 2nd parameter for main to NULL.
	CALL32	_exit, R12
__exit:

;**************************************************************************
; 6.13  Program end loop
;**************************************************************************
end:
	BRA	end
	.END	__start

;**************************************************************************
; 7  Revision and History
;**************************************************************************
;Date        Ver     Description
;2011-12-28  V00L01  Preliminary version
;2012-03-09  V00L02  Preliminary version(not released)
;                   -startup.asm:
;                     -Clock gear function addition
;                     -NMI activation moved forward
;                     -WORK_FLASH section addition
;                     -Rearranging of the item number
;                   -ChangeHighSpeed_befor4/8.prc:
;                     -Clock gear function addition
;                   -ChangeHighSpeed_after4/8.prc:
;                     -unified it in ChangeHighSpeed_after.prc
;                   -main.c:
;                     -added Watchdog timer clear in main loop
;2012-03-16  V01L01  First edition
;2012-04-13  V01L02  The following are changed.
;                   -startup.asm:
;                     -Clock setting menu addition (Clock setting through)
;                   -ChangeHighSpeed_after.prc:
;                     -Improvement at processing time
