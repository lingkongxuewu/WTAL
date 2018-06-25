/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Calibration.c
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
#define __CALIBR_C__
#include "_Include.h"
//struct temp_calib xdata Temp_Xcal;    //X���¶Ȳ���
//struct temp_calib xdata Temp_Ycal;    //Y���¶Ȳ���

//struct ang_calib xdata Ang_Xcal;    //X��Ƕ�У׼
//struct ang_calib xdata Ang_Ycal;    //Y��Ƕ�У׼

//static int16u xdata temp_buf[TEMP_CAL_NUM];    //21

static float    Ang_RdXAVal(struct ang_calib *p_cal, int8u posid);



//***********************************************************************/
// *��    �ܣ����ýǶȱ궨�����
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);num,����
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Ang_SetCalCnt(struct ang_calib *p_cal, int8u num)
{
    p_cal->calcnt=num;
}
//***********************************************************************/
// *��    �ܣ���ȡ�Ƕȱ궨ģ���еĽǶ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);posid,λ��ID��
// *��    �ڣ��Ƕ�ֵ
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static int32s   Ang_RdAngVal(struct ang_calib *p_cal, int8u posid)
{
    return (p_cal->angarr[posid]);
}

//***********************************************************************/
// *��    �ܣ���ȡ�Ƕȱ궨ģ���еĽǶ�ADֵ�����е�ĳ��Ԫ��
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);posid,λ��ID��
// *��    �ڣ��Ƕ�ADCֵ
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static int16u   Ang_RdAdcVal(struct ang_calib *p_cal, int8u posid)
{
    return (p_cal->adcarr[posid]);
}
//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���еĽǶ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);val,�Ƕ�ֵ;posid,λ��ID��
// *��    �ڣ�
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static void Ang_WrAngVal(struct ang_calib *p_cal,int32s val, int8u posid)
{
    p_cal->angarr[posid]=val;
}

//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���еĽǶ�ADֵ�����е�ĳ��Ԫ��
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);val,�Ƕ�ADCֵ;posid,λ��ID��
// *��    �ڣ�
// *��    ע����֤λ��ID�������С�ķ�Χ��
// *�������ʣ�
//***********************************************************************/
static void Ang_WrAdcVal(struct ang_calib *p_cal,int16u val, int8u posid)
{
    p_cal->adcarr[posid]=val;
}

//***********************************************************************/
// *��    �ܣ��Ƕ�A,K��������
// *��    �ڣ�p_cal,ang_calib�ṹ��ָ��(X,Y);
// *��    �ڣ�
// *��    ע��K=(sin(ang1)-sin(ang0))/(adc1-adc0);A=sin(ang0)-K*adc0;
// *�������ʣ�
//***********************************************************************/
static void Ang_CalcPrm(struct ang_calib *p_cal)
{
    float ang0;
    float ang1;

    int8u i;
    for(i=0;i<(p_cal->calcnt-1);i++)
    {
        ang0=(PI*(p_cal->angarr[i]))/180000.0;    //�ǶȻ���ɻ���
        ang1=(PI*(p_cal->angarr[i+1]))/180000.0;
        p_cal->prm_k[i] = (sin(ang1)-sin(ang0))/(int16s)(p_cal->adcarr[i+1]-p_cal->adcarr[i]);
        p_cal->prm_a[i] = sin(ang0)- p_cal->prm_k[i] * (p_cal->adcarr[i]);
    }
}



int8u GetAngXCalCnt(void)
{
    return (Ang_Xcal.calcnt);    //��ȡX��Ƕ�У׼�����
}
/*
int8u GetTempYCalCnt(void)
{
    return (Temp_Ycal.calcnt);    //��ȡY���¶Ȳ�������� (û���õ�ע��������
}
*/
int8u GetAngYCalCnt(void)
{
    return (Ang_Ycal.calcnt);    //��ȡY��Ƕ�У׼�����
}


//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/

void SetAngXCalCnt(int8u cnt)
{
     Ang_SetCalCnt(&Ang_Xcal,cnt);     //����X��Ƕ�У׼�����
}



void SetAngYCalCnt(int8u cnt)
{
     Ang_SetCalCnt(&Ang_Ycal,cnt);    //����Y��Ƕ�У׼�����
}



void SetAngCalCnt(int8u cnt)         //���ýǶ�У׼�����
{
     SetAngXCalCnt(cnt);
     SetAngYCalCnt(cnt);
}




//***********************************************************************/
// *��    �ܣ���ȡ�Ƕȱ궨ģ���е�X��Ƕ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�ֵ
// *��    ע����֤λ��ID�������С�ķ�Χ��
//***********************************************************************/
int32s GetAngXCalAngVal(int8u pos)
{
    return (Ang_RdAngVal(&Ang_Xcal,pos));
}

//***********************************************************************/
// *��    �ܣ���ȡ�Ƕȱ궨ģ���е�Y��Ƕ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�ֵ
// *��    ע����֤λ��ID�������С�ķ�Χ��
//***********************************************************************/
int32s GetAngYCalAngVal(int8u pos)
{
    return (Ang_RdAngVal(&Ang_Ycal,pos));
}


//***********************************************************************/
// *��    �ܣ����Ƕȱ궨ģ����X��ADC�����ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�ADCֵ
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
int16u GetAngXCalAdcVal(int8u pos)
{
    return (Ang_RdAdcVal(&Ang_Xcal,pos));
}
//***********************************************************************/

//***********************************************************************/
// *��    �ܣ����Ƕȱ궨ģ����Y��ADC�����ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�ADCֵ
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
int16u GetAngYCalAdcVal(int8u pos)
{
    return (Ang_RdAdcVal(&Ang_Ycal,pos));
}






//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���е�X��Ƕ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�val,�Ƕ�ֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��ID�������С�ķ�Χ��
//***********************************************************************/
void SetAngXCalAngVal(int32s val, int8u pos)
{
    Ang_WrAngVal(&Ang_Xcal,val,pos);
}

//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���е�Y��Ƕ�ֵ�����е�ĳ��Ԫ��
// *��    �ڣ�val,�Ƕ�ֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��ID�������С�ķ�Χ��
//***********************************************************************/
void SetAngYCalAngVal(int32s val, int8u pos)
{
    Ang_WrAngVal(&Ang_Ycal,val,pos);
}


//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���е�X��Ƕ�ADֵ�����е�ĳ��Ԫ��
// *��    �ڣ�val,�Ƕ�ADCֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
void SetAngXCalAdcVal(int16u val, int8u pos)
{
    Ang_WrAdcVal(&Ang_Xcal,val,pos);
}

//***********************************************************************/
// *��    �ܣ����ýǶȱ궨ģ���е�Y��Ƕ�ADֵ�����е�ĳ��Ԫ��
// *��    �ڣ�val,�Ƕ�ADCֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
void SetAngYCalAdcVal(int16u val, int8u pos)
{
    Ang_WrAdcVal(&Ang_Ycal,val,pos);
}




//***********************************************************************/
// *��    �ܣ�X��Ƕȱ궨��������
// *��    ע��
//***********************************************************************/
void    AngXCalcPrm(void)
{
    Ang_CalcPrm(&Ang_Xcal);
}
//***********************************************************************/
// *��    �ܣ�Y��Ƕȱ궨��������
// *��    ע��
//***********************************************************************/
void    AngYCalcPrm(void)
{
    Ang_CalcPrm(&Ang_Ycal);
}



//***********************************************************************/
// *��    �ܣ��Ƕȱ궨Ĭ�����ݳ�ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��Flash ��ʼ����ʱ�����
// *�������ʣ�
//***********************************************************************/
void AngCalDataInit()
{
    int8u i;
    SetAngCalCnt(3);          //�Ƕ�У׼�����
    for(i=0;i<ANG_CAL_NUM;i++)//�Ƕ�
    {
        Ang_Xcal.angarr[i] = 2000*(i-ANG_CAL_NUM/2);
        Ang_Xcal.adcarr[i] = 500*(i-ANG_CAL_NUM/2)+8000;//2011-11-07 �޸�У׼�Ƕ�Ϊ��λС��

        Ang_Ycal.angarr[i] = 2000*(i-ANG_CAL_NUM/2);
        Ang_Ycal.adcarr[i] = 500*(i-ANG_CAL_NUM/2)+8000;
    }
    //----------------test-------------------
    Ang_Xcal.angarr[0]  =   -30000  ;
    Ang_Xcal.adcarr[0]  =   764     ;//2011-11-07 �޸�У׼�Ƕ�Ϊ��λС��
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
// *��    �ܣ�У׼ģ���ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ���ʼ������ָ��
//***********************************************************************/
void    CalibInit(void)
{
//    Temp_Xcal.SetCalNum=SetTempXCalCnt;     //����У׼����
//    Temp_Xcal.RdTempVal=GetTempXCalTempVal;  //����������
//    Temp_Xcal.RdAdcVal=GetTempXCalAdcVal;    //������
//    Temp_Xcal.WrTempVal=SetTempXCalTempVal;   //д����
//    Temp_Xcal.WrAdcVal=SetTempXCalAdcVal;     //д��������
////  Temp_Xcal.CalcPrm=TempXCalcPrm;           //�������A,K
//
//    Temp_Ycal.ptempval=temp_buf;            //�¶Ȳ���ģ�����¶�ADֵ��������
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
