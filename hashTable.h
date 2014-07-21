//Team Number 7
//Danish      2011A7PS037P
//Mansi Gupta 2011A7PS023P
#ifndef MN
#define MN
#include"hashDef.h"
#include"lexer.h"

nodeHT* createHashTable();

int equal(nodeHT node1, nodeHT node2);

int addNode(tokenInfo info, int Type, int scope, int offset, tree funcPtr, nodeHT* hashtable, int pr,int line);

int findNode(tokenInfo info, int scope,  nodeHT* hashtable); //returns type of a lexeme in a given scope.


void printHashTable();
void addToSimpleTable(tokenInfo info,int type, int scope, int offset, tree funcPtr);
#endif
