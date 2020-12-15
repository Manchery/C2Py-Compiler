def do_math(a):
	x = (a + (5 * 3))
	x = (x + 1)
	x = ((1 + 1) - (1 * 4))
	if (((x == 2) and (x != 2)) and (x < 2)):
		x = (x + 1)
	else:
		if ((x == (2 + 3)) or (a == 2)):
			pass
		else:
			pass
	if ((x == (2 + 3)) or (a == 2)):
		pass
	b = None
	c = 0
	i = 0
	while (i < 10):
		c = (c + i)
		i = (i + 1)
	while (c > 0):
		c = (c - 1)
		while (a != 0):
			a = (a + b)
		if (a != b):
			d = None

def main():
	a = 3
	do_math(a)

if __name__ == '__main__':
	main()

