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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#include "range.h"
#include "csvline.h"

#define BUFSIZE 4096
#define MAXCOL 1024


/*
-i infile (default: stdin")
-o outfile (default: stdout")

-d delimiter char
-q quote char

-b allow binary

-c output column order (1,4   1,4-8,11-  )

-v verbose (messages to stderr)
*/


// Globals
int32_t gLineCnt;

//Global parameters
FILE* gpInput;
FILE* gpOutput;
char gpDelimIn;
char gpDelimOut;
char gpQuoteIn;
char gpQuoteOut;
bool gpAllowBinaryFlag = false;
int gpVerbose;
struct rangeElement* gpOutColumns = NULL;

int rvStateNormal = 0;
int rvStateMultiline = 0x01;
int rvStateEOF = 0x02;
int rvDelim = 0x04;

void outputLine(struct csvline* cline);

void usage(FILE* fp) {
  char* help =
    "cissy [options]\n" 
    "\t-i <inputfile>\t\t (defaults to stdin)\n" 
    "\t-o <outputfile>\t\t (defaults to stdout)\n" 
    "\n"
    "\t-c <columns>\t\t specify columns to output eg. [2][5-8][12-]\n"
    "\t-d <delimiter>\t\t set the input and output delimiter\n"
    "\t\t\t\t defaults to ','\n"
    "\t-di <input delimiter>\t set the input delimiter\n" 
    "\t-do <output delimiter>\t set the output delimiter\n"
    "\n"
    "\t-q <quote character>\t defaults to \"\n"
    "\t-qi <quote input character>\n"
    "\t-qo <quote output character>\n"
    "\t-b \t\t\t allow binary data\n"
    "\t-v \t\t\t send processing info to stderr\n"
    "\t-h \t\t\t help\n"
    ""
    ;
  fprintf(fp, help);
}

void debug(int level, char* fmt, ...) {
  if (level <= gpVerbose) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
  }
}

bool isBinChar(char c) {
  switch(c) {
  case 1 ... 8:
  case 11:
  case 12:
  case 14 ... 26:
  case 28 ... 31:
  case 127:
    return true;
  }
  return false;
}

