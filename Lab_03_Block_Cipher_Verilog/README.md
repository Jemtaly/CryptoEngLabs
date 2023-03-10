# 密码工程第三次实验

<p align="right">信安二班 李岱耕 202000460088</p>

## 实验目的

- 使用 Verilog HDL 实现一种块密码算法。

## 实验内容

&emsp;&emsp;使用 SystemVerilog 语言实现了基于组合逻辑的 SM4 密码算法，包括密钥扩展、加密、解密三个部分。

## 实验代码及说明

- 密钥扩展

```verilog
module SM4_gen_key(
    input [7:0] sb [0:255], // S 盒
    input [31:0] ck [0:31], // CK
    input [7:0] mk [0:15], // 输入的密钥
    output [31:0] rk [0:31] // 输出的轮密钥
  );
  wire [31:0] a [0:31];
  wire [31:0] b [0:31];
  wire [31:0] t [0:35];
  assign t[0] = {mk['h0], mk['h1], mk['h2], mk['h3]} ^ 32'ha3b1bac6;
  assign t[1] = {mk['h4], mk['h5], mk['h6], mk['h7]} ^ 32'h56aa3350;
  assign t[2] = {mk['h8], mk['h9], mk['ha], mk['hb]} ^ 32'h677d9197;
  assign t[3] = {mk['hc], mk['hd], mk['he], mk['hf]} ^ 32'hb27022dc;
  genvar i;
  generate
    for (i = 0; i < 32; i = i + 1) begin
      assign a[i] = t[i + 1] ^ t[i + 2] ^ t[i + 3] ^ ck[i];
      assign b[i] = {sb[a[i][31:24]], sb[a[i][23:16]], sb[a[i][15:8]], sb[a[i][7:0]]};
      assign t[i + 4] = t[i] ^ b[i] ^ {b[i][18:0], b[i][31:19]} ^ {b[i][8:0], b[i][31:9]};
      assign rk[i] = t[i + 4];
    end
  endgenerate
endmodule
```

- 加密

```verilog
module SM4_encrypt(
    input [7:0] sb [0:255], // S 盒
    input [31:0] rk [0:31], // 轮密钥
    input [7:0] src [0:15], // 输入的明文
    output [7:0] dst [0:15] // 输出的密文
  );
  wire [31:0] a [0:31];
  wire [31:0] b [0:31];
  wire [31:0] t [0:35];
  assign t[0] = {src['h0], src['h1], src['h2], src['h3]};
  assign t[1] = {src['h4], src['h5], src['h6], src['h7]};
  assign t[2] = {src['h8], src['h9], src['ha], src['hb]};
  assign t[3] = {src['hc], src['hd], src['he], src['hf]};
  genvar i;
  generate
    for (i = 0; i < 32; i = i + 1) begin
      assign a[i] = t[i + 1] ^ t[i + 2] ^ t[i + 3] ^ rk[i];
      assign b[i] = {sb[a[i][31:24]], sb[a[i][23:16]], sb[a[i][15:8]], sb[a[i][7:0]]};
      assign t[i + 4] = t[i] ^ b[i] ^ {b[i][29:0], b[i][31:30]} ^ {b[i][21:0], b[i][31:22]} ^ {b[i][13:0], b[i][31:14]} ^ {b[i][7:0], b[i][31:8]};
    end
  endgenerate
  assign {dst['h0], dst['h1], dst['h2], dst['h3]} = t[35];
  assign {dst['h4], dst['h5], dst['h6], dst['h7]} = t[34];
  assign {dst['h8], dst['h9], dst['ha], dst['hb]} = t[33];
  assign {dst['hc], dst['hd], dst['he], dst['hf]} = t[32];
endmodule
```

- 解密

&emsp;&emsp;由于加密和解密过程的区别只在于轮密钥的使用顺序，因此无需单独实现解密模块，只需要将轮密钥的顺序翻转，然后复用加密模块即可。

- 顶层模块

