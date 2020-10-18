#include <myhead.h>
#include <getinfo.h>


//获取划屏动作
int get_screen_act(int *start_x,int *start_y,int *end_x,int *end_y)
{
	//定义输入子系统模型有关的结构体变量
	struct input_event event;	
	//打开触摸屏的驱动
	int tsfd=open("/dev/input/event0",O_RDWR);
	if(tsfd==-1)
	{
		perror("打开触摸屏失败!\n");
		return -1;
	}
	int flag1 = 0;
	int x,y;
	while(1)
	{
		//读取触摸屏的坐标
		read(tsfd,&event,sizeof(event));
		//判断一下事件类型
		if(event.type==EV_KEY)  //说明触发的是按键事件
			if(event.code==BTN_TOUCH && event.value>0) //手按下去了
				printf("手按下去了！\n");
		if(event.type==EV_ABS && event.code==ABS_X)	
		{
			x = event.value*800/1024;
			flag1++;
		}	
		if(event.type==EV_ABS && event.code==ABS_Y)	
		{
			y = event.value*480/600;
			flag1++;
		}
		if(flag1 == 2)
		{
			*start_x = x;
			*start_y = y;
		}
		if(event.type==EV_KEY) //说明触发的是按键事件
			if(event.code==BTN_TOUCH && event.value == 0) //手松开了
			{
				usleep(200000);             //延迟0.2s再测一遍
				printf("手松开了！\n");

				*end_x = x;
				*end_y = y;
				break;	
			}						
	}
	//关闭触摸屏
	close(tsfd);
	return 0;
}






