
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "range.h"

int main(int argc, char** argv) {
  printf("start\n");
  struct rangeElement* list = NULL;
  bool b;
  int bufsize=1024;
  char buf[bufsize];
  char* rv;

  //////////////////////////////////////////////
  list = rangeAddSingle(5, list);
  printf("list = 5\n");

  printf("test: not contains 4\n");
  b = rangeContainsNum(4, list);
  assert(b == false);
  printf("\tok\n");

  printf("test: contains 5\n");
  b = rangeContainsNum(5, list);
  assert(b == true);
  printf("\tok\n");

  printf("test: element to string\n");
  memset(buf, 0, bufsize);
  rangeElementToString(buf, bufsize, list);
  rv = "[5]";
  printf("\t%s == %s ???", rv, buf);
  assert(strcmp(rv, buf) == 0);
  printf("\tok\n");


  printf("test: list to string\n");
  memset(buf, 0, bufsize);
  rangeListToString(buf, bufsize, list);
  rv = "[5]";
  printf("\t%s == %s ???\n", rv, buf);
  assert(strcmp("[5]", buf) == 0);
  printf("\tok\n");

  printf("test: free\n");
  rangeFree(list);
  assert(true);
  printf("\tok\n");

  /////////////////////////////////////////////////
  list = NULL;
  list = rangeAddStartEnd(5, 10, list);

  printf("test b1:\n");
  b = rangeContainsNum(4, list);
  assert(b == false);
  printf("\tok\n");

  printf("test b2:\n");
  b = rangeContainsNum(5, list);
  assert(b == true);
  printf("\tok\n");

  printf("test b3:\n");
  b = rangeContainsNum(7, list);
  assert(b == true);
  printf("\tok\n");

  printf("test b4:\n");
  b = rangeContainsNum(10, list);
  assert(b == true);
  printf("\tok\n");

  printf("test b5:\n");
  b = rangeContainsNum(12, list);
  assert(b == false);
  printf("\tok\n");

  printf("test b6:\n");
  memset(buf, 0, bufsize);
  rangeElementToString(buf, bufsize, list);
  rv = "[5-10]";
  printf("\t%s == %s ???", rv, buf);
  assert(strcmp(rv, buf) == 0);
  printf("\tok\n");

  printf("test b7:\n");
  memset(buf, 0, bufsize);
  rangeListToString(buf, bufsize, list);
  rv = "[5-10]";
  printf("\t%s == %s ???", rv, buf);
  assert(strcmp(rv, buf) == 0);
  printf("\tok\n");

  rangeFree(list);

  /////////////////////////////////////////////////
  list = NULL;
  list = rangeAddStartEnd(5, 10, list);
  list = rangeAddSingle(15, list);
  list = rangeAddGreaterEqual(40, list);


  printf("test c1:\n");
  b = rangeContainsNum(1, list);
  assert(b == false);
  printf("\tok\n");

  printf("test c2:\n");
  b = rangeContainsNum(6, list);
  assert(b == true);
  printf("\tok\n");

  printf("test c3:\n");
  b = rangeContainsNum(12, list);
  assert(b == false);
  printf("\tok\n");

  printf("test c4:\n");
  b = rangeContainsNum(50, list);
  assert(b == true);
  printf("\tok\n");

  printf("test c6:\n");
  memset(buf, 0, bufsize);
  rangeElementToString(buf, bufsize, list);
  printf("\t%s", buf);
  rv = "[5-10]";
  assert(strcmp(buf, rv) == 0);
  printf("\tok\n");

  printf("test c7:");
  memset(buf, 0, bufsize);
  rangeListToString(buf, bufsize, list);
  rv = "[5-10][15][40-]";
  printf("\t%s == %s ???", rv, buf);
  assert(strcmp(buf, rv) == 0);
  printf("\tok\n");
  
  rangeFree(list);

  /////////////////////////////////////////////////
  list = NULL;
  list = rangeAddSingle(2, list);
  list = rangeAddStartEnd(2, 4, list);

  printf("testd:");
  memset(buf, 0, bufsize);
  rangeListToString(buf, bufsize, list);
  rv = "[2][2-4]";
  printf("\t%s == %s ???\n", rv, buf);
  assert(strcmp(buf, rv) == 0);
  printf("\tok\n");


  printf("test:");
  struct rangeElement* l2 = parseIntRanges("2,2-4");
  memset(buf, 0, bufsize);
  rangeListToString(buf, bufsize, l2);
  rv = "[2][2-4]";
  printf("\t%s == %s ???\n", rv, buf);
  assert(strcmp(buf, rv) == 0);
  printf("\tok\n");
  

  return 0;
}
