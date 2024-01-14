#ifndef __CHOISEFUNCMANU_H_
#define __CHOISEFUNCMANU_H_

#include "keyMonitoring.h"
#include "newAddressBookUserInterface.h"
#include "enterFunc.h"
#include "dataStructure.h"

int switchFuncManu();   //选择播放哪个画面
void addFrame();        //新增联系人的选项画面
void searchFrame();     //搜索联系人的选项画面
void delFrame();        //删除联系人的选项画面
void changeFrame();      //修改联系人的选项画面
void byeBye();          //关机画面
int switchFuncManu(balanceBinarySearchTree *contactsData);   //监测键盘输入，菜单相应选项变色显示

#endif  //__CHOISEFUNCMANU_H_