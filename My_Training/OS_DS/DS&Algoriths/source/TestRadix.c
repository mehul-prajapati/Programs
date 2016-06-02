/* 
  Test Radix Sort 
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "RadixSort.h"


int *FillList( int *ip, int n ) {
	int k;
	if ( ip != NULL ) {
		for(k=0;k<n;k++) ip[k] = rand();
		}
	return ip;
	}

int *AllocList( int n ) {
	int *ip;
	ip = (int *)malloc( n * sizeof( int ) );
	return ip;
	}

int VerifySort( int *ip, int n ) {
	int k;
	for(k=0;k<(n-1);k++) {
		if ( ip[k+1]<ip[k] ) {
			printf("Order error %d: %d s/b < %d\n",
				k, ip[k], ip[k+1] );
			return 0;
		}
	}
	return 1;
	}
	

int comp( const int *a, const int *b ) {
	if ( *a < *b ) return -1;
	if ( *a == *b ) return 0;
	return 1;
	}
	
int main( int argc, char *argv[] ) {
	int *list, *new_list, n;
	long tr, tq;
	long max_n;
	double t;

	if ( argc>1 ) max_n = atol( argv[1] );
	else max_n = 256;
	printf("Test Sorts\n"); fflush( stdout );
	for( n=4;n<max_n;n = n<<1) {
		printf("%8d: ", n); fflush( stdout );
		list = AllocList( n );
		printf(" %u ", (unsigned)list );
		list = FillList( list, n );
		tr = clock();
		new_list = RSort( list, n );
		tr = clock() - tr;
		t = tr/1.0e6;
		if ( VerifySort( new_list, n ) ) {
			printf(" OK ");
			}
		printf(" tr %10.3e %10.3e ", t, t/n );
		free( list );
		list = FillList( new_list, n );
		tq = clock();
		qsort(list,n,sizeof(int), comp );
		tq = clock() - tq;
		t = tq/1.0e6;
		printf(" tq %10.3e %10.3e ", t, t/(n*log((double)n)) );
		if ( VerifySort( list, n ) ) {
			printf(" OK ");
			}
		printf("\n");
		free( list );
		}
	return 1;
}
