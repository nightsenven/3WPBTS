#include <myhead.h>
#include <kernel_list.h>

//定义一个结构体保存接收客户端的信息
struct sock_list
{
	int sockid;               //套接字描述符
	short port;  			 //端口号
	char ip[16];               //ip地址
	
	char name[20];                //用户名
	char pass[16];                //密码
	char phone[12];               //手机号码
	struct list_head mylist;
};
//定义一个结构体保存当天车次信息
struct train_info
{
	char train_ID[6];               //车次
	char sname[20];                //出发点
	char ename[20];                //终点
	char stime[20];                //发车时间
	char count[10];						//票数
	char flag[2];						//特等座标志位
	struct list_head mylist;
};

//定义一个保存列车信息结构体的结构体
struct check_train
{
	struct train_info *ch_train;
	struct list_head mylist;
};

//登录信息
struct login_info
{
	char name[20];                //用户名
	char pass[16];                //密码
	char phone[12];               //手机号码
	struct list_head mylist;
};

int sockfd;

//初始化内核链表
struct sock_list *list_init()
{
	struct sock_list *head=malloc(sizeof(struct sock_list));
	if(head!=NULL)
		INIT_LIST_HEAD(&head->mylist);
	return head;
}

//释放链表
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
//释放链表
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

//验证账号密码
struct login_info *checking_up(struct login_info *head,struct login_info *dest)
{
	struct login_info *p;
	list_for_each_entry(p,&head->mylist,mylist)
	{
		if(strcmp(dest->name,p->name)==0 && strcmp(dest->pass,p->pass)==0)
			return p;    //账号密码正确返回 p
	}
	return NULL;   //有一个不匹配返回 NULL
}

//保存乘客信息到文件中
int save_for_info(char *arr)
{
	//打开文件偏移量并移到末尾，如果不存在就创建
	FILE *fd = fopen("info/passenger_info.txt","a+");
	if(fd == NULL)
	{
		perror("fopen err!");
		return -1;
	}
	//写入一行数据
	fputs(arr,fd);

	fclose(fd);
	return 0;
}

//从文件读取乘客信息
struct login_info *read_info(struct login_info *head)
{
	//只读打开文件
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
		//读取一行数据
		char *ret = fgets(arr,sizeof(arr),fd);
		if(ret == NULL)
		{
			if(feof(fd))  //文件到达末尾
				break;
			if(ferror(fd))  //其他错误
			{
				perror("fgets err");
				break;
			}
		}
		struct login_info *new = malloc(sizeof(struct login_info));
		INIT_LIST_HEAD(&new->mylist);
		char *p = strtok(arr,":");               //拆分字符串
		strcpy(new->name,p);
		p = strtok(NULL,":");
		strcpy(new->pass,p);
		p = strtok(NULL,":");
		strcpy(new->phone,p);
		list_add_tail(&new->mylist,&head->mylist);       //尾插
	}
	fclose(fd);
	return head;
}

//从文件读取当前的列车信息
struct train_info *read_today_train(struct train_info *head)
{
	//只读打开文件
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
		//读取一行数据
		char *ret = fgets(arr,sizeof(arr),fd);
		if(ret == NULL)
		{
			if(feof(fd))  //文件到达末尾
				break;
			if(ferror(fd))  //其他错误
			{
				perror("fgets err");
				break;
			}
		}
		struct train_info *new = malloc(sizeof(struct train_info));
		INIT_LIST_HEAD(&new->mylist);
		char *p = strtok(arr,"\t");               //拆分字符串
		strcpy(new->train_ID,p);
		p = strtok(NULL,"\t");
		strcpy(new->sname,p);
		p = strtok(NULL,"\t");
		strcpy(new->ename,p);
		p = strtok(NULL,"\n");
		strcpy(new->stime,p);
		list_add_tail(&new->mylist,&head->mylist);       //尾插
	}
	
	fclose(fd);
	return head;
}

//从文件读取所有的列车信息
struct train_info *read_all_train(struct train_info *head)
{
	//只读打开文件
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
		//读取一行数据
		char *ret = fgets(arr,sizeof(arr),fd);
		if(ret == NULL)
		{
			if(feof(fd))  //文件到达末尾
				break;
			if(ferror(fd))  //其他错误
			{
				perror("fgets err");
				break;
			}
		}
		struct train_info *new = malloc(sizeof(struct train_info));
		INIT_LIST_HEAD(&new->mylist);
		char *p = strtok(arr,"\t");               //拆分字符串
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
		list_add_tail(&new->mylist,&head->mylist);       //尾插
	}
	
	fclose(fd);
	return head;
}


//修改票数
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


//修改保存所有的列车信息
int save_all_train(struct train_info *head)
{
	//打开文件偏移量并清空，如果不存在就创建
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


//在所有列车里查询列车
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
			list_add_tail(&new->mylist,&ch_head->mylist);       //尾插
		}
	}
	return ch_head;
}

//响应信号函数
void end(int sig)
{
	close(sockfd);
	exit(0);	
}

int main()
{
	signal(2,end);
	
	//创建内核链表-->保存连接到服务器的客户端信息
	struct sock_list *head = list_init();
	//初始化内核链表
	struct train_info *today_head = malloc(sizeof(struct train_info));
	INIT_LIST_HEAD(&today_head->mylist);
	//读取列车信息并保存
	today_head=read_today_train(today_head);
	//初始化内核链表
	struct train_info *all_head = malloc(sizeof(struct train_info));
	INIT_LIST_HEAD(&all_head->mylist);
	//读取列车信息并保存
	all_head=read_all_train(all_head);
	
	
	//创建套接字    ipv4协议    TCP（数据流套接字） 扩展协议，暂时用不到
	sockfd = socket(AF_INET,     SOCK_STREAM,          0);
	if(sockfd==-1)
	{
		perror("socket err");
		return -1;
	}
	//设置端口号和ip可以重复使用
	int on=1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	//绑定端口号和ip
	struct sockaddr_in s_addr;
	bzero(&s_addr,sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	//INADDR_ANY自动匹配当前系统的ip地址
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY); //绑定服务器的自己的ip
	s_addr.sin_port = htons(40960);                                      //绑定服务器的自己的端口号
	bind(sockfd, (struct sockaddr *)&s_addr ,sizeof(s_addr));
	
	
	listen(sockfd,7);   //设置可以同时监听7个客户端
	//客户端的ip和端口号
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
	int maxid;      //最大文件描述符
	//定义文件描述符集合
	fd_set myset;
	while(1)
	{
		FD_ZERO(&myset);
		FD_SET(sockfd,&myset);
		maxid=sockfd;
		//添加文件描述符
		list_for_each_entry(p,&head->mylist,mylist)
		{
			FD_SET(p->sockid,&myset);
			if(p->mylist.next == &head->mylist)
				maxid=p->sockid;
		}

		//阻塞                     状态可读 可写 异常 定时检测
		int ret = select(maxid+1,&myset,NULL,NULL, NULL);     //不定时监测
		if(ret>0)
		{
			//如果是旧的套接字描述符状态发生了变化
			if(FD_ISSET(sockfd,&myset))
			{
				//阻塞
				int acfd = accept(sockfd,(struct sockaddr *)&c_addr, &len);   //接收客户端
				if(acfd==-1)
				{
					perror("accept err");
					return -1;
				}
				
				//创建新节点并初始化
				struct sock_list *new = malloc(sizeof(struct sock_list));
				new->sockid=acfd;        //新客户的套接字
				strcpy(new->ip,inet_ntoa(c_addr.sin_addr));          //(大端转小端)新客户的ip地址
				new->port = ntohs(c_addr.sin_port);         //新客户的端口号
				INIT_LIST_HEAD(&new->mylist);
				list_add_tail(&new->mylist,&head->mylist);       //尾插 
				printf("端口号：%d，ip：%s 连接成功。\n",new->port,new->ip);
				
			}	
			list_for_each_entry(p,&head->mylist,mylist)
			{
				
				if(FD_ISSET(p->sockid,&myset))            //判断是否是接收的描述符状态发生改变
				{
					bzero(buf,sizeof(buf));
					ret = read(p->sockid,buf,sizeof(buf));      //获取信息
					if(ret==0)
					{
						printf("端口号：%d，ip：%s退出了群聊。\n",p->port,p->ip);
						
						list_del(&p->mylist);
						free(p);
						break;
					}

					if(strcmp(buf,"login")==0)     //登录验证信息
					{
						//初始化
						struct login_info *login_head = malloc(sizeof(struct login_info));
						INIT_LIST_HEAD(&login_head->mylist);
						//读取文件数据
						login_head = read_info(login_head);
						//创建新节点保存登录信息
						struct login_info *new = malloc(sizeof(struct login_info));
						INIT_LIST_HEAD(&new->mylist);
						strcpy(new->name,users);
						strcpy(new->pass,pass);
						//匹配用户名和密码
						struct login_info *user = checking_up(login_head,new);
						printf("new->name:%s,new->password:%s\n",new->name,new->pass);
						if(user)    
						{
							write(p->sockid,"phone",strlen("phone"));
							bzero(phone,sizeof(phone));
							read(p->sockid,phone,sizeof(phone));      //获取信息
							if(strcmp(phone,user->phone)==0)  	 //匹配成功
							{
								strcpy(p->name,user->name);
								strcpy(p->pass,user->pass);
								strcpy(p->phone,user->phone);
								write(p->sockid,"agree",strlen("agree"));
							}
							else                         //匹配失败
								write(p->sockid,"unagree",strlen("unagree"));
						}
						else     //匹配失败
							write(p->sockid,"unagree",strlen("unagree"));
						free_list(login_head);
						free(new);
					}
					else if(strcmp(buf,"users")==0)  //用户名
					{
						bzero(users,sizeof(users));
						read(p->sockid,users,sizeof(users));      //获取信息
					}
					else if(strcmp(buf,"password")==0)   //密码
					{
						bzero(pass,sizeof(pass));
						read(p->sockid,pass,sizeof(pass));      //获取信息
					}
					else if(strcmp(buf,"regname")==0)    //注册用户名
					{
						bzero(users,sizeof(users));
						read(p->sockid,users,sizeof(users));      //获取信息
					}
					else if(strcmp(buf,"regpass")==0)     //注册密码
					{
						bzero(pass,sizeof(pass));
						read(p->sockid,pass,sizeof(pass));      //获取信息			
					}
					else if(strcmp(buf,"regphone")==0)     //注册手机号码
					{
						bzero(phone,sizeof(phone));
						read(p->sockid,phone,sizeof(phone));      //获取信息			
					}
					else if(strcmp(buf,"reg")==0)     //注册
					{
						/*保存注册信息到txt文件中*/
						bzero(arr,sizeof(arr));
						sprintf(arr,"%s:%s:%s:\n",users,pass,phone);
						save_for_info(arr);
					}
					else if(strcmp(buf,"getlist")==0)     //获取当天当前准备发车的列车
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
					else if(strcmp(buf,"start")==0)     //起始地
					{
						bzero(sbuf,sizeof(sbuf));
						read(p->sockid,sbuf,sizeof(sbuf));      //获取信息	
					}
					else if(strcmp(buf,"end")==0)     //目的地
					{
						bzero(ebuf,sizeof(ebuf));
						read(p->sockid,ebuf,sizeof(ebuf));      //获取信息	
					}
					else if(strcmp(buf,"check")==0)     //查询列车
					{
						struct check_train *ch_head = malloc(sizeof(struct check_train));
						INIT_LIST_HEAD(&ch_head->mylist);
						//查询列车
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
					else if(strcmp(buf,"buyend")==0)     //修改票数
					{
						int count;
						bzero(buf,sizeof(buf));
						read(p->sockid,buf,sizeof(buf));
						read(p->sockid,&count,4);
						all_head = change_node(all_head,buf,count);        //修改票数
						save_all_train(all_head);         //修改文件数据
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