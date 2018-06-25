/*--------------------------------文*件*信*息-----------------------------
* 文件名：  AdcDrv.c
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
#define  __ADC_DRV_C__
#include "_Include.h"

/***********************************************/
struct adc_entity xdata x_axis_adc;
struct adc_entity xdata y_axis_adc;
struct adc_entity xdata temp_adc;
//struct adc_entity xdata vdd_adc;
/*****************************************************/
static int8u    code    PinIdTab[13] = {   2,   0,   1,   0,   1,   0,   1,   0,   1,   0,   1,   0,   1};
static int8u    code    PinMuxTab[]  = {0x0b,0x09,0x0a,0x09,0x0a,0x09,0x0a,0x09,0x0a,0x09,0x0a,0x09,0x0a};
                        //AD输入通道选择0x0a=P1.2,0x09=P1.1,0x30=temp,0x0b=Vdd
/*****************************************************/
int8u   idata   AdcSwitchId=0;
int8u   idata   AdcEndTag=0;
int8u   idata   AdcEndId;

static void Adc_SetInit (void);
static void Adc_VrefInit (void);
static void Adc_IsrInit (void);
static void AdcEntityInit(void);
//***********************************************************************/
// *功    能：ADC初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Adc_Init (void)
{
    Adc_SetInit();
    Adc_VrefInit();
    Adc_IsrInit();
    AdcEntityInit();    //ADC实体初始化
}
//***********************************************************************/
void Adc_Enable (void)
{
    ADC0CN |= 0x10;     // enable ADC0      AD0EN = 1;
}
//***********************************************************************/
// *功    能：获取X轴ADC值
// *入    口：
// *出    口：X轴ADC值（窗口滤波后）
// *备    注：
// *函数性质：
//***********************************************************************/
int16u Adc_GetXVal(void)
{
    return  (x_axis_adc.ftval);
}
//***********************************************************************/
// *功    能：获取Y轴ADC值
// *入    口：
// *出    口：Y轴ADC值（窗口滤波后）
// *备    注：
// *函数性质：
//***********************************************************************/
int16u Adc_GetYVal(void)
{
    return  (y_axis_adc.ftval);
}
//***********************************************************************/
// *功    能：获取温度ADC值
// *入    口：
// *出    口：温度ADC值（窗口滤波后）
// *备    注：
// *函数性质：
//***********************************************************************/
int16u Adc_GetTemp(void)
{
    return  (temp_adc.ftval);
}
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
/*
int16u Adc_GetVdd(void)
{
    return  (vdd_adc.ftval);
}
*/
//***********************************************************************/
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Adc_SetInit (void)
{   int8u SfrPage_ = SFRPAGE;


    SFRPAGE = ACTIVE_PAGE;
    ADC0MX  = 0x0c;     //0x0c
    ADC0CF |= 0x01;     // Set GAINEN = 1
    ADC0H   = 0x04;     // Load the ADC0GNH address
    ADC0L   = 0xFC;     // Load the upper byte of 0x6CA to
    ADC0H   = 0x07;     // Load the ADC0GNL address
    ADC0L   = 0x00;     // Load the lower nibble of 0x6CA to
    ADC0H   = 0x08;     // Load the ADC0GNA address
    ADC0L   = 0x01;     // Set the GAINADD bit
    ADC0CF &= ~0x01;    // Set GAINEN = 0
    ADC0CN  = 0x80;     // ADC0 disabled, normal tracking,
    REF0CN  = 0x00;     // Enable on-chip VREF and buffer
    ADC0MX  = 0x09;     // Set ADC input to P1.2
    ADC0CF  = ((SYSCLK / 3000000) - 1) << 3;    // Set SAR clock to 3MHz
    SFRPAGE = SfrPage_;
}
//***********************************************************************/
static void Adc_VrefInit (void)
{
    REF0CN    = 0x37;       // Enable on-chip VREF = 2.2v and buffer    ok
}
//***********************************************************************/
static void Adc_IsrInit (void)
{
    EIE1 |= 0x04;           // Enable ADC0 conversion complete int.
}

