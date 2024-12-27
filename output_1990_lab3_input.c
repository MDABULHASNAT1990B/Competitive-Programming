/ C program for the above approach
#include <stdio.h>

/ Function to check if a
/ number is even or odd
 checkevenodd( n)
{
    / Find remainder
     r = n % 2;

    / Condition for even
     (r == 0)
    {
        printf("even");
    }

    // otherwise
    else
    {
        printf("odd");
    }
}

/ Driver Code
 main()
{
    / Given number N
     n = 101;

    / Function Call
    checkevenodd(n);

     0;
}
