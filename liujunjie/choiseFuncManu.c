/* 根据键盘输入，显示不同的页面 */
#include <stdio.h>
#include <stdlib.h>
#include "keyMonitoring.h"
#include "newAddressBookUserInterface.h"

#include <unistd.h>

#define ADD 0
#define SEARCH 1
#define DEL 2
#define CHANGE 3
#define EXIT 4
#define ESC 27
#define ENTER 10

void addFrame()
{
    system("clear");
    print_();                                           //line 1

    printf("\t\033[0;30;47m|");                         //line 2
    printf("\033[3;30;47m 请按s/w键更换选项");           //line 2
    printf("\033[0;30;47m|");                           //line 2

    printf("\033[0;0;0m\n");                            //换行
    
    printf("\t");                                       //line 3
    printf("\033[0;30;47m|");                           //line 3
    printf("\033[3;30;47m 按Entre键进入选项");           //line 3
    printf("\033[0;30;47m|");                           //line 3

    printf("\033[0;0;0m\n");                            //换行

    printspace(1);                                      //line 4

    printf("\t");                                       //line 5
    printf("\033[0;30;47m|  ");                         //line 5
    printf("\033[7;30;47m 1.新增联系人 ");               //line 5
    printf("\033[0;30;47m  |");                         //line 5

    printf("\033[0;0;0m\n");                            //换行

    int nums = 0;   //如果没有联系人，则无法查找、删除和修改，并且功能选项虚化显示

    printf("\t");
    printf("\033[0;30;47m|");                           //line 6

    printf("\033[%d;30;47m   2.查找联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");
    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[%d;30;47m   3.删除联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");

    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[%d;30;47m   4.修改联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");
    printf("\033[0;30;47m\t|   5.退出通讯录   |");
    printf("\t\033[0;0;0m\n");
    printspace(1);
    print_();
    printf("\t\033[0;0;0m\n");

}

void searchFrame()
{
    system("clear");
    print_();                                           //line 1

    printf("\t\033[0;30;47m|");                         //line 2
    printf("\033[3;30;47m 请按s/w键更换选项");           //line 2
    printf("\033[0;30;47m|");                           //line 2

    printf("\033[0;0;0m\n");                            //换行
    
    printf("\t");                                       //line 3
    printf("\033[0;30;47m|");                           //line 3
    printf("\033[3;30;47m 按Entre键进入选项");           //line 3
    printf("\033[0;30;47m|");                           //line 3

    printf("\033[0;0;0m\n");                            //换行

    printspace(1);                                      //line 4



    int nums = 0;   //如果没有联系人，则无法查找、删除和修改，并且功能选项虚化显示

    printf("\t");
    printf("\033[0;30;47m|");                           //line 5


    printf("\033[0;30;47m   1.新增联系人   \033[0;30;47m|");  //line 5
    printf("\033[0;0;0m\n");                            //换行

    printf("\t");                                       //line 6
    printf("\033[0;30;47m|  ");                         //line 6
    printf("\033[7;30;47m 2.查找联系人 ");  //line 6
    printf("\033[0;30;47m  |");                         //line 6

    printf("\033[0;0;0m\n");                            //换行

    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[%d;30;47m   3.删除联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");

    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[%d;30;47m   4.修改联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");
    printf("\033[0;30;47m\t|   5.退出通讯录   |");
    printf("\t\033[0;0;0m\n");
    printspace(1);
    print_();
    printf("\t\033[0;0;0m\n");

}

void delFrame()
{
    system("clear");
    print_();                                           //line 1

    printf("\t\033[0;30;47m|");                         //line 2
    printf("\033[3;30;47m 请按s/w键更换选项");           //line 2
    printf("\033[0;30;47m|");                           //line 2

    printf("\033[0;0;0m\n");                            //换行
    
    printf("\t");                                       //line 3
    printf("\033[0;30;47m|");                           //line 3
    printf("\033[3;30;47m 按Entre键进入选项");           //line 3
    printf("\033[0;30;47m|");                           //line 3

    printf("\033[0;0;0m\n");                            //换行

    printspace(1);                                      //line 4

    int nums = 0;   //如果没有联系人，则无法查找、删除和修改，并且功能选项虚化显示

    printf("\t");
    printf("\033[0;30;47m|");                           //line 5


    printf("\033[0;30;47m   1.新增联系人   \033[0;30;47m|");  //line 5
    printf("\033[0;0;0m\n");                            //换行

    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[%d;30;47m   2.查找联系人   \033[0;30;47m|", numsIsEmpty(nums));    //line 6
    printf("\t\033[0;0;0m\n");

    printf("\t");                                       //line 7
    printf("\033[0;30;47m|  ");                         //line 7
    printf("\033[7;30;47m 3.删除联系人 ");               //line 7
    printf("\033[0;30;47m  |");                         //line 7

    printf("\033[0;0;0m\n");                            //换行

    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[%d;30;47m   4.修改联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");
    printf("\033[0;30;47m\t|   5.退出通讯录   |");
    printf("\t\033[0;0;0m\n");
    printspace(1);
    print_();
    printf("\t\033[0;0;0m\n");
}

void changeFrame()
{
    system("clear");
    print_();                                           //line 1

    printf("\t\033[0;30;47m|");                         //line 2
    printf("\033[3;30;47m 请按s/w键更换选项");           //line 2
    printf("\033[0;30;47m|");                           //line 2

    printf("\033[0;0;0m\n");                            //换行
    
    printf("\t");                                       //line 3
    printf("\033[0;30;47m|");                           //line 3
    printf("\033[3;30;47m 按Entre键进入选项");           //line 3
    printf("\033[0;30;47m|");                           //line 3

    printf("\033[0;0;0m\n");                            //换行

    printspace(1);                                      //line 4



    int nums = 0;   //如果没有联系人，则无法查找、删除和修改，并且功能选项虚化显示

    printf("\t");
    printf("\033[0;30;47m|");                           //line 5


    printf("\033[0;30;47m   1.新增联系人   \033[0;30;47m|");  //line 5
    printf("\033[0;0;0m\n");                            //换行

    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[%d;30;47m   2.查找联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");

    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[%d;30;47m   3.删除联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");

    printf("\t");                                       //line 7
    printf("\033[0;30;47m|  ");                         //line 7
    printf("\033[7;30;47m 4.修改联系人 ");               //line 7
    printf("\033[0;30;47m  |");                         //line 7

    printf("\033[0;0;0m\n");                            //换行

    printf("\033[0;30;47m\t|   5.退出通讯录   |");
    printf("\t\033[0;0;0m\n");
    printspace(1);
    print_();
    printf("\t\033[0;0;0m\n");
}

void exitFrame()
{
    system("clear");
    print_();                                           //line 1

    printf("\t\033[0;30;47m|");                         //line 2
    printf("\033[3;30;47m 请按s/w键更换选项");           //line 2
    printf("\033[0;30;47m|");                           //line 2

    printf("\033[0;0;0m\n");                            //换行
    
    printf("\t");                                       //line 3
    printf("\033[0;30;47m|");                           //line 3
    printf("\033[3;30;47m 按Entre键进入选项");           //line 3
    printf("\033[0;30;47m|");                           //line 3

    printf("\033[0;0;0m\n");                            //换行

    printspace(1);                                      //line 4

    printf("\033[0;30;47m\t|   1.新增联系人   |");

    printf("\033[0;0;0m\n");                            //换行

    int nums = 0;   //如果没有联系人，则无法查找、删除和修改，并且功能选项虚化显示

    printf("\t");
    printf("\033[0;30;47m|");                           //line 6

    printf("\033[%d;30;47m   2.查找联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");
    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[%d;30;47m   3.删除联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");

    printf("\t");
    printf("\033[0;30;47m|");
    printf("\033[%d;30;47m   4.修改联系人   \033[0;30;47m|", numsIsEmpty(nums));
    printf("\t\033[0;0;0m\n");

    printf("\t");                                       //line 5
    printf("\033[0;30;47m|  ");                         //line 5
    printf("\033[7;30;47m 5.退出通讯录 ");               //line 5
    printf("\033[0;30;47m  |");                         //line 5

    printf("\t\033[0;0;0m\n");
    printspace(1);
    print_();
    printf("\t\033[0;0;0m\n");

}

void byebye()
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

    sleep(2);
    system("clear");
    exit(0);
}

int switchFuncManu()
{
    int flag = 0;   //显示哪个画面
    int getkey = 0;
    while (1)
    {
        switch (flag)
        {
            case ADD:
                addFrame();
                break;
            case SEARCH:
                searchFrame();
                break;
            case DEL:
                delFrame();
                break;
            case CHANGE:
                changeFrame();
                break;
            case EXIT:
                exitFrame();
                break;
        }
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
        
        if (getkey == ENTER)
        {
            //enterFunc(flag); 
            byebye();
        }
    }
}

