import re

__SCANF_BUFFER_2018013418__ = ""


def scanf(format: str):
    global __SCANF_BUFFER_2018013418__
    while True:
        pattern = re.compile(format.replace(
            r"%d", r"([-+]?\d+)").replace(r"%s", r"(\S+)"))
        match = pattern.search(__SCANF_BUFFER_2018013418__)
        if match:
            __SCANF_BUFFER_2018013418__ = __SCANF_BUFFER_2018013418__[match.span()[
                1]:]
            return match.groups()
        else:
            __SCANF_BUFFER_2018013418__ += input()


[a] = scanf(r"%s")
print(a)
[b] = scanf(r"%d")
print(b)
[c] = scanf(r"%d")
print(c)
