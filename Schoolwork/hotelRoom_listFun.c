#include "hotelManageSys.h"

// 定义链表节点结构体
struct Node
{
    struct Room *room;
    struct Node *next;
};

// 定义房间结构体
// struct Room {
// int room_number;
// char room_type[20];
// bool is_available;
// double room_price;
// char check_in_time[20];
// char check_out_time[20];
// };

// 创建哨兵节点
struct Node *create_dummy_node()
{
    struct Node *dummy = (struct Node *)malloc(sizeof(struct Node));
    dummy->next = NULL;
    dummy->room = NULL;
    return dummy;
}

// 创建房间节点
struct Node *create_node(int room_number, char room_type[], bool is_available, double room_price, char check_in_time[], char check_out_time[])
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    memset(new_node, 0, sizeof(struct Node));
    struct Room *new_room = (struct Room *)malloc(sizeof(struct Room));
    memset(new_room, 0, sizeof(struct Room));

    new_room->room_number = room_number;
    if (room_type)
        strcpy(new_room->room_type, room_type);
    new_room->is_available = is_available;
    new_room->room_price = room_price;
    if (check_in_time)
        strcpy(new_room->check_in_time, check_in_time);
    if (check_out_time)
        strcpy(new_room->check_out_time, check_out_time);

    new_node->room = new_room;
    new_node->next = NULL;

    return new_node;
}
struct Node *headDel(struct Node *head) // shao bing
{
    if ((head)->next == NULL)
    {
        return head;
    }
    struct Node *tmp = head->next;
    head->next = head->next->next;
    free(tmp->room);
    free(tmp);
    return head;
}

struct Node *destory(struct Node *head) // 销毁后 head 是 野指针
{
    while (headDel(head)->next)
        ;
    free(head);
    return head;
}
struct Node *find(struct Node *head, int room_number, bool *if_tail)
{
    struct Node *tmp1 = head;
    struct Node *tmp2 = head->next;
    while (tmp2->room->room_number != room_number || (!tmp2))
    {
        tmp1 = tmp1->next;
        tmp2 = tmp2->next;
    }
    if (!tmp2)
        return NULL;
    else if (!(tmp2->next))
        *if_tail = true;
    return tmp1;
} // 返回找到节点的前一个节点 如是尾节点 则标识位为true 未找到返回 null
// 删除特定节点
struct Node *delete(struct Node *head, int room_number)
{
    bool Tail = false;
    struct Node *tmp;
    if (tmp = find(head, room_number, &Tail))
        ;
    else
    {
        puts("no found");
        return head;
    }
    if (Tail)
    {
        free(tmp->next->room);
        free(tmp->next);
        tmp->next = NULL;
    }
    else
    {
        struct Node *tmp2 = tmp->next;
        tmp->next = tmp->next->next;
        free(tmp2->room);
        free(tmp2);
    }
    return head;
}
struct Node *findForChange(struct Node *head, int room_number)
{
    bool a;
    return find(head, room_number, &a);
}
// 修改特定节点 // 带交互
struct Node *change(struct Node *head, int room_number)
{
    struct Node *tmp = findForChange(head, room_number);
    if (!tmp)
    {
        puts("wrong Room_number");
        return NULL;
    }
    puts("exchange what?");
    char buffer[64];
    memset(buffer, 0, 64);
    int choice = chooselist(); /////undone
    switch (choice)
    {
    case ROOM_NUMBER:
        puts("input:> num or '-1'for quit");
        int aa = 0;
        scanf("%d", &aa);
        if (aa == -1)
            break;
        else
            tmp->next->room->room_number = aa;
        break;
    case ROOM_TYPE:
        while (1)
        {
            puts("input:> string( letters <= 19 ) or 'q'for quit");
            scanf("%63s", buffer);
            if (strlen(buffer) > 19)
            {
                puts("wrong type again please");
                memset(buffer, 0, 64);
                continue;
            }
            else if (!(strcmp(buffer, "q")))
            {
                return head;
            }
            else
            {
                sprintf(tmp->next->room->room_type, "%s", buffer);
                break;
            }
        }
        break;
    case IS_AVAILABLE:
        // scanf("%19s", tmp->room->room_type);
        puts("input:> Y/N or 'q'for quit");
        while (1)
        {
            scanf("%s", buffer);
            if (!strcmp(buffer, "Y"))
            {
                tmp->next->room->is_available = 1;
                break;
            }
            else if (!strcmp(buffer, "N"))
            {
                tmp->next->room->is_available = 0;
                break;
            }
            else if (!strcmp(buffer, "q"))
                return head;
            else
                puts("wrong input, again plese");
        }
        break;
    case ROOM_PRICE:
        while (1)
        {
            double num;
            puts("input:> number or '-1'for quit");
            scanf("%lf", &num);
            if (num < -1)
            {
                puts("wrong number , again please");
                continue;
            }
            else if (num == -1)
            {
                return head;
            }
            else
            {
                tmp->next->room->room_price = num;
                break;
            }
        }
        break;
    case CHECK_IN_TIME:
        while (1)
        {
            puts("input:> time ( letters <= 19 ) or 'q'for quit");
            scanf("%63s", buffer);
            if (strlen(buffer) > 19)
            {
                puts("too long , again please");
                memset(buffer, 0, 64);
                continue;
            }
            else if (!(strcmp(buffer, "q")))
            {
                return head;
            }
            else
            {
                sprintf(tmp->next->room->check_in_time, "%s", buffer);
                break;
            }
        }
        break;
    case CHECK_OUT_TIME:
        while (1)
        {
            puts("input:> time ( letters <= 19 ) or 'q'for quit");
            scanf("%63s", buffer);
            if (strlen(buffer) > 19)
            {
                puts("too long , again please");
                memset(buffer, 0, 64);
                continue;
            }
            else if (!(strcmp(buffer, "q")))
            {
                return head;
            }
            else
            {
                sprintf(tmp->next->room->check_out_time, "%s", buffer);
                break;
            }
        }
        break;
    default:
        return head;
    }
    return head;
}
// 打印
void print_list(struct Node *dummy)
{
    struct Node *cur = dummy->next;
    while (cur != NULL)
    {
        printf("Room Number: %d\n", cur->room->room_number);
        printf("Room Type: %s\n", cur->room->room_type);
        printf("Is Available: %d\n", cur->room->is_available);
        printf("Room Price: %.2lf$\n", cur->room->room_price);
        printf("Check-in Time: %s\n", cur->room->check_in_time);
        printf("Check-out Time: %s\n\n", cur->room->check_out_time);
        cur = cur->next;
    }
}

