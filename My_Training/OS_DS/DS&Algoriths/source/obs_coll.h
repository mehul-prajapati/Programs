/* Specification for Collection
   Suitable for constructing optimal binary search tree
*/

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

typedef struct t_Collection *Collection;

Collection ConsCollection( int max_items, int (*ItemCmp)(void *, void *) );
/* Construct a new Collection
   Pre-condition: max_items > 0
   Post-condition: returns a pointer to an empty Collection
*/

int AddToCollection( Collection c, void *item, double freq );
/* Add an item to a Collection
   Pre-condition: (c is a Collection created by a call to
                       ConsCollection) &&
                  (existing item count < max_items) &&
                  (item != NULL)
									(freq > 0.0)
   Post-condition: return TRUE if item has been added to c,
                          FALSE otherwise
*/

void DeleteFromCollection( Collection c, void *item );
/* Delete an item from a Collection
   Pre-condition: (c is a Collection created by a call to
                     ConsCollection) &&
                  (existing item count >= 1) &&
         	  (item != NULL)
   Post-condition: item has been deleted from c
*/

void *FindInCollection( Collection c, void *item );
/* Find an item in a Collection
   Pre-condition: c is a Collection created by a call to
                     ConsCollection
                  key != NULL
   Post-condition: returns an item identified by key if one
                   exists, otherwise returns NULL
*/

int FormSearchTree( Collection c );

void PrintCollection( Collection c );
