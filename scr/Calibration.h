/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Calibration.h
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
#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

/***********************************************************************/
/*-----------------------------宏定义----------------------------------*/

#define	ANG_CAL_NUM  25     
/*-------------------------结构体数据类型定义--------------------------*/

	struct ang_calib{
		int8u calcnt;                               //个数
		int32s angarr[ANG_CAL_NUM];                 //ang缓存数组
		int16u adcarr[ANG_CAL_NUM];                 //ADC缓存数组
		float prm_a[ANG_CAL_NUM-1];                 //参数A缓存数组
		float prm_k[ANG_CAL_NUM-1];                 //参数K缓存数组
		void (*SetCalNum)(int8u num);               //设置校准个数
		int32s (*RdAngVal)( int8u id);              //读设置数据
		int16u (*RdAdcVal)(int8u id);               //读数据
		void (*WrAngVal)(int16u val, int8u id);     //写数据
		void (*WrAdcVal)(int16u val, int8u id);     //写返回数据
//		void (*CalcPrm)(void);                      //计算参数A,K
	};

		

	/*-----常量（表格等）声明---------*/
	
	/*-----函数声明-------------------*/
	int8u GetAngXCalCnt(void);                     //获取X轴角度校准点的个数
	
	int8u GetAngYCalCnt(void);                     //获取Y轴角度校准点的个数
	
	void SetAngXCalCnt(int8u cnt);                 //设置X轴角度校准点的个数
	
	void SetAngYCalCnt(int8u cnt);                 //设置Y轴角度校准点的个数
	
	void SetAngCalCnt(int8u cnt);                  //设置角度校准点的个数
	
	
	int32s GetAngXCalAngVal(int8u pos);            //获取X轴角度校准中校准数组中的某个角度值
	
	int32s GetAngYCalAngVal(int8u pos);            //获取Y轴角度校准中校准数组中的某个角度值
	
	int16u GetAngXCalAdcVal(int8u pos);            //获取X轴角度校准中ADC数组中的某个AD值
	
	int16u GetAngYCalAdcVal(int8u pos);
	
	
	void SetAngXCalAngVal(int32s val, int8u pos);   //设置X轴角度校准中校准数组中的某个角度值
	
	void SetAngYCalAngVal(int32s val, int8u pos);
	
	void SetAngXCalAdcVal(int16u val, int8u pos);   //设置X轴角度校准中ADC数组中的某个AD值
	
	void SetAngYCalAdcVal(int16u val, int8u pos);

	void AngCalDataInit();

	void AngXCalcPrm(void);
	void AngYCalcPrm(void);

	void	CalibInit(void);
	float GetAngXaVal(int8u pos);
#endif

#ifdef   __CALIBR_C__
#define  CALIBR_EXT
#else
#define	 CALIBR_EXT  extern
#endif
CALIBR_EXT struct ang_calib  xdata Ang_Xcal;
CALIBR_EXT struct ang_calib  xdata Ang_Ycal;	