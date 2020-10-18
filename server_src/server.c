#include <myhead.h>
#include <kernel_list.h>

//����һ���ṹ�屣����տͻ��˵���Ϣ
struct sock_list
{
	int sockid;               //�׽���������
	short port;  			 //�˿ں�
	char ip[16];               //ip��ַ
	
	char name[20];                //�û���
	char pass[16];                //����
	char phone[12];               //�ֻ�����
	struct list_head mylist;
};
//����һ���ṹ�屣�浱�쳵����Ϣ
struct train_info
{
	char train_ID[6];               //����
	char sname[20];                //������
	char ename[20];                //�յ�
	char stime[20];                //����ʱ��
	char count[10];						//Ʊ��
	char flag[2];						//�ص�����־λ
	struct list_head mylist;
};

//����һ�������г���Ϣ�ṹ��Ľṹ��
struct check_train
{
	struct train_info *ch_train;
	struct list_head mylist;
};

//��¼��Ϣ
struct login_info
{
	char name[20];                //�û���
	char pass[16];                //����
	char phone[12];               //�ֻ�����
	struct list_head mylist;
};

int sockfd;

//��ʼ���ں�����
struct sock_list *list_init()
{
	struct sock_list *head=malloc(sizeof(struct sock_list));
	if(head!=NULL)
		INIT_LIST_HEAD(&head->mylist);
	return head;
}

//�ͷ�����
void free_list(struct login_info *head)
{
	struct login_info *p;
	struct login_info *n;
	list_for_each_entry_safe(p, n, &(head->mylist), mylist)
	{
		list_del(&(p->mylist));
		free(p);
	}
	list_del(&(p->mylist));
	free(p);
}
//�ͷ�����
void free_list_ch(struct check_train *head)
{
	struct check_train *p;
	struct check_train *n;
	list_for_each_entry_safe(p, n, &(head->mylist), mylist)
	{
		list_del(&(p->mylist));
		free(p);
	}
	list_del(&(p->mylist));
	free(p);
}

//��֤�˺�����
struct login_info *checking_up(struct login_info *head,struct login_info *dest)
{
	struct login_info *p;
	list_for_each_entry(p,&head->mylist,mylist)
	{
		if(strcmp(dest->name,p->name)==0 && strcmp(dest->pass,p->pass)==0)
			return p;    //�˺�������ȷ���� p
	}
	return NULL;   //��һ����ƥ�䷵�� NULL
}

//����˿���Ϣ���ļ���
int save_for_info(char *arr)
{
	//���ļ�ƫ�������Ƶ�ĩβ����������ھʹ���
	FILE *fd = fopen("info/passenger_info.txt","a+");
	if(fd == NULL)
	{
		perror("fopen err!");
		return -1;
	}
	//д��һ������
	fputs(arr,fd);

	fclose(fd);
	return 0;
}

//���ļ���ȡ�˿���Ϣ
struct login_info *read_info(struct login_info *head)
{
	//ֻ�����ļ�
	FILE *fd = fopen("info/passenger_info.txt","r");
	if(fd == NULL)
	{
		perror("fopen err!");
		return NULL;
	}
	char arr[256];
	while(1)
	{
		bzero(arr,sizeof(arr));
		//��ȡһ������
		char *ret = fgets(arr,sizeof(arr),fd);
		if(ret == NULL)
		{
			if(feof(fd))  //�ļ�����ĩβ
				break;
			if(ferror(fd))  //��������
			{
				perror("fgets err");
				break;
			}
		}
		struct login_info *new = malloc(sizeof(struct login_info));
		INIT_LIST_HEAD(&new->mylist);
		char *p = strtok(arr,":");               //����ַ���
		strcpy(new->name,p);
		p = strtok(NULL,":");
		strcpy(new->pass,p);
		p = strtok(NULL,":");
		strcpy(new->phone,p);
		list_add_tail(&new->mylist,&head->mylist);       //β��
	}
	fclose(fd);
	return head;
}

