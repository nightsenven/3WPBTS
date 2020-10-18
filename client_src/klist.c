#include <myhead.h>

//��ʼ���ں�����
struct klist *init_list()
{
	struct klist *head = malloc(sizeof(struct klist));
	if(head)
		INIT_LIST_HEAD(&(head->mylist));
	return head;
}


//Ѱ�ҽڵ�
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


//�ͷ��ں���������жѿռ�
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


