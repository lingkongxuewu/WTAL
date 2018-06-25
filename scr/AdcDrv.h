/*--------------------------------文*件*信*息-----------------------------
* 文件名：  AdcDrv.h
* 版　本：  v1.0
* 描  述：  ADC底层驱动模块	100T双轴 整合后的版本
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
	#include "_Type.h"
	#ifndef __ADC_DRV_H__
	#define __ADC_DRV_H__
	#include <compiler_defs.h>
	#include <c8051F500_defs.h>


	/*-----宏定义---------------------*/
	#define	SUM_CNT			254
	#define	SUM_CNT1		16
	#define FILTER_NUM 		64
	#define FILTER_SUBB		300
	
	/*-----结构体数据类型定义---------*/
	struct adc_entity{
		int8u num;					  //ADC端口号
		int8u arrpt;				  //缓存数组指针
		int8u arrcnt;				  //存入数组元素个数
		int16u valarr[FILTER_NUM];	  //缓存数组
		int16u ftval;	  			  //滤波后的值
		struct adc_entity *next;	  //ADC链表的下一个元素
//		void (*Filter)(int16u adc_val);	//滤波函数指针
	};
	

	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
	void Adc_Init (void);
	void Adc_Enable (void);	
	int16u Adc_GetXVal(void);
	int16u Adc_GetYVal(void);
	int16u Adc_GetTemp(void);
//	int16u Adc_GetVdd(void);

#endif
/***********************************************************************/
#ifdef   __ADC_DRV_C__
#define  ADC_DRV_EXT
#else
#define  ADC_DRV_EXT  extern
#endif
extern int8u idata	AdcEndTag;
extern int8u idata	AdcEndId;	
