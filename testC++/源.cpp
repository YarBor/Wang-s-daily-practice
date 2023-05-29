#include "??Í·.h"
#define MUBIAO 2
// Definition for singly-linked list.
struct ListNode
{
    int val;
    struct ListNode *next;
};

struct ListNode *removeNthFromEnd(struct ListNode *head, int n);
struct ListNode e = {5, NULL};
struct ListNode d = {4, &e};
struct ListNode c = {3, &d};
struct ListNode b = {2, &c};
struct ListNode a = {1, &b};
struct ListNode **point;
int main()
{
    int n = MUBIAO;
    removeNthFromEnd(&a, n);
    return 0;
}

struct ListNode *removeNthFromEnd(struct ListNode *head, int n)
{
    point = (struct ListNode **)malloc(sizeof(struct ListNode **));
    struct ListNode *u = (struct ListNode *)malloc(sizeof(struct ListNode *));
    u = head;
    //  struct ListNode* u = head;
    int e = 0;
    while (1)
    {
        *(point + e) = (struct ListNode *)malloc(sizeof(struct ListNode *));
        if (!*(point + e))
        {
            *(point + e) = u;
        }
        u = u->next;
        if (!u)
            break;
        e++;
    }
    if (e + 1 == 2) //Á½??
    {
        if (n == 1)
        {
            free(point[1]);
            point[0]->next = NULL;
            return point[0];
        }
        if (n == 2)
        {
            free(head);
            return point[1];
        }
    }
    u = point[e - n + 1];
    point[e - n]->next = point[e - n + 2];
    //    free(u);
    //    free(&point);
    //  for (int p=0; p <= e; p++)
    //     free(point[p]);
    return head;
}