int main(int argc, char** argv) {
  //init
  gpInput=stdin;
  gpOutput=stdout;
  gpDelimIn = ',';
  gpDelimOut = ',';
  gpQuoteIn = '"';
  gpQuoteOut = '"';
  gpVerbose = 0;

  
  int arginc = 1;
  while(arginc < argc) {
    debug(5, "argc=(%d) arginc=(%d)  argv[arginc]=(%s)\n", argc, arginc, argv[arginc]);
    if (strcmp(argv[arginc], "-i")==0) {
      if (argc <= arginc+1) {
	fprintf(stderr, "error: missing argument for '%s'\n", argv[arginc]);
	return -1;
      }
      arginc++;
      debug(5, "arg %s\n", argv[arginc]);
      gpInput = fopen(argv[arginc], "r");
      if (gpInput == NULL) {
	fprintf(stderr, "error: unable to open (%s) for gpInput\n", argv[arginc]);
	return -1;
      }
      arginc++;
    }
    else if (strcmp(argv[arginc], "-o")==0) {
      if (argc <= arginc+1) {
	fprintf(stderr, "error: missing argument for '%s'\n", argv[arginc]);
	return -1;
      }
      arginc++;
      debug(5, "arg %s\n", argv[arginc]);
      gpOutput = fopen(argv[arginc], "r");
      if (gpOutput == NULL) {
	fprintf(stderr, "error: unable to open (%s) for gpInput\n", argv[arginc]);
	return -1;
      }
      arginc++;
    }
    else if (strcmp(argv[arginc], "-d")==0) {
      if (argc <= arginc+1) {
	fprintf(stderr, "error: missing argument for '%s'\n", argv[arginc]);
	return -1;
      }
      arginc++;
      if (strlen(argv[arginc]) != 1) {
	fprintf(stderr, "error: only single character delimiters allowed: '%s'\n", argv[arginc]);
	return -1;
      }
      debug(5, "arg %s %d\n", argv[arginc], strlen(argv[arginc]));
      gpDelimIn = argv[arginc][0];
      gpDelimOut = argv[arginc][0];
      arginc++;
    }
    else if (strcmp(argv[arginc], "-di")==0) {
      if (argc <= arginc+1) {
	fprintf(stderr, "error: missing argument for '%s'\n", argv[arginc]);
	return -1;
      }
      arginc++;
      if (strlen(argv[arginc]) != 1) {
	fprintf(stderr, "error: only single character delimiters allowed: '%s'\n", argv[arginc]);
	return -1;
      }
      debug(5, "arg %s %d\n", argv[arginc], strlen(argv[arginc]));
      gpDelimIn = argv[arginc][0];
      arginc++;
    }
    else if (strcmp(argv[arginc], "-do")==0) {
      if (argc <= arginc+1) {
	fprintf(stderr, "error: missing argument for '%s'\n", argv[arginc]);
	return -1;
      }
      arginc++;
      if (strlen(argv[arginc]) != 1) {
	fprintf(stderr, "error: only single character delimiters allowed: '%s'\n", argv[arginc]);
	return -1;
      }
      debug(5, "arg %s %d\n", argv[arginc], strlen(argv[arginc]));
      gpDelimOut = argv[arginc][0];
      arginc++;
    }
    else if (strcmp(argv[arginc], "-q")==0) {
      if (argc <= arginc+1) {
	fprintf(stderr, "error: missing argument for '%s'\n", argv[arginc]);
	return -1;
      }
      arginc++;
      debug(5, "arg %s %d\n", argv[arginc], strlen(argv[arginc]));
      gpQuoteIn = argv[arginc][0];
      arginc++;
    }
    else if (strcmp(argv[arginc], "-qi")==0) {
      if (argc <= arginc+1) {
	fprintf(stderr, "error: missing argument for '%s'\n", argv[arginc]);
	return -1;
      }
      arginc++;
      if (strlen(argv[arginc]) != 1) {
	fprintf(stderr, "error: only single character delimiters allowed: '%s'\n", argv[arginc]);
	return -1;
      }
      debug(5, "arg %s %d\n", argv[arginc], strlen(argv[arginc]));
      gpQuoteIn = argv[arginc][0];
      arginc++;
    }
    else if (strcmp(argv[arginc], "-qo")==0) {
      if (argc <= arginc+1) {
	fprintf(stderr, "error: missing argument for '%s'\n", argv[arginc]);
	return -1;
      }
      arginc++;
      if (strlen(argv[arginc]) != 1) {
	fprintf(stderr, "error: only single character delimiters allowed: '%s'\n", argv[arginc]);
	return -1;
      }
      debug(5, "arg %s %d\n", argv[arginc], strlen(argv[arginc]));
      gpQuoteOut = argv[arginc][0];
      arginc++;
    } 
    else if (strcmp(argv[arginc], "-c")==0) {
      if (argc <= arginc+1) {
	fprintf(stderr, "error: missing argument for '%s'\n", argv[arginc]);
	return -1;
      }
      arginc++;
      debug(5, "arg %s %d\n", argv[arginc], strlen(argv[arginc]));
      gpOutColumns = parseIntRanges(argv[arginc]);
      arginc++;
    }
    else if (strcmp(argv[arginc], "-h")==0) {
      usage(stdout);
      exit(0);
    }    
    else if (strcmp(argv[arginc], "-v")==0) {
      gpVerbose++;
      arginc++;
    }
    else if (strcmp(argv[arginc], "-b")==0) {
      gpAllowBinaryFlag = true;
      arginc++;
    }
    else {
      fprintf(stderr, "error: unknown switch (%s)\n", argv[arginc]);
      fclose(gpInput);
      fclose(gpOutput);
      return -1;
    }
  }
  debug(5, "main: input delimiter(%c) output delimiter(%c)\n", gpDelimIn, gpDelimOut);
  debug(5, "main: input quote(%c) output quote(%c)\n", gpQuoteIn, gpQuoteOut);
  char sbuf[1024]; //FIXME
  debug(5, "main: OutputColumns = %s\n", rangeListToString(sbuf, sizeof(sbuf), gpOutColumns));
  debug(5, "main:start\n");
    
  int psAccumFlag = 0x01;
  int psBeginFlag = 0x02;
  int psQuotedFlag = 0x04;
  
  int parseState = psBeginFlag;
  
  char colData[BUFSIZE];
  size_t colDataLen[MAXCOL];
  char* colPtr[MAXCOL];
  //init
  {
    memset(colData, 0, BUFSIZE);
    int i;
    for (i=0; i<MAXCOL; i++) {
      colPtr[i] = NULL;
      colDataLen[i] = 0;
    }
  }
  
  char* rawInput = NULL;
  size_t rawInputSize = 128;
  // getline malloc/realloc
  rawInput = malloc(rawInputSize);
  if (rawInput == NULL) {
    fprintf(stderr, "main: rawInput malloc error\n");
    exit(-1);
  }

  bool bdone = false;
  gLineCnt = 0;

  while (! bdone) {
    struct csvline* cline = csvline_create();
    memset(rawInput, 0, rawInputSize);
    int c = getline(&rawInput, &rawInputSize, gpInput);
    gLineCnt++;
    debug(5, "main: line(%d) read (%d) bytes\n", gLineCnt, c);
    if (c == -1 || c == 0) {
      debug(10, "main: EOF\n");
      //EOF ... we're done
      fclose(gpInput);
      fclose(gpOutput);
      return 0;
    }
    int startIdx = 0;
    int fieldLen = 0;
    int bufsize = c - startIdx;
    bool bline = false;

    int d=0;
    while (!bline) {
      bool bInsideQuote = false;
      int parseState = getField(&rawInput[startIdx], bufsize-startIdx, &fieldLen, &bInsideQuote);
      //debug(50, "s(%d)e(%d)\n", startIdx, fieldLen);
      csvline_addField(cline, rawInput, startIdx, fieldLen);
      startIdx += fieldLen;
      if (parseState == rvDelim) {
	startIdx++;
      }
      if (parseState == rvStateEOF) {
	if (bInsideQuote) {
	  c = getline(&rawInput, &rawInputSize, gpInput);
	  if (c == -1 || c == 0) {
	    fprintf(stderr, "error: unterminated quote\n");
	    fclose(gpInput);
	    fclose(gpOutput);
	    exit(-1);
	  }
	  //reset line
	  startIdx = 0;
	  fieldLen = 0;
	  bufsize = c - startIdx;
	  fprintf(stderr, "error: to-do: multiline\n");
	}
	else {
	  bline=true;
	}
      }
    }
    if (gpVerbose > 2) {
      csvline_printToFile(cline, stderr);
    }
    outputLine(cline);
  }
  return 0;
}

