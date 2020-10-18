#include <myhead.h>
#include <getinfo.h>
#include <botton.h>

#include <lcd.h>

char arr[300] = {0};
char turnarr[500] = {};
int chat_y;             //聊天信息显示的y值   
int pthflag=0;    //标志位

//获取bmp图片的宽和高
int get_bmp_wh(char *bmp_name,int *w,int *h)
{
	
	int bmpfd = open(bmp_name,O_RDWR);//打开BMP图片
	if(bmpfd == -1)
	{
		perror("open err!");
		return -1;
	}
	if(w!=NULL&&h!=NULL)
	{
		//bmp图片前54个字节的第18个字节之后的8个字节分别保存着图片的宽和高
		lseek(bmpfd,18,SEEK_SET);   
		read(bmpfd,w,4);  //获取bmp图片的宽
		read(bmpfd,h,4);  //获取bmp图片的高
	}
	return bmpfd;
}

//显示一张任意大小的图片
int show_any_bmp(int bmpfd,int bmp_x,int bmp_y,int s_x,int s_y,int n)
{

	//定义一个数组，依据图片的大小
	char bmpbuf[bmp_x*bmp_y*3];
	int lcdbuf[bmp_x*bmp_y];
	int lastbuf[bmp_x*bmp_y];
	bzero(bmpbuf,sizeof(bmpbuf));
	bzero(lcdbuf,sizeof(lcdbuf));
	bzero(lastbuf,sizeof(lastbuf));
	
	lseek(bmpfd,54,SEEK_SET);  //从头偏移图片属性信息54个字节
	//读取bmp图片的RGB数据
	int i,j;
	for(i=0; i<bmp_y; i++)  //按行读取
	{
		read(bmpfd, &bmpbuf[i*bmp_x*3], bmp_x*3);
		int tmp;
		for(tmp=bmp_x*3; tmp%4 != 0; tmp++);
		lseek(bmpfd,tmp-bmp_x*3,SEEK_CUR);
	}
	
	//3字节转换成4字节
	for(i=0; i<bmp_x*bmp_y; i++)
		lcdbuf[i] = bmpbuf[i*3+2]<<16|bmpbuf[i*3+1]<<8|bmpbuf[i*3+0];
	
	//图片按行颠倒
	for(i=0; i<bmp_x; i++)   //列
		for(j=0; j<bmp_y; j++)  //行
			lastbuf[(bmp_y-1-j)*bmp_x+i] = lcdbuf[i+j*bmp_x];
	
	//正常显示
	if(n==0)
	{
		//映射内存
		for(j=0; j<bmp_y; j++)
			for(i=0; i<bmp_x; i++)
				*(fb_mem+(j+s_y)*800+s_x+i) = lastbuf[j*bmp_x+i];
	}
	
	//只显示光标所在的区域 800*480
	if(n==1)
	{
		//映射内存
		for(j=s_y; j<s_y+30; j++)
			for(i=s_x; i<s_x+30; i++)
				*(fb_mem+j*800+i) = lastbuf[j*bmp_x+i];
	}
	//只显示光标所在的区域 500*300
	if(n==2)
	{
		//映射内存
		for(j=s_y; j<s_y+30; j++)
			for(i=s_x; i<s_x+30; i++)
				*(fb_mem+(j+90)*800+i+150) = lastbuf[j*bmp_x+i];
	}
	//只显示部分区域--->刷新局部区域  800*480
	if(n==3)
	{
		//映射内存
		for(j=s_y; j<s_y+30; j++)
			for(i=290; i<594; i++)
				*(fb_mem+j*800+i) = lastbuf[j*bmp_x+i];
	}
	
	//只显示部分区域--->刷新局部区域   500*300
	if(n==4)
	{
		//映射内存
		for(j=s_y; j<s_y+25; j++)
			for(i=213; i<402; i++)
				*(fb_mem+(j+90)*800+i+150) = lastbuf[j*bmp_x+i];    //居lcd中显示
	}
	//只显示部分区域--->刷新局部区域   700*270
	if(n==5)
	{
		//映射内存
		for(j=s_y; j<s_y+34; j++)
			for(i=0; i<700; i++)
				*(fb_mem+(j+105)*800+i+44) = lastbuf[j*bmp_x+i];    //居lcd中显示
	}
	//只显示部分区域--->刷新局部区域  800*480
	if(n==6)
	{
		//映射内存
		for(j=s_y; j<s_y+53; j++)
			for(i=s_x; i<s_x+120; i++)
				*(fb_mem+j*800+i) = lastbuf[j*bmp_x+i];
	}
	//只显示部分区域--->刷新局部区域  400*200
	if(n==7)
	{
		//映射内存
		for(j=s_y; j<s_y+44; j++)
			for(i=s_x; i<s_x+70; i++)
				*(fb_mem+(j+50)*800+i+200) = lastbuf[j*bmp_x+i];
	}
	//只显示光标所在的区域 400*200
	if(n==8)
	{
		//映射内存
		for(j=s_y; j<s_y+30; j++)
			for(i=s_x; i<s_x+30; i++)
				*(fb_mem+(j+50)*800+i+200) = lastbuf[j*bmp_x+i];
	}
	//关闭
	close(bmpfd);
	return 0;
}

