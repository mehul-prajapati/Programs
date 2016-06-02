/* Specification for Collection */

typedef struct t_Collection *Collection;

Collection ConsCollection( int max_items, int item_size );
/* Construct a new Collection
   Pre-condition: max_items > 0
   Post-condition: returns a pointer to an empty Collection
*/

void AddToCollection( Collection c, void *item );
/* Add an item to a Collection
   Pre-condition: (c is a Collection created by a call to
                       ConsCollection) &&
                  (existing item count < max_items) &&
                  (item != NULL)
   Post-condition: item has been added to c
*/

void DeleteFromCollection( Collection c, void *item );
/* Delete an item from a Collection
   Pre-condition: (c is a Collection created by a call to
                     ConsCollection) &&
                  (existing item count >= 1) &&
         	  (item != NULL)
   Post-condition: item has been deleted from c
*/

void *FindInCollection( Collection c, void *key );
/* Find an item in a Collection
   Pre-condition: c is a Collection created by a call to
                     ConsCollection
                  key != NULL
   Post-condition: returns an item identified by key if one
                   exists, otherwise returns NULL
*/
