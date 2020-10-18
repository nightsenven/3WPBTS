#include <myhead.h>

//初始化内核链表
struct klist *init_list()
{
	struct klist *head = malloc(sizeof(struct klist));
	if(head)
		INIT_LIST_HEAD(&(head->mylist));
	return head;
}


//寻找节点
struct klist *fine_node(struct klist *head,int num)
{
	struct klist *p;
	list_for_each_entry(p, &(head->mylist), mylist)
	{
		if(p->number==num)
			return p;
	}
	return NULL;
}


//释放内核链表的所有堆空间
int free_klist(struct klist *head)
{
	struct klist *p;
	struct klist *n;
	list_for_each_entry_safe(p, n, &(head->mylist), mylist)
	{
		list_del(&(p->mylist));
		free(p);
	}
	list_del(&(p->mylist));
	free(p);
	return 0;
}


