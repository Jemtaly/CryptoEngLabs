import sys, random, time
sys.setrecursionlimit(0x10000)
def exgcd(a, b): # 扩展欧几里得算法，本实验中用于求逆元
    if b == 0:
        return abs(a), ((a > 0) - (a < 0), 0)
    d, (x, y) = exgcd(b, a % b)
    return d, (y, x - a // b * y)
def power(b, p, m): # 快速幂取模
    if p == 0:
        return 1
    t = power(b, p >> 1, m)
    return (t * t * b if p & 1 else t * t) % m
def isprime(n): # Miller-Rabin 素性测试
    if n == 2:
        return True
    if n < 2 or n & 1 == 0:
        return False
    for _ in range(16):
        a = random.randrange(1, n)
        d = n - 1
        while d & 1 == 0:
            t = power(a, d, n)
            if t == n - 1:
                break
            if t != 1:
                return False
            d >>= 1
    return True
def randprime(l): # 生成 l 位素数
    while True:
        r = random.getrandbits(l)
        if isprime(r):
            return r
def RSAGenKey(l): # 生成 RSA 密钥对
    p, q = randprime(l), randprime(l)
    phi = (p - 1) * (q - 1)
    while True:
        e = random.randrange(0, phi)
        gcd, (r, _) = exgcd(e, phi)
        if gcd == 1:
            d = r % phi
            break
    return (p, q, e, d)
def RSAEncrpyt(n, e, m): # RSA 加密
    return power(m, e, n)
def RSADecrypt(n, d, c): # RSA 解密
    return power(c, d, n)
def RSACRTDecrypt(p, q, d, c): # RSA-CRT 解密
    cp = c % p
    cq = c % q
    dp = d % (p - 1)
    dq = d % (q - 1)
    mp = power(cp, dp, p)
    mq = power(cq, dq, q)
    _, (u, v) = exgcd(p, q)
    return (mp * v * q + mq * u * p) % (p * q)
def test(f, *args): # 测试函数 f 的运行时间
    start = time.time()
    result = f(*args)
    end = time.time()
    return result, end - start
def main():
    p, q, e, d = RSAGenKey(1024) # 生成 RSA 密钥对
    N = p * q # 计算 N
    m = random.randrange(0, N) # 生成随机消息
    c = RSAEncrpyt(N, e, m) # 加密
    d1, t1 = test(RSADecrypt, N, d, c) # 使用普通 RSA 算法进行解密并计算运行时间
    d2, t2 = test(RSACRTDecrypt, p, q, d, c) # 使用 RSA-CRT 算法进行解密并计算运行时间
    print('Message:         %x' % m)
    print('RSA Encrypt:     %x' % c)
    print('RSA Decrypt:     %x' % d1)
    print('RSA CRT Decrypt: %x' % d2)
    print('RSA Decrypt Time:     %f' % t1)
    print('RSA CRT Decrypt Time: %f' % t2)
if __name__ == '__main__':
    main()
