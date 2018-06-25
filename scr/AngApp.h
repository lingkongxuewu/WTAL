/*--------------------------------文*件*信*息-----------------------------
* 文件名：  AngApp.h
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
#include "_Type.h"
#ifndef __ANGAPP_H__
#define __ANGAPP_H__
	/*-----宏定义---------------------*/
	#define		Alarm_X_Enb			1	 //X轴	报警编译使能
	#define		Alarm_Y_Enb			1	 //Y轴	报警编译使能
	#define		Alarm_P_Enb			0	 //P轴	报警编译使能
	sbit		LED_GREEN 	=	P1^5;     //green 
	sbit		LED_RED 	=	P1^4;	 //red
	sbit		LED_S 		=	P2^5;
	
	#define		LED_GREEN_ON() 	LED_GREEN=1
	#define		LED_GREEN_OFF() LED_GREEN=0
	#define		LED_RED_ON() 	LED_RED=1
	#define		LED_RED_OFF()   LED_RED=0
	
	
//	#define		LED_S_ON()      LED_S=1
//	#define		LED_S_OFF()     LED_S=0
	
	
	#define		LED_S_ON()      LedS_On()		//LED_S=1
	
	
	#define		LED_S_OFF()     LedS_Off()		//LED_S=0
	
	
	/*-----结构体数据类型定义---------*/

typedef struct 
{
	int16u  alarmcnt;     //报警延时时间计时
	int8u   alarming;     // delay time 到
	int16u  excnt;	 //撤警延时时间计时
	int8u   exalarm;	 // delay time 到
} STARSTP_STRUCT;

	/*-----常量（表格等）声明---------*/
	
	/*-----函数声明-------------------*/
	void AngInit(void);
	
	int32s GetAngP(void);
	int32s GetAngXSysZero(void);
	int32s GetAngYSysZero(void);
	int32s GetAngXUsrZero(void);
	int32s GetAngYUsrZero(void);
	int32s GetAngXInst(void);
	int32s GetAngYInst(void);
	int32s GetAngXSet(void);
	int32s GetAngYSet(void);
	int32s GetAngPSet(void);
	void SetAngXSysZero(int32s zore);
	void SetAngYSysZero(int32s zore);
	void SetAngXUsrZero(int32s zore);
	void SetAngYUsrZero(int32s zore);
	void SetAngXInst(int32s ang_inst);
	void SetAngYInst(int32s ang_inst);
	void SetAngXSet(int32s ang_set);
	void SetAngYSet(int32s ang_set);
	void SetAngPSet(int32s ang_set);
	int32s GetXAngAllSetVal(void);

	int32s GetYAngAllSetVal(void);
	int32s GetAngP(void);

	int32s GetAngXZeroUsr(void);
	int32s GetAngYZeroUsr(void);
	int32s GetAngXRelSys(void);
	int32s GetAngYRelSys(void);
	void  Led_HandleProc(void);
	void LedFlashProc(void);
	void Led_GreenOn(void);
	void Led_RedOn(void);
	
	
	void LedS_On (void);
	void LedS_Off (void);
//**************************************************************************
#endif
/***********************************************************************/
#ifdef   __CAL_C__
#define  CAL_EXT
#else
#define	 CAL_EXT  extern
#endif
CAL_EXT xdata STARSTP_STRUCT DELAY_X;
CAL_EXT xdata STARSTP_STRUCT DELAY_Y;
//CAL_EXT xdata STARSTP_STRUCT DELAY_S;
CAL_EXT xdata int8u LedMode;
CAL_EXT xdata int8u LedFMode;
CAL_EXT xdata int16u EnDlyTime;        //unit:ms
CAL_EXT xdata int16u ExDlyTime;
CAL_EXT xdata int8u FlashCnt;