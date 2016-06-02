/* Special linked list counting test */

#include "collection_ll.c"

int CollCnt( collection c )
	{
	struct t_node *n;
	int cnt = 0;
	n = c->node;
	while ( n != NULL ) 
		{
		cnt++;
		n = n->next;
		}
	return cnt;
	}

static int Cnt( struct t_node *n )
	{
	if ( n == NULL ) return 0;
	else return 1 + Cnt( n->next );
	}
	
int CollCnt_rec( collection c )
	{
	return Cnt( c->node );
	}
