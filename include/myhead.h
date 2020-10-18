#ifndef MYHEAD_H_
#define MYHEAD_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <dirent.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <kernel_list.h>

int sockfd;
pthread_t id1,id2,pid,pid1;

int x_x;
int y_y;

int turnlen;

struct klist
{
	char train_ID[6];               //车次
	char sname[20];                //出发点
	char ename[20];                //终点
	char stime[20];                //发车时间 --- 时：分
	char count[20];						//票数/票价
	char flag[2];						//特等座标志位
	int number;
	struct list_head mylist;
};

struct set_out
{
	struct klist *set_out;
	struct list_head mylist;
};

//当前时间结构体
struct tm *now;
//保存lcd屏幕的数据
unsigned long *save_fb_mem;

struct klist *head;              //当前准备出发列车信息头
struct klist *buy_head;          //购票信息

struct klist *init_list();

struct klist *fine_node(struct klist *head,int num);
int free_klist(struct klist *head);


#endif