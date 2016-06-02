/* Specification for collection */

typedef struct t_collection *collection;

collection ConsCollection( int max_items, 
		int (*ItemCmp)(void *, void *) );
/* Construct a new collection
   Pre-condition: max_items > 0
   Post-condition: returns a pointer to an empty collection
*/

void AddToCollection( collection c, void *item );
/* Add an item to a collection
   Pre-condition: (c is a collection created by a call to
                       ConsCollection) &&
                  (existing item count < max_items) &&
                  (item != NULL)
   Post-condition: item has been added to c
*/

void DeleteFromCollection( collection c, void *item );
/* Delete an item from a collection
   Pre-condition: (c is a collection created by a call to
                     ConsCollection) &&
                  (existing item count >= 1) &&
         	  (item != NULL)
   Post-condition: item has been deleted from c
*/

void *FindInCollection( collection c, void *key );
/* Find an item in a collection
   Pre-condition: c is a collection created by a call to
                     ConsCollection
                  key != NULL
   Post-condition: returns an item identified by key if one
                   exists, otherwise returns NULL
*/
