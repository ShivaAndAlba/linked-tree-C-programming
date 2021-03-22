/*------------------------------------------------------------------------
author:George Mirsoyan
ID:309096485

header file

------------------------------------------------------------------------*/
#ifndef MAMAN_16_H
#define MAMAN_16_H

/*-------------------------------------macro--------------------------------------------*/
#define TRUE 1
#define FALSE 0
#define LEFT_CHILD_CHECK(node) ( (node)->left->parent == (node) ? (TRUE) : (FALSE)  )
#define RIGHT_CHILD_CHECK(node) ( (node)->right->parent == (node)  ? (TRUE) : (FALSE)  )
#define LEFT_NULL_AND_CHILD_CHECK(node) ( (node)->left->parent != NULL  ? (LEFT_CHILD_CHECK(node)) : (FALSE) )
#define RIGHT_NULL_AND_CHILD_CHECK(node) ( (node)->right->parent != NULL ? (RIGHT_CHILD_CHECK(node)) : (FALSE) )
#define LEFT_PARENT_NULL_CHECK(node) ( (node)->left->parent == NULL ? (TRUE) : (FALSE) )
#define RIGHT_PARENT_NULL_CHECK(node) ( (node)->right->parent == NULL ? (TRUE) : (FALSE) )
#define LEFT_NULL_CHILD(node) ( (node)->left == NULL ? (TRUE) : (FALSE) )
#define RIGHT_NULL_CHILD(node) ( (node)->right == NULL ? (TRUE) : (FALSE) )
/*----------------------------constant size macros---------------------------------------*/
#define MAX_LINE_SIZE 80
#define MAX_COMM_LEN 10
#define FUNC_NUM 11
/*------------------------------data structures------------------------------------------*/
/*enumeration for each individual function used the binary tree*/
enum COMMAND_FLAG {MIN,MAX,PRED,SUCC,FIND,INSERT,DELETE,PREORDER,INORDER,POSTORDER,MEDIAN};

/*binary tree node */
typedef struct bt_node
{
        int key_value;
        struct bt_node *parent;
        struct bt_node *left;
        struct bt_node *right;
}bt_node;
/*median pointer*/
typedef struct med_ptr
{
    bt_node *node;  /*pointer to binary tree node*/
    int even;       /*variable containing parity status*/
    int insertion;  /*variable containing insertion\deletion status*/
}med_ptr;
/*lookup element-an element used to store function name calls*/
typedef struct lookup_elem
{
    char name[MAX_COMM_LEN];    /*string of the function's names*/
    int COMM_FLAG_VAL;          /*unique value of each function*/
}lookup_elem;
/*array containing all function names and their enumeration*/
static lookup_elem lookup_arr[]=
{
    {"min",MIN},
    {"max",MAX},
    {"pred",PRED},
    {"succ",SUCC},
    {"find",FIND},
    {"insert",INSERT},
    {"delete",DELETE},
    {"preorder",PREORDER},
    {"inorder",INORDER},
    {"postorder",POSTORDER},
    {"median",MEDIAN}
};
/*------------------------------------------------------------------------
median moves according to insertion\deletion and value of the inserted key
here are two tables represent all variations of median movement.
first table is for insertion case, second table is deletion table.
first dimension- value if key is bigger or smaller.
second dimension-if number of elements is even or odd.

first table(insertion):                            second table(deletion):

    {SUCC,0   } <--inserted key is smaller then median-->{0   ,PRED}
    {0   ,PRED} <--  inserted key bigger then median  -->{SUCC,0   }
      ^    ^                                                ^    ^
      |    |                                                |    |
    odd    even  <---    number of nodes in tree   --->    odd   even
------------------------------------------------------------------------*/
static int med_decision[2][2][2]=
{
{
    {SUCC,0},
    {0,PRED}
},
{
    {0,PRED},
    {SUCC,0}
}
};
/*-------------------------function declarations----------------------------------------------*/

/*-----utilities function declaration------*/
bt_node *pred_thread(bt_node *);
bt_node *succ_thread(bt_node *);
void thread_fix(bt_node *);
bt_node *create_node(int);
bt_node *del_case_A(bt_node *,bt_node *);
bt_node *del_case_B(bt_node *,bt_node *);
bt_node *del_case_C(bt_node *,bt_node *);
void key_input(bt_node *);
void file_input(int,char **,bt_node *);
void median(med_ptr *,int,bt_node *);
bt_node *fun_case(bt_node *,char *,int,int,med_ptr *);
med_ptr *memory_allocate(med_ptr *);



/*-----------main function declaration---------------*/
bt_node *min(bt_node *);
bt_node *max(bt_node *);
bt_node *find(bt_node *,int);
bt_node *insert(bt_node *,int,med_ptr *);
void delete(bt_node *,int,med_ptr *);
void preorder(bt_node *);
void inorder(bt_node *);
void postorder(bt_node *);
bt_node *pred(bt_node *);
bt_node *succ(bt_node *);

#endif