//指定区域显示灰色
void area_show_black(int x,int y,int s_x,int s_y,int color)
{
	int i,j;
	//映射内存
	for(j=0; j<y; j++)
		for(i=0; i<x; i++)
			*(fb_mem+(j+s_y)*800+s_x+i) = color;
}


//显示光标线程  --  登录界面
void *login_cursor(void *arg)
{
	while(1)
	{
		Display_characterXXX(x_x,y_y,strlen("|"),"|",2,0x00);       //显示汉字│
		usleep(700000);            //延迟0.7s
		int bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
		show_any_bmp(bmpfd,800,480,x_x,y_y,1);
		usleep(700000);            //延迟0.7s
	}
}

//显示光标线程  --  注册界面
void *register_cursor(void *arg)
{
	while(1)
	{
		Display_characterXXX(x_x+150,y_y+90,strlen("|"),"|",2,0x00);       //显示汉字│
		usleep(700000);            //延迟0.7s
		int bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
		show_any_bmp(bmpfd,500,300,x_x,y_y,2);
		usleep(700000);            //延迟0.7s
	}
}

//显示光标线程  --  列车界面
void *train_cursor(void *arg)
{
	while(1)
	{
		Display_characterXXX(x_x,y_y,strlen("|"),"|",2,0x00);       //显示汉字│
		usleep(700000);            //延迟0.7s
		int bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
		show_any_bmp(bmpfd,800,480,x_x,y_y,1);
		usleep(700000);            //延迟0.7s
	}
}

//显示光标线程  --  购买界面
void *buy_cursor(void *arg)
{
	while(1)
	{
		Display_characterXXX(x_x+200,y_y+50,strlen("|"),"|",2,0x00);       //显示汉字│
		usleep(700000);            //延迟0.7s
		int bmpfd = get_bmp_wh("interface/buy.bmp",NULL,NULL);
		show_any_bmp(bmpfd,400,200,x_x,y_y,8);
		usleep(700000);            //延迟0.7s
	}
}

//注册登录信息
int apply_info()
{
	char name[20];
	char pass[16];
	char phone[12];
	//显示注册界面
	int bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
	show_any_bmp(bmpfd,500,300,150,90,0);
	
	int flag=0;
	while(1)
	{
		int botton = act_screen_XXX();
		switch(botton)
		{
			case REGNAME:    //注册用户名
			
				write(sockfd,"regname",strlen("regname"));
				//刷新局部区域
				bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
				show_any_bmp(bmpfd,500,300,0,70,4);
				
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
					show_any_bmp(bmpfd,500,300,x_x,y_y,2);
				}
				x_x=213;
				y_y=70;
				//创建光标显示线程
				pthread_create(&id1,NULL,register_cursor,NULL);
				
				flag=1;	
				bzero(name,20);
				printf("请注册用户名：\n");
				scanf("%s",name);
				while(getchar()!='\n');
				write(sockfd,name,strlen(name));
		
				Display_characterXX(x_x+150,y_y+90,strlen(name),name,2,0xffffff);    //显示白色汉字
				x_x-=145;
				y_y-=90;
				break;
			case REGPASS:     //注册密码
				write(sockfd,"regpass",strlen("regpass"));
				//刷新局部区域
				bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
				show_any_bmp(bmpfd,500,300,0,103,4);
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
					show_any_bmp(bmpfd,500,300,x_x,y_y,2);
				}
				x_x=213;
				y_y=103;
				//创建光标显示线程
				pthread_create(&id1,NULL,register_cursor,NULL);
				
				flag=1;	
				
				bzero(pass,16);
				printf("请注册密码：\n");
				scanf("%s",pass);
				while(getchar()!='\n');
				write(sockfd,pass,strlen(pass));
				
				Display_characterXX(x_x+150,y_y+90,strlen(pass),pass,2,0xffffff);    //显示白色汉字
				x_x-=145;
				y_y-=90;
				break;
			case REGPHONE:       	   //注册手机号码
				write(sockfd,"regphone",strlen("regphone"));
				//刷新局部区域
				bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
				show_any_bmp(bmpfd,500,300,0,136,4);
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
					show_any_bmp(bmpfd,500,300,x_x,y_y,2);
				}
				x_x=213;
				y_y=136;
				//创建光标显示线程
				pthread_create(&id1,NULL,register_cursor,NULL);
				
				flag=1;	
				bzero(phone,sizeof(phone));
				printf("请输入注册手机号码：\n");
				scanf("%s",phone);
				while(getchar()!='\n');
				write(sockfd,phone,strlen(phone));

				Display_characterXX(x_x+150,y_y+90,strlen(phone),phone,2,0xffffff);    //显示白色汉字
				x_x-=145;
				y_y-=90;
				break;
			case REGISTER1:
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
					show_any_bmp(bmpfd,500,300,x_x,y_y,2);
				}
				write(sockfd,"reg",strlen("reg"));
				usleep(5000);
				//显示注册成功
				bmpfd = get_bmp_wh("interface/rgs.bmp",NULL,NULL);
				show_any_bmp(bmpfd,200,86,300,197,0);
				usleep(500000);
				return 0;
			case CANCEL1:
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
					show_any_bmp(bmpfd,500,300,x_x,y_y,2);
				}
				return -1;
		}	
	}
}

