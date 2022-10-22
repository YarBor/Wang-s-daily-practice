#include "my_ls.h"

struct stat *statnode = NULL;
void print_ls_of_one(char *path);
void print_ls_of_one_trtrtr(char *path);
void print_Permissions(mode_t mode) //获得权限输出
{
    printf("%c%c%c%c%c%c%c%c%c  ", (mode & S_IRUSR) ? 'r' : '-', (mode & S_IWUSR) ? 'w' : '-', (mode & S_IXUSR) ? 'x' : '-', (mode & S_IRGRP) ? 'r' : '-',
           (mode & S_IWGRP) ? 'w' : '-', (mode & S_IXGRP) ? 'x' : '-', (mode & S_IROTH) ? 'r' : '-', (mode & S_IWOTH) ? 'w' : '-', (mode & S_IXOTH) ? 'x' : '-');
}
void print_ls(char *path)
{
    if (R)
    {
        print_ls_of_one_RRR(path);
    }
    else if (t | r)
    {
        print_ls_of_one_trtrtr(path);
    }
    else
    {
        print_ls_of_one(path);
    }
}
void stack_iniput(struct stack_dirent *stack, struct dirent *direntpointre)
{
    if ((*stack).end == NULL)
    {
        (*stack).end = (struct stack_dirent_node *)malloc(sizeof(struct stack_dirent_node) * 10);
        (*stack).head = (*stack).end;
        memset(stack->end, 0, sizeof(struct stack_dirent_node) * 10);
    }
    if ((*stack).head - (*stack).end == (*stack).size)
    {
        (*stack).end = (struct stack_dirent_node *)realloc((*stack).end, sizeof(struct stack_dirent_node) * (*stack).size * 2);
        (*stack).head = (*stack).end + (*stack).size;
        memset(stack->end+(*stack).size, 0, sizeof(struct stack_dirent_node) * ((*stack).size));
        (*stack).size *= 2;
    }
    memcpy((*stack).head->d_name, direntpointre->d_name, 256);
    (*stack).head->d_ino = direntpointre->d_ino;
    (*stack).head++;
}
struct stack_dirent_node *stack_output(struct stack_dirent *stack)
{
    if ((*stack).head - (*stack).end == 0)
    {
        free((*stack).end);
        return NULL;
    }
    struct stack_dirent_node *a = --((*stack).head);
    // if((*stack).head-(*stack).end==0) free((*stack).end);
    return a;
}
void sortR(my_dirent *p_head)
{
    my_dirent *pb, *pf, temp;
    pf = p_head;
    if (p_head == NULL)
    {   //链表为空
        //  printf("needn't order.\n");
        return;
    }
    if (p_head->next == NULL)
    { //链表有1个节点
        // printf("only one print, needn't order.\n");
        return;
    }
    while (pf->next != NULL)
    {                  //以pf指向的节点为基准节点
        pb = pf->next; // pb从基准点的下一个节点开始ino_t d_ino
        while (pb != NULL)
        {
            if (strcmp(pf->d_name, pb->d_name) < 0)
            {
                long i;
                i = pf->d_ino;
                pf->d_ino = pb->d_ino;
                pb->d_ino = i;
                char a[256];
                memcpy(a, pf->d_name, 256);
                memcpy(pf->d_name, pb->d_name, 256); // youhua
                memcpy(pb->d_name, a, 256);
            }
            pb = pb->next;
        }
        pf = pf->next;
    }
    return;
}
long judgment(my_dirent *a, my_dirent *b) //时间排序判断 // 无分配内存
{
    struct stat aa, bb;
    memset(&aa, 0, sizeof(aa));
    memset(&bb, 0, sizeof(bb));

    if (0 != (stat(a->d_name, &aa)))
    {
        printf("%20s\n", a->d_name);
        puts("can't open\n");
        return 0;
    }
    if (0 != stat(b->d_name, &bb))
    {
        printf("%20s\n", b->d_name);
        puts("can't open\n");
        return 0;
    }
    return (aa.st_atime - bb.st_atime);
}
void sortTr(my_dirent *p_head) //逆向时间排序  //无分配内存
{
    my_dirent *pb, *pf, temp;
    pf = p_head;
    if (p_head == NULL)
    {   //链表为空
        //  printf("needn't order.\n");
        return;
    }
    if (p_head->next == NULL)
    { //链表有1个节点
        // printf("only one print, needn't order.\n");
        return;
    }
    while (pf->next != NULL)
    {                  //以pf指向的节点为基准节点
        pb = pf->next; // pb从基准点的下一个节点开始ino_t d_ino
        while (pb != NULL)
        {
            if (judgment(pb, pf) > 0)
            { //判断函数
                long i;
                i = pf->d_ino;
                pf->d_ino = pb->d_ino;
                pb->d_ino = i;
                char a[256];
                memcpy(a, pf->d_name, 256);
                memcpy(pf->d_name, pb->d_name, 256); // youhua
                memcpy(pb->d_name, a, 256);
            }
            pb = pb->next;
        }
        pf = pf->next;
    }
    return;
}
void sortT(my_dirent *p_head) //正向时间排序  //无分配内存
{
    my_dirent *pb, *pf, temp;
    pf = p_head;
    if (p_head == NULL)
    {   //链表为空
        //  printf("needn't order.\n");
        return;
    }
    if (p_head->next == NULL)
    { //链表有1个节点
        // printf("only one print, needn't order.\n");
        return;
    }
    while (pf->next != NULL)
    {                  //以pf指向的节点为基准节点
        pb = pf->next; // pb从基准点的下一个节点开始ino_t d_ino
        while (pb != NULL)
        {
            if (judgment(pb, pf) < 0)
            { //判断函数
                long i;
                i = pf->d_ino;
                pf->d_ino = pb->d_ino;
                pb->d_ino = i;
                char a[256];
                memcpy(a, pf->d_name, 256);
                memcpy(pf->d_name, pb->d_name, 256); // youhua
                memcpy(pb->d_name, a, 256);
            }
            pb = pb->next;
        }
        pf = pf->next;
    }
    return;
}
void print_ls_of_one_trtrtr(char *path) // 排序打印 // 链表
{
    chdir(path);
    if (r && (!t))
    {
        DIR *DIRpointer;
        struct dirent *direntpointer;
        DIRpointer = opendir(path);
        // int i = 0;
        my_dirent *head = NULL;
        while (!((direntpointer = readdir(DIRpointer)) == NULL))
        {

            {
                my_dirent *a = (my_dirent *)malloc(sizeof(my_dirent));
                assert(a);
                a->next = (head);
                (a)->d_ino = direntpointer->d_ino;
                memcpy((a)->d_name, direntpointer->d_name, 256);
                (head) = a;
            }
        }
        sortR(head); //链表排序完
        // while(!((direntpointer=readdir(DIRpointer))==NULL))
        my_dirent *direntpointer1;

        for (direntpointer1 = head; direntpointer1 != NULL; direntpointer1 = direntpointer1->next)
        {
            // char name_wenjian [512];
            /*可以优化*/ //  memset(name_wenjian,0,512);
                         //  memcpy(name_wenjian,path,strlen(path));
                         //  strncat(name_wenjian,direntpointer->d_name,strlen(direntpointer->d_name));
            struct stat statnode;
            memset(&statnode, 0, sizeof(statnode));
            chdir(path);
            if (0 != lstat(direntpointer1->d_name, &statnode))
            {
                printf("%20s\n", direntpointer1->d_name);
                puts("can't open");
                continue;
            }; //文件名不是文件路径
            if (S_ISLNK(statnode.st_mode))
            {//软链接 X
               // printf("%s\t @", direntpointer->d_name);
                continue;
            }
            bool if_dir = !(S_ISREG(statnode.st_mode));
            if (!(s | l)) //不需要访问stat时
            {
                if (!(i)) //单纯打翻了代码
                {
                    if (!(a)) //无i-node情况
                    {
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name); //彩色文件
                        else
                            printf("%20s\n", direntpointer1->d_name);
                    }
                    else
                    {

                        if (direntpointer1->d_name[0] == '.')
                            continue;
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name);
                        else //彩色文件
                            printf("%20s\n", direntpointer1->d_name);
                    }
                }
                else
                {
                    if (a) // 有i-node情况
                    {
                        printf("%lu  ", direntpointer1->d_ino);
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name);
                        else //彩色文件
                            printf("%20s\n", direntpointer1->d_name);
                    }
                    else
                    {
                        if (direntpointer1->d_name[0] == '.')
                            continue;
                        printf("%lu  ", direntpointer1->d_ino);
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name);
                        else //彩色文件
                            printf("%20s\n", direntpointer1->d_name);
                    }
                }
            }
            else
            { //需要stat时
                if (!(a))
                    if (direntpointer1->d_name[0] == '.')
                        continue;
                // char name_wenjian [512];
                /*可以优化*/ // memset(name_wenjian,0,512);========== 优化p [error]
                             // memcpy(name_wenjian,path,strlen(path));
                // strncat(name_wenjian,direntpointer->d_name,strlen(direntpointer->d_name));
                struct stat statnode;
                stat(direntpointer1->d_name, &statnode);
                if (S_ISLNK(statnode.st_mode))
                {
                   // printf("%s\t @", direntpointer->d_name);
                    continue;
                }
                // bool if_dir = !(S_ISREG(statnode.st_mode)); 外部定义过了
                //打印过程
                if (i)
                    printf("%7lu  ", statnode.st_ino);
                if (s)
                    printf("%lu  ", statnode.st_blksize);
                if (l)
                {
                    if (if_dir)
                        putchar('d');
                    else
                        putchar('-');
                    print_Permissions(statnode.st_mode);
                    //打印权限
                    printf("%2lu ", statnode.st_nlink);
                    //打印用户和组
                    char *fileUser_group = getpwuid(statnode.st_uid)->pw_name;
                    printf("%s ", fileUser_group);
                    fileUser_group == getgrgid(statnode.st_gid)->gr_name;
                    printf("%s ", fileUser_group);
                    // 打印大小
                    printf("%6ld ", statnode.st_size);
                    // time
                    char *time = ctime(&statnode.st_mtime);
                    char mtime[512] = {0};
                    strncpy(mtime, time, strlen(time) - 1);
                    printf("%10s ", mtime);
                    // printf("%s",direntpointer1->d_name);
                }
                if (if_dir)
                    printf(GREEN "%s\n" NONE, direntpointer1->d_name);
                else
                    printf("%s\n", direntpointer1->d_name);
            }
        }
        while (head != NULL) //释放my_drient的内存
        {
            direntpointer1 = head;
            head = head->next;
            free(direntpointer1);
        }
        closedir(DIRpointer);
    }
    else if (!r && t)
    {
        DIR *DIRpointer;
        struct dirent *direntpointer;

        DIRpointer = opendir(path);
        //        //int i = 0;
        my_dirent *head = NULL;
        while (!((direntpointer = readdir(DIRpointer)) == NULL))
        {
            {
                {
                    my_dirent *a = (my_dirent *)malloc(sizeof(my_dirent));
                    assert(a);
                    a->next = (head);
                    (a)->d_ino = direntpointer->d_ino;
                    memcpy((a)->d_name, direntpointer->d_name, 256);
                    (head) = a;
                }
            }
            // int i = 0;;
        }
        sortT(head);
        my_dirent *direntpointer1;

        for (direntpointer1 = head; direntpointer1 != NULL; direntpointer1 = direntpointer1->next)
        {
            // char name_wenjian [512];
            /*可以优化*/ //  memset(name_wenjian,0,512);
                         //  memcpy(name_wenjian,path,strlen(path));
                         //  strncat(name_wenjian,direntpointer->d_name,strlen(direntpointer->d_name));
            struct stat statnode;
            memset(&statnode, 0, sizeof(statnode));
            chdir(path);
            if (0 != stat(direntpointer1->d_name, &statnode))
            {
                printf("%20s\n", direntpointer1->d_name);
                puts("can't open");
                continue;
            }; //文件名不是文件路径
            if (S_ISLNK(statnode.st_mode))
            {
               // printf("%s\t @", direntpointer->d_name);
                continue;
            }
            bool if_dir = !(S_ISREG(statnode.st_mode));
            if (!(s | l)) //不需要访问stat时
            {
                if (!(i)) //单纯打翻了代码
                {
                    if (!(a)) //无i-node情况
                    {
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name); //彩色文件
                        else
                            printf("%20s\n", direntpointer1->d_name);
                    }
                    else
                    {

                        if (direntpointer1->d_name[0] == '.')
                            continue;
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name);
                        else //彩色文件
                            printf("%20s\n", direntpointer1->d_name);
                    }
                }
                else
                {
                    if (a) // 有i-node情况
                    {
                        printf("%lu  ", direntpointer1->d_ino);
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name);
                        else //彩色文件
                            printf("%20s\n", direntpointer1->d_name);
                    }
                    else
                    {
                        if (direntpointer1->d_name[0] == '.')
                            continue;
                        printf("%lu  ", direntpointer1->d_ino);
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name);
                        else //彩色文件
                            printf("%20s\n", direntpointer1->d_name);
                    }
                }
            }
            else
            { //需要stat时
                if (!(a))
                    if (direntpointer1->d_name[0] == '.')
                        continue;
                // char name_wenjian [512];
                /*可以优化*/ // memset(name_wenjian,0,512);========== 优化p [error]
                             // memcpy(name_wenjian,path,strlen(path));
                // strncat(name_wenjian,direntpointer->d_name,strlen(direntpointer->d_name));
                struct stat statnode;
                stat(direntpointer1->d_name, &statnode);
                if (S_ISLNK(statnode.st_mode))
                {
                   // printf("%s\t @", direntpointer->d_name);
                    continue;
                }
                // bool if_dir = !(S_ISREG(statnode.st_mode)); 外部定义过了
                //打印过程
                if (i)
                    printf("%7lu  ", statnode.st_ino);
                if (s)
                    printf("%lu  ", statnode.st_blksize);
                if (l)
                {
                    if (if_dir)
                        putchar('d');
                    else
                        putchar('-');
                    print_Permissions(statnode.st_mode);
                    //打印权限
                    printf("%2lu ", statnode.st_nlink);
                    //打印用户和组
                    char *fileUser_group = getpwuid(statnode.st_uid)->pw_name;
                    printf("%s ", fileUser_group);
                    fileUser_group == getgrgid(statnode.st_gid)->gr_name;
                    printf("%s ", fileUser_group);
                    // 打印大小
                    printf("%6ld ", statnode.st_size);
                    // time
                    char *time = ctime(&statnode.st_mtime);
                    char mtime[512] = {0};
                    strncpy(mtime, time, strlen(time) - 1);
                    printf("%10s ", mtime);
                    // printf("%s",direntpointer1->d_name);
                }
                if (if_dir)
                    printf(GREEN "%s\n" NONE, direntpointer1->d_name);
                else
                    printf("%s\n", direntpointer1->d_name);
            }
        }
        while (head != NULL) //释放my_drient的内存
        {
            direntpointer1 = head;
            head = head->next;
            free(direntpointer1);
        }
        closedir(DIRpointer);
    }
    else
    {
        DIR *DIRpointer;
        struct dirent *direntpointer;

        DIRpointer = opendir(path);
        //        //int i = 0;
        my_dirent *head = NULL;
        while (!((direntpointer = readdir(DIRpointer)) == NULL))
        {
            {
                {
                    my_dirent *a = (my_dirent *)malloc(sizeof(my_dirent));
                    assert(a);
                    a->next = (head);
                    (a)->d_ino = direntpointer->d_ino;
                    memcpy((a)->d_name, direntpointer->d_name, 256);
                    (head) = a;
                }
            }
            // int i = 0;;
        }
        sortTr(head);
        my_dirent *direntpointer1;

        for (direntpointer1 = head; direntpointer1 != NULL; direntpointer1 = direntpointer1->next)
        {
            // char name_wenjian [512];
            /*可以优化*/ //  memset(name_wenjian,0,512);
                         //  memcpy(name_wenjian,path,strlen(path));
                         //  strncat(name_wenjian,direntpointer->d_name,strlen(direntpointer->d_name));
            struct stat statnode;
            memset(&statnode, 0, sizeof(statnode));
            chdir(path);
            if (0 != stat(direntpointer1->d_name, &statnode))
            {
                printf("%20s\n", direntpointer1->d_name);
                puts("can't open");
                continue;
            }; //文件名不是文件路径
            if (S_ISLNK(statnode.st_mode))
            {
               // printf("%s\t @", direntpointer->d_name);
                continue;
            }
            bool if_dir = !(S_ISREG(statnode.st_mode));
            if (!(s | l)) //不需要访问stat时
            {
                if (!(i)) //单纯打翻了代码
                {
                    if (!(a)) //无i-node情况
                    {
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name); //彩色文件
                        else
                            printf("%20s\n", direntpointer1->d_name);
                    }
                    else
                    {

                        if (direntpointer1->d_name[0] == '.')
                            continue;
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name);
                        else //彩色文件
                            printf("%20s\n", direntpointer1->d_name);
                    }
                }
                else
                {
                    if (a) // 有i-node情况
                    {
                        printf("%lu  ", direntpointer1->d_ino);
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name);
                        else //彩色文件
                            printf("%20s\n", direntpointer1->d_name);
                    }
                    else
                    {
                        if (direntpointer1->d_name[0] == '.')
                            continue;
                        printf("%lu  ", direntpointer1->d_ino);
                        if (if_dir)
                            printf(GREEN "%20s\n" NONE, direntpointer1->d_name);
                        else //彩色文件
                            printf("%20s\n", direntpointer1->d_name);
                    }
                }
            }
            else
            { //需要stat时
                if (!(a))
                    if (direntpointer1->d_name[0] == '.')
                        continue;
                // char name_wenjian [512];
                /*可以优化*/ // memset(name_wenjian,0,512);========== 优化p [error]
                             // memcpy(name_wenjian,path,strlen(path));
                // strncat(name_wenjian,direntpointer->d_name,strlen(direntpointer->d_name));
                struct stat statnode;
                stat(direntpointer1->d_name, &statnode);
                if (S_ISLNK(statnode.st_mode))
                {
                   // printf("%s\t @", direntpointer->d_name);
                    continue;
                }
                // bool if_dir = !(S_ISREG(statnode.st_mode)); 外部定义过了
                //打印过程
                if (i)
                    printf("%7lu  ", statnode.st_ino);
                if (s)
                    printf("%lu  ", statnode.st_blksize);
                if (l)
                {
                    if (if_dir)
                        putchar('d');
                    else
                        putchar('-');
                    print_Permissions(statnode.st_mode);
                    //打印权限
                    printf("%2lu ", statnode.st_nlink);
                    //打印用户和组
                    char *fileUser_group = getpwuid(statnode.st_uid)->pw_name;
                    printf("%s ", fileUser_group);
                    fileUser_group == getgrgid(statnode.st_gid)->gr_name;
                    printf("%s ", fileUser_group);
                    // 打印大小
                    printf("%6ld ", statnode.st_size);
                    // time
                    char *time = ctime(&statnode.st_mtime);
                    char mtime[512] = {0};
                    strncpy(mtime, time, strlen(time) - 1);
                    printf("%10s ", mtime);
                    // printf("%s",direntpointer1->d_name);
                }
                if (if_dir)
                    printf(GREEN "%s\n" NONE, direntpointer1->d_name);
                else
                    printf("%s\n", direntpointer1->d_name);
            }
        }
        while (head != NULL) //释放my_drient的内存
        {
            direntpointer1 = head;
            head = head->next;
            free(direntpointer1);
        }
        closedir(DIRpointer);
    }

}
void print_ls_of_one(char *path)
{
    DIR *DIRpointer;
    struct dirent *direntpointer;
    // DIRpointer = opendir(path);
    if (!(DIRpointer = opendir(path)))
        ; //{printf("wrong!");return;}
          // 建立dirent*数组存放 dirent 结果体
    while (!((direntpointer = readdir(DIRpointer)) == NULL))
    {
        // char name_wenjian [512];
        /*可以优化*/ //  memset(name_wenjian,0,512);
                     //  memcpy(name_wenjian,path,strlen(path));
                     //  strncat(name_wenjian,direntpointer->d_name,strlen(direntpointer->d_name));
        struct stat statnode;
        memset(&statnode, 0, sizeof(statnode));
        chdir(path);
        if (0 != stat(direntpointer->d_name, &statnode))
        {
            printf("%20s\n", direntpointer->d_name);
            puts("can't open");
            continue;
        }; //文件名不是文件路径
        if (S_ISLNK(statnode.st_mode))
        {
           // printf("%s\t @", direntpointer->d_name);
            continue;
        }
        bool if_dir = !(S_ISREG(statnode.st_mode));
        if (!(s | l)) //不需要访问stat时
        {
            if ((~i)) //单纯打翻了代码
            {
                if (a) //无i-node情况
                {
                    if (if_dir)
                        printf(GREEN "%20s\n" NONE, direntpointer->d_name); //彩色文件
                    else
                        printf("%20s\n", direntpointer->d_name);
                }
                else
                {

                    if (direntpointer->d_name[0] == '.')
                        continue;
                    if (if_dir)
                        printf(GREEN "%20s\n" NONE, direntpointer->d_name);
                    else //彩色文件
                        printf("%20s\n", direntpointer->d_name);
                }
            }
            else
            {
                if (a) // 有i-node情况
                {
                    printf("%lu  ", direntpointer->d_ino);
                    if (if_dir)
                        printf(GREEN "%20s\n" NONE, direntpointer->d_name);
                    else //彩色文件
                        printf("%20s\n", direntpointer->d_name);
                }
                else
                {
                    if (direntpointer->d_name[0] == '.')
                        continue;
                    printf("%lu  ", direntpointer->d_ino);
                    if (if_dir)
                        printf(GREEN "%20s\n" NONE, direntpointer->d_name);
                    else //彩色文件
                        printf("%20s\n", direntpointer->d_name);
                }
            }
        }
        else
        { //需要stat时
            if (!(a))
                if (direntpointer->d_name[0] == '.')
                    continue;
            // char name_wenjian [512];
            /*可以优化*/ // memset(name_wenjian,0,512);========== 优化p [error]
                         // memcpy(name_wenjian,path,strlen(path));
            // strncat(name_wenjian,direntpointer->d_name,strlen(direntpointer->d_name));
            struct stat statnode;
            stat(direntpointer->d_name, &statnode);
            if (S_ISLNK(statnode.st_mode))
            {
               // printf("%s\t @", direntpointer->d_name);
                continue;
            }

            bool if_dir = !(S_ISREG(statnode.st_mode));
            if(!S_ISREG(statnode.st_mode)||if_dir){
                continue;
            }
            //打印过程
            if (i)
                printf("%7lu  ", statnode.st_ino);
            if (s)
                printf("%lu  ", statnode.st_blksize);
            if (l)
            {
                if (if_dir)
                    putchar('d');
                else
                    putchar('-');
                print_Permissions(statnode.st_mode);
                //打印权限
                printf("%2lu ", statnode.st_nlink);
                //打印用户和组
                char *fileUser_group = getpwuid(statnode.st_uid)->pw_name;
                printf("%s ", fileUser_group);
                fileUser_group == getgrgid(statnode.st_gid)->gr_name;
                printf("%s ", fileUser_group);
                // 打印大小
                printf("%6ld ", statnode.st_size);
                // time
                char *time = ctime(&statnode.st_mtime);
                char mtime[512] = {0};
                strncpy(mtime, time, strlen(time) - 1);
                printf("%10s ", mtime);
                // printf("%s",direntpointer->d_name);
            }
            if (if_dir)
                printf(GREEN "%s\n" NONE, direntpointer->d_name);
            else
                printf("%s\n", direntpointer->d_name);
        }
    }
    closedir(DIRpointer);
}
void print_ls_of_one_RRR(char *path);
void print_ls_FOR_R(char *path, char *a)
{
    if (a)
        printf(GREEN "\n\n%s:> \n" NONE, a);
    if (t | r)
    {
        print_ls_of_one_trtrtr(path);
    }
    else
    {
        print_ls_of_one(path);
    }
    printf("\n");
}

