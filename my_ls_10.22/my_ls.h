#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<dirent.h>
#include<string.h>
#include<libgen.h>
#include<sys/types.h>
#include<sys/stat.h>

#include<time.h>
#include<stdbool.h>
#define NONE                 "\e[0m"
#define BLACK                "\e[0;30m"
#define L_BLACK              "\e[1;30m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define GREEN                "\e[0;32m"
#define L_GREEN              "\e[1;32m"
#include <pwd.h>
#include <grp.h> 
typedef struct my_dirent
{
    struct my_dirent* next;
    ino_t d_ino;     
    char d_name[256];  
    }my_dirent;

struct stack_dirent{
    //struct stack_dirent_node * stack;
    struct stack_dirent_node * head;
    struct stack_dirent_node * end;
    int size;
};
struct stack_dirent_node{
    ino_t d_ino;
    char d_name[256];  
};
void print_ls_of_one_RRR(char * path);
void print_ls_of_one(char * path);
void  print_ls_of_one_trtrtr(char * path);
void print_Permissions(mode_t mode);//获得权限输出
void print_ls(char * path);
void sortR(my_dirent *p_head);
long judgment(my_dirent *a,my_dirent *b);
void sortTr(my_dirent* p_head);
void sortT(my_dirent* p_head);
void  print_ls_of_one_trtrtr(char * path);
void print_ls_of_one(char * path);
void print_ls_FOR_R(char * path,char* a);
void stack_iniput(struct stack_dirent *stack,struct dirent *direntpointre);
struct stack_dirent_node* stack_output(struct stack_dirent *stack);
bool a = 0;
bool s = 0;
bool i = 0;
bool t = 0;
bool l = 0;
bool r = 0;
bool R = 1;
