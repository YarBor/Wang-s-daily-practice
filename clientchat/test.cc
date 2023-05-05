#include <sys/ioctl.h>
#include <iostream>
#include <fstream>
#include <openssl/aes.h>
#include <cstring>

// 加密函数
void encrypt_file(const std::string& input_filename, const std::string& output_filename, const std::string& key) {
    // 将密钥转换为128位
    unsigned char aes_key[AES_BLOCK_SIZE];
    memset(aes_key, 0, AES_BLOCK_SIZE);
    memcpy(aes_key, key.c_str(), std::min(key.length(), static_cast<size_t>(AES_BLOCK_SIZE)));

    // 初始化加密器
    AES_KEY aes;
    if (AES_set_encrypt_key(aes_key, AES_BLOCK_SIZE * 8, &aes) != 0) {
        return;
    }

    // 打开输入文件和输出文件
    std::ifstream input(input_filename, std::ios::binary);
    std::ofstream output(output_filename, std::ios::binary);

    // 加密数据
    unsigned char input_buffer[AES_BLOCK_SIZE];
    unsigned char output_buffer[AES_BLOCK_SIZE];
    while (input.read(reinterpret_cast<char*>(input_buffer), AES_BLOCK_SIZE)) {
        AES_cbc_encrypt(input_buffer, output_buffer, AES_BLOCK_SIZE, &aes, aes_key, AES_ENCRYPT);
        output.write(reinterpret_cast<const char*>(output_buffer), AES_BLOCK_SIZE);
    }
    size_t remaining = input.gcount();
    if (remaining > 0) {
        memset(input_buffer + remaining, 0, AES_BLOCK_SIZE - remaining);
        input.read(reinterpret_cast<char*>(input_buffer), remaining);
        AES_cbc_encrypt(input_buffer, output_buffer, AES_BLOCK_SIZE, &aes, aes_key, AES_ENCRYPT);
        output.write(reinterpret_cast<const char*>(output_buffer), AES_BLOCK_SIZE);
    }

    // 关闭文件
    input.close();
    output.close();
}

// 解密函数
void decrypt_file(const std::string& input_filename, const std::string& output_filename, const std::string& key) {
    // 将密钥转换为128位
    unsigned char aes_key[AES_BLOCK_SIZE];
    memset(aes_key, 0, AES_BLOCK_SIZE);
    memcpy(aes_key, key.c_str(), std::min(key.length(), static_cast<size_t>(AES_BLOCK_SIZE)));

    // 初始化解密器
    AES_KEY aes;
    if (AES_set_decrypt_key(aes_key, AES_BLOCK_SIZE * 8, &aes) != 0) {
        return;
    }

    // 打开输入文件和输出文件
    std::ifstream input(input_filename, std::ios::binary);
    std::ofstream output(output_filename, std::ios::binary);

    // 解密数据
    unsigned char input_buffer[AES_BLOCK_SIZE];
    unsigned char output_buffer[AES_BLOCK_SIZE];
    while (input.read(reinterpret_cast<char*>(input_buffer), AES_BLOCK_SIZE)) {
        AES_cbc_encrypt(input_buffer, output_buffer, AES_BLOCK_SIZE, &aes, aes_key, AES_DECRYPT);
        output.write(reinterpret_cast<const char*>(output_buffer), AES_BLOCK_SIZE);
    }
    size_t remaining = input.gcount();
    if (remaining > 0) {
        memset(input_buffer + remaining, 0, AES_BLOCK_SIZE - remaining);
        input.read(reinterpret_cast<char*>(input_buffer), remaining);
        AES_cbc_encrypt(input_buffer, output_buffer, AES_BLOCK_SIZE, &aes, aes_key, AES_DECRYPT);
        output.write(reinterpret_cast<const char*>(output_buffer), remaining);
    }

    // 关闭文件
    input.close();
    output.close();
}

int main() {
    std::string input_filename = "test.cc";
    std::string encrypted_filename = "encrypted.bin";
    std::string decrypted_filename = "decrypted.txt";
    std::string key = "mysecretkey";

    // 加密文件
    encrypt_file(input_filename, encrypted_filename, key);

    // 解密文件
    decrypt_file(encrypted_filename, decrypted_filename, key);

    return 0;
}
