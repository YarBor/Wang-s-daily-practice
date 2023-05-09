#pragma once

#define MALLOC(type, num) (type *)malloc(sizeof(type) * (num))
#define REALLOC(obj, type, num) (type *)realloc(obj, sizeof(type) * (num))

#include <assert.h>
#include <iostream>
#include <stdlib.h>

struct string {
  // friend std::ostream &operator<<(std::ostream &o, const string &a) {
  //   return o << a._arr;
  // };
  string();
  string(const string &p);
  string(const char *p);
  string(const char *p, int len);
  string(const char *p, int pos, int len);
  string(int count, char a);

  string &assign(const char *p);
  string &assign(const char *p, int len);
  string &assign(const char *p, int pos, int len);
  string &assign(int count, char a);

  string &insert(int i, int count, char a);
  string &insert(int i, const char *p);
  string &insert(int i, const char *p, int count);
  string &insert(int i, const char *p, int pos, int count);

  string substr(int i);
  string substr(int pos, int len);

  const char *data();
  char *copy(char *p, int count, int pos);

  string operator+(const string &str);
  string &operator+=(const string &str);
  string &operator+=(const char *p);
  string &operator=(const string &str);
  bool operator<(const string &str);
  bool operator<=(const string &str);
  bool operator==(const string &str);
  bool operator>(const string &str);
  bool operator>=(const string &str);

  void printarr();

  ~string();

  char *_arr;
  int _size;
  int _capacity;
};
