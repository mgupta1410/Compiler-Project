//Team Number 7
//Danish      2011A7PS037P
//Mansi Gupta 2011A7PS023P
#ifndef mac
#define mac
#define LINES 86
struct Rule                     //structure to store Grammar.
{	
	int lhs;
	int count;
	int rhs[50];
};
struct valRule
{
	int val,ruleNum;
};
typedef struct valRule valrule;
struct first                    //structure to store first sets.
{
	valrule list[16];
	int count;
};
struct follow                 //structure to store follow sets.
{
	int list[16];
	int lhs,rhsCount;
};

int parsing_table[100][50];  //structure to store Parse table.
typedef struct follow FOLLOW;
typedef struct Rule rule;
typedef struct first FIRST;
FOLLOW ff[200];
FIRST f[200];
rule l[LINES];
char mapTo[200][50];
#endif
