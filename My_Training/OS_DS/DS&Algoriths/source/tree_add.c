/* Binary tree implementation of a collection */

static void AddToTree( Node *t, Node new ) {
	Node base;
	base = *t;
	/* If it's a null tree, just add it here */
	if ( base == NULL ) {
		*t = new;
		return;
		}
	else {
		if ( KeyLess( ItemKey( new->item ), ItemKey( base->item ) ) )
			{
			AddToTree( &(base->left), new );
			}
		else
			AddToTree( &(base->right), new );
		}
	}

void AddToCollection( Collection c, void *item ) {
	Node new, node_p;
	assert( c != NULL );
	assert( item != NULL );
	/* Allocate space for a node for the new item */
	new = (Node)malloc(sizeof(struct t_node));
	/* Attach the item to the node */
	new->item = item;
	new->left = new->right = (Node)0;
	AddToTree( &(c->node), new );
	}
