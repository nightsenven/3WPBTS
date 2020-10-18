#ifndef GETINFO_H_
#define GETINFO_H_

//获取bmp图片的宽和高
int get_bmp_wh(char *bmp_name,int *w,int *h);

//获取划屏动作
int get_screen_act(int *start_x,int *start_y,int *end_x,int *end_y);


#endif