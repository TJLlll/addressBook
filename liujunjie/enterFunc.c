#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "enterFunc.h"
#include "choiseFuncManu.h"
#include "newAddressBookUserInterface.h"
#include "addressBookUserInterface.h"
#include "dataStructure.h"
#include "keyMonitoring.h"

#define ADD 0
#define SEARCH 1
#define DEL 2
#define CHANGE 3
#define EXIT 4
#define SEARCHBUFFER 21
#define ESC 27
#define ENTER 10

/* 删除成功的画面 */
static void deleteSuccessful();

int addFunc(balanceBinarySearchTree* addressGetData)      //添加联系人功能
{
    system("clear");
    print_();
    printspace(1);
    printf("\033\t[0;30;47m|\033[3;30;47m请输入新联系人姓名\033[0;30;47m|\033\t[0;0;0m\n");
    printspace(2);
    printf("\t\033[0;30;47m|\033[2;30;47m ˉˉˉˉˉˉˉˉˉˉˉˉˉˉˉˉ \033[0;30;47m|\033[0;0;0m\n");
    printf("\033\t[0;30;47m|\033[3;30;47m请输入新联系人号码\033[0;30;47m|\033\t[0;0;0m\n");
    printspace(2);
    printf("\t\033[0;30;47m|\033[2;30;47m ˉˉˉˉˉˉˉˉˉˉˉˉˉˉˉˉ \033[0;30;47m|\033[0;0;0m\n");
    print_();
    printf("\033[5;13H\033[0;30;47m");
    contacts *newContacts = (contacts *)malloc(sizeof(contacts));
    if (!newContacts)
    {
        perror("malloc error");
    }
    scanf("%s", newContacts->name);
    printf("\033[9;13H\033[0;30;47m");
    scanf("%s", newContacts->phoneNumber);
    printf("\033[u\033[0;0;0m\n");
    balanceBinarySearchTreeInsert(addressGetData, newContacts);
    // getchar();      //清除缓冲区中的\n
}


AVLTreeNode* searchFunc(balanceBinarySearchTree* addressGetData)   //搜索结点
{
    system("clear");
    print_();
    printspace(1);
    printf("\033\t[0;30;47m|\033[3;30;47m  请输入联系人的  \033[0;30;47m|\033\t[0;0;0m\n");
    printf("\033\t[0;30;47m|\033[3;30;47m   姓名或电话：   \033[0;30;47m|\033\t[0;0;0m\n");
    printspace(2);
    printf("\t\033[0;30;47m|\033[2;30;47m ˉˉˉˉˉˉˉˉˉˉˉˉˉˉˉˉ \033[0;30;47m|\033[0;0;0m\n");
    printspace(3);
    print_();
    printf("\033[6;13H\033[0;30;47m");
    char searchInfo[SEARCHBUFFER] = {0};
    memset(searchInfo, 0, sizeof(char) * SEARCHBUFFER);
    scanf("%s", searchInfo);
    AVLTreeNode *temp = baseAppointValGetAVLTreeNode(addressGetData, searchInfo);
    if( temp != NULL)
    {
        printf("\033[u\033[0;0;0m\n");
        system("clear");    //清屏
        print_();
        printspace(1);
        printf("\033\t[0;30;47m|\033[3;30;47m为您找到联系人：  \033[0;30;47m|\033[0;0;0m\n");
        printspace(1);
        printf("\033\t[0;30;47m|\033[3;30;47m姓名：            \033[0;30;47m|\033[0;0;0m\n");
        printf("\t\033[0;30;47m|\033[2;30;47m     ˉˉˉˉˉˉˉˉˉˉˉˉˉ\033[0;30;47m|\033[0;0;0m\n");
        printf("\033\t[0;30;47m|\033[3;30;47m电话：            \033[0;30;47m|\033[0;0;0m\n");
        printf("\t\033[0;30;47m|\033[2;30;47m     ˉˉˉˉˉˉˉˉˉˉˉˉˉ\033[0;30;47m|\033[0;0;0m\n");
        printspace(2);
        print_();

        printf("\033[s");       //保存光标位置
        printf("\033[5;17H\033[0;30;47m");  //设置光标位置
        printf("\033[0;30;47m%s", temp->data->name);
        printf("\033[7;16H\033[0;30;47m");  //再次设置光标位置
        printf("\033[0;30;47m%s", temp->data->phoneNumber);
        
        printf("\033[u");   //恢复光标位置
        printf("\033[0;0;0m\n");
    }
    else
    {
        printf("\033[0;0;0m\n");
        system("clear");//清屏
        print_();
        printspace(4);
        printf("\033\t[0;30;47m|\033[3;30;47m  联系人不存在！  \033[0;30;47m|\033[0;0;0m\n");
        printspace(4);
        print_();
        printf("\033[0;0;0m\n");
        sleep(2);
        return NULL;
    }
    getchar();      //清除缓冲区中的\n
    printf("\033[u\033[0;0;0m\n");
    return temp;
}

/* 删除成功画面 */
static void deleteSuccessful()
{
    system("clear");//清屏
    print_();
    printspace(4);
    printf("\033[0;30;47m\t|  删除联系人成功  |\n");
    printspace(4);
    print_();
    printf("\033[0;0;0m\n");
    sleep(2);
}

int delFunc(balanceBinarySearchTree* addressGetData)      //删除功能
{
    AVLTreeNode *delNode = searchFunc(addressGetData);
    if (delNode)
    {
        printf("    (请按回车键确认删除该联系人)\n        (按ESC返回上一级)\n");
    }
    while (1)
    {
        int key = 0;
        key = returnKey();
        if (key == ESC)
        {
            break;
        }
        else if (key == ENTER)
        {
            balanceBinarySearchTreeDeleteNode(addressGetData, delNode);
            deleteSuccessful();
            break;
        }
    }

    return ON_SUCCESS;
}
int changeFunc(balanceBinarySearchTree* addressGetData)   //修改联系人信息功能
{
    AVLTreeNode *changeNode = searchFunc(addressGetData);
    if (changeNode)
    {
        printf("    (请按回车键确认修改该联系人)\n        (按ESC返回上一级)\n");
        while (1)
        {
            int key = 0;
            key = returnKey();
            if (key == ESC)
            {
                break;
            }
            else if (key == ENTER)
            {
                addFunc(addressGetData);
                balanceBinarySearchTreeDeleteNode(addressGetData, changeNode);
                newSuccessfullyChanged();
                getchar();
                break;
            }
        }
    }
    
    return ON_SUCCESS;
}


int enterFunc(int flag, balanceBinarySearchTree* addressGetData)
{
    switch (flag)
    {
        case ADD :
            addFunc(addressGetData);
            newSuccessfullyAdded();
            fflush(stdin);
            break;
        case SEARCH :   //查看全部联系人
            if (addressGetData->size == 0)
            {
                IsEmptyAddressBook();
                break;
            }
            else
            {
                displayAllContactInfo(addressGetData);
                break;
            }    
        case DEL :      //查找并删除
            if (addressGetData->size == 0)
                {
                    IsEmptyAddressBook();
                    break;
                }
            else
            {
                delFunc(addressGetData);
                break;
            }    
        case CHANGE :   //查找并修改
            if (addressGetData->size == 0)
                {
                    IsEmptyAddressBook();
                    break;
                }
            else
            {
                changeFunc(addressGetData);
                break;
            }
        case EXIT :
            //balanceBinarySearchTreeDestroy(addressGetData);
            byeBye();   
    }
}
