/*
 MB91520 Series I/O register declaration file V01L05
 ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU SEMICONDUCTOR LIMITED 2011
 LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU SEMICONDUCTOR LIMITED
*/
#if defined(__CPU_MB91F524BW__) || \
    defined(__CPU_MB91F524BS__) || \
    defined(__CPU_MB91F524DW__) || \
    defined(__CPU_MB91F524DS__) || \
    defined(__CPU_MB91F524FW__) || \
    defined(__CPU_MB91F524FS__) || \
    defined(__CPU_MB91F524JW__) || \
    defined(__CPU_MB91F524JS__) || \
    defined(__CPU_MB91F524KW__) || \
    defined(__CPU_MB91F524KS__) || \
    defined(__CPU_MB91F524LW__) || \
    defined(__CPU_MB91F524LS__) || \
    defined(__CPU_MB91F525FW__) || \
    defined(__CPU_MB91F525FS__) || \
    defined(__CPU_MB91F525JW__) || \
    defined(__CPU_MB91F525JS__) || \
    defined(__CPU_MB91F525KW__) || \
    defined(__CPU_MB91F525KS__) || \
    defined(__CPU_MB91F525LW__) || \
    defined(__CPU_MB91F525LS__) || \
    defined(__CPU_MB91F526FW__) || \
    defined(__CPU_MB91F526FS__) || \
    defined(__CPU_MB91F526JW__) || \
    defined(__CPU_MB91F526JS__) || \
    defined(__CPU_MB91F526KW__) || \
    defined(__CPU_MB91F526KS__) || \
    defined(__CPU_MB91F526LW__) || \
    defined(__CPU_MB91F526LS__) || \
    defined(__CPU_MB91F522BS__) || \
    defined(__CPU_MB91F522BW__) || \
    defined(__CPU_MB91F522DS__) || \
    defined(__CPU_MB91F522DW__) || \
    defined(__CPU_MB91F522FS__) || \
    defined(__CPU_MB91F522FW__) || \
    defined(__CPU_MB91F522JS__) || \
    defined(__CPU_MB91F522JW__) || \
    defined(__CPU_MB91F522KS__) || \
    defined(__CPU_MB91F522KW__) || \
    defined(__CPU_MB91F522LS__) || \
    defined(__CPU_MB91F522LW__) || \
    defined(__CPU_MB91F523BS__) || \
    defined(__CPU_MB91F523BW__) || \
    defined(__CPU_MB91F523DS__) || \
    defined(__CPU_MB91F523DW__) || \
    defined(__CPU_MB91F523FS__) || \
    defined(__CPU_MB91F523FW__) || \
    defined(__CPU_MB91F523JS__) || \
    defined(__CPU_MB91F523JW__) || \
    defined(__CPU_MB91F523KS__) || \
    defined(__CPU_MB91F523KW__) || \
    defined(__CPU_MB91F523LS__) || \
    defined(__CPU_MB91F523LW__) || \
    defined(__CPU_MB91F525BS__) || \
    defined(__CPU_MB91F525BW__) || \
    defined(__CPU_MB91F525DS__) || \
    defined(__CPU_MB91F525DW__) || \
    defined(__CPU_MB91F526BS__) || \
    defined(__CPU_MB91F526BW__) || \
    defined(__CPU_MB91F526DS__) || \
    defined(__CPU_MB91F526DW__) || \
    defined(__CPU_MB91F524KS_FR80__) || \
    defined(__CPU_MB91F524KW_FR80__) || \
    defined(__CPU_MB91F526KS_FR80__) || \
    defined(__CPU_MB91F526KW_FR80__)
#ifdef __FASM__
#include "mb91520_a.inc"
#else
#include "mb91520.h"
#endif
#else
#error "The I/O register file of the specified CPU option does not exist"
#endif

