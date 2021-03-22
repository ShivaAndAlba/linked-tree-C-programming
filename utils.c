/*------------------------------------------------------------------------
author:George Mirsoyan
ID:309096485

utilitis file containing all the the function used by main file

------------------------------------------------------------------------*/


#include "maman_16.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/*------------------------------------------------------------------------
predecessor\successor is a generic function from our book.
this functions are used to connect threads of leafs only.
------------------------------------------------------------------------*/
bt_node *pred_thread(bt_node *node)
{
    bt_node *temp;

    if(node == NULL)
        return NULL;

    if (node->left != NULL)
        return max(node->left);

    temp=node->parent;
    while( (temp != NULL) && (node == temp->left) )
    {
        node=temp;
        temp=temp->parent;
    }

    return temp;
}

bt_node *succ_thread(bt_node *node)
{
    bt_node *temp;

    if (node->right != NULL)
        return min(node->right);

    temp=node->parent;
    while ( (temp != NULL) && (node == temp->right) )
    {
        node=temp;
        temp=temp->parent;
    }

    return temp;
}
bt_node *create_node(int key)
{
    bt_node *node;

    node=(bt_node*)malloc(sizeof(bt_node));
    if (node == NULL)
        printf("Error:not enough space for new node.\n");
    else
    {
        node->key_value=key;
        node->left=NULL;
        node->right=NULL;
        node->parent=NULL;
    }
    return node;
}
/*------------------------------------------------------------------------
thread_fix takes a node and threads nodes left and right pointers.
------------------------------------------------------------------------*/
void thread_fix(bt_node *node)
{
    node->left=pred_thread(node);
    node->right=succ_thread(node);
}
/*------------------------------------------------------------------------
this median function maintains the median pointer.
this function uses variable of type med_node witch hold a pointer to median node
and two integer variables called even and insertion.
then using a table we can determine what sort of action this function should make.
------------------------------------------------------------------------*/
void median(med_ptr *med_node,int key,bt_node *root)
{
    int big_then_med=0,insertion=0,even=0;
/*------if inserted key if bigger then median-----------*/
    if ( key > med_node->node->key_value )
        big_then_med = TRUE;
    else
        big_then_med = FALSE;

    med_node->even = (med_node->even) ^ TRUE;
    even = med_node->even;
    insertion = med_node->insertion;

    switch ( med_decision[insertion][big_then_med][even] )
    {
        case PRED:
        {
            med_node->node=pred(med_node->node);
        }
        case SUCC:
        {
            med_node->node=succ(med_node->node);
        }
    }

}
/*------------------------------------------------------------------------
this function allocates memory for median of type struct
------------------------------------------------------------------------*/

med_ptr *memory_allocate(med_ptr *med_node)
{
    med_node = (med_ptr *)malloc( sizeof(med_ptr) );
    if ( med_node == NULL )
        return med_node;

    med_node->node = (bt_node *)malloc( sizeof(bt_node) );
    if ( med_node->node == NULL)
    {
        free(med_node);
        return med_node;
    }

    med_node->node=NULL;
    med_node->even=FALSE;
    med_node->insertion=FALSE;

    return med_node;
}
/*------------------------------------------------------------------------
key_input and file_input are input function.
only one of then can be run each program call.
------------------------------------------------------------------------*/
void key_input(bt_node *root)
{
    int key,scanned_count;
    char *token,*com,*line,delims[]=" \t";
    med_ptr *med_node=(med_ptr *)malloc(sizeof(med_ptr));

    med_node = memory_allocate(med_node);

    if ( med_node == NULL )
    {
        printf("Not enought memory.\n");
        return;
    }

    line=(char *)malloc(MAX_LINE_SIZE);
    com=(char *)malloc(MAX_LINE_SIZE);

    com=NULL;

    printf("Enter commands:\n");
    while ( TRUE )
    {
        scanned_count=0;

        gets(line);
        token=strtok(line,delims);
        key=0;

        do
        {
            if ( com == NULL )
            {
                com=token;
                scanned_count++;
            }
            else
            {
                key=atoi(token);
                scanned_count++;
            }
        }
        while ( (token=strtok(NULL,delims)) );

        if( scanned_count == 2 )
            printf("Executing: %s %d.\n",com,key);
        else
            printf("Executing: %s.\n",com);

        root=fun_case(root,com,key,scanned_count,med_node);
        com=NULL;
    }
    free(med_node);

}
void file_input(int argc,char *argv[],bt_node *root)
{

    FILE *fp = NULL;
    int i,scanned_count,key;
    char *com;
    med_ptr *med_node=(med_ptr *)malloc(sizeof(med_ptr));


    med_node = memory_allocate(med_node);

    com=(char *)malloc(MAX_COMM_LEN);

    for ( i=0 ; argc > 0  ; i++)
    {
        if( strstr(argv[i],".txt") )
        {
            fp=fopen(argv[i],"r");

            while ( (scanned_count = fscanf(fp,"%s %d",com,&key) ) > 0 )
            {
                if( scanned_count == 2 )
                    printf("Executing: %s %d.\n",com,key);
                else
                    printf("Executing: %s.\n",com);
                root=fun_case(root,com,key,scanned_count,med_node);
            }
        }
        argc--;
    }


    if(fp != NULL)
    {
        fclose(fp);
        free(com);
    }
    free(med_node);
    getchar();
}
/*------------------------------------------------------------------------
this function analyzes commands read from input and executes the commands.
------------------------------------------------------------------------*/
bt_node *fun_case(bt_node *root,char *com,int key,int scanned_count,med_ptr *med_node)
{
    int i;
    bt_node *node;
    enum COMMAND_FLAG FUNC_FLAG;


    for(i=0 ; i < FUNC_NUM ; i++)
    {
        if( strstr(com,lookup_arr[i].name) )
            FUNC_FLAG = lookup_arr[i].COMM_FLAG_VAL;
    }

    if (scanned_count == 1)
    {
        switch (FUNC_FLAG)
        {
            case MIN:
            {
                node=min(root);
                printf("Minimum is: (%d)\n\n",node->key_value);
                break;
            }
            case MAX:
            {
                node=max(root);
                printf("Maximum is: (%d)\n\n",node->key_value);
                break;
            }
            case INORDER:
            {
                inorder(root);
                printf("\n\n");
                break;
            }
            case POSTORDER:
            {
                postorder(root);
                printf("\n\n");
                break;
            }
            case PREORDER:
            {
                preorder(root);
                printf("\n\n");
                break;
            }
            case MEDIAN:
            {
                printf("Median is: (%d)\n\n",med_node->node->key_value);
            }
            default:
                break;
        }
    }
    else
    {
        switch (FUNC_FLAG)
        {
            case PRED:
            {
                node=pred(find(root,key));
                if (node != NULL)
                    printf("Predecessor of %d is: %d\n\n",key,node->key_value);
                else
                    printf("No predecessor.\n\n");
                break;
            }
            case SUCC:
            {
                node=succ(find(root,key));
                if (node != NULL)
                    printf("Successor of %d is: %d\n\n",key,node->key_value);
                else
                    printf("No successor.\n\n");
                break;
            }
            case FIND:
            {
                node=find(root,key);

                if (node->key_value != key)
                    printf("Key not found.\n\n");

                else
                    printf("Key found.\n\n");

                break;
            }
            case INSERT:
            {
                root=insert(root,key,med_node);
                break;
            }
            case DELETE:
            {
                delete(root,key,med_node);
                break;
            }
            default:
                break;
        }

    }
    return root;
}
