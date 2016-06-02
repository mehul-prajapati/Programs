/* Extract the highest priority from the heap */

#define LEFT(k)		(2*k)
#define RIGHT(k)	(2*k+1)
#define EMPTY(c,k)	(k>=c->item_cnt)
#define SWAP(i,j)	{ void *x = c->items[i]; \
			c->items[i] = c->items[j]; \
			c->items[j] = x; }

void MoveDown( collection c, int k )
	{
	int larger, right, left;
	left = LEFT(k);
	right = RIGHT(k);
	if ( !EMPTY(c,k) )	/* Termination condition! */
		{
		larger=left;
		if ( !EMPTY(c,right) )
			{
			if ( ItemCmp( c->items[right], c->items[larger] ) > 0 )
				larger = right;
			}
		if ( ItemCmp( c->items[k], c->items[larger] ) )
			{
			SWAP( k, larger );
			MoveDown( c, larger );
			}
		}
	}

void *HighestPriority( collection c )
/* Return the highest priority item
   Pre-condition: (c is a collection created by a call to
                     ConsCollection) &&
                  (existing item count >= 1) &&
                  (item != NULL)
   Post-condition: item has been deleted from c
*/
	{
	int i, cnt;
	void *save;
	assert( c != NULL );
	assert( c->item_cnt >= 1 );
	/* Save the root */
	save = c->items[0];
	/* Put the last item in the root */
	cnt = c->item_cnt;
	c->items[0] = c->items[cnt-1];
	/* Adjust the count */
	c->item_cnt--;
	/* Move the new root item down if necessary */
	MoveDown( c, 1 );
	return save;
	}
