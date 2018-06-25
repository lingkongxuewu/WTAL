/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Sub.c
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
#define   __SUB_C__
#include  "_Include.h"
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：32有符号位转BCD 值存DisBuf数组，后将BCD存于发送缓冲区
//***********************************************************************/
void Comm_PutInt32s (int32s val)
{
	Sub_Int32s2Str(DispBuf,val);	// 将角度值转BCD ，存于DisBuf[]
	Comm_PutStr((int8u*)DispBuf,7);	// 从DisBuf[]将XY的角度值存到发送缓冲区
}
//***********************************************************************/
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：16有符号位转BCD 值存DisBuf数组，后将BCD存于发送缓冲区
//***********************************************************************/
void Comm_PutInt16s (int16s val)
{
	Sub_Int16s2Str(DispBuf,val);
	Comm_PutStr((int8u*)DispBuf,6);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：16无符号位转BCD 值存DisBuf数组，后将BCD存于发送缓冲区
//***********************************************************************/
void Comm_PutInt16u (int16u val)
{
	Sub_Int16u2Str(DispBuf,val);
	Comm_PutStr((int8u*)DispBuf,5);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：8无符号位转BCD 值存DisBuf数组，后将BCD存于发送缓冲区
//***********************************************************************/
void Comm_PutInt8u (int8u val)
{
	Sub_Int8u2Str(DispBuf,val);
	Comm_PutStr((int8u*)DispBuf,2);
}
//***********************************************************************/
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：将回车换行的ASCII值存TX缓冲区
//***********************************************************************/
void Comm_PutNewLine (void)
{
	Comm_PutChar(0x0D);
	Comm_PutChar(0x0A);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：Send "ok"
//***********************************************************************/
void Sub_SendOk (void)
{	
	char* str={",OK"};
	Comm_PutStr((int8u*)str,3);
	Comm_PutNewLine();
	Comm_SendSta();
}




//***********************************************************************/
void Sub_SendErr (void)
{	char* str={"ERR"};
	
	
	Comm_PutStr((int8u*)str,3);
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendAngXY (void)
{
	Comm_PutStr("X=",2);						 //将字符存入发生缓冲区RingBufTx[COMM_TX_BUF_SIZE]
	Comm_PutInt32s(GetXAngAllSetVal());			//(滤波后的角度+系统、用户、安装角后的角度）转BCD

//	Comm_PutNewLine();							//回车换行
	Comm_PutStr("  Y=",4);
	Comm_PutInt32s(GetYAngAllSetVal());			//

	Comm_PutNewLine();
//	Comm_PutChar(0x05);
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendAngXYP (void)
{	
	Comm_PutStr("X=",2);
	Comm_PutInt32s(GetXAngAllSetVal());		
//	Comm_PutInt32s(StarStp_X.old_ang);	
//	Comm_PutNewLine();
	Comm_PutStr("  Y=",4);
	Comm_PutInt32s(GetYAngAllSetVal());		
//	Comm_PutInt32s(StarStp_Y.old_ang);
//	Comm_PutNewLine();
	Comm_PutStr("  P=",4);
	Comm_PutInt32s(GetAngP());				
	Comm_PutNewLine();
//	Comm_PutChar(0x05);
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendPrmtAll(void)
{
	Comm_PutStr("M,",2);	Comm_PutInt16u(Get_McuTemp());

	Comm_PutStr(",x,",3);	Comm_PutInt16u(GetXFtVal());			 
//	Comm_PutChar(',');		Comm_PutInt16u(GetXOftVal());			  
	Comm_PutChar(',');		Comm_PutInt32s(GetXAng());				   

	Comm_PutStr(",y,",3);	Comm_PutInt16u(GetYFtVal());			   
//	Comm_PutChar(',');		Comm_PutInt16u(GetYOftVal());			  
	Comm_PutChar(',');		Comm_PutInt32s(GetYAng());

//	Comm_PutChar(',');		Comm_PutInt32s(GetYOffset(Get_McuTemp()));
//	Comm_PutChar(',');		Comm_PutInt32s(Get_McuTemp());

	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendAngXCal (int8u i)
{	
	Comm_PutStr("X,",2);	Comm_PutInt8u(i);
	Comm_PutStr(",",1);	   Comm_PutInt16u(GetAngXCalAdcVal(i+0));		 

	Comm_PutStr(",K,",3);	Comm_PutInt16s(100000000*Ang_Xcal.prm_k[i+0]); 
	Comm_PutStr(",A,",3);	Comm_PutInt16s(100000000*Ang_Xcal.prm_a[i+0]); 

	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendAngYCal (int8u i)
{	
	Comm_PutStr("Y,",2);	Comm_PutInt8u(i);
	Comm_PutChar(',');		Comm_PutInt16u(GetAngYCalAdcVal(i+0));        
	Comm_PutStr(",K,",3);	Comm_PutInt16s(100000000*Ang_Ycal.prm_a[i+0]); 
	Comm_PutStr(",A,",3);	Comm_PutInt16s(100000000*Ang_Ycal.prm_a[i+0]); 

	Comm_PutNewLine();
	Comm_SendSta();
}
////***********************************************************************/
//void Sub_SendTempCal (int8u i)
//{	
//	Comm_PutStr("t,",2);	Comm_PutInt8u(i);
//	Comm_PutChar(',');		Comm_PutInt16u(GetTempXCalTempVal(i+0)); 
//	
//	Comm_PutStr(",x,",3);	Comm_PutInt16s(GetTempXCalAdcVal(i+0));
//	Comm_PutStr(",y,",3);	Comm_PutInt16s(GetTempYCalAdcVal(i+0));	  
//
//	Comm_PutStr(",XK,",4);	Comm_PutInt16s(1000*Temp_Xcal.prm_k[i+0]);
//	Comm_PutStr(",A,",3);	Comm_PutInt16s(Temp_Xcal.prm_a[i+0]);
//	Comm_PutStr(",YK,",4);	Comm_PutInt16s(1000*Temp_Ycal.prm_k[i+0]);
//	Comm_PutStr(",A,",3);	Comm_PutInt16s(Temp_Ycal.prm_a[i+0]);
//
//	Comm_PutNewLine();
//	Comm_SendSta();
//}
//***********************************************************************/
void Sub_SendOffset (int32s val)
{	
	Comm_PutInt32s(val);
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendID (void)
{
	Comm_PutInt16u(MenuId);			Comm_PutChar(',');
	Comm_PutInt16u(GetAngXCalCnt());Comm_PutChar(',');	  		//HXL	整合更新 12/8/26 ？
	Comm_PutInt16u(ACalXId);		Comm_PutChar(',');
	Comm_PutInt16u(ACalYId);		Comm_PutChar(',');
//	Comm_PutInt16u(GetTempXCalCnt());							//HXL	整合更新 12/8/26 
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendSoftVer (void)
{		
	Comm_PutStr("097,186B,20180316",17);	
	Comm_PutNewLine();
	Comm_SendSta();
}
//***********************************************************************/
void Sub_SendSysNum (void)
{			
	Comm_PutStr("sn,",3);
	Comm_PutStr(SystemVer,SystemVerNum);	
	Comm_PutNewLine();
	Comm_SendSta();
}
////***********************************************************************/
//void Sub_SendTCalTest (void)
//{	
//	int16s	TempX;
//	int16s	TempY;
//
//	TempX = accel_x.GetTempOffset(Get_McuTemp());	//温度补尝
//	TempY = accel_y.GetTempOffset(Get_McuTemp());	//温度补尝	  HXL 12/8/27
//
//	Comm_PutInt16u(TempA);	Comm_PutChar(',');
//	Comm_PutInt16s(TempX);	Comm_PutChar(',');
//	Comm_PutInt16s(TempY);
//	Comm_PutNewLine();
//	Comm_SendSta();
//	
//	TempA++;
//}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u Sub_Str2Int32s (int32s *val,int8u* str)
{	int32s temp;	
	
	
	if((str[1]<'0')||(str[1]>'9'))return FALSE;
	if((str[2]<'0')||(str[2]>'9'))return FALSE;
	if(str[3]!='.')				  return FALSE;
	if((str[4]<'0')||(str[4]>'9'))return FALSE;
	if((str[5]<'0')||(str[5]>'9'))return FALSE;
	if((str[6]<'0')||(str[6]>'9'))return FALSE;
	temp=(int32s)(str[1]-'0')*10000+(str[2]-'0')*1000+(str[4]-'0')*100+(str[5]-'0')*10+(str[6]-'0');
	if(str[0]=='+')
	{	*val = temp;
		return TRUE;
	}
	else if(str[0]=='-')
	{	*val = -temp;
		return TRUE;
	}
	return FALSE;
}
////***********************************************************************/
//int8u Sub_Str2Int16s (int16s *val,int8u* str)
//{	int16u temp;
//	if(FALSE==Sub_Str2Int16u(&temp,&str[1]))return (FALSE);
//	
//	if(str[0]=='+')
//	{	*val = temp;
//		return TRUE;
//	}
//	else if(str[0]=='-')
//	{	*val = -temp;
//		return TRUE;
//	}
//	return FALSE;
//}
////***********************************************************************/
//int8u Sub_Str2Int16u (int16u *val,int8u* str)
//{	int8u i;
//	
//	
//	for(i=0;i<5;i++)
//	{	if((str[i]<'0')||(str[i]>'9'))
//		return FALSE;
//	}
//	*val=(str[0]-'0')*10000+(str[1]-'0')*1000+(str[2]-'0')*100+(str[3]-'0')*10+(str[4]-'0');
//	return TRUE;
//}
//***********************************************************************/
int8u Sub_Str2Int8u (int8u* str)
{	int8u val;	
	
	if((str[0]<'0')||(str[0]>'9'))return(100);
	if((str[1]<'0')||(str[1]>'9'))return(100);
	val=(str[0]-'0')*10+(str[1]-'0');
	return  val;
}
//***********************************************************************/
void Sub_Int32s2Str (char* s,int32s val)
{
	if(val<0)
	{	
		val=-val;
		s[0]='-';
	}
	else
	{	
		s[0]='+';
	}
	if(val>99999)val-=100000;
	
	s[1] = val/10000+'0';
	val  = val%10000;
	s[2] = val/1000+'0';
	val  = val%1000;
	s[3] = '.';
	s[4] = val/100+'0';
	val  = val%100;
	s[5] = val/10+'0';
	s[6] = val%10+'0';
}
//***********************************************************************/
void Sub_Int16s2Str (char* s,int16s val)
{
	if(val<0)
	{	val=-val;
		s[0]='-';
	}
	else
	{	s[0]='+';
	}
	Sub_Int16u2Str(&s[1],val);
}
//***********************************************************************/
void Sub_Int16u2Str (char* s,int16u val)
{
	s[0] = val/10000+'0';
	val  = val%10000;
	s[1] = val/1000+'0';
	val  = val%1000;
	s[2] = val/100+'0';
	val  = val%100;
	s[3] = val/10+'0';
	s[4] = val%10+'0';
}
//***********************************************************************/
void Sub_Int8u2Str (char* s,int8u val)
{
	val  = val%100;
	s[0] = val/10+'0';
	s[1] = val%10+'0';
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
/*
void Sub_TVCalValCnt (int16u temp0,int16u temp1,int16s adc0,int16s adc1,float* a,float* k)
{
	*k = ((float)adc1-adc0)/((float)temp1-temp0);
	*a = (float)adc0- *k * temp0;
}
*/
//***********************************************************************/
/*
void Sub_AngCalValCnt (int16u adc0,int16u adc1,int32s ang0,int32s ang1,float* a,float* k)
{	float _ang0=(PI*ang0)/180000.0;
	float _ang1=(PI*ang1)/180000.0;
	
	*k = (sin(_ang1)-sin(_ang0))/(int16s)(adc1-adc0);
	*a = sin(_ang0) - *k * adc0;
}
*/   //HXL 12/8/28
//***********************************************************************/
/*
void Sub_AngCalValGet (float* a,float* k,int16u adc,int8u tag)
{	int8u i;
	
	
	if(tag==0)
	{	//for(i=0;i<ACalN-2;i++)				//HXL 12/8/26  原來的
		for(i=0;i<(GetAngXCalCnt()-2);i++)		//HXL 12/8/26  整合更新
		{
//			if(adc<ACalXAdc[i+1])break;		
			if(adc<GetAngXCalAdcVal(i+1))break;	//HXL 12/8/27  整合更新
		}
//		*a=APrmtXA[i];	
//		*k=APrmtXK[i];
		*a=Ang_Xcal.prm_a[i]; //HXL 		12/8/27
		*k=Ang_Xcal.prm_k[i]; //HXL 		12/8/27
	}
	else
	{	//for(i=0;i<ACalN-2;i++)
		for(i=0;i<(GetAngYCalCnt()-2);i++)		//HXL 12/8/26  整合更新
		{
//			if(adc<ACalYAdc[i+1])break;
			if(adc<GetAngYCalAdcVal(i+1))break;	//HXL 12/8/27  整合更新	
		}
//		*a=APrmtYA[i];	
//		*k=APrmtYK[i];
		*a=Ang_Ycal.prm_a[i]; //HXL 		12/8/27
		*k=Ang_Ycal.prm_k[i]; //HXL 		12/8/27
				
	}
}
*/
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Sub_SBufSwitch (int32s* buf,int8u i,int8u j)
{	int32s temp;
	
	
	temp=buf[i];
	buf[i]=buf[j];
	buf[j]=temp;
}
//***********************************************************************/

//void Sub_16SBufSwitch (int16s* buf,int8u i,int8u j)
//{	int16s temp;
//	
//	
//	temp=buf[i];
//	buf[i]=buf[j];
//	buf[j]=temp;
//}
 //HXL 12/8/28 
//***********************************************************************/
void Sub_UBufSwitch (int16u* buf,int8u i,int8u j)
{	int16u temp;
	
	
	temp=buf[i];
	buf[i]=buf[j];
	buf[j]=temp;
}
//***********************************************************************/
void Sub_BufBubble (int32s* id,int16u* x,int8u n)
{
	int8u i,j;
	for(i=0;i<n;i++)
	{	for(j=n-1;j>i;j--)
		{	if(id[i]>id[j])
			{	Sub_SBufSwitch(id,i,j);
				Sub_UBufSwitch(x,i,j);
			}
		}
	}
}
////***********************************************************************/
//void Sub_BufBubble2 (int16u* id,int16s* x,int16s* y,int8u n)
//{
//	int8u i,j;
//	for(i=0;i<n;i++)
//	{	for(j=n-1;j>i;j--)
//		{	if(id[i]>id[j])
//			{	Sub_UBufSwitch(id,i,j);
//				Sub_16SBufSwitch(x,i,j);
//				Sub_16SBufSwitch(y,i,j);
//			}
//		}
//	}
//}
  //HXL 12/8/28
//***********************************************************************/
/*
void Sub_SysRst (void)
{	int8u SfrPage_ = SFRPAGE;
	
	
	SFRPAGE = ACTIVE_PAGE;
	RSTSRC|=0x10;
	SFRPAGE = SfrPage_;
}
*/
//***********************************************************************/


//int8u Sub_StralarmCheck(int32s *val,int8u* str)
//{
//	if((str[0]=='H')||(str[0]=='L'))
//	{
//		*val=str[0];
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//
//}
