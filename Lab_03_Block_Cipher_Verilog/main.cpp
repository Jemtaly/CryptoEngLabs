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
