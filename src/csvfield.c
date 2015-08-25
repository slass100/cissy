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

#include "csvfield.h"

const int STR_MEM_PAD = 10;

void csvfield_printToFile(struct csvfield* ptr, FILE* fp) {
	fprintf(fp, "[%s:%d]\n", ptr->data, ptr->len);
}

void csvfield_set(struct csvfield* ptr, const char* buf, int bufStartIdx, int buflen) {
	if (buflen+1 > ptr->len) {
		if (ptr->data != NULL) {
			free(ptr->data);
		}
		ptr->len = buflen + 1 + STR_MEM_PAD;
		ptr->data = malloc(ptr->len);
		if (ptr->data == NULL) {
			fprintf(stderr, "csv_setCsvfield: malloc error\n");
			exit(-1);
		}
	}
	strncpy(ptr->data, &buf[bufStartIdx], buflen);
	ptr->data[buflen] = '\0';
}

struct csvfield* csvfield_create() {
	struct csvfield* ptr = malloc(sizeof(struct csvfield));
	ptr->data = NULL;
	ptr->len = 0;
	return ptr;
}

// try to be efficient
void csvfield_reset(struct csvfield* ptr) {
	ptr->data = '\0';
}
