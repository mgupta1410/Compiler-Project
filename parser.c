#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tree.h"
#include"stack.h"
#include"lexer.h"
#include"parserDef.h"
#include"ast.h"
//#include"typeChecking.h"

int len;
int endSymbol;
stack st;
rule* grammar;
tree headTree;
int done[200];
int totalRules = LINES;
FILE *fp1;
FILE *fp2;
void mapper()
{
	FILE *fp;
	fp = fopen("mapping.txt","r");
	int dummy;
	char str[32];
	while(fscanf(fp,"%d %s",&dummy,str)!=EOF)
	{		
			strncpy(mapTo[dummy],str,strlen(str));
	}
	fclose(fp);
}
void calculateFirst(int start,int rule)
{	
	int i,j,k;
	if(done[start])
	{
		return ;
	}	
	for(i=0;i<totalRules;i++)
	{
		if(l[i].lhs == start)
		{	
				int j;
				if(l[i].rhs[0]>=100)
				{
					calculateFirst(l[i].rhs[0],i);
					for(j=0;j<f[l[i].rhs[0]].count;j++)
					{
						f[start].list[f[start].count].val = f[l[i].rhs[0]].list[j].val;
						f[start].list[f[start].count].ruleNum = rule;
						f[start].count++;
					}
				}
				else{
					
					f[start].list[f[start].count].val = l[i].rhs[0];
					f[start].list[f[start].count].ruleNum = i;
					f[start].count++;
				}
		}
	}
	done[start] = 1;
	return ;
}
void getFollow()
{
	
	int i,j,k;
	fp2 = fopen("hackingFollow.txt","r");
	char dch;
	int dleft,dright;
	int countLines = 44; // lines in hackingFollow.txt
	for(i=0;i<200;i++)
	{
		ff[i].rhsCount = 0;
	}
	for(k=0;k<countLines;k++)
	{		
			
			fscanf(fp2,"%d %d%c",&dleft,&dright,&dch);
			int counter = 0;
			ff[dleft].list[counter] = dright;
			counter++;
			ff[dleft].lhs = dleft;
			while(dch!='\n')
			{
				if(!fscanf(fp2,"%d%c",&dright,&dch)) break;
				ff[dleft].list[counter] = dright;
				counter++;
	
			}
			ff[dleft].rhsCount = counter;
			
	}
	
}
void createParseTable()
{
	int i,j,k;		
	for(i=0;i<100;i++)
		for(j=0;j<50;j++)	
			parsing_table[i][j] = -1;
	for(i=0;i<LINES;i++)
	{		
			int ePresent = 1;
			for(j=0;j<l[i].count && ePresent;j++)
			{	
					ePresent = 0;
					if(l[i].rhs[j]==0){
					//	parsing_table[l[i].lhs][l[i].rhs[j]] = i;
						ePresent = 1;
						continue;
					}
					else if(l[i].rhs[j]<100)
					{
						parsing_table[l[i].lhs-100][l[i].rhs[j]] = i;
						break;
					}
					for( k=0;k<f[l[i].rhs[j]].count;k++)
					{
							if(f[l[i].rhs[j]].list[k].val == 0)
							{
								ePresent = 1;
							//arsing_table[l[i].lhs][f[l[i].rhs[j]].list[k].val] = i;
							}
							parsing_table[l[i].lhs-100][f[l[i].rhs[j]].list[k].val] = i;
					}
			}
			
			if(ePresent)
			{
				for(j=0;j<ff[l[i].lhs].rhsCount;j++)
				{
					parsing_table[l[i].lhs-100][ff[l[i].lhs].list[j]] = i;
				}
			}
			
	}
	for(i=0;i<100;i++)
	{
		for(j=0;j<50;j++)
		if(parsing_table[i][j]!=-1) parsing_table[i][j]++;
	}
}

void generateFirstFollowParseTable(char* grammarFile)
{	
	int i,j,k;
	for(i=0;i<200;i++)
	{
		done[i]  = 0;
	}
	fp1 = fopen(grammarFile,"r");
	for(i=0;i<totalRules;i++)
	{	
		int d,left,right;
		char ch;
		fscanf(fp1,"%d %d -> %d%c",&d,&left,&right,&ch);
		l[i].lhs = left;
		l[i].rhs[l[i].count] = right;
		l[i].count++;
		while(ch!='\n')
		{
			if(!fscanf(fp1,"%d%c",&right,&ch)) break;
			l[i].rhs[l[i].count] = right;
			l[i].count++;
		}
	}
		fclose(fp1);
	for(i=0;i<totalRules;i++)
	{
		calculateFirst(l[i].lhs,i);
	}
	getFollow();
	createParseTable();
}

void pushIntoStack(int rule)
{
   int j;
   pop(&st);
   for(j=grammar[rule-1].count-1;j>=0;j--)
      push(grammar[rule-1].rhs[j],&st);
}


int parseInputSource(char *sourceFile,char *grammarFile) // returns whether syntactically correct or not!
{	 
	 mapper();
	 generateFirstFollowParseTable(grammarFile);
         st = createStack();
         grammar = l;
	 openFile(sourceFile);
	 lineCount = 1;
	 startOfToken = 0;
	 tokenInfo temp;
	 endSymbol =-2;
	 temp = getNextToken();
         push(endSymbol,&st);
         push(100,&st);
	 tokenInfo raddi;
	 raddi.tokenId = -3;
         headTree = create(100,NULL,raddi);
	 tree dummy = headTree;
	 while(temp.tokenId!=-1)
	 {
     if(temp.tokenId == endSymbol && top(st)==endSymbol) 
		 {	
		 		return 1;
     }
		 if((top(st))==0) //is e 
     {
      pop(&st);
     }
     else if(top(st)>=100) //is non terminal
      {
         int c = temp.tokenId;
         int r = top(st)-100;
         int rule = parsing_table[r][c];
         //headTree.ruleNo = rule;
         if(rule==-1) {printf("SYNTAX_ERROR: Found :TokenId %s  ::  Expecting: TokenId %s \n",mapTo[c],mapTo[top(st)]);return 0;}
         headTree = insertChildren(headTree,raddi,rule,grammar);//tree

         pushIntoStack(rule);
      }
      else //if terminal
      {
         if(top(st)==temp.tokenId) 
         {				addLexeme(dummy,temp);
						temp = getNextToken();
						
            pop(&st);
         }
         else {
           printf("ERROR_5: The token %s for lexeme %s does not match at line %d.The expected token here is %d",temp.token,temp.lexeme,temp.line,top(st));
					 return 0;
         }
			
	 	  }
		}
		return 0;
}
void printParseTree(char *file)
{	
	openFileForPrinting(file);
//    printf("1\n");
    //headTree = removeTerm(headTree);
    //headTree = removeChaining(headTree);
    //printf("%s, %d\n",mapTo[headTree->value], headTree->countChildren);
    //printf("%s, %d\n",mapTo[headTree->links[0]->value], headTree->links[0]->countChildren);
    //printf("%s, %d\n",mapTo[headTree->links[0]->links[0]->value], headTree->links[0]->countChildren);
    //
    //printTree(headTree);
    tree tr = makeAST(headTree); //and print it.
    printTree(tr);
    //exit(0);
   // printf("#################################################################################\n");
   // typeChecker(tr);
    //display2(st);
}
void printErrorList()
{
    tree tr = makeAST(headTree);
    typeChecker(tr);
}
void printSymbolTable()
{
    tree tr = makeAST(headTree);
    typeCheckerAndPrint(tr);

}
