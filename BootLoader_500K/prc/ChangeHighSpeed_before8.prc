#====================================================================
#  FR-81S Family Template Project V01L01
#  ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU SEMICONDUCTOR LIMITED 2012
#  LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU SEMICONDUCTOR LIMITED
#====================================================================
CANCEL FREQUENCY /MAX		# disable high speed mode
reset

while 1				# check CMONR.MCRDY
  if %b(0x511)&0x20		# CMONR.MCRDY=1
    break;
  endif
endw

set mem 0x0510 = 0x20|(%b(0x510)&0xDF)
				# CSELR set clock source
				# SCEN(bit7) = 0 :enable sub clock
				# PCEN(bit6) = 0 :enable PLL clock
				#>MCEN(bit5) = 1 :enable main clock
				# CKS(bit1-0)= 2'b00 :clock select MCLK/2

while 1				# check CMONR.CKM
  if !(%b(0x511)&0x03)		# CMONR.CKM(bit1-0) = 2'b00
    break;
  endif
endw

				#------- set PLL clock(80MHz) -------
set mem/half 0x0514 = 0x00B0|(%h(0x514)&0xFF0F)
				# PLLCR
				#>POSW(bit7-4) = 4'b1101 (1024us)
				# PDS (bit3-0) = 4'b0000 (MCLK/1)

set mem 0x0520 = 0x00|(%b(0x520)&0xFE)
				# CCPSSELR : select PLL
				#>PCSEL(bit0) = 1'b0

set mem 0x0525 = 0x13		# CCPLLFBR PLL macro clock = MCLK/1x20
				#>IDIV(bit6-0) = 7'b001_0011

set mem 0x0523 = 0x00		# CCPSDIVR PLL clock =  PLL macro clock / 2
				#>PODS(bit6-4) = 3'b000
				# SODS(bit2-0) = 3'b000

set mem 0x052D = 0x01|(%b(0x52D))
				# Clock Gear ON
				#>CCCGRCR0.GREN(bit0) = 1
set mem 0x052E = 0x00
				# CCCGRCR1 Step Set
				#>CCCGRCR1.GRSTP(bit7-6)=2'b00, CCCGRCR1.GRSTN(bit5-0) = 6'b00_0000,
set mem 0x052F = 0xFF
				# CCCGRCR2 Step Set
				#>CCCGRCR2.GRLP(bit7-0)=8'b1111_1111

set mem 0x0510 = 0x60|(%b(0x510)&0x9F)
				# CSELR set clock source
				# SCEN(bit7) = 0 :enable sub clock
				#>PCEN(bit6) = 1 :enable PLL clock
				#>MCEN(bit5) = 1 :enable main clock
				# CKS(bit1-0)= 2'b00 :clock select MCLK/2

while 1				# check CMONR.PCRDY
  if %b(0x511)&0x40		# CMONR.PCRDY=1
    break;
  endif
endw

set mem 0x0488 = 0x00|(%b(0x488)&0x1F)
				# DIVR0 : MCLK/1
				#>DIVB(bit7-5) = 3'b000

set mem 0x048a = 0x10|(%b(0x48a)&0x0F)
				# DIVR2 : PCLK1= 80/2 = 40MHz
				#>DIVP(bit7-4) = 4'b0001

set mem 0x1000 = 0x00|(%b(0x1000)&0xFE)
				# SACR : PCLK2 set
				#>M(bit0) = 0(sync)

set mem 0x0510 = 0x62|(%b(0x510)&0x9C)
				# CSELR set clock source
				# SCEN(bit7) = 0 :enable sub clock
				#>PCEN(bit6) = 1 :enable PLL clock
				#>MCEN(bit5) = 1 :enable main clock
				#>CKS(bit1-0)= 2'b10 :clock select PLL

while 1				# check SRCCLK==PLLSSCLK
  if %b(0x511)&0x42		# CMONR.PCRDY=1
    break;
  endif
endw

while 1				# check CCCGR0.GRSTS
  if !(%b(0x52D)&0xC0)		# CCCGR0.GRSTS(bit7-6)==2'b00
    break;
  endif
endw

set mem 0x052D = 0x02|(%b(0x52D))
				# Clock Gear Start
				#>CCCGRCR0.GSTR(bit1)=1

while 1				# check CCCGR0.GRSTS
  if %b(0x52D)&0x80		# CCCGR0.GRSTS==2'b10?
    break;
  endif
endw

while 1				# check Gear End
  if !(%b(0x52D)&0x02)		# CCCGRCR0.GSTR(bit1)==0?
    break;
  endif
endw

SET FREQUENCY /MAX 80		# enable high speed mode
SHOW SYSTEM
