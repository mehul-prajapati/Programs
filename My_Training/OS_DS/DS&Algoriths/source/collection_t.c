/* Binary tree implementation of a collection */

#include <stdlib.h>	/* calloc */
#include <stdio.h>	/* NULL */
#include <assert.h>	/* Needed for assertions */
#include "collection.h"	/* import the specification */

extern void *ItemKey( void * );
extern void *KeyLess( void *, void * );

struct t_node {
	void *item;
	struct t_node *left;
	struct t_node *right;
	};

typedef struct t_node *Node;

struct t_collection {
	int size;	/* Needed by FindInCollection */
	Node root;
	};

collection ConsCollection(int max_items, int item_size ) {
/* Construct a new collection
   Pre-condition: (max_items > 0) && (item_size > 0)
   Post-condition: returns a pointer to an empty
collection
*/
	collection c;
	/* Although redundant, this assertion should be
	   retained as it tests compliance with the formal
           specification */
 	assert( max_items > 0 );
	c = (collection)calloc( 1, sizeof(struct t_collection) );
	c->root = (Node)0;
	c->size = item_size;
	return c;
	}

static void AddToTree( Node *t, Node new ) {
	Node base;
	base = *t;
	/* If it's a null tree, just add it here */
	if ( base == NULL ) {
		*t = new;
		return;
		}
	else {
		if ( KeyLess( ItemKey( base->item ), ItemKey( new ) ) )
			{
			AddToTree( &(base->left), new );
			}
		else
			AddToTree( &(base->right), new );
		}
	}

void AddToCollection( collection c, void *item ) {
/* Add an item to a collection
   Pre-condition: (c is a collection created by a call to 
                ConsCollection) &&
                  (existing item count < max_items) &&
                  (item != NULL)
   Post-condition: item has been added to c
*/
	Node new;
	assert( c != NULL );
	assert( item != NULL );
	/* Allocate space for a node for the new item */
	new = (Node)malloc(sizeof(struct t_node));
	/* Attach the item to the node */
	new->item = item;
	new->left = new->right = (Node)0;
	AddToTree( &(c->root), new );
	}

void DeleteFromTree( Node *t, void *item ) {

	}

void DeleteFromCollection( collection c, void *item ) {
/* Delete an item from a collection
   Pre-condition: (c is a collection created by a call to
                     ConsCollection) &&
                  (existing item count >= 1) &&
                  (item != NULL)
   Post-condition: item has been deleted from c
*/
	Node node;
	assert( c != NULL );
        /* The requirement that the collection has at least
           one item is expressed a little differently */
	assert( c->root != NULL );
	assert( item != NULL);
	/* Select node at head of list */
	node = c->root;
	DeleteFromTree( &node, item );
	}

void *FindInTree( Node t, void *key ) {


	}

void *FindInCollection( collection c, void *key ) {
/* Find an item in a collection
   Pre-condition: (c is a collection created by a call to ConsCollection) &&
                  (key != NULL)
   Post-condition: returns an item identified by key if
                    one exists, otherwise returns NULL
*/
	Node node;
	assert( c != NULL );
	assert( key != NULL );
	/* Select node at head of list */
	node = c->root;
	return FindInTree( node, key );
	}
