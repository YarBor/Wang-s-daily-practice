#include <cstring>
#include <iostream>
#include <stdlib.h>

class string {
#define BUFFER_SIZE 1024
#define npos -1
public:
  ~string() {
    if (buffer)
      free(buffer);
    buffer = NULL;
  }

  string(string const &other) : length(other.length), size(other.size) {
    buffer = (char *)malloc(size);
    memcpy(buffer, other.buffer, size);
  }

  string() : length(0), size(BUFFER_SIZE) {
    buffer = (char *)malloc(size);
    memset(buffer, 0, size);
  };
  string(const char *s)
      : length(strlen(s)),
        size(BUFFER_SIZE > strlen(s) ? BUFFER_SIZE : strlen(s) * 2) {
    buffer = (char *)malloc(size);
    memset(buffer, 0, size);
    strcpy(buffer, s);
  };
  string(const char *s, size_t n)
      : length(n), size(BUFFER_SIZE > n ? BUFFER_SIZE : n * 2) {
    buffer = (char *)malloc(size);
    strncpy(buffer, s, n);
  };
  string(size_t n, char c)
      : length(n), size(BUFFER_SIZE > n ? BUFFER_SIZE : n * 2) {
    buffer = (char *)malloc(size);
    memset(buffer, c, n);
  };
  string(const string &str, size_t pos, size_t len) {
    if (pos >= str.length) {
      size = BUFFER_SIZE;
      len = 0;
      buffer = (char *)malloc(size);
      memset(buffer, 0, size);
    } else {
      auto tmp = strnlen(str.c_str() + pos, str.length);
      size = tmp > BUFFER_SIZE ? tmp * 2 : BUFFER_SIZE;
      buffer = (char *)malloc(size);
      memset(buffer, 0, size);
      length = strlen(strncpy(buffer, str.c_str() + pos, str.length));
      buffer[length] = '\0';
    }
  }
  // string(size_t n, char c){};
  char const *c_str() const { return buffer; };
  string operator+(string const &str) {
    string result(this->c_str());
    result += str.c_str();
    return result;
  };
  string &operator+=(string const &str) {
    if (this->size <= this->length + str.length) {
      size = (this->length + str.length) * 2;
      buffer = (char *)realloc(buffer, size);
    }
    strcat(buffer + length, str.c_str());
    length = strlen(buffer);
    buffer[length] = '\0';
    return *this;
  };
  string &operator=(string const &str) {
    if (this->size <= str.length)
      buffer = (char *)realloc(buffer, (str.length) * 2);
    strcat(buffer, str.c_str());
    length = str.length;
    buffer[length] = '\0';
    return *this;
  }
  bool operator==(string const &str) const {
    return strcmp(buffer, str.c_str()) == 0;
  }
  bool operator!=(string const &str) const { return !(*this == str); }
  bool operator<(string const &str) const {
    return strcmp(buffer, str.c_str()) < 0;
  }
  bool operator<=(string const &str) const {
    return *this < str || *this == str;
  }
  bool operator>=(string const &str) const {
    return !(*this < str) || *this == str;
  }
  bool operator>(string const &str) const { return !(*this <= str); }

  string &insert(size_t pos, const string &str) {
    if (size <= str.length + length) {
      size = (str.length + length) * 2;
      buffer = (char *)realloc(buffer, size);
      if (buffer == nullptr) {
        throw std::bad_alloc();
      }
    }
    memmove(buffer + pos + str.length, buffer + pos, length - pos);
    memcpy(buffer + pos, str.c_str(), str.length);
    length += str.length;
    buffer[length] = '\0';
    return *this;
  }

  string &insert(size_t pos, const char *s) {
    return insert(pos, s, strlen(s));
  }

  string &insert(size_t pos, const char *s, size_t n) {
    if (size <= n + length) {
      size = (n + length) * 2;
      buffer = (char *)realloc(buffer, size);
    }
    memmove(buffer + pos + n, buffer + pos, length - pos);
    memcpy(buffer + pos, s, n);
    length += n;
    buffer[length] = '\0';
    return *this;
  }

  string &insert(size_t pos, size_t n, char c) {
    if (size <= n + length) {
      size = (n + length) * 2;
      buffer = (char *)realloc(buffer, size);
    }
    memmove(buffer + pos + n, buffer + pos, length - pos);
    memset(buffer + pos, c, n);
    length += n;
    buffer[length] = '\0';
    return *this;
  }

  string &assign(const char *p) { return assign(p, strlen(p)); }

  string &assign(const char *p, int len) {
    if (size <= len) {
      size = len * 2;
      buffer = (char *)realloc(buffer, size);
    }
    memcpy(buffer, p, len);
    length = len;
    buffer[length] = '\0';
    return *this;
  }

  string &assign(const char *p, int pos, int len) {
    return assign(p + pos, len);
  }

  string &assign(int count, char a) {
    if (size <= count) {
      size = count * 2;
      buffer = (char *)realloc(buffer, size);
    }
    memset(buffer, a, count);
    length = count;
    buffer[length] = '\0';
    return *this;
  }
  string &assign(string &other) { return this->operator=(other); }

private:
  char *buffer = NULL;
  size_t length = 0;
  size_t size = BUFFER_SIZE;
};

int main() {
  // default constructor
  string str1;
  std::cout << "str1: " << str1.c_str() << std::endl;

  // character array constructor
  string str2("hello");
  std::cout << "str2: " << str2.c_str() << std::endl;

  // character array and length constructor
  string str3("world", 3);
  std::cout << "str3: " << str3.c_str() << std::endl;

  // character and count constructor
  string str4(10000, 'x');
  std::cout << "str4: " << str4.c_str() << std::endl;

  // copy constructor
  string str5 = str2;
  std::cout << "str5: " << str5.c_str() << std::endl;

  // operator + and +=
  string str6 = "hi";
  str6 += " there";
  string str7 = str6 + "!";
  std::cout << "str6: " << str6.c_str() << std::endl;
  std::cout << "str7: " << str7.c_str() << std::endl;

  // relational operators
  std::cout << std::boolalpha;
  std::cout << "'hello' == 'world' ? " << (str2 == str3) << std::endl;
  std::cout << "'hello' != 'world' ? " << (str2 != str3) << std::endl;
  std::cout << "'hello' < 'world' ? " << (str2 < str3) << std::endl;
  std::cout << "'hello' <= 'world' ? " << (str2 <= str3) << std::endl;
  std::cout << "'hello' >= 'world' ? " << (str2 >= str3) << std::endl;
  std::cout << "'hello' > 'world' ? " << (str2 > str3) << std::endl;

  // insert function
  string str8 = "banana";
  str8.insert(2, "x");
  std::cout << "str8: " << str8.c_str() << std::endl;
  return 0;
}
