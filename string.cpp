#define _CRT_SECURE_NO_WARNINGS

#include "string.h"
#include <cstring>

string::string() {
  _arr = MALLOC(char, 1);
  *_arr = '\0';
  _capacity = 1;
  _size = 0;
}
string::string(const string &p) {
  char *t = p._arr;
  int len = 0;
  while (*(t++) != '\0') {
    len++;
  }
  _arr = MALLOC(char, len + 1);
  t = p._arr;
  for (int j = 0; j <= len; j++) {
    _arr[j] = *(t++);
  }
  _capacity = len + 1;
  _size = len;
}
string::string(const char *p) {
  assert(p);
  char *t = (char *)p;
  int len = 0;
  while (*(t++) != '\0') {
    len++;
  }
  _arr = MALLOC(char, len + 1);
  t = (char *)p;
  for (int j = 0; j <= len; j++) {
    _arr[j] = *(t++);
  }
  _capacity = len + 1;
  _size = len;
}
string::string(const char *p, int len) {
  assert(p);
  assert(len >= 0);
  char *t = (char *)p;
  int l = 0;
  while (*(t++) != '\0') {
    l++;
  }
  if (len >= l) {
    _arr = MALLOC(char, l + 1);
    t = (char *)p;
    for (int j = 0; j <= l; j++) {
      _arr[j] = *(t++);
    }
    _capacity = l + 1;
    _size = l;
  } else {
    _arr = MALLOC(char, len + 1);
    t = (char *)p;
    for (int j = 0; j < len; j++) {
      _arr[j] = *(t++);
    }
    _capacity = len + 1;
    _size = len;
    _arr[_size] = '\0';
  }
}
string::string(const char *p, int pos, int len) {
  assert(p);
  assert(len >= 0);
  assert(pos <= _size);
  pos--;
  char *t = ((char *)p) + pos;
  int l = 0;
  while (*(t++) != '\0') {
    l++;
  }
  if (len >= l) {
    _arr = MALLOC(char, l + 1);
    t = ((char *)p) + pos;
    for (int j = 0; j <= l; j++) {
      _arr[j] = *(t++);
    }
    _capacity = l + 1;
    _size = l;
  } else {
    _arr = MALLOC(char, len + 1);
    t = ((char *)p) + pos;
    for (int j = 0; j < len; j++) {
      _arr[j] = *(t++);
    }
    _capacity = len + 1;
    _size = len;
    _arr[_size] = '\0';
  }
}
string::string(int count, char a) {
  assert(count > 0);
  _arr = MALLOC(char, count + 1);
  int i = 0;
  for (i = 0; i < count; i++) {
    _arr[i] = a;
  }
  _arr[i] = '\0';
  _capacity = count + 1;
  _size = count;
}
string &string::assign(const char *p) {
  assert(p);
  free(_arr);
  char *t = (char *)p;
  int len = 0;
  while (*(t++) != '\0') {
    len++;
  }
  _arr = MALLOC(char, len + 1);
  t = (char *)p;
  for (int j = 0; j <= len; j++) {
    _arr[j] = *(t++);
  }
  _capacity = len + 1;
  _size = len;
  return *this;
}
string &string::assign(const char *p, int len) {
  assert(p);
  assert(len >= 0);
  free(_arr);
  char *t = (char *)p;
  int l = 0;
  while (*(t++) != '\0') {
    l++;
  }
  if (len >= l) {
    _arr = MALLOC(char, l + 1);
    t = (char *)p;
    for (int j = 0; j <= l; j++) {
      _arr[j] = *(t++);
    }
    _capacity = l + 1;
    _size = l;
  } else {
    _arr = MALLOC(char, len + 1);
    t = (char *)p;
    for (int j = 0; j < len; j++) {
      _arr[j] = *(t++);
    }
    _capacity = len + 1;
    _size = len;
    _arr[_size] = '\0';
  }
  return *this;
}
string &string::assign(const char *p, int pos, int len) {
  assert(p);
  assert(len >= 0);
  assert(pos <= _size);
  free(_arr);
  pos--;
  char *t = ((char *)p) + pos;
  int l = 0;
  while (*(t++) != '\0') {
    l++;
  }
  if (len >= l) {
    _arr = MALLOC(char, l + 1);
    t = ((char *)p) + pos;
    for (int j = 0; j <= l; j++) {
      _arr[j] = *(t++);
    }
    _capacity = l + 1;
    _size = l;
  } else {
    _arr = MALLOC(char, len + 1);
    t = ((char *)p) + pos;
    for (int j = 0; j < len; j++) {
      _arr[j] = *(t++);
    }
    _capacity = len + 1;
    _size = len;
    _arr[_size] = '\0';
  }
  return *this;
}
string &string::assign(int count, char a) {
  assert(count > 0);
  free(_arr);
  _arr = MALLOC(char, count + 1);
  int i = 0;
  for (i = 0; i < count; i++) {
    _arr[i] = a;
  }
  _arr[i] = '\0';
  _capacity = count + 1;
  _size = count;
  return *this;
}
string string::substr(int i) {
  assert(i > 0);
  assert(i <= _size);
  char c = _arr[i - 1];
  string tmp(1, c);
  return tmp;
}
string string::substr(int pos, int len) {
  assert(len >= 0);
  assert(pos <= _size);
  int l = _size - pos + 1;
  char *p;
  if (len >= l) {
    p = MALLOC(char, l + 1);
    int i = 0;
    for (int j = pos - 1; j <= pos - 1 + l; j++) {
      p[i++] = _arr[j];
    }
  } else {
    p = MALLOC(char, len + 1);
    int i = 0;
    for (int j = pos - 1; j < pos - 1 + len; j++) {
      p[i++] = _arr[j];
    }
    p[i] = '\0';
  }
  string tmp(p);
  return tmp;
}
const char *string::data() {
  char *p = MALLOC(char, _capacity);
  for (int i = 0; i < _capacity; i++) {
    p[i] = _arr[i];
  }
  return (const char *)p;
}
char *string::copy(char *p, int count, int pos) {
  assert(p);
  assert(count >= 0);
  assert(pos <= _size);
  int l = _size - pos + 1;
  if (count >= l) {
    int i = 0;
    for (int j = pos - 1; j <= pos - 1 + l; j++) {
      p[i++] = _arr[j];
    }
  } else {
    int i = 0;
    for (int j = pos - 1; j < pos - 1 + count; j++) {
      p[i++] = _arr[j];
    }
    p[i] = '\0';
  }
  return p;
}
string &string::insert(int i, int count, char a) {
  assert(i <= _capacity);
  assert(i > 0);
  int q = 0;
  char *tmp = MALLOC(char, _size - i + 2);

  for (int j = i - 1; j < _capacity; j++) {
    tmp[q++] = _arr[j];
  }
  if (_capacity - (count + _size) < 1) {
    _arr = REALLOC(_arr, char, _capacity + count);
    _capacity = _capacity + count;
  }
  for (int j = i - 1; j < i - 1 + count; j++) {
    _arr[j] = a;
  }
  int j = i + count - 1;
  for (int k = 0; k < _size - i + 2; k++) {
    _arr[j++] = tmp[k];
  }
  _size += count;
  free(tmp);
  return *this;
}
string &string::insert(int i, const char *p) {
  char *tmp = MALLOC(char, _size - i + 2);
  int q = 0;
  for (int j = i - 1; j < _capacity; j++) {
    tmp[q++] = _arr[j];
  }

  char *t = (char *)p;
  int len = 0;
  while (*(t++) != '\0') {
    len++;
  }
  if (_capacity - (len + _size) < 1) {
    _arr = REALLOC(_arr, char, _capacity + len);
    _capacity = _capacity + len;
  }

  t = (char *)p;
  for (int j = i - 1; j < i + len - 1; j++) {
    _arr[j] = *(t++);
  }
  int j = i + len - 1;
  for (int k = 0; k < _size - i + 2; k++) {
    _arr[j++] = tmp[k];
  }
  _size += len;
  free(tmp);
  return *this;
}
string &string::insert(int i, const char *p, int count) {
  char *tmp = MALLOC(char, _size - i + 2);
  int q = 0;
  for (int j = i - 1; j < _capacity; j++) {
    tmp[q++] = _arr[j];
  }

  char *t = (char *)p;
  int len = 0;
  while (*(t++) != '\0') {
    len++;
  }
  if (count > len) {
    count = len;
  }
  if (_capacity - (count + _size) < 1) {
    _arr = REALLOC(_arr, char, _capacity + count);
    _capacity = _capacity + count;
  }

  t = (char *)p;
  for (int j = i - 1; j < i + count - 1; j++) {
    _arr[j] = *(t++);
  }
  int j = i + count - 1;
  for (int k = 0; k < _size - i + 2; k++) {
    _arr[j++] = tmp[k];
  }
  free(tmp);
  _size += count;
  return *this;
}
string &string::insert(int i, const char *p, int pos, int count) {
  char *tmp = MALLOC(char, _size - i + 2);
  int q = 0;
  for (int j = i - 1; j < _capacity; j++) {
    tmp[q++] = _arr[j];
  }

  char *t = (char *)p;
  int len = 0;
  while (*(t++) != '\0') {
    len++;
  }
  assert(pos <= len);
  if (count > len) {
    count = len;
  }
  if (_capacity - (count + _size) < 1) {
    _arr = REALLOC(_arr, char, _capacity + count - pos + 1);
    _capacity = _capacity + count - pos + 1;
  }

  t = (char *)p + pos - 1;
  for (int j = i - 1; j < i + count - pos; j++) {
    _arr[j] = *(t++);
  }
  int j = i + count - pos;
  for (int k = 0; k < _size - i + 2; k++) {
    _arr[j++] = tmp[k];
  }
  free(tmp);
  _size += count;
  return *this;
}
string &string::operator=(const string &str) {
  assert(this != &str);
  free(_arr);
  char *p = str._arr, *t = p;
  int len = 0;
  while (*(t++) != '\0') {
    len++;
  }
  _arr = MALLOC(char, len + 1);
  t = p;
  for (int i = 0; i <= len; i++) {
    _arr[i] = *(t++);
  }
  _capacity = len + 1;
  _size = len;
  return *this;
}
bool string::operator<(const string &str) {
  char *a = _arr, *b = str._arr;
  while (*a != '\0' && *b != '\0') {
    if (*a < *b) {
      return true;
    }
    a++;
    b++;
  }
  if (*b != '\0') {
    return true;
  }
  return false;
}
bool string::operator==(const string &str) {
  char *a = _arr, *b = str._arr;
  while (*a != '\0' && *b != '\0') {
    if (*a != *b) {
      return false;
    }
    a++;
    b++;
  }
  if (*b != '\0' || *a != '\0') {
    return false;
  }
  return true;
}
bool string::operator<=(const string &str) {
  return *this < str || *this == str;
}
bool string::operator>(const string &str) { return !((*this) <= str); }
bool string::operator>=(const string &str) { return !((*this) > str); }
string &string::operator+=(const string &str) {
  if (_capacity - (_size + str._size) < 1) {
    _arr = REALLOC(_arr, char, _capacity + str._size);
    _capacity = _capacity + str._size;
  }
  char *t = str._arr;
  for (int i = 0; i < str._capacity; i++) {
    _arr[_size++] = *(t++);
  }
  _size--;
  return *this;
}
string string::operator+(const string &str) {
  string arr1 = *this;
  arr1 += str;
  return arr1;
}
string &string::operator+=(const char *p) {
  assert(p);
  char *t = (char *)p;
  int l = 0;
  while (*(t++) != '\0') {
    l++;
  }
  if (_capacity - (_size + l) < 1) {
    _arr = REALLOC(_arr, char, _capacity + l + 2);
    _capacity = _capacity + l + 1;
  }
  t = (char *)p;
  for (int i = 0; i <= l; i++) {
    _arr[_size++] = *(t++);
  }
  _size--;
  return *this;
}
void string::printarr() { std::cout << _arr << std::endl; }
string::~string() {
  free(_arr);
  _capacity = 0;
  _size = 0;
}
void test() {
  string arr1("abc");
  arr1.printarr();
  string arr2("abcd", 2, 5);
  arr2.printarr();
  string arr3(3, 'a');
  arr3.printarr();
  string arr4(arr3);
  arr4.printarr();
  string arr5;
  arr5.assign("abcde");
  arr5.printarr();
  arr3 = arr3.assign("abcccddd", 3, 7);
  arr3.printarr();
}
void test1() {
  string arr1("abc");
  string arr2("abcd", 2, 5);
  arr1 += arr2;
  arr1.printarr();
  arr1 += "love";
  arr1.printarr();
}
void test2() {
  string arr1("abcdef");
  string arr2;
  arr2 = arr1.substr(1);
  arr2.printarr();
  string arr3 = arr1.substr(2, 9);
  arr3.printarr();
  const char *arr4 = arr1.data();
  std::cout << arr4 << std::endl;
  char *p = MALLOC(char, 4);
  arr1.copy(p, 2, 4);
  std::cout << p << std::endl;
}
void test3() {
  string arr1("abcdef");
  arr1.insert(2, 3, 'a');
  arr1.printarr();
  arr1.insert(2, "abc");
  arr1.printarr();
  arr1.insert(2, "abc", 2);
  arr1.printarr();
  arr1.insert(7, "abcd", 2, 10);
  arr1.printarr();
}
void test4() {
  string arr1 = "abc";
  string arr2("def");
  string arr3 = arr1 + arr2;
  arr3.printarr();
  arr1 = arr1 + arr2;
  arr1.printarr();
  printf("%s\n", arr1.data());
  std::cout << (arr1 == arr3) << std::endl;
}
int main() {
#include <string>
 
  string a = "abc";
  std::string aa = "abc";
 
  aa.insert(0, "wang", strlen("wang"));
  a.insert(0, "wang", strlen("wang"));
 
  std::cout << a << std::endl;
  std::cout << aa << std::endl;

//   a = aa.c_str();
  std::cout << a.substr(2, 4) << std::endl;
  std::cout << aa.substr(2, 4) << std::endl;

  return 0;
}