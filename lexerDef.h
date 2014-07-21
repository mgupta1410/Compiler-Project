//Team Number 7
//Danish      2011A7PS037P
//Mansi Gupta 2011A7PS023P
#ifndef MACRO1
#define MACRO1
#define ID 14
#define DIGIT 150
#define BUFFER_SIZE 6400
#define COMMENT 89
#define FUNCTION 99
#define STRING 160
struct tokenInformation //returned by getNextToken();
{
   char lexeme[21];
   int tokenId; 
   char token[16];
   int line;
};
typedef struct tokenInformation tokenInfo ; 
char buff[BUFFER_SIZE];
int totalChars;
int lineCount;
char currLexeme[20];
int startOfToken;
#endif