//显示登录界面
int longin_show()
{
	int flag = 0,mark = 0,phide=0,passh = 0;
	char users[20];
	char pass[16];
	char passhide[17] = "****************";
	char phone[12];
	char buf[20]={};
	int pflag=0,pmark=0;
	//显示登录界面
	int bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
	show_any_bmp(bmpfd,800,480,0,0,0);
	while(1)
	{
		int botton = act_screen();       //获取按键操作
		switch(botton)
		{
			case HIDE:    //隐藏密码
				if(phide==0)
				{
					if(passh==1)
					{
						//刷新局部区域
						bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
						show_any_bmp(bmpfd,800,480,0,205,3);
						Display_characterXX(290,205,strlen(pass),passhide,2,0xffffff);    //显示汉字
						x_x+=5;
					}
					phide=1;       //隐藏
				}
				else if(phide==1)
				{
					if(passh==1)
					{
						//刷新局部区域
						bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
						show_any_bmp(bmpfd,800,480,0,205,3);
						Display_characterXX(290,205,strlen(pass),pass,2,0xffffff);    //显示汉字
						x_x+=5;
					}
					phide=0;     //显示
				}
				break;
			case USERS:  //登录用户名
				write(sockfd,"users",strlen("users"));
				
				//刷新局部区域
				bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
				show_any_bmp(bmpfd,800,480,0,158,3);
				
				if(pflag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				x_x=290;
				y_y=158;
				//创建光标显示线程
				pthread_create(&id1,NULL,login_cursor,NULL);
				
				pflag=1;
				flag = 0;
				bzero(users,sizeof(users));
				//给服务器发送用户名信息
				printf("请输入用户名：\n");
				scanf("%s",users);
				while(getchar()!='\n');
				write(sockfd,users,strlen(users));  
			
				Display_characterXX(x_x,y_y,strlen(users),users,2,0xffffff);    //显示白色汉字
				x_x+=5;
				break;
			case PASSWORD:   //登录密码
				write(sockfd,"password",strlen("password"));
				
				//刷新局部区域
				bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
				show_any_bmp(bmpfd,800,480,0,205,3);
				
				if(pflag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				x_x=290;
				y_y=205;
				//创建光标显示线程
				pthread_create(&id1,NULL,login_cursor,NULL);
				
				pflag=1;
				flag = 0;
				bzero(pass,sizeof(pass));
				//给服务器发送密码信息
				printf("请输入密码：\n");
				scanf("%s",pass);
				while(getchar()!='\n');
				write(sockfd,pass,strlen(pass));
				if(phide==1)          //隐藏密码
					Display_characterXX(x_x,y_y,strlen(pass),passhide,2,0xffffff);    //显示汉字
				else if(phide==0)         //显示密码
					Display_characterXX(x_x,y_y,strlen(pass),pass,2,0xffffff);    //显示汉字
				x_x+=5;
				passh=1;
				break;
			case CHPHONE:        //验证手机号码
				if(pmark==1)
				{
					//刷新局部区域
					bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,0,253,3);
					if(pflag!=0)
					{
						pthread_cancel(id1);
						bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
						show_any_bmp(bmpfd,800,480,x_x,y_y,1);
					}
					x_x=290;
					y_y=253;
					//创建光标显示线程
					pthread_create(&id1,NULL,login_cursor,NULL);
					
					pflag=1;
					
					bzero(phone,sizeof(phone));
					//给服务器发送密码信息
					printf("请输入手机号码验证：\n");
					scanf("%s",phone);
					while(getchar()!='\n');
					
					Display_characterXX(x_x,y_y,strlen(phone),phone,2,0xffffff);    //显示汉字
					x_x+=5;
					flag = 3;
					sleep(1);
				}
				break;
			case REGISTER:  //注册
				flag = 0;
				mark = 0;
				pmark = 0;
				if(pflag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				printf("注册\n");
				apply_info();
				bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
				show_any_bmp(bmpfd,800,480,0,0,0);
				break;
			case LOGIN:  //登录
				printf("登录\n");
				if(pflag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				write(sockfd,"login",strlen("login"));
				
				bzero(buf,sizeof(buf));
				read(sockfd,buf,sizeof(buf));
				if(strcmp(buf,"unagree")==0)      //账号密码正确
				{
					flag = 1;
				}
				else if(strcmp(buf,"phone")==0)
				{
					flag = 2;
				}
				break;
			case CANCEL:   //退出系统
				printf("退出\n");
				return -1;
			case FORGET:  //忘记密码
				
				break;
		}
		if(flag==1)
		{
			//显示登录失败
			bmpfd = get_bmp_wh("interface/lgf.bmp",NULL,NULL);
			show_any_bmp(bmpfd,299,85,250,197,0);
			usleep(500000);
			mark++;
			bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
			show_any_bmp(bmpfd,800,480,0,0,0);
		}
		if(flag == 2)
		{
			memcpy(save_fb_mem,fb_mem,800*480*4);   //保存lcd数据
			
			bmpfd = get_bmp_wh("interface/chphone.bmp",NULL,NULL);
			show_any_bmp(bmpfd,200,100,300,140,0);
			sleep(1);
			memcpy(fb_mem,save_fb_mem,800*480*4);   //还原lcd数据
			
			bmpfd = get_bmp_wh("interface/phone.bmp",NULL,NULL);
			show_any_bmp(bmpfd,515,46,136,253,0);
			pmark=1;
		}
		if(flag == 3)
		{
			write(sockfd,phone,strlen(phone));
			
			bzero(buf,sizeof(buf));
			read(sockfd,buf,sizeof(buf));
			if(strcmp(buf,"agree")==0)      //账号密码正确
			{
				if(pflag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				 //显示登录成功
				bmpfd = get_bmp_wh("interface/lgs.bmp",NULL,NULL);
				show_any_bmp(bmpfd,200,86,300,197,0);
				usleep(500000);
				return 0;
			}	
			else
			{
				if(pflag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				 //显示验证失败
				bmpfd = get_bmp_wh("interface/chphonef.bmp",NULL,NULL);
				show_any_bmp(bmpfd,200,100,300,140,0);
				usleep(500000);
				mark++;
				bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
				show_any_bmp(bmpfd,800,480,0,0,0);
			}		
		}
		
		if(mark == 3)
			return -1;
	}
}

/*
		获取当天准备出发的列车信息
		
*/
struct klist *get_train_info(struct klist *head)
{
	write(sockfd,"getlist",strlen("getlist"));
	char buf[256];
	int i=0;
	while(1)
	{
		bzero(buf,256);
		read(sockfd,buf,sizeof(buf));
		if(strcmp(buf,"none")==0)
			break;
		
		//创建新节点
		struct klist *new = malloc(sizeof(struct klist));
		INIT_LIST_HEAD(&new->mylist);
		char *p = strtok(buf,"\t");               //拆分字符串
		strcpy(new->train_ID,p);         //车次
		p = strtok(NULL,"\t");
		strcpy(new->sname,p);            //出发地
		p = strtok(NULL,"\t");
		strcpy(new->ename,p);            //目的地
		p = strtok(NULL,"\t");
		strcpy(new->stime,p);            //发车时间
		list_add_tail(&(new->mylist), &(head->mylist)); //尾插
	}
	return head;
}

//显示当天准备出发的列车信息
int show_today_train(struct klist *head)
{
	if(pthflag == 1)
	{
		int bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
		show_any_bmp(bmpfd,800,480,0,0,0);
		pthflag = 0;
	}
	int i=0;
	struct klist *p;
	list_for_each_entry(p,&head->mylist,mylist)
	{
		if(104+i*35 >= 370)
			break;
		Display_characterXXX(95,104+i*35,strlen(p->train_ID),p->train_ID,2,0x00);
		Display_characterXXX(245,104+i*35,strlen(p->sname),p->sname,2,0x00);
		Display_characterXXX(425,104+i*35,strlen(p->ename),p->ename,2,0x00);
		Display_characterXXX(595,104+i*35,strlen(p->stime),p->stime,2,0x00);
		i++;
	}
	return 0;
}

//获取查询结果
struct klist *get_query_results(struct klist *que_head)
{
	write(sockfd,"check",strlen("check"));
	char buf[256];
	int i=0;
	while(1)
	{
		bzero(buf,256);
		read(sockfd,buf,sizeof(buf));
		if(strcmp(buf,"none")==0)
			break;
		i++;
		//创建新节点
		struct klist *new = malloc(sizeof(struct klist));
		INIT_LIST_HEAD(&new->mylist);
		char *p = strtok(buf,"\t");               //拆分字符串
		strcpy(new->train_ID,p);         //车次
		p = strtok(NULL,"\t");
		strcpy(new->sname,p);            //出发地
		p = strtok(NULL,"\t");
		strcpy(new->ename,p);            //目的地
		p = strtok(NULL,"\t");
		strcpy(new->stime,p);            //发车时间
		p = strtok(NULL,"\t");      
		strcpy(new->count,p);         	//票数
		p = strtok(NULL,"\n");
		strcpy(new->flag,p);             //特等座
		new->number = i;
		list_add_tail(&(new->mylist), &(que_head->mylist)); //尾插
	}
	return que_head;
}

//显示查询到的列车信息
int show_query_results(struct klist *head)
{
	int i=0;
	struct klist *p;
	char tempbuf[20];
	list_for_each_entry(p,&head->mylist,mylist)
	{
		
		if(104+i*35 >= 370)
			break;
		Display_characterXXX(90,104+i*35,strlen(p->train_ID),p->train_ID,2,0x00);
		Display_characterXXX(175,104+i*35,strlen(p->sname),p->sname,2,0x00);
		Display_characterXXX(275,104+i*35,strlen(p->ename),p->ename,2,0x00);
		bzero(tempbuf,20);
		strcpy(tempbuf,p->stime);
		char *s = strtok(tempbuf," ");
		Display_characterXXX(400,120+i*35,strlen(s),s,1,0x00);
		s = strtok(NULL,"\0");
		Display_characterXXX(410,104+i*35,strlen(s),s,1,0x00);
		Display_characterXXX(500,104+i*35,strlen(p->count),p->count,2,0x00);
		Display_characterXXX(650,104+i*35,strlen(p->flag),p->flag,2,0x00);
		i++;
	}
	return 0;
}
//刷新显示
void show_again(struct klist *p,int i)
{
	char tempbuf[20] = {};
	Display_characterXXX(90,104+i*35,strlen(p->train_ID),p->train_ID,2,0x00);
	Display_characterXXX(175,104+i*35,strlen(p->sname),p->sname,2,0x00);
	Display_characterXXX(275,104+i*35,strlen(p->ename),p->ename,2,0x00);
	strcpy(tempbuf,p->stime);
	char *s = strtok(tempbuf," ");
	Display_characterXXX(400,120+i*35,strlen(s),s,1,0x00);
	s = strtok(NULL,"\0");
	Display_characterXXX(410,104+i*35,strlen(s),s,1,0x00);
	Display_characterXXX(500,104+i*35,strlen(p->count),p->count,2,0x00);
	Display_characterXXX(650,104+i*35,strlen(p->flag),p->flag,2,0x00);
}
//显示已购买的列车票信息
int show_buy_results(struct klist *head)
{
	int i=0;
	struct klist *p;
	char tempbuf[20];
	list_for_each_entry(p,&head->mylist,mylist)
	{
		
		if(104+i*35 >= 370)
			break;
		Display_characterXXX(90,104+i*35,strlen(p->train_ID),p->train_ID,2,0x00);
		Display_characterXXX(175,104+i*35,strlen(p->sname),p->sname,2,0x00);
		Display_characterXXX(275,104+i*35,strlen(p->ename),p->ename,2,0x00);
		bzero(tempbuf,20);
		strcpy(tempbuf,p->stime);
		char *s = strtok(tempbuf," ");
		Display_characterXXX(400,120+i*35,strlen(s),s,1,0x00);
		s = strtok(NULL,"\0");
		Display_characterXXX(410,104+i*35,strlen(s),s,1,0x00);
		Display_characterXXX(500,104+i*35,strlen(p->count),p->count,2,0x00);
		i++;
	}
	return 0;
}

//购票
int buy_train(struct klist *p,int count)
{
	char tempcount[20];
	char arr[256];
	bzero(tempcount,20);
	strcpy(tempcount,p->count);
	char *s = strtok(tempcount,"/");
	s = strtok(NULL,"￥");
	int price = atoi(s)*count;
	int bmpfd = get_bmp_wh("interface/buy.bmp",NULL,NULL);
	show_any_bmp(bmpfd,400,200,200,50,0);
	bzero(arr,sizeof(arr));
	sprintf(arr,"你将购买由%s到%s的%s列车，将在%s开始发车，支付的金额为%d。",  \
					p->sname,p->ename,p->train_ID,p->stime,price);
	Display_characterXXXX(240,80,strlen(arr),arr,2,0x0000ff);
	
	
	 //初始化购票链表
	buy_head = init_list();
					
	char money[10];
	int flag=0;
	int small=0;   //找零
	int summoney=0;   //总金额
	while(1)
	{
		int botton = act_screen_BUY();       //获取按键操作
		switch(botton)
		{
			case MONEY:
				//刷新局部区域
				bmpfd = get_bmp_wh("interface/buy.bmp",NULL,NULL);
				show_any_bmp(bmpfd,400,200,90,156,7);
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/buy.bmp",NULL,NULL);
					show_any_bmp(bmpfd,400,200,x_x,y_y,8);
				}
				x_x=90;
				y_y=156;
				//创建光标显示线程
				pthread_create(&id1,NULL,buy_cursor,NULL);
				flag=1;
				
				bzero(money,sizeof(money));
				//给服务器发送用户名信息
				printf("请输入金额：\n");
				scanf("%s",money);
				while(getchar()!='\n');
				Display_characterXX(x_x+200,y_y+50,strlen(money),money,2,0xff);    //显示蓝色汉字
				x_x-=195;
				y_y-=50;
				summoney = atoi(money) + small;
				break;
			case PAY:
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/buy.bmp",NULL,NULL);
					show_any_bmp(bmpfd,400,200,x_x,y_y,8);
				}

				if(summoney == price)   //支付完成  购票成功
				{

					//从保存查询结果的链表里删除
					list_del(&(p->mylist));
					
					write(sockfd,"buyend",strlen("buyend"));     //发送购买标志，给服务器修改票数
					usleep(1000);
					write(sockfd,p->train_ID,strlen(p->train_ID));     //发送购买标志，给服务器修改票数
					usleep(1000);
					write(sockfd,&count,4);
					bzero(p->count,sizeof(p->count));
					sprintf(p->count,"%d/%d￥",count,atoi(s));
					list_add_tail(&(p->mylist), &(buy_head->mylist)); //尾插

					bmpfd = get_bmp_wh("interface/buys.bmp",NULL,NULL);
					show_any_bmp(bmpfd,200,100,300,190,0);
					sleep(1);
					return 0;
				}
				if(summoney < price)   //支付金额不足  还需支付
				{
					small = price-summoney;
					
					area_show_black(200,100,300,100,0xb4b4b4);    //显示灰色
					
					bzero(arr,sizeof(arr));
					sprintf(arr,"支付金额不足,还需支付%d元。",small);
					Display_characterXXXX(300,110,strlen(arr),arr,2,0xff);
					sleep(1);
					bmpfd = get_bmp_wh("interface/buy.bmp",NULL,NULL);
					show_any_bmp(bmpfd,400,200,200,50,0);
					bzero(arr,sizeof(arr));
					sprintf(arr,"你将购买由%s到%s的%s列车，将在%s开始发车，支付的金额为%d。",  \
									p->sname,p->ename,p->train_ID,p->stime,price);
					Display_characterXXXX(240,80,strlen(arr),arr,2,0x0000ff);
					
				}
				if(summoney > price)   //支付超出金额  找零钱
				{
					small = summoney - price;
					
					area_show_black(200,100,300,100,0xb4b4b4);    //显示灰色
					
					bzero(arr,sizeof(arr));
					sprintf(arr,"支付超出金额,找你%d元零钱。",small);
							
					Display_characterXXXX(300,110,strlen(arr),arr,2,0xff);
					sleep(1);
					//从保存查询结果的链表里删除
					list_del(&(p->mylist));
					
					write(sockfd,"buyend",strlen("buyend"));     //发送购买标志，给服务器修改票数
					usleep(1000);
					write(sockfd,p->train_ID,strlen(p->train_ID));     //发送购买标志，给服务器修改票数
					usleep(1000);
					write(sockfd,&count,4);
					bzero(p->count,sizeof(p->count));
					sprintf(p->count,"%d/%d￥",count,atoi(s));
					list_add_tail(&(p->mylist), &(buy_head->mylist)); //尾插
					bmpfd = get_bmp_wh("interface/buys.bmp",NULL,NULL);
					show_any_bmp(bmpfd,200,100,300,190,0);
					sleep(1);
					return 0;
				}
				break;
		}
	}
}


//查询列车结果
int query_results()
{
	//创建内核链表--->保存查询得到的列车信息
	struct klist *que_head = init_list();
	que_head=get_query_results(que_head);//获取查询结果
	
	//保存lcd屏幕的数据
	unsigned long *save_fb_mem_s = malloc(800*480*4);

	int bmpfd = get_bmp_wh("interface/query_results.bmp",NULL,NULL);
	show_any_bmp(bmpfd,700,330,45,49,0);
	show_query_results(que_head); //显示查询到的列车信息
	
	memcpy(save_fb_mem_s,fb_mem,800*480*4);   //保存lcd数据
	int count=0,flag=0;
	char countbuf[10];
	
	struct klist *p;
	while(1)
	{
		int botton = act_screen_XX();       //获取按键操作
		switch(botton)
		{
			case BACK:        //返回
				free_klist(que_head);
				free(save_fb_mem_s);
				return -1;
			case BUY:        //购票
				if(flag==1)
					buy_train(p,count);

				free_klist(que_head);
				free(save_fb_mem_s);
				return 0;
			case ONE:       //第一个位置
				p = fine_node(que_head,ONE-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //还原lcd数据
					
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,0,5);
					Display_characterXXX(559,380,strlen("票数:+ 0 -"),"票数:+ 0 -",2,0xff);
					show_again(p,0);
					flag=1;
				}
				break;
			case TWO:       //第二个位置
				p = fine_node(que_head,TWO-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //还原lcd数据
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,35,5);
					Display_characterXXX(559,380,strlen("票数:+ 0 -"),"票数:+ 0 -",2,0xff);
					show_again(p,1);
					flag=1;
				}
				break;
			case THREE:       //第三个位置
				p = fine_node(que_head,THREE-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //还原lcd数据
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,70,5);
					Display_characterXXX(559,380,strlen("票数：+ 0 -"),"票数：+ 0 -",2,0xff);
					show_again(p,2);
					flag=1;
				}
				break;
			case FOUR:       //第四个位置
				p = fine_node(que_head,FOUR-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //还原lcd数据
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,105,5);
					Display_characterXXX(559,380,strlen("票数:+ 0 -"),"票数:+ 0 -",2,0xff);
					show_again(p,3);
					flag=1;
				}
				break;
			case FIVE:       //第五个位置
				p = fine_node(que_head,FIVE-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //还原lcd数据
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,140,5);
					Display_characterXXX(559,380,strlen("票数:+ 0 -"),"票数:+ 0 -",2,0xff);
					show_again(p,4);
					flag=1;
				}
				break;
			case SIX:       //第六个位置
				p = fine_node(que_head,SIX-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //还原lcd数据
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,175,5);
					Display_characterXXX(559,380,strlen("票数:+ 0 -"),"票数:+ 0 -",2,0xff);
					show_again(p,5);
					flag=1;
				}
				break;
			case SEVEN:       //第七个位置
				p = fine_node(que_head,SEVEN-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //还原lcd数据
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,210,5);
					Display_characterXXX(559,380,strlen("票数:+ 0 -"),"票数:+ 0 -",2,0xff);
					show_again(p,6);
					flag=1;
				}
				break;
			case EIGHT:       //第八个位置
				p = fine_node(que_head,EIGHT-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //还原lcd数据
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,245,5);
					Display_characterXXX(559,380,strlen("票数:+ 0 -"),"票数:+ 0 -",2,0xff);
					show_again(p,7);
					flag=1;
				}
				break;
			case ADD:    //票数加
				if(flag==1)
				{
					count++;
					bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,665,380,1);
					bzero(countbuf,sizeof(countbuf));
					sprintf(countbuf,"%d",count);
					Display_characterXXX(668,380,strlen(countbuf),countbuf,2,0xff);
				}
				break;
			case DEL:    //票数减
				if(flag==1)
				{
					count--;
					bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,665,380,1);
					bzero(countbuf,sizeof(countbuf));
					sprintf(countbuf,"%d",count);
					Display_characterXXX(668,380,strlen(countbuf),countbuf,2,0xff);
				}
				break;
		}
	}
	
}

