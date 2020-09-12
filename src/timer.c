#include"../include/timer.h"
#include"../include/common.h"
#include<unistd.h>
#include<string.h>
#include<stdio.h>

int timer;//计时器
char str_time[TIME_STR_BUFFER_SIZE];//输出缓存
int mins_per_sec;//等效时间设置

int flag;//终止计时变量
pthread_t p_timer;//计时器线程号

void* run()//启动计时器
{
    while(flag != 0)
    {
        timer+=mins_per_sec;
        sleep(1);//一秒钟增加一
    }
    return NULL;
}

int init_timer(int seconds_per_min)
{
    int i = 0;
    timer = START_MIN;
    for(i = 0;i<TIME_STR_BUFFER_SIZE;++i)
        str_time[i] = 0;
    mins_per_sec = seconds_per_min;
    flag = 1;
    return 0;
}
int start_timer()//创建计时器内计时线程
{
    return pthread_create(&p_timer,NULL,run,NULL);
}

int check_timer()
{
    return timer;
}

int stop_timer()//通过变量控制线程停止
{
    flag = 0;
    return pthread_join(p_timer,NULL);
}
char*time_show()
{
    char buffer[10];
    int day = 0;
    int hour = 0;
    int min = 0;
    day = GET_DAY(timer);
    hour = GET_HOUR(timer);
    min = GET_MIN(timer);
    memset(str_time,0,TIME_STR_BUFFER_SIZE);
    switch (day)
    {
    case 0:
        strcat(str_time,"Monday");
        break;
    case 1:
        strcat(str_time,"Tuesday");
        break;
    case 2:
        strcat(str_time,"Wednesday");
        break;
    case 3:
        strcat(str_time,"Thursday");
        break;
    case 4:
        strcat(str_time,"Friday");
        break;
    default:return "day out of range\n";
        break;
    }
    sprintf(buffer," %d:%d",hour,min);
    strcat(str_time,buffer);
    return str_time;
}