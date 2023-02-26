#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define ROOM_NUMBER 1
#define ROOM_TYPE 2
#define IS_AVAILABLE 3
#define ROOM_PRICE 4
#define CHECK_IN_TIME 5
#define CHECK_OUT_TIME 6

struct Room
{
  int room_number;         // 房号
  char room_type[20];      // 房间种类
  bool is_available;       // 是否居住
  double room_price;       // 房间价格
  char check_in_time[20];  // 入住时间
  char check_out_time[20]; // 交房时间
};

// 创建哨兵节点
struct Node *create_dummy_node();
// 创建房间节点
struct Node *create_node(int room_number, char room_type[], bool is_available, double room_price,
                         char check_in_time[], char check_out_time[]);
// 删除特定节点
struct Node *headDel(struct Node *head); // shao bing

// 销毁
struct Node *destory(struct Node *head);

// 返回找到节点的前一个节点 如是尾节点 则标识位为true 未找到返回 null
struct Node *find(struct Node *head, int room_number, bool *if_tail);

// 删除特定节点
struct Node *delete(struct Node *head, int room_number);

// 修改特定节点
struct Node *change(struct Node *head, int room_number);

// 打印链表
void print_list(struct Node *dummy);

// 打印菜单
int chooselist();

struct Node *tailInsert(struct Node *head, struct Node *node);
void search_print(struct Node *head);
struct Node *filein(char *usrname, struct Node *head);

struct Node *fileout(char *usrname, struct Node *head);
struct Node *sort(struct Node *head);