#include <myhead.h>
#include <getinfo.h>
#include <botton.h>

#include <lcd.h>

char arr[300] = {0};
char turnarr[500] = {};
int chat_y;             //������Ϣ��ʾ��yֵ   
int pthflag=0;    //��־λ

//��ȡbmpͼƬ�Ŀ�͸�
int get_bmp_wh(char *bmp_name,int *w,int *h)
{
	
	int bmpfd = open(bmp_name,O_RDWR);//��BMPͼƬ
	if(bmpfd == -1)
	{
		perror("open err!");
		return -1;
	}
	if(w!=NULL&&h!=NULL)
	{
		//bmpͼƬǰ54���ֽڵĵ�18���ֽ�֮���8���ֽڷֱ𱣴���ͼƬ�Ŀ�͸�
		lseek(bmpfd,18,SEEK_SET);   
		read(bmpfd,w,4);  //��ȡbmpͼƬ�Ŀ�
		read(bmpfd,h,4);  //��ȡbmpͼƬ�ĸ�
	}
	return bmpfd;
}

//��ʾһ�������С��ͼƬ
int show_any_bmp(int bmpfd,int bmp_x,int bmp_y,int s_x,int s_y,int n)
{

	//����һ�����飬����ͼƬ�Ĵ�С
	char bmpbuf[bmp_x*bmp_y*3];
	int lcdbuf[bmp_x*bmp_y];
	int lastbuf[bmp_x*bmp_y];
	bzero(bmpbuf,sizeof(bmpbuf));
	bzero(lcdbuf,sizeof(lcdbuf));
	bzero(lastbuf,sizeof(lastbuf));
	
	lseek(bmpfd,54,SEEK_SET);  //��ͷƫ��ͼƬ������Ϣ54���ֽ�
	//��ȡbmpͼƬ��RGB����
	int i,j;
	for(i=0; i<bmp_y; i++)  //���ж�ȡ
	{
		read(bmpfd, &bmpbuf[i*bmp_x*3], bmp_x*3);
		int tmp;
		for(tmp=bmp_x*3; tmp%4 != 0; tmp++);
		lseek(bmpfd,tmp-bmp_x*3,SEEK_CUR);
	}
	
	//3�ֽ�ת����4�ֽ�
	for(i=0; i<bmp_x*bmp_y; i++)
		lcdbuf[i] = bmpbuf[i*3+2]<<16|bmpbuf[i*3+1]<<8|bmpbuf[i*3+0];
	
	//ͼƬ���еߵ�
	for(i=0; i<bmp_x; i++)   //��
		for(j=0; j<bmp_y; j++)  //��
			lastbuf[(bmp_y-1-j)*bmp_x+i] = lcdbuf[i+j*bmp_x];
	
	//������ʾ
	if(n==0)
	{
		//ӳ���ڴ�
		for(j=0; j<bmp_y; j++)
			for(i=0; i<bmp_x; i++)
				*(fb_mem+(j+s_y)*800+s_x+i) = lastbuf[j*bmp_x+i];
	}
	
	//ֻ��ʾ������ڵ����� 800*480
	if(n==1)
	{
		//ӳ���ڴ�
		for(j=s_y; j<s_y+30; j++)
			for(i=s_x; i<s_x+30; i++)
				*(fb_mem+j*800+i) = lastbuf[j*bmp_x+i];
	}
	//ֻ��ʾ������ڵ����� 500*300
	if(n==2)
	{
		//ӳ���ڴ�
		for(j=s_y; j<s_y+30; j++)
			for(i=s_x; i<s_x+30; i++)
				*(fb_mem+(j+90)*800+i+150) = lastbuf[j*bmp_x+i];
	}
	//ֻ��ʾ��������--->ˢ�¾ֲ�����  800*480
	if(n==3)
	{
		//ӳ���ڴ�
		for(j=s_y; j<s_y+30; j++)
			for(i=290; i<594; i++)
				*(fb_mem+j*800+i) = lastbuf[j*bmp_x+i];
	}
	
	//ֻ��ʾ��������--->ˢ�¾ֲ�����   500*300
	if(n==4)
	{
		//ӳ���ڴ�
		for(j=s_y; j<s_y+25; j++)
			for(i=213; i<402; i++)
				*(fb_mem+(j+90)*800+i+150) = lastbuf[j*bmp_x+i];    //��lcd����ʾ
	}
	//ֻ��ʾ��������--->ˢ�¾ֲ�����   700*270
	if(n==5)
	{
		//ӳ���ڴ�
		for(j=s_y; j<s_y+34; j++)
			for(i=0; i<700; i++)
				*(fb_mem+(j+105)*800+i+44) = lastbuf[j*bmp_x+i];    //��lcd����ʾ
	}
	//ֻ��ʾ��������--->ˢ�¾ֲ�����  800*480
	if(n==6)
	{
		//ӳ���ڴ�
		for(j=s_y; j<s_y+53; j++)
			for(i=s_x; i<s_x+120; i++)
				*(fb_mem+j*800+i) = lastbuf[j*bmp_x+i];
	}
	//ֻ��ʾ��������--->ˢ�¾ֲ�����  400*200
	if(n==7)
	{
		//ӳ���ڴ�
		for(j=s_y; j<s_y+44; j++)
			for(i=s_x; i<s_x+70; i++)
				*(fb_mem+(j+50)*800+i+200) = lastbuf[j*bmp_x+i];
	}
	//ֻ��ʾ������ڵ����� 400*200
	if(n==8)
	{
		//ӳ���ڴ�
		for(j=s_y; j<s_y+30; j++)
			for(i=s_x; i<s_x+30; i++)
				*(fb_mem+(j+50)*800+i+200) = lastbuf[j*bmp_x+i];
	}
	//�ر�
	close(bmpfd);
	return 0;
}