void print_ls_of_one_RRR(char *path)
{
    chdir(path);
    DIR *DIRpointer;
    struct dirent *direntpointer;
    if (!(DIRpointer = opendir(path)))
    {
        printf("%s!\n 权限不够",path);
        return;
    }
    struct stat node;
    memset(&node, 0, sizeof(node));
    int i = 0;
    struct stack_dirent stack = {NULL, NULL, 10};
    while (!((direntpointer = readdir(DIRpointer)) == NULL))
    {
        if (*(direntpointer->d_name) == '.')
            continue;
        stat(direntpointer->d_name, &node);
        if (S_ISDIR(node.st_mode))
        {
            i++;
            stack_iniput(&stack, direntpointer);
        }
    }
    rewinddir(DIRpointer);
    print_ls_FOR_R(path,basename(path));
    if(i != 0){ closedir(DIRpointer) ;}
    char *a;
    char path2[PATH_MAX] = {0};
    memcpy(path2, path, PATH_MAX);
    if (i == 0)
    {
        // R = 0;
        //  print_ls_FOR_R(path,0);
        closedir(DIRpointer);
        return;
    }
    else
    {
        while (i--)
        {
            strcat(path2, "/");
            a = strcat(path2, stack_output(&stack)->d_name);
            print_ls_of_one_RRR(a);
            memcpy(path2, path, 4096);
        }
//        closedir(DIRpointer);
        stack_output(&stack);
    }

    // free(stack.end);
}

