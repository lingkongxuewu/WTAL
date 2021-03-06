/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Time.h
* 版　本：  v1.0
* 描  述：  100T双轴 整合后的版本
* 创建人：  
* 日  期：  
--------------------------------------------------------------------------
*当前版本:	v1.0
*修 改 人:
*日　  期:
----------------------------------修改履历--------------------------------
1、
2、
3、
4、
------------------------------------------------------------------------*/
#include	"_Type.h"
//***********************************************************************/
#ifndef __TIMER_H__

	#define __TIMER_H__
	/*-----宏定义---------------------*/
	/*-----结构体数据类型定义---------*/
	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
	void Timer2_Init (void);
	void Timer2_TaskInIsr (void);
	void Timer2_TaskInMain (void);
	
#endif
/*-----------------------------------*/
#ifdef   __TIMER_C__
#define  TIMER_EXT
#else
#define	 TIMER_EXT  extern	   
#endif
/*-------------变量声明--------------*/

TIMER_EXT   xdata int16u MainTime4Cnt;
TIMER_EXT   xdata int16u alarmcnt;

//***********************************************************************/
