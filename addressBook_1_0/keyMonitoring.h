#ifndef __KEYMONITORING_H_
#define __KEYMONITORING_H_

int tty_reset(void);
int tty_set(void);
int kbhit(void);
 
int returnKey();      //每0.7秒返回一次按键输入

#endif  //__KEYMONITORING_H_