# 密码工程第四次实验

<p align="right">信安二班 李岱耕 202000460088</p>

## 实验要求

- 实现 Montgomery Multiplication 算法

- 实现 Barrett Reduction 算法

## 主要代码及说明

- Montgomery Reduction

```py
def MontRedc(X, N, k):
    M = exgcd(N, 1 << k)[1][0]
    m = X * M & (1 << k) - 1
    y = (X + m * N) >> k
    return y - N if y >= N else y
```

- Montgomery Multiplication

```python
def MontMult(a, b, N, k):
    x = (a << k) % N
    y = (b << k) % N
    X = x * y % N
    return MontRedc(MontRedc(X, N, k), N, k)
```

- Barrett Reduction

```py
def BarrRedc(a, k, N):
    m = (1 << k) // N
    q = a * m >> k
    a = a - q * N
    if N <= a:
        a = a - N
    return a
```

## 完整测试代码

- MontMult.py

```py
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
```

- BarrRedc.py

```py
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
```

## 测试结果

&emsp;&emsp;运行 MontMult.py，结果如下：

```plain
x =
c8dac421f30281f37795c028e95e849dd8790945686fa5f48a099dc380085c761ded226b158e533fcae88fc6752c8bac59b2ed767b165e629dc302588d13cfd63c072e746e9dedb6c5f2ad1e5f8284380c506c27c5ee41e963adc597049861fbb9af610a5a68f3038e4416a569f01884f93e68a53c673d1003c69701b70ba58ea1d223bc9cdbec71d4bc17c1f2baede7ee7797db26885444347acf608dfdff7d6f5f554a66c304a7deb5d584a90b43211942c3c8852911cb700ca3fb45c74ed417f335a160136ab170a5939f0d6004df72f0078446952e73e9e8d2b0dd1b4627541d78ad4e3ffd61ddb4e42f72e239c4c3072be8a2f8c9ee09e8021b5ea0bdc7
y =
8651ad1bfc1fbeaed33b32516ef7d8b38d49b7cc05fdbbe537eecfa667d174c08adb72a96346cdb6c9ce35f7e33b886a5f2aa55b57e0cadb198e618148e14563024015ec3f96f686fa462f510183eb9b4571b4749998010a1465253ea8409e45f47325b51068589c69002aa53f66661a0eb83756e43e812a7386c511d660969f3ab98b326cb05b2ce7f1f8247d567be12d4d5a270144a358f19ee1fa887ef58ced7a1e8c3adc7734c13c05fb0689869986a7369b8a0ce8fb959790f1f3d4bdd70197b9e85473a85887ac9d8da8043731dcd68da8e262eb3ac914f3f6f975b657f811a84a6a58e53bd2ca2fd647ccd42cedd9ced199686cf64b22ad0340b5a5c4
x * y % N =
6e29bafb1500b64947235e64eeb7d41724ea8eca257bcc9956cbd8b037c4774f9ad9b5d81ec9c8b223ecf6e525c92b73bc5f41899312408178b1d7ba741642976f26c51f1b77e9442c82f5f05fba3ed3c3c767d84f12ce6dc967a3d9f9e5c43688ae2dcffb555a8ff20239900297c745d7c964b342e289cb71b8b4c60dc0933442f23625f90bb5b71cae6891707cc1e65de8f3104d077a6ad0bc0507c8a86a8795a2fa919cda1db386821d92c92be0d7011f10deeb4e92aca9483a0f42c6e002ccba4a27d19bdfe8fb288bfef219b8263965770fa3c282d6f13bb8a4eaff24f95d164567e953d4897f809693ab9b67206d3d2da9afc4f682fd226c4771ad296f
MontMult(x, y, N, k) =
6e29bafb1500b64947235e64eeb7d41724ea8eca257bcc9956cbd8b037c4774f9ad9b5d81ec9c8b223ecf6e525c92b73bc5f41899312408178b1d7ba741642976f26c51f1b77e9442c82f5f05fba3ed3c3c767d84f12ce6dc967a3d9f9e5c43688ae2dcffb555a8ff20239900297c745d7c964b342e289cb71b8b4c60dc0933442f23625f90bb5b71cae6891707cc1e65de8f3104d077a6ad0bc0507c8a86a8795a2fa919cda1db386821d92c92be0d7011f10deeb4e92aca9483a0f42c6e002ccba4a27d19bdfe8fb288bfef219b8263965770fa3c282d6f13bb8a4eaff24f95d164567e953d4897f809693ab9b67206d3d2da9afc4f682fd226c4771ad296f
```

