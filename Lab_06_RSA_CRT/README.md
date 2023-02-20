# 密码工程第六次实验

<p align="right">信安二班 李岱耕 202000460088</p>

## 实验要求

- 实现 RSA-CRT 算法

## 实验内容

&emsp;&emsp;RSA-CRT 算法是 RSA 算法的一种优化，其主要思想是将 RSA 算法中的求模运算分解为两个求模运算，从而减少求模运算的次数，提高运算效率。由于 RSA-CRT 算法需要用到 p 和 q 的值，因此只有解密时才能使用 RSA-CRT 算法。本实验中使用 Python 实现了 RSA-CRT 算法。

## 实验代码及注释

```python
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
```

## 实验结果

```plain
Message:         13aa7f822d4e6e95ce51f332fcde04140e45ade3d99f2add362af70941420cedee879013ba88b9eaf962940a7fb7f4763b1e998e09111b9918afa78524a1baa581298641b4ffd54a45a918aec13b7790809d68c050e3276073b02282c3b955707a94b81cff39cf292b6683dc5d7e653d1cfe784bade894f0d7f7442a74f5b1b2b844f33f50141d8049c7c80f05ec546a7801b7c4ad046f09929fb5faa1819a3751082da14b0a749728e51f3d0ff1016c7a90bd2ae5e5ce27c300c3602a24a7ceb3551641b9375836b0cfba7e9ad044ca75f5ec8c627ecd86be1835e7aaacd7d332ab95ceaa487d33c4b2074caa00acce82ad006773d43a09f176a3e19b61cf26
RSA Encrypt:     24712dc38ffbb10f1c91dab3d4ce7ea0de40d70bb2abe5ee7c0bcc30326ea8f320cf2978953ea970e12ff2058d2778a81d1023041f35135e38f87255b368992a7d834492a18c9f453cc05a5bf8b7f3e3581992d4be5ef8ff94ebd78bd1083f19e27f288f8ecc6be24d863ec3d604e81b87f9a11f186ae75d08af311e91089d07ebcbc2512add35bacdff48225f459ce407536d4aefed9d427ce0ea01faaec182a84f29ac1a9900692ea354711a37a5f92488fa0e94811ff52d0774c9061fbd0ca48d8a1d49951dd5bd9c79dd96133d6211f683e567f403a5fc27cdbbc99cf1059b9bdc0be13ba0a7282469abd87bdbd857e739382aa6ec61f2a04b633a8319ee
RSA Decrypt:     13aa7f822d4e6e95ce51f332fcde04140e45ade3d99f2add362af70941420cedee879013ba88b9eaf962940a7fb7f4763b1e998e09111b9918afa78524a1baa581298641b4ffd54a45a918aec13b7790809d68c050e3276073b02282c3b955707a94b81cff39cf292b6683dc5d7e653d1cfe784bade894f0d7f7442a74f5b1b2b844f33f50141d8049c7c80f05ec546a7801b7c4ad046f09929fb5faa1819a3751082da14b0a749728e51f3d0ff1016c7a90bd2ae5e5ce27c300c3602a24a7ceb3551641b9375836b0cfba7e9ad044ca75f5ec8c627ecd86be1835e7aaacd7d332ab95ceaa487d33c4b2074caa00acce82ad006773d43a09f176a3e19b61cf26
RSA CRT Decrypt: 13aa7f822d4e6e95ce51f332fcde04140e45ade3d99f2add362af70941420cedee879013ba88b9eaf962940a7fb7f4763b1e998e09111b9918afa78524a1baa581298641b4ffd54a45a918aec13b7790809d68c050e3276073b02282c3b955707a94b81cff39cf292b6683dc5d7e653d1cfe784bade894f0d7f7442a74f5b1b2b844f33f50141d8049c7c80f05ec546a7801b7c4ad046f09929fb5faa1819a3751082da14b0a749728e51f3d0ff1016c7a90bd2ae5e5ce27c300c3602a24a7ceb3551641b9375836b0cfba7e9ad044ca75f5ec8c627ecd86be1835e7aaacd7d332ab95ceaa487d33c4b2074caa00acce82ad006773d43a09f176a3e19b61cf26
RSA Decrypt Time:     0.041497
RSA CRT Decrypt Time: 0.012002
```

&emsp;&emsp;可以看到，使用 RSA-CRT 算法进行解密的运行时间比使用普通 RSA 算法进行解密的运行时间要短得多。
