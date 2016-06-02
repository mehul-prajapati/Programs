/* Binary tree implementation of a collection */

#include <stdlib.h>	/* calloc */
#include <stdio.h>	/* NULL */
#include <assert.h>	/* Needed for assertions */
#include "coll_a.h"	/* import the specification */


struct t_node {
	void *item;
	struct t_node *left;
	struct t_node *right;
	} node;

struct t_collection {
	/* Note that size is not needed any longer! */
	int (*ItemCmp)( void *, void * );
	struct t_node *node;
	};

collection ConsCollection(int max_items, 
		int (*ItemCmp)(void *,void *) )
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
	c->ItemCmp = ItemCmp;
	return c;
	}

static void AddToTree( struct t_node **t, struct t_node *new,
			int (*ItemCmp)(void *, void *) )
	{
	struct t_node *base;
	base = *t;
	/* If it's a null tree, just add it here */
	if ( base == NULL )
		{
		*t = new;
		return;
		}
	else
		{
		if ( ItemCmp( base->item, new ) < 0 )
			{
			AddToTree( &(base->left), new, ItemCmp );
			}
		else
			AddToTree( &(base->right), new, ItemCmp );
		}
	}

void AddToCollection( collection c, void *item )
/* Add an item to a collection
   Pre-condition: (c is a collection created by a call to 
                ConsCollection) &&
                  (existing item count < max_items) &&
                  (item != NULL)
   Post-condition: item has been added to c
*/
	{
	struct t_node *new, *node_p;
	assert( c != NULL );
	assert( item != NULL );
	/* Allocate space for a node for the new item */
	new = (struct t_node *)malloc(sizeof(struct t_node));
	/* Attach the item to the node */
	new->item = item;
	new->left = new->right = (struct t_node *)0;
	node_p = c->node;
	AddToTree( &node_p, new, c->ItemCmp );
	}

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

void *FindInTree( struct t_node *t, void *key )
	{


	}

void *FindInCollection( collection c, void *key )
/* Find an item in a collection
   Pre-condition: (c is a collection created by a call to ConsCollection) &&
                  (key != NULL)
   Post-condition: returns an item identified by key if
                    one exists, otherwise returns NULL
*/
	{
	struct t_node *node;
	assert( c != NULL );
	assert( key != NULL );
	/* Select node at head of list */
	node = c->node;
	return FindInTree( node, key );
	}
