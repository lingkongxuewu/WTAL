/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  CmdSub.c
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
#define __CMDSUB_C__
#include "_Include.h"


static struct sensor xdata *px_axis=&accel_x;
static struct sensor xdata *py_axis=&accel_y;



//***********************************************************************/
// *��    �ܣ����ݽǶ�ֵ�滻�Ƕ�ADCֵ
// *��    �ڣ�ang,�Ƕ�ֵ��adc���Ƕ�ADCֵ;num,�����С;angarr���Ƕ�����ָ��;
//            adcarr,�Ƕ�ADCֵ����ָ��
// *��    �ڣ�TRUE���ɹ�;FALSE��ʧ��
// *��    ע��
// *�������ʣ�
//***********************************************************************/



int8u AngChkDiff(int32s ang,int16u adc,int8u num,int32s* angarr,int16u* adcarr)
{	
	int8u xdata i;
	for(i=0;i<num;i++)
	{	if(ang==angarr[i])       //���ҽǶ��������е�λ��	
		{	
			adcarr[i] = adc;    //����Ӧ��λ�ò���ADCֵ
			return TRUE;
		}
	}
	return FALSE;               //���ڷ�Χ��
}
//***********************************************************************/
// *��    �ܣ�X��Ƕȱ궨��ʼ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u Adc_XCalSta (void)
{
	if(MenuId!=MOD_MOD0)return FALSE;
	MenuId=MOD_X_CAL;
	ACalXId=0;
	return TRUE;
}
//***********************************************************************/
// *��    �ܣ�X��Ƕȱ궨
// *��    �ڣ�ang,�Ƕ�ֵ;adc,�Ƕ�ADCֵ
// *��    �ڣ�TRUE,����ɹ�;FALSE��ʧ��
// *��    ע�������±궨ĳ���Ƕ�ֵ�ǣ�ԭ����ADֵ���ᱻ�滻
// *�������ʣ�
//***********************************************************************/
int8u Adc_XCalProc (int32s ang,int16u adc)
{
	if(MenuId!=MOD_X_CAL)return FALSE;
	if((ACalXId>0)&&(TRUE==AngChkDiff(ang,adc,ACalXId,px_axis->angcal->angarr,px_axis->angcal->adcarr)))
		return TRUE;    									//�滻�ɹ�
	if(ACalXId>=px_axis->angcal->calcnt) return FALSE;
	px_axis->angcal->angarr[ACalXId]=ang;    				//д����ֵ
	px_axis->angcal->adcarr[ACalXId]=adc;
	ACalXId++;
	return TRUE;
}
//***********************************************************************/
// *��    �ܣ�X��Ƕȱ궨����
// *��    �ڣ�
// *��    �ڣ��궨���� A,K
// *��    ע��
// *�������ʣ�
//***********************************************************************/

int8u Adc_XCalStop (void)
{	
	int8u n=px_axis->angcal->calcnt-1;
	
	if(ACalXId<px_axis->angcal->calcnt)  return FALSE;
	if(MenuId!=MOD_X_CAL)return FALSE;
	MenuId=MOD_MOD0;
	Sub_BufBubble(px_axis->angcal->angarr,px_axis->angcal->adcarr,px_axis->angcal->calcnt);//����Ƕ�ֵ�ͽǶ�ADֵ
//	px_axis->angcal->CalcPrm();    														   //�궨��������
	AngXCalcPrm();				   
	return TRUE;
}


//***********************************************************************/
// *��    �ܣ�X��Ƕȱ궨��ʼ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u Adc_YCalSta (void)
{
	if(MenuId!=MOD_MOD0)return FALSE;	
	MenuId=MOD_Y_CAL;
	ACalYId=0;
	return TRUE;
}
//***********************************************************************/
// *��    �ܣ�Y��Ƕȱ궨
// *��    �ڣ�ang,�Ƕ�ֵ;adc,�Ƕ�ADCֵ
// *��    �ڣ�TRUE,����ɹ�;FALSE��ʧ��
// *��    ע�������±궨ĳ���Ƕ�ֵ�ǣ�ԭ����ADֵ���ᱻ�滻
//***********************************************************************/
int8u Adc_YCalProc (int32s ang,int16u adc)
{
	if(MenuId!=MOD_Y_CAL)return FALSE;
	if((ACalYId>0)&&(TRUE==AngChkDiff(ang,adc,ACalYId,py_axis->angcal->angarr,py_axis->angcal->adcarr)))
		return TRUE;    					//�滻�ɹ�
	if(ACalYId>=py_axis->angcal->calcnt) return FALSE;
	py_axis->angcal->angarr[ACalYId]=ang;    //д����ֵ
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
	Sub_BufBubble(py_axis->angcal->angarr,py_axis->angcal->adcarr,py_axis->angcal->calcnt);//����Ƕ�ֵ�ͽǶ�ADֵ
//	py_axis->angcal->CalcPrm();    
	AngYCalcPrm();				   
	return TRUE;
}