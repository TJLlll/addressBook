#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "addressBookUserInterface.h"

#define NOUNDERLINE 0   //字体正常显示
#define UNDERLINE 2     //字体虚化显示
#define ADDCONTACTS '1'   //新增联系人
#define SEARCHCONTACTS '2'   //查找联系人
#define DELECONTACTS '3'   //删除联系人
#define CHANGECONTACTS '4'   //修改联系人
#define EXITPROCEDURE '5'   //退出程序

int print_()    //打印一行-
{
    printf("\033[0;30;47m\t--------------------\n");
    return 0;
}

void clearBuffer()      //处理输入缓存区的垃圾字符，防止用户输入非法值
{
    char ch = 0;
    while (ch = getchar()!='\n');   
}


int printspace(int line)    //打印一行空格
{
    for (int idx = 0; idx < line; idx++)
    {
        printf("\033[0;30;47m\t|                  |\n");
    }
}

int PowerOnAnimation()   //开机动画
{
    system("clear");

    print_();
    printspace(2);
    printf("\033[0;30;47m\t|  欢迎使用通讯录  |\n");
    printspace(2);
    printf("\033[0;30;47m\t|   加载中 。      |\n");
    printspace(3);
    print_();
    printf("\033[0;0;0m\n");
    sleep(1.5);
    system("clear");

    print_();
    printspace(2);
    printf("\033[0;30;47m\t|  欢迎使用通讯录  |\n");
    printspace(2);
    printf("\033[0;30;47m\t|   加载中 。。    |\n");
    printspace(3);
    print_();
    printf("\033[0;0;0m\n");
    sleep(1.5);
    system("clear");

    print_();
    printspace(2);
    printf("\033[0;30;47m\t|  欢迎使用通讯录  |\n");
    printspace(2);
    printf("\033[0;30;47m\t|   加载中 。。。  |\n");
    printspace(3);
    print_();
    printf("\t\033[0;0;0m\n");
    sleep(1.5);

    return 0;
}

int numsIsEmpty(int nums)
{
    return nums == 0 ? UNDERLINE : NOUNDERLINE;
}

int funcManu()      //功能菜单
{
    system("clear");
    print_();
    printspace(1);
    printf("\033[0;30;47m\t|  请输入功能选项  |\n");
    printspace(1);
    printf("\033[0;30;47m\t|   1.新增联系人   |\n");

    int nums = 0;   //如果没有联系人，则无法查找、删除和修改，并且功能选项虚化显示
    printf("\t|\033[%d;30;47m   2.查找联系人   \033[0;30;47m|\n", numsIsEmpty(nums));
    printf("\t|\033[%d;30;47m   3.删除联系人   \033[0;30;47m|\n", numsIsEmpty(nums));
    printf("\t|\033[%d;30;47m   4.修改联系人   \033[0;30;47m|\n", numsIsEmpty(nums));
    printf("\033[0;30;47m\t|   5.退出通讯录   |\n");
    printspace(1);
    print_();
    printf("\t\033[0;0;0m\n");

    return 0;
}

int illegalInputDisplay()   //输入了非法值的显示
{
    system("clear");
    clearBuffer();
    print_();
    printspace(2);
    printf("\033[0;30;47m\t|    输入错误！    |\n");
    printspace(2);
    printf("\033[0;30;47m\t| 请输入正确的编号 |\n");
    printspace(3);
    print_();
    sleep(2);
}

int choiseFunc()    //选择功能
{
    int input = 0;
    char choise = 0;
    while (1)
    {
        scanf("%d", &input);
        if (input < 1 || input > 5)
        {
            illegalInputDisplay();
            printf("\t\033[0;0;0m\n");
            funcManu();
            continue;
        }
        else
        {
            choise = input + '0';
            break;
        }
    }
    switch (choise)
    {
        case ADDCONTACTS:
                printf("\n--1--\n");
            break;
        case SEARCHCONTACTS:
                printf("\n--2--\n");
            break;
        case DELECONTACTS:
                printf("\n--3--\n");
            break;
        case CHANGECONTACTS:
                printf("\n--4--\n");
            break;
        case EXITPROCEDURE:
            exit(0);  
    }
}