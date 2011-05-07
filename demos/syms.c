#include <stdlib.h>
#include <assert.h>
#include "syms.h"

#include <stdio.h>

static char **valTable=0;
static int *keyTable=0;
static size_t tableSize=0;

/*  A crude symbol table implementation */

void
put(char *s, int key)
{
  if ( 1 ) { /* TODO: check to see if key is already used */
  ++tableSize;
  keyTable = realloc(keyTable,tableSize * ( sizeof (int) ) );
  keyTable[tableSize-1]=key;
  }

  valTable = realloc(valTable,tableSize * ( sizeof (char*) ) );
  valTable[tableSize-1]=(char *) malloc(strlen(s)+1);
  strcpy(valTable[tableSize-1],s);



}


char *
get(int key)
{
  int i;
  for (i=0; i < tableSize ; ++i ) {
    if ( keyTable[i] == key ) 
      return valTable[i];
  }
  return 0;
}
