#include <stdio.h>

int main()
{

    double pi = 3.141593;                                  // function for π
    int deg[10] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90}; // table with all degrees from 0 to 90 in steps of 10
    double rad;                                            // temp = temporary storage, rad = resulting storage for radians
    int x;
    x = 0;

    printf("Degree\tRadian\n");
    while (x < 10)
    {
        rad = ((deg[x] * pi) / 180) / pi;
        printf("%d\t", deg[x]);
        printf("%.2lf", rad);
        printf("π\n");
        x = x + 1;
    }
    printf("\a");
    return 0;
}

// Degree to radian table from Chapter 1.4