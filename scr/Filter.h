/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Filter.h
* �桡����  v1.0
* ��  ����  �˲����� 100T˫�� ���Ϻ�İ汾
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
//***********************************************************************/
#ifndef __FILTER_H__
#define __FILTER_H__
	#include	"_Type.h"

	/*-----�궨��---------------------*/

	/*-----�ṹ���������Ͷ���---------*/


	/*-----����������ȣ�����---------*/
	/*-----��������-------------------*/
	int8u ValInRange(int16u ref_val,int16u new_val,int16u range);
	int16u SlideWinFilter (int8u *pbufpos, int8u *pbufcnt, int16u *pbuf, int16u val, int16u ftcnt);
	float Filter_1st (float ref_val,int16u new_val, int8u filtercnt);		   
	
#endif