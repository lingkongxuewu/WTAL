/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Time.c
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
#define   __TIMER_C__
#include "_Include.h"
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Timer2_Init (void)
{	
	int8u SfrPage_ = SFRPAGE;
	SFRPAGE = ACTIVE_PAGE;	
	TMR2CN  = 0x00;							// Stop Timer2; Clear TF2;	
	CKCON  |= 0x10;							// Select SYSCLK for timer 2 source		0x30
	TMR2RL  = -(SYSCLK/1000);				// Init reload value for 100 us			65535-(SYSCLK/10000)  ???1MS mickxie 20111031
	TMR2	= 0xFFFF;						// Set to reload immediately
	ET2	  	= 1;							// Enable Timer2 interrupts
	TR2	  	= 1;							// Start Timer2
	SFRPAGE = SfrPage_;
	MainTime4Cnt=SendSpeed[SendSpeedNum];


}
/***********************************************************************/
// *功能：
// *入口：主动发送时间周期，轮询COMM的时间周期
// *出口：
// *备注：
// *函数性质：
/***********************************************************************/
void Timer2_ISR (void) interrupt 5
{
	TF2H = 0;								// Clear Timer2 interrupt alarming
	Timer2_TaskInIsr();
}

/***********************************************************************/

#define	MAIN_TIME1_INIT	(1000/100)
#define	MAIN_TIME2_INIT	(1000/8)
#define	MAIN_TIME3_INIT	(1000/2)
#define	MAIN_TIME4_INIT	(1000/1)

static   int8u MainTime4En  = OFF;


/***********************************************************************/
// *功能：定时器2 控制主动发送模式的频率和时间周期
// *入口：
// *出口：
// *备注：
// *函数性质：
/***********************************************************************/
void Timer2_TaskInIsr (void)
{	
	static int8u cnt100ms;		

	Comm_TimerProc();						// CommTimer=10 轮询COMM的时间周期
 	MainTime4Cnt--;	   						//发送时间周期
	if(MainTime4Cnt==0)
	{
		MainTime4Cnt=SendSpeed[SendSpeedNum];  //发送时间周期
		MainTime4En=ON;						   //发送使能
		/****** 主动发送频率bps设置***********/
	}

	if(++cnt100ms==100)
	{
		cnt100ms=0;
	}
}
/***********************************************************************/
// *功能：主动发送模式 发送XY的角度
// *入口：MainTime4En SendActPosi
// *出口：
// *备注：
// *函数性质：
/***********************************************************************/
void Timer2_TaskInMain (void)
{	
	if(MainTime4En==ON)
	{	MainTime4En=OFF;
		if(SendActPosi==0)
		Sub_SendAngXY();				   //主动发送模式 发送XY的角度
		/******主动发送***********/
	}
}

