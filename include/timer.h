#ifndef __TIMER_H__
#define __TIMER_H__
#include"pthread.h"

#define TIME_STR_BUFFER_SIZE 20
#define DAY_NUM 5
#define HOUR_NUM 24
#define MIN_NUM 60
#define START_HOUR 7
#define START_MIN START_HOUR*MIN_NUM
#define TOTAL_MINUTES DAY_NUM*HOUR_NUM*MIN_NUM
#define WORK_START_HOUR 9   
#define WORK_START_MIN WORK_START_HOUR*MIN_NUM
//分钟单位转换
#define GET_DAY(min) min/HOUR_NUM/MIN_NUM
#define GET_HOUR(min) (min/MIN_NUM)%HOUR_NUM
#define GET_MIN(min) (min)%MIN_NUM

#define GET_TODAY_MIN(ori_min) ori_min%(HOUR_NUM*MIN_NUM)

extern int timer;
extern char str_time[TIME_STR_BUFFER_SIZE];
extern int mins_per_sec;

int init_timer(int seconds_per_hour);//计时器初始化
int start_timer();//启动计时器
int check_timer();//检查当前时间
int stop_timer();//停止当前时间
char*time_show();//输出当前时间（格式化输出）



#endif//__TIMER_H__