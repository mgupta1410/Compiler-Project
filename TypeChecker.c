//Team Number 7
//Danish      2011A7PS037P
//Mansi Gupta 2011A7PS023P
#ifndef MNIu
#define MNIu
#include"semanticAnalyzerDef.h"

//FUNCTION NO 2

//FUNCTION 7
//Simply returns the type of the subtree at the head (tr).
//Note that it doesn't traverse the tree.
int treetype(tree tr)
{ 
    int v = tr->value;
    if(v==NUM) return INT;
    if(v==RNUM) return REAL;
    if(v==STR) return STRING2;
    if(v==matrixElement) return INT;
    
    if(v==ID2)
    {
        int location =  findNode(tr->tokeninfo,scope,table);
        if(location!=-1 ) return table[location].type;
        else  return ERROR;
    }

    if(v==FUNID)
    { 
        int location =  findNode(tr->tokeninfo,scope,table); 
        if(location!=-1) return table[location].type;
        else return ERROR;
    }

    if(v==PLUS)
    { 
        int lhs = treetype(tr->links[0]);
        int rhs = treetype(tr->links[1]);
        if(lhs==INT || lhs==REAL || lhs==STRING2 || lhs==MATRIX)
        {
            if(lhs==rhs)
            {    
                
                
                return lhs;
            }
        }
        return ERROR;
    }

    if(v==MUL)
    { 
        int lhs = treetype(tr->links[0]);
        if(lhs==INT || lhs==REAL) 
        {
            if(lhs==treetype(tr->links[1]))
            {
                return lhs;
            }
        }
        return ERROR;
    }

    if(v==MINUS)
    {
        int lhs = treetype(tr->links[0]);
        if( lhs==INT || lhs==REAL || lhs==MATRIX )
        {
            if(lhs==treetype(tr->links[1]))
            {
                return lhs;
            }
        }
       return ERROR;
    }

    if(v==DIV)
    {
        int lhs = treetype(tr->links[0]);
        if(lhs==INT || lhs==REAL)
        {
            if(lhs==treetype(tr->links[1])) return REAL;
        }
        return ERROR;
    }
    
    if(v==OR || v==AND)
    {
        int lhs = treetype(tr->links[0]);
        if(lhs==BOOL)
        {
            if(lhs==treetype(tr->links[1])) return BOOL;
        }
        return ERROR;
    }
    
    if(v==NOT)
    {
        int lhs = treetype(tr->links[0]);
            if(lhs==BOOL) return BOOL;
        return ERROR;
    }

    if(v>=33 && v<=38)
    {
        int lhs = treetype(tr->links[0]);
        if(lhs==INT || lhs==REAL)
        {
            if(lhs==treetype(tr->links[1])) return BOOL;
        }
        return ERROR;
    }

    
    if(v==row || v==rows)
    {
        return MATRIX;
    }

    if(v == SIZE)
    {
        if(treetype(tr->links[0])==MATRIX)
        {
            return CROSS;
        }
        else if(treetype(tr->links[0])==STRING2)
        {
            return INT;
        }
        else return ERROR;
    }
    if(v==varList)
    {
        if(tr->countChildren==2 && tr->links[0]->value == ID2 && tr->links[1]->value == ID2 && treetype(tr->links[0])==INT && treetype(tr->links[1])==INT)        
        {
            return CROSS;
        }
        return ERROR;
    }
}
//for returning size of matrix
int matrixTreetype(tree tr) 
{
    if(tr->value==ID2)
    {
        int loc1 = findNode(tr->links[0]->tokeninfo,scope,table);
        int size =  1000*(table[loc1].m1) + table[loc1].m2;
        return size;
    }
    if(tr->value == PLUS || tr->value == MINUS)
    {
        int loc1 = findNode(tr->links[0]->tokeninfo,scope,table);
        int lhs = 1000*(table[loc1].m1) + table[loc1].m2;
        int rhs = matrixTreetype(tr->links[1]);
        if(rhs == lhs) return lhs;
        else return -1;
    }
    else return 0;
    
}
//FUNCTION 6//Driver for typechecking.
//Traverses the tree and matches left hand side and right hand side of the assignment operators (if they are not functions, ofcourse) and gives errors in case of mismatch.
void typeCheck(tree tr)
{
    if(tr->value==ASSIGNOP)
    {
        if(tr->links[1]->value==PLUS || tr->links[1]->value==MINUS || tr->links[1]->value==MUL||tr->links[1]->value==DIV || tr->links[1]->value==SIZE || ((tr->countChildren == 2 && tr->links[0]->value==ID2 )))
        {
            int a = treetype(tr->links[0]);//Go to this function now.
            int b = treetype(tr->links[1]);
            if(a!=b && b!=funCallStmt) printf("Line %d: Type Mismatch Error in %s and %s.\n",tr->tokeninfo.line,mapTo[a],mapTo[b]);
            if(a==MATRIX && b==MATRIX && !(tr->links[1]->value==rows || tr->links[1]->value==row))
            {
                int h = matrixTreetype(tr->links[1]);
                if(h==0 || h==-1) printf("Line %d: Matrix sizes not compatible\n",tr->links[0]->tokeninfo.line);
            }
        }
        
    }
    
    else
    {
        int n = tr->countChildren;
        int i;
        for(i=0;i<n;i++)
        {
            typeCheck(tr->links[i]);
        }
    }
}

void typeChecker(tree tr)
{
    currMax = 1;
    scope = 1;
    dummy = 0;
    table = createHashTable(); 
    strcpy(mapV[1],"_main");  
    param = &dummy;
    Populate(tr,1); //Start from this function. 
    typeCheck(tr); //Then this one.
}
void typeCheckerAndPrint(tree tr)
{
    currMax = 1;
    scope = 1;
    dummy = 0;
    table = createHashTable(); 
    strcpy(mapV[1],"_main");  
    param = &dummy;
    Populate(tr,0); //Start from this function. 
    printHashTable(table);
}
#endif
