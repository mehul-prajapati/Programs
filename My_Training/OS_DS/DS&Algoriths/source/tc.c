/** Test the Collection class **/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "Collection.h"

#include "timer.h"


int KeyCmp( void *a, void *b ) {
	int av, bv;
	av = *(int *)a;
	bv = *(int *)b;
	/* printf("KC a %d b %d\n", av, bv ); */
	if ( av < bv ) return -1;
	else 
		if ( av > bv ) return +1;
	return 0;
	}

void AddAll( Collection c, int *list, int n )
	{
	int i;
	for(i=0;i<n;i++) {
		/* printf("Add %d ..\n", list[i] ); */
		AddToCollection( c, &list[i] );
		/* printf("Find %d ..\n", list[i] );
		if ( FindInCollection( c, &list[i] ) ) {}
		else {
			printf("Add failure item %d, value %d\n",
					i, list[i] );
			}
			*/
		}
	}

int Check( Collection c, int *list, int n )
	{
	int i, *ip, found;
	found = 0;
	for(i=0;i<n;i++)
		{
		if ( (ip=FindInCollection( c, &list[i] )) != NULL )
			{
			if ( *ip == list[i] ) { found++; }
			}
		}
	return found;
	}

int *GenList( int n, int random ) {
	int i, *ip;
	ip = (int *)malloc( n*sizeof(int) );
	for(i=0;i<n;i++) {
		ip[i] = random?rand():i;
		}
	return ip;
	}

#define LOW		1e2
#define HIGH  1e4
#define N_REP	100
#define TESTS	1e4

void main()
	{
	Collection c, cs[N_REP];
	int cnt, n, random, k;
	int *list, *list2;
	double dt, mdt;

	printf("Tree Complexity\n");
	for(random=0;random<2;random++) {
		for( n = LOW; n < HIGH; n = n * 2 ) {
			printf("%6d: ", n ); fflush( stdout );
			list = GenList( n, !random );		
			if ( list == NULL ) {
				printf("Insuff space for list\n");
				continue;
				}
			(void)Elapsed();
			for(k=0;k<N_REP;k++) {
				cs[k] = c = ConsCollection( 100, KeyCmp );
				if( c == NULL ) { break;
					}
				else {
					AddAll( c, list, n );
					}
				}
			if ( c != NULL ) {
				dt = Elapsed();
				mdt = (dt / n) / N_REP;

				printf(" %10.3f %12.4e %12.4e %12.4e",
						dt, mdt, mdt/log((double)n), mdt/n );
				}
			for(k=0;k<N_REP;k++) {
				if( cs[k] != NULL ) { DeleteCollection( cs[k] ); }
				}

#ifdef EXTRA
			list2 = GenList( TESTS, TRUE );
			if ( list2 != NULL ) {
				(void)Elapsed();
				AddAll( c, list2, TESTS );
				dt = Elapsed();
				mdt = dt/TESTS;
				printf("| %10.3f %12.4e %12.4e %12.4e\n",
						dt, mdt, mdt/log((double)n), mdt/n );
				free( list2 );
				}
			else {
				printf("Insuff space for extra list!\n");
				}
#endif
			free( list );
			printf("\n");
			}
		}
	}
	
