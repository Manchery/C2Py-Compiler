#include <stdio.h>
#include <string.h>

int len;
char str[500];

int main()
{
    scanf("%s", str);
    len = strlen(str);

    int palindrome = 1;

    for (int i = 0; i < len; i = i + 1)
    {
        if (str[i] != str[len - i - 1])
        {
            palindrome = 0;
        }
    }

    if (palindrome == 1)
    {
        printf("True\n");
    }
    else
    {
        printf("False\n");
    }
}