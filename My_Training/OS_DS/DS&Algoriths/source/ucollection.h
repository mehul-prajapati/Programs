/* Specification of a collection which contains unique items only
   Assumes that the items of which the collection is
   composed supply an ItemKey method which returns the
   pointer to an identifying key for the item
*/

typedef struct u_collection_struct *u_collection;

u_collection ConsUCollection( int max_items );
/* Construct a new collection
   Pre-condition: None
   Post-condition: returns a pointer to an empty
      	collection
*/

void AddToUCollection( u_collection c, void *item );
/* Add an item to a collection
   Pre-condition: 
       (c was created by a call to ConsUCollection) &&
       (item != NULL) &&
       (FindInUCollection(c,ItemKey(item))==NULL)
   Post-condition: 
       FindInUCollection(c,ItemKey(item)) != NULL
*/

void DeleteFromUCollection( u_collection c, void *item );
/* Delete an item from a collection
   Pre-condition: 
       (c was created by a call to ConsUCollection) &&
       (item != NULL) &&
       (FindInUCollection(c,ItemKey(item)) != NULL)
   Post-condition: 
       FindInUCollection(c,ItemKey(item)) == NULL
*/

void *FindInUCollection( u_collection c, void *key );
/* Find an item in a collection
   Pre-condition: c is a collection created by a call to 
                     ConsCollection
                  key != NULL
   Post-condition: returns an item identified by key if
                   one exists, otherwise returns NULL
*/
