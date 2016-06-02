/* Array implementation of a Collection */

#include <stdio.h>	/* Definition of NULL */
#include <assert.h>	/* Needed for assertions */
#include "Collection.h"	/* import the specification */

extern void *ItemKey( void * );

struct t_Collection {
	int item_cnt;
	int max_items;	/* Not strictly necessary */
	int size;	/* Needed by FindInCollection */
	void **items;
        };

Collection ConsCollection(int max_items, int item_size )
/* Construct a new Collection
   Pre-condition: (max_items > 0) &&
                  (item_size > 0)
   Post-condition: returns a pointer to an empty Collection
*/
	{
	Collection c;
	assert( max_items > 0 );
	assert( item_size > 0 );
	c = (Collection)calloc( 1, sizeof(struct t_Collection) );
	c->items = (void **)calloc(max_items,sizeof(void *));
	c->size = item_size;
	c->max_items = max_items;
	return c;
	}

static void DeleteTree( Node n ) {
	if ( n != NULL ) {
		DeleteTree( n->left );
		DeleteTree( n->left );
		free( n );
		}
	}

void DeleteCollection( Collection c ) {
	DeleteTree( c->root );
	free( c );
	}

void AddToCollection( Collection c, void *item )
/* Add an item to a Collection
   Pre-condition: (c is a Collection created by a call to
                       ConsCollection) &&
                (existing item count < max_items) &&
                (item != NULL)
   Post-condition: item has been added to c
*/
	{
	assert( c != NULL);
	assert( c->item_cnt < c->max_items );
	assert( item != NULL);
	c->items[c->item_cnt++] = item;
	/* Post-condition */
	assert( FindInCollection( c, ItemKey( item ) ) != NULL );
	}

void DeleteFromCollection( Collection c, void *item )
/* Delete an item from a Collection
   Pre-condition: (c is a Collection created by a call to
                     ConsCollection) &&
                  (existing item count >= 1) &&
                  (item != NULL)
   Post-condition: item has been deleted from c
*/
	{
	int i;
	assert( c != NULL );
	assert( c->item_cnt >= 1 );
	assert( item != NULL );
	for(i=0;i<c->item_cnt;i++)
		{
		if ( item == c->items[i] )
			{
			/* Found the item to be deleted,
			   shuffle all the rest down */
			while( i < c->item_cnt )
				{
				c->items[i] = c->items[i+1];
				i++;
				}
			c->item_cnt--;
			break;
			}
		}
	}

void *FindInCollection( Collection c, void *key )
/* Find an item in a Collection
   Pre-condition: c is a Collection created by a call to 
	               ConsCollection
                  key != NULL
   Post-condition: returns an item identified by key if one
                   exists, otherwise returns NULL
*/
	{
	int i;
	assert( c != NULL );
	assert( key != NULL );
	for(i=0;i<c->item_cnt;i++)
		{
		if (memcmp(ItemKey(c->items[i]),key,c->size)==0)
			return c->items[i];
		}
	return NULL;
	}
