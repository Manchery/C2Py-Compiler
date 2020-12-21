#include <stdio.h>

int plus(int a, int b)
{
    return a + b;
}

int minus(int a, int b)
{
    return a - b;
}

void sayHello()
{
    printf("Hello world!\n");
    return;
}

int main()
{
    int a;
    int b;
    int arr[4 + 5 - 2];
    a = arr[3] + arr[2];
    sayHello();
    scanf("%d %d", &a, &b);
    printf("%d %d\n", plus(a, b), minus(a, b));
}