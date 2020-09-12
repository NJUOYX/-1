#include "common.h"
#include "timer.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "data.h"
#include"lib.h"
uint8_t buffer[STR_BUFFER_SIZE];//输入命令的缓存

int get_check_code(int code);//根据id计算验证码

int clockin_machine_start(int s2m, int workhours)//主逻辑函数
{
    int emp_id = 0;
    int check_code = 0;
    int res = 0;
    if (data_lib_init(workhours) != 0)//先初始化数据记录结构
    {
        printf("data lib init failed\n");
        return -1;
    }
    init_timer(s2m);//初始化时钟
    start_timer();//时钟开始运行
    while (check_timer() < TOTAL_MINUTES) //命令模式循环，持5天的上班时间
    {
        emp_id = 0;
        check_code = 0;
        res = 0;
        printf("sys$:");//提示输入命令
        scanf("%s", buffer);
        if (strcmp((const char*)buffer, WORK_COMMAND) == 0)//匹配打卡命令
        {
            do
            {
                printf("your id:");
                scanf("%d", &emp_id);
                res = check_id(emp_id);
                if (res != 0)
                    printf("id wrong\n");
            } while (res != 0);//输入id，优先检查id是否正确
            res = in_work(emp_id,check_timer());//检查此时是否为上班打卡
            if (res == 0)//上班打卡，需要输入验证码检查
            {
                printf("请输入验证码：%d\nyour input:", get_check_code(emp_id));
                scanf("%d", &check_code);
                if(get_check_code(emp_id) != check_code)
                {
                    printf("check code wrong\n");
                    continue;
                }
                res = on_duty(check_timer(), emp_id);//进行上班打卡
                if(res == 0)
                    printf("check success\n");
                else
                    printf("you are late for %d minutes\n",res);
            }
            else//验证为下班打卡
            {
                res = off_duty(check_timer(),emp_id);//下班打卡
                if(res == 0)
                    printf("check success\n");
                else
                {
                    printf("off duty early for %d minutes\n",res);
                }
            }
        }
        else if (strcmp((const char*)buffer, TIME_COMMAND) == 0)//显示当前时间的命令
        {
            printf("%s\n", time_show());
        }
        else
            continue;
    }
    write_report();//结束运行周期，进行周报输出
    stop_timer();//停止计时器
    return 0;
}

int get_check_code(int code)
{
    int res = code;
    int e = 10000;
    while(code!=1)
    {
        res+=(code%10)*e;
        code/=10;
        e/=10;
    }
    return res;
}