#ifndef AUTO_STORE_H
#define AUTO_STORE_H

#include "cmsis_os.h"
#include "store.h"
#include "cmd_task.h"

/* 键盘触发变量（keyboard.c 写入，Auto_store.c 读取） */
extern volatile uint8_t auto_store_kb_trigger;
extern volatile Auto_ctrl_mode auto_store_kb_target;

/* 当前自动抓取/放置模式（Auto_store.c 定义） */
extern volatile Auto_ctrl_mode auto_ctrl_mode;

void auto_store_init(void);
void auto_store_trigger(Auto_ctrl_mode target);
uint8_t auto_store_update(void);
void auto_store_complete(void);

#endif // AUTO_STORE_H
