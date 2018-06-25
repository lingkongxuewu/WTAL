/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Send.c
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
#define   __SEND_C__
#include  "_Include.h"

static void Send_RDeal ( char* buf,int8u n);
static void Send_CDeal ( char* buf,int8u n);
static void Send_SDeal ( char* buf,int8u n);
static void Send_WDeal ( char* buf,int8u n);
//static void Send_TDeal ( char* buf,int8u n);
static void Send_UDeal ( char* buf,int8u n);


static struct sensor xdata *px_ax=&accel_x;
static struct sensor xdata *py_ax=&accel_y;     


//***********************************************************************/
// *功    能：
// *入    口：CommTimer时间到 ，CommRxFlag有接收完成标志
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Send_TimeProc (void)
{   static xdata int8u rx_buf[COMM_RX_BUF_SIZE];
    static int8u* ptr;
    static int8u i;
    static int8u err;
    static int8u cmd;

//    if(COMM_TRAN==Comm_StateGet()) return;		 //COMM_TRAN==0，CommTimer减到0返回1进入COM处理，CommTimer减没到0返回0
    if(CommRxFlag==0)   return;					 //接收标志

    CommRxFlag = 0;
    ptr=rx_buf;									 
    for(i=0;i<COMM_RX_BUF_SIZE;i++)
    {   
		*ptr++=Comm_GetChar(&err);				  //将接收缓冲区的数据 取出存入	rx_buf
        if(err==COMM_RX_EPT)					 //COMM_RX_EPT ==1	取完标志
        break;
    }
    if(i<3)
    {
    #ifdef  COMM_485
        if(SendActPosi == 0)
        {  
			 MainTime4Cnt = 2000;
        }
    #endif
        return;
    }
#if MODBUS_CHECK_ENB==1
    if((rx_buf[0] == Addr) || (rx_buf[0] == 0))    //支持广播方式
    {

        ModRtu_RxProc(rx_buf,i);
        return;
    }
#endif
    if((0x0D!=rx_buf[i-2])||(0x0A!=rx_buf[i-1]))	 //判断后两位数据是否是回车换行
    {   
    #ifdef  COMM_485
        if(SendActPosi == 0)
        {   
			MainTime4Cnt = 2000;
        }
    #endif
        return;
    }
    cmd=rx_buf[0];
    switch(cmd)
    { case 'r': if(1!=SendActPosi)return;   //SendActPosi=0:主动发送模式，=1：应答模式
                Send_RDeal(rx_buf,i);
                break;
      case 'c': if(1!=SendActPosi)return;
                Send_CDeal(rx_buf,i);
                break;
      case 's': Send_SDeal(rx_buf,i);
                break;
      case 'u': Send_UDeal(rx_buf,i);
                break;
      case 'w': if(1!=SendActPosi)return;
                Send_WDeal(rx_buf,i);
                break;
//      case 't': if(1!=SendActPosi)return;
//                Send_TDeal(rx_buf,i);
//                break;
    }
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_RDeal ( char* buf,int8u n)
{   int8u data_8u;

    if(0==strncmp(buf,"rd m all",8))
    {   if(n!=10)return;
        Sub_SendPrmtAll();
    }
    else if(0==strncmp(buf,"rd ang s",8))
    {   if(n!=10)return;
        Sub_SendAngXYP();
    }
    else if(0==strncmp(buf,"rd ang",6))
    {   if(n!=8)return;
        Sub_SendAngXY();
    }
    else if(0==strncmp(buf,"rd ver",6))
    {   if(n!=8)return;
        Sub_SendSoftVer();
    }
    else if(0==strncmp(buf,"rd sn",5))
    {   if(n!=7)return;
        Sub_SendSysNum();
    }
    else if(0==strncmp(buf,"rd id",5))
    {   if(n!=7)return;
        Sub_SendID();
    }
    else if(0==strncmp(buf,"rd x offset",11))
    {   if(n!=13)return;
        Comm_PutStr("x offset,",9);
        Sub_SendOffset(GetAngXInst());                 // 获取安装角
    }
    else if(0==strncmp(buf,"rd y offset",11))
    {   if(n!=13)return;
        Comm_PutStr("y offset,",9);
        Sub_SendOffset(GetAngYInst());				  // 获取安装角
    }
    else if(0==strncmp(buf,"rd x cal,",9))
    {   if(n!=13)return;
        data_8u=Sub_Str2Int8u(&buf[9]);
        if(data_8u>=ANG_CAL_NUM){Sub_SendErr();return;}
        Sub_SendAngXCal(data_8u);
    }
    else if(0==strncmp(buf,"rd y cal,",9))
    {   if(n!=13)return;
        data_8u=Sub_Str2Int8u(&buf[9]);
        if(data_8u>=ANG_CAL_NUM){Sub_SendErr();return;}
        Sub_SendAngYCal(data_8u);
    }
    else Sub_SendErr();
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_CDeal ( char* buf,int8u n)
{
    if(0==strncmp(buf,"clr zero internal",17))	//温度补尝后再进做一次0点校准(清零)
    {   if(n!=19)Sub_SendErr();
        SetAngXSysZero(0);
        SetAngYSysZero(0);                      
        Comm_PutStr(buf,17);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"clr zero",8))		//用户清零
    {   if(n!=10)return;
        Comm_PutStr(buf,8);
        Sub_SendOk();
        SetAngXUsrZero(0);
        SetAngYUsrZero(0);
    }


    else if(0==strncmp(buf,"cal x sta",9))  //x开始校准
    {   if(n!=11)return;
        if(TRUE==Adc_XCalSta())
        {   Comm_PutStr(buf,9);
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"cal x stop",10))    //x结束校准
    {   if(n!=12)return;
        if(TRUE==Adc_XCalStop())
        {   Comm_PutStr(buf,10);
            Sub_SendOk();
            SetAngXSysZero(0);//X结束校准完，X轴相对测量值归0(内部清零）
            SetAngXUsrZero(0); //X结束校准完，X轴相对测量值归0(用户清零）

        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"cal y sta",9))  //y开始校准
    {   if(n!=11)return;
        if(TRUE==Adc_YCalSta())
        {   Comm_PutStr(buf,9);
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"cal y stop",10))    //y结束校准
    {   if(n!=12)return;
        if(TRUE==Adc_YCalStop())
        {   Comm_PutStr(buf,10);
            Sub_SendOk();
            SetAngYSysZero(0);//Y结束校准完，X轴相对测量值归0(内部清零）
            SetAngYUsrZero(0);// Y结束校准完，Y轴相对测量值归0(用户清零）
        }
        else Sub_SendErr();
    }

}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_SDeal ( char* buf,int8u n)
{   int8u data_ram,data_8u;
    int32s data_32s;
    int8u i;
    int8u SfrPage_ = SFRPAGE;

    if(0==strncmp(buf,"set mode p",10)  )       //set mode p
    {   if(n!=12)return;
        Comm_PutStr("set mode p",10);
        Sub_SendOk();
        SendActPosi=1;  						//SendActPosi=0:主动发送模式，=1：应答模式
    }
    else if(0==strncmp(buf,"set mode a",10))    //set mode a
    {   if(n!=12)return;
        Comm_PutStr("set mode a",10);
        Sub_SendOk();
        SendActPosi=0;  						//SendActPosi=0:主动发送模式，=1：应答模式
        if((Braud_Num < 3)&&(SendSpeedNum > 4))
        {
            SendSpeedNum = 3;
        }
    }
    else if((0==strncmp(buf,"set bps ",8))&&(0==SendActPosi))   //set bps n
    {   if(n!=11)return;
        if(((buf[8]-'0')>=0)&&((buf[8]-'0')<10))
        {
            i = buf[8]-'0';
            if((i > 3)&&(Braud_Num > 2)||(i < 4))		///2011-11-10 14:24修改波特率低于9600时发送频率不能高于4次每秒

            {
                SendSpeedNum= i;
                Comm_PutStr(buf,9);
                Sub_SendOk();
            }
            else
            {
                Sub_SendErr();
            }
            MainTime4Cnt=SendSpeed[SendSpeedNum];
        }
        else if((buf[8]=='t')&&(SendSpeedNum==0))
        {   Sub_SendAngXY();
        }
        else Sub_SendErr();
    }
    else if(1!=SendActPosi)return;  				//以下指令全部在SendActPosi==1 时才有效

    else if(0==strncmp(buf,"set filter1 num,",16))		//滤波系数可设
    {   if(n!=20)return;
        data_8u=Sub_Str2Int8u(&buf[16]);
        if((data_8u>99)||(data_8u<1)){Sub_SendErr();return;}
        SetFilterCnt(data_8u);                  
        Comm_PutStr(buf,18);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set zero internal",17))		//内部置0
    {   if(n!=19)return;
        SetAngXSysZero(GetXAng());
        SetAngYSysZero(GetYAng());            
        Comm_PutStr(buf,17);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set zero",8))       
    {   if(n!=10)return;
	    SetAngXUsrZero(GetAngXRelSys());
        SetAngYUsrZero(GetAngYRelSys());      		//用户置0（获取系统置0）
		Comm_PutStr(buf,8);
        Sub_SendOk();
    }
    else if(0==strncmp(buf,"set x offset,",13))     //设置安装角
    {   if(n!=22)return;
        if(Sub_Str2Int32s(&data_32s,&buf[13])==FALSE){Sub_SendErr();return;}
        Comm_PutStr(buf,20);
        Sub_SendOk();
        SetAngXInst(data_32s);                      
    }
    else if(0==strncmp(buf,"set y offset,",13))     //
    {   if(n!=22)return;
        if(Sub_Str2Int32s(&data_32s,&buf[13])==FALSE){Sub_SendErr();return;}
        Comm_PutStr(buf,20);
        Sub_SendOk();
        SetAngYInst(data_32s);                  
    }
    else if(0==strncmp(buf,"set ang num,",12))		//角度校准点数可设
    {   if(n!=16)return;
        data_ram = Sub_Str2Int8u(&buf[12]);
        if((data_ram>ANG_CAL_NUM)||(data_ram<2))return;
        SetAngCalCnt(data_ram);
        Comm_PutStr(buf,14);
        Sub_SendOk();
    }						 
    else if(0==strncmp(buf,"samp x,",7))            //X轴校准
    {   if(n!=16)return;
        if(Sub_Str2Int32s(&data_32s,&buf[7])==FALSE){Sub_SendErr();return;}
        if(TRUE==Adc_XCalProc(data_32s,GetXFtVal()))   
        {   Comm_PutStr(&buf[5],9);
            Comm_PutChar(',');
            Comm_PutInt16u(GetXFtVal());
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"single samp x,",14))    //X轴单点校准     Ang_Xcal.angarr[i]
    {   if(n!=23)return;
        if(Sub_Str2Int32s(&data_32s,&buf[14])==FALSE){Sub_SendErr();return;}
        if((GetAngXCalCnt()<1)||(FALSE == AngChkDiff(data_32s,GetXFtVal(),GetAngXCalCnt(),px_ax->angcal->angarr,px_ax->angcal->adcarr))) //

        {   Sub_SendErr();
            return;
        }
        Comm_PutStr(&buf[0],21);
        Comm_PutChar(',');
        Comm_PutInt16u(GetXFtVal());
        Sub_SendOk();
        ACalXId=GetAngXCalCnt();    			 //獲取X軸校準個數 
        MenuId=MOD_X_CAL;
        Adc_XCalStop();
    }
    else if(0==strncmp(buf,"samp y,",7))        //Y轴校准
    {   if(n!=16)return;
        if(Sub_Str2Int32s(&data_32s,&buf[7])==FALSE){Sub_SendErr();return;}
        if(TRUE==Adc_YCalProc(data_32s,GetYFtVal()))    
        {   Comm_PutStr(&buf[5],9);
            Comm_PutChar(',');
            Comm_PutInt16u(GetYFtVal());                
            Sub_SendOk();
        }
        else Sub_SendErr();
    }
    else if(0==strncmp(buf,"single samp y,",14))    //Y轴单点校准
    {   if(n!=23)return;
        if(Sub_Str2Int32s(&data_32s,&buf[14])==FALSE){Sub_SendErr();return;}
        if((GetAngXCalCnt()<1)||(FALSE == AngChkDiff(data_32s,GetYFtVal(),GetAngXCalCnt(),py_ax->angcal->angarr,py_ax->angcal->adcarr)))

        {   Sub_SendErr();
            return;
        }
        Comm_PutStr(&buf[0],21);
        Comm_PutChar(',');
        Comm_PutInt16u(GetYFtVal());

        Sub_SendOk();
        ACalYId=GetAngXCalCnt();    
        MenuId=MOD_Y_CAL;
        Adc_YCalStop();
    }
    else if(0==strncmp(buf,"set baud ",9))//设置UART通信参数
    {
        if(n!=12)return;
        data_8u = buf[9]-'0';
        if(data_8u>9)return;
        Braud_Num=data_8u;
        Comm_PutStr(buf,10);
        Sub_SendOk();
        delay_ms(500);
        SFRPAGE = 0x0F;
        SBRL0   = -(SYSCLK / Braud[Braud_Num] / 2);//设置波特率   SFRPAGE=0x0F     SBRL0 = -(SYSCLK / BAUDRATE / 2);
        SFRPAGE = SfrPage_;
    }
	 	 	  
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_WDeal ( char* buf,int8u n)
{  
	if(0==strncmp(buf,"wr sn,",6))//写产品编号
    {   if(n!=(SystemVerNum+8))
        {   Sub_SendErr();
            return;
        }
        strncpy(SystemVer,&buf[6],SystemVerNum);
        Comm_PutStr(buf,(SystemVerNum+6));
        Sub_SendOk();
    }
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Send_UDeal( char* buf,int8u n)
{
    if(0==strncmp(buf,"update flash sys",16))
    {

        if(n!=18)return;
        Flash_CalDataUpdate();//保存一次数据
        Comm_PutStr(buf,16);
        Sub_SendOk();

    }
    else if(0==strncmp(buf,"update flash ",12))
    {
        if(n!=14)return;
        Flash_UserDataUpdate();
        Comm_PutStr(buf,12);
        Sub_SendOk();
    }

    else Sub_SendErr();
}



//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
//static void Send_TDeal( char* buf,int8u n)
//{
//    if(0==strncmp(buf,"t cal test",10))
//    {   if(n!=12)return;
//        Sub_SendTCalTest();
//    }
//}

//***********************************************************************/
