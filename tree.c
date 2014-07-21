//#include"grammarDataType.h"
#include "treeDataType.h"
#include<stdlib.h>
#include<stdio.h>
#include "lexer.h"
FILE *printHere;
tree create(int val,tree parent,tokenInfo tok)
{
      tree tr= (nodet*)malloc(sizeof(nodet));
      tr->value = val;
      tr->countChildren = 0;
      tr->parent = parent;
      tr->tokeninfo = tok;
      int i;
      for(i=0;i<MAXRHS;i++) tr->links[i]=NULL;
      return tr;
}
tree traverse(tree tr) 
{
   if(tr->value>=100 && tr->countChildren==0) return tr;
     int i=0;
     while(i < tr->countChildren)
     {
       if(tr->links[i]->value<100) 
        {
           i++; continue;
        }
       tree tr1 = traverse(tr->links[i]);
       if( tr1!=NULL ) return tr1;
       else i++;
     }
  return NULL;
}
tree getLocation(tree head,tokenInfo correct)
{	
	if(head->value<100 && head->tokeninfo.tokenId == -3 && head->value == correct.tokenId)
	{
		return head;
	}
	int i=0;
	while(i< head->countChildren)
	{
           tree intermed = getLocation(head->links[i],correct);
					 if(intermed!=NULL) return intermed;
					 i++; 
	
	}
	return NULL;
}
void addLexeme(tree head,tokenInfo correct)
{
		tree tr = getLocation(head,correct);
		tr->tokeninfo = correct;
}
tree insertChildren(tree head,tokenInfo temp,int ruleNo,rule* grammar)
{
   int i=0;
   tree tr = traverse(head);
   tr->countChildren=grammar[ruleNo-1].count;
	 
   for(i=0;i<grammar[ruleNo-1].count;i++)
   {	
      tr->links[i]= create(grammar[ruleNo-1].rhs[i],tr,temp);
   }
   return head;
}

void openFileForPrinting(char *nameOfFile)
{
	printHere = fopen(nameOfFile,"w");
}
void print_tree(tree tr)
{
  if(tr==NULL) return ;
	if(tr->value<100)
	{
		printf("|%20s|",tr->tokeninfo.lexeme);
	}
	else
	{
		printf("|%20s|","");
	}

        if(tr->value==0)	printf("|----|");
        else if(tr->value<100)
	{
		printf("|%4d|",tr->tokeninfo.line);
	}
	else
	{
		printf("|----|");
	}

  	if(tr->value < 100 ) printf( "|%20s|",mapTo[tr->value]);
        else                 printf( "|%20s|","");
	
	if(tr->value == 5) 	printf("|%5s|",tr->tokeninfo.lexeme);
        else             	printf("|%5s|","");

	if(tr->parent==NULL)
	{
		printf( "|%20s|","ROOT");
	}
	else
	{
		printf("|%20s|",mapTo[tr->parent->value]);
	}
	
	if(tr->value >=100)
	{
		printf("|%13s|","LEAF? No");
	}
	else
	{
		printf("|%13s|","LEAF? Yes");
	}
	
        
	printf("|%20s|",mapTo[tr->value]);
	
	printf("\n");

  int i;
  for(i=0;i<tr->countChildren;i++)
  {
    print_tree(tr->links[i]);
  }
//  printf("\n");
  return;
}
void printTree(tree tr)
{
  printf("|%20s||%4s||%20s||%5s||%20s||%13s||%20s|\n","lexeme","line","Type","Val","parentNode","leaf:y/n","currentNode");
  print_tree(tr);  
}
