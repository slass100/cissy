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

#ifndef CSVLINE_H
#define CSVLINE_H

#include <stdlib.h>
#include <stdbool.h>

struct csvline {
  struct csvfield** field; // array
  size_t fieldsize;  // num of array entries
  size_t currentIdx; //next open slot
  char* eolStr;
};

size_t csvline_getFieldCnt(struct csvline* cline);
char* csvline_getField(struct csvline* cline, size_t idx);

void csvline_reset(struct csvline* cline);
void csvline_printToFile(struct csvline* cline, FILE* fp);
struct csvline* csvline_create();
void csvline_addField(struct csvline* cline, const char* txtfield, int fieldstartidx, int fieldlen);
void csvline_appendField(struct csvline* cline, const char* txtfield, int fieldstartidx, int fieldlen);

#endif // CSVLINE_H
