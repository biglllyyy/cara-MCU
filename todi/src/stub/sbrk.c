/*=========================================================================
; FR Family Softune Workbench sample,
; ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU SEMICODUCTOR LIMITED 2011
; LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU SEMICODUCTOR LIMITED
=========================================================================*/

#include <stdio.h>
#include <unistd.h>

#define  HEAP_SIZE	((BUFSIZ+2)*3)

static long brk_siz  =  0;
static char _heap[HEAP_SIZE];

/*
 * minimum sbrk() implementation for FILE structure allocation.
 */

char  *sbrk(int size)
{
  if  (((brk_siz+size) > sizeof(_heap)) || ((brk_siz+size)  <  0))
    return ((char*)-1);
  brk_siz+=size;
  return (_heap+brk_siz-size);
}