```verilog
module SM4(
    input [7:0] key [0:15],
    input [7:0] src [0:15],
    output [7:0] dst [0:15],
    input mode
  );
  wire [7:0] SB [0:255];
  wire [31:0] CK [0:31];
  wire [31:0] rk [0:31];
  wire [31:0] xk [0:31];
  assign SB = {
    8'hd6, 8'h90, 8'he9, 8'hfe, 8'hcc, 8'he1, 8'h3d, 8'hb7, 8'h16, 8'hb6, 8'h14, 8'hc2, 8'h28, 8'hfb, 8'h2c, 8'h05,
    8'h2b, 8'h67, 8'h9a, 8'h76, 8'h2a, 8'hbe, 8'h04, 8'hc3, 8'haa, 8'h44, 8'h13, 8'h26, 8'h49, 8'h86, 8'h06, 8'h99,
    8'h9c, 8'h42, 8'h50, 8'hf4, 8'h91, 8'hef, 8'h98, 8'h7a, 8'h33, 8'h54, 8'h0b, 8'h43, 8'hed, 8'hcf, 8'hac, 8'h62,
    8'he4, 8'hb3, 8'h1c, 8'ha9, 8'hc9, 8'h08, 8'he8, 8'h95, 8'h80, 8'hdf, 8'h94, 8'hfa, 8'h75, 8'h8f, 8'h3f, 8'ha6,
    8'h47, 8'h07, 8'ha7, 8'hfc, 8'hf3, 8'h73, 8'h17, 8'hba, 8'h83, 8'h59, 8'h3c, 8'h19, 8'he6, 8'h85, 8'h4f, 8'ha8,
    8'h68, 8'h6b, 8'h81, 8'hb2, 8'h71, 8'h64, 8'hda, 8'h8b, 8'hf8, 8'heb, 8'h0f, 8'h4b, 8'h70, 8'h56, 8'h9d, 8'h35,
    8'h1e, 8'h24, 8'h0e, 8'h5e, 8'h63, 8'h58, 8'hd1, 8'ha2, 8'h25, 8'h22, 8'h7c, 8'h3b, 8'h01, 8'h21, 8'h78, 8'h87,
    8'hd4, 8'h00, 8'h46, 8'h57, 8'h9f, 8'hd3, 8'h27, 8'h52, 8'h4c, 8'h36, 8'h02, 8'he7, 8'ha0, 8'hc4, 8'hc8, 8'h9e,
    8'hea, 8'hbf, 8'h8a, 8'hd2, 8'h40, 8'hc7, 8'h38, 8'hb5, 8'ha3, 8'hf7, 8'hf2, 8'hce, 8'hf9, 8'h61, 8'h15, 8'ha1,
    8'he0, 8'hae, 8'h5d, 8'ha4, 8'h9b, 8'h34, 8'h1a, 8'h55, 8'had, 8'h93, 8'h32, 8'h30, 8'hf5, 8'h8c, 8'hb1, 8'he3,
    8'h1d, 8'hf6, 8'he2, 8'h2e, 8'h82, 8'h66, 8'hca, 8'h60, 8'hc0, 8'h29, 8'h23, 8'hab, 8'h0d, 8'h53, 8'h4e, 8'h6f,
    8'hd5, 8'hdb, 8'h37, 8'h45, 8'hde, 8'hfd, 8'h8e, 8'h2f, 8'h03, 8'hff, 8'h6a, 8'h72, 8'h6d, 8'h6c, 8'h5b, 8'h51,
    8'h8d, 8'h1b, 8'haf, 8'h92, 8'hbb, 8'hdd, 8'hbc, 8'h7f, 8'h11, 8'hd9, 8'h5c, 8'h41, 8'h1f, 8'h10, 8'h5a, 8'hd8,
    8'h0a, 8'hc1, 8'h31, 8'h88, 8'ha5, 8'hcd, 8'h7b, 8'hbd, 8'h2d, 8'h74, 8'hd0, 8'h12, 8'hb8, 8'he5, 8'hb4, 8'hb0,
    8'h89, 8'h69, 8'h97, 8'h4a, 8'h0c, 8'h96, 8'h77, 8'h7e, 8'h65, 8'hb9, 8'hf1, 8'h09, 8'hc5, 8'h6e, 8'hc6, 8'h84,
    8'h18, 8'hf0, 8'h7d, 8'hec, 8'h3a, 8'hdc, 8'h4d, 8'h20, 8'h79, 8'hee, 8'h5f, 8'h3e, 8'hd7, 8'hcb, 8'h39, 8'h48
  };
  assign CK = {
    32'h00070e15, 32'h1c232a31, 32'h383f464d, 32'h545b6269,
    32'h70777e85, 32'h8c939aa1, 32'ha8afb6bd, 32'hc4cbd2d9,
    32'he0e7eef5, 32'hfc030a11, 32'h181f262d, 32'h343b4249,
    32'h50575e65, 32'h6c737a81, 32'h888f969d, 32'ha4abb2b9,
    32'hc0c7ced5, 32'hdce3eaf1, 32'hf8ff060d, 32'h141b2229,
    32'h30373e45, 32'h4c535a61, 32'h686f767d, 32'h848b9299,
    32'ha0a7aeb5, 32'hbcc3cad1, 32'hd8dfe6ed, 32'hf4fb0209,
    32'h10171e25, 32'h2c333a41, 32'h484f565d, 32'h646b7279
  };
  genvar i;
  generate
    for (i = 0; i < 32; i = i + 1) begin
      assign xk[i] = mode ? rk[i] : rk[31 - i]; // 如果是解密模式，则将轮密钥的顺序翻转
    end
  endgenerate
  SM4_gen_key sm4_gnk(.SB(SB), .CK(CK), .key(key), .rk(rk));
  SM4_encrypt sm4_xxc(.SB(SB), .rk(xk), .src(src), .dst(dst));
endmodule
```

