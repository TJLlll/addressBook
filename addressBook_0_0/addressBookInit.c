/* 初始化通讯录 二叉平衡搜索树的数据结构 */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "addressBookInit.h"
#include "dataStructure.h"

#define RW_R_R 0644     //本用户可读写，组用户和其他用户可读

// readRecord()     //读取上次保存的数据
// {
//     int fd = open("./contactData.txt", O_RDONLY | O_CREAT, RW_R_R);
//     if (fd == -1)
//     {
//         perror("open error\n");
//     }


//     close(fd);
//     return 0;
// }

balanceBinarySearchTree * addressBookInit()     //初始化通讯录
{
    //readRecord();     //读取上次保存的数据
    balanceBinarySearchTree *dataTree = NULL;
    balanceBinarySearchTreeInit(&dataTree, compareFunc);
    return dataTree;
}

