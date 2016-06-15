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

#ifndef CSVFIELD_H
#define CSVFIELD_H

#include <stdlib.h>


// reusable, memory
struct csvfield {
  char* data;
  size_t len; //malloc'd size
};

struct csvfield* csvfield_create();
void csvfield_reset(struct csvfield* pField);
void csvfield_set(struct csvfield* pField, const char* buf, int bufStartIdx, int len);
void csvfield_append(struct csvfield* pField, const char* buf, int bufStartIdx, int buflen);
void csvfield_printToFile(const struct csvfield* pField, FILE* fp);

#endif
