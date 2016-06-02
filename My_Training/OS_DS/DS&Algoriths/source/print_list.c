#ifdef DEBUG
void PrintList( collection c )
	{
	struct t_node *n;
	printf("coll %d head %d size %d {",
		c, c->node, c->size );
	n = c->node;
	while( n!=NULL)
		{
		printf("(%d -> %d)", *(int *)(n->item), n->next );
		n = n->next;
		}
	printf("}\n");
	}
