/* Binary tree implementation of a collection */

#include <stdlib.h>	/* calloc */
#include <stdio.h>	/* NULL */
#include <assert.h>	/* Needed for assertions */
#include "collection.h"	/* import the specification */

extern void *ItemKey( void * );

#include "tree_struct.c"

collection ConsCollection(int max_items, int item_size )
/* Construct a new collection
   Pre-condition: (max_items > 0) && (item_size > 0)
   Post-condition: returns a pointer to an empty
collection
*/
	{
	collection c;
	/* Although redundant, this assertion should be
	   retained as it tests compliance with the formal
           specification */
 	assert( max_items > 0 );
	c = (collection)calloc( 1, sizeof(struct t_collection) );
	c->node = (struct t_node *)0;
	c->size = item_size;
	return c;
	}

#define KeyLess(a,b)	(KeyCmp(a,b)<0)

void PrintSubTree( struct t_node *t, int level )
	{
	int i;
	for(i=0;i<level;i++) printf( "\t" );
	printf("Node: %d L %d R %d Item %d (%d)\n",
		t, t->left, t->right, t->item, *(int *)(t->item) );
	if ( t->left != NULL ) PrintSubTree( t->left, level+1 );
	if ( t->right != NULL ) PrintSubTree( t->right, level+1 );
	}

void PrintTree( collection c )
	{
	printf("\nTree: size %d node %d\n", c->size, c->node );
	PrintSubTree( c->node, 0 );
	}

#include "tree_add.c"

void DeleteFromTree( struct t_node **t, void *item )
	{

	}

void DeleteFromCollection( collection c, void *item )
/* Delete an item from a collection
   Pre-condition: (c is a collection created by a call to
                     ConsCollection) &&
                  (existing item count >= 1) &&
                  (item != NULL)
   Post-condition: item has been deleted from c
*/
	{
	struct t_node *node;
	assert( c != NULL );
        /* The requirement that the collection has at least
           one item is expressed a little differently */
	assert( c->node != NULL );
	assert( item != NULL);
	/* Select node at head of list */
	node = c->node;
	DeleteFromTree( &node, item );
	}

#include "tree_find.c"
