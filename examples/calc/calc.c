#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int len;
char expr[500];

char opStack[500];
int sgnStack[500];
int opTop = 0;
double numStack[500];
int numTop = 0;

int sign = 0;

int isOp(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

int opPrior(char c)
{
    if (c == '+' || c == '-')
    {
        return 1;
    }
    if (c == '*' || c == '/')
    {
        return 2;
    }
    if (c == '(' || c == ')')
    {
        return 0;
    }
}

double calc(double a, double b, char op)
{
    if (op == '+')
    {
        return a + b;
    }
    if (op == '-')
    {
        return a - b;
    }
    if (op == '*')
    {
        return a * b;
    }
    if (op == '/')
    {
        return a / b;
    }
}

int main()
{
    scanf("%s", expr);
    len = strlen(expr);

    int pointer = 0;
    double num = 0.0;

    char numBuffer[500];
    int bufPtr = 0;

    for (int i = 0; i < len; i = i + 1)
    {
        if (isOp(expr[i]) == 1)
        {
            if ((expr[i] == '+' || expr[i] == '-') && (i == 0 || expr[i - 1] == '(' || expr[i - 1] == '+' || expr[i - 1] == '-' || expr[i - 1] == '*' || expr[i - 1] == '/'))
            {
                if (expr[i] == '+')
                {
                    sign = 1;
                }
                else
                {
                    sign = -1;
                }
            }
            else
            {
                if (bufPtr > 0)
                {
                    numBuffer[bufPtr] = 0;
                    bufPtr = 0;

                    numStack[numTop] = atoi(numBuffer);
                    if (sign != 0)
                    {
                        numStack[numTop] = numStack[numTop] * sign;
                        sign = 0;
                    }
                    numTop = numTop + 1;
                }

                if (expr[i] != '(')
                {
                    while (opTop > 0 && opStack[opTop - 1] != '(' && opPrior(expr[i]) <= opPrior(opStack[opTop - 1]))
                    {
                        double tmp = calc(numStack[numTop - 2], numStack[numTop - 1], opStack[opTop - 1]);
                        numStack[numTop - 2] = tmp;
                        numTop = numTop - 1;
                        opTop = opTop - 1;
                    }
                }

                if (expr[i] == ')' && opStack[opTop - 1] == '(')
                {
                    if (sgnStack[opTop - 1] != 0)
                    {
                        numStack[numTop - 1] = numStack[numTop - 1] * sgnStack[opTop - 1];
                        sgnStack[opTop - 1] = 0;
                    }
                    opTop = opTop - 1;
                }
                else
                {
                    opStack[opTop] = expr[i];
                    if (expr[i] == '(')
                    {
                        sgnStack[opTop] = sign;
                        sign = 0;
                    }
                    opTop = opTop + 1;
                }
            }
        }
        else
        {
            numBuffer[bufPtr] = expr[i];
            bufPtr = bufPtr + 1;
        }
    }

    if (bufPtr > 0)
    {
        numBuffer[bufPtr] = 0;
        bufPtr = 0;

        numStack[numTop] = atoi(numBuffer);
        if (sign != 0)
        {
            numStack[numTop] = numStack[numTop] * sign;
            sign = 0;
        }
        numTop = numTop + 1;
    }

    while (opTop > 0)
    {
        double tmp = calc(numStack[numTop - 2], numStack[numTop - 1], opStack[opTop - 1]);
        numStack[numTop - 2] = tmp;
        numTop = numTop - 1;
        opTop = opTop - 1;
    }

    printf("%f\n", numStack[0]);
}