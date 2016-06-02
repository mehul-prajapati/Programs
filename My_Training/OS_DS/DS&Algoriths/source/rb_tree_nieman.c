/* Red-Black Tree Code from Thomas Niemann's "book" */

#include <stdlib.h>
#include <stdio.h>

/* modify these lines to establish data type */
typedef int T;
#define CompLT(a,b) (a < b)
#define CompEQ(a,b) (a == b)

/* red-black tree description */
typedef enum { Black, Red } NodeColor;

typedef struct Node_ {
    struct Node_ *Left;         /* left child */
    struct Node_ *Right;        /* right child */
    struct Node_ *Parent;       /* parent */
    NodeColor Color;            /* node color (black, red) */
    T Data;                     /* data stored in node */
} Node;

#define NIL &Sentinel           /* all leafs are sentinels */
Node Sentinel = { NIL, NIL, 0, Black, 0};

Node *Root = NIL;               /* root of red-black tree */


Node *InsertNode(T Data) {
    Node *Current, *Parent, *X;

   /***********************************************
    *  allocate node for Data and insert in tree  *
    ***********************************************/

    /* find where node belongs */
    Current = Root;
    Parent = 0;
    while (Current != NIL) {
        if (CompEQ(Data, Current->Data)) return (Current);
        Parent = Current;
        Current = CompLT(Data, Current->Data) ?
            Current->Left : Current->Right;
    }

    /* setup new node */
    if ((X = malloc (sizeof(*X))) == 0) {
        printf ("insufficient memory (InsertNode)\n");
        exit(1);
    }
    X->Data = Data;
    X->Parent = Parent;
    X->Left = NIL;
    X->Right = NIL;
    X->Color = Red;

    /* insert node in tree */
    if(Parent) {
        if(CompLT(Data, Parent->Data))
            Parent->Left = X;
        else
            Parent->Right = X;
    } else {
        Root = X;
    }

    InsertFixup(X);
    return(X);
}

void InsertFixup(Node *X) {

   /*************************************
    *  maintain red-black tree balance  *
    *  after inserting node X           *
    *************************************/

    /* check red-black properties */
    while (X != Root && X->Parent->Color == Red) {
        /* we have a violation */
        if (X->Parent == X->Parent->Parent->Left) {
            Node *Y = X->Parent->Parent->Right;
            if (Y->Color == Red) {

                /* uncle is red */
                X->Parent->Color = Black;
                Y->Color = Black;
                X->Parent->Parent->Color = Red;
                X = X->Parent->Parent;
            } else {

                /* uncle is black */
                if (X == X->Parent->Right) {
                    /* make X a left child */
                    X = X->Parent;
                    RotateLeft(X);
                }

                /* recolor and rotate */
                X->Parent->Color = Black;
                X->Parent->Parent->Color = Red;
                RotateRight(X->Parent->Parent);
            }
        } else {

            /* mirror image of above code */
            Node *Y = X->Parent->Parent->Left;
            if (Y->Color == Red) {

                /* uncle is red */
                X->Parent->Color = Black;
                Y->Color = Black;
                X->Parent->Parent->Color = Red;
                X = X->Parent->Parent;
            } else {

                /* uncle is black */
                if (X == X->Parent->Left) {
                    X = X->Parent;
                    RotateRight(X);
                }
                X->Parent->Color = Black;
                X->Parent->Parent->Color = Red;
                RotateLeft(X->Parent->Parent);
            }
        }
    }
    Root->Color = Black;
}

void RotateLeft(Node *X) {

   /**************************
    *  rotate Node X to left *
    **************************/

    Node *Y = X->Right;

    /* establish X->Right link */
    X->Right = Y->Left;
    if (Y->Left != NIL) Y->Left->Parent = X;

    /* establish Y->Parent link */
    if (Y != NIL) Y->Parent = X->Parent;
    if (X->Parent) {
        if (X == X->Parent->Left)
            X->Parent->Left = Y;
        else
            X->Parent->Right = Y;
    } else {
        Root = Y;
    }

    /* link X and Y */
    Y->Left = X;
    if (X != NIL) X->Parent = Y;
}

