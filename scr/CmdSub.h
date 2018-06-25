/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  CmdSub.h
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
#include "_Type.h"
#ifndef _CMDSUB_H_
#define _CMDSUB_H_

/***********************************************************************/
	/*-----�궨��---------------------*/
	#define SystemVerNum	16  

	/*-----�ṹ���������Ͷ���---------*/
	struct sensor;
	/*-----����������ȣ�����---------*/
	
	/*-----��������-------------------*/
	
	int8u Adc_XCalSta (void);
	int8u Adc_XCalProc (int32s ang,int16u adc);
	int8u Adc_XCalStop (void);
	
	int8u Adc_YCalSta (void);
	int8u Adc_YCalProc (int32s ang,int16u adc);
	int8u Adc_YCalStop (void);
	int8u AngChkDiff(int32s ang,int16u adc,int8u num,int32s* angarr,int16u* adcarr);


#endif
	/***************��������******************/
#ifdef   __CMDSUB_C__
#define  CMDSUB_EXT
#else
#define	 CMDSUB_EXT  extern
#endif
	CMDSUB_EXT	int8u  xdata	ACalXId;
	CMDSUB_EXT	int8u  xdata	ACalYId;

	CMDSUB_EXT int8u  xdata  SendActPosi;             //SendActPosi=0:��������ģʽ��=1��Ӧ��ģʽ
	CMDSUB_EXT int8u  xdata  SendSpeedNum;
	CMDSUB_EXT int8u  xdata  SystemVer[SystemVerNum]; //��Ʒ���