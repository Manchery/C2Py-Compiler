from utils import *

a = None
def main():
	global len
	global expr
	global opStack
	global sgnStack
	global opTop
	global numStack
	global numTop
	global sign
	[expr] = scanf("%s")
	len = strlen(expr)
	pointer = 0
	num = 0
	numBuffer = [None] * 500
	bufPtr = 0
	i = 0
	while (i < len):
		if (isOp(expr[i]) == 1):
			if (((expr[i] == '+') or (expr[i] == '-')) and ((((i == 0) or (expr[(i - 1)] == '(')) or (expr[(i - 1)] == '*')) or (expr[(i - 1)] == '/'))):
				if (expr[i] == '+'):
					sign = 1
				else:
					sign = -1
			else:
				if (bufPtr > 0):
					numBuffer[bufPtr] = 0
					bufPtr = 0
					numStack[numTop] = atoi(numBuffer)
					if (sign != 0):
						numStack[numTop] = (numStack[numTop] * sign)
						sign = 0
					numTop = (numTop + 1)
				if (expr[i] != '('):
					while (((opTop > 0) and (opStack[(opTop - 1)] != '(')) and (opPrior(expr[i]) <= opPrior(opStack[(opTop - 1)]))):
						tmp = calc(numStack[(numTop - 2)], numStack[(numTop - 1)], opStack[(opTop - 1)])
						numStack[(numTop - 2)] = tmp
						numTop = (numTop - 1)
						opTop = (opTop - 1)
				if ((expr[i] == ')') and (opStack[(opTop - 1)] == '(')):
					if (sgnStack[(opTop - 1)] != 0):
						numStack[(numTop - 1)] = (numStack[(numTop - 1)] * sgnStack[(opTop - 1)])
						sgnStack[(opTop - 1)] = 0
					opTop = (opTop - 1)
				else:
					opStack[opTop] = expr[i]
					if (expr[i] == '('):
						sgnStack[opTop] = sign
						sign = 0
					opTop = (opTop + 1)
		else:
			numBuffer[bufPtr] = expr[i]
			bufPtr = (bufPtr + 1)
		i = (i + 1)
	if (bufPtr > 0):
		numBuffer[bufPtr] = 0
		bufPtr = 0
		numStack[numTop] = atoi(numBuffer)
		if (sign != 0):
			numStack[numTop] = (numStack[numTop] * sign)
			sign = 0
		numTop = (numTop + 1)
	while (opTop > 0):
		tmp = calc(numStack[(numTop - 2)], numStack[(numTop - 1)], opStack[(opTop - 1)])
		numStack[(numTop - 2)] = tmp
		numTop = (numTop - 1)
		opTop = (opTop - 1)
	printf("%f\n", (numStack[0]))

if __name__ == '__main__':
	main()