&emsp;&emsp;可见 `x * y % N` 的值和 `MontMult(x, y, N, k)` 相同。

&emsp;&emsp;运行 BarrRedc.py，结果如下：

```plain
a =
1c6f69044b8dc050a69e0a3ab2353deee517d03d816a9c37a083cf3c202e8638ddca258b920a1fe25f70de2dbbcd789f90a3ff8de9ead4d4fa3bbf4a9d69e3d0d2e5d9c891b2b505cb9766b3e578f279db6b6060b9360572e4911eac3d280f678d8ee5390f36b8f67d0ec0012da9c8cbc5bdd8badbeba692dc4bfc16f6f47b27b277ef03def40cca4b8688a52a1604d3fec02896c9c38935c884abba9826b450ea2d465ab54d5335bf4d0e16e98fe2a328fb597044f346d2b1f46cd38f748b0240c2853f6a496fbdb93a34362f36bbbe4f2e7d833414cfa441cd05c3de88025dd6c07f792380cade6fb999b94aed1a7fafa76085112ac933b5f849871a227c77a
N =
998c546bee95e81225e2e453bd1bf321914858c768928e34b010561e1f0972b212b8f8132d1b5dbd652bd9cbc5262b3bd33731c0ec6ee4c5acf66f4e37e5a548d4bc821d7b50712823adaead24fee95a3cc0602ef6df72700ec2b788ac1e585c02b62d58f39865d9574445274332d7f2d6d3e95a2375e3f36f142d99f3daf08073f228fdb6d8256e42c2d01fb57be511873e06b1b812a11a793b706630a7d688b17ae6f893fff11cc5c0578ee2d8c9bbe7e066410e8994b8ef05931d0d3c30cbaf7bda0928bb50486362482e94b17b07880852c30910a79bac52f81f75ccdbd730a1f89735a14e181f36d14e089f9151826b5c0e349fb0b31208b39dac2a1f6e
a % N =
93dde76cdbb034e61e1adb03a91bf8ab2eec52494584a710a81c4785c4d57e29b7306892c66b42ab2cb62f44328b338163d1955cc5cf83c449cf160d66d2f27b84e4984e248a6e0c721b0de40d9154e93d3545ada5a1724e2b8c7bb27a4445c0d381f8df0c3ac3b5226375c45436dcd6ae35b8f977cea146e697663b8791d17a3f9a9e42819081c832e2ea133668831cdd867c092c13512795d3dadd211b97fd3fde97ba2cd5512269503250d34c96babff4ca82322143b9413ba6fedcd04e8cad309fe453205b4accdeb305ca08c5d5e2d732ad2f2bab0cc42a6bfefce66e2f0ac40663ccca11b6bd2bf8f89d928557f59f5034a96d31d53b73313649d3889e
BarrRedc(a, k, N) =
93dde76cdbb034e61e1adb03a91bf8ab2eec52494584a710a81c4785c4d57e29b7306892c66b42ab2cb62f44328b338163d1955cc5cf83c449cf160d66d2f27b84e4984e248a6e0c721b0de40d9154e93d3545ada5a1724e2b8c7bb27a4445c0d381f8df0c3ac3b5226375c45436dcd6ae35b8f977cea146e697663b8791d17a3f9a9e42819081c832e2ea133668831cdd867c092c13512795d3dadd211b97fd3fde97ba2cd5512269503250d34c96babff4ca82322143b9413ba6fedcd04e8cad309fe453205b4accdeb305ca08c5d5e2d732ad2f2bab0cc42a6bfefce66e2f0ac40663ccca11b6bd2bf8f89d928557f59f5034a96d31d53b73313649d3889e
```

&emsp;&emsp;可见 `a % N` 的值和 `BarrRedc(a, k, N)` 相同。