void RotateRight(Node *X) {

   /****************************
    *  rotate Node X to right  *
    ****************************/

    Node *Y = X->Left;

    /* establish X->Left link */
    X->Left = Y->Right;
    if (Y->Right != NIL) Y->Right->Parent = X;

    /* establish Y->Parent link */
    if (Y != NIL) Y->Parent = X->Parent;
    if (X->Parent) {
        if (X == X->Parent->Right)
            X->Parent->Right = Y;
        else
            X->Parent->Left = Y;
    } else {
        Root = Y;
    }

    /* link X and Y */
    Y->Right = X;
    if (X != NIL) X->Parent = Y;
}

void DeleteNode(Node *Z) {
    Node *X, *Y;

   /*****************************
    *  delete node Z from tree  *
    *****************************/

    if (!Z || Z == NIL) return;

    if (Z->Left == NIL || Z->Right == NIL) {
        /* Y has a NIL node as a child */
        Y = Z;
    } else {
        /* find tree successor with a NIL node as a child */
        Y = Z->Right;
        while (Y->Left != NIL) Y = Y->Left;
    }

    /* X is Y's only child */
    if (Y->Left != NIL)
        X = Y->Left;
    else
        X = Y->Right;

    /* remove Y from the parent chain */
    X->Parent = Y->Parent;
    if (Y->Parent)
        if (Y == Y->Parent->Left)
            Y->Parent->Left = X;
        else
            Y->Parent->Right = X;
    else
        Root = X;

    if (Y != Z) Z->Data = Y->Data;
    if (Y->Color == Black)
        DeleteFixup (X);
    free (Y);
}

void DeleteFixup(Node *X) {

   /*************************************
    *  maintain red-black tree balance  *
    *  after deleting node X            *
    *************************************/

    while (X != Root && X->Color == Black) {
        if (X == X->Parent->Left) {
            Node *W = X->Parent->Right;
            if (W->Color == Red) {
                W->Color = Black;
                X->Parent->Color = Red;
                RotateLeft (X->Parent);
                W = X->Parent->Right;
            }
            if (W->Left->Color == Black && W->Right->Color == Black) {
                W->Color = Red;
                X = X->Parent;
            } else {
                if (W->Right->Color == Black) {
                    W->Left->Color = Black;
                    W->Color = Red;
                    RotateRight (W);
                    W = X->Parent->Right;
                }
                W->Color = X->Parent->Color;
                X->Parent->Color = Black;
                W->Right->Color = Black;
                RotateLeft (X->Parent);
                X = Root;
            }
        } else {
            Node *W = X->Parent->Left;
            if (W->Color == Red) {
                W->Color = Black;
                X->Parent->Color = Red;
                RotateRight (X->Parent);
                W = X->Parent->Left;
            }
            if (W->Right->Color == Black && W->Left->Color == Black) {
                W->Color = Red;
                X = X->Parent;
            } else {
                if (W->Left->Color == Black) {
                    W->Right->Color = Black;
                    W->Color = Red;
                    RotateLeft (W);
                    W = X->Parent->Left;
                }
                W->Color = X->Parent->Color;
                X->Parent->Color = Black;
                W->Left->Color = Black;
                RotateRight (X->Parent);
                X = Root;
            }
        }
    }
    X->Color = Black;
}

Node *FindNode(T Data) {

   /*******************************
    *  find node containing Data  *
    *******************************/

    Node *Current = Root;
    while(Current != NIL)
        if(CompEQ(Data, Current->Data))
            return (Current);
        else
            Current = CompLT (Data, Current->Data) ?
                Current->Left : Current->Right;
    return(0);
}
