// C program for the above approach
#include <stdio.h>

// Function to check if a
// number is even or odd
void checkEvenOdd(int N)
{
    // Find remainder
    int r = N % 2;

    // Condition for even
    if (r == 0)
    {
        printf("Even");
    }

    // Otherwise
    else
    {
        printf("Odd");
    }
}

// Driver Code
int main()
{
    // Given number N
    int N = 101;

    // Function Call
    checkEvenOdd(N);

    return 0;
}
