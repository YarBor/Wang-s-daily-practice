/*
* 文件名：mypwd.c
* 描述：实现简单的pwd命令
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
// char pathname[512]={0};
// int main(int argc,char **argv)
// {
// struct aaa 
// {
//     int a ;
//     char aa;

// }a;
// a.a=123;
// a.aa= 'a';
// memset(&a,0,sizeof(a));
// return 0;
// }
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
struct stack_dirent stack={NULL,NULL,16}; 
void stack_iniput(struct stack_dirent stack,struct dirent *direntpointre)
{
    if(stack.end==NULL)
    {
        stack.end=(struct stack_dirent_node *)malloc(sizeof(struct stack_dirent_node)*16);
        stack.head=stack.end;
    }
    if(stack.head-stack.end==stack.size)
    {
        stack.end=(struct stack_dirent_node *)realloc(stack.end,sizeof(struct stack_dirent_node)*stack.size*2);
        stack.head=stack.end+stack.size;
        stack.size*=2;
    }
    memcpy(stack.head->d_name,direntpointre->d_name,256);
    stack.head->d_ino=direntpointre->d_ino;
    stack.head++;

}
struct stack_dirent_node* stack_output(struct stack_dirent stack)
{
    if (stack.head-stack.end==0) 
    {
        free(stack.end);
        return NULL;
    }
    struct stack_dirent_node* a = --(stack.head);
    if(stack.head-stack.end==0) free(stack.end);
    return a;
}

int main()
{
    DIR* a = NULL;
    a = opendir("/home/wang/Desktop/object/my_ls_10.21");
    a = opendir("/home/wang/Desktop/object/my_ls_10.21");
    closedir(a);
    closedir(a);


}
open_(char * name,DIR * dir)
{

}