//���ļ���ȡ��ǰ���г���Ϣ
struct train_info *read_today_train(struct train_info *head)
{
	//ֻ�����ļ�
	FILE *fd = fopen("info/today_train.txt","r");
	if(fd == NULL)
	{
		perror("fopen err!");
		return NULL;
	}
	char arr[256];
	while(1)
	{
		bzero(arr,sizeof(arr));
		//��ȡһ������
		char *ret = fgets(arr,sizeof(arr),fd);
		if(ret == NULL)
		{
			if(feof(fd))  //�ļ�����ĩβ
				break;
			if(ferror(fd))  //��������
			{
				perror("fgets err");
				break;
			}
		}
		struct train_info *new = malloc(sizeof(struct train_info));
		INIT_LIST_HEAD(&new->mylist);
		char *p = strtok(arr,"\t");               //����ַ���
		strcpy(new->train_ID,p);
		p = strtok(NULL,"\t");
		strcpy(new->sname,p);
		p = strtok(NULL,"\t");
		strcpy(new->ename,p);
		p = strtok(NULL,"\n");
		strcpy(new->stime,p);
		list_add_tail(&new->mylist,&head->mylist);       //β��
	}
	
	fclose(fd);
	return head;
}

//���ļ���ȡ���е��г���Ϣ
struct train_info *read_all_train(struct train_info *head)
{
	//ֻ�����ļ�
	FILE *fd = fopen("info/train.txt","r");
	if(fd == NULL)
	{
		perror("fopen err!");
		return NULL;
	}
	char arr[256];
	while(1)
	{
		bzero(arr,sizeof(arr));
		//��ȡһ������
		char *ret = fgets(arr,sizeof(arr),fd);
		if(ret == NULL)
		{
			if(feof(fd))  //�ļ�����ĩβ
				break;
			if(ferror(fd))  //��������
			{
				perror("fgets err");
				break;
			}
		}
		struct train_info *new = malloc(sizeof(struct train_info));
		INIT_LIST_HEAD(&new->mylist);
		char *p = strtok(arr,"\t");               //����ַ���
		strcpy(new->train_ID,p);
		p = strtok(NULL,"\t");
		strcpy(new->sname,p);
		p = strtok(NULL,"\t");
		strcpy(new->ename,p);
		p = strtok(NULL,"\t");
		strcpy(new->stime,p);
		p = strtok(NULL,"\t");
		strcpy(new->count,p);
		p = strtok(NULL,"\n");
		strcpy(new->flag,p);
		list_add_tail(&new->mylist,&head->mylist);       //β��
	}
	
	fclose(fd);
	return head;
}


//�޸�Ʊ��
struct train_info *change_node(struct train_info *head,char *ID,int count)
{
	struct train_info *p;
	list_for_each_entry(p, &(head->mylist), mylist)
	{
		if(strcmp(p->train_ID,ID)==0)
			break;
	}
	if(p!=head)
	{
		char temp[20] = {};
		strcpy(temp,p->count);
		char *s = strtok(temp,"/");
		count = atoi(s)-count;
		s = strtok(NULL,"");
		bzero(p->count,sizeof(p->count));
		sprintf(p->count,"%d/%s",count,s);
	}
	return head;
}


//�޸ı������е��г���Ϣ
int save_all_train(struct train_info *head)
{
	//���ļ�ƫ��������գ���������ھʹ���
	int fd = open("info/train.txt",O_RDWR|O_CREAT|O_TRUNC,0777);
	if(fd == -1)
	{
		perror("open err!");
		return -1;
	}
	struct train_info *p;
	char temp[100];
	list_for_each_entry(p, &(head->mylist), mylist)
	{
		bzero(temp,100);
		sprintf(temp,"%s\t%s\t%s\t%s\t%s\t%s\n",      \
					p->train_ID,p->sname,p->ename,p->stime,p->count,p->flag);
		write(fd,temp,strlen(temp));
	}
	close(fd);
	return 0;
}


//�������г����ѯ�г�
struct check_train *find_train(struct check_train *ch_head,struct train_info *head,char *sname,char *ename)
{	
	struct train_info *p;
	
	int i=0;
	list_for_each_entry(p, &(head->mylist), mylist)
	{
		if(strcmp(p->sname,sname)==0 && strcmp(p->ename,ename)==0)
		{
			struct check_train *new = malloc(sizeof(struct check_train));
			INIT_LIST_HEAD(&new->mylist);
			new->ch_train = malloc(sizeof(struct train_info));
			new->ch_train = p;
			list_add_tail(&new->mylist,&ch_head->mylist);       //β��
		}
	}
	return ch_head;
}

//��Ӧ�źź���
void end(int sig)
{
	close(sockfd);
	exit(0);	
}

