/*
   Copyright 2015 Steven Lass

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "csvline.h"
#include "csvfield.h"

size_t csvline_getFieldCnt(struct csvline* cline) {
  return cline->currentIdx;
}

char* csvline_getField(struct csvline* cline, size_t idx) {
  if (idx >= cline->currentIdx) {
    return "";
  }
  return cline->field[idx]->data;
}

// try to be efficient
void csvline_reset(struct csvline* ptr) {
  int i;
  for (i=0; i<ptr->fieldsize; i++) {
    if (ptr->field[i] != NULL) {
      csvfield_reset(ptr->field[i]);
    }
  }
  ptr->currentIdx = 0;
  ptr->eolStr = "\n";
}

struct csvline* csvline_create() {
  struct csvline* ptr = malloc(sizeof(struct csvline));
  if (ptr == NULL) {
    fprintf(stderr, "error: malloc\n");
    exit(-1);
  }
  ptr->field = NULL;
  ptr->fieldsize = 0;
  ptr->currentIdx = 0;
  ptr->eolStr = "\n";
  return ptr;
}

void csvline_printToFile(struct csvline* ptr, FILE* fp) {
  fprintf(fp, "[[");
  int i;
  for (i=0; i<ptr->fieldsize; i++) {
    fprintf(stderr, "[%s:%d]", ptr->field[i]->data, (int) ptr->field[i]->len);
  }
  fprintf(fp, "]");
  fprintf(fp, "fs(%d):", (int) ptr->fieldsize);
  fprintf(fp, "i(%d)", (int) ptr->currentIdx);
  fprintf(fp, "]\n");
}

// copy field
// do not include \0 in fieldlen
void csvline_addField(struct csvline* cline, const char* txtfield, int fieldstartidx, int fieldlen) {
  if (cline->fieldsize <= cline->currentIdx) {
    //realloc a block for effieciency
    struct csvfield** tmp = cline->field;
    cline->field = malloc( (cline->fieldsize + 10)*sizeof(struct csvfield*));
    if (cline->field == NULL) {
      fprintf(stderr, "error: malloc\n");
      exit(-1);
    }
    if (tmp != NULL) {
      memcpy(cline->field, tmp, cline->fieldsize*sizeof(struct csvfield *));
      free(tmp);
    }
    cline->fieldsize += 10;
    int i;
    for (i=cline->fieldsize-10; i<cline->fieldsize; i++) {
      cline->field[i]=csvfield_create();
    }
  }
  csvfield_set(cline->field[cline->currentIdx], txtfield, fieldstartidx, fieldlen);
  cline->currentIdx++;
}

// append to the last field in the line
void csvline_appendField(struct csvline* cline, const char* txtfield, int fieldstartidx, int fieldlen) {
  csvfield_append(cline->field[cline->currentIdx - 1], txtfield, fieldstartidx, fieldlen);
}
