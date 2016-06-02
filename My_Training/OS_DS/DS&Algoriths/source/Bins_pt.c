
/* Bins.c
   Possible bin array for RadixSort
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "Bins.h"

struct t_bins {
	int n_bins, max_items;
	int *bin_cnt;
	void ***bin_pts;
	};

Bins ConsBins( int n_bins, int items_per_bin )
/* Construct an array of n_bins bins,
   each with items_per_bin spaces */
{
	Bins b;
	int i;
	int *bins;
	b = (Bins)malloc( sizeof( struct t_bins ) );
	if ( b != NULL ) {
		b->n_bins = n_bins;
		b->max_items = items_per_bin;
		b->bin_pts = (void ***)malloc( n_bins*sizeof(void **) );
		b->bin_cnt = (int *)calloc( n_bins, sizeof(int) );
		if ( b->bin_pts != NULL ) {
			bins = malloc( n_bins*items_per_bin*sizeof(void *));
			if( bins == NULL ) {
				printf("ConsBins: insuff mem %d bytes needed\n",
					n_bins*items_per_bin*sizeof(void *));
				return NULL;
				}
							
			for(i=0;i<n_bins;i++) {
/*
				b->bin_pts[i] = (void **)malloc( items_per_bin*sizeof(void *) );
				if( b->bin_pts[i] == NULL ) {
					printf("ConsBins: insuff mem after %d bins\n", i );
					b = NULL;
				}
*/	
				b->bin_pts[i] = (void *)bins;
				bins += (items_per_bin);
			}
		}
	}
	else {
		printf("Insuff mem\n");
	}
	return b;
	}
	
int AddItem( Bins b, void *item, int bin_index ) {
/* Add item to bin bin_index
   Pre: b != NULL && item != NULL &&
        bin_index >= 0 && bin_index < n_bins
*/
	int k;
	assert( b != NULL );
	assert( item != NULL );
	assert( bin_index >= 0 );
	assert( bin_index < b->n_bins );
	k = b->bin_cnt[bin_index];
	(b->bin_pts[bin_index])[k] = item;
	b->bin_cnt[bin_index]++;
	return 1;
	}

void *MergeBins( Bins b ) {
/* Merge the bins by copying all the elements in bins 1..n_bins-1
   into bin 0 ..
   return a pointer to bin 0
   (This pointer can be used in the next phase!)
*/
	int i0, j, k;
	assert( b != NULL );
	
	i0 = b->bin_cnt[0];
	for( j = 1; j<b->n_bins; j++ ) {
		for(k=0;k<b->bin_cnt[j];k++) {
			(b->bin_pts[0])[i0] = (b->bin_pts[j])[k];
			i0++;
			}
		}
	return b->bin_pts[0];
	}

	

void FreeUnusedBins( Bins b ) {
/* Free bins 1 .. n_bins-1 in preparation for next phase */
	int k;
	assert( b != NULL );
	for(k=1;k<b->n_bins;k++) {
		assert( b->bin_pts[k] != NULL );
		free( b->bin_pts[k] );
		}
	}

void DeleteBins( Bins b ) {
/* Destructor .. frees all space used by b */
	assert( b != NULL );
	free( b->bin_pts[0] );
	free( b->bin_cnt );
	free( b );
	}
