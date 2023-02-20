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
        vrc4.in = key[i];
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
