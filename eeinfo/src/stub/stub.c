/*=========================================================================
; FR Family Softune Workbench sample,
; ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU SEMICODUCTOR LIMITED 2011
; LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU SEMICODUCTOR LIMITED
=========================================================================*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/* This is dummy functions */
/*
 * notice
 * - This library does not support the following functions.
 *   - 'stdin' handle (fread() etc. do not operate).
 *   - fseek()
 *   - fopen()
 * - stdout&stderr ware opened. freopen() is not necessary.
 * - stdout&stderr are terminal.
 */

/* fcntl.h */
int open(char* fname, int fmode, int p)
{
  return -1;
}

/* unistd.h */
int read(int fileno, char* buf, int size)
{
  return -1;
}
long lseek(int fileno, off_t offset, int whence)
{
  return -1;
}
int close(int fileno)
{
  return 0;
}
int isatty(int fileno)
{
  return (fileno < 3);
}
