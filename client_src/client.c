#include <myhead.h>

//��ʼ�������ӿͻ���
int client_init()
{
	//�����׽���    ipv4Э��    TCP���������׽��֣� ��չЭ�飬��ʱ�ò���
	sockfd = socket(AF_INET,     SOCK_STREAM,          0);
	if(sockfd==-1)
	{
		perror("socket err");
		return -1;
	}
	//�󶨶˿ںź�ip
	struct sockaddr_in c_addr;
	bzero(&c_addr,sizeof(c_addr));
	c_addr.sin_family = AF_INET;           //ipv4Э�����PF_INET
	//INADDR_ANY�Զ�ƥ�䵱ǰϵͳ��ip��ַ
	c_addr.sin_addr.s_addr = htonl(INADDR_ANY);            //�󶨿ͻ��˵��Լ���ip
	c_addr.sin_port = htons(20480);                           //�󶨿ͻ��˵��Լ��Ķ˿ں�
	//���ö˿ںź�ip�����ظ�ʹ��
	int on=1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	//�󶨶˿ںź�ip
	bind(sockfd, (struct sockaddr *)&c_addr ,sizeof(c_addr));
	//��������ip��ַ�˿ں�
	struct sockaddr_in s_addr;
	bzero(&s_addr,sizeof(s_addr));
	s_addr.sin_family = AF_INET;             //ipv4Э��
	s_addr.sin_addr.s_addr = inet_addr("192.168.24.28");            //(С����ת�����)��������ip
	s_addr.sin_port = htons(40960);                             //(С����ת�����)�������Ķ˿ں�
	
	int ret = connect(sockfd,(struct sockaddr *)&s_addr,sizeof(s_addr));   //���ӷ�����
	if(ret == -1)
	{
		perror("connect err");
		return -1;
	}
	return 0;
}


	