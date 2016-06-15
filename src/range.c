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
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "range.h"

/**
* Used for column lists
*/

/**
* free memory
*
* @param list range
*/
void rangeFree(struct rangeElement* list) {
  if (list == NULL) {
    return;
  }
  struct rangeElement* ptr = list;
  while (ptr->next != NULL) {
    struct rangeElement* ptrNext = ptr->next;
    free(ptr);
    ptr = ptrNext;
  }
  free(ptr);
  list=NULL;
}

/**
* Create a new range
*/
struct rangeElement* rangeCreate() {
  struct rangeElement* ptr = malloc(sizeof(struct rangeElement));
  if (ptr == NULL) {
    fprintf(stderr, "rangeCreate: malloc error\n");
    exit(-1);
  }
  ptr->start = -1;
  ptr->end = -1;
  ptr->next = NULL;
  return ptr;
}

// pass NULL list to create new list
/**
* add a n-m range to the list
*
* Add start-end range.  Create a new list if necessary
*
* @param start beginning 
* @param end ending
* @param startOfList list
* @return list
*/
struct rangeElement* rangeAddStartEnd(uint32_t start, uint32_t end, struct rangeElement* startOfList) {
  assert(start > 0);
  assert(end >= start);
  if (startOfList == NULL) {
    startOfList = rangeCreate();
    startOfList->start = start;
    if (start == end) {
      startOfList->rangetype = SINGLE;
    }
    else {
      startOfList->rangetype = STARTEND;
      startOfList->end = end;
    }
    return startOfList;
  }
  struct rangeElement* ptr = startOfList;
  //advance to the end of list
  while (ptr->next != NULL) {
    ptr = ptr->next;
  }
  ptr->next = rangeCreate();
  ptr = ptr->next;
  ptr->start = start;
  if (end == start) {
    ptr->rangetype = SINGLE;
  }
  else {
    ptr->rangetype = STARTEND;
    ptr->end = end;
  }
  return startOfList;
}

/**
* Add a n- range (to end of line) to list
*
* Add a greater than range, create new list if necessary
*
* @param num beginning 
* @param startOfList list
* @return list
*/
struct rangeElement* rangeAddGreaterEqual(uint32_t num, struct rangeElement* startOfList) {
  assert(num >= 0);
  if (startOfList == NULL) {
    startOfList = rangeCreate();
    startOfList->start = num;
    startOfList->rangetype = GREATEREQUAL;
    return startOfList;
  }
  struct rangeElement* ptr = startOfList;
  //advance to the end of list
  while (ptr->next != NULL) {
    ptr = ptr->next;
  }
  ptr->next = rangeCreate();
  ptr = ptr->next;
  ptr->start = num;
  ptr->rangetype = GREATEREQUAL;
  return startOfList;
}

/**
* Add a single element to list
*
* Add a number, create new list if necessary
*
* @param num number to add 
* @param startOfList list
* @return list
*/
struct rangeElement* rangeAddSingle(uint32_t num, struct rangeElement* startOfList) {
  assert(num > 0);
  if (startOfList == NULL) {
    startOfList = rangeCreate();
    startOfList->start = num;
    startOfList->rangetype = SINGLE;
    return startOfList;
  }
  struct rangeElement* ptr = startOfList;
  //advance to the end of list
  while (ptr->next != NULL) {
    ptr = ptr->next;
  }
  ptr->next = rangeCreate();
  ptr = ptr->next;
  ptr->start = num;
  ptr->rangetype = SINGLE;
  return startOfList;
}

/**
* format as string
*
* pass in the buffer
*
* @param buf buffer for output
* @param bufsize size of buffer
* @param element range element to format
* @return buf
*/
char* rangeElementToString(char* buf, uint32_t bufsize, struct rangeElement* element) {
  assert(bufsize >= 0);
  assert(element != NULL);
  switch (element->rangetype) {
  case SINGLE:
    snprintf(buf, bufsize, "[%d]", element->start);
    return buf;
  case GREATEREQUAL:
    snprintf(buf, bufsize, "[%d-]", element->start);
    return buf;
  case STARTEND:
    snprintf(buf, bufsize, "[%d-%d]", element->start, element->end);
    return buf;
  case EMPTY:
    snprintf(buf, bufsize, "[]");
    return buf;
  default:
    return buf;
  }
}

/**
* format range list as string
*
* pass in the buffer
*
* @param buf buffer for output
* @param bufsize size of buffer
* @param element range element to format
* @return buf
*/
char* rangeListToString(char* buf, uint32_t bufsize, struct rangeElement* startOfList) {
  int tmpbufsize = 256;
  char tmpbuf[tmpbufsize];
  struct rangeElement* ptr = startOfList;
  //should I?
  memset(buf, 0, bufsize);
  int bufidx = 0;
  while (ptr != NULL) {
    memset(tmpbuf, 0, tmpbufsize);
    rangeElementToString(&tmpbuf[0], tmpbufsize, ptr);
    int tblen = sizeof(tmpbuf);
    // try not to buffer overflow
    int copylen = ( (tblen+bufidx) >= bufsize ? bufsize-bufidx : tblen);
    strncat(buf, tmpbuf, copylen);
    bufidx += copylen;
    ptr = ptr->next;
  }
  return buf;
}

/**
* test if num in range
*
* @param num number to check
* @param startOfList range
* @return true if num in range
*/
bool rangeContainsNum(uint32_t num, struct rangeElement* startOfList) {
  assert(startOfList != NULL);
  assert(num >= 0);
  struct rangeElement* ptr = startOfList;
  while(true) {
    if (ptr->rangetype == SINGLE && ptr->start == num) {
      return true;
    }
    if (ptr->rangetype == GREATEREQUAL && num >= ptr->start) {
      return true;
    }
    if (ptr->rangetype == STARTEND && num >= ptr->start && num <= ptr->end) {
      return true;
    }
    ptr = ptr->next;
    if (ptr == NULL) {
      return false;
    }
  }
}

/**
* parse a string to a range list
*
* @param text input string
* @return rangelist
*/
struct rangeElement* parseIntRanges(const char* text) {
  int state = 0;
  int flag_dash = 0x01;
  int i;
  char numbuf[1024];
  int numbufi;
  struct rangeElement* rv = NULL;
  int start=0;
  int end=0;
  
  memset(numbuf, 0, sizeof(numbuf));
  numbufi = 0;
  
  for (i=0; i<strlen(text); i++) {
    if (text[i] >= '0' && text[i] <= '9') {
      numbuf[numbufi] = text[i];
      numbufi++;
    }
    else if (text[i] == '-') {
      if (state & flag_dash) {
	fprintf(stderr, "error: too many dashes in range\n");
	exit(-1);
      }
      state |= flag_dash;
      if (numbufi == 0) {
	// nothing stored for start
	fprintf(stderr, "error: range cannot start with '-'\n");
	exit(-1);
      }
      start = atoi(numbuf);
      if (start <= 0) {
	fprintf(stderr, "error: start range <= 0\n");
	exit(-1);
      }
      memset(numbuf, 0, sizeof(numbuf));
      numbufi = 0;
    }
    else if (text[i] == ',') {
      if (numbufi == 0) {
	if (flag_dash) {
	  rv = rangeAddGreaterEqual(start, rv);
	}
      }
      else if (start > 0) {
	end = atoi(numbuf);
	if (end <= 0 || start > end) {
	  fprintf(stderr, "error: illegal start/end ranges\n");
	  exit(-1);
	}
	rv = rangeAddStartEnd(start, end, rv);
      }
      else {
	start = atoi(numbuf);
	rv = rangeAddSingle(start, rv);
      }
      memset(numbuf, 0, sizeof(numbuf));
      numbufi = 0;
      start = 0;
      end = 0;
      state = 0;
    }
  }
  // finish up
  if (numbufi == 0) {
    if (flag_dash) {
      rv = rangeAddGreaterEqual(start, rv);
    }
  }
  else if (start > 0) {
    end = atoi(numbuf);
    if (end <= 0 || start > end) {
      fprintf(stderr, "error: illegal start/end range %s\n", text);
      exit(-1);
    }
    rv = rangeAddStartEnd(start, end, rv);
  }
  else {
    start = atoi(numbuf);
    rv = rangeAddSingle(start, rv);
  }
  return rv;
}
