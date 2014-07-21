//Team Number 7
//Danish      2011A7PS037P
//Mansi Gupta 2011A7PS023P
#ifndef MNIb
#define MNIb
#include"semanticAnalyzerDef.h"


//FUNCTION NO 2

void traverseForId(tree tr, int type,int scope,int pr) //this function traverses the declaration statement and populates the hashTable.
{
    if(tr->value == ID2)
    {
        //putting in hash table
        int result = addNode(tr->tokeninfo,type,scope,offset[scope],NULL,table,pr,tr->tokeninfo.line);
        if(result!=-1) addToSimpleTable(tr->tokeninfo,type,scope,offset[scope],NULL);
        if(result!=-1 && type==INT) offset[scope] = offset[scope] + 2;
        else if(result!=-1 &&type==REAL) offset[scope] = offset[scope] + 4;
        else if(result!=-1 && type==MATRIX) offset[scope] = offset[scope] + 200;
        else if(result!=-1 && type==STRING2) 
                offset[scope] = offset[scope] + 20;
    }
    else
    {
        int n = tr->countChildren;
        int i=0;
        for(i=0;i<n;i++)
        {
            traverseForId(tr->links[i],type,scope,pr);
        }
    }        
}

//FUNCTION NO 3
//this function traverses the input parameter list of function definition and populates the hashTable.
void traverseForIdPL(tree tr, int scope, int pr)
{
    int n = tr->countChildren;
    if(tr->value == parameterList)
    {
        int type = tr->links[0]->value;
        int result = addNode(tr->links[1]->tokeninfo,tr->links[0]->value,scope,offset[scope],NULL,table,pr,tr->links[1]->tokeninfo.line);
        
        if(result!=-1) addToSimpleTable(tr->links[1]->tokeninfo,type,scope,offset[scope],NULL);
        if(result!=-1 && type==INT) offset[scope]+=2;
        else if(result!=-1 && type==REAL) offset[scope]+=4;
        else if(result!=-1 && type==MATRIX) offset[scope]+=200;
        else if(result!=-1 && type==STRING2) offset[scope]+=20;
        
        if(n==3) traverseForIdPL(tr->links[2],scope,pr);
    }       
}

//The module is to calcualate number of rows and columns of a matrix
//MODULE START

int count1 = 0;
int count2= 0;
int countColms(tree);
int countRows(tree head)
{
    if(head->value==rows && head->links[0]->value==NUM) return countColms(head);
    else
    {
        if(head->value==row) count1++;
        int i = 0;
        int n = head->countChildren;
        for(i=0;i<n;i++)
        {
            countRows(head->links[i]);
        }
        return count1;
    }
}

int countColms(tree head)
{
    if(head->value==NUM) count2++;
    int i = 0;
    int n = head->countChildren;
    for(i=0;i<n;i++)
    {
        countColms(head->links[i]);
    }
    return count2;
}
//MODULE END


// THE LOWER MODULE IS TO MATCH RETURN TYPES AND INPUT PARAMETERS OF FUNCTION DECLARATION AND FUNCTION CALLS.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCTION NO 4 //Assistant function to 3.
int array[30];
int global = 0;

void fillingArray(tree head1)
{
    int ch1 = head1->countChildren;
    
    if(head1->value == ID2) {}
    else if(head1->value == parameterList) {}
    else //STRING INT etc
    {
        array[global]=head1->value;
        global++;
    }

    int ch = head1->countChildren;
    int i = 0;
    for(i=0;i<ch;i++)
    {
        fillingArray(head1->links[i]);
    }

}