/*
 *  Change quote char if needed.
 */
void formatOutput(char** str) {
  debug(100, "formatOutput:start\n");
  if (gpQuoteIn == gpQuoteOut) {
    return;
  }
  int slen = strlen(*str);
  if (slen < 2) {
    return;
  }
  debug(100, "formatOutput:slen(%d)\n", slen);
  debug(100, "formatOutput:str[%d](%c) str[%d](%c)\n", 0, (*str)[0], slen-1, (*str)[2]);
  if ((*str)[0] == gpQuoteIn && (*str)[slen-1] == gpQuoteIn) {
    // modify in place for efficiency
    debug(100, "formatOutput:quoted\n");
    (*str)[0] = gpQuoteOut;
    (*str)[slen-1] = gpQuoteOut;
  }
  
  return;
}

void outputLine(struct csvline* cline) {
  debug(50,"outputLine:start\n");
  if (cline == NULL) return;
  if (gpOutColumns == NULL) { // print all
    debug(50,"outputLine:printall\n");
    int fieldcnt = csvline_getFieldCnt(cline);
    int i;
    for (i=0; i<fieldcnt; i++) {
      char* str = csvline_getField(cline,i);
      formatOutput(&str);
      fprintf(gpOutput, "%s", str);
      if ( (i+1) != fieldcnt) {
	fprintf(gpOutput, "%c", gpDelimOut);
      }
    }    
  }
  else {
    debug(50,"outputLine:printranges\n");
    struct rangeElement* list = gpOutColumns;
    int colCnt = csvline_getFieldCnt(cline);
    do {
      switch (list->rangetype) {
      case EMPTY:
	break;
      case SINGLE:
	debug(50,"outputLine:single(%d)\n", list->start);
	char* str = csvline_getField(cline,list->start - 1);
	formatOutput(&str);
	if (strlen(str) == 0) {
	  //don't print (null)
	}
	else {
	  fprintf(gpOutput, "%s",str);
	}
	if (list->next != NULL) {
	  fprintf (gpOutput, "%c", gpDelimOut);
	}
	break;
      case STARTEND:
	debug(50,"outputLine:startend (%d - %d)\n", list->start, list->end);
	{
	  int i=0;
	  char* str = NULL;
	  for (i=list->start - 1; (i+1) < list->end; i++) {
	    str = csvline_getField(cline,i);
	    formatOutput(&str);
	    if (strlen(str) == 0) {
	      fprintf(gpOutput, "%c", gpDelimOut);
	    }
	    else {
	      fprintf(gpOutput, "%s%c", str, gpDelimOut);
	    }
	  }
	  int last = list->end - 1;
	  if (last >= colCnt) {
	    break;
	  }
	  str = csvline_getField(cline, last);
	  formatOutput(&str);
	  if (strlen(str) == 0) {
	  }
	  else {
	    fprintf(gpOutput, "%s",str);
	  }
	}
	if (list->next != NULL) {
	  fprintf (gpOutput, "%c", gpDelimOut);
	}
	break;
      case GREATEREQUAL:
	debug(50,"outputLine:greaterequal\n");
	{
	  int i;
	  char* str = "";
	  for (i=list->start - 1; (i+1)<colCnt; i++) {
	    str =  csvline_getField(cline,i);
	    formatOutput(&str);
	    if (strlen(str) == 0) {
	    }
	    else {
	      fprintf(gpOutput, "%s%c", str, gpDelimOut);
	    }
	  }
	  int last = colCnt - 1;
	  str =  csvline_getField(cline,last);
	  formatOutput(&str);
	  if (strlen(str) == 0) {
	  }
	  else {
	    fprintf(gpOutput, "%s",str);
	  }
	}
	if (list->next != NULL) {
	  fprintf (gpOutput, "%c", gpDelimOut);
	}
	break;
      default:
	debug(50,"outputLine:default (error)\n");
	break;
      }
      list = list->next;
    }
    while (list != NULL);
  }
  fprintf(gpOutput, "%s", cline->eolStr);
  debug(50,"outputLine:end\n");
  return;
}

