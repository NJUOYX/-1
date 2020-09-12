#include "data.h"
#include "timer.h"
#include <stdio.h>
Emp_table emp_data_libs[EMP_NUM]; //id对应的成员工作记录
int work_hours;                   //必须工作时长

int check_id(int id)
{ //id必须在范围内
    if (id >= EMP_ID_MIN && id <= EMP_ID_MAX)
    {
        emp_data_libs[GET_IDX(id)].id_v = 1;
        return 0;
    }
    return -1;
}

int in_work(int id, int ntime)
{ //检查是否上班打卡过
    return emp_data_libs[GET_IDX(id)].recs[GET_DAY(ntime)].on_v;
}

int on_duty(int minutes, int emp_id) 
{
    int late = 0;
    int over_time = 0;
    late = GET_TODAY_MIN(minutes) - WORK_START_MIN;
    emp_data_libs[GET_IDX(emp_id)].recs[GET_DAY(minutes)].on_duty = GET_TODAY_MIN(minutes);
    emp_data_libs[GET_IDX(emp_id)].recs[GET_DAY(minutes)].on_v = 1;
    if (GET_DAY(minutes) > 0) //如果在周一之后工作
    {
        over_time = get_work_long(GET_IDX(emp_id), GET_DAY(minutes) - 1); //检查前一天是否超时工作
    }
    if (late < 0) //今天未迟到
        return 0;
    if (GET_HOUR(over_time)> OVER_TIME && GET_HOUR(late) < AVAILABLE_TIME) //迟到但是前一天超时工作且今天迟到时间在容忍范围内
        return 0;
    else
        return late;
}

int off_duty(int minutes, int emp_id) 
{
    int early = 0;
    emp_data_libs[GET_IDX(emp_id)].recs[GET_DAY(minutes)].off_duty = GET_TODAY_MIN(minutes);
    emp_data_libs[GET_IDX(emp_id)].recs[GET_DAY(minutes)].off_v = 1;
    early = work_hours*MIN_NUM - get_work_long(GET_IDX(emp_id),GET_DAY(minutes));
    if (early < 0)
        return 0;
    else
        return early;
}

int data_lib_init(int wh)
{
    int i = 0;
    int j = 0;
    work_hours = wh;
    for (i = 0; i < EMP_NUM; ++i)
    {
        for (j = 0; j < WORK_DAYS; ++j)
            emp_data_libs[i].id_v = emp_data_libs[i].recs[j].on_v = emp_data_libs[i].recs[j].off_v = 0;
    }
    return 0;
}

int get_work_long(int idx, int day) //获取工作时长
{
    if (emp_data_libs[idx].recs[day].on_v == 1 && emp_data_libs[idx].recs[day].off_v == 1)
    {
        return emp_data_libs[idx].recs[day].off_duty - emp_data_libs[idx].recs[day].on_duty;
    }
    return 0;
}

//周平均上班时长，周迟到，早退，缺卡次数
int get_avarange(int idx)
{
    int i = 0;
    uint16_t total = 0;
    for (i = 0; i < DAY_NUM; ++i)
    {
        total += get_work_long(idx, i);
    }
    return total / DAY_NUM;
}

int get_late(int idx)
{
    int i = 0;
    int res = 0;
    for (i = 0; i < DAY_NUM; ++i)
    {
        if (emp_data_libs[idx].recs[i].on_v == 0 || emp_data_libs[idx].recs[i].on_duty > WORK_START_MIN)
            res++;
    }
    return res;
}

int get_early(int idx)
{
    int i = 0;
    int res = 0;
    for (i = 0; i < DAY_NUM; ++i)
    {
        if (get_work_long(idx, i) < work_hours*MIN_NUM)
            res++;
    }
    return res;
}

int get_lack(int idx)
{
    int i = 0;
    int res = 0;
    for (i = 0; i < DAY_NUM; ++i)
    {
        if (emp_data_libs[idx].recs[i].off_v == 0)
            res++;
    }
    return res;
}

void write_report()
{
    int i = 0;
    int avar_min = 0;
    int late = 0;
    int early = 0;
    int lack = 0;
    FILE *f = NULL;
    f = fopen("report.txt", "w");
    for (i = 0; i < EMP_NUM; ++i)
    {
        if (emp_data_libs[i].id_v)
        {
            fprintf(f, "id:%d-----------------\n", i + EMP_ID_MIN);
            avar_min = get_avarange(i);
            fprintf(f, "avarange: %d hours,%d minutes\n", GET_HOUR(avar_min), GET_MIN(avar_min));
            late = get_late(i);
            fprintf(f, "late: %d times\n", late);
            early = get_early(i);
            fprintf(f, "early: %d times\n", early);
            lack = get_lack(i);
            fprintf(f, "lack of duty: %d times\n", lack);
        }
    }
    fclose(f);
}