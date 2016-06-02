/* Optimal Binary Search Tree */

#define MAX_FREQ	1.0e38
#define C(i,j)		cost[i*(n+1)+j]
#define BEST(i,j)	best[i*(n+1)+j]

int *opt_bin( double *rf, int n ) {
	int i, j, k;
	int *best;
	double *cost, t;
	/* Allocate best array */
	best = (int *)calloc( (n+1)*(n+1), sizeof(int) );
	/* Alocate cost array */
	cost = (double *)calloc( (n+1)*(n+1), sizeof(double) );
	/* Initialise all sub-trees to maximum */
	for(i=0;i<n;i++)
		for(j=(i+1);j<(n+1);j++) 
			C(i,j) = MAX_FREQ;
	/* but we know the cost of single-item trees */
	for(i=0;i<n;i++) C(i,i) = rf[i];
	/* Initialise above the diagonal to allow for nodes with
           only one child */
	for(i=0;i<(n+1);i++) C(i,i-1) = 0;

	/* For sub-trees ending at j */
	for(j=1;j<n;j++) {
		/* Consider trees starting at 1 to n-j */
		for(i=1;i<=(n-j);i++) {
			/* Check each key in i,i+j as a possible root */
			for(k=i;k<=(i+j);k++) {
				t = C(i,k-1) + C(k+1,i+j);
				if ( t < C(i,i+j) ) {
					C(i,i+j) = t;
					BEST(i,i+j) = k;
					}
				}
			/* Add the cost of each key because we're
			   pushing the whole tree down one level */
			t = 0;
			for(k=i;k<=(i+j);k++) t = t + rf[k];
			C(i,i+j) = C(i,i+j) + t;
			}
		}
	return best;
}
	
