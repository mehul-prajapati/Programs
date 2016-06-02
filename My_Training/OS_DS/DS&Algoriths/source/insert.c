
/* Insertion sort for integers */

void insertion( int a[], int n )
/* Pre-condition: a contains n items to be sorted */
    {
    int i, j, v;
    /* Initially, the first item is considered 'sorted' */
    /* i divides a into a sorted region, x<i, and an
       unsorted one, x >= i */
    for(i=1;i<n;i++)
        {
        /* Select the item at the beginning of the
           as yet unsorted section */
        v = a[i];
        /* Work backwards through the array, finding where v 
           should go */
        j = i;
        while( j > 0 )
           {
           /* If this element is greater than v,
              move it up one */
           while ( a[j-1] > v )
               {
               a[j] = a[j-1]; j = j-1;
               }
           /* Stopped when a[j-1] <= v, so put v at position j */
           a[j] = v;
           }
        }
    }    
