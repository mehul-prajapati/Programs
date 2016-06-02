/** Test the collection class **/

#include <stdio.h>
#include "collection.h"


int *ItemKey( int *item )
	{
	return item;
	}

void AddAll( collection c, int *list, int n )
	{
	int i;
	for(i=0;i<n;i++)
		{
		AddToCollection( c, &list[i] );
		if ( FindInCollection( c, &list[i] ) )
			{}
		else
			{
			printf("Add failure item %d, value %d\n",
					i, list[i] );
			}
		}
	}

void CheckAll( collection c, int *list, int n )
	{
	int i, *ip;
	for(i=0;i<n;i++)
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

void DeleteAll_1( collection c, int *list, int n )
	{
	int i;
	for(i=0;i<n;i++)
		{
		DeleteFromCollection( c, &list[i] );
		if ( FindInCollection( c, &list[i] ) )
			{
			printf("Delete failure item %d, value %d\n",
					i, list[i] );
			}
		}
	}

void DeleteAll_2( collection c, int *list, int n )
	{
	int i;
	for(i=n-1;i>=0;i--)
		{
		DeleteFromCollection( c, &list[i] );
		if ( FindInCollection( c, &list[i] ) )
			{
			printf("Delete failure item %d, value %d\n",
					i, list[i] );
			}
		}
	}

void main()
	{
	collection c;
int list[] = { 2, 3, 45, 67, 89, 99 };
#define N	(sizeof(list)/sizeof(int))

	c = ConsCollection( 100, sizeof( int ) );

	AddAll( c, list, N );
	printf("Added %d items\n", N );
	CheckAll( c, list, N );
	printf("Checked %d items\n", N );
	DeleteAll_1( c, list, N );
	printf("Deleted all items\n" );
	AddAll( c, list, N );
	printf("Added %d items\n", N );
	DeleteAll_2( c, list, N );
	printf("Deleted all items\n" );
	}
	
