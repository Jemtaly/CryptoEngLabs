import random
def BarrRedc(a, k, N):
    m = (1 << k) // N
    q = a * m >> k
    a = a - q * N
    if N <= a:
        a = a - N
    return a
k = 2048
a = random.randint(2 ** 2048, 2 ** 2049)
N = random.randint(2 ** 2047, 2 ** 2048)
print("a =\n%x" % a)
print("N =\n%x" % N)
print("a %% N =\n%x" % (a % N))
print("BarrRedc(a, k, N) =\n%x" % BarrRedc(a, k, N))
