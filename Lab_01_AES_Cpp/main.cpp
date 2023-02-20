#include <iostream>
#include "aes.hpp"
void hexdump(uint8_t const *data, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		printf("%02x%c", data[i], (i + 1) % 16 == 0 || i + 1 == len ? '\n' : ' ');
	}
}
int main() {
    // AES examples from http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
    // PLAINTEXT:        00112233445566778899aabbccddeeff
    // AES128KEY:        000102030405060708090a0b0c0d0e0f
    // AES128CIPHERTEXT: 69c4e0d86a7b0430d8cdb78070b4c55a
    // AES192KEY:        000102030405060708090a0b0c0d0e0f1011121314151617
    // AES192CIPHERTEXT: dda97ca4864cdfe06eaf70a0ec0d7191
    // AES256KEY:        000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f
    // AES256CIPHERTEXT: 8ea2b7ca516745bfeafc49904b496089
	uint8_t buffer[16] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    };
    uint8_t aes128mk[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    };
    uint8_t aes192mk[24] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    };
    uint8_t aes256mk[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    };
    AES128 aes128(aes128mk);
    AES192 aes192(aes192mk);
    AES256 aes256(aes256mk);
    std::cout << "plain text:" << std::endl;
    hexdump(buffer, 16);
    std::cout << std::endl;
    std::cout << "AES128 key:" << std::endl;
    hexdump(aes128mk, 16);
    aes128.encrypt(buffer, buffer);
    std::cout << "AES128 cipher text:" << std::endl;
    hexdump(buffer, 16);
    aes128.decrypt(buffer, buffer);
    std::cout << "AES128 plain text:" << std::endl;
    hexdump(buffer, 16);
    std::cout << std::endl;
    std::cout << "AES192 key:" << std::endl;
    hexdump(aes192mk, 24);
    aes192.encrypt(buffer, buffer);
    std::cout << "AES192 cipher text:" << std::endl;
    hexdump(buffer, 16);
    aes192.decrypt(buffer, buffer);
    std::cout << "AES192 plain text:" << std::endl;
    hexdump(buffer, 16);
    aes256.encrypt(buffer, buffer);
    std::cout << std::endl;
    std::cout << "AES256 key:" << std::endl;
    hexdump(aes256mk, 32);
    std::cout << "AES256 cipher text:" << std::endl;
    hexdump(buffer, 16);
    aes256.decrypt(buffer, buffer);
    std::cout << "AES256 plain text:" << std::endl;
    hexdump(buffer, 16);
    return 0;
}
