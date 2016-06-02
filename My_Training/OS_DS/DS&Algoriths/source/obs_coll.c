/* Collection with optimal binary search tree */

#include <stdio.h>	
#include <stdlib.h>	
#include <assert.h>	

#include "obs_coll.h"	

#define C(i,j)          Cost(cost,i,j,c->cnt)
#define CL(i,j)         cost[i*c->cnt+j]

#define BEST(i,j)       best[i*(c->cnt)+j]

double Cost( double *cost, int j, int k, int n ) {
	if ( (j<0) || (j>k) || (j>=n) || (k>=n) ) return 0.0;
	return cost[j*n+k];
	}
	

#define POS(i)		(i-1)
#define MAX_FREQ        1.0e38
#define PARENT(i)	(i/2)
#define LEFT(i)		(2*i)
#define RIGHT(i)	(2*i+1)
#define EMPTY(i,j)	(j>=c->cnt)
#define SWAP(i,j)	{void *x=c->items[i]; \
			 double d=c->rel_freq[i];\
					c->items[i]=c->items[j]; \
					c->rel_freq[i]=c->rel_freq[j];\
					c->items[j]=x;\
					c->rel_freq[j]=d; }


/****************************************************************************/
struct t_node {
	int *treeitem;
	struct t_node *left;
	struct t_node *right;
	}node;

typedef struct t_node *Node;

static Node ConsNode( void *item ) {
	Node n;
	n = malloc( sizeof(struct t_node) );
	if ( n != NULL ) {
		n->treeitem = item;
		n->left = n->right = NULL;
		}
	return n;
	}

typedef struct item {
	void *item;
	double rel_freq;
	} *Item;

struct t_Collection {
	int cnt;
	int max_items;

	/* original */
	double *rel_freq;
	void **items;	/* array for initially inserting items */

	/* New */
	Item item_list;
	
	Node root;		/* root of binary search tree */
	int tree_cnt;

	int (*ItemCmp)(void *,void *);
  };

void print_int_array( int *best, int n ) {
	int k, j;
	printf("--- %dx%d-----\n",n,n );
	for(j=0;j<n;j++) {
		for(k=0;k<n;k++) {
			printf("%4d", best[n*k+j] );
			}
		printf("\n");
		}
	printf("-------------\n");
	}

void print_d_array( double *best, int n ) {
	int k, j;
	printf("--- %dx%d-----\n",n,n );
	for(j=0;j<n;j++) {
		for(k=0;k<n;k++) {
			printf("%9.1g", best[n*k+j] );
			}
		printf("\n");
		}
	printf("-------------\n");
	}

Collection ConsCollection(int max_items, int (*ItemCmp)(void *,void *))
	{
	Collection c;
	assert( max_items > 0 );
	assert( ItemCmp != NULL );

	c = (Collection)calloc( 1, sizeof(struct t_Collection) );
	if ( c != NULL ) {
		c->rel_freq = (double *)calloc(max_items,sizeof(double ));
		c->items = (void **)calloc(max_items,sizeof(void *));
		c->item_list = (Item)calloc( max_items, sizeof( struct item ) );
		c->max_items = max_items;
		c->ItemCmp = ItemCmp;
		}
	return c;
	}


int AddToCollection( Collection c, void *item, double freq ) {
	assert( c != NULL );
	assert( item != NULL );
	assert( c->cnt < c->max_items );
	c->item_list[c->cnt].item = item;
	c->item_list[c->cnt].rel_freq = freq;
	c->rel_freq[c->cnt] = freq;
	c->items[c->cnt++] = item;
	return TRUE;
	}

