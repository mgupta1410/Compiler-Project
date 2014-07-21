#ifndef MACRO
#define MACRO
#include<stdio.h>
#include"parserDef.h"
#include"lexerDef.h"
#define MAXRHS 10   

struct NodeTree;
typedef struct NodeTree nodet;
typedef nodet* tree;

struct NodeTree
{
   int value;
   int countChildren;
   tree parent;//parent Node symbol has to be stored.
   tokenInfo tokeninfo; 
   tree links[MAXRHS];  
};

#endif
