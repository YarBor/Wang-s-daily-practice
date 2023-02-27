#include "hotelManageSys.h"

int main()
{
    struct Node *head = create_dummy_node();
    // // tailInsert(head,create_node(10004,"vip",0,150,NULL,NULL));
    // // tailInsert(head,create_node(10001,"normal",1,100,"22/2/2","22/3/3"));
    // // tailInsert(head,create_node(10003,"normal",1,100,"22/6/6","22/7/7"));
    // // tailInsert(head,create_node(10002,"normal",1,100,"22/4/4","22/5/5"));
    // filein("wang",head);
    // sort(head);
    // // fileout("wang",head);
    // print_list(head);
    // destory(head);
    // free(head);
    // printf("%d", init());

    char usrname[64] = {0};
    while (!INIT(usrname, head))
        ;

    while (1)
    {
        viewList();
        // while('\n'!=getchar());
        fflush(stdin);
        char a;

    begin:
        scanf("%c", &a);
        fflush(stdin);
        // while('\n'!=getchar());
        // while(EOF!=getchar());
        // while ('\n' != getchar())
        // ;
        switch (a)
        {
        case '1':
            add(head);
            break;

        case '2':
            int num;
            printf("-1 to quit\ninput roomNumber:>");
            scanf("%d", &num);
            // while ('\n' != getchar())
            // ;
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
            break;
        case 'q':
            goto end;
        default:
            goto begin;
        }
        // while (getchar() != '\n')
        // ;
    }
end:

    fileout(usrname, head);
    destory(head);
}