/* Bubble sort for integers */
#define SWAP(a,b)   { int t; t=a; a=b; b=t; }

void bubble( int a[], int n )
/* Pre-condition: a contains n items to be sorted */
    {
    int i, j;
    /* Make n passes through the array */
    for(i=0;i<n;i++)
        {
        /* From the first element to the end
           of the unsorted section */
	for(j=1;j<(n-i);j++)
           {
           /* If adjacent items are out of order, swap them */
           if( a[j-1]>a[j] ) SWAP(a[j-1],a[j]);
           }
        }
    }    