int chooselist()
{
    puts("*************************");
    puts("1.     room_number       ");
    puts("2.     room_type         ");
    puts("3.     is_available      ");
    puts("4.     room_price        ");
    puts("5.      in_time          ");
    puts("6.      out_time         ");
    puts("                         ");
    puts("      'q' for quit       ");
    puts("*************************\ninput:>");
    int a;
    scanf("%d", &a);
    // //while (getchar() != '\n')
    ;
    return a;
}

struct Node *tailInsert(struct Node *head, struct Node *node)
{
    struct Node *tmp = head;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = node;
    return head;
}

void swap(struct Node *a, struct Node *b)
{
    struct Node tmp;
    tmp.room = a->room;
    a->room = b->room;
    b->room = tmp.room;
    // memcpy(&tmp, a, sizeof(struct Node));
    // memcpy(a, b, sizeof(struct Node));
    // memcpy(b, &tmp, sizeof(struct Node));
}
struct Node *sort(struct Node *head)
{
    puts("sort by what?");
    int flag = chooselist();
    if (flag == -1)
        return head;
    for (struct Node *aaa = head->next; aaa != NULL; aaa = aaa->next)
    {
        struct Node *a = head->next;
        for (struct Node *aa = head->next->next; aa != NULL; aa = aa->next, a = a->next)
        {
            switch (flag)
            {
            case ROOM_NUMBER:
                if (a->room->room_number > aa->room->room_number)
                    swap(a, aa);
                break;
            case ROOM_TYPE:
                if (strcmp(a->room->room_type, aa->room->room_type) <= 0)
                    swap(a, aa);
                break;
            case IS_AVAILABLE:
                if (a->room->is_available < aa->room->is_available)
                    swap(a, aa);
                break;
            case ROOM_PRICE:
                if (a->room->room_price > aa->room->room_price)
                    swap(a, aa);
                break;
            case CHECK_IN_TIME:
                if (strcmp(a->room->check_in_time, aa->room->check_in_time) >= 0)
                    swap(a, aa);
                break;
            case CHECK_OUT_TIME:
                if (strcmp(a->room->check_out_time, aa->room->check_out_time) >= 0)
                    swap(a, aa);
                break;
            default:
                break;
            }
        }
    }
    return head;
}

void search_print(struct Node *head)
{
    int num = -1;
    while (1)
    {
        puts("-1 to quit\ninput roomNumber:>");
        scanf("%d", &num);
        if (num == -1)
            return;
        struct Node *cur = findForChange(head, num);
        if (!cur)
        {
            puts("no found , wrong number , again?");
            continue;
        }
        cur = cur->next;
        printf("Room Number: %d\n", cur->room->room_number);
        printf("Room Type: %s\n", cur->room->room_type);
        printf("Is Available: %d\n", cur->room->is_available);
        printf("Room Price: %.2lf$\n", cur->room->room_price);
        printf("Check-in Time: %s\n", cur->room->check_in_time);
        printf("Check-out Time: %s\n\n", cur->room->check_out_time);
        break;
    }
}

struct Node *filein(char *usrname, struct Node *head)
{
    FILE *fd;
    fd = fopen(usrname, "rb");
    int room_number;               // 房号
    char room_type[20] = {0};      // 房间种类
    int is_available;              // 是否居住
    double room_price;             // 房间价格
    char check_in_time[20] = {0};  // 入住时间
    char check_out_time[20] = {0}; // 交房时间