- Verilator 测试文件

```c++
#include <iostream>
#include "Vsm4.h"
#include "verilated.h"
void hexdump(uint8_t const *data, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		printf("%02x%c", data[i], (i + 1) % 16 == 0 || i + 1 == len ? '\n' : ' ');
	}
}
int main(int argc, char **argv, char **env) {
	// 示例：
	// 密钥：01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
	// 明文：01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
	// 密文：68 1e df 34 d2 06 96 5e 86 b3 e9 4f 53 6e 42 46
	uint8_t key[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
	uint8_t buf[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
	Vsm4 vsm4;
	for (int i = 0; i < 16; i++) {
		vsm4.mk[i] = key[i]; // 输入密钥
	}
	std::cout << "Key: " << std::endl;
	hexdump(key, 16);
	// 加密测试
	std::cout << "--------------- SM4 Enc Test -------------" << std::endl;
	std::cout << "Plaintext: " << std::endl;
	hexdump(buf, 16);
	for (int i = 0; i < 16; i++) {
		vsm4.src[i] = buf[i]; // 输入明文
	}
	vsm4.mode = 1; // 加密模式
	vsm4.eval(); // 执行
	for (int i = 0; i < 16; i++) {
		buf[i] = vsm4.dst[i]; // 输出密文
	}
	std::cout << "Ciphertext: " << std::endl;
	hexdump(buf, 16);
	// 解密测试
	std::cout << "--------------- SM4 Dec Test -------------" << std::endl;
	std::cout << "Ciphertext: " << std::endl;
	hexdump(buf, 16);
	for (int i = 0; i < 16; i++) {
		vsm4.src[i] = buf[i]; // 输入密文
	}
	vsm4.mode = 0; // 解密模式
	vsm4.eval(); // 执行
	for (int i = 0; i < 16; i++) {
		buf[i] = vsm4.dst[i]; // 输出明文
	}
	std::cout << "Plaintext: " << std::endl;
	hexdump(buf, 16);
	return 0;
}
```

## 实验结果

&emsp;&emsp;使用 Verilator 对代码进行编译，命令如下：

```bash
verilator --cc --exe --build main.cpp sm4.v
```

*&emsp;&emsp;注：Verilator 版本需为 5.0.0 及以上。*

&emsp;&emsp;编译完成后，进入 obj_dir 目录，运行 Vsm4 程序即可得到仿真结果：

```plain
Key:
01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
--------------- SM4 Enc Test -------------
Plaintext:
01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
Ciphertext:
68 1e df 34 d2 06 96 5e 86 b3 e9 4f 53 6e 42 46
--------------- SM4 Dec Test -------------
Ciphertext:
68 1e df 34 d2 06 96 5e 86 b3 e9 4f 53 6e 42 46
Plaintext:
01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
```

&emsp;&emsp;输出结果与预期结果一致，说明实现正确。