// find field
// return state
int getField(char* buf, int buflen, int* end, bool* inQuoted) {
  bool bquoted = false;
  for (*end=0; *end<buflen; *end += 1) {
    char c = buf[*end];
    //debug(50, "char(%c)(%d)(%d)(%d)\n", c, (int)c, *end, buflen);
    if (!gpAllowBinaryFlag && isBinChar(c)) {
      fprintf(stderr, "error: line(%d): binary character (%d) found.  Use flag to ignore/pass\n", gLineCnt, (int)c);
      exit(-1);
    }
    if (*inQuoted) {
      if (c==gpQuoteIn) {
	*inQuoted=false;
      }
    }
    else {
      if (c==gpQuoteIn) {
	*inQuoted=true;
      }
      if (c==gpDelimIn) {
	return rvDelim;
      }
      if (c=='\r') {
	return rvStateEOF;
      }
      if (c=='\n') {
	return rvStateEOF;
      }
      //if (buf[*end]=='\n' && *end == buflen-1) {
      //	return rvStateEOF;
      //}
      //if (buf[*end]=='\r' && *end == buflen-1) {
      //	return rvStateEOF;
      //}
      if (c=='\0' && *end == buflen-1) {
	return rvStateEOF;
      }      
    }
  }
  return rvStateEOF;
}
