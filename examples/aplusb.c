#include <stdio.h>

int plus(int a, int b)
{
    return a + b;
}

int minus(int a, int b)
{
    return a - b;
}

int main()
{
    int a;
    int b;
    scanf("%d %d", &a, &b);
    printf("%d %d\n", plus(a, b), minus(a, b));
}