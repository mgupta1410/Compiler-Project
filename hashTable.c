//Team Number 7
//Danish      2011A7PS037P
//Mansi Gupta 2011A7PS023P
#ifndef PMN
#define PMN
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"hashDef.h"
//#include"semanticAnalyzerDef.h"

nodeHT simpleTable[TABLE_SIZE];
int hashFunction(char str[])
{
   int len = strlen(str);
   int i;
   int val = 0;
   for(i=0;i<len;i++)
   {
       val += (int)str[i];
       val %= TABLE_SIZE;
   }
   return val;
}

nodeHT* createHashTable()
{
    nodeHT* hashtable = (nodeHT*) malloc(TABLE_SIZE*sizeof(nodeHT));
    int i = 0;
    for(i;i<TABLE_SIZE;i++)
    {
      hashtable[i].scopeDepth = -1; 
    }
    //nodeHT *head = &hashtable[0];
    return hashtable;;
}

int equal(nodeHT nodeOld, nodeHT nodeNew)
{
    if(strcmp(nodeNew.info.lexeme, nodeOld.info.lexeme)==0)
    {
        if(nodeOld.scopeDepth == nodeNew.scopeDepth)
        {
            return 1;
        }
        else
        {
            int scope = nodeOld.scopeDepth;
            int newScope = nodeNew.scopeDepth;
            int temp = scope;
            while(1)
            {
                temp = parent[temp];
                if(temp == 0) break;
                if(newScope == temp) return 2;
            }
            
        }
    }
    return 0;
}
int addNode(tokenInfo info, int Type, int scope,int offset,tree funcPtr, nodeHT* hashtable,int pr,int line)
{  
    ////////////////////////////////////////////
    //if id>20, give error.
    ///////////////////////////////////////////
    //lexeme is declared [21] ??
    
    nodeHT node1;
    node1.info = info;
    node1.type = Type;
    node1.scopeDepth = scope;
    node1.funcPtr = funcPtr;
    node1.offset = offset;
    tree tr = funcPtr;
     int hashVal = hashFunction(info.lexeme);
     int location = hashVal;
     
     //if lexeme and  scopedepth are same, redeclaration error (types may be different, as in, int a and string a won't work in same scope).
    int guts;  
     while(hashtable[location].scopeDepth!=-1)
     {  
        nodeHT node2 = hashtable[location];
        guts = equal(node1,node2);   
        if(guts==1 || guts == 2)
        {
            if(pr==1)
            {
                printf("Line %d: The variable '%s' is declared more than once.\n",line,node1.info.lexeme);
            }
            
        if(guts==1)  return -1;
        }
         location ++;
         location %= TABLE_SIZE;
     }
     
        hashtable[location].type=Type;
        hashtable[location].info = info;
        hashtable[location].scopeDepth = scope;
        hashtable[location].funcPtr = funcPtr;
        hashtable[location].offset = offset;
     
    return 0;
}


int findNode(tokenInfo info, int scope,  nodeHT* hashtable) //returns location of a lexeme in a given scope.
{
    int location = hashFunction(info.lexeme);
    
    nodeHT node1;
    node1.info = info;
    node1.type = 0;
    node1.scopeDepth = scope;
    
    while(hashtable[location].scopeDepth!=-1)
     {
         if(equal(hashtable[location], node1) == 1)
         {
            //found!
            return location;
         }
         nodeHT node2 = hashtable[location];
         location ++;
         location %= TABLE_SIZE;
     }
    return -1;
     
}
int curr = 0;

void printHashTable()
{
    int i =0;
    for(i = 0;i<curr;i++)
    {       
            if(simpleTable[i].type!=3)
             printf("%20s|%20s|%20s|            %d\n",simpleTable[i].info.lexeme,mapV[simpleTable[i].scopeDepth],mapTo[simpleTable[i].type],simpleTable[i].offset); 
    
    }
}
int addToSimpleTable(tokenInfo info, int Type, int scope,int offset,tree funcPtr)
{
    simpleTable[curr].scopeDepth = scope;
    simpleTable[curr].offset  = offset;
    simpleTable[curr].funcPtr = funcPtr;
    simpleTable[curr].type = Type;
    simpleTable[curr].info = info;
    curr++;
}
#endif
