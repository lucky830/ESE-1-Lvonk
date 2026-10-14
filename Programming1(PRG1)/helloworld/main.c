#include <stdio.h>

int main()
{

    double pi = 3.141593;                                     // function for π
    double deg[10] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90}; // table with all degrees from 0 to 90 in steps of 10
    double rad;                                         // temp = temporary storage, rad = resulting storage for radians
//    double temp;
    int x;
    x = 0;


    while (x < 10)
    {
        rad = (deg[x] * pi) / 180;
        printf("%f\n", rad);
        x = x + 1;
    }

    return 0;
}
