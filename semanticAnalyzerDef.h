//Team Number 7
//Danish      2011A7PS037P
//Mansi Gupta 2011A7PS023P

#include<string.h>
#include<stdio.h>
#include"treeDataType.h"
#include"hashTable.h"
#define parameterList 113
#define IF 20
#define ELSE 21
#define ASSIGNOP 1
#define decStmt 120
#define funCallStmt 140 
#define INT 9
#define REAL 10
#define STRING2 11///////////
#define MATRIX 12

#define FUNID 3 
#define ID2 4//////////
#define NUM 5
#define RNUM 6
#define STR 7
#define matrixElement 170
#define PLUS 25
#define MINUS 26
#define MUL 27
#define DIV 28
#define OR 31
#define AND 30
#define NOT 32
//33-38 LT,LE etc
#define READ 23
#define PRINT 24
#define SIZE 29

#define varList 121

#define CROSS 97//return type for size of matrix
#define BOOL 98
#define ERROR 99

#define row 175
#define rows 172
nodeHT* table;

//int parent[256];
int offset[256];
int currMax; 
int scope;//for main
int *param;
int dummy;
