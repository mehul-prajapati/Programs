/* Binary tree implementation of a collection */

/* Now we need to know whether one key is less, equal or greater than
   another
*/
extern int KeyCmp( void *a, void *b );
/* Returns -1, 0, 1 for a < b, a == b, a > b */


void *FindInTree( Node t, void *key ) {
	if ( t == (Node)0 ) return NULL;
	switch( KeyCmp( key, ItemKey(t->item) ) ) {
		case -1 : return FindInTree( t->left, key );	
		case 0: return t->item;
		case +1 : return FindInTree( t->right, key );
		}
	}

void *FindInCollection( collection c, void *key ) {
/* Find an item in a collection
   Pre-condition: (c is a collection created by a call to ConsCollection) &&
                  (key != NULL)
   Post-condition: returns an item identified by key if
                    one exists, otherwise returns NULL
*/
	assert( c != NULL );
	assert( key != NULL );
	/* Select node at head of list */
	return FindInTree( c->root, key );
	}
