/* Program to demonstrate the cost of
   recursive calls

   Compile and link with coll_ll_cnt.c
*/

#include "collection.h"
#include "timer.h"

void *ItemKey( int *x ) { return x; };

void AddNItems( collection c, int n )
	{
	int x; int cnt = 0;
	x = 23;	/* Any item will do */
	while( n-- ) 
		{
		AddToCollection( c, &x );
		cnt++;
		}
	}

main()
	{
	collection c;
	double t1, t2;
	int i, n, cnt;
	int tests[] = { 1, 100, 1000, 10000, 100000, 200000 };
#define N_TESTS	sizeof(tests)/sizeof(int)

	for(i=0;i<N_TESTS;i++)
	{
	n = tests[i];

	c = ConsCollection( n, sizeof(int) );

	AddNItems( c, n );

	(void)TimeSinceLastCall();
	cnt = CollCnt( c );
	t1 = TimeSinceLastCall();
	printf("Iter cnt %d %s\n", cnt, cnt==n?"OK":"FAIL!" );

	(void)TimeSinceLastCall();
	cnt = CollCnt_rec( c );
	t2 = TimeSinceLastCall();
	printf("Rec cnt %d %s\n", cnt, cnt==n?"OK":"FAIL!" );

	printf("Iterative time %g\nRecursive time %g\n",
		t1, t2 );
	}
	}
