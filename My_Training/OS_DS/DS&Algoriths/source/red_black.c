/* red_black.c
*/

typedef struct t_red_black_node *node;
typedef struct t_red_black_tree *Tree;

struct t_red_black_node {
    enum { red, black } colour;
    void *item;
    struct t_red_black_node *left,
                     *right,
                     *parent;
    };

#define NULL_NODE (node)0

struct t_red_black_tree {
	node root;
	}

left_rotate( Tree T, node x ) {
    node y;
    y = x->right;
    /* Turn y's left sub-tree into x's right sub-tree */
    x->right = y->left;
    if ( y->left != NULL_NODE )
        y->left->parent = x;
    /* y's new parent was x's parent */
    y->parent = x->parent;
    /* Set the parent to point to y instead of x */
    /* First see whether we're at the root */
    if ( x->parent == NULL_NODE ) T->root = y;
    else
        if ( x == (x->parent)->left )
            /* x was on the left of its parent */
            x->parent->left = y;
        else
            /* x must have been on the right */
            x->parent->right = y;
    /* Finally, put x on y's left */
    y->left = x;
    x->parent = y;
    }

rb_insert( Tree T, node x ) {
    node y;
    /* Insert in the tree in the usual way */
    tree_insert( T, x );
    /* Now restore the red-black property */
    x->colour = red;
    while ( (x != T->root) && (x->parent->colour == red) ) {
       if ( x->parent == x->parent->parent->left ) {
           /* If x's parent is a left, y is x's right 'uncle' */
           y = x->parent->parent->right;
           if ( y->colour == red ) {
               /* case 1 - change the colours */
               x->parent->colour = black;
               y->colour = black;
               x->parent->parent->colour = red;
               /* Move x up the tree */
               x = x->parent->parent;
               }
           else {
               /* x's parent is a right */
               if ( x == x->parent->right ) {
                   /* and x is to the right */ 
                   /* case 2 - move x up and rotate */
                   x = x->parent;
                   left_rotate( T, x );
                   }
               /* case 3 */
               x->parent->colour = black;
               x->parent->parent->colour = red;
               right_rotate( T, x->parent->parent );
               }
           }
       else {
           /* repeat the "if" part with right and left
              exchanged */

           /* If x's parent is a right, y is x's left 'uncle' */
           y = x->parent->parent->left;
           if ( y->colour == red ) {
               /* case 1 - change the colours */
               x->parent->colour = black;
               y->colour = black;
               x->parent->parent->colour = red;
               /* Move x up the tree */
               x = x->parent->parent;
               }
           else {
               /* x's parent is a left */
               if ( x == x->parent->left ) {
                   /* and x is to the left */ 
                   /* case 2 - move x up and rotate */
                   x = x->parent;
                   right_rotate( T, x );
                   }
               /* case 3 */
               x->parent->colour = black;
               x->parent->parent->colour = red;
               left_rotate( T, x->parent->parent );
               }
       }
    }
}
