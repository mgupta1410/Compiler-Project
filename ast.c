/*
   Danish 2011A7PS037P
   Mansi 2011A7PS023P
   */
#ifndef this
#define this
#include<stdio.h>
//#include"astDataType.h"
#include"parserDef.h"
#include"treeDataType.h"
#include<stdlib.h>
#include"lexerDef.h"

tree removeTerm(tree tr) 
{
     int i=0;
     int newCountChildren = tr->countChildren;
     i=0;

     while(i < newCountChildren)
     {  
           int v = tr->links[i]->value;
           int c = tr->links[i]->countChildren;
       
        if((v < 100)||(v>=100 && c==0) ) 
        { 

           if( v==0 || v==8 || (v>=13&&v<=19) || (v==22)  || v==39 ||(v >= 100 && c==0))
           {
            int j;
            for(j=i;j<newCountChildren-1;j++)
            {
                tr->links[j] = tr->links[j+1];
            }
            newCountChildren--;
            i--;
           }
           i++;
        }
       else
       {
            tree tr1 = removeTerm(tr->links[i]);
            i++;
       }
     }
     tr->countChildren = newCountChildren;
  return tr;
}

tree remove_Chaining(tree tr)
{
    if(tr->countChildren ==1 && tr->value>=100)
    {
        tree ptr = tr->links[0];

        tr->value = ptr->value;
        tr->tokeninfo = ptr->tokeninfo;
        tr->countChildren = ptr->countChildren;
        int i =0;
        int c = ptr->countChildren;

        for(i=0;i<c;i++)
        {
            tr->links[i] = ptr->links[i];
        }

        remove_Chaining(tr);
    }

        if(tr->countChildren > 1 ) //not necassarily a non-terminal
        {
            int i =0;
            int n = tr->countChildren;
            while(i < n)
            {
                if(tr->links[i]->countChildren > 0) 
                    remove_Chaining(tr->links[i]);
                i++;
            }
        }
    return tr;
}


tree removeExtra(tree tr)
{
   int i=0;
   int newCountChildren = tr->countChildren;
   int flag = 0;
   while(i < newCountChildren)
   {
   //printf("in remove extra : %d : %s\n", i, mapTo[tr->value]);
      int v = tr->links[i]->value;
      //printf("%d\n",v);

        int n = tr->countChildren;
      if(v==1 || (v>=25 && v<=28) || (v>=32 && v<=38)||v==23||v==24||v==29 || v==30 || v==31)//21 -> ELSE
      {
        tree ptr = tr->links[i];
        tr->value = v;
        tr->tokeninfo = tr->links[i]->tokeninfo;
        int j;
        for(j=i; j<newCountChildren-1;j++)
        {
            tr->links[j] = tr->links[j+1];
        }

        newCountChildren -= 1;
        free(ptr);
      }
          
        //printf("here\n");
        else if(v==20 && tr->links[newCountChildren-1]->value==135)//restifstmt
        {
            tr->links[0]->countChildren = n-2;
            int j;
            for(j=1;j<=n-2;j++)
            {
                tr->links[0]->links[j-1] = tr->links[j];
            }
            tr->links[1]=tr->links[n-1];
            newCountChildren = 2;
            tr->links[1]->value = tr->links[1]->links[0]->value;
            tr->links[1]->tokeninfo = tr->links[1]->links[0]->tokeninfo; 
            tr->links[1]->countChildren--;
            for(j=0;j<tr->countChildren;j++)
            {
                tr->links[1]->links[j]=tr->links[1]->links[j+1];
            }
            i=0;
            flag = 1;
        }
        else if( v==20 && tr->links[newCountChildren-1]->value != 135 && flag==0)//if elsepart not present // tr-> istmt-133
        {
            tr->countChildren -= 1;
            tr->value = tr->links[0]->value;
            tr->tokeninfo = tr->links[0]->tokeninfo;
            int j;
            for(j=1;j<=n-1;j++)
            {
                tr->links[j-1] = tr->links[j];
            }
            newCountChildren-=1;
        }
        
      else 
      {
        if(v >= 100 || v==20 || v==21) removeExtra(tr->links[i]);
        i++;
      }
   }
   tr->countChildren = newCountChildren;
   return tr;
}

tree arithmeticPass(tree tr)
{
    
        if(tr->value == 154 || tr->value == 148)//ae,at
        {
            tr->value = tr->links[1]->value; //PLUS/MINUS, MUL/DIV
            tr->tokeninfo = tr->links[1]->tokeninfo;
            tr->links[1] = tr->links[1]->links[0]; 
        }
        if(tr->countChildren > 0 ) //not necassarily a non-terminal
        {
            int i =0;
            int n = tr->countChildren;
            while(i < n)
            {
                if(tr->links[i]->countChildren > 0) 
                    arithmeticPass(tr->links[i]);
                i++;
            }
        }
    return tr;
}
/*
void printAST(tree tr)
{
    int i=0;
    printf("%d %s %d\n",tr-> value, mapTo[tr->value],tr->countChildren);
    for(i=0;i<tr->countChildren;i++)
    {
        printAST(tr->links[i]);
    }
}
*/
tree makeAST(tree tr)
{ 
    tr = removeTerm(tr);
    tr = remove_Chaining(tr);
    tr = removeTerm(tr);
    tr = remove_Chaining(tr);
    tr = removeExtra(tr);
    tr = remove_Chaining(tr);
    tr = arithmeticPass(tr);
    //printAST(tr);
    return tr;
}

#endif