//初始化savebuf
int savebuf_init()
{
	bzero(savebuf,sizeof(savebuf));
	int i,j;
	for(j=0; j<32; j++)
		for(i=0; i<530*5; i++)
			savebuf[j*530*5+i] = 0x00;	
	return 0;
}
//显示savebuf
int show_savebuf(int k)
{
	int i,j;
	for(j=0; j<27; j++)
		for(i=0; i<530; i++)
			fb_mem[(j+8)*800+i+130] = savebuf[j*530*5+i+k];	
	return 0;
}

//显示轮播文本线程
void *show_turn_word(void *arg)
{
	savebuf_init();
	int k=0;
	while(1)
	{
		if(strlen(turnarr)!=0)
		{
			Display_characterX(0,0,strlen(turnarr),turnarr,2);
			if(k+530 > turnlen)
			{
				k=0;
				usleep(10000);
			}
				
			show_savebuf(k); 
			k+=1;
			usleep(50000);
		}
	}
}


//显示当天当前的时间
void *show_now_time(void *arg)
{
	time_t nowtime;
	char buf[20];
	int bmpfd;
	int temp;
	while(1)
	{
		nowtime = time(&nowtime);      //获取当前时间
		//转换成本地时间
		now = localtime(&nowtime);
		
		//相当于1秒刷新一次显示
		if(now->tm_sec!=temp)
		{
			bzero(buf,sizeof(buf));
			sprintf(buf,"%d:%d:%d",now->tm_hour,now->tm_min,now->tm_sec);
			bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
			show_any_bmp(bmpfd,800,480,0,0,6);
			if(now->tm_wday==0)
				Display_characterXXX(0,0,strlen("星期一"),"星期一",2,0xffffff);
			else if(now->tm_wday==1)
				Display_characterXXX(0,0,strlen("星期二"),"星期二",2,0xffffff);
			else if(now->tm_wday==2)
				Display_characterXXX(0,0,strlen("星期三"),"星期三",2,0xffffff);
			else if(now->tm_wday==3)
				Display_characterXXX(0,0,strlen("星期四"),"星期四",2,0xffffff);
			else if(now->tm_wday==4)
				Display_characterXXX(0,0,strlen("星期五"),"星期五",2,0xffffff);
			else if(now->tm_wday==5)
				Display_characterXXX(0,0,strlen("星期六"),"星期六",2,0xffffff);
			else if(now->tm_wday==6)
				Display_characterXXX(0,0,strlen("星期日"),"星期日",2,0xffffff);
			Display_characterXXX(0,27,strlen(buf),buf,2,0xffffff);
			temp=now->tm_sec;
		}
	}
}

