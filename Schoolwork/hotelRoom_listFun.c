#include "hotelManageSys.h"

// 定义链表节点结构体
// struct Node
// {
//     struct Room *room;
//     struct Node *next;
// };

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

struct Node *destory(struct Node *head)
{
    while (head->next != NULL)
    {
        head = headDel(head);
    }
    free(head);
    return NULL;
}

struct Node *find(struct Node *head, int room_number, bool *if_tail)
{
    struct Node *tmp1 = head;
    struct Node *tmp2 = head->next;
    while ((tmp2) && tmp2->room->room_number != room_number)
    {
        tmp1 = tmp1->next;
        tmp2 = tmp2->next;
    }
    if (!tmp2)
    {
        return NULL;
    }
    else if (!(tmp2->next))
    {
        *if_tail = true;
    }

    return head;
}

// 删除特定节点
struct Node *delete(struct Node *head, int room_number)
{
    bool Tail = false;
    struct Node *tmp;
    if (tmp = find(head, room_number, &Tail))
        ; // Check whether we found the node in the list
    else
    {
        puts("no found");
        return head;
    }

    if (Tail) // If the node is at the end of the list
    {
        free(tmp->next->room); // Free the node's data area
        free(tmp->next);       // Free the node
        tmp->next = NULL;
    }
    else // If the node is not at the end of the list
    {
        struct Node *tmp2 = tmp->next; // Store a reference to the node we want to delete
        tmp->next = tmp->next->next;   // Set this node's pointer to the next next node
        free(tmp2->room);              // Free the data area of the deleted node
        free(tmp2);                    // Free the deleted node itself
    }
    return head; // Return the head of the list
}

struct Node *findForChange(struct Node *head, int room_number)
{
    bool a;
    return find(head, room_number, &a);
}

// Adding comments for the code begins here
void for_change(struct Node *head)
{
    puts("-1 or Invaild symbol will quit \ninput the target RoomNumber:>");
    int room_number;
    scanf("%d", &room_number);
    while ('\n' != getchar())
        ;

    if (room_number == -1)
        return;
    else
        change(head, room_number);
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
        puts("-1 or Invaild symbol will quit \ninput the target RoomNumber:>");
        int aa = -1;
        scanf("%d", &aa);
        while ('\n' != getchar())
            ;
        if (aa == -1)
            break;
        else
            tmp->next->room->room_number = aa;
        break;
    case ROOM_TYPE:
        while (1)
        {
        room_type_goto_because_buffer_out:
            puts("input:> string( letters <= 19 ) or 'q'for quit");
            scanf("%63s", buffer);
            while ('\n' != getchar())
                ;
            if (strlen(buffer) > 19)
            {
                puts("wrong type again please");
                memset(buffer, 0, 64);
                // continue;
                goto room_type_goto_because_buffer_out;
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
        puts("input:> Y/N or 'q'for quit");
        while (1)
        {
            scanf("%s", buffer);
            while ('\n' != getchar())
                ;
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
            double num = 0;
            puts("-1 or Invaild symbol will quit \ninput the target RoomNumber:>");
            scanf("%lf", &num);
            while ('\n' != getchar())
                ;
            if (num < -1)
            {
                puts("wrong number , again please");
                continue;
            }
            else if (0 == num)
            {
                puts("input number please");
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
        room_IN_TIME_goto_because_buffer_out:
            puts("input:> String of time ( letters <= 19 ) or 'q'for quit");
            scanf("%63s", buffer);
            while ('\n' != getchar())
                ;
            if (strlen(buffer) > 19)
            {
                puts("too long , again please");
                memset(buffer, 0, 64);
                // continue;
                goto room_IN_TIME_goto_because_buffer_out;
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
        room_OUT_TIME_goto_because_buffer_out:
            puts("input:> time ( letters <= 19 ) or 'q'for quit");
            scanf("%63s", buffer);
            while ('\n' != getchar())
                ;
            if (strlen(buffer) > 19)
            {
                puts("too long , again please");
                memset(buffer, 0, 64);
                goto room_OUT_TIME_goto_because_buffer_out;
                // continue;
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
    if (!cur)
    {
        puts("!No data!");
        return;
    }
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
    while (getchar() != '\n')
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
}
struct Node *sort(struct Node *head)
{
    puts("sort by what?");
    int flag = chooselist();
    if (flag == -1)
        return head;
    char for_order;

begin:
    puts("ascending OR descending?\n(1/2)[q to quit] :>");
    scanf("%c", &for_order);
    while ('\n' != getchar())
        ;
    if (for_order == '1' || for_order == '2' || for_order == 'q')
        if (for_order == 'q')
            return head;
        else
            ;
    else
    {
        puts("wrong choice,again");
        goto begin;
    }
    for (struct Node *aaa = head->next; aaa != NULL; aaa = aaa->next)
    {
        struct Node *a = head->next;
        for (struct Node *aa = head->next->next; aa != NULL; aa = aa->next, a = a->next)
        {
            switch (flag)
            {
            case ROOM_NUMBER:
                if (for_order == '1' ? a->room->room_number > aa->room->room_number
                                     : a->room->room_number < aa->room->room_number)
                    swap(a, aa);
                break;
            case ROOM_TYPE:
                if (for_order == '1' ? strcmp(a->room->room_type, aa->room->room_type) < 0
                                     : strcmp(a->room->room_type, aa->room->room_type) > 0)
                    swap(a, aa);
                break;
            case IS_AVAILABLE:
                if (for_order == '1' ? a->room->is_available < aa->room->is_available
                                     : a->room->is_available > aa->room->is_available)
                    swap(a, aa);
                break;
            case ROOM_PRICE:

                if (for_order == '1' ? a->room->room_price > aa->room->room_price
                                     : a->room->is_available < aa->room->is_available)

                    swap(a, aa);
                break;
            case CHECK_IN_TIME:
                if (for_order == '1' ? strcmp(a->room->check_in_time, aa->room->check_in_time) > 0
                                     : strcmp(a->room->check_in_time, aa->room->check_in_time) < 0)

                    swap(a, aa);
                break;
            case CHECK_OUT_TIME:
                if (for_order == '1' ? strcmp(a->room->check_out_time, aa->room->check_out_time) > 0
                                     : strcmp(a->room->check_out_time, aa->room->check_out_time) < 0)
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
        puts("-1 to quit___and___InvaildSymbol also quit\ninput roomNumber:>");
        scanf("%d", &num);
        while ('\n' != getchar())
            ;
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

    FILE *fd = fopen(strcat(usrname,".data"), "rb");
    if (!fd)
    {
        puts("open file error");
        exit(1);
    }
    char buffer[sizeof(struct Room)] = {0};
    while (fread(buffer, sizeof(struct Room), 1, fd))
    {
        struct Node *node = (struct Node *)malloc(sizeof(struct Node));
        node->room = (struct Room *)malloc(sizeof(struct Room));
        node->next = NULL;
        memcpy(node->room, buffer, sizeof(struct Room));
        tailInsert(head, node);
    }
    fclose(fd);
    return head;
}

struct Node *fileout(char *usrname, struct Node *head)
{
    FILE *fd;
    if (!(fd = fopen(strcat(usrname,".data"), "wb")))
    {
        puts("open file wrong");
        return head;
    }
    struct Node *tmp = head->next;
    while (tmp)
    {
        fwrite(tmp->room, sizeof(struct Room), 1, fd);
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
    FILE *fd = fopen("usrnameAndPassword", "rb");
    struct usrPassWord tmp;
    int flag = 0;
    char buffer[128];
    while (fread(&tmp, sizeof(struct usrPassWord), 1, fd))
    {
        if (strcmp(tmp.usrname, usrname) == 0)
        {
            flag = 1;
            for (int a = 0; a < 3; a++)
            {
                puts("input passwords[letters < 63]:>");
                scanf("%s", buffer);
                if (strlen(buffer) > 63)
                {
                    printf("wrong passord , %d more times", 2 - a);
                    continue;
                }
                else if (strcmp(buffer, tmp.password) == 0)
                    return true;
                else
                {
                    printf("wrong passord , %d more times", 2 - a);
                    continue;
                }
            }
        }
    };
    if(flag==0){
        puts("wrong usrname");
    }
    return false;
}

// Add comment to singup function
bool singup(char *username)
{
    char buffer[128] = {0};
    struct usrPassWord a;
    FILE *fd = fopen("usrnameAndPassword", "ab");
    if (!fd)
        return 0;

    puts("usrName [letters < 63]:>");
    scanf("%s", buffer);
    while ('\n' != getchar())
        ;
    memcpy(username, buffer, 64);
    memcpy(a.usrname, username, 64);

    puts("password [letters < 63]:>");
    scanf("%s", buffer);
    while ('\n' != getchar())
        ;
    memcpy(a.password, buffer, 64);

    fwrite(&a, sizeof(struct usrPassWord), 1, fd);
    fclose(fd);
    return 1;
}

bool login(char *usrname, struct Node *head)
{
    puts("enter the usrname");
    scanf("%s", usrname);
    while ('\n' != getchar())
        ;

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
    printf("|6)--------change--------|\n");
    printf("|------------------------|\n");
    printf("|q)---------quit---------|\n");
}

struct Node *add(struct Node *head)
{
    int room_number = -1;          // 房号
    char room_type[20] = {0};      // 房间种类
    int is_available = -1;         // 是否居住
    double room_price = -1;        // 房间价格
    char check_in_time[20] = {0};  // 入住时间
    char check_out_time[20] = {0}; // 交房时间
room_number_input_fail:
    printf("room_number:>");
    scanf("%d", &room_number);
    while ('\n' != getchar())
        ;
    if (room_number == -1)
    {
        puts("wrong number,again");
        goto room_number_input_fail;
    }

    printf("room_type:>");
    scanf("%s", room_type);
    while ('\n' != getchar())
        ;

room_is_available_input_fail:
    printf("is_avaliable(0/1):>");
    scanf("%d", &is_available);
    while ('\n' != getchar())
        ;
    if (is_available != 0 && is_available != 1)
    {
        puts("wrong number,again");
        goto room_is_available_input_fail;
    }

room_price_input_fail:
    printf("price:>");
    scanf("%lf", &room_price);
    while ('\n' != getchar())
        ;
    if (room_price == -1)
    {
        puts("wrong number , again");
        goto room_price_input_fail;
    }

    printf("check_in_time:>");
    scanf("%s", check_in_time);

    while ('\n' != getchar())
        ;
    printf("check_out_time:>");
    scanf("%s", check_out_time);

    while ('\n' != getchar())
        ;
    return tailInsert(head, create_node(room_number, room_type, is_available, room_price, check_in_time, check_out_time));
};
