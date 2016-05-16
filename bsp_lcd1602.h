#ifndef __BSP_LCD1602_H__
#define __BSP_LCD1602_H__

/*----添加需要包含的头文件----*/
//////////////////////////////////////////////////////////////////
#include "hal_delay.h"
#include "CoOS.h"
//////////////////////////////////////////////////////////////////

void BSP_LCD1602_Clear(void);
void BSP_LCD1602_Home(void);

void BSP_LCD1602_SetCursor(u8 col,u8 row);	//设定光标位置

void BSP_LCD1602_NoDisplay(void);
void BSP_LCD1602_Display(void);

void BSP_LCD1602_NoCursor(void);		//不显示光标
void BSP_LCD1602_Cursor(void);			//显示光标

void BSP_LCD1602_NoBlink(void);		//光标不闪烁
void BSP_LCD1602_Blink(void);			//光标闪烁

void BSP_LCD1602_ScrollDisplayLeft(void);		//画面向左移动
void BSP_LCD1602_ScrollDisplayRight(void);	//画面向右移动

void BSP_LCD1602_LeftToRight(void);
void BSP_LCD1602_RightToLeft(void);

void BSP_LCD1602_AutoScroll(void);		//自动滚动8个格
void BSP_LCD1602_NoAutoScroll(void);

void BSP_LCD1602_CreateChar(u8 location,u8* charmap);

void BSP_LCD1602_SendStr(char *str);
void BSP_LCD1602_SendData(u8 value);

void BSP_LCD1602_Init(u8 cols,u8 lines);

#endif
