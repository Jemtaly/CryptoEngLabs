# 密码工程第二次实验

<p align="right">信安二班 李岱耕 202000460088</p>

## 实验目的

- 使用 Verilog HDL 实现一种流密码算法。

## 实验内容

&emsp;&emsp;使用 Verilog 实现了 RC4 流密码算法，并使用 Verilator 进行仿真。

## 实验代码及说明

- RC4 流密码算法

```verilog
`timescale 1ns / 1ps
/* verilator lint_off WIDTH */
module RC4(
    input wire clk, // 时钟
    input wire rst, // 复位
    input wire [7:0] in, // 密钥或明文输入
    output reg [7:0] out // 密文输出
  );
  reg  mixing = 1, rec = 1;
  reg  [7:0] m [0:255]; // S 盒
  reg  [7:0] k [0:255]; // 记录输入的密钥
  reg  [7:0] x, y, i, j, n;
  wire [7:0] X, Y, a, b, t;
  integer _;
  assign t = j + m[i] + k[i % n];
  assign X = x + 1;
  assign a = m[X];
  assign Y = y + a;
  assign b = m[Y];
  always @(posedge clk) begin // 时钟上升沿进行密钥读取、密钥混淆、
    if (rst) begin // 当 rst 被置为 1 时，初始化所有寄存器并顺序读入密钥
      if (rec) begin // 在 rst 被置为 1 后的第一个时钟上升沿对各个寄存器进行初始化，rec 表示上一时钟周期 rst 的状态，若上一时钟周期 rst 为 1，则 rec 为 0，否则为 1
        for (_ = 0; _ < 256; _ = _ + 1) begin
          m[_] = _;
        end
        x <= 0;
        y <= 0;
        i <= 0;
        j <= 0;
        mixing <= 1;
        out <= 0;
        k[0] <= in; // 读入密钥第一位
        n <= 1;     // 并初始化密钥长度
      end else begin
        k[n] <= in; // 读入密钥的剩余位
        n <= n + 1; // 并记录密钥长度
      end
      rec <= 0;
    end else begin // 当 rst 被置为 0 时，进行密钥混淆和加密
      if (mixing) begin // mixing 值为 1 时进行密钥混淆
        m[i] <= m[t];
        m[t] <= m[i];
        i <= i + 1;
        j <= t;
        if (i == 255) begin
          mixing <= 0; // 将 mixing 置为 0，表示密钥混淆完成
        end
      end else begin // mixing 值为 0 时进行加（解）密
        m[X] <= b;
        m[Y] <= a;
        out <= m[a + b] ^ in;
        x <= X;
        y <= Y;
      end
      rec <= 1;
    end
  end
endmodule
```

&emsp;&emsp;当要加密时，在接下来的第一个时钟下降沿将 rst 置为 1，同时输入密钥密钥第一位，并在接下来的每个时钟下降沿依次输入密钥的其余位，当密钥的最后一位输入完成后，在下一个时钟下降沿将 rst 置为 0，等待 256 个时钟周期（密钥混淆），之后即可在每个时钟下降沿依次输入要加密的数据，每次输入的字节所对应的加密结果可在下个周期的时钟下降沿读取。

- Verilator 仿真文件

```c++
#include <iostream>
#include "Vrc4.h"
#include "verilated.h"
void hexdump(uint8_t const *data, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		printf("%02x%c", data[i], (i + 1) % 16 == 0 || i + 1 == len ? '\n' : ' ');
	}
}
void test(Vrc4 &vrc4, size_t keylen, uint8_t *key, size_t msglen, uint8_t *msg, uint8_t *buf) {
    vrc4.rst = 1; // 将 rst 置为 1，开始输入密钥
    for (int i = 0; i < keylen; i++) {
        vrc4.in = key[i]; // 输入密钥
        vrc4.clk = 0;
        vrc4.eval();
        vrc4.clk = 1;
        vrc4.eval();
    }
    vrc4.rst = 0; // 将 rst 置为 0，开始密钥混淆
    for (int i = 0; i < 256; i++) {
        vrc4.clk = 0;
        vrc4.eval();
        vrc4.clk = 1;
        vrc4.eval();
    }
    for (int i = 0; i < msglen; i++) {
        vrc4.in = msg[i]; // 输入明文
        vrc4.clk = 0;
        vrc4.eval();
        vrc4.clk = 1;
        vrc4.eval();
        buf[i] = vrc4.out; // 输出密文
    }
	std::cout << "Key: " << std::endl;
	hexdump(key, keylen);
	std::cout << "Plaintext: " << std::endl;
	hexdump(msg, msglen);
	std::cout << "Ciphertext: " << std::endl;
	hexdump(buf, msglen);
}
int main(int argc, char **argv, char **env) {
	// RC4 examples from http://en.wikipedia.org/wiki/RC4
	uint8_t key1[] = {'K', 'e', 'y'};
	uint8_t msg1[] = {'P', 'l', 'a', 'i', 'n', 't', 'e', 'x', 't'};
	uint8_t buf1[sizeof(msg1)];
	uint8_t key2[] = {'W', 'i', 'k', 'i'};
	uint8_t msg2[] = {'p', 'e', 'd', 'i', 'a'};
	uint8_t buf2[sizeof(msg2)];
	uint8_t key3[] = {'S', 'e', 'c', 'r', 'e', 't'};
	uint8_t msg3[] = {'A', 't', 't', 'a', 'c', 'k', ' ', 'a', 't', ' ', 'd', 'a', 'w', 'n'};
	uint8_t buf3[sizeof(msg3)];
	Vrc4 vrc4;
	std::cout << "-------------- RC4 test 1 ---------------" << std::endl;
	test(vrc4, sizeof(key1), key1, sizeof(msg1), msg1, buf1);
	std::cout << "-------------- RC4 test 2 ---------------" << std::endl;
	test(vrc4, sizeof(key2), key2, sizeof(msg2), msg2, buf2);
	std::cout << "-------------- RC4 test 3 ---------------" << std::endl;
	test(vrc4, sizeof(key3), key3, sizeof(msg3), msg3, buf3);
	return 0;
}
```

## 实验结果

&emsp;&emsp;使用 Verilator 对代码进行编译，命令如下：

```bash
verilator --cc --exe --build main.cpp rc4.v
```

&emsp;&emsp;编译完成后，进入 obj_dir 目录，运行 Vrc4 程序即可得到仿真结果：

```plain
-------------- RC4 test 1 ---------------
Key:
4b 65 79
Plaintext:
50 6c 61 69 6e 74 65 78 74
Ciphertext:
bb f3 16 e8 d9 40 af 0a d3
-------------- RC4 test 2 ---------------
Key:
57 69 6b 69
Plaintext:
70 65 64 69 61
Ciphertext:
10 21 bf 04 20
-------------- RC4 test 3 ---------------
Key:
53 65 63 72 65 74
Plaintext:
41 74 74 61 63 6b 20 61 74 20 64 61 77 6e
Ciphertext:
45 a0 1f 64 5f c3 5b 38 35 52 54 4b 9b f5
```

&emsp;&emsp;可以看到，仿真结果与[预期](https://en.wikipedia.org/wiki/RC4#Test_vectors)结果一致。
