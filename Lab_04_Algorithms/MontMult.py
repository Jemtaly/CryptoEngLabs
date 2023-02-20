import random
def exgcd(a, b):
    if b == 0:
        return abs(a), ((a > 0) - (a < 0), 0)
    d, (x, y) = exgcd(b, a % b)
    return d, (y, x - a // b * y)
def MontRedc(X, N, k):
    M = exgcd(N, 1 << k)[1][0]
    m = X * M & (1 << k) - 1
    y = (X + m * N) >> k
    return y - N if y >= N else y
def MontMult(a, b, N, k):
    x = (a << k) % N
    y = (b << k) % N
    X = x * y % N
    return MontRedc(MontRedc(X, N, k), N, k)
x = random.randint(2 ** 2047, 2 ** 2048)
y = random.randint(2 ** 2047, 2 ** 2048)
k = 2048
N = 2 ** 2047 + 3
print("x =\n%x" % x)
print("y =\n%x" % y)
print("x * y %% N =\n%x" % ((x * y) % N))
print("MontMult(x, y, N, k) =\n%x" % MontMult(x, y, N, k))
