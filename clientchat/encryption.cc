#include <iostream>
#include <regex>
#include <fstream>
#include "client.hh"
int main()
{

    // FILE * fd = fopen("example.txt", "r"); // 打开一个文件并返回其文件描述符
    // std::fstream message_file; // 使用文件描述符fd创建一个 fstream 对象
    // message_file.attach(fd);
    // message_file.
    // std::string message;
    std::stringstream sstream;
    int f = open("test.txt", O_RDONLY);
    char buffer[512];
    size_t len = 0;
    while ((len = read(f, buffer, sizeof(buffer))) > 0)
    {
        sstream.write(buffer, len);
    }

    std::string message;
    while (getline(sstream, message))
    {
        std::regex pattern("\\[(.*?)\\](.*?): (.*)");
        std::smatch match;

        if (std::regex_match(message, match, pattern))
        {
            std::string timestamp = match[1];
            std::string sender = match[2];
            std::string content = match[3];

            std::cout << "Timestamp: " << timestamp << std::endl;
            std::cout << "Sender: " << sender << std::endl;
            std::cout << "Content: " << content << std::endl;
        }
        else
        {
            std::cout << "No match" << std::endl;
        }
    }
    return 0;
}

// char * arr1 = R"(3!@^q8#9Z$%&*aAbBcCdD4(5)6fFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ7{0}1|2[3]4:5;6'7"8<9>0?,.)";
// char * arr2 = R"(QwErTyUiOpAsDfGhJkLzXcVbNm1234567890-=!@#$%^&*()_+[]{}\|;':",./<>?)";
// char * arr3 = R"(9oO8pP7iI6uU5yY4tT3rR2eE1wWqQ0aAsSdDfFgGhHjJkKlLzZxXcCvVbBnNmM`~!@#$%^&*()_-+={[}]|\:;"'<,>.?/)";
// char * arr4 = R"(aA1sS2dD3fF4gG5hH6jJ7kK8lL9zZ0xXcCvVbBnNmMqQwWeErRtTyYuUiIoOpP-_=+[{]}|\;:'",<.>/?`~)";
// char * arr5 = R"(5tT4rR3eE2wW1qQ0aAsSdDfFgGhHjJkKlLzZxXcCvVbBnNmM6yY7uU8iI9oO0pP-[{]}|\;:'",<.>/?`~!@#$%^&*()_+=)";
