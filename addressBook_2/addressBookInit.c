/* 初始化通讯录 二叉平衡搜索树的数据结构 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "addressBookInit.h"
#include "dataStructure.h"

static void readRecord(balanceBinarySearchTree *pBstree, AVLTreeNode *root)     //读取上次保存的数据
{
    if (!root)
    {
        return;
    }

    int fd = open("./contactData.txt", O_RDONLY);
    if (fd == -1)
    {
        return;
    }

    while (1)
    {
        contacts *newContacts = (contacts *)malloc(sizeof(contacts));
        if (!newContacts)
        {
            perror("malloc error");
        }
        int reallen = read(fd, newContacts->name, NAME_SIZE);
        printf("name reallen:%d\n", reallen);getchar();

        if (reallen == 0 || newContacts->name == "\n" || newContacts->name == "\0")
        {
            printf("36\n");getchar();
            free(newContacts);
            newContacts = NULL;
            break;
        }

        reallen = read(fd, newContacts->phoneNumber, PHONENUMBER_SIZE);
        printf("phone reallen:%d\n", reallen);getchar();
        lseek(fd, 1, SEEK_CUR);
        if (reallen == 0 || newContacts->name == "\n" || newContacts->name == "\0")
        {
            free(newContacts);
            newContacts = NULL;
            break;
        }
        balanceBinarySearchTreeInsert(pBstree, newContacts);
    }
    close(fd);
}

balanceBinarySearchTree * addressBookInit()     //初始化通讯录
{
    balanceBinarySearchTree *dataTree = NULL;
    balanceBinarySearchTreeInit(&dataTree, compareFunc);
    readRecord(dataTree ,dataTree->root);     //读取上次保存的数据
    return dataTree;
}

