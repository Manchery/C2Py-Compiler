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
                else:
                    targets.append(g)
            return targets
        else:
            __SCANF_BUFFER_2018013418__ += input()


def printf(format: str, values: list):
    print(format % values)


[a, b] = scanf(r"%s %d")
[c, d] = scanf(r"%d %d")

printf(r"a=%s, b=%d, c=%d, d=%d", (a, b, c, d))
