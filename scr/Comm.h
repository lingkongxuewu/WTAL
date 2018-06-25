/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Comm.h
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
/***********************************************************************/
#ifndef __COMM_H__

	#define __COMM_H__
	/*-----宏定义---------------------*/	
	#define  COMM_RX_BUF_SIZE     200
	#define  COMM_TX_BUF_SIZE     200
	#define  UART_T();			  P1|=0x01;  	//P1.0
	#define  UART_R();			  P1&=0xfe;  	//

	#define BAUDRATE        	9600           	// Baud rate of UART in bps
	#define COMM_NO_ERR			0
	#define COMM_RX_EPT			1
	#define COMM_TX_FULL		2
	#define COMM_TX_EPT			3
	#define	COMM_STOP			1
	#define COMM_TRAN			0
	#define	ISR_EN()		    EA=1
	#define	ISR_DIS()		    EA=0
		
	#define	COMM_485							//485通信宏定义
	/*-----结构体数据类型定义---------*/
typedef struct {
				    int16u  RingBufRxCtr;
				    int8u   RxHead;
				    int8u   RxTail;
				    int8u   RingBufRx[COMM_RX_BUF_SIZE];
				    int16u  RingBufTxCtr;
				    int8u   TxHead;
				    int8u   TxTail;
				    int8u   RingBufTx[COMM_TX_BUF_SIZE];
				} COMM_RING_BUF;
	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
	void  Comm_HardwareInit (void);
	void  Comm_SoftwareInit (void);
	void  Comm_SendSta (void);
	void  Comm_SendStop (void);
	int8u Comm_GetChar (int8u *err);
	int8u Comm_PutChar (int8u c);
	int8u Comm_GetTxChar (int8u *err);
	void  Comm_PutRxChar (int8u c);
	void  Comm_TimerProc (void);
	int8u Comm_StateGet (void);
	void  Comm_PutStr (int8u* ptr,int8u n);
	void  Sub_SendVer(void);
	void Comm_ClrRxBuf (void);
#endif
/*-----------------------------------*/
#ifdef   __COMM_C__
#define  COMM_EXT
#else
#define	 COMM_EXT  extern
#endif
/*-------------变量声明--------------*/
COMM_EXT COMM_RING_BUF	xdata	CommBuf;
COMM_EXT bit CommRxFlag;		//通信接收标志位
COMM_EXT bit CommTxFlag;		//通信发送标志位

