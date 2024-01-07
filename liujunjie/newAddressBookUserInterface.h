#ifndef __ADDRESSBOOKUSERINTERFACE_H_
#define __ADDRESSBOOKUSERINTERFACE_H_

int print_();    //打印一行-
void clearBuffer();      //处理输入缓存区的垃圾字符，防止用户输入非法值
int printspace(int line);    //打印一行空格
int PowerOnAnimation();   //开机动画
int numsIsEmpty(int nums);
//int funcManu();      //功能菜单
int choiseFunc();   //选择功能
void newSuccessfullyAdded();    //新增联系人成功界面
/* 修改联系人成功界面 */
void newSuccessfullyChanged();
/* 联系人为空提示界面 */
void IsEmptyAddressBook();

#endif  //__ADDRESSBOOKUSERINTERFACE_H_