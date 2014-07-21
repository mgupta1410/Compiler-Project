
#ifndef LMN
#define LMN
#include"lexerDef.h"
#include"tree.h"
#define TABLE_SIZE 256
struct nodeHashTable
{
  int type;// INT -> 9, real -> 10, string -> 11, matrix -> 12
  tokenInfo info; // to be replaced by tokenInfo;
  int scopeDepth;
  tree funcPtr;
  int m1;
  int m2;
  int s;
  int offset;
};

int parent[256];
char mapV[256][256];
typedef struct nodeHashTable nodeHT;
#endif
