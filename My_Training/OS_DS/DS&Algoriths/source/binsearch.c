static void *bin_search( collection c, int low, int high, void *key ) {
	int mid, cmp;
	/* Termination check */
	if (low > high) return NULL;
	mid = (high+low)/2;
	cmp = memcmp(key,ItemKey(c->items[mid]),c->size);
	if ( cmp == 0 ) 
		/* Match, return item found */
		return c->items[mid];
	else
		if ( cmp < 0 )
			/* key is less than mid, search lower half */
			return bin_search( c, low, mid-1, key);
		else	
			/* key is greater than mid, search upper half */
			return bin_search( c, mid+1, high, key );
	}

void *FindInCollection( collection c, void *key )
/* Find an item in a collection
   Pre-condition: 
	c is a collection created by ConsCollection
	c is sorted in ascending order of the key
	key != NULL
   Post-condition: returns an item identified by key if
   one exists, otherwise returns NULL
*/
	{
	int low, high;
	low = 0; high = c->item_cnt-1;
	return bin_search( c, low, high, key );
	}
