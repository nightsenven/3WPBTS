#ifndef BOTTON_H_
#define BOTTON_H_

#define LOGIN 0       	//登录
#define REGISTER 1       	//注册
#define CANCEL 2       	//取消
#define FORGET 12       	//忘记密码

#define USERS 3      	//用户名
#define PASSWORD 4       	//密码
#define HIDE 5       	//隐藏
#define CHPHONE 6       	//输入手机号码

#define REGNAME 7       	//注册账号
#define REGPASS 8       	//注册密码
#define REGPHONE 9       	//注册手机号码
#define REGISTER1 10       	//开始注册
#define CANCEL1 11       	//取消注册

#define START 13       	//起始地
#define END 14       	//目的地
#define CHECK 15       	//查询
#define BUY 16       	//购票
#define RETURN 17       	//退票
#define BACK 18       	//退票
#define ADD 19       	//加
#define DEL 20       	//减



#define ONE 21          //第一个好友
#define TWO 22          //第二个好友
#define THREE 23          //第三个好友
#define FOUR 24          //第四个好友
#define FIVE 25          //第五个好友
#define SIX 26          //第六个好友
#define SEVEN 27          //第七个好友
#define EIGHT 28          //第八个好友


#define CANCELL 46       	//退出
#define MONEY 47       	//金额
#define PAY 48       	//支付


#define UP 31       	//向上划屏
#define DOWN 32			//向下划屏
#define LEFT 33			//向左划屏
#define RIGHT 34		//向右划屏

//与上面宏定义用法一样
// enum
// {
	// UP=31,
	// #define UP UP
	// DOWN,
	// #define DOWN DOWN
	// LEFT
// };
//划屏or触摸
int act_screen();
int act_screen_X();
int act_screen_XX();
int act_screen_XXX();
int act_screen_BUY();
#endif