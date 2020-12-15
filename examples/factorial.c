#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n = 10;
    int x = 1;
    for (int i = 1; i < n; i=i+1)
    {
        x = x * i;
    }
    printf("%d", x);
}