#include <myhead.h>

//初始化并连接客户端
int client_init()
{
	//创建套接字    ipv4协议    TCP（数据流套接字） 扩展协议，暂时用不到
	sockfd = socket(AF_INET,     SOCK_STREAM,          0);
	if(sockfd==-1)
	{
		perror("socket err");
		return -1;
	}
	//绑定端口号和ip
	struct sockaddr_in c_addr;
	bzero(&c_addr,sizeof(c_addr));
	c_addr.sin_family = AF_INET;           //ipv4协议或者PF_INET
	//INADDR_ANY自动匹配当前系统的ip地址
	c_addr.sin_addr.s_addr = htonl(INADDR_ANY);            //绑定客户端的自己的ip
	c_addr.sin_port = htons(20480);                           //绑定客户端的自己的端口号
	//设置端口号和ip可以重复使用
	int on=1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	//绑定端口号和ip
	bind(sockfd, (struct sockaddr *)&c_addr ,sizeof(c_addr));
	//服务器的ip地址端口号
	struct sockaddr_in s_addr;
	bzero(&s_addr,sizeof(s_addr));
	s_addr.sin_family = AF_INET;             //ipv4协议
	s_addr.sin_addr.s_addr = inet_addr("192.168.24.28");            //(小端序转大端序)服务器的ip
	s_addr.sin_port = htons(40960);                             //(小端序转大端序)服务器的端口号
	
	int ret = connect(sockfd,(struct sockaddr *)&s_addr,sizeof(s_addr));   //连接服务器
	if(ret == -1)
	{
		perror("connect err");
		return -1;
	}
	return 0;
}


	