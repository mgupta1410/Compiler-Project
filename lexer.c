//Team Number 7
//Danish      2011A7PS037P
//Mansi Gupta 2011A7PS023P

#include <stdio.h>
#include <string.h> // memset
#include <stdlib.h>
#include "lexerDef.h"
char tempStr[24];
FILE *sourceFile;
void openFile(const char* filename)
{
	sourceFile = fopen(filename,"r");	
}
void append(char *ptr,char x)
{
	int len = 	strlen(ptr);
	ptr[len] = x;
	ptr[len+1] = '\0';
}
void removeLast(char *ptr)
{
		int len = strlen(ptr);
		if(len>0)
		{
			ptr[len-1] = '\0';
		}
}
tokenInfo errorToken(int l)
{
	tokenInfo temp;
	temp.tokenId = -1;
	temp.line = l;
	tempStr[0] = '\0';
	printf("\n ERROR @ line num: %d",l);
	return temp;
}
tokenInfo endToken()
{
	tokenInfo temp;
	temp.tokenId = -2;
	tempStr[0] = '\0';
	return temp;
}
void getStream(FILE *fp,int k) //Interface function, getting stream from the input file.
{	
	totalChars = fread(buff,sizeof(char),k,fp);
}
tokenInfo makeToken(int id,const char *lex,const char* tok,int l)
{
	tokenInfo temp;
	temp.tokenId = id;
	strncpy(temp.lexeme,lex,strlen(lex));
	strncpy(temp.token,tok,strlen(tok));
	temp.lexeme[strlen(lex)] = '\0';
	temp.token[strlen(tok)] = '\0';
	temp.line = l;
	tempStr[0] = '\0';
	return temp;
}
tokenInfo getNextToken()                //Interface function, getting next token from the input stream.
{
	// implement the DFA
		int currState = 0;				
		int i;
		if(totalChars == 0)
		{
			getStream(sourceFile,BUFFER_SIZE);
		}
		for(i=startOfToken;i<=totalChars;i++)
		{		
			if(i==totalChars)
			{
				if(totalChars < BUFFER_SIZE)
				{
					//EOF reached 
					return endToken();
				}
				else{
					getStream(sourceFile,BUFFER_SIZE);
					startOfToken = 0;
					i = -1;
					continue; 
				}
			}

			char ch = buff[i];
			char t = buff[i];
			if(t=='\n') lineCount++;
			append(tempStr,ch);
		  
			if(currState == COMMENT)
			{	
				removeLast(tempStr);
				if(buff[i]=='\n')
					{
						currState = 0;
						startOfToken = i+1;
					}
				continue;
			}
			if(currState == FUNCTION)
			{
				if((buff[i]>='a' && buff[i]<='z')||(buff[i]>='A' && buff[i]<='Z'))	
				{
					currState = 100;
				}
				
				else
				{	
					startOfToken = i+1;
					return errorToken(lineCount);
				}
			}
			if(currState == 100)
			{
			 if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
			 	continue;
			 	
			 }
				else{
					removeLast(tempStr);
					startOfToken = i;
					i--;
					if(strcmp(tempStr,"_main")==0)
					{
						return makeToken(13,tempStr,"MAIN",lineCount);		
					}	
					return makeToken(3,tempStr,"FUNID",lineCount);	
				}
			}
			if(currState == DIGIT)
			{
				if(buff[i]>='0' && buff[i]<='9')
				{
					continue;
				}
				else if(buff[i]=='.'){
					currState = 151;
					continue;
				}
				else{
					removeLast(tempStr);
					startOfToken = i;
					i--;
					return makeToken(5,tempStr,"NUM",lineCount);
				}
			}
			if(currState == 151)
			{
				
				if(buff[i]>='0' && buff[i]<='9')
					{
						currState = 152;
						continue;
					}
				else
				{	
					startOfToken = i+1;
					return errorToken(lineCount);
				}
			}
			if(currState == 152)
			{
				if((buff[i]>='0' && buff[i]<='9')&&(buff[i+1]<'0' || buff[i+1]>'9'))
				{
						startOfToken = i+1;
						return makeToken(6,tempStr,"RNUM",lineCount);
				}
				else
				{	
					startOfToken = i+1;
					return errorToken(lineCount);
				}
				
			}
			if(currState == STRING)
			{
				if(buff[i]==' ' ||(buff[i]>='a' && buff[i]<='z')||(buff[i]>='A'&&buff[i]<='Z'))
				{	
					if(strlen(tempStr)>=22)
					{
						printf("\nERROR_1:Identifier at line num %d is longer than the prescribed length of 20",lineCount);
						startOfToken = i+1;
						return errorToken(lineCount);
					}
					continue;
				}
				else if(buff[i]=='"')
				{
					startOfToken = i+1;
					if(strlen(tempStr)>=22)
					{
						printf("\neERROR_1:Identifier at line num %d is longer than the prescribed length of 20",lineCount);
						startOfToken = i+1;
						return errorToken(lineCount);
					}
					return makeToken(7,tempStr,"STR",lineCount);
				}
				else
				{	
					startOfToken = i+1;
					return errorToken(lineCount);
				}
			}
			if(currState == 0)
			{
				if(buff[i]=='"')
				{
					currState = STRING;
					continue;
				}
				if(buff[i]=='#')
				{
					currState = COMMENT;
					removeLast(tempStr);
					continue;
				}
				if(buff[i]=='_')
				{
					currState = FUNCTION;
					continue;
				}
				if(buff[i]>='0' && buff[i]<='9')
				{
					currState = DIGIT;
					continue;
				}
				if(buff[i] == ' ')
				{
					startOfToken = i+1;
					removeLast(tempStr);
					continue;
				}
				if(buff[i] == '\t')
				{
					startOfToken = i+1;
					removeLast(tempStr);
					continue;
				}
				if(buff[i] == '\n')
				{
					startOfToken = i+1;
					removeLast(tempStr);
					continue;
				}
				if(buff[i] == '[')
				{
					startOfToken = i+1;
				  return makeToken(14,tempStr,"SQO",lineCount);	
				}
				if(buff[i] == ']')
				{
					startOfToken = i+1;
				  return makeToken(15,tempStr,"SQC",lineCount);	
				}
				if(buff[i] == '(')
				{
					startOfToken = i+1;
				  return makeToken(16,tempStr,"OP",lineCount);	
				}
				if(buff[i] == ')')
				{
					startOfToken = i+1;
				  return makeToken(17,tempStr,"CL",lineCount);	
				}
				if(buff[i] == ';')
				{
					startOfToken = i+1;
				  return makeToken(18,tempStr,"SEMICOLON",lineCount);	
				}
				if(buff[i] == '+')
				{
					startOfToken = i+1;
				  return makeToken(25,tempStr,"PLUS",lineCount);	
				}
				if(buff[i] == ',')
				{
					startOfToken = i+1;
				  return makeToken(19,tempStr,"COMMA",lineCount);	
				}
				if(buff[i] == '-')
				{
					startOfToken = i+1;
				  return makeToken(26,tempStr,"MINUS",lineCount);	
				}
				if(buff[i] == '*')
				{
					startOfToken = i+1;
				  return makeToken(27,tempStr,"MUL",lineCount);	
				}
				if(buff[i] == '/')
				{
					startOfToken = i+1;
				  return makeToken(28,tempStr,"DIV",lineCount);	
				}
				if(buff[i] == '@')
				{
					startOfToken = i+1;
				  return makeToken(29,tempStr,"SIZE",lineCount);	
				}
			  if(buff[i]=='=')
			 	{
					currState = 1;
					continue;
				}
				if(buff[i]=='.')
				{
					currState = 3;
					continue;
				}
				if(buff[i]=='>')
				{
					currState = 12;
					continue;
				}
				if(buff[i]=='<')
				{
					currState = 13;
					continue;
				}
				if((buff[i] >='a' && buff[i]<='z') || (buff[i] >='A' && buff[i] <='Z'))
				{
						if(t!='e' && t!='m' && t!='p' && t!='s' && t!='r' )
						{
							currState = ID; 
							continue;
						}
				}
				if(buff[i] == 'e')
				{
						currState = 15;
						continue;
				}	
				if(buff[i] == 'f')
				{
						currState = 70;
						continue;
				}
				if(buff[i] == 's')
				{
						currState = 30;
						continue;
				}
				if(buff[i] == 'm')
				{
						currState = 40;
						continue;
				}
				if(buff[i] == 'p')
				{
						currState = 50;
						continue;
				}
				if(buff[i] =='r')
				{
						currState = 60;
						continue;
				}
			}

			
			if(currState == 1)
			{
				if(buff[i]!= '=' && buff[i]!='/')
				{	
					startOfToken = i;
					removeLast(tempStr);
					i--;
					return makeToken(1,"=","ASSIGNOP",lineCount);
				}
				if(buff[i] =='=')
				{
					startOfToken = i+1;
					return makeToken(35,"==","EQ",lineCount);
				}
				if(buff[i] == '/')
				{
					currState = 2;
					continue;
				}
			}
			if(currState == 2)
			{
				if(buff[i]=='=')
				{
					startOfToken = i+1;
					return makeToken(38,"=/=","NE",lineCount);
				}
				else
				{	
					startOfToken = i+1;
					return errorToken(lineCount);
				}
			}
			if(currState == 3)
			{
				 // . already found;
				 if(buff[i] == 'a')
				 {
						currState = 4;
						continue;
				 }
				 if(buff[i]=='o')
				 {	
						currState = 5;
						continue;
				 }
				 if(buff[i] == 'n')
				 {
						currState = 6;
						continue;
				 }
				 else
					{	
						startOfToken = i+1;
						return errorToken(lineCount);
					}
				 
			}
			if(currState == 4 )
			{
				// .a already found!
				if(buff[i]=='n')
				{
					currState = 7;
					continue;
				}
				else
				{	
						startOfToken = i+1;
						return errorToken(lineCount);
				}
			}
			if(currState == 5)
			{
				if(buff[i] == 'r')
				{
					currState = 8;
					continue;
				}
				else
				{	
						startOfToken = i+1;
						return errorToken(lineCount);
				}
			}
			if(currState == 6)
			{
					if(buff[i] == 'o')
					{
						currState = 9;
						continue;
					}
					else
					{	
						startOfToken = i+1;
						return errorToken(lineCount);
					}
			}
			if(currState == 7)
			{
					if(buff[i]=='d')
					{	
						currState = 10;
						continue;
					}
					else
					{	
						startOfToken = i+1;
						return errorToken(lineCount);
					}
			}
			if(currState == 10)
			{
					if(buff[i]=='.')
					{
						startOfToken = i+1;
						return makeToken(30,".and.","AND",lineCount);
					}
					else
					{	
						startOfToken = i+1;
						return errorToken(lineCount);
					}
			}

			if(currState == 8)
			{
					if(buff[i] == '.')
					{
						startOfToken = i+1;
						return makeToken(31,".or.","OR",lineCount);
					}
					else
					{	
						startOfToken = i+1;
						return errorToken(lineCount);
					}
			}
			if(currState == 9)
			{
					// .no arleady found!
					if(buff[i] == 't')
					{
						currState = 11;
						continue;
					}
					else
					{	
						startOfToken = i+1;
						return errorToken(lineCount);
					}
			}
			if(currState == 11)
			{
	    	if(buff[i] == '.')
				{
						startOfToken = i+1;
						return makeToken(32,".not.","NOT",lineCount);
				}
				else
					{	
						startOfToken = i+1;
						return errorToken(lineCount);
					}
			}
			// states used upto 11
			if(currState == 12)
			{
					if(buff[i] == '=')
					{
						 startOfToken = i+1;
						 return makeToken(37,">=","GE",lineCount);
					}
					else
					{
						 startOfToken = i;
						 return makeToken(36,">","GT",lineCount);
					}
			}
			if(currState == 13)
			{
					if(buff[i] == '=')
					{
						 startOfToken = i+1;
						 return makeToken(34,"<=","LE",lineCount);
					}
					else
					{
						 startOfToken = i;
						 return makeToken(33,"<","LT",lineCount);
							
					}
			}
		   if(currState == ID)
		   {


				if(i+1 - startOfToken > 20)
				{		startOfToken = i+1;
						printf("\nERROR_1:Identifier at line num%d is longer than the prescribed length of 20 characters\n",lineCount);
						return errorToken(lineCount);
				}
				if((buff[i]>='0' && buff[i]<='9'))
				{		startOfToken = i+1;
						if(buff[i+1]>='0' && buff[i+1]<='9')
							return errorToken(lineCount);
						return makeToken(4,tempStr,"ID",lineCount);
				}
				else if((buff[i]>='a' && buff[i]<='z') || buff[i]>='A' && buff[i]<='Z')
				{
					continue;
				}
				else{
					removeLast(tempStr);
					startOfToken = i;
					i--;
					if(strcmp(tempStr,"int")==0)
								return makeToken(9,"int","INT",lineCount);
					if(strcmp(tempStr,"if")==0)
								return makeToken(20,tempStr,"IF",lineCount);
					if(strcmp(tempStr,"function")==0)
								return makeToken(39,"function","FUNCTION",lineCount);
					return makeToken(4,tempStr,"ID",lineCount);	
				}
				 	
				
		 	}
			if(currState == 15)
			{	
				 char ch = buff[i];
		 		 if(buff[i]=='l')
				 {
					currState = 16;
					continue;
			 	 }
			 	else if(buff[i]=='n')
			 	{
					currState = 19;
					continue;
				 }
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					currState = ID;
					removeLast(tempStr);
					i--;
					continue;
			 	}
				else{
					removeLast(tempStr);
					i--;
          startOfToken = i+1;
					return makeToken(4,tempStr,"ID",lineCount);	
				}
			}
	 		if(currState == 16)
	 		{	// el already there
				if(buff[i]=='s')
				{
					currState = 17;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else{
					removeLast(tempStr);
					i--;
          startOfToken = i+1;
					return makeToken(4,tempStr,"ID",lineCount);	
				}
	 		}
	 		if(currState == 17)
			{
				if(buff[i] == 'e')
				{
					currState = 18;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else{
					removeLast(tempStr);
					i--;
          startOfToken = i+1;
					return makeToken(4,tempStr,"ID",lineCount);	
				}
	 		}
			if(currState == 18)
			{
			 	if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(21,tempStr,"ELSE",lineCount);
				}
					
				
			}
	 
	 		if(currState == 19)
	 		{	// reached en
				if(buff[i]=='d')
				{
					currState = 20;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
	 		}
			if(currState == 20)
	 		{	
				// reached end
				if(buff[i]=='i')
				{
					currState = 21;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(8,tempStr,"END",lineCount);
				}
	 		}
	 		if(currState == 21)
	 		{	
				// reached endi
				if(buff[i] == 'f')
				{
					currState = 22;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
	 		}
	 		if(currState == 22)
		 	{	// reached endif
			 	if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(22,tempStr,"ENDIF",lineCount);
				}
	 		}
	 		if(currState == 50)
			{
				// reached p
				if(buff[i]=='r')
				{
					currState = 51;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState == 51)
			{ 
				// reached pr
				if(buff[i]=='i')
				{
					currState = 52;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState == 52)
			{
				// pri already there
				if(buff[i]=='n')
				{
					currState = 53;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState == 53)
			{	// reached prin	
				if(buff[i]=='t')
				{	
					currState = 54;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState ==54 )
			{
			 	if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(24,tempStr,"PRINT",lineCount);
				}
			}
			if(currState == 30)
			{
				if(buff[i]=='t')
				{
					currState = 31;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState == 31)
			{
				if(buff[i]=='r')
				{
					currState = 32;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState == 32)
			{	// str reached
				if(buff[i] == 'i')
				{
					currState = 33;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			
			}
			if(currState == 33)
			{
				if(buff[i]=='n')
				{
					currState = 34;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}

			}
			if(currState == 34)
			{	
				if(buff[i]=='g')
				{	
					currState = 35;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState == 35)
			{ // string reached
			 	if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(11,tempStr,"STRING",lineCount);
				}
			}

			if(currState==40)
			{
				if(buff[i] == 'a')
				{
					currState = 41;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
				
			}
			if(currState == 41)
			{
				// ma reached
				if(buff[i] == 't')
				{
					currState = 42;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
	
			}
			if(currState == 42)
			{
				// mat reached
				if(buff[i] == 'r')
				{
					currState = 43;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState == 43)
			{
				
				if(buff[i] == 'i')
				{
					currState = 44;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState == 44)
			{
				if(buff[i]=='x')
				{
					currState = 45;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
				
			}
			if(currState == 45)
			{	
				//matrix reached
			 	if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(12,tempStr,"MATRIX",lineCount);
				}
			}
			if(currState == 60)
			{
				if(buff[i]=='e')
				{
					currState = 61;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState == 61)
			{
				if(buff[i]=='a')
				{	
					currState = 62;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
			}
			if(currState == 62)
			{ 
				if(buff[i]=='l')
				{
					currState = 63;
					continue;
				}
				if(buff[i]=='d')
				{
					currState = 64;
					continue;
				}
			 	else if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(4,tempStr,"ID",lineCount);
				}
				
			}
			if(currState == 63)
			{
			 	if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						i--;
						removeLast(tempStr);
						return makeToken(10,tempStr,"REAL",lineCount);
				}
				
			}
			if(currState == 64)
			{	// read already arrived
			 	 if((t<='z' && t>='a')||(t>='A' && t<='Z') ||(t>='0' && t<='9')){
					removeLast(tempStr);
					currState = ID;
					i--;
					continue;
				}
				else {
						startOfToken = i;
						removeLast(tempStr);
						return makeToken(23,tempStr,"READ",lineCount);
				}
			}



	}
}