int main(int argc, char **argv)
{   //还没有加closedir()
    // 记得加!!!

//    statnode = (struct stat *)malloc(sizeof(struct stat));
    if (argc == 1) //只运行
    {
        char pathname[PATH_MAX] = {0};
        getcwd(pathname, sizeof(pathname));
        print_ls("/boot");
    }
    if (argc == 2)
    {
        if (*(argv[1]) == '-')
        { // 运行 只加参数
            if (strstr(argv[1], "a"))
                a = 1;
            if (strstr(argv[1], "R"))
                R = 1;
            if (strstr(argv[1], "t"))
                t = 1;
            if (strstr(argv[1], "l"))
                l = 1;
            if (strstr(argv[1], "i"))
                i = 1;
            if (strstr(argv[1], "s"))
                s = 1;
            if (strstr(argv[1], "r"))
                r = 1;
            char pathname[PATH_MAX] = {0};
            getcwd(pathname, sizeof(pathname));
            print_ls(pathname);
        }
        else
        { // 运行 只有路径
            char *pathname = strdup(argv[1]);
            assert(pathname =(char * ) realloc(pathname, PATH_MAX));
            print_ls(pathname);
            free(pathname);
        }
    }
    else if (argc == 3) // 运行 都有
    {
        if (*(argv[1]) == '-') //参数在前
        {
            if (strstr(argv[1], "R"))
                R = 1;
            if (strstr(argv[1], "a"))
                a = 1;
            if (strstr(argv[1], "t"))
                t = 1;
            if (strstr(argv[1], "l"))
                l = 1;
            if (strstr(argv[1], "i"))
                i = 1;
            if (strstr(argv[1], "s"))
                s = 1;
            if (strstr(argv[1], "r"))
                r = 1;
            char *pathname = strdup(argv[2]);
            assert(pathname =(char * ) realloc(pathname, PATH_MAX));
            print_ls(pathname);
            free(pathname);
        }
        else
        { // 路径在前
            if (strstr(argv[2], "R"))
                R = 1;
            if (strstr(argv[2], "a"))
                a = 1;
            if (strstr(argv[2], "t"))
                t = 1;
            if (strstr(argv[2], "l"))
                l = 1;
            if (strstr(argv[2], "i"))
                i = 1;
            if (strstr(argv[2], "s"))
                s = 1;
            if (strstr(argv[2], "r"))
                r = 1;
            char *pathname = strdup(argv[1]);
            assert(pathname =(char * ) realloc(pathname, PATH_MAX));
            print_ls(pathname);
            free(pathname);
        }
    }
//    free(statnode);
}