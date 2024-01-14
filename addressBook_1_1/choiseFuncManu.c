/* 根据键盘输入，显示不同的画面 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "choiseFuncManu.h"

#define ADD 0
#define SEARCH 1
#define DEL 2
#define CHANGE 3
#define EXIT 4
#define ESC 27
#define ENTER 10
#define EMPHASIZE 7     //选项变色
#define STD 0

/* 判断是否是本行变色显示 */
static int IsTheRow(int thisrow, int choiserow);
/* 显示菜单画面 */
static int funcManuPrint(int flag);

void byeBye()       //关机画面
{
    system("clear");
    print_();                                           //line 1

    printspace(2);
    
    printf("\t");                                       //line 4
    printf("\033[0;30;47m|");                           //line 4
    printf("\033[3;30;47m   谢 谢 使 用 ！ ");                //line 4
    printf("\033[0;30;47m|");                           //line 4

    printf("\033[0;0;0m\n");                            //换行

    printspace(2);                                      //line 4
    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[3;30;47m    再   见  !    ");
    printf("\033[0;30;47m|");

    printf("\033[0;0;0m\n");                            //换行

    printspace(3);
    print_();

    sleep(1);
    system("clear");

    exit(0);
}

/* 判断此行的显示格式 */
static int IsTheRow(int thisrow, int choiserow)
{
    return (thisrow == choiserow) ?  EMPHASIZE : STD; 
}

static int funcManuPrint(int flag)
{
    system("clear");
    print_();
    printspace(1);
    printf("\033[0;30;47m\t|  请输入功能选项  |\n");
    printspace(1);
    /* |边框和文字分开配置，不一起变色 */ 
    printf("\t\033[0;30;47m|"); 
    printf("\033[%d;30;47m   1.新增联系人   \033[0;30;47m|", IsTheRow(0,flag));
    printf("\033[0;0;0m\n");                            //换行
    printf("\t\033[0;30;47m|\033[%d;30;47m   2.查看联系人   \033[0;30;47m|", IsTheRow(1,flag));
    printf("\033[0;0;0m\n");                            //换行
    printf("\t\033[0;30;47m|\033[%d;30;47m   3.删除联系人   \033[0;30;47m|", IsTheRow(2,flag));
    printf("\033[0;0;0m\n");                            //换行
    printf("\t\033[0;30;47m|\033[%d;30;47m   4.修改联系人   \033[0;30;47m|", IsTheRow(3,flag));
    printf("\033[0;0;0m\n");                            //换行
    printf("\t\033[0;30;47m|\033[%d;30;47m   5.退出通讯录   \033[0;30;47m|", IsTheRow(4,flag));
    printf("\033[0;0;0m\n");                            //换行
    printspace(1);
    print_();
    printf("\t\033[0;0;0m\n");//重置文本属性为默认属性
    printf("   (请按s/w键更换选项,按Entre键进入)\n     (或直接输入选项编号进入选项)\n");

    return 0;
}

static void linkNumsToFlag(int *flag, int getkey)     //将数字与功能选项对应起来
{
    *flag = getkey - '0' - 1;
}

int switchFuncManu(balanceBinarySearchTree *contactsData)   //监测键盘输入，菜单相应选项变色显示
{
    int flag = 0;   //显示哪个画面
    int getkey = 0;
    
    while (1)
    {
        funcManuPrint(flag);
        getkey = returnKey();
        if (getkey == 's')
        {
            if (flag == 4)
            {
                flag = -1;
            }
            flag++;
        }
        if (getkey == 'w')
        {
            if (flag == 0)
            {
                flag = 5;
            }
            flag--;
        }
        
        if (getkey == ENTER || getkey == '1' ||  getkey == '2' || getkey =='3' || getkey == '4' || getkey == '5' || getkey == ESC)
        {
            if (getkey == ENTER)
            {
                enterFunc(flag, contactsData); 
            }
            else if (getkey == ESC)
            {
                flag = EXIT;
            }
            else
            {
                linkNumsToFlag(&flag, getkey);
                enterFunc(flag, contactsData); 
            }
        }
    }
}

