/* Array implementation of a collection */

#include <stdio.h>	/* Definition of NULL */
#include <assert.h>	/* Needed for assertions */
#include "collection.h"	/* import the specification */

extern char *ItemKey( void * );

struct t_collection {
	int item_cnt;
	int max_items;	/* Not strictly necessary */
	int size;	/* Needed by FindInCollection */
	char **items;
        };

collection ConsCollection(int max_items, int item_size )
/* Construct a new collection
   Pre-condition: (max_items > 0) &&
                  (item_size > 0)
   Post-condition: returns a pointer to an empty collection
*/
	{
	collection c;
	assert( max_items > 0 );
	c = (collection)calloc( 1, sizeof(struct t_collection) );
	c->items = (char **)calloc(max_items,sizeof(void *));
	c->size = item_size;
	c->max_items = max_items;
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
	assert( c != NULL);
	assert( c->item_cnt < c->max_items );
	assert( item != NULL);
	c->items[c->item_cnt++] = item;
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

#include "binsearch.c"
