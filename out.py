from utils import *

len = None
str = [None] * 500
def main():
	global len, str
	[str] = scanf("%s")
	len = strlen(str)
	palindrome = 1
	i = 0
	while (i < len):
		if (str[i] != str[((len - i) - 1)]):
			palindrome = 0
		i = (i + 1)
	if (palindrome == 1):
		printf("True\n", ())
	else:
		printf("False\n", ())

if __name__ == '__main__':
	main()