static int *opt_bin( Collection c ) {
  int i, k, tree_size;
  double t;
	int *best;
	double *cost;

	int left,right,c_root=0,best_root;
	double a,best_cost;

	best = (int *)calloc( (c->cnt)*(c->cnt), sizeof(int) );
  /* Alocate cost array */
  cost = (double *)calloc( (c->cnt)*(c->cnt), sizeof(double) );
  /* Initialise all sub-trees to maximum */
	
	PrintCollection(c);
  for(i=0;i<c->cnt;i++) {
  	for(k=(i+1);k<(c->cnt);k++) {
			CL(i,k) = MAX_FREQ;
			}
		}

	/* but we know the cost of single-item trees */
	for(i=0;i<c->cnt;i++) {
		CL(i,i) = c->rel_freq[i];
		BEST(i,i) = i;
		printf("cost(%d,%d) = %f\n", i,i, C(i,i));
		}

	print_d_array(cost, c->cnt );
	
  /* For trees of size j */
	printf("the number of items is %i\n", c->cnt);

	/***********TREE EVALUATE*******************************************/
	for( tree_size=2; tree_size<=c->cnt; tree_size++ ) {
		printf("\nTrees of size %d\n",tree_size);
		for(left=0;left<= (c->cnt-tree_size);left++) {
			right = left + tree_size - 1;
			best_cost = C(left,right);
			best_root = left;
			printf("ts %d left %d right = %d bc %10.2f\n", tree_size, left, right, best_cost);
			for( c_root=left; c_root<=right; c_root++ )
				{
				printf("c_root = %d ", c_root );
				a = C(left,c_root-1) + C(c_root+1,right);
				printf(" c1 %10.2f ", C(left,c_root-1)  );
				printf(" c2 %10.2f\n ", C(c_root+1,right) );
				if (a<best_cost)
					{
					best_cost = a;
					best_root = c_root;
					}
				printf(" best_cost = %10.2g  b %d\n", best_cost, best_root );
				}
			BEST(left,right) = best_root;
			CL(left,right) = best_cost;
			a=0;
			for (k=left;k<=right-1;k++) {
				a = a + c->rel_freq[k];
		    }
			CL(left,right) = C(left,right) + a;
			printf("TS %d bst_c %10.2g  b %d\n", tree_size, best_cost, best_root );
			}
		//print_int_array( best, c->cnt );
		}
			
	print_int_array( best, c->cnt );
	return best;
	}

/*************************************************************/
Node BuildSubTree( Collection c, int *best, int low, int high ) {
	int root;
	Node n;
	if ( low <= high ) {
		root = BEST(low,high);
		n = ConsNode( c->item_list[root].item );
		c->tree_cnt++;
		n->left = BuildSubTree( c, best, low, root-1 );
		n->right = BuildSubTree( c, best, root+1, high );
		}
	else {
		n = NULL;
		}
	return n;
	}


int FormSearchTree( Collection c ) {
	int *best;
	int root;

	/* Sort data on key */
	
	best = opt_bin( c );

	/* Use the best matrix form a tree */
	root = BEST(0, c->cnt-1);
	printf("Root is %d\n", root );

	c->tree_cnt = 0;
	c->root = BuildSubTree( c, best, 0, c->cnt-1 );

	printf("Tree constructed %d items (total %d)\n", c->tree_cnt, c->cnt );
	return TRUE;
	}


/*
the root of the optimal tree is best[0,n-1]  
the left child in best[1,[best[1,n]-1]
right in best[best[1,n]+1,n], etc
*/

void *FindInTree( Node t, void *item, int (*ItemCmp)(void*, void*) ) {
	if ( t == (Node)0 ) return NULL;
	switch(ItemCmp( item, t->treeitem ) ) {
		case -1 : return FindInTree( t->left, item, ItemCmp );
		case 0: return t->treeitem;
		case +1 : return FindInTree( t->right, item, ItemCmp );
		}
	}

void *FindInCollection( Collection c, void *item ) {
/* Find an item in a collection
	Pre-condition: (c is a collection created by a call to ConsCollection) &&
						(key != NULL)
	Post-condition: returns an item identified by key if
						  one exists, otherwise returns NULL
*/
	assert( c != NULL );
	assert( item != NULL );
	assert(c->ItemCmp != NULL);
	/* Select node at head of list */
	return FindInTree( c->root, item, c->ItemCmp );
	}

void PrintCollection( Collection c ) {
	int i;
	assert( c != NULL );
	assert( c->items != NULL );

	printf("key\titem\tfrequency\n");
	for(i=0;i<c->cnt;i++) {
		printf("%d %d %f\n", i, (c->items[i]), c->rel_freq[i]);
		}
	printf("\n");
}

