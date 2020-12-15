from utils import *

def main():
	n = 10
	x = 1
	i = 1
	while (i < n):
		x = (x * i)
		i = (i + 1)
	printf(r"%d", x)

if __name__ == '__main__':
	main()

