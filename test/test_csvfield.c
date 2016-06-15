
#include <stdio.h>
#include <string.h>

#include "csvfield.h"

int main(int argc, char** argv) {
  printf("test\n");
  struct csvfield* cfield;
  cfield = csvfield_create();
  csvfield_printToFile(cfield, stderr);
  {
    char txt[] = "hello";
    csvfield_set(cfield, txt, 0, strlen(txt));
    csvfield_printToFile(cfield, stderr);
  }
  {
    char txt[] = "123";
    csvfield_set(cfield, txt, 0, strlen(txt));
    csvfield_printToFile(cfield, stderr);
  }
  {
    char txt[] = "123456789012345678901234567";
    csvfield_set(cfield, txt, 0, strlen(txt));
    csvfield_printToFile(cfield, stderr);
  }
   {
    char txt[] = "123";
    csvfield_set(cfield, txt, 0, strlen(txt));
    csvfield_append(cfield, txt, 0, strlen(txt));
    csvfield_printToFile(cfield, stderr);
  }
  csvfield_reset(cfield);
  csvfield_printToFile(cfield, stderr);
  return 0;
}
