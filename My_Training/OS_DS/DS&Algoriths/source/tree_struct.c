/* Binary tree implementation of a collection */

struct t_node {
	void *item;
	struct t_node *left;
	struct t_node *right;
	};

typedef struct t_node *Node;

struct t_collection {
	int size;	/* Needed by FindInCollection */
	Node root;
	};

