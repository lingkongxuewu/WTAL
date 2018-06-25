/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Calibration.h
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
#include	"_Type.h"
#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

/***********************************************************************/
/*-----------------------------�궨��----------------------------------*/

#define	ANG_CAL_NUM  25     
/*-------------------------�ṹ���������Ͷ���--------------------------*/

	struct ang_calib{
		int8u calcnt;                               //����
		int32s angarr[ANG_CAL_NUM];                 //ang��������
		int16u adcarr[ANG_CAL_NUM];                 //ADC��������
		float prm_a[ANG_CAL_NUM-1];                 //����A��������
		float prm_k[ANG_CAL_NUM-1];                 //����K��������
		void (*SetCalNum)(int8u num);               //����У׼����
		int32s (*RdAngVal)( int8u id);              //����������
		int16u (*RdAdcVal)(int8u id);               //������
		void (*WrAngVal)(int16u val, int8u id);     //д����
		void (*WrAdcVal)(int16u val, int8u id);     //д��������
//		void (*CalcPrm)(void);                      //�������A,K
	};

		

	/*-----���������ȣ�����---------*/
	
	/*-----��������-------------------*/
	int8u GetAngXCalCnt(void);                     //��ȡX��Ƕ�У׼��ĸ���
	
	int8u GetAngYCalCnt(void);                     //��ȡY��Ƕ�У׼��ĸ���
	
	void SetAngXCalCnt(int8u cnt);                 //����X��Ƕ�У׼��ĸ���
	
	void SetAngYCalCnt(int8u cnt);                 //����Y��Ƕ�У׼��ĸ���
	
	void SetAngCalCnt(int8u cnt);                  //���ýǶ�У׼��ĸ���
	
	
	int32s GetAngXCalAngVal(int8u pos);            //��ȡX��Ƕ�У׼��У׼�����е�ĳ���Ƕ�ֵ
	
	int32s GetAngYCalAngVal(int8u pos);            //��ȡY��Ƕ�У׼��У׼�����е�ĳ���Ƕ�ֵ
	
	int16u GetAngXCalAdcVal(int8u pos);            //��ȡX��Ƕ�У׼��ADC�����е�ĳ��ADֵ
	
	int16u GetAngYCalAdcVal(int8u pos);
	
	
	void SetAngXCalAngVal(int32s val, int8u pos);   //����X��Ƕ�У׼��У׼�����е�ĳ���Ƕ�ֵ
	
	void SetAngYCalAngVal(int32s val, int8u pos);
	
	void SetAngXCalAdcVal(int16u val, int8u pos);   //����X��Ƕ�У׼��ADC�����е�ĳ��ADֵ
	
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