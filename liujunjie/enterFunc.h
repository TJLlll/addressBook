#ifndef __ENTERFUNC_H_
#define __ENTERFUNC_H_

#include "dataStructure.h"

int addFunc(balanceBinarySearchTree* addressGetData);      //添加联系人功能
AVLTreeNode* searchFunc(balanceBinarySearchTree* addressGetData);   //搜索联系人所在结点功能
int delFunc(balanceBinarySearchTree* addressGetData);      //删除联系人功能
int changeFunc(balanceBinarySearchTree* addressGetData);   //修改联系人信息功能
int enterFunc(int flag, balanceBinarySearchTree* addressGetData);    //选择进入哪一个功能

#endif  //__ENTERFUNC_H_