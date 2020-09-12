#include"include/lib.h"
#include<stdio.h>
int main(int argc,char*argv[])//通过命令行参数进行两个主要参数的修改
{
    int min_per_sec = 1;
    int worktime = 9;
    if(argc > 1)
        sscanf(argv[1],"%d",&min_per_sec);//？程序内分钟时间=现实秒时间
    if(argc >2)
        sscanf(argv[2],"%d",&worktime);//？设置工作时长
    return clockin_machine_start(min_per_sec,worktime);
}