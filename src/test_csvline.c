
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csvfield.h"
#include "csvline.h"

int main(int argc, char** argv) {
  char txt[] = "hello";
  struct csvline* cline = csvline_create();
  int i;
  for (i=0; i<25; i++) {
    csvline_addField(cline, txt, 0, strlen(txt));
    csvline_printToFile(cline, stdout);
  }
  printf("Count = %d\n", (int) csvline_getFieldCnt(cline));
  printf("1st = %s\n", csvline_getField(cline, 0));
  csvline_reset(cline);
  csvline_printToFile(cline, stdout);

  if(cline) {}
  return 0;
}