int main()
{
	signal(2,end);
	
	//�����ں�����-->�������ӵ��������Ŀͻ�����Ϣ
	struct sock_list *head = list_init();
	//��ʼ���ں�����
	struct train_info *today_head = malloc(sizeof(struct train_info));
	INIT_LIST_HEAD(&today_head->mylist);
	//��ȡ�г���Ϣ������
	today_head=read_today_train(today_head);
	//��ʼ���ں�����
	struct train_info *all_head = malloc(sizeof(struct train_info));
	INIT_LIST_HEAD(&all_head->mylist);
	//��ȡ�г���Ϣ������
	all_head=read_all_train(all_head);
	
	
	//�����׽���    ipv4Э��    TCP���������׽��֣� ��չЭ�飬��ʱ�ò���
	sockfd = socket(AF_INET,     SOCK_STREAM,          0);
	if(sockfd==-1)
	{
		perror("socket err");
		return -1;
	}
	//���ö˿ںź�ip�����ظ�ʹ��
	int on=1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	//�󶨶˿ںź�ip
	struct sockaddr_in s_addr;
	bzero(&s_addr,sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	//INADDR_ANY�Զ�ƥ�䵱ǰϵͳ��ip��ַ
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY); //�󶨷��������Լ���ip
	s_addr.sin_port = htons(40960);                                      //�󶨷��������Լ��Ķ˿ں�
	bind(sockfd, (struct sockaddr *)&s_addr ,sizeof(s_addr));
	
	
	listen(sockfd,7);   //���ÿ���ͬʱ����7���ͻ���
	//�ͻ��˵�ip�Ͷ˿ں�
	struct sockaddr_in c_addr;
	bzero(&c_addr,sizeof(c_addr));
	socklen_t len=sizeof(c_addr);

	int flag=0;
	char buf[256];
	char arr[300];
	char users[20];
	char pass[16];
	char phone[12];
	char sbuf[20];
	char ebuf[20];
	struct sock_list *p;
	struct train_info *n;
	int maxid;      //����ļ�������
	//�����ļ�����������
	fd_set myset;
	while(1)
	{
		FD_ZERO(&myset);
		FD_SET(sockfd,&myset);
		maxid=sockfd;
		//����ļ�������
		list_for_each_entry(p,&head->mylist,mylist)
		{
			FD_SET(p->sockid,&myset);
			if(p->mylist.next == &head->mylist)
				maxid=p->sockid;
		}

		//����                     ״̬�ɶ� ��д �쳣 ��ʱ���
		int ret = select(maxid+1,&myset,NULL,NULL, NULL);     //����ʱ���
		if(ret>0)
		{
			//����Ǿɵ��׽���������״̬�����˱仯
			if(FD_ISSET(sockfd,&myset))
			{
				//����
				int acfd = accept(sockfd,(struct sockaddr *)&c_addr, &len);   //���տͻ���
				if(acfd==-1)
				{
					perror("accept err");
					return -1;
				}
				
				//�����½ڵ㲢��ʼ��
				struct sock_list *new = malloc(sizeof(struct sock_list));
				new->sockid=acfd;        //�¿ͻ����׽���
				strcpy(new->ip,inet_ntoa(c_addr.sin_addr));          //(���תС��)�¿ͻ���ip��ַ
				new->port = ntohs(c_addr.sin_port);         //�¿ͻ��Ķ˿ں�
				INIT_LIST_HEAD(&new->mylist);
				list_add_tail(&new->mylist,&head->mylist);       //β�� 
				printf("�˿ںţ�%d��ip��%s ���ӳɹ���\n",new->port,new->ip);
				
			}	
			list_for_each_entry(p,&head->mylist,mylist)
			{
				
				if(FD_ISSET(p->sockid,&myset))            //�ж��Ƿ��ǽ��յ�������״̬�����ı�
				{
					bzero(buf,sizeof(buf));
					ret = read(p->sockid,buf,sizeof(buf));      //��ȡ��Ϣ
					if(ret==0)
					{
						printf("�˿ںţ�%d��ip��%s�˳���Ⱥ�ġ�\n",p->port,p->ip);
						
						list_del(&p->mylist);
						free(p);
						break;
					}

					if(strcmp(buf,"login")==0)     //��¼��֤��Ϣ
					{
						//��ʼ��
						struct login_info *login_head = malloc(sizeof(struct login_info));
						INIT_LIST_HEAD(&login_head->mylist);
						//��ȡ�ļ�����
						login_head = read_info(login_head);
						//�����½ڵ㱣���¼��Ϣ
						struct login_info *new = malloc(sizeof(struct login_info));
						INIT_LIST_HEAD(&new->mylist);
						strcpy(new->name,users);
						strcpy(new->pass,pass);
						//ƥ���û���������
						struct login_info *user = checking_up(login_head,new);
						printf("new->name:%s,new->password:%s\n",new->name,new->pass);
						if(user)    
						{
							write(p->sockid,"phone",strlen("phone"));
							bzero(phone,sizeof(phone));
							read(p->sockid,phone,sizeof(phone));      //��ȡ��Ϣ
							if(strcmp(phone,user->phone)==0)  	 //ƥ��ɹ�
							{
								strcpy(p->name,user->name);
								strcpy(p->pass,user->pass);
								strcpy(p->phone,user->phone);
								write(p->sockid,"agree",strlen("agree"));
							}
							else                         //ƥ��ʧ��
								write(p->sockid,"unagree",strlen("unagree"));
						}
						else     //ƥ��ʧ��
							write(p->sockid,"unagree",strlen("unagree"));
						free_list(login_head);
						free(new);
					}
					else if(strcmp(buf,"users")==0)  //�û���
					{
						bzero(users,sizeof(users));
						read(p->sockid,users,sizeof(users));      //��ȡ��Ϣ
					}
					else if(strcmp(buf,"password")==0)   //����
					{
						bzero(pass,sizeof(pass));
						read(p->sockid,pass,sizeof(pass));      //��ȡ��Ϣ
					}
					else if(strcmp(buf,"regname")==0)    //ע���û���
					{
						bzero(users,sizeof(users));
						read(p->sockid,users,sizeof(users));      //��ȡ��Ϣ
					}
					else if(strcmp(buf,"regpass")==0)     //ע������
					{
						bzero(pass,sizeof(pass));
						read(p->sockid,pass,sizeof(pass));      //��ȡ��Ϣ			
					}
					else if(strcmp(buf,"regphone")==0)     //ע���ֻ�����
					{
						bzero(phone,sizeof(phone));
						read(p->sockid,phone,sizeof(phone));      //��ȡ��Ϣ			
					}
					else if(strcmp(buf,"reg")==0)     //ע��
					{
						/*����ע����Ϣ��txt�ļ���*/
						bzero(arr,sizeof(arr));
						sprintf(arr,"%s:%s:%s:\n",users,pass,phone);
						save_for_info(arr);
					}
					else if(strcmp(buf,"getlist")==0)     //��ȡ���쵱ǰ׼���������г�
					{
						list_for_each_entry(n,&today_head->mylist,mylist)
						{
							bzero(arr,sizeof(arr));
							sprintf(arr,"%s\t%s\t%s\t%s\t",n->train_ID,n->sname,n->ename,n->stime);
							write(p->sockid,arr,strlen(arr));
							usleep(1000);
						}
						write(p->sockid,"none",strlen("none"));
					}
					else if(strcmp(buf,"start")==0)     //��ʼ��
					{
						bzero(sbuf,sizeof(sbuf));
						read(p->sockid,sbuf,sizeof(sbuf));      //��ȡ��Ϣ	
					}
					else if(strcmp(buf,"end")==0)     //Ŀ�ĵ�
					{
						bzero(ebuf,sizeof(ebuf));
						read(p->sockid,ebuf,sizeof(ebuf));      //��ȡ��Ϣ	
					}
					else if(strcmp(buf,"check")==0)     //��ѯ�г�
					{
						struct check_train *ch_head = malloc(sizeof(struct check_train));
						INIT_LIST_HEAD(&ch_head->mylist);
						//��ѯ�г�
						ch_head=find_train(ch_head,all_head,sbuf,ebuf);
						struct check_train *q;
						list_for_each_entry(q,&ch_head->mylist,mylist)
						{
							
							bzero(arr,sizeof(arr));
							sprintf(arr,"%s\t%s\t%s\t%s\t%s\t%s\t",         		 \
										q->ch_train->train_ID,q->ch_train->sname,    \
										q->ch_train->ename,q->ch_train->stime,       \
										q->ch_train->count,q->ch_train->flag);
							write(p->sockid,arr,strlen(arr));
							usleep(1000);
						}
						write(p->sockid,"none",strlen("none"));
						free_list_ch(ch_head);
					}
					else if(strcmp(buf,"buyend")==0)     //�޸�Ʊ��
					{
						int count;
						bzero(buf,sizeof(buf));
						read(p->sockid,buf,sizeof(buf));
						read(p->sockid,&count,4);
						all_head = change_node(all_head,buf,count);        //�޸�Ʊ��
						save_all_train(all_head);         //�޸��ļ�����
					}
				} 
			}				
		}
		else
		{
			perror("select err");
			return -1;
		}
	}
	return 0;
}