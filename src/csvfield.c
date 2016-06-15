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

/**
*  Re-usable data structure for field/line processing.
*  
*
*/

/**
* Malloc a bit extra so we hopefully do not have to 
* free/malloc/re-malloc
*/
const int STR_MEM_PAD = 10;

/**
* simple print for debugging
*
* @param ptr field to print
* @param fp output file
*/
void csvfield_printToFile(const struct csvfield* pField, FILE* fp) {
	fprintf(fp, "[%s:%d]\n", pField->data, (int) pField->len);
}

/**
*  Set the Field. Add nul termination.
*
* @param pField field to set
* @param buf input
* @param offset offset
* @param buflen length
*
*/
void csvfield_set(struct csvfield* pField, const char* buf, int offset, int buflen) {
	// create a bigger buffer if needed
	if (buflen+1 > pField->len) {
		if (pField->data != NULL) {
			free(pField->data);
		}
		pField->len = buflen + 1 + STR_MEM_PAD;
		pField->data = malloc(pField->len);
		if (pField->data == NULL) {
			fprintf(stderr, "csv_setCsvfield: malloc error\n");
			exit(-1);
		}
	}
	strncpy(pField->data, &buf[offset], buflen);
	pField->data[buflen] = '\0';
}

/**
*  Append to the Field. Add nul termination.
*
* @param pField field to set
* @param buf input
* @param offset offset
* @param buflen length
*/
void csvfield_append(struct csvfield* pField, const char* buf, int offset, int buflen) {
  int origflen = ( pField->data ? strlen(pField->data) : 0);
  if ( (origflen + buflen + 1) > pField->len ) {
    char* tmp = pField->data;
    pField->len = pField->len + buflen + STR_MEM_PAD;
    pField->data = malloc(pField->len);
    if (pField->data == NULL) {
      fprintf(stderr, "csv_setCsvfield: malloc error\n");
      exit(-1);
    }
    if (tmp != NULL) {
      strncpy(pField->data, tmp, strlen(tmp)+1);
      free(tmp);
    }
  }
  strncpy(&pField->data[origflen], &buf[offset], buflen);
  pField->data[origflen + buflen] = '\0';
}

/**
* create an empty field
* 
* @return new created field
*/
struct csvfield* csvfield_create() {
	struct csvfield* pField = malloc(sizeof(struct csvfield));
    if (pField == NULL) {
      fprintf(stderr, "csvfield_create: malloc error\n");
      exit(-1);
    }	
	pField->len = STR_MEM_PAD;
    pField->data = malloc(pField->len);
    if (pField->data == NULL) {
      fprintf(stderr, "csvfield_create: malloc error\n");
      exit(-1);
    }
	return pField;
}

/**
*  Reset for re-use.
*
* @param pField field to reset
*
*/
void csvfield_reset(struct csvfield* pField) {
	pField->data = '\0';
}
