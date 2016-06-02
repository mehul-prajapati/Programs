/* Test the optimal binary search tree */

#include <stdio.h>
#include <stdlib.h>

#include "obs_coll.h"


struct c_item {
	char a;
	double freq;
	} test[] = {
	{ 'A', 23.0 },
	{ 'B',  3.0 },
	{ 'C', 10.0 },
	{ 'D', 16.0 },
	{ 'E', 30.0 },
	{ 'F', 11.0 },
	{ 'G', 17.0 },
	{ 'H', 18.0 },
	{ 'I', 20.0 }
	};

typedef struct c_item *Item;

#define N_ITEMS	(sizeof(test)/sizeof(struct c_item))

int ItemCmp( void *a, void *b ) {
	if ( ((Item)a)->a < ((Item)b)->a ) return -1;
	if ( ((Item)a)->a > ((Item)b)->a ) return  1;
	return 0;
	}
	
main( int argc, char *argv[] ) {
	Collection c;
	int k, n;

	n = N_ITEMS;
	c = ConsCollection( n, ItemCmp );

	for(k=0;k<n;k++) {
		if( AddToCollection( c, &(test[k].a), test[k].freq ) ) {}
		else printf("Error adding item %d\n", k );
		}

	FormSearchTree( c );

	
	for(k=0;k<n;k++) {
		if( FindInCollection( c, &(test[k]) ) != NULL ) {
			printf("Found item %d\n", k );
			}
		else printf("Error finding item %d\n", k );
		}

}
