/*
   Header fÅr die erweiterten stdio Routinen

*/


typedef struct _cx_file
{
  char *pwd;		/* password */
  int pwd_l;		/* length of password */
  int pwd_actpos;	/* actual position on the password */
  int isfree;		/* shows if this file handle is free */

  FILE *stream;		/* the real file handle */
} cx_file;



cx_file* cx_fopen(const char* name, const char* mode);

int cx_setkey (const char* key, cx_file* cxp);
int cx_fclose (cx_file* cxp);

char* cx_fgets (char* buffer, int count, cx_file* cxp);
int cx_fputs (const char* str, cx_file* cxp);

size_t cx_freadc(char *buffer, size_t size, size_t count, cx_file* cxp);
size_t cx_fwritec(const char *buffer, size_t size, size_t count, cx_file* cxp);

int cx_feof(cx_file* cxp);






