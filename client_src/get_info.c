#include <myhead.h>
#include <getinfo.h>


//��ȡ��������
int get_screen_act(int *start_x,int *start_y,int *end_x,int *end_y)
{
	//����������ϵͳģ���йصĽṹ�����
	struct input_event event;	
	//�򿪴�����������
	int tsfd=open("/dev/input/event0",O_RDWR);
	if(tsfd==-1)
	{
		perror("�򿪴�����ʧ��!\n");
		return -1;
	}
	int flag1 = 0;
	int x,y;
	while(1)
	{
		//��ȡ������������
		read(tsfd,&event,sizeof(event));
		//�ж�һ���¼�����
		if(event.type==EV_KEY)  //˵���������ǰ����¼�
			if(event.code==BTN_TOUCH && event.value>0) //�ְ���ȥ��
				printf("�ְ���ȥ�ˣ�\n");
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
		if(event.type==EV_KEY) //˵���������ǰ����¼�
			if(event.code==BTN_TOUCH && event.value == 0) //���ɿ���
			{
				usleep(200000);             //�ӳ�0.2s�ٲ�һ��
				printf("���ɿ��ˣ�\n");

				*end_x = x;
				*end_y = y;
				break;	
			}						
	}
	//�رմ�����
	close(tsfd);
	return 0;
}






