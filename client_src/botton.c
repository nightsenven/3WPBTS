#include <myhead.h>
#include <getinfo.h>
#include <botton.h>


//划屏or触摸
int act_screen()
{
	int start_x,start_y,end_x,end_y;
	get_screen_act(&start_x,&start_y,&end_x,&end_y);   //获取坐标	
	if(abs(end_x-start_x)<=5 && abs(end_y-start_y)<=5)
	{
		if(start_x>175 && start_x<278 && start_y>350 && start_y<383)   //登录
			return LOGIN;
		if(start_x>530 && start_x<635 && start_y>350 && start_y<383)   //注册
			return REGISTER;
		if(start_x>740 && start_x<800 && start_y>0 && start_y<30)   //取消
			return CANCEL;
		if(start_x>284 && start_x<357 && start_y>367 && start_y<382)   //忘记密码
			return FORGET;
			
		if(start_x>290 && start_x<593 && start_y>160 && start_y<190)   //输入用户名
			return USERS;
		if(start_x>290 && start_x<593 && start_y>200 && start_y<238)   //输入密码
			return PASSWORD;
		if(start_x>605 && start_x<645 && start_y>200 && start_y<238)   //隐藏密码
			return HIDE;
		if(start_x>290 && start_x<593 && start_y>253 && start_y<284)   //输入手机号码
			return CHPHONE;
			
		
	}
}
//按键
int act_screen_XXX()
{
	int start_x,start_y,end_x,end_y;
	get_screen_act(&start_x,&start_y,&end_x,&end_y);   //获取坐标
	if(abs(end_x-start_x)<=5 && abs(end_y-start_y)<=5)
	{
		if(start_x>363 && start_x<552 && start_y>160 && start_y<188)   //注册账号
			return REGNAME;
		if(start_x>363 && start_x<552 && start_y>193 && start_y<220)   //注册密码
			return REGPASS;
		if(start_x>363 && start_x<552 && start_y>224 && start_y<253)   //注册手机号码
			return REGPHONE;
		if(start_x>224 && start_x<305 && start_y>312 && start_y<350)   //开始注册
			return REGISTER1;
		if(start_x>528 && start_x<610 && start_y>312 && start_y<350)   //取消注册
			return CANCEL1;
	}
}
//按键
int act_screen_BUY()
{
	int start_x,start_y,end_x,end_y;
	get_screen_act(&start_x,&start_y,&end_x,&end_y);   //获取坐标
	if(abs(end_x-start_x)<=5 && abs(end_y-start_y)<=5)
	{
		if(start_x>300 && start_x<360 && start_y>200 && start_y<232)   //输入金额
			return MONEY;
		if(start_x>506 && start_x<574 && start_y>206 && start_y<235)   //支付
			return PAY;
	}
}
//按键
int act_screen_X()
{
	int start_x,start_y,end_x,end_y;
	get_screen_act(&start_x,&start_y,&end_x,&end_y);   //获取坐标	
	if(abs(end_x-start_x)<=5 && abs(end_y-start_y)<=5)
	{
		if(start_x>750 && start_x<800 && start_y>0 && start_y<30)   //退出主界面
			return CANCELL;
			
		if(start_x>113 && start_x<204 && start_y>410 && start_y<440)   //输入起始地
			return START;
		if(start_x>339 && start_x<429 && start_y>410 && start_y<440)   //输入目的地
			return END;	
		if(start_x>458 && start_x<534 && start_y>412 && start_y<444)   //查询
			return CHECK;		
		if(start_x>590 && start_x<665 && start_y>412 && start_y<444)   //购票
			return BUY;	
		if(start_x>715 && start_x<790 && start_y>412 && start_y<444)   //退票
			return RETURN;	
			
	}
	else if(end_y-start_y>0 && end_y-start_y > abs(end_x-start_x))   //对应x，y坐标的差值绝对值比较
	{
		//你向下划屏了！
		return DOWN;
	}								           
	else if(end_y-start_y<0 && (abs(end_y-start_y) > abs(end_x-start_x)))   //对应x，y坐标的差值绝对值比较
	{
		//你向上划屏了！
		return UP;
	}	
	else if(end_x-start_x<0 && (abs(end_y-start_y)<abs(end_x-start_x)))  //对应x，y坐标的差值绝对值比较
	{
		//你向左划屏了！
		return LEFT;
	}	
	else if(end_x-start_x>0 && end_x-start_x > abs(end_y-start_y))   //对应x，y坐标的差值绝对值比较
	{
		//你向右划屏了！
		return RIGHT;
	}
}
//按键
int act_screen_XX()
{
	int start_x,start_y,end_x,end_y;
	get_screen_act(&start_x,&start_y,&end_x,&end_y);   //获取坐标	
	if(abs(end_x-start_x)<=5 && abs(end_y-start_y)<=5)
	{
				
		if(start_x>590 && start_x<665 && start_y>412 && start_y<444)   //购票
			return BUY;	

		else if(start_x>70 && start_x<700 && start_y>104 && start_y<134)   //第一个位置
			return ONE;	
		else if(start_x>70 && start_x<700 && start_y>139 && start_y<169)   //第二个位置
			return TWO;
		else if(start_x>70 && start_x<700 && start_y>174 && start_y<204)   //第三个位置
			return THREE;	
		else if(start_x>70 && start_x<700 && start_y>209 && start_y<239)   //第四个位置
			return FOUR;	
		else if(start_x>70 && start_x<700 && start_y>244 && start_y<274)   //第五个位置
			return FIVE;
		else if(start_x>70 && start_x<700 && start_y>279 && start_y<309)   //第六个位置
			return SIX;
		else if(start_x>70 && start_x<700 && start_y>314 && start_y<344)   //第七个位置
			return SEVEN;		
		else if(start_x>70 && start_x<700 && start_y>349 && start_y<379)   //第八个位置
			return EIGHT;
		else if(start_x>638 && start_x<670 && start_y>380 && start_y<410)   //数量加
			return ADD;
		else if(start_x>699 && start_x<730 && start_y>380 && start_y<410)   //数量减
			return DEL;	
		else 
			return BACK;
	}
}


