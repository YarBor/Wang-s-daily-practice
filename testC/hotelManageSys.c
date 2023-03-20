#include"hotelManageSys.h"

int main(){
    struct Node * head = create_dummy_node();
    // tailInsert(head,create_node(10004,"vip",0,150,NULL,NULL));
    // tailInsert(head,create_node(10001,"normal",1,100,"22/2/2","22/3/3"));
    // tailInsert(head,create_node(10003,"normal",1,100,"22/6/6","22/7/7"));
    // tailInsert(head,create_node(10002,"normal",1,100,"22/4/4","22/5/5"));
    filein("wang",head);
    sort(head);
    // fileout("wang",head);
    print_list(head);
    destory(head);
    free(head);
}