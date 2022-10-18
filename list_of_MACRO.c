#include"head_of_lsit_of_MACRO.h"
// typedef struct list {
// 	int value;
// 	struct list* next;
// }list;



// 增 删 查 改 初始化 销毁
////       ----下面是屎山-----           ///////////////////////////////////////////////////////////////////////////////
void head_insert(list**head,int value)
{
    head_insert_(head,value);
}
void head_insert_(list**head,int value)
{
	if (!(*head))
	{
		(*head) = (list*)malloc(sizeof(list));
		assert((*head));
		(*head)->next = NULL;
		(*head)->value = value;
		return;
	}
	else
	{
		list* a = (list*)malloc(sizeof(list));
		assert(a);
		a->next = (*head);
		a->value = value;
		(*head) = a;
		return;
	}

}
void tail_insert(list ** head,int value)
{
    tail_insert_(head,value);
}
void tail_insert_(list** head, int value)
{
	if (!(*head))
	{
		(*head) = (list*)malloc(sizeof(list));
		assert((*head));
		(*head)->next = NULL;
		(*head)->value = value;
		return;
	}
	else {
		list* i = (*head);
		while (i->next)
		{
			i = i->next;
		}
		i->next = (list*)malloc(sizeof(list));
		assert(i->next);
		i->next->next = NULL;
		i->next->value = value;
		return;
	}
}
void head_delete_(list**head) {
	if (!(*head))return ;
	list* a = (*head);
	if((*head)->next)
	{
		(*head)=(*head)->next;
		free(a);
	}
	else{
		free(a);
		(*head)=NULL;
	}
}

void tail_delete(list**head) {
	if (!(*head))return;
	if(!(*head)->next)
	{
		free((*head));
		(*head)=NULL;
		return ;
	}
	list* i = (*head);
	list* i_next = (*head)->next;
	while (i_next->next)
	{
		i = i->next;
		i_next = i_next->next;
	}
	i->next = NULL;
	free(i_next);
}
list* find_for_list(list* head, int target)//返回所在节点
{
	if(!head) return NULL;
	list* a = head;
	while (a->value!=target && a->next)
	{
		a = a->next;
	}
	if (a->value == target)
		return a;
	return NULL;
}
void head_delete(list **head)
{
    head_delete_(head);
}
void list_node_change(list* head, int target, int for_what)
{
	list* i = find_for_list(head, target);
	if (!i)return;
	else i->value = for_what;
}
list* find_for_list_node_delete(list* head, int target)
{
	if (head->value == target)
	{
		list* a = (list*)malloc(sizeof(list));
		assert(a);
		a->next = head;
		return a;
	}
	list* i = head;
	list* i_next = head->next;
	while (i_next && i_next->value != target ) {
		i = i->next;
		i_next = i_next->next;
	}
	if (!i_next)return NULL;
	if (i_next->value == target)return i;
	return NULL;
}
void specified_node_delete_list(list* *head, int target) {
	assert((*head));
	list* a = find_for_list_node_delete((*head), target);//返回的是头节点的前一个
	if(!a) return;
	if (a->next ==(*head)) {
		free(a);
		head_delete(head);
	}
	else if(!a->next->next)
	{
		tail_delete(head);
	}
	else {
		list* a_next = a->next;
		a->next = a->next->next;
		free(a_next);
	}
}
void print_list(list* head)
{
	if(head==NULL)	return ;
	if (!head->value)
	{
		return;
		/* code */
	}
	
	while (head)
	{
		printf("%d ", head->value);
		head = head->next;
	}
}

// int main() {
// 	list* wang = NULL;
// 	HEAD_INSERT(wang, 2);
// 	TAIL_INSERT(wang, 3);
// 	TAIL_INSERT(wang, 4);
// 	HEAD_INSERT(wang, 1);
// 	NODE_DELETE(wang,4);
// 	NODE_DELETE(wang,1);
// 	NODE_DELETE(wang,2);
// 	NODE_DELETE(wang,3);
// 	HEAD_INSERT(wang, 1);
// 	TAIL_DELETE(wang);
// 	TAIL_INSERT(wang, 4);
// 	HEAD_DELETE(wang);
// 	PRINT_LIST(wang);
// 	LIST_NODE_CHANGE(wang, 3, 5);
// 	PRINT_LIST(wang);
// 	return 0;
// }