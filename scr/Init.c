/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Init.c
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
#define   __INIT_C__
#include  "_Include.h"
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Init_SystemPara (void)
{
	SendActPosi=1;
	SendSpeedNum=1;		        			//初始化自动发送频率为每秒钟一次
    MainTime4Cnt = SendSpeed[SendSpeedNum];	

//	TempA=2500;
	Braud_Num=3;
	Addr=1;
}
//***********************************************************************/
static void Sys_ClkInit (void)
{	int8u SfrPage_ = SFRPAGE;
	
	SFRPAGE = CONFIG_PAGE;
//	OSCICN = 0xD6;		//12M
	OSCICN = 0xD5;		//6M
//	RSTSRC = 0x04;						  


	CLKSEL = 0X00;				//	选择内部振荡器作为系统时钟
	while((OSCICN & BIT4) == 0);//	等待时钟稳定
	VDM0CN = 0xA0;				//	Enable VDD monitor and high threshold
	RSTSRC = 0x02;				//	Enable VDD monitor as a reset source

	SFRPAGE = SfrPage_;
}
//***********************************************************************/
void Sys_PortInit (void)
{	
	int8u SfrPage_ = SFRPAGE;
	SFRPAGE  = CONFIG_PAGE;  
	P0SKIP |= 0x01;		// Skip P0.0 (VREF)
	P1SKIP |= 0x0E;		// Skip P1.1 P1.2 P1.3(ADC input)		  
	P0MDOUT  |= 0x50;	
	P0MDOUT  |= 0x80;				 
	P0MDIN  &= ~0x01;	// Set VREF to analog
//	P1MDOUT  |= 0x08;		
	P1MDIN  &= ~0x0E;	// Set P1.1 P1.2 P1.3 as an analog input
	P1MDOUT |= 0x30;	  
	P2MDOUT  |= 0xED;	// Set Push-Pull
	XBR0	  = 0x01;	// Uart	Enable				
	XBR2	  = 0x40;							  
	SFRPAGE = SfrPage_;
}
//***********************************************************************/
void Init_All (void)
{
	Sys_PortInit();		// Initialize Port I/O
	Sys_ClkInit();		// Initialize Oscillator
	
	Comm_HardwareInit();
	Comm_SoftwareInit();
	
	Timer2_Init();
	Init_SystemPara();
	AngInit();				//HXL 12/8/28
	Adc_Init();				//HXL 12/8/28
	Adc_Enable();

}

//***********************************************************************/