    while (EOF != fscanf(fd, "%d %s %d %lf",
                         &room_number, room_type, &is_available,
                         &room_price //, check_in_time, check_out_time))
                         ))
    {
        if (is_available)
            fscanf(fd, "%s %s\n", check_in_time, check_out_time);
        else
        {
            memset(check_in_time, 0, 20);
            memset(check_out_time, 0, 20);
        }
        tailInsert(head, create_node(room_number, room_type,
                                     is_available, room_price,
                                     check_in_time, check_out_time));
    }
    fclose(fd);
    return head;
}
struct Node *fileout(char *usrname, struct Node *head)
{
    FILE *fd;
    if (!(fd = fopen(usrname, "wb+")))
    {
        puts("open file wrong");
        return head;
    }
    struct Node *tmp = head->next;
    while (tmp)
    {
        fprintf(fd, "%d %s %d %.2lf %s %s\n", tmp->room->room_number, tmp->room->room_type, tmp->room->is_available, tmp->room->room_price, tmp->room->check_in_time, tmp->room->check_out_time);
        tmp = tmp->next;
    }
    fclose(fd);
    return head;
}

int init()
{
    printf("----------------------------\n");
    printf("-----HotelManageSystem------\n");
    printf("----------------------------\n");
    printf("--------(1)-sign-up---------\n");
    printf("--------(2)-log--in---------\n");
    printf("----------------------------\n");
    printf("--------q for quit----------\n");
    char a;
begin:
    scanf("%c", &a);
    while ('\n' != getchar())
        ;

    if (a == 'q')
        return -1;
    else if (a == '1')
        return 1;
    else if (a == '2')
        return 2;
    else
    {
        goto begin;
        // puts("wrong num,again please");
        // while (getchar() != '\n')
        ;
        return init();
    }
}

bool usrnameAndPassword(const char *usrname)
{
    FILE *fd = fopen("usrnameAndPassword", "rb+");
    char name[64] = {0};
    char password[64] = {0};
    char buffer[64] = {0};
    bool flag = false;
    while (EOF != fscanf(fd, "%s %s", name, password))
    {
        if (strcmp(name, usrname) == 0)
        {
            puts("enter the passward:>");
            scanf("%s", buffer);
            if (strcmp(buffer, password) == 0)
                return true;
            else
            {
                puts("wrong ! 2 more times");
                puts("enter the passward:>");
                scanf("%s", buffer);
                if (strcmp(buffer, password) == 0)
                    return true;
                else
                {
                    puts("wrong ! 1 more times");
                    puts("enter the passward:>");
                    scanf("%s", buffer);
                    if (strcmp(buffer, password) == 0)
                        return true;
                    else
                    {
                        puts("wrong!");
                        return false;
                    }
                }
            }
        }
    }
    puts("wrong user name");
    fclose(fd);
    return false;
}

bool singup(char *username)
{
    char buffer[64] = {0};
    FILE *fd = fopen("usrnameAndPassword", "ab+");
    if (!fd)
        return 0;
    puts("usrName :>");
    scanf("%s", buffer);
    memcpy(username, buffer, 64);
    fwrite(buffer, 1, strlen(buffer), fd);
    fwrite(" ", 1, 1, fd);
    puts("password :>");
    scanf("%s", buffer);
    fwrite(buffer, 1, strlen(buffer), fd);
    fwrite("\n", 1, 1, fd);
    fclose(fd);
    return 1;
}

bool login(char *usrname, struct Node *head)
{
    puts("enter the usrname");
    scanf("%s", usrname);
    if (usrnameAndPassword(usrname))
    {
        filein(usrname, head);
        return true;
    }
    return false;
}

struct Node *INIT(char *usrname, struct Node *head)
{
    int a = init();
    if (a == 1)
        singup(usrname);
    else if (a == 2)
        if (login(usrname, head))
            return head;
        else
            return NULL;
    else if (a == -1)
        exit(0);
}

void viewList()
{
    printf("|========================|\n");
    printf("|1)---------add----------|\n");
    printf("|2)--------delete--------|\n");
    printf("|3)--------search--------|\n");
    printf("|4)-------view all-------|\n");
    printf("|5)---------sort---------|\n");
    printf("|------------------------|\n");
    printf("|q)---------quit---------|\n");
}

struct Node *add(struct Node *head)
{
    int room_number;               // 房号
    char room_type[20] = {0};      // 房间种类
    int is_available;              // 是否居住
    double room_price;             // 房间价格
    char check_in_time[20] = {0};  // 入住时间
    char check_out_time[20] = {0}; // 交房时间
    printf("room_number:>");
    scanf("%d", &room_number);
    printf("room_type:>");
    scanf("%s", room_type);
    printf("is_avaliable:>");
    scanf("%d", &is_available);
    printf("price:>");
    scanf("%lf", &room_price);
    printf("check_in_time:>");
    scanf("%s", check_in_time);
    printf("check_out_time:>");
    scanf("%s", check_out_time);

    return tailInsert(head, create_node(room_number, room_type, is_available, room_price, check_in_time, check_out_time));
};
