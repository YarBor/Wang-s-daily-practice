// #include <sys/ioctl.h>
// #include <iostream>
// #include <fstream>
// #include <openssl/aes.h>
// #include <cstring>

// // 加密函数
// void encrypt_file(const std::string& input_filename, const std::string& output_filename, const std::string& key) {
//     // 将密钥转换为128位
//     unsigned char aes_key[AES_BLOCK_SIZE];
//     memset(aes_key, 0, AES_BLOCK_SIZE);
//     memcpy(aes_key, key.c_str(), std::min(key.length(), static_cast<size_t>(AES_BLOCK_SIZE)));

//     // 初始化加密器
//     AES_KEY aes;
//     if (AES_set_encrypt_key(aes_key, AES_BLOCK_SIZE * 8, &aes) != 0) {
//         return;
//     }

//     // 打开输入文件和输出文件
//     std::ifstream input(input_filename, std::ios::binary);
//     std::ofstream output(output_filename, std::ios::binary);

//     // 加密数据
//     unsigned char input_buffer[AES_BLOCK_SIZE];
//     unsigned char output_buffer[AES_BLOCK_SIZE];
//     while (input.read(reinterpret_cast<char*>(input_buffer), AES_BLOCK_SIZE)) {
//         AES_cbc_encrypt(input_buffer, output_buffer, AES_BLOCK_SIZE, &aes, aes_key, AES_ENCRYPT);
//         output.write(reinterpret_cast<const char*>(output_buffer), AES_BLOCK_SIZE);
//     }
//     size_t remaining = input.gcount();
//     if (remaining > 0) {
//         memset(input_buffer + remaining, 0, AES_BLOCK_SIZE - remaining);
//         input.read(reinterpret_cast<char*>(input_buffer), remaining);
//         AES_cbc_encrypt(input_buffer, output_buffer, AES_BLOCK_SIZE, &aes, aes_key, AES_ENCRYPT);
//         output.write(reinterpret_cast<const char*>(output_buffer), AES_BLOCK_SIZE);
//     }

//     // 关闭文件
//     input.close();
//     output.close();
// }

// // 解密函数
// void decrypt_file(const std::string& input_filename, const std::string& output_filename, const std::string& key) {
//     // 将密钥转换为128位
//     unsigned char aes_key[AES_BLOCK_SIZE];
//     memset(aes_key, 0, AES_BLOCK_SIZE);
//     memcpy(aes_key, key.c_str(), std::min(key.length(), static_cast<size_t>(AES_BLOCK_SIZE)));

//     // 初始化解密器
//     AES_KEY aes;
//     if (AES_set_decrypt_key(aes_key, AES_BLOCK_SIZE * 8, &aes) != 0) {
//         return;
//     }

//     // 打开输入文件和输出文件
//     std::ifstream input(input_filename, std::ios::binary);
//     std::ofstream output(output_filename, std::ios::binary);

//     // 解密数据
//     unsigned char input_buffer[AES_BLOCK_SIZE];
//     unsigned char output_buffer[AES_BLOCK_SIZE];
//     while (input.read(reinterpret_cast<char*>(input_buffer), AES_BLOCK_SIZE)) {
//         AES_cbc_encrypt(input_buffer, output_buffer, AES_BLOCK_SIZE, &aes, aes_key, AES_DECRYPT);
//         output.write(reinterpret_cast<const char*>(output_buffer), AES_BLOCK_SIZE);
//     }
//     size_t remaining = input.gcount();
//     if (remaining > 0) {
//         memset(input_buffer + remaining, 0, AES_BLOCK_SIZE - remaining);
//         input.read(reinterpret_cast<char*>(input_buffer), remaining);
//         AES_cbc_encrypt(input_buffer, output_buffer, AES_BLOCK_SIZE, &aes, aes_key, AES_DECRYPT);
//         output.write(reinterpret_cast<const char*>(output_buffer), remaining);
//     }

//     // 关闭文件
//     input.close();
//     output.close();
// }

#include <string>
#include <iostream>
#include <assert.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <iostream>
#include <net/if.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

std::string get_local_ipv4() {
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    std::string ipv4;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
            continue;

        int s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                            host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if (s != 0) {
            perror("getnameinfo");
            exit(EXIT_FAILURE);
        }

        if (strcmp(ifa->ifa_name, "lo") != 0) {
            ipv4 = std::string(host);
            break;
        }
    }

    freeifaddrs(ifaddr);
    return ipv4;
}


int main () {
    std::cout << get_local_ipv4() << std:: endl;
}