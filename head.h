#include <string.h>
#include <stdio.h>  
#include <stdlib.h>
#include <assert.h>
#include<stdbool.h>
typedef struct strbuf {
  int len;     //当前缓冲区（字符串）长度
  int alloc;   //当前缓冲区（字符串）容量
  char *buf;   //缓冲区（字符串）
}strbuf;
int swap(int *a, int* b) ;
void strbuf_init(struct strbuf *sb, size_t alloc);
    //初始化 sb 结构体，容量为 alloc

void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc);//这里的alloc目的是啥????????
    //将字符串填充到 sb 中，长度为 len, 容量为 alloc。
    // 哈哈哈哈哈哈 

void strbuf_release(struct strbuf *sb);
    //释放 sb 结构体的内存。

void strbuf_swap(struct strbuf *a, struct strbuf *b);
    //交换两个 strbuf。

char *strbuf_detach(struct strbuf *const sb, size_t *sz);//!!返回的字符串是malloc的!!
        //需求是啥???取出的详细含义???以什么形式取出
    //将 sb 中的原始内存取出，并将 sz 设置为其 alloc 大小 

int strbuf_cmp(const struct strbuf *first, const struct strbuf *second);
    //比较两个 strbuf 的内存是否相同。

void strbuf_reset(struct strbuf *sb);
    //清空 sb。
int strbuf_grow(struct strbuf *sb, size_t extra);
    //确保在 len 之后 strbuf 中至少有 extra 个字节的空闲空间可用。

///secend task //////////////////////////////////////////////////////////////////////////////
void strbuf_add(struct strbuf *sb, const void *data, size_t len);
//向 sb 追加长度为 len 的数据 data。

void strbuf_addch(struct strbuf *sb, char c);
//	向 sb 追加一个字符 c。

void strbuf_addstr(strbuf * sb,const char* s);
//向 sb 追加一个字符串 s。

void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2);
//向一个 sb 追加另一个 strbuf的数据。

void strbuf_setlen(struct strbuf *sb, size_t len);
//设置 sb 的长度 len。

size_t strbuf_avail(const struct strbuf *sb);
//计算 sb 目前仍可以向后追加的字符串长度。

void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len);
//向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data。

void strbuf_ltrim(struct strbuf *sb);	                        
//去除 sb 缓冲区左端的所有 空格，tab, '\t'。

void strbuf_rtrim(struct strbuf *sb);                           
//去除 sb 缓冲区右端的所有 空格，tab, '\t'。

void strbuf_remove(struct strbuf *sb, size_t pos, size_t len);  
//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。

// ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint);
//     //sb 增长 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容追加到 sb 中。
// int strbuf_getline(struct strbuf *sb, FILE *fp);
//     //将 将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb 。
// 没看懂

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max);
//将长度为 len 的字符串 str 根据切割字符 terminator 切成多个 strbuf,并从结果返回，max 可以用来限定最大切割数量。
//返回 struct strbuf 的指针数组，数组的最后元素为 NULL

bool strbuf_begin_judge(char* target_str, const char* str, int strlen);
//target_str : 目标字符串，str : 前缀字符串，strlen : target_str 长度 ，前缀相同返回 true 失败返回 false


char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len);
//target_str : 目标字符串，begin : 开始下标，end 结束下标。len : target_buf的长度，参数不合法返回 NULL. 下标从0开始，[begin, end)区间。