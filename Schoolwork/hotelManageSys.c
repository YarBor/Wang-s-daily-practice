#include "hotelManageSys.h"

int main()
{
    struct Node *head = create_dummy_node();
    char usrname[64] = {0};
    while (!INIT(usrname, head))
        ;
    while (1)
    {
        viewList();
        fflush(stdin);
        char a;

    begin:
        scanf("%c", &a);
        fflush(stdin);
        switch (a)
        {
        case '1':
            add(head);
            break;

        case '2':
            int num;
            printf("-1 to quit\ninput roomNumber:>");
            scanf("%d", &num);
            if (num == -1)
                continue;
            delete (head, num);
            break;
        case '3':
            search_print(head);
            break;
        case '4':
            print_list(head);
            break;
        case '5':
            sort(head);
            print_list(head);
            break;
        case '6':
            for_change(head);
            break;
        case 'q':
            goto end;
        default:
            goto begin;
        }
    }
end:
    fileout(usrname, head);
    destory(head);
}