#include <myhead.h>


void end(int sig)
{
	pthread_cancel(id1);
	pthread_cancel(pid);
	UnInit_Lcd();
	free(save_fb_mem);
	free_klist(head);
	close(sockfd);
	exit(0);
}

int main()
{
	signal(2,end);
	
	//��ʼ���ͻ��˲����ӷ�����
	client_init();
	//��ʼ��lcd
	Init_Lcd();

	//����һ��800*480*4�Ķ��ڴ�
	save_fb_mem = malloc(800*480*4);
	
	// //��ʾ�ַ���
	// Display_characterX(0,0,strlen("hello"),"hello",2); //ע�⺺�ֱ�������޸ĳ�GB2312
	while(1)
	{
		//��¼����
		int ret = longin_show();
		if(ret==-1)
		{
			UnInit_Lcd();
			close(sockfd);
			return -1;
		}
		//������
		menu();
	}
}