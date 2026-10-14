#include <stdio.h>

int main()
{

    int C = 0;     // temp in celcius
    float F = 0;   // temp in fahrenheit converted from celsius
    float F2C = 0; // temp conversion reverted, now F -> C

    printf("C°\tF°\tF° to C°\n");

    C = 0;
    while (C < 30)
    {
        F = (C * 1.8) + 32;
        F2C = (F - 32) / 1.8;
        printf("%d\t%.1lf\t%.1lf\n", C, F, F2C);
        C = C + 5;
    }
    return 0;
}

// Celsius to Fahrenheit conversion and back from Chapter 1.7