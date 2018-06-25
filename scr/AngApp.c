/*--------------------------------文*件*信*息-----------------------------
* 文件名：  AngApp.c
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
#define __CAL_C__
#include "_Include.h"


static int32s xdata  angp;
static int32s xdata  angx_syszero;
static int32s xdata  angy_syszero;
static int32s xdata  angx_usrzero;
static int32s xdata  angy_usrzero;
static int32s xdata  angx_inst;     //安装角
static int32s xdata  angy_inst;     //安装角
static int32s xdata  BufZerox_usr;
static int32s xdata  BufZeroy_usr;
static int32s xdata  BufZerox_sys;
static int32s xdata  BufZeroy_sys;
	

void AngInit(void)
{
    SnInit();                   //传感器模块初始化

    angp=0;                     //面角度
    SetAngXInst(0);
    SetAngYInst(0);             //安裝角
    SetAngXSysZero(0);
    SetAngYSysZero(0);          //系統置0
    SetAngXUsrZero(0);          //用戶置0
    SetAngYUsrZero(0);
                                //
    BufZerox_usr=0;             //用户置0后相对角度值
    BufZeroy_usr=0;
    BufZerox_sys=0;
    BufZeroy_sys=0;

}
//***********************************************************************/
// *功    能：面角度计算
// *入    口：x= X轴倾角值 y = y轴倾角值 p = 面角度存储地址
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static  int32s  PAngCalc (int32s angx,int32s angy)
{
    float x,y,s;

    x  = angx;
    x  = x / 180000 * PI;
    x  = sin(x);
    x *= x;

    y  = angy;
    y  = y / 180000 * PI;
    y  = sin(y);
    y *= y;

    s  = sqrt(x+y);
    if( s >= 1 )//面角度计算出错?
    {
        return 90000;
    }
    s = 180000 * asin(s) / PI ;
    return( (int32s)s );
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int32s GetAngP(void)
{

	angp=PAngCalc(GetXAngAllSetVal(),GetYAngAllSetVal());
    return angp;    //获取面角度
}

int32s GetAngXSysZero(void)
{
    return angx_syszero;    //获取X轴系统置零参数
}
int32s GetAngYSysZero(void)
{
    return angy_syszero;    //获取Y轴系统置零参数
}
int32s GetAngXUsrZero(void)
{
    return angx_usrzero;    //获取X轴用户置零参数
}
int32s GetAngYUsrZero(void)
{
    return angy_usrzero;    //获取Y轴用户置零参数
}

int32s GetAngXInst(void)
{
    return angx_inst;    //获取X轴用户安装角参数
}
int32s GetAngYInst(void)
{
    return angy_inst;    //获取Y轴用户安装角参数
}


#if MODBUS_CHECK_ENB==1
int32s GetAngXZeroUsr(void)
{
    return  BufZerox_usr; //获取X轴相对角度值用户       
}
int32s GetAngYZeroUsr(void)
{
    return  BufZeroy_usr; //获取y轴相对角度值用户       
}
#endif
int32s GetAngXRelSys(void)
{
    return  BufZerox_sys; //获取X轴相对角度值内部       
}
int32s GetAngYRelSys(void)
{
    return  BufZeroy_sys; //获取y轴相对角度值内部       
}

//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void SetAngXSysZero(int32s zore)
{
    angx_syszero=zore;    //设置X轴系统置零参数
}
void SetAngYSysZero(int32s zore)
{
    angy_syszero=zore;    //设置Y轴系统置零参数
}

void SetAngXUsrZero(int32s zore)
{
    angx_usrzero=zore;    //设置X轴用户置零参数
}
void SetAngYUsrZero(int32s zore)
{
    angy_usrzero=zore;    //设置Y轴用户置零参数
}

void SetAngXInst(int32s ang_inst)
{
    angx_inst=ang_inst;    //设置X轴用户安装角参数
}
void SetAngYInst(int32s ang_inst)
{
    angy_inst=ang_inst;    //设置Y轴用户安装角参数
}



//***********************************************************************/
// *功    能：设置角度值0位置
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void SetAngZero (int32s *angval ,int32s setval)
{
    *angval=*angval-setval;
}

void SetAngZeroAdd (int32s *angval ,int32s setval)
{
    *angval=*angval+setval;
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int32s GetXAngAllSetVal(void)
{
    int32s tmp;
    tmp=GetXAng();                    //获取X轴的原始角度值
    SetAngZero(&tmp,angx_syszero);    //系统置0
    BufZerox_sys=tmp;                 //系统置0后相对角度值
    SetAngZero(&tmp,angx_usrzero);    //用户置0
    BufZerox_usr=tmp;                 //用户置0后相对角度值
    SetAngZeroAdd(&tmp,angx_inst);    //安装角
    return tmp;

}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int32s GetYAngAllSetVal(void)
{
    int32s tmp;
    tmp=GetYAng();                    //获取X轴的原始角度值
    SetAngZero(&tmp,angy_syszero);    //系统置0
    BufZeroy_sys=tmp;                 //系统置0后相对角度值
    SetAngZero(&tmp,angy_usrzero);    //用户置0
    BufZeroy_usr=tmp;                 //用户置0后相对角度值
    SetAngZeroAdd(&tmp,angy_inst);    //安装角
    return tmp;
}


//***********************************************************************/
//        E N D
//***********************************************************************/
