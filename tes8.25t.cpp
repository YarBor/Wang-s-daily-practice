#include "БъЭЗ.h"
#define MUBIAO 2
// Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode* next;
};

int main() {
    struct ListNode* a = (struct ListNode*)malloc(sizeof(struct ListNode));
    if(!a)
    *a = {12,NULL };
   free(a);
    return 0;
}