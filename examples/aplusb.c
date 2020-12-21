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
    sayHello();
    scanf("%d %d", &a, &b);
    printf("%d %d\n", plus(a, b), minus(a, b));
}