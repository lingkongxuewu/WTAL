/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Comm.c
* �桡����  v1.0
* ��  ����  100T˫�� ���Ϻ�İ汾
* �����ˣ�   
* ��  �ڣ�   
--------------------------------------------------------------------------
*��ǰ�汾:	v1.0
*�� �� ��:
*�ա�  ��:
----------------------------------�޸�����--------------------------------
1��
2��
3��
4��
------------------------------------------------------------------------*/
#define   __COMM_C__
#include  "_Include.h"
//***********************************************************************/
/*-----��������-------------------*/
static int16u  xdata	CommTimer;
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void Comm_Isr (void) interrupt INTERRUPT_UART0
{	int8u temp;
	int8u err;
	
	
	if(TI0==1)								// Check if transmit flag is set
	{	TI0=0;								// Clear interrupt flag
		temp=Comm_GetTxChar(&err);
		if(COMM_TX_EPT!=err)
		{	SBUF0 = temp;					// Transmit to Hyperterminal
//			Comm_TimerInit();
			CommTxFlag = 1;
		}
		else
		{	Comm_SendStop();
			CommTxFlag = 0;
		}
	}

	if(RI0==1)
	{	RI0=0;								//Clear interrupt flag
		temp=SBUF0;							// Read a character from UART	
		#ifdef	COMM_485
		if(CommTxFlag == 0)					
		#endif
		{
			Comm_PutRxChar(temp);
			CommTimer=10;			   //�жϽ������ݵ���
		}
	}
	if(OVR0 == 1)
	{	OVR0 = 0;
	}									//�����־���豣��
}
//***********************************************************************/
void Comm_HardwareInit (void)
{	int8u SfrPage_ = SFRPAGE;
	
	
	SFRPAGE = CONFIG_PAGE;
	SCON0 = 0x10;							  
	// Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

	#if	((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 1)
			SBRL0 = -(SYSCLK / BAUDRATE / 2);
			SBCON0 |= 0x03;						 // Set prescaler to 1*/
	#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 4)
			SBRL0 = -(SYSCLK / BAUDRATE / 2 / 4);
			SBCON0 &= ~0x03;
			SBCON0 |= 0x01;						 // Set prescaler to 4
	#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 12)
			SBRL0 = -(SYSCLK / BAUDRATE / 2 / 12);
			SBCON0 &= ~0x03;					  // Set prescaler to 12
	#else
			SBRL0 = -(SYSCLK / BAUDRATE / 2 / 48);
			SBCON0 &= ~0x03;
			SBCON0 |= 0x02;						 // Set prescaler to 48
	#endif
	
	IP |= 0x10;								    // Make UART high priority
	ES0 = 1;								    // Enable UART0 interrupts
		
	SBCON0 |= 0x40;							    // Enable baud rate generator
	TI0 = 1;									// Indicate TX0 ready
	SFRPAGE = SfrPage_;
}
//***********************************************************************/
void Comm_SoftwareInit (void)
{	
	CommBuf.RingBufRxCtr= 0;
	CommBuf.RxHead  = 0;
	CommBuf.RxTail  = 0;
	CommBuf.RingBufTxCtr= 0;
	CommBuf.TxHead  = 0;
	CommBuf.TxTail  = 0;	
}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
//void Comm_ClrRxBuf (void)
//{
//	CommBuf.RingBufRxCtr= 0;
//	CommBuf.RingBufRxInPtr  = &CommBuf.RingBufRx[0];
//	CommBuf.RingBufRxOutPtr = &CommBuf.RingBufRx[0];
//}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void Comm_SendSta (void)
{
    UART_T();	
	TI0=1;
}
//***********************************************************************/
void Comm_SendStop (void)
{
	UART_R();
}
//***********************************************************************/
//***********************************************************************/
// *��    �ܣ� ��ȡcom��������ҪTX������
// *��    �ڣ� RingBufTxCtr �洢����������RingBufTxOutPtr ������ݵ�ָ��
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u Comm_GetTxChar (int8u *err)
{	
	int8u c;
	
	ISR_DIS();
	if (CommBuf.RingBufTxCtr > 0)
	{	CommBuf.RingBufTxCtr--;
		c = CommBuf.RingBufTx[CommBuf.TxHead++];
		if (CommBuf.TxHead >= COMM_TX_BUF_SIZE)
		{	CommBuf.TxHead=0;
		}
		*err = COMM_NO_ERR;
		ISR_EN();
		return (c);
	}
	else
	{	*err = COMM_TX_EPT;
		ISR_EN();
		return (NUL);
	}
}
//***********************************************************************/
//***********************************************************************/
// *��    �ܣ� ��ҪTX����������com������
// *��    �ڣ� RingBufTxCtr �洢����������RingBufTxInPtr �������ݵ�ָ��
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u Comm_PutChar (int8u c)
{	
	ISR_DIS();
	if (CommBuf.RingBufTxCtr < COMM_TX_BUF_SIZE)
	{	
		CommBuf.RingBufTxCtr++;
		CommBuf.RingBufTx[CommBuf.TxTail++] = c;
		if (CommBuf.TxTail >= COMM_TX_BUF_SIZE)
		{	
			CommBuf.TxTail = 0;
		}
		ISR_EN();
		return (COMM_NO_ERR);
	}
	else
	{	ISR_EN();
		return (COMM_TX_FULL);
	}
}



//***********************************************************************/
//***********************************************************************/
// *��    �ܣ� �����յ���������com������
// *��    �ڣ� RingBufRxCtr �洢����������RingBufRxInPtr �������ݵ�ָ��
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/

void Comm_PutRxChar (int8u c)
{	
	ISR_DIS();
	if (CommBuf.RingBufRxCtr < COMM_RX_BUF_SIZE)
	{	
		CommBuf.RingBufRxCtr++;
		CommBuf.RingBufRx[CommBuf.RxTail++]= c;
		if (CommBuf.RxTail >= COMM_RX_BUF_SIZE)
		{	
			CommBuf.RxTail = 0;
		}
	}
	ISR_EN();
}
//***********************************************************************/
// *��    �ܣ� �����յ�����ȡ��com������
// *��    �ڣ� RingBufRxCtr �洢����������RingBufRxInPtr �������ݵ�ָ��
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u Comm_GetChar (int8u *err)
{	
	int8u c;
	
	ISR_DIS();
	if (CommBuf.RingBufRxCtr > 0)
	{	CommBuf.RingBufRxCtr--;
		c = CommBuf.RingBufRx[CommBuf.RxHead++];
		if (CommBuf.RxHead >= COMM_RX_BUF_SIZE)
		{	
			CommBuf.RxHead = 0;
		}
		*err = COMM_NO_ERR;
		ISR_EN();
		return (c);
	}
	else
	{	*err = COMM_RX_EPT;
		ISR_EN();
		return (NUL);
	}
}

//***********************************************************************/
void Comm_PutStr (int8u* ptr,int8u n)
{	int8u i;
	
	for(i=0;i<n;i++)
	{	
		Comm_PutChar(*ptr++);
	}
}
//***********************************************************************/
void Comm_TimerProc (void)
{
	if( CommTimer>0 ) 
	{   
	if(--CommTimer==0) 
	CommRxFlag=1;         //liuliu     2013-02-22
	}
}
//***********************************************************************/

//***********************************************************************/