//计算距离发车时间还有多久
void *compute(void *arg)
{
	struct klist *p;
	struct set_out *k;
	
	//初始化
	struct set_out *sethead = malloc(sizeof(struct set_out));
	INIT_LIST_HEAD(&(sethead->mylist));
	
	char timebuf[10];

	while(1)
	{	
		struct klist *n = list_entry(head->mylist.next, struct klist, mylist);
		bzero(timebuf,10);
		strcpy(timebuf,n->stime);
		char *s = strtok(timebuf,":");
		int min = atoi(s)- now->tm_hour;
		
		list_for_each_entry(p, &(head->mylist), mylist)
		{
			
			bzero(timebuf,10);
			strcpy(timebuf,p->stime);
			s = strtok(timebuf,":");
			if(min > atoi(s)- now->tm_hour)  //找到最小的数
			{
				min = atoi(s)- now->tm_hour;
				n = p;
			}
			else if(min == atoi(s)- now->tm_hour)  //相等的时候
			{
				if(p!=n)
				{
					//新节点
					struct set_out *new = malloc(sizeof(struct set_out));
					INIT_LIST_HEAD(&(new->mylist));	
					new->set_out = malloc(sizeof(struct set_out));
					new->set_out = p;
					list_add_tail(&new->mylist,&sethead->mylist);       //尾插
				}
			}
		}

		//新节点
		struct set_out *new = malloc(sizeof(struct set_out));
		INIT_LIST_HEAD(&(new->mylist));
		new->set_out = malloc(sizeof(struct set_out));
		new->set_out = n;
		list_add_tail(&new->mylist,&sethead->mylist);       //尾插
		
		struct set_out *m = list_entry(sethead->mylist.next, struct set_out, mylist);
		bzero(timebuf,10);
		strcpy(timebuf,m->set_out->stime);
		s = strtok(timebuf,":");
		s = strtok(NULL,"");
		min = atoi(s)- now->tm_min;
		
		list_for_each_entry(k, &(sethead->mylist), mylist)
		{
			bzero(timebuf,10);
			strcpy(timebuf,k->set_out->stime);
			s = strtok(timebuf,":");
			s = strtok(NULL,"");

			if(min > atoi(s)- now->tm_min)  //找到最小的数
			{
				min = atoi(s)- now->tm_min;
				m = k;
			}
		}
		sprintf(turnarr,"由%s到%s的%s列车准备发车了，请购买了此票的旅客到检票口检票。", \
						m->set_out->sname,m->set_out->ename,m->set_out->train_ID);
		bzero(timebuf,10);
		strcpy(timebuf,m->set_out->stime);
		s = strtok(timebuf,":");
		int timehour = atoi(s);   //小时
		s = strtok(NULL,"");
		int timemin = atoi(s);    //分
		if(timehour == now->tm_hour && timemin == now->tm_min)
		{
			list_for_each_entry(p, &(head->mylist), mylist)
			{
				if(strcmp(p->train_ID,m->set_out->train_ID)==0)
				{
					list_del(&(p->mylist));
					free(p);
					bzero(turnarr,sizeof(turnarr));
					savebuf_init();
					pthflag = 1;
					break;
				}
			}
		}
		
		
		sleep(1);
	}
	
}


//显示主界面
int menu()
{	
	//创建内核链表--->保存当前准备出发的列车信息
	head = init_list();

	
	int bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
	show_any_bmp(bmpfd,800,480,0,0,0);
	memcpy(save_fb_mem,fb_mem,800*480*4);   //保存lcd数据
	
	pthread_t pthid;
	pthread_attr_t myattr;
	pthread_attr_setdetachstate(&myattr,PTHREAD_CREATE_DETACHED);    //设置线程分离属性
	pthread_create(&pthid,&myattr,show_now_time,NULL);     //显示当前时间线程
	
	int flag=0,mark1=0,mark2=0,bflag=0;
	char sbuf[20];
	char ebuf[20];
	/*
		获取当天准备出发的列车信息
			
	*/
	head = get_train_info(head);

	pthread_create(&pid1,NULL,compute,NULL);     //计算距离发车时间还有多久
	
	pthread_create(&pid,NULL,show_turn_word,NULL);     //显示轮播文本线程
	
	while(1)
	{
		//显示当天准备出发的列车信息
		show_today_train(head);
		
		int botton = act_screen_X();       //获取按键操作
		switch(botton)
		{
			case CANCELL:        //退出
				free_klist(head);	
				pthread_cancel(pid1);
				pthread_cancel(pid);
				return 0;
			case START:        //输入起始地
				write(sockfd,"start",strlen("start"));
				//刷新局部区域
				bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
				show_any_bmp(bmpfd,800,480,110,380,6);
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				x_x=108;
				y_y=407;
				//创建光标显示线程
				pthread_create(&id1,NULL,train_cursor,NULL);
				flag=1;
				
				bzero(sbuf,sizeof(sbuf));
				//给服务器发送用户名信息
				printf("请输入起始地：\n");
				scanf("%s",sbuf);
				while(getchar()!='\n');
				write(sockfd,sbuf,strlen(sbuf));  
			
				Display_characterXX(x_x,y_y,strlen(sbuf),sbuf,2,0xffffff);    //显示白色汉字
				x_x+=5;
				if(mark1==0)
					mark2=1;
				else if(mark1==1)
					mark2=2;
				break;
			case END:        //输入目的地
				write(sockfd,"end",strlen("end"));
				//刷新局部区域
				bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
				show_any_bmp(bmpfd,800,480,330,380,6);
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				x_x=334;
				y_y=407;
				//创建光标显示线程
				pthread_create(&id1,NULL,train_cursor,NULL);
				flag=1;
				
				bzero(ebuf,sizeof(ebuf));
				//给服务器发送用户名信息
				printf("请输入目的地：\n");
				scanf("%s",ebuf);
				while(getchar()!='\n');
				write(sockfd,ebuf,strlen(ebuf));  
			
				Display_characterXX(x_x,y_y,strlen(ebuf),ebuf,2,0xffffff);    //显示白色汉字
				x_x+=5;
				if(mark2==0)
					mark1=1;
				else if(mark2==1)
					mark1=2;
				break;
			case CHECK:        //查询
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				if(mark1==2 || mark2==2)
				{	
					//查询列车结果
					query_results();
					mark1=0;
					mark2=0;
					bflag=1;
					memcpy(fb_mem,save_fb_mem,800*480*4);   //还原lcd数据
				}
				else
				{
					bmpfd = get_bmp_wh("interface/checkf.bmp",NULL,NULL);
					show_any_bmp(bmpfd,300,100,250,190,0);
					sleep(1);
					memcpy(fb_mem,save_fb_mem,800*480*4);   //还原lcd数据
				}
				break;
			case BUY:        //购票
				bmpfd = get_bmp_wh("interface/buyf.bmp",NULL,NULL);
				show_any_bmp(bmpfd,300,100,250,190,0);
				sleep(1);
				memcpy(fb_mem,save_fb_mem,800*480*4);   //还原lcd数据
				break;
			case RETURN:        //退票
				bmpfd = get_bmp_wh("interface/buy_results.bmp",NULL,NULL);
				show_any_bmp(bmpfd,700,330,45,49,0);
				if(bflag!=0)
					show_buy_results(buy_head); //显示已购买的列车票信息
				act_screen_X();
				memcpy(fb_mem,save_fb_mem,800*480*4);   //还原lcd数据
				break;
				
		}
	}
}


