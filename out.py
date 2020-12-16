from utils import *

def plus(a, b):
	return (a + b)

def minus(a, b):
	return (a - b)

def main():
	a = None
	b = None
	[a, b] = scanf("%d %d")
	printf("%d %d\n", (plus(a, b), minus(a, b)))

if __name__ == '__main__':
	main()