//***********************************************************************/
// *功    能：ADC值滤波
// *入    口：p_adc,ADC结构体指针（X,Y,Temp);val,新值
// *出    口：
// *备    注：为了提高响应速度，进行限幅滤波   ADC值底层滤波供X/Y/T的函数调用
// *函数性质：
//***********************************************************************/
static void AdcFilter (struct adc_entity *p_adc, int16u val)
{
    if(ValInRange(p_adc->ftval,val,(int16u)FILTER_NUM))                     //限幅滤波
    {
        p_adc->ftval=SlideWinFilter (&(p_adc->arrpt),&(p_adc->arrcnt),
                                     p_adc->valarr,val,(int16u)FILTER_NUM); //窗口滤波
    }
    else
    {
            p_adc->ftval=val;                                               //幅度变化大，直接使用当前值
            p_adc->arrcnt=0;                                                //重新写数组
    }
}

//***********************************************************************/
// *功    能：ADC实体初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void AdcEntityInit(void)
{
    //---------X轴
    x_axis_adc.num=0;                 //ADC虚拟端口号
    x_axis_adc.arrpt=0;               //ADC缓存数组位置指针
    x_axis_adc.arrcnt=0;              //元素个数为0
    x_axis_adc.ftval=0;               //ADC滤波之后的值
//  x_axis_adc.Filter=XAdcFilter;     //滤波函数指针初始化
    x_axis_adc.next=&y_axis_adc;      //ADC结构体链表初始化
    //---------Y轴
    y_axis_adc.num=1;
    y_axis_adc.arrpt=0;
    y_axis_adc.arrcnt=0;              //元素个数为0
    y_axis_adc.ftval=0;
//  y_axis_adc.Filter=YAdcFilter;
    y_axis_adc.next=&temp_adc;
    //------------温度
    temp_adc.num=2;
    temp_adc.arrpt=0;
    temp_adc.arrcnt=0;                //元素个数为0
    temp_adc.ftval=0;
//  temp_adc.Filter=TAdcFilter;
    temp_adc.next=(struct adc_entity *)0;  //链表结束
/*
    vdd_adc.num=3;
    vdd_adc.arrpt=0;
    vdd_adc.ftval=0;
    vdd_adc.Filter=VAdcFilter;
    vdd_adc.next=(struct adc_entity *)0;
*/
}
/*
void Adc_Init(struct adc_entity *adc_ent, int8u adc_num)
{
    adc_ent->num=adc_num;
    adc_ent->arrpt=0;
    adc_ent->ftval=0;
}
*/
//***********************************************************************/
// *功    能：ADC值的保存和采集端口选择
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Adc_SaveAndSwitch (int16u val)
{
    struct adc_entity *adc_ent;
    adc_ent=&x_axis_adc;                            //指向ADC结构体链表的起始元素
   if(PinIdTab[AdcSwitchId]==0)
	  val=65536-val;								//2013520xinglong  修正轴向
	
    while(adc_ent!=(struct adc_entity *)0)          //搜索链表
    {
        if(adc_ent->num==PinIdTab[AdcSwitchId])     //比较端口号
            break;                                  //相等
        adc_ent=adc_ent->next;                      //不相等，比较下一个元素
    }
    if(adc_ent!=(struct adc_entity *)0)             //搜索的元素在链表中
//      adc_ent->Filter(val);                       //滤波
        AdcFilter(adc_ent,val);

    AdcEndId=PinIdTab[AdcSwitchId];                 //保存端口号
    AdcSwitchId++;                                  //取表中下一端口号

    if(13==AdcSwitchId)AdcSwitchId=0;
    ADC0MX=PinMuxTab[AdcSwitchId];
    AdcEndTag=1;
}
//***********************************************************************/
// *功    能：AD中断处理
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Adc_Isr (void) interrupt INTERRUPT_ADC0_EOC//9
{
    static xdata int16u sum = 0;
    static xdata int8u cnt = 0;

    AD0INT=0;                                    // clear ADC0 conv. complete flag
    sum+=ADC0;
    if(++cnt==SUM_CNT)
    {
        Adc_SaveAndSwitch(sum/SUM_CNT1);
        sum=0;
        cnt=0;
    }
    else
        ADC0CN |= 0x10;
}