//ָ��������ʾ��ɫ
void area_show_black(int x,int y,int s_x,int s_y,int color)
{
	int i,j;
	//ӳ���ڴ�
	for(j=0; j<y; j++)
		for(i=0; i<x; i++)
			*(fb_mem+(j+s_y)*800+s_x+i) = color;
}


//��ʾ����߳�  --  ��¼����
void *login_cursor(void *arg)
{
	while(1)
	{
		Display_characterXXX(x_x,y_y,strlen("|"),"|",2,0x00);       //��ʾ���֩�
		usleep(700000);            //�ӳ�0.7s
		int bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
		show_any_bmp(bmpfd,800,480,x_x,y_y,1);
		usleep(700000);            //�ӳ�0.7s
	}
}

//��ʾ����߳�  --  ע�����
void *register_cursor(void *arg)
{
	while(1)
	{
		Display_characterXXX(x_x+150,y_y+90,strlen("|"),"|",2,0x00);       //��ʾ���֩�
		usleep(700000);            //�ӳ�0.7s
		int bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
		show_any_bmp(bmpfd,500,300,x_x,y_y,2);
		usleep(700000);            //�ӳ�0.7s
	}
}

//��ʾ����߳�  --  �г�����
void *train_cursor(void *arg)
{
	while(1)
	{
		Display_characterXXX(x_x,y_y,strlen("|"),"|",2,0x00);       //��ʾ���֩�
		usleep(700000);            //�ӳ�0.7s
		int bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
		show_any_bmp(bmpfd,800,480,x_x,y_y,1);
		usleep(700000);            //�ӳ�0.7s
	}
}

//��ʾ����߳�  --  �������
void *buy_cursor(void *arg)
{
	while(1)
	{
		Display_characterXXX(x_x+200,y_y+50,strlen("|"),"|",2,0x00);       //��ʾ���֩�
		usleep(700000);            //�ӳ�0.7s
		int bmpfd = get_bmp_wh("interface/buy.bmp",NULL,NULL);
		show_any_bmp(bmpfd,400,200,x_x,y_y,8);
		usleep(700000);            //�ӳ�0.7s
	}
}

