/*
   cxio library
   Build in a "learning C course" at the University of Applied Science FH-Wedel by Julian Buss and Holger Leskien.
   (c) 1998 by Julian Buss, Holger Leskien. Distributed unter the GLP. See file COPYING for details.

   We are not skilled C developers.
   So if you find bugs or you have hints for better code - please tell me!
   eMail to julian@wh.fh-wedel.de, constructive critism is always welcome.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cxio.h"

static cx_file cx_files[FOPEN_MAX];
static int cx_files_init = 0;

/* init the cx_file structure */
static void _init (void)
{
  int i;

  for (i = 0; i < FOPEN_MAX; i++)
  {
    cx_files[i].isfree = 1;
    cx_files[i].pwd = NULL;
    cx_files[i].pwd_l = -1;
    cx_files[i].pwd_actpos = -1;
  }

  cx_files_init = 1;
}

void _cx_codec(char *buffer, int count, cx_file* cxp)
{
  int i;

  for (i = 0; i <= count; i++)
  {
     buffer[i] ^= (int) cxp->pwd[cxp->pwd_actpos];
     buffer[i] ^= cxp->pwd_l;

     cxp->pwd_actpos++;
     if (cxp->pwd_actpos >= cxp->pwd_l ) cxp->pwd_actpos = 0;
  }
}


/*
  enocdes a char and writes to file
*/
static int _cx_fputc  (int c, cx_file* cxp)
{
  int rc;

  c ^= (int) cxp->pwd[cxp->pwd_actpos];		/* encode: (char XOR actual char of password) XOR length of password */
  c ^= cxp->pwd_l;					/* better than only XOR with password */

  rc = fputc( c, cxp->stream);


  cxp->pwd_actpos++;
  if (cxp->pwd_actpos >= cxp->pwd_l ) cxp->pwd_actpos = 0;

  return rc;
}


/*
  gets a char and decodes it
*/
static int _cx_fgetc  (cx_file* cxp)
{
  int c;

  c = fgetc(cxp->stream);

  if (c == EOF) return EOF;

  c ^= cxp->pwd[cxp->pwd_actpos];			/* same as _cx_fputc */
  c ^= cxp->pwd_l;

  cxp->pwd_actpos++;
  if (cxp->pwd_actpos >= cxp->pwd_l ) cxp->pwd_actpos = 0;

  return c;
}


/*
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³                        ³
   ³ exported functions     ³
   ³                        ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/


/*
*/
cx_file* cx_fopen (const char* name, const char* mode)
{
  int i = 0;

  char dummy[16] = "\0";
  char *cx_mode;


  cx_mode = strcpy(dummy, mode);

  if (!strchr(mode, 'b') ) strcat(cx_mode, "b\0");

  /* init needed? */
  if (!cx_files_init) _init();

  /* get next free file handle */
  for (i = 0; i < FOPEN_MAX; i++) { if (cx_files[i].isfree) break; }

  if (i == FOPEN_MAX)
  {
    /* ToDo: setting errno if no more file handles */
    return NULL;
  }

  cx_files[i].isfree = 0;
  if ( ((strcmp(name, "stdin") != 0)) && ((strcmp(name, "stdout") != 0)) ) cx_files[i].stream = fopen(name, cx_mode);
  else
  {
    if ( strcmp(name, "stdin") == 0 ) cx_files[i].stream = stdin;
    if ( strcmp(name, "stdout") == 0 ) cx_files[i].stream = stdout;
  }

  return &cx_files[i];
}


/*
*/
int cx_fclose (cx_file* cxp)
{
  cxp->isfree = 1;
  free(cxp->pwd);

  return fclose(cxp->stream);
}

/*
   sets the password (up to 255 chars)
   if ok, return length of password
   if failure, return -1
*/
int cx_setkey (const char* key, cx_file* cxp)
{
  int i = 0;

  if (!key) return -1;

  cxp->pwd = malloc(256 * sizeof(char));
  if (!cxp->pwd) return -1;			/* if no more memory on heap, return with error */

  for (i = 0; (key[i]) && (i <=255); i++) cxp->pwd[i] = key[i];
  cxp->pwd[i + 1] = '\0';

  cxp->pwd_l = strlen(cxp->pwd);
  cxp->pwd_actpos = 0;

  return cxp->pwd_l;
}

/*
*/
char* cx_fgets (char* buffer, int count, cx_file* cxp)
{
  int i = 0;
  char c;

  while (i < count)
  {
    c = _cx_fgetc(cxp);
    if (c == EOF) break;

    buffer[i] = c;
    buffer[i + 1] = '\0';

    i++;
  }

  return buffer;
}

/*
*/
int cx_fputs (const char* str, cx_file* cxp)
{
  int i = 0;

  while (str[i])
  {
    if (!_cx_fputc(str[i], cxp) ) return i;
    i++;
  }

  return i;
}


size_t cx_freadc(char *buffer, size_t size, size_t count, cx_file* cxp)
{
  size_t rc;

  rc = fread(buffer, sizeof(char), count, cxp->stream);
  _cx_codec(buffer, rc, cxp);

  return rc;
}

size_t cx_fwritec(const char *buffer, size_t size, size_t count, cx_file* cxp)
{
  size_t rc;
  char *cbuffer;
  int i;

  cbuffer = malloc(256 * sizeof(char));
  if (!cbuffer) return -1;			/* if no more memory on heap, return with error */

  for (i = 0; i <=255; i++) cbuffer[i] = buffer[i];

  _cx_codec(cbuffer, count, cxp);
  rc = fwrite(cbuffer, sizeof(char), count, cxp->stream);

  return rc;
}


/*
*/
int cx_feof (cx_file* cxp)
{
  return feof(cxp->stream);
}

