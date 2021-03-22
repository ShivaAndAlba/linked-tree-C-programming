/*------------------------------------------------------------------------
author:George Mirsoyan
ID:309096485

main file containing all related function to the assigment

------------------------------------------------------------------------*/

#include "maman_16.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc,char *argv[])
{
    bt_node *root=NULL;

    if( argc == 1 )
    {
        key_input(root);
    }
    else
    {
        file_input(argc,argv,root);
    }

    return 0;
}
/*------------------------------------------------------------------------
minimum\maximum is a generic function from our book with one exception of avoiding
going through threads.
------------------------------------------------------------------------*/
bt_node *min(bt_node *node)
{
    if ( node == NULL)
        return node;
    while ( (node->left != NULL) && ( LEFT_NULL_AND_CHILD_CHECK(node)) )
        node=node->left;
    return node;
}

bt_node *max(bt_node *node)
{
    while ( (node->right != NULL) && (RIGHT_NULL_AND_CHILD_CHECK(node)) )
        node=node->right;
    return node;
}
/*------------------------------------------------------------------------
find is recursive function.
stops if found given key or node is a leaf node.(if most right or left node
reached or threaded leaf is found, we stop and return that node.)
------------------------------------------------------------------------*/
bt_node *find(bt_node *node,int key)
{
    if ( (node == NULL) || (key == node->key_value) )
        return node;

    if (key < (node->key_value) )
    {
        if ( !(LEFT_NULL_AND_CHILD_CHECK(node)) )
            return node;
        return find(node->left,key);
    }
    else
    {
        if ( !(RIGHT_NULL_AND_CHILD_CHECK(node)) )
            return node;
        return find(node->right,key);
    }
}
/*------------------------------------------------------------------------
insert_key receives root and key to be inserted to the binary tree.
insert_key returns the root of the tree after insertion.
------------------------------------------------------------------------*/
bt_node *insert(bt_node *root,int key,med_ptr *med_node)
{
    bt_node *temp1,*temp2,*node;

    temp1=NULL;
    temp2=root;
    med_node->insertion = TRUE;

    node=create_node(key);
    if (node == NULL)
        return root;
/*------------------------------------------------------------------------
here we compare the node's key and temp2's key(temp2 is a node in the binary tree), if node's key is smaller
then temp2's key we proceed to the left subtree otherwise we proceed to right subtree.

before we proceed with temp2 we check 3 things:
1.If temp2's child is node(in case we reached max or min) if so we break from while loop.
2.If temp2's child has a connected thread to root(in that case roots parent is NULL) if so we break from loop.
3.If temp2's child has a connected thread to any other node,if so we break from loop.

If all if statements fail we are guaranteed with a leaf
------------------------------------------------------------------------*/
    while (temp2 != NULL)
    {
        temp1=temp2;
        if (node->key_value < temp2->key_value)
        {
            if ( (temp2->left == NULL ))
                break;
            if ( LEFT_PARENT_NULL_CHECK(temp2) )
                break;
            if (!LEFT_CHILD_CHECK(temp2))
                break;

            temp2=temp2->left;
        }
        else
        {
            if (temp2->right == NULL)
                break;
            if ( RIGHT_PARENT_NULL_CHECK(temp2) )
                break;
            if ( !RIGHT_CHILD_CHECK(temp2) )
                break;

            temp2=temp2->right;
        }
    }
    node->parent=temp1;
/*------------------------------------------------------------------------
here we check if the tree is empty,if so insert node to root,
otherwise we insert the new node to the binary tree and print a successful insertion.
------------------------------------------------------------------------*/
    if (temp1 == NULL)
    {
        root=node;
        med_node->node = root;
        printf("(%d) was successfully inserted.\n\n",root->key_value);
    }
    else
    {
        if (node->key_value < temp1->key_value)
        {
            temp1->left=node;
            thread_fix(temp1->left);
            median(med_node,key,root);
            printf("(%d)  was successfully inserted.\n\n",temp1->left->key_value);
        }
        else
        {
            temp1->right=node;
            thread_fix(temp1->right);
            median(med_node,key,root);
            printf("(%d)  was successfully inserted.\n\n",temp1->right->key_value);
        }
    }


    return root;
}
/*------------------------------------------------------------------------
delete_key is a function which determines if a node to be deleted has one,two or no children and
executes case B,case C and case A accordingly.
------------------------------------------------------------------------*/
void delete(bt_node *root,int key,med_ptr *med_node)
{
    bt_node *node;

    node=find(root,key);
    med_node->insertion = FALSE;

    if(node == NULL)
        return;

/*-------if node is a max/min and if node is a leaf-----------*/
    if ( LEFT_NULL_CHILD(node) )
    {
        if ( !RIGHT_CHILD_CHECK(node) )
            root=del_case_A(root,node);
        else
            root=del_case_B(root,node);
    }
    else if ( RIGHT_NULL_CHILD(node) )
    {
        if ( !LEFT_CHILD_CHECK(node) )
            root=del_case_A(root,node);
        else
            root=del_case_B(root,node);
    }

/*------------if node has 2 children--------------------------*/
    else if(LEFT_CHILD_CHECK(node) && RIGHT_CHILD_CHECK(node))
        root=del_case_C(root,node);
/*----------two cases of to determine if node has a right or left child-----*/
    else if ( LEFT_NULL_CHILD(node) || (LEFT_CHILD_CHECK(node)) )
        root=del_case_B(root,node);
    else if ( RIGHT_NULL_CHILD(node) || (RIGHT_CHILD_CHECK(node)) )
        root=del_case_B(root,node);
    else
/*-----------------if node is a leaf but not a min or max------------------*/
        root=del_case_A(root,node);
    printf("%d was successfully deleted.\n\n",key);

    median(med_node,key,root);
}
/*------------------------------------------------------------------------
Case A is when the node to be deleted has no children.

The function simply changes its pointer from the parent node to node's predecessor or successor accordingly
to if node to be deleted is a left child or right child.
------------------------------------------------------------------------*/
bt_node *del_case_A(bt_node *root,bt_node *node)
{
    bt_node *par_node;

    par_node=node->parent;
    if ( par_node->left == node )
        par_node->left = par_node->left;
    else
        par_node->right = par_node->right;
    free( node );
    return root;
}
/*------------------------------------------------------------------------
Case B is when the node to be deleted has 1 child node.

First the function checks if the node has a left or right child, for right or left
child it changes its predecessor or successor accordingly to the node's predecessor or successor.
Then the function changes its node's parent to node's only child.

------------------------------------------------------------------------*/
bt_node *del_case_B(bt_node *root,bt_node *node)
{
    bt_node *succ_node,*pred_node,*par_node;

    par_node=node->parent;

/*-----if node is max or min then we connect parent node to child--------*/
    if ( node->left == NULL )
    {
        par_node->left = node->right;
        node->right->parent = par_node;
        return root;
    }
    else if ( node->right == NULL )
    {
        par_node->right = node->left;
        node->left->parent = par_node;
        return root;
    }
/*------------------------------------------------------------------------
we determine here pred_node and succ_node depending on if node has a
right child or left child.
------------------------------------------------------------------------*/
    if( LEFT_NULL_AND_CHILD_CHECK(node) )
    {
        succ_node = node->right;
        pred_node = max(node->left);
    }
    else
    {
        pred_node = node->left;
        succ_node = min(node->right);

    }
/*------------------------------------------------------------------------

if node has a left child then it's predecessors right child will point
to node successor.
if node has a right child then the exact opposite from the above.

------------------------------------------------------------------------*/
    if (RIGHT_CHILD_CHECK(node))
        succ_node->left=node->left;
    else
        pred_node->right=node->right;
/*------------------------------------------------------------------------
if node is root
------------------------------------------------------------------------*/
    if(par_node == NULL)
    {
        if(node->left == NULL)
            root=node->right;
        else
            root=node->left;
    }
/*------------------------------------------------------------------------
if node is left or right child of it's parent then we connect parent node to node's
child.

-------------------------------------------------------------------------*/
    if ( par_node->left == node )
    {
        if (LEFT_CHILD_CHECK(node))
        {
            par_node->left=node->left;
            node->left->parent = par_node;
        }
        else
        {
            par_node->left=node->right;
            node->right->parent = par_node;
        }
    }
    else
    {
        if(LEFT_CHILD_CHECK(node))
        {
            par_node->right=node->left;
            node->left->parent = par_node;
        }
        else
        {
            par_node->right=node->right;
            node->right->parent = par_node;
        }
    }

    free(node);
    return root;
}
/*------------------------------------------------------------------------
Case C is when the to be deleted node has 2 children nodes.

first we find the successor to the node then we copy it's data to the node.
then we delete successor using case A or case B.
------------------------------------------------------------------------*/
bt_node *del_case_C(bt_node *root,bt_node *node)
{
    bt_node *succ;


    succ=min(node->right);

    while (succ->left != NULL && !LEFT_CHILD_CHECK(node))
        succ=succ->left;

    node->key_value=succ->key_value;
    node=succ;

/*--------------if node has one child--------------------*/
    if ( RIGHT_NULL_AND_CHILD_CHECK(node) )
        root=del_case_B(root,node);
    else if ( LEFT_NULL_AND_CHILD_CHECK(node) )
        root=del_case_B(root,node);
/*------------------if node is a leaf-----------------*/
    else
        root=del_case_A(root,node);

    return root;

}
/*------------------------------------------------------------------------
Inorder\preorder\postorder tree walk adapted to threaded binary tree with thread checks before each recursion.
The function stops when we reached the max key.(max key's right child is NULL).
before each step of the recursion we check if node's child is null if not we check if node's child is a thread.
only then we proceed to the next recursion.
------------------------------------------------------------------------*/
void inorder(bt_node *node)
{
    if (node->left != NULL)
        if ( node->left->parent == node )
            inorder(node->left);

    printf("%d ",node->key_value);

    if ( node->right != NULL )
        if (  node->right->parent == node )
            inorder(node->right);
}

void preorder(bt_node *node)
{
    printf("%d ",node->key_value);

    if (node->left != NULL)
        if ( node->left->parent == node )
            preorder(node->left);

    if ( node->right != NULL )
    {
        if (  node->right->parent == node )
            preorder(node->right);
    }
}

void postorder(bt_node *node)
{
    if (node->left != NULL)
        if ( node->left->parent == node )
            postorder(node->left);

    if ( node->right != NULL )
    {
        if (  node->right->parent == node )
            postorder(node->right);
    }
    printf("%d ",node->key_value);
}
/*------------------------------------------------------------------------
using threaded quality of our tree we can find predecessor\successor by simply going to the
threads or to min\max of child.
------------------------------------------------------------------------*/
bt_node *pred(bt_node *node)
{
    if ( node->left == NULL )
        return NULL;
    if ( LEFT_CHILD_CHECK(node) )
        return node=( max(node->left) );
    else
        return node->left;
}

bt_node *succ(bt_node *node)
{
    if ( node->right == NULL )
        return NULL;
    if ( RIGHT_CHILD_CHECK(node) )
        return node=( min(node->right) );
    else
        return node->right;
}
