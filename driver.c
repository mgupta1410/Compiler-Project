#include "parser.h"
#include  <stdio.h>
int main(int argc,char *argv[])
{
    printf("LEVEL 3: Symbol table/AST/Typechecking/Semantic Rules modules work.\n");
	//printf("(a) FIRST automated and FOLLOW manually calculated and scanned from a file!\n(c) Both lexical and syntax analysis modules implemented\n");
	printf(" Press option for the defined task :\n 1: for printing the tokenList\n 2: for verifying the syntactic correctness of grammar\n 3: for printing the Abstract Syntax Tree\n 4: for printing the Symbol Table\n 5: for compiling front-end and producing ERRORS\n ");
	int choice;
	scanf("%d",&choice);
	if(choice==2)
	{
		int res = parseInputSource(argv[1],"finalGrammar.txt");
		if(res == 1)
		{
			printf("Syntax is correct!\n");
		}
		else {
			printf("Syntax not correct\n");
		}
	}
	if(choice == 1)
	{
		lineCount = 1;
		openFile(argv[1]);
		tokenInfo temp;
		temp = getNextToken();
		while(temp.tokenId!=-2)
		{
			printf("Token : %s  Line Num : %d Lexeme: %s\n",temp.token,temp.line,temp.lexeme);
			temp = getNextToken();
		}
	}
	if(choice == 3)
	{
		int res = parseInputSource(argv[1],"finalGrammar.txt");
		if(res==1)
		printParseTree(argv[2]);
		else printf("there is syntax error, can't print parse tree!\n");
	}
    if(choice == 5)
    {
        
		int res = parseInputSource(argv[1],"finalGrammar.txt");
		if(res==1)
		printErrorList();
		else printf("there is syntax error, can't print parse tree!\n");
     
    }
    if(choice == 4)
    {
		int res = parseInputSource(argv[1],"finalGrammar.txt");
		if(res==1) printSymbolTable();
		else printf("there is syntax error, can't print parse tree!\n");
    }
    if(choice == 6)
    {
       printf("Code Generation Module not made\n");
    }
    return 0;
}
