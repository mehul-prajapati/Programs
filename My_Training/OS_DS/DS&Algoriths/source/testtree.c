/** Test the collection class **/

#include <stdio.h>
#include "collection.h"

int *list;
int N = 100;

extern double drand48();
int rand()
	{
	return 100*drand48();
	}

int *ItemKey( int *item )
	{
	return item;
	}

int KeyCmp( int *a, int *b )
	{
	if ( *a < *b ) return -1;
	else
		if ( *a == *b ) return 0;
		else return 1;
	}

void GenList( int n )
	{
	int i;
	list = (int *)malloc( n*sizeof( int ) );
	for(i=0;i<n;i++) list[i] = rand();
	}
	
void AddAll( collection c )
	{
	int i;
	extern void PrintTree( collection c );
	for(i=0;i<N;i++)
		{
		AddToCollection( c, &list[i] );
		PrintTree( c );
		if ( FindInCollection( c, &list[i] ) )
			{}
		else
			{
			printf("Add failure item %d, value %d\n",
					i, list[i] );
			}
		}
	}

void CheckAll( collection c )
	{
	int i, *ip;
	for(i=0;i<N;i++)
		{
		if ( (ip=FindInCollection( c, &list[i] )) != NULL )
			{
			if ( ip == &list[i] ) {}
			else
				{
				printf("Find mismatch: list[%d] = %d ", i, list[i] );
				printf(" @ %d/ %d @ %d\n", &list[i], *ip, ip );
				}
			}
		else
			{
			printf("Find failure item %d, value %d\n",
					i, list[i] );
			}
		}
	}

void DeleteAll_1( collection c )
	{
	int i;
	for(i=0;i<N;i++)
		{
		DeleteFromCollection( c, &list[i] );
		if ( FindInCollection( c, &list[i] ) )
			{
			printf("Delete failure item %d, value %d\n",
					i, list[i] );
			}
		}
	}
void DeleteAll_2( collection c )
	{
	int i;
	for(i=N-1;i>=0;i--)
		{
		DeleteFromCollection( c, &list[i] );
		if ( FindInCollection( c, &list[i] ) )
			{
			printf("Delete failure item %d, value %d\n",
					i, list[i] );
			}
		}
	}

void main( int argc, char *argv[] )
	{
	collection c;
	if ( argc > 1 ) N = atoi( argv[1] );
	printf("Collection size %d\n", N );
	GenList( N );
	c = ConsCollection( N, sizeof( int ) );

	AddAll( c );
	printf("Added %d items\n", N );
	CheckAll( c );
	printf("Checked %d items\n", N );
	DeleteAll_1( c );
	printf("Deleted all items\n" );
	AddAll( c );
	printf("Added %d items\n", N );
	DeleteAll_2( c );
	printf("Deleted all items\n" );
	}
	
