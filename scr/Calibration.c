/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Calibration.c
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
#define __CALIBR_C__
#include "_Include.h"
//struct temp_calib xdata Temp_Xcal;    //X轴温度补偿
//struct temp_calib xdata Temp_Ycal;    //Y轴温度补偿

//struct ang_calib xdata Ang_Xcal;    //X轴角度校准
//struct ang_calib xdata Ang_Ycal;    //Y轴角度校准

//static int16u xdata temp_buf[TEMP_CAL_NUM];    //21

static float    Ang_RdXAVal(struct ang_calib *p_cal, int8u posid);



//***********************************************************************/
// *功    能：设置角度标定点个数
// *入    口：p_cal,ang_calib结构体指针(X,Y);num,点数
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Ang_SetCalCnt(struct ang_calib *p_cal, int8u num)
{
    p_cal->calcnt=num;
}
//***********************************************************************/
// *功    能：读取角度标定模块中的角度值数组中的某个元素
// *入    口：p_cal,ang_calib结构体指针(X,Y);posid,位置ID号
// *出    口：角度值
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static int32s   Ang_RdAngVal(struct ang_calib *p_cal, int8u posid)
{
    return (p_cal->angarr[posid]);
}

//***********************************************************************/
// *功    能：读取角度标定模块中的角度AD值数组中的某个元素
// *入    口：p_cal,ang_calib结构体指针(X,Y);posid,位置ID号
// *出    口：角度ADC值
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static int16u   Ang_RdAdcVal(struct ang_calib *p_cal, int8u posid)
{
    return (p_cal->adcarr[posid]);
}
//***********************************************************************/
// *功    能：设置角度标定模块中的角度值数组中的某个元素
// *入    口：p_cal,ang_calib结构体指针(X,Y);val,角度值;posid,位置ID号
// *出    口：
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static void Ang_WrAngVal(struct ang_calib *p_cal,int32s val, int8u posid)
{
    p_cal->angarr[posid]=val;
}

//***********************************************************************/
// *功    能：设置角度标定模块中的角度AD值数组中的某个元素
// *入    口：p_cal,ang_calib结构体指针(X,Y);val,角度ADC值;posid,位置ID号
// *出    口：
// *备    注：保证位置ID在数组大小的范围内
// *函数性质：
//***********************************************************************/
static void Ang_WrAdcVal(struct ang_calib *p_cal,int16u val, int8u posid)
{
    p_cal->adcarr[posid]=val;
}

//***********************************************************************/
// *功    能：角度A,K参数计算
// *入    口：p_cal,ang_calib结构体指针(X,Y);
// *出    口：
// *备    注：K=(sin(ang1)-sin(ang0))/(adc1-adc0);A=sin(ang0)-K*adc0;
// *函数性质：
//***********************************************************************/
static void Ang_CalcPrm(struct ang_calib *p_cal)
{
    float ang0;
    float ang1;

    int8u i;
    for(i=0;i<(p_cal->calcnt-1);i++)
    {
        ang0=(PI*(p_cal->angarr[i]))/180000.0;    //角度换算成弧度
        ang1=(PI*(p_cal->angarr[i+1]))/180000.0;
        p_cal->prm_k[i] = (sin(ang1)-sin(ang0))/(int16s)(p_cal->adcarr[i+1]-p_cal->adcarr[i]);
        p_cal->prm_a[i] = sin(ang0)- p_cal->prm_k[i] * (p_cal->adcarr[i]);
    }
}



int8u GetAngXCalCnt(void)
{
    return (Ang_Xcal.calcnt);    //获取X轴角度校准点个数
}
/*
int8u GetTempYCalCnt(void)
{
    return (Temp_Ycal.calcnt);    //获取Y轴温度补偿点个数 (没有用到注释起来）
}
*/
int8u GetAngYCalCnt(void)
{
    return (Ang_Ycal.calcnt);    //获取Y轴角度校准点个数
}


//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/

void SetAngXCalCnt(int8u cnt)
{
     Ang_SetCalCnt(&Ang_Xcal,cnt);     //设置X轴角度校准点个数
}



void SetAngYCalCnt(int8u cnt)
{
     Ang_SetCalCnt(&Ang_Ycal,cnt);    //设置Y轴角度校准点个数
}



void SetAngCalCnt(int8u cnt)         //设置角度校准点个数
{
     SetAngXCalCnt(cnt);
     SetAngYCalCnt(cnt);
}




//***********************************************************************/
// *功    能：读取角度标定模块中的X轴角度值数组中的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度值
// *备    注：保证位置ID在数组大小的范围内
//***********************************************************************/
int32s GetAngXCalAngVal(int8u pos)
{
    return (Ang_RdAngVal(&Ang_Xcal,pos));
}

//***********************************************************************/
// *功    能：读取角度标定模块中的Y轴角度值数组中的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度值
// *备    注：保证位置ID在数组大小的范围内
//***********************************************************************/
int32s GetAngYCalAngVal(int8u pos)
{
    return (Ang_RdAngVal(&Ang_Ycal,pos));
}


//***********************************************************************/
// *功    能：读角度标定模块中X轴ADC数组的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度ADC值
// *备    注：保证位置在数组的范围内
//***********************************************************************/
int16u GetAngXCalAdcVal(int8u pos)
{
    return (Ang_RdAdcVal(&Ang_Xcal,pos));
}
//***********************************************************************/

//***********************************************************************/
// *功    能：读角度标定模块中Y轴ADC数组的某个元素
// *入    口：pos,数组中的位置
// *出    口：角度ADC值
// *备    注：保证位置在数组的范围内
//***********************************************************************/
int16u GetAngYCalAdcVal(int8u pos)
{
    return (Ang_RdAdcVal(&Ang_Ycal,pos));
}






//***********************************************************************/
// *功    能：设置角度标定模块中的X轴角度值数组中的某个元素
// *入    口：val,角度值;pos,位置
// *出    口：
// *备    注：保证位置ID在数组大小的范围内
//***********************************************************************/
void SetAngXCalAngVal(int32s val, int8u pos)
{
    Ang_WrAngVal(&Ang_Xcal,val,pos);
}

//***********************************************************************/
// *功    能：设置角度标定模块中的Y轴角度值数组中的某个元素
// *入    口：val,角度值;pos,位置
// *出    口：
// *备    注：保证位置ID在数组大小的范围内
//***********************************************************************/
void SetAngYCalAngVal(int32s val, int8u pos)
{
    Ang_WrAngVal(&Ang_Ycal,val,pos);
}


//***********************************************************************/
// *功    能：设置角度标定模块中的X轴角度AD值数组中的某个元素
// *入    口：val,角度ADC值;pos,位置
// *出    口：
// *备    注：保证位置在数组的范围内
//***********************************************************************/
void SetAngXCalAdcVal(int16u val, int8u pos)
{
    Ang_WrAdcVal(&Ang_Xcal,val,pos);
}

//***********************************************************************/
// *功    能：设置角度标定模块中的Y轴角度AD值数组中的某个元素
// *入    口：val,角度ADC值;pos,位置
// *出    口：
// *备    注：保证位置在数组的范围内
//***********************************************************************/
void SetAngYCalAdcVal(int16u val, int8u pos)
{
    Ang_WrAdcVal(&Ang_Ycal,val,pos);
}




//***********************************************************************/
// *功    能：X轴角度标定参数计算
// *备    注：
//***********************************************************************/
void    AngXCalcPrm(void)
{
    Ang_CalcPrm(&Ang_Xcal);
}
//***********************************************************************/
// *功    能：Y轴角度标定参数计算
// *备    注：
//***********************************************************************/
void    AngYCalcPrm(void)
{
    Ang_CalcPrm(&Ang_Ycal);
}



//***********************************************************************/
// *功    能：角度标定默认数据初始化
// *入    口：
// *出    口：
// *备    注：Flash 初始化的时候调用
// *函数性质：
//***********************************************************************/
void AngCalDataInit()
{
    int8u i;
    SetAngCalCnt(3);          //角度校准点个数
    for(i=0;i<ANG_CAL_NUM;i++)//角度
    {
        Ang_Xcal.angarr[i] = 2000*(i-ANG_CAL_NUM/2);
        Ang_Xcal.adcarr[i] = 500*(i-ANG_CAL_NUM/2)+8000;//2011-11-07 修改校准角度为三位小数

        Ang_Ycal.angarr[i] = 2000*(i-ANG_CAL_NUM/2);
        Ang_Ycal.adcarr[i] = 500*(i-ANG_CAL_NUM/2)+8000;
    }
    //----------------test-------------------
    Ang_Xcal.angarr[0]  =   -30000  ;
    Ang_Xcal.adcarr[0]  =   764     ;//2011-11-07 修改校准角度为三位小数
    Ang_Xcal.angarr[1]  =   0       ;
    Ang_Xcal.adcarr[1]  =   7907    ;
    Ang_Xcal.angarr[2]  =   30000   ;
    Ang_Xcal.adcarr[2]  =   14988   ;

    Ang_Ycal.angarr[0]  =   -30000  ;
    Ang_Ycal.adcarr[0]  =   744     ;
    Ang_Ycal.angarr[1]  =   0       ;
    Ang_Ycal.adcarr[1]  =   7979    ;
    Ang_Ycal.angarr[2]  =   30000   ;
    Ang_Ycal.adcarr[2]  =   15107   ;
    //-------------test-------------------	
}
//***********************************************************************/
// *功    能：校准模块初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：初始化函数指针
//***********************************************************************/
void    CalibInit(void)
{
//    Temp_Xcal.SetCalNum=SetTempXCalCnt;     //设置校准个数
//    Temp_Xcal.RdTempVal=GetTempXCalTempVal;  //读设置数据
//    Temp_Xcal.RdAdcVal=GetTempXCalAdcVal;    //读数据
//    Temp_Xcal.WrTempVal=SetTempXCalTempVal;   //写数据
//    Temp_Xcal.WrAdcVal=SetTempXCalAdcVal;     //写返回数据
////  Temp_Xcal.CalcPrm=TempXCalcPrm;           //计算参数A,K
//
//    Temp_Ycal.ptempval=temp_buf;            //温度补偿模块中温度AD值缓存数组
//    Temp_Ycal.SetCalNum=SetTempYCalCnt;
//    Temp_Ycal.RdTempVal=GetTempYCalTempVal;
//    Temp_Ycal.RdAdcVal=GetTempYCalAdcVal;
//    Temp_Ycal.WrTempVal=SetTempYCalTempVal;
//    Temp_Ycal.WrAdcVal=SetTempYCalAdcVal;
////  Temp_Ycal.CalcPrm=TempYCalcPrm;

    Ang_Xcal.SetCalNum=SetAngXCalCnt;
    Ang_Xcal.RdAngVal=GetAngXCalAngVal;
    Ang_Xcal.RdAdcVal=GetAngXCalAdcVal;
    Ang_Xcal.WrAngVal=SetAngXCalAngVal;
    Ang_Xcal.WrAdcVal=SetAngXCalAdcVal;
//  Ang_Xcal.CalcPrm=AngXCalcPrm;

    Ang_Ycal.SetCalNum=SetAngYCalCnt;
    Ang_Ycal.RdAngVal=GetAngYCalAngVal;
    Ang_Ycal.RdAdcVal=GetAngYCalAdcVal;
    Ang_Ycal.WrAngVal=SetAngYCalAngVal;
    Ang_Ycal.WrAdcVal=SetAngYCalAdcVal;
//  Ang_Ycal.CalcPrm=AngYCalcPrm;
}
