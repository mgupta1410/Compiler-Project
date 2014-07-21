#ifndef m
#define m
#include"treeDataType.h"
#include"parserDef.h"

extern tree insertChildren(tree head,tokenInfo dummy,int runleNo,rule* grammar);
extern tree create(int val,tree parent,tokenInfo tok);
extern tree traverse(tree tr); 
extern void addLexeme(tree head,tokenInfo tok);
extern void printTree(tree head);
extern void openFileForPrinting(char *nameOfFile);
#endif
