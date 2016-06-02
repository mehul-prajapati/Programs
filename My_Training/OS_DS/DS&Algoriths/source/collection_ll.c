/* Linked list implementation of a collection */

#include <stdlib.h>	/* calloc */
#include <stdio.h>	/* NULL */
#include <assert.h>	/* Needed for assertions */
#include "collection.h"	/* import the specification */

extern void *ItemKey( void * );

struct t_node {
	void *item;
	struct t_node *next;
	} node;

struct t_collection {
	int size;	/* Needed by FindInCollection */
	struct t_node *node;
	};

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
 	assert( item_size > 0 );
	c = (collection)calloc( 1, sizeof(struct t_collection) );
	c->node = (struct t_node *)0;
	c->size = item_size;
	return c;
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
	struct t_node *new;
	assert( c != NULL );
	assert( item != NULL );
	/* Allocate space for a node for the new item */
	new = (struct t_node *)malloc(sizeof(struct t_node));
	/* Attach the item to the node */
	new->item = item;
	/* Make the existing list `hang' from this one */
	new->next = c->node;
	/* The new item is the new head of the list */
	c->node = new;
	assert( FindInCollection( c, ItemKey( item ) ) != NULL );
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
	struct t_node *node, *prev;
	assert( c != NULL );
        /* The requirement that the collection has at least
           one item is expressed a little differently */
	assert( c->node != NULL );
	assert( item != NULL);
	/* Select node at head of list */
	prev = node = c->node;
	/* Loop until we've reached the end of the list */
	while( node != NULL )
		{
		if ( item == node->item )
			{
			/* Found the item to be deleted,
                           re-link the list around it */
			if( node == c->node )
				/* We're deleting the head */
				c->node = node->next;
			else
				prev->next = node->next;
			/* Free the node */
			free( node );
			break;
			}
		prev = node;
		node = node->next;
		}
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
	while( node != NULL)
		{
		if ( memcmp(key,ItemKey(node->item),c->size)==0 )
			{
			return node->item;
			}
                node = node->next;
		}
	return NULL;
	}