//ע���¼��Ϣ
int apply_info()
{
	char name[20];
	char pass[16];
	char phone[12];
	//��ʾע�����
	int bmpfd = get_bmp_wh("interface/reg.bmp",NULL,NULL);
	show_any_bmp(bmpfd,500,300,150,90,0);
	
	int flag=0;
	while(1)
	{
		int botton = act_screen_XXX();
		switch(botton)
		{
			case REGNAME:    //ע���û���
			
				write(sockfd,"regname",strlen("regname"));
				//ˢ�¾ֲ�����
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
				//���������ʾ�߳�
				pthread_create(&id1,NULL,register_cursor,NULL);
				
				flag=1;	
				bzero(name,20);
				printf("��ע���û�����\n");
				scanf("%s",name);
				while(getchar()!='\n');
				write(sockfd,name,strlen(name));
		
				Display_characterXX(x_x+150,y_y+90,strlen(name),name,2,0xffffff);    //��ʾ��ɫ����
				x_x-=145;
				y_y-=90;
				break;
			case REGPASS:     //ע������
				write(sockfd,"regpass",strlen("regpass"));
				//ˢ�¾ֲ�����
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
				//���������ʾ�߳�
				pthread_create(&id1,NULL,register_cursor,NULL);
				
				flag=1;	
				
				bzero(pass,16);
				printf("��ע�����룺\n");
				scanf("%s",pass);
				while(getchar()!='\n');
				write(sockfd,pass,strlen(pass));
				
				Display_characterXX(x_x+150,y_y+90,strlen(pass),pass,2,0xffffff);    //��ʾ��ɫ����
				x_x-=145;
				y_y-=90;
				break;
			case REGPHONE:       	   //ע���ֻ�����
				write(sockfd,"regphone",strlen("regphone"));
				//ˢ�¾ֲ�����
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
				//���������ʾ�߳�
				pthread_create(&id1,NULL,register_cursor,NULL);
				
				flag=1;	
				bzero(phone,sizeof(phone));
				printf("������ע���ֻ����룺\n");
				scanf("%s",phone);
				while(getchar()!='\n');
				write(sockfd,phone,strlen(phone));

				Display_characterXX(x_x+150,y_y+90,strlen(phone),phone,2,0xffffff);    //��ʾ��ɫ����
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
				//��ʾע��ɹ�
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

//��ʾ��¼����
int longin_show()
{
	int flag = 0,mark = 0,phide=0,passh = 0;
	char users[20];
	char pass[16];
	char passhide[17] = "****************";
	char phone[12];
	char buf[20]={};
	int pflag=0,pmark=0;
	//��ʾ��¼����
	int bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
	show_any_bmp(bmpfd,800,480,0,0,0);
	while(1)
	{
		int botton = act_screen();       //��ȡ��������
		switch(botton)
		{
			case HIDE:    //��������
				if(phide==0)
				{
					if(passh==1)
					{
						//ˢ�¾ֲ�����
						bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
						show_any_bmp(bmpfd,800,480,0,205,3);
						Display_characterXX(290,205,strlen(pass),passhide,2,0xffffff);    //��ʾ����
						x_x+=5;
					}
					phide=1;       //����
				}
				else if(phide==1)
				{
					if(passh==1)
					{
						//ˢ�¾ֲ�����
						bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
						show_any_bmp(bmpfd,800,480,0,205,3);
						Display_characterXX(290,205,strlen(pass),pass,2,0xffffff);    //��ʾ����
						x_x+=5;
					}
					phide=0;     //��ʾ
				}
				break;
			case USERS:  //��¼�û���
				write(sockfd,"users",strlen("users"));
				
				//ˢ�¾ֲ�����
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
				//���������ʾ�߳�
				pthread_create(&id1,NULL,login_cursor,NULL);
				
				pflag=1;
				flag = 0;
				bzero(users,sizeof(users));
				//�������������û�����Ϣ
				printf("�������û�����\n");
				scanf("%s",users);
				while(getchar()!='\n');
				write(sockfd,users,strlen(users));  
			
				Display_characterXX(x_x,y_y,strlen(users),users,2,0xffffff);    //��ʾ��ɫ����
				x_x+=5;
				break;
			case PASSWORD:   //��¼����
				write(sockfd,"password",strlen("password"));
				
				//ˢ�¾ֲ�����
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
				//���������ʾ�߳�
				pthread_create(&id1,NULL,login_cursor,NULL);
				
				pflag=1;
				flag = 0;
				bzero(pass,sizeof(pass));
				//������������������Ϣ
				printf("���������룺\n");
				scanf("%s",pass);
				while(getchar()!='\n');
				write(sockfd,pass,strlen(pass));
				if(phide==1)          //��������
					Display_characterXX(x_x,y_y,strlen(pass),passhide,2,0xffffff);    //��ʾ����
				else if(phide==0)         //��ʾ����
					Display_characterXX(x_x,y_y,strlen(pass),pass,2,0xffffff);    //��ʾ����
				x_x+=5;
				passh=1;
				break;
			case CHPHONE:        //��֤�ֻ�����
				if(pmark==1)
				{
					//ˢ�¾ֲ�����
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
					//���������ʾ�߳�
					pthread_create(&id1,NULL,login_cursor,NULL);
					
					pflag=1;
					
					bzero(phone,sizeof(phone));
					//������������������Ϣ
					printf("�������ֻ�������֤��\n");
					scanf("%s",phone);
					while(getchar()!='\n');
					
					Display_characterXX(x_x,y_y,strlen(phone),phone,2,0xffffff);    //��ʾ����
					x_x+=5;
					flag = 3;
					sleep(1);
				}
				break;
			case REGISTER:  //ע��
				flag = 0;
				mark = 0;
				pmark = 0;
				if(pflag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				printf("ע��\n");
				apply_info();
				bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
				show_any_bmp(bmpfd,800,480,0,0,0);
				break;
			case LOGIN:  //��¼
				printf("��¼\n");
				if(pflag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				write(sockfd,"login",strlen("login"));
				
				bzero(buf,sizeof(buf));
				read(sockfd,buf,sizeof(buf));
				if(strcmp(buf,"unagree")==0)      //�˺�������ȷ
				{
					flag = 1;
				}
				else if(strcmp(buf,"phone")==0)
				{
					flag = 2;
				}
				break;
			case CANCEL:   //�˳�ϵͳ
				printf("�˳�\n");
				return -1;
			case FORGET:  //��������
				
				break;
		}
		if(flag==1)
		{
			//��ʾ��¼ʧ��
			bmpfd = get_bmp_wh("interface/lgf.bmp",NULL,NULL);
			show_any_bmp(bmpfd,299,85,250,197,0);
			usleep(500000);
			mark++;
			bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
			show_any_bmp(bmpfd,800,480,0,0,0);
		}
		if(flag == 2)
		{
			memcpy(save_fb_mem,fb_mem,800*480*4);   //����lcd����
			
			bmpfd = get_bmp_wh("interface/chphone.bmp",NULL,NULL);
			show_any_bmp(bmpfd,200,100,300,140,0);
			sleep(1);
			memcpy(fb_mem,save_fb_mem,800*480*4);   //��ԭlcd����
			
			bmpfd = get_bmp_wh("interface/phone.bmp",NULL,NULL);
			show_any_bmp(bmpfd,515,46,136,253,0);
			pmark=1;
		}
		if(flag == 3)
		{
			write(sockfd,phone,strlen(phone));
			
			bzero(buf,sizeof(buf));
			read(sockfd,buf,sizeof(buf));
			if(strcmp(buf,"agree")==0)      //�˺�������ȷ
			{
				if(pflag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/login.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				 //��ʾ��¼�ɹ�
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
				 //��ʾ��֤ʧ��
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
		��ȡ����׼���������г���Ϣ
		
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
		
		//�����½ڵ�
		struct klist *new = malloc(sizeof(struct klist));
		INIT_LIST_HEAD(&new->mylist);
		char *p = strtok(buf,"\t");               //����ַ���
		strcpy(new->train_ID,p);         //����
		p = strtok(NULL,"\t");
		strcpy(new->sname,p);            //������
		p = strtok(NULL,"\t");
		strcpy(new->ename,p);            //Ŀ�ĵ�
		p = strtok(NULL,"\t");
		strcpy(new->stime,p);            //����ʱ��
		list_add_tail(&(new->mylist), &(head->mylist)); //β��
	}
	return head;
}

//��ʾ����׼���������г���Ϣ
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

//��ȡ��ѯ���
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
		//�����½ڵ�
		struct klist *new = malloc(sizeof(struct klist));
		INIT_LIST_HEAD(&new->mylist);
		char *p = strtok(buf,"\t");               //����ַ���
		strcpy(new->train_ID,p);         //����
		p = strtok(NULL,"\t");
		strcpy(new->sname,p);            //������
		p = strtok(NULL,"\t");
		strcpy(new->ename,p);            //Ŀ�ĵ�
		p = strtok(NULL,"\t");
		strcpy(new->stime,p);            //����ʱ��
		p = strtok(NULL,"\t");      
		strcpy(new->count,p);         	//Ʊ��
		p = strtok(NULL,"\n");
		strcpy(new->flag,p);             //�ص���
		new->number = i;
		list_add_tail(&(new->mylist), &(que_head->mylist)); //β��
	}
	return que_head;
}

//��ʾ��ѯ�����г���Ϣ
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
//ˢ����ʾ
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
//��ʾ�ѹ�����г�Ʊ��Ϣ
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

//��Ʊ
int buy_train(struct klist *p,int count)
{
	char tempcount[20];
	char arr[256];
	bzero(tempcount,20);
	strcpy(tempcount,p->count);
	char *s = strtok(tempcount,"/");
	s = strtok(NULL,"��");
	int price = atoi(s)*count;
	int bmpfd = get_bmp_wh("interface/buy.bmp",NULL,NULL);
	show_any_bmp(bmpfd,400,200,200,50,0);
	bzero(arr,sizeof(arr));
	sprintf(arr,"�㽫������%s��%s��%s�г�������%s��ʼ������֧���Ľ��Ϊ%d��",  \
					p->sname,p->ename,p->train_ID,p->stime,price);
	Display_characterXXXX(240,80,strlen(arr),arr,2,0x0000ff);
	
	
	 //��ʼ����Ʊ����
	buy_head = init_list();
					
	char money[10];
	int flag=0;
	int small=0;   //����
	int summoney=0;   //�ܽ��
	while(1)
	{
		int botton = act_screen_BUY();       //��ȡ��������
		switch(botton)
		{
			case MONEY:
				//ˢ�¾ֲ�����
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
				//���������ʾ�߳�
				pthread_create(&id1,NULL,buy_cursor,NULL);
				flag=1;
				
				bzero(money,sizeof(money));
				//�������������û�����Ϣ
				printf("�������\n");
				scanf("%s",money);
				while(getchar()!='\n');
				Display_characterXX(x_x+200,y_y+50,strlen(money),money,2,0xff);    //��ʾ��ɫ����
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

				if(summoney == price)   //֧�����  ��Ʊ�ɹ�
				{

					//�ӱ����ѯ�����������ɾ��
					list_del(&(p->mylist));
					
					write(sockfd,"buyend",strlen("buyend"));     //���͹����־�����������޸�Ʊ��
					usleep(1000);
					write(sockfd,p->train_ID,strlen(p->train_ID));     //���͹����־�����������޸�Ʊ��
					usleep(1000);
					write(sockfd,&count,4);
					bzero(p->count,sizeof(p->count));
					sprintf(p->count,"%d/%d��",count,atoi(s));
					list_add_tail(&(p->mylist), &(buy_head->mylist)); //β��

					bmpfd = get_bmp_wh("interface/buys.bmp",NULL,NULL);
					show_any_bmp(bmpfd,200,100,300,190,0);
					sleep(1);
					return 0;
				}
				if(summoney < price)   //֧������  ����֧��
				{
					small = price-summoney;
					
					area_show_black(200,100,300,100,0xb4b4b4);    //��ʾ��ɫ
					
					bzero(arr,sizeof(arr));
					sprintf(arr,"֧������,����֧��%dԪ��",small);
					Display_characterXXXX(300,110,strlen(arr),arr,2,0xff);
					sleep(1);
					bmpfd = get_bmp_wh("interface/buy.bmp",NULL,NULL);
					show_any_bmp(bmpfd,400,200,200,50,0);
					bzero(arr,sizeof(arr));
					sprintf(arr,"�㽫������%s��%s��%s�г�������%s��ʼ������֧���Ľ��Ϊ%d��",  \
									p->sname,p->ename,p->train_ID,p->stime,price);
					Display_characterXXXX(240,80,strlen(arr),arr,2,0x0000ff);
					
				}
				if(summoney > price)   //֧���������  ����Ǯ
				{
					small = summoney - price;
					
					area_show_black(200,100,300,100,0xb4b4b4);    //��ʾ��ɫ
					
					bzero(arr,sizeof(arr));
					sprintf(arr,"֧���������,����%dԪ��Ǯ��",small);
							
					Display_characterXXXX(300,110,strlen(arr),arr,2,0xff);
					sleep(1);
					//�ӱ����ѯ�����������ɾ��
					list_del(&(p->mylist));
					
					write(sockfd,"buyend",strlen("buyend"));     //���͹����־�����������޸�Ʊ��
					usleep(1000);
					write(sockfd,p->train_ID,strlen(p->train_ID));     //���͹����־�����������޸�Ʊ��
					usleep(1000);
					write(sockfd,&count,4);
					bzero(p->count,sizeof(p->count));
					sprintf(p->count,"%d/%d��",count,atoi(s));
					list_add_tail(&(p->mylist), &(buy_head->mylist)); //β��
					bmpfd = get_bmp_wh("interface/buys.bmp",NULL,NULL);
					show_any_bmp(bmpfd,200,100,300,190,0);
					sleep(1);
					return 0;
				}
				break;
		}
	}
}


//��ѯ�г����
int query_results()
{
	//�����ں�����--->�����ѯ�õ����г���Ϣ
	struct klist *que_head = init_list();
	que_head=get_query_results(que_head);//��ȡ��ѯ���
	
	//����lcd��Ļ������
	unsigned long *save_fb_mem_s = malloc(800*480*4);

	int bmpfd = get_bmp_wh("interface/query_results.bmp",NULL,NULL);
	show_any_bmp(bmpfd,700,330,45,49,0);
	show_query_results(que_head); //��ʾ��ѯ�����г���Ϣ
	
	memcpy(save_fb_mem_s,fb_mem,800*480*4);   //����lcd����
	int count=0,flag=0;
	char countbuf[10];
	
	struct klist *p;
	while(1)
	{
		int botton = act_screen_XX();       //��ȡ��������
		switch(botton)
		{
			case BACK:        //����
				free_klist(que_head);
				free(save_fb_mem_s);
				return -1;
			case BUY:        //��Ʊ
				if(flag==1)
					buy_train(p,count);

				free_klist(que_head);
				free(save_fb_mem_s);
				return 0;
			case ONE:       //��һ��λ��
				p = fine_node(que_head,ONE-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //��ԭlcd����
					
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,0,5);
					Display_characterXXX(559,380,strlen("Ʊ��:+ 0 -"),"Ʊ��:+ 0 -",2,0xff);
					show_again(p,0);
					flag=1;
				}
				break;
			case TWO:       //�ڶ���λ��
				p = fine_node(que_head,TWO-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //��ԭlcd����
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,35,5);
					Display_characterXXX(559,380,strlen("Ʊ��:+ 0 -"),"Ʊ��:+ 0 -",2,0xff);
					show_again(p,1);
					flag=1;
				}
				break;
			case THREE:       //������λ��
				p = fine_node(que_head,THREE-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //��ԭlcd����
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,70,5);
					Display_characterXXX(559,380,strlen("Ʊ����+ 0 -"),"Ʊ����+ 0 -",2,0xff);
					show_again(p,2);
					flag=1;
				}
				break;
			case FOUR:       //���ĸ�λ��
				p = fine_node(que_head,FOUR-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //��ԭlcd����
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,105,5);
					Display_characterXXX(559,380,strlen("Ʊ��:+ 0 -"),"Ʊ��:+ 0 -",2,0xff);
					show_again(p,3);
					flag=1;
				}
				break;
			case FIVE:       //�����λ��
				p = fine_node(que_head,FIVE-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //��ԭlcd����
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,140,5);
					Display_characterXXX(559,380,strlen("Ʊ��:+ 0 -"),"Ʊ��:+ 0 -",2,0xff);
					show_again(p,4);
					flag=1;
				}
				break;
			case SIX:       //������λ��
				p = fine_node(que_head,SIX-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //��ԭlcd����
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,175,5);
					Display_characterXXX(559,380,strlen("Ʊ��:+ 0 -"),"Ʊ��:+ 0 -",2,0xff);
					show_again(p,5);
					flag=1;
				}
				break;
			case SEVEN:       //���߸�λ��
				p = fine_node(que_head,SEVEN-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //��ԭlcd����
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,210,5);
					Display_characterXXX(559,380,strlen("Ʊ��:+ 0 -"),"Ʊ��:+ 0 -",2,0xff);
					show_again(p,6);
					flag=1;
				}
				break;
			case EIGHT:       //�ڰ˸�λ��
				p = fine_node(que_head,EIGHT-20);
				if(p)
				{
					if(flag!=0)
						memcpy(fb_mem,save_fb_mem_s,800*480*4);   //��ԭlcd����
					bmpfd = get_bmp_wh("interface/choose.bmp",NULL,NULL);
					show_any_bmp(bmpfd,700,270,0,245,5);
					Display_characterXXX(559,380,strlen("Ʊ��:+ 0 -"),"Ʊ��:+ 0 -",2,0xff);
					show_again(p,7);
					flag=1;
				}
				break;
			case ADD:    //Ʊ����
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
			case DEL:    //Ʊ����
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

//��ʼ��savebuf
int savebuf_init()
{
	bzero(savebuf,sizeof(savebuf));
	int i,j;
	for(j=0; j<32; j++)
		for(i=0; i<530*5; i++)
			savebuf[j*530*5+i] = 0x00;	
	return 0;
}
//��ʾsavebuf
int show_savebuf(int k)
{
	int i,j;
	for(j=0; j<27; j++)
		for(i=0; i<530; i++)
			fb_mem[(j+8)*800+i+130] = savebuf[j*530*5+i+k];	
	return 0;
}

//��ʾ�ֲ��ı��߳�
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


//��ʾ���쵱ǰ��ʱ��
void *show_now_time(void *arg)
{
	time_t nowtime;
	char buf[20];
	int bmpfd;
	int temp;
	while(1)
	{
		nowtime = time(&nowtime);      //��ȡ��ǰʱ��
		//ת���ɱ���ʱ��
		now = localtime(&nowtime);
		
		//�൱��1��ˢ��һ����ʾ
		if(now->tm_sec!=temp)
		{
			bzero(buf,sizeof(buf));
			sprintf(buf,"%d:%d:%d",now->tm_hour,now->tm_min,now->tm_sec);
			bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
			show_any_bmp(bmpfd,800,480,0,0,6);
			if(now->tm_wday==0)
				Display_characterXXX(0,0,strlen("����һ"),"����һ",2,0xffffff);
			else if(now->tm_wday==1)
				Display_characterXXX(0,0,strlen("���ڶ�"),"���ڶ�",2,0xffffff);
			else if(now->tm_wday==2)
				Display_characterXXX(0,0,strlen("������"),"������",2,0xffffff);
			else if(now->tm_wday==3)
				Display_characterXXX(0,0,strlen("������"),"������",2,0xffffff);
			else if(now->tm_wday==4)
				Display_characterXXX(0,0,strlen("������"),"������",2,0xffffff);
			else if(now->tm_wday==5)
				Display_characterXXX(0,0,strlen("������"),"������",2,0xffffff);
			else if(now->tm_wday==6)
				Display_characterXXX(0,0,strlen("������"),"������",2,0xffffff);
			Display_characterXXX(0,27,strlen(buf),buf,2,0xffffff);
			temp=now->tm_sec;
		}
	}
}

//������뷢��ʱ�仹�ж��
void *compute(void *arg)
{
	struct klist *p;
	struct set_out *k;
	
	//��ʼ��
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
			if(min > atoi(s)- now->tm_hour)  //�ҵ���С����
			{
				min = atoi(s)- now->tm_hour;
				n = p;
			}
			else if(min == atoi(s)- now->tm_hour)  //��ȵ�ʱ��
			{
				if(p!=n)
				{
					//�½ڵ�
					struct set_out *new = malloc(sizeof(struct set_out));
					INIT_LIST_HEAD(&(new->mylist));	
					new->set_out = malloc(sizeof(struct set_out));
					new->set_out = p;
					list_add_tail(&new->mylist,&sethead->mylist);       //β��
				}
			}
		}

		//�½ڵ�
		struct set_out *new = malloc(sizeof(struct set_out));
		INIT_LIST_HEAD(&(new->mylist));
		new->set_out = malloc(sizeof(struct set_out));
		new->set_out = n;
		list_add_tail(&new->mylist,&sethead->mylist);       //β��
		
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

			if(min > atoi(s)- now->tm_min)  //�ҵ���С����
			{
				min = atoi(s)- now->tm_min;
				m = k;
			}
		}
		sprintf(turnarr,"��%s��%s��%s�г�׼�������ˣ��빺���˴�Ʊ���ÿ͵���Ʊ�ڼ�Ʊ��", \
						m->set_out->sname,m->set_out->ename,m->set_out->train_ID);
		bzero(timebuf,10);
		strcpy(timebuf,m->set_out->stime);
		s = strtok(timebuf,":");
		int timehour = atoi(s);   //Сʱ
		s = strtok(NULL,"");
		int timemin = atoi(s);    //��
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


//��ʾ������
int menu()
{	
	//�����ں�����--->���浱ǰ׼���������г���Ϣ
	head = init_list();

	
	int bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
	show_any_bmp(bmpfd,800,480,0,0,0);
	memcpy(save_fb_mem,fb_mem,800*480*4);   //����lcd����
	
	pthread_t pthid;
	pthread_attr_t myattr;
	pthread_attr_setdetachstate(&myattr,PTHREAD_CREATE_DETACHED);    //�����̷߳�������
	pthread_create(&pthid,&myattr,show_now_time,NULL);     //��ʾ��ǰʱ���߳�
	
	int flag=0,mark1=0,mark2=0,bflag=0;
	char sbuf[20];
	char ebuf[20];
	/*
		��ȡ����׼���������г���Ϣ
			
	*/
	head = get_train_info(head);

	pthread_create(&pid1,NULL,compute,NULL);     //������뷢��ʱ�仹�ж��
	
	pthread_create(&pid,NULL,show_turn_word,NULL);     //��ʾ�ֲ��ı��߳�
	
	while(1)
	{
		//��ʾ����׼���������г���Ϣ
		show_today_train(head);
		
		int botton = act_screen_X();       //��ȡ��������
		switch(botton)
		{
			case CANCELL:        //�˳�
				free_klist(head);	
				pthread_cancel(pid1);
				pthread_cancel(pid);
				return 0;
			case START:        //������ʼ��
				write(sockfd,"start",strlen("start"));
				//ˢ�¾ֲ�����
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
				//���������ʾ�߳�
				pthread_create(&id1,NULL,train_cursor,NULL);
				flag=1;
				
				bzero(sbuf,sizeof(sbuf));
				//�������������û�����Ϣ
				printf("��������ʼ�أ�\n");
				scanf("%s",sbuf);
				while(getchar()!='\n');
				write(sockfd,sbuf,strlen(sbuf));  
			
				Display_characterXX(x_x,y_y,strlen(sbuf),sbuf,2,0xffffff);    //��ʾ��ɫ����
				x_x+=5;
				if(mark1==0)
					mark2=1;
				else if(mark1==1)
					mark2=2;
				break;
			case END:        //����Ŀ�ĵ�
				write(sockfd,"end",strlen("end"));
				//ˢ�¾ֲ�����
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
				//���������ʾ�߳�
				pthread_create(&id1,NULL,train_cursor,NULL);
				flag=1;
				
				bzero(ebuf,sizeof(ebuf));
				//�������������û�����Ϣ
				printf("������Ŀ�ĵأ�\n");
				scanf("%s",ebuf);
				while(getchar()!='\n');
				write(sockfd,ebuf,strlen(ebuf));  
			
				Display_characterXX(x_x,y_y,strlen(ebuf),ebuf,2,0xffffff);    //��ʾ��ɫ����
				x_x+=5;
				if(mark2==0)
					mark1=1;
				else if(mark2==1)
					mark1=2;
				break;
			case CHECK:        //��ѯ
				if(flag!=0)
				{
					pthread_cancel(id1);
					bmpfd = get_bmp_wh("interface/train.bmp",NULL,NULL);
					show_any_bmp(bmpfd,800,480,x_x,y_y,1);
				}
				if(mark1==2 || mark2==2)
				{	
					//��ѯ�г����
					query_results();
					mark1=0;
					mark2=0;
					bflag=1;
					memcpy(fb_mem,save_fb_mem,800*480*4);   //��ԭlcd����
				}
				else
				{
					bmpfd = get_bmp_wh("interface/checkf.bmp",NULL,NULL);
					show_any_bmp(bmpfd,300,100,250,190,0);
					sleep(1);
					memcpy(fb_mem,save_fb_mem,800*480*4);   //��ԭlcd����
				}
				break;
			case BUY:        //��Ʊ
				bmpfd = get_bmp_wh("interface/buyf.bmp",NULL,NULL);
				show_any_bmp(bmpfd,300,100,250,190,0);
				sleep(1);
				memcpy(fb_mem,save_fb_mem,800*480*4);   //��ԭlcd����
				break;
			case RETURN:        //��Ʊ
				bmpfd = get_bmp_wh("interface/buy_results.bmp",NULL,NULL);
				show_any_bmp(bmpfd,700,330,45,49,0);
				if(bflag!=0)
					show_buy_results(buy_head); //��ʾ�ѹ�����г�Ʊ��Ϣ
				act_screen_X();
				memcpy(fb_mem,save_fb_mem,800*480*4);   //��ԭlcd����
				break;
				
		}
	}
}


