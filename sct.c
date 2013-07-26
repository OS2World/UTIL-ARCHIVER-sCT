/*
   C-Source for sCT tool
   written by Julian Buss, julian@wh.fh-wedel.de

   This source code is distributed under the GPL licence. See file COPYING.SCT for details.

   I am NOT a skilled C developer - I just started learning C.

   So if you find bugs or you have hints for better code - please tell me!
   eMail to julian@wh.fh-wedel.de, constructive critism is always welcome.

*/

#include <stdio.h>
#include <limits.h>
#include <strings.h>
#include "cxio.h"

/* for debugging version... is not used much at this time... */
//#define __DEBUG

#define PRGNAME "sCT - simple crypto tool"
#define VERSION "0.1beta"

char *password = NULL;
char *workmode = NULL;
char *fin = NULL;
char *fout = NULL;
int verbose = 1;		/* be verbose or not */

void help(char *msg)
{
 printf("%s version %s by Julian Buss, julian@wh.fh-wedel.de.\n", PRGNAME, VERSION);
 printf("Encodes and decodes a file with a password.\n\n");
 printf("Based on cxio routines from Julian Buss and Holger Leskien.\n");
 printf("Support free OS/2 software - visit http://www.netlabs.org!\n\n");
 printf("%s\n", msg);
 printf("Usage: sct e|d [option 1] .. [option n]\n");
 printf("       e                encode\n");
 printf("       d                decode\n");
 printf("       -fin <file>      use this file for input ('stdin' ok)\n");
 printf("       -fout <file>     use this file for output ('stdout' ok)\n");
 printf("       -p <password>    use this password\n\n");
 printf("If no infile is given, read from stdin.\n");
 printf("If no outfile is given, write to stdout.\n");
 printf("If no password is given, ask (not possible when input comes from stdin).\n");
 printf("If no work mode or is given, terminate.\n");
 printf("\n");
 printf("Examples:\n");
 printf("  sct e -fin readme -fout readmy.crypt\n");
 printf("  sct d -fin readme\n");
 printf("  sct e -fin mypicture.jpg -fout crypted.jpg\n");
 printf("       encodes picture, saves in other file and asks for password.\n");
 printf("  dir | sct e -p test\n");
 printf("       encodes output from dir command to stdout.\n\n");

 exit(1);
}


/* get command line parameters and init the needed vars */
void init(int argc, char *argv[])
{
  int i;
  char buffer[256];
  char tmp[256];

  for (i = 1; i < argc; ++i)
  {
    if (strchr(argv[i], '?') != NULL) help("Help wanted. Here it is:");  /* if there's a ? in the arg, print out help */
    if (i == 1) workmode = argv[i];
    if (strcmp(argv[i], "-fin") == 0) fin = argv[++i];  		/* take this as in file */
    if (strcmp(argv[i], "-fout") == 0) fout = argv[++i];		/* take this as out file */
    if (strcmp(argv[i], "-p") == 0) password = argv[++i];		/* and finally the password */
  }
  if (!fin) fin = "stdin";						/* if fin is not defined, take stdin */
  if (!fout) fout = "stdout";						/* if fout is not defined, take stdout */
  if (strcmp(fout, "stdout") == 0) verbose = 0;
  if (!workmode) help("ERROR: No workmode specified!");			/* no workmode? terminate with help! */

  if (verbose)
  {
    printf("%s version %s by Julian Buss, julian@wh.fh-wedel.de.\n", PRGNAME, VERSION);
    printf("Support free OS/2 software - visit http://www.netlabs.org!\n\n");
  }

  if (!password && (strcmp(fin, "stdin") != 0) )/* still no password? ask, but not if fin is stdin! */
  {
    printf("password: ");
    fgets(buffer, 255, stdin);			
    i = sscanf(buffer, "%s", tmp);
    if (i == -1) help("ERROR: No password given!");

    password = strdup((char*)tmp);					/* donnow why "password = &tmp" doesn't work... */
  }
  if (!password) help("ERROR: No password given!");			/* still no password (because no one given on commandline
  									   and fin == "stdin"), exit */

  if (verbose)
  {
    if (strcmp(workmode, "e") == 0) printf("Encoding"); else printf("Decoding");
    printf(" file %s to %s...", fin, fout);
  }
}

void progressChar(int count)
{
  switch (count)
  {
    case 10: { printf("\x08|"); break; }
    case 20: { printf("\x08/"); break; }
    case 30: { printf("\x08-"); break; }
    case 40: { printf("\x08\\"); break; }
  }
}

/* encode the file
   read with normal fgets the file
   write out with cx_fputs
*/
void encode()
{
  FILE *instream;
  cx_file *cx_outstream;
  char buffer[256];
  int  rc = -1;
  int count = 1;

  if (verbose) printf("working... ");
  if ( strcmp(fin, "stdin") == 0 ) instream = stdin; else instream = fopen(fin, "rb");
  cx_outstream = cx_fopen(fout, "w");
  cx_setkey(password, cx_outstream);

  while (!feof(instream))
  {
    rc = fread(buffer, sizeof(char), 255, instream);
    rc = cx_fwritec(buffer, sizeof(char), rc, cx_outstream);

    count++;
    progressChar(count);
    if (count == 40) count = 1;
  }

  fclose(instream);
  cx_fclose(cx_outstream);
  if (verbose) printf("\ndone.\n");
}


/* decode the file
   read with cx_fgets
   put out with normal fputs
*/
void decode()
{
  FILE *outstream;
  cx_file *cx_instream;
  char buffer[256];
  int  rc = -1;
  int count = 1;

  if (verbose) printf("working... ");
  cx_instream = cx_fopen(fin, "r");
  if ( strcmp(fout, "stdout") == 0 ) outstream = stdout; else outstream = fopen(fout, "wb");
  cx_setkey(password, cx_instream);

  while (!cx_feof(cx_instream))
  {
    rc = cx_freadc(buffer, sizeof(char), 255, cx_instream);
    rc = fwrite(buffer, sizeof(char), rc, outstream);

    count++;
    progressChar(count);
    if (count == 40) count = 1;
  }

  cx_fclose(cx_instream);
  fclose(outstream);
  if (verbose) printf("\ndone.\n");
}

void go(void)
{
  if (*workmode == 'e') encode(); else decode();
}

int main (int argc, char *argv[])
{

  init(argc, argv);
  go();

  return 0;
}


/* the end - simple, eh? */

