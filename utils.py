#!/usr/bin/python
# -*- coding: UTF-8 -*-

import re

__SCANF_BUFFER_2018013418__ = ""


# [re — Regular expression operations — Python 3.9.1 documentation](https://docs.python.org/3/library/re.html#simulating-scanf)
def scanf(format: str):
    global __SCANF_BUFFER_2018013418__
    while True:
        specifiers = format.strip().split()
        pattern = r"\s*".join(specifiers)
        # TODO: more specifier, e.g. %f %c
        pattern = re.compile(pattern.replace(
            r"%d", r"([-+]?\d+)").replace(r"%s", r"(\S+)"))
        match = pattern.search(__SCANF_BUFFER_2018013418__)
        if match:
            __SCANF_BUFFER_2018013418__ = __SCANF_BUFFER_2018013418__[match.span()[
                1]:]
            targets = []
            for s, g in zip(specifiers, match.groups()):
                if s == r"%d":
                    targets.append(eval(g))
                elif s == r"%s":
                    charList = [g[i] for i in range(len(g))]
                    targets.append(charList)
                else:
                    targets.append(g)
            return targets
        else:
            __SCANF_BUFFER_2018013418__ += input()


def printf(format: str, values: list):
    print(format % values, end='')


def atoi(chars: list):
    # print('what', chars)
    sgn = 1
    num = 0
    if chars[0] == '-':
        chars = chars[1:]
        sgn = -1
    for x in chars:
        if x == 0:
            break
        num = num * 10 + ord(x) - ord('0')
    return num * sgn

def strlen(string: list):
    return len(list(filter(None, string)))


# [a, b] = scanf("%s %d")
# [c, d] = scanf("%d %d")

# printf("a=%s, b=%d, c=%d, d=%d", (a, b, c, d))

# print(atoi(['-', '1', '2']))
# print(atoi(['9', '0', '1']))