//FUNCTION NO 5//Assistant function to 3.
int indx = 0;
int ret = 1;
void checkingArray(tree head2)
{
    if(head2->value == ID2)
    {
        int location = findNode(head2->tokeninfo,scope,table);
        int type = table[location].type;
        if(indx<=global)
        {
            if(array[indx] != type) ret=0;
            indx++;
        }
        else ret= 0;
    }

    int ch = head2->countChildren;
    int i;
    for(i=0;i<ch;i++)
    {
        checkingArray(head2->links[i]);
    }
}
//FUNCTION NO 3
int checkingFuncId(tree funcPtr, tree tr) 
{
    //checking return type
    tree head1 = funcPtr->links[0];
    global = 0;
    indx = 0;
    fillingArray(head1);
    tree head2 = tr->links[0];
    ret=1;
    checkingArray(head2);
    if(indx != global) ret=0;
    if(ret==0) return 0;
    //checking input parameter list
    head1 = funcPtr->links[2];
    global = 0;
    indx = 0;
    fillingArray(head1);
    head2 = tr->links[1]->links[1];
    ret = 1;
    checkingArray(head2);
    if(indx != global) ret=0;
    if(ret==0) return -1;
    return 1;
}
//MODULE END

//MODULE 3 START//For checking if the return variables of functions are used in its statement.
int chh = 0;
void checkInHead(tree head,char* lex)
{

    if(head!=NULL && head->value == ID2)
    { 
        if(strcmp(head->tokeninfo.lexeme,lex)==0) chh=1;
    }
    //non terminals
    else 
    {
        int ch = head->countChildren;
        int i;
        if(ch>0)
        for(i=0;i<ch;i++)
        {
            checkInHead(head->links[i],lex);
        }
    }
}

void checkForFunction(tree head,tree tr,int pr)
{
    int n = head->countChildren;
    if(tr->value==parameterList)
    {
    
        char lex[50];
        strcpy(lex,tr->links[1]->tokeninfo.lexeme);
        int i;
        int max = 0;
        for(i=3;i<n;i++)
        { 
            chh=0;    
            checkInHead(head->links[i],lex);
            int a = chh;
            if(a>max) max = a;
        }
        if(max == 0 && pr==1) printf("Line %d: Variable '%s' of Function %s not used.\n",head->tokeninfo.line,lex,head->links[1]->tokeninfo.lexeme);
        if(tr->countChildren==3) {checkForFunction(head,tr->links[2],pr);}
    }
    
}
//MODULE 3 END

//FUNCTION NO 1
//This function populates the hash table from 1.) declaration statement 2.) Input parameter list in function definition//This also checks if the variables (ID's/FUNID's) are declared previously or not.//The redeclaration error is taken care in addNode function of hashTable itself.
void Populate(tree tr,int pr) 
{
    int i;
    int n = tr->countChildren;
    
    if(tr->value == decStmt)//declaration statement
    {
        int Type = tr->links[0]->value;
        traverseForId(tr->links[1],Type,scope,pr); //Go to this function now. //pushed declarations statements in hashtable
    }
    else if(tr->value == parameterList)
    {
        traverseForIdPL(tr,scope,pr); //Go to this function now //pushed declarations statements from input parameter list in hashtable
    }

    else if(tr->value == IF || tr->value == ELSE) //scope change IF/ELSE 
    {
        for(i=0;i<n;i++)
        {
            Populate(tr->links[i],pr);
        }
    }
    else if(tr->countChildren>1 && (tr->value == ASSIGNOP && tr->links[0]->value == parameterList && tr->links[1]->value==FUNID))//scope change//function definition
    {
        
        tree funcPtr = tr; 
        int result = addNode(tr->links[1]->tokeninfo,FUNID,scope,offset[scope],funcPtr,table,pr,tr->links[1]->tokeninfo.line);
        if(result!=-1) addToSimpleTable(tr->links[1]->tokeninfo,FUNID,scope,offset[scope],NULL);
        currMax++;
        parent[currMax] = scope;
        scope = currMax;

        strcpy(mapV[scope],tr->links[1]->tokeninfo.lexeme);
        offset[scope] = 0;
        for(i=0;i<n;i++)
        {
            Populate(tr->links[i],pr);
        }
        scope = parent[scope];
        //check if the return type IDs are used at least once in the rhs of the statements.
        checkForFunction(tr,tr->links[0], pr);   
    }

    //whenever IDs are encountered while traversing (except when they are declared [taken care earlier in first two checks]), it checks whether they are declared in the scope before.
    else if(tr->value == ID2)
    {
        int temp = scope;
        int c=-1;
        while(1)
        {
            c = findNode(tr->tokeninfo,temp,table);
            if(c!=-1) break;
            if(temp==0) break;
            temp = parent[temp];
        }
        if(c==-1)
        {
            if(pr==1)
            printf("Line %d: The variable '%s' not declared in the scope.\n",tr->tokeninfo.line,tr->tokeninfo.lexeme); 
        }
    }
    
    //function call Statement//Finding if the function has been defined earlier in the same or higher scope.//check if the variables in input parameter list and return statement are as per the function definition.
    else if(tr-> countChildren > 1 && ( tr->value == ASSIGNOP && tr->links[1]->value == funCallStmt))
    { 
        int temp = scope;
        int c=-1;
        while(1)
        {
            c = findNode(tr->links[1]->links[0]->tokeninfo,temp,table);

            if(c!=-1) break;
            if(temp==0) break;
            temp = parent[temp];

        }
	if(temp == parent[scope]) {printf("Line %d: Recursion_ERROR: function %s is recursive\n",tr->links[1]->links[0]->tokeninfo.line,tr->links[1]->links[0]->tokeninfo.lexeme);}
        if(c==-1) 
        {   
            if(pr==1)
            printf("Line %d: Function '%s' not declared in the scope.\n",tr->tokeninfo.line,tr->links[1]->links[0]->tokeninfo.lexeme); 
        }

        //check if IDs used are declared
        Populate(tr->links[0],pr);//checking if Ids used for storing returned values are declared.
        Populate(tr->links[1],pr);//checking if ids used in input parameter list are declared or not.
        //checking if the IDs declared are as per the IDs defined.     
        tree funcPtr = table[c].funcPtr;
        if(funcPtr!=NULL)
        {
            int check = checkingFuncId(funcPtr,tr);//This is a separate module.
            if(check == 0)
            {
                if(pr==1)
                printf("Line %d: In function '%s', Return Type do not match the function definition\n",tr->tokeninfo.line,tr->links[1]->links[0]->tokeninfo.lexeme);
            }
            else if(check == -1)
            {
                if(pr==1)
                printf("Line %d: In function '%s', Input parameters do not match the function definition\n",tr->tokeninfo.line,tr->links[1]->links[0]->tokeninfo.lexeme);
            }
        }
    }
    else if(tr->countChildren>1 && tr->value == ASSIGNOP && (tr->links[1]->value==row || tr->links[1]->value == rows)) //in matrix assignment
    {
        int temp = scope;
        int c=-1;
        while(1)
        {
            c = findNode(tr->links[0]->tokeninfo,temp,table);
            if(c!=-1) break;
            if(temp==0) break;
            temp = parent[temp];
        }
        if(c==-1) {if(pr==1) printf("Line %d: Variable '%s' not declared in the scope.\n",tr->tokeninfo.line,tr->links[0]->tokeninfo.lexeme);} 
        
        else
        {
        //populating number of rows and columns in hash table.
        tree head = tr->links[1];    
        count1 = 0;
        count2 = 0;
        
        table[c].m1 = countRows(head);
        if(head->value==rows && head->links[0]->value==NUM) table[c].m2 = 1;
        else
        {
            if(head->value == rows) head = head->links[0];
            table[c].m2 = countColms(head);
        }
        }
    }

    //for string assignments// populate string length
    else if(tr->countChildren > 1 && tr->value == ASSIGNOP && tr->links[0]->value==ID2 && tr->links[1]->value==STR)
    {
        int temp = scope;
        int c=-1;
        while(1)
        {
            c = findNode(tr->links[0]->tokeninfo,temp,table);
            if(c!=-1) break;
            if(temp==0) break;
            temp = parent[temp];
        }
        if(c==-1) {if(pr==1) printf("Line %d: Variable '%s' not declared in the scope.\n",tr->links[0]->tokeninfo.line,tr->links[0]->tokeninfo.lexeme);} 
        else
        {
            table[c].s = strlen(tr->links[1]->tokeninfo.lexeme);
        }
    }
    else
    {
        for(i=0;i<n;i++)
        {
            Populate(tr->links[i],pr);
        }
    }
}

#endif
