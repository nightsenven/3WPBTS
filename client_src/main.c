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
	
	//初始化客户端并连接服务器
	client_init();
	//初始化lcd
	Init_Lcd();

	//申请一个800*480*4的堆内存
	save_fb_mem = malloc(800*480*4);
	
	// //显示字符串
	// Display_characterX(0,0,strlen("hello"),"hello",2); //注意汉字编码必须修改成GB2312
	while(1)
	{
		//登录界面
		int ret = longin_show();
		if(ret==-1)
		{
			UnInit_Lcd();
			close(sockfd);
			return -1;
		}
		//主界面
		menu();
	}
}