#include <stdio.h>

int main(void)
{

    int length = 20; // add the correct values here
    int width = 30;

    // Here your code to calculate Perimeter and Area!
    int perimiter = (length * 2) + (width * 2);
    int area = length * width;
    printf("Area in cm^2: %d\nPerimiter in cm: %d\n", area, perimiter);

    return 0;
}