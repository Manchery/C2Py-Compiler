import random
import sys

def gen(d=0, nonempty=1):
    if (d>=5):
        return str(random.randint(-100, 100))

    s = random.randint(0,5)

    if s==0:
        s = random.randint(0,4)
        if s == 2:
            sgn = '+'
        elif s == 3:
            sgn = '-'
        else:
            sgn = ''
        return sgn + '(' + gen(d+1, 1) + ')'
    elif s==1:
        return gen(d+1) + '+' + gen(d+1)
    elif s==2:
        return gen(d+1) + '-' + gen(d+1)
    elif s==3:
        return gen(d+1) + '*' + gen(d+1)
    elif s==4:
        return gen(d+1) + '/' + gen(d+1)
    elif s==5:
        return str(random.randint(-100, 100))

if __name__=='__main__':
    exp = gen()
    try:
        eval(exp)
        print(exp)
    except:
        sys.exit(1)