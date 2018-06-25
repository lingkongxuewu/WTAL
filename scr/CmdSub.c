/*--------------------------------文*件*信*息-----------------------------
* 文件名：  CmdSub.c
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
#define __CMDSUB_C__
#include "_Include.h"


static struct sensor xdata *px_axis=&accel_x;
static struct sensor xdata *py_axis=&accel_y;



//***********************************************************************/
// *功    能：根据角度值替换角度ADC值
// *入    口：ang,角度值；adc，角度ADC值;num,数组大小;angarr，角度数组指针;
//            adcarr,角度ADC值数组指针
// *出    口：TRUE，成功;FALSE，失败
// *备    注：
// *函数性质：
//***********************************************************************/



int8u AngChkDiff(int32s ang,int16u adc,int8u num,int32s* angarr,int16u* adcarr)
{	
	int8u xdata i;
	for(i=0;i<num;i++)
	{	if(ang==angarr[i])       //查找角度再数组中的位置	
		{	
			adcarr[i] = adc;    //在相应的位置插入ADC值
			return TRUE;
		}
	}
	return FALSE;               //不在范围内
}
//***********************************************************************/
// *功    能：X轴角度标定开始
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u Adc_XCalSta (void)
{
	if(MenuId!=MOD_MOD0)return FALSE;
	MenuId=MOD_X_CAL;
	ACalXId=0;
	return TRUE;
}
//***********************************************************************/
// *功    能：X轴角度标定
// *入    口：ang,角度值;adc,角度ADC值
// *出    口：TRUE,保存成功;FALSE，失败
// *备    注：当重新标定某个角度值是，原来的AD值将会被替换
// *函数性质：
//***********************************************************************/
int8u Adc_XCalProc (int32s ang,int16u adc)
{
	if(MenuId!=MOD_X_CAL)return FALSE;
	if((ACalXId>0)&&(TRUE==AngChkDiff(ang,adc,ACalXId,px_axis->angcal->angarr,px_axis->angcal->adcarr)))
		return TRUE;    									//替换成功
	if(ACalXId>=px_axis->angcal->calcnt) return FALSE;
	px_axis->angcal->angarr[ACalXId]=ang;    				//写入新值
	px_axis->angcal->adcarr[ACalXId]=adc;
	ACalXId++;
	return TRUE;
}
//***********************************************************************/
// *功    能：X轴角度标定结束
// *入    口：
// *出    口：标定参数 A,K
// *备    注：
// *函数性质：
//***********************************************************************/

int8u Adc_XCalStop (void)
{	
	int8u n=px_axis->angcal->calcnt-1;
	
	if(ACalXId<px_axis->angcal->calcnt)  return FALSE;
	if(MenuId!=MOD_X_CAL)return FALSE;
	MenuId=MOD_MOD0;
	Sub_BufBubble(px_axis->angcal->angarr,px_axis->angcal->adcarr,px_axis->angcal->calcnt);//排序角度值和角度AD值
//	px_axis->angcal->CalcPrm();    														   //标定参数计算
	AngXCalcPrm();				   
	return TRUE;
}


//***********************************************************************/
// *功    能：X轴角度标定开始
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int8u Adc_YCalSta (void)
{
	if(MenuId!=MOD_MOD0)return FALSE;	
	MenuId=MOD_Y_CAL;
	ACalYId=0;
	return TRUE;
}
//***********************************************************************/
// *功    能：Y轴角度标定
// *入    口：ang,角度值;adc,角度ADC值
// *出    口：TRUE,保存成功;FALSE，失败
// *备    注：当重新标定某个角度值是，原来的AD值将会被替换
//***********************************************************************/
int8u Adc_YCalProc (int32s ang,int16u adc)
{
	if(MenuId!=MOD_Y_CAL)return FALSE;
	if((ACalYId>0)&&(TRUE==AngChkDiff(ang,adc,ACalYId,py_axis->angcal->angarr,py_axis->angcal->adcarr)))
		return TRUE;    					//替换成功
	if(ACalYId>=py_axis->angcal->calcnt) return FALSE;
	py_axis->angcal->angarr[ACalYId]=ang;    //写入新值
	py_axis->angcal->adcarr[ACalYId]=adc;
	ACalYId++;
	return TRUE;
}
//***********************************************************************/
int8u Adc_YCalStop (void)
{	
	int8u n=py_axis->angcal->calcnt-1;
	
	if(ACalYId<py_axis->angcal->calcnt)  return FALSE;
	if(MenuId!=MOD_Y_CAL)return FALSE;
	MenuId=MOD_MOD0;
	Sub_BufBubble(py_axis->angcal->angarr,py_axis->angcal->adcarr,py_axis->angcal->calcnt);//排序角度值和角度AD值
//	py_axis->angcal->CalcPrm();    
	AngYCalcPrm();				   
	return TRUE;
}