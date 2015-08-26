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

#ifndef RANGE_H
#define RANGE_H

#include <stdbool.h>
#include <stdint.h>

struct rangeElement {
	uint32_t start;
	uint32_t end;
	enum {EMPTY, SINGLE, STARTEND, GREATEREQUAL} rangetype;
	struct rangeElement* next;
};

struct rangeElement* rangeCreate();

void rangeFree(struct rangeElement* list);

struct rangeElement* rangeAddSingle(uint32_t num,
	struct rangeElement* startOfList);

struct rangeElement* rangeAddStartEnd(uint32_t start, uint32_t end,
	struct rangeElement* startOfList);

struct rangeElement* rangeAddGreaterEqual(uint32_t num,
	struct rangeElement* startOfList);

bool rangeContainsNum(uint32_t num, struct rangeElement* startOfList);

char* rangeElementToString(char* buf, uint32_t bufsize,
	struct rangeElement* element);

char* rangeListToString(char* buf, uint32_t bufsize,
	struct rangeElement* startOfList)	;

struct rangeElement* parseIntRanges(const char* text);

#endif
