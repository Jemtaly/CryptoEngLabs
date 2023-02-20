# 密码工程第五次实验

<p align="right">信安二班 李岱耕 202000460088</p>

## 实验要求

- 回答：密码法哪一条款规定了，关键基础设施需要使用密码保护？

- 通过侧信道攻击，完成简单的密钥恢复。

## 第一题

&emsp;&emsp;中华人民共和国密码法（2020 年 1 月 1 日起实施）第二十七条：法律、行政法规和国家有关规定要求使用商用密码进行保护的，其运营者应当使用商用密码进行保护。

## 实验代码及说明

&emsp;&emsp;使用 Python 实现汉明重量计算函数如下：

```python
def H(x):
    r = 0
    while x:
        r += 1
        x &= x - 1
    return r
```

&emsp;&emsp;根据定义实现相关性计算函数如下：

```python
def correlation(A, B):
    x = sum(A) / len(A)
    y = sum(B) / len(B)
    return sum((a - x) * (b - y) for a, b in zip(A, B)) / (sum((a - x) ** 2 for a in A) * sum((b - y) ** 2 for b in B)) ** 0.5
```

&emsp;&emsp;然后即可实现侧信道攻击，代码如下：

```python
input = [0, 1, 2, 3]
power = [2, 0, 1, 1]
S_box = [1, 2, 3, 0]
corrs = [correlation([H(S_box[i ^ key]) for i in input], power) for key in range(4)]
print("Correlation: ", corrs)
print("Correct Key(s): ", [i for i, x in enumerate(corrs) if abs(x) == max(abs(x) for x in corrs)])
```

## 实验结果

执行结果如下：

```plain
Correlation:  [0.0, 0.0, 1.0, -1.0]
Correct Key(s):  [2, 3]
```

根据结果可以得出，正确的密钥为 2 或 3。
