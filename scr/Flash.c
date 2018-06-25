/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Flash.c
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
#define   __FLASH_C__
#include  "_Include.h"
/***********************************************************************/
static void Ram_CalDataInit (void);
static void Ram_CalData2Buf (int8u);
static void Ram_Buf2CalData (int8u);
static void FlashArea_User (void) ;
static int8u Flash_CheckSum (int8u *addr,int16u n);
static int8u FlashChk (void);
static  void  Ram_CalDataInit_User(void);
static  void Flash_PageErase (int16u addr);
static  void Flash_BufWr (int16u addr,char* buf,int16u n);
static  void Flash_BufRd (int16u addr,char* buf,int16u n);
static  void FlashUpdate (int16u addr,int8u* buf,int16u n);
static  void Flash_CalDataUpdate_User (int8u page,int16u addr,int8u* buf,int16u n);

//***********************************************************************/
// *��    �ܣ�Flash calibrate Init У׼���ݳ�ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void Flash_CalDataInit (void)
{
    int8u SfrPage_ = SFRPAGE;
    Flash_BufRd(FLASH_SYS,DispBuf,FLASH_BUF_NUM);               //Reading Form FLASH_SYS ��ϵͳ����
    if(OK != FlashChk())                                        //�ж�У���� 0x85
    {
        Ram_CalDataInit();                                      //�ڴ����ݳ�ʼ��
        Flash_CalDataUpdate();                                  //���ڴ������Update��Flash
    }
    else
    {
        Ram_Buf2CalData(1);                                     //��������ϵͳ���ݱ��浽����ڴ�
        Flash_BufRd(FLASH_TEMP,DispBuf,FLASH_BUF_NUM);          //Reading Form FLASH_USER ����ʱ����
        if((OK != FlashChk())||(DispBuf[1] != Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2))))
        {
           FlashArea_User();                                    //�ж�USER����
        }
        else
        {
            Flash_BufRd(FLASH_USER,DispBuf,FLASH_BUF_NUM);      //ȡ���û����������
            if((OK != FlashChk())||(DispBuf[1] != Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2))))
            {
               Flash_BufRd(FLASH_TEMP,DispBuf,FLASH_BUF_NUM);
               Ram_Buf2CalData(2);                              //������ʱ��������ݱ��浽����ڴ�
               Flash_CalDataUpdate_User (2,FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);      
            }													 //USER�������쳣�� ��TEMP ������ UPDATE ��USER ����		
            else
            {
                Flash_BufRd(FLASH_TEMP,DispBuf,FLASH_BUF_NUM);
                Ram_Buf2CalData(2);                             //������ʱ��������ݱ��浽����ڴ�
            }
         }

      }

    ACalXId=GetAngXCalCnt();                                    //��ȡx��У׼��
    ACalYId=GetAngYCalCnt();                                    //��ȡy��У׼��  

    MenuId=MOD_X_CAL;
    Adc_XCalStop();                                              //ͨ����Flash�����ݵ���У׼�������� ���A,K

    MenuId=MOD_Y_CAL;
    Adc_YCalStop();


	MainTime4Cnt = SendSpeed[SendSpeedNum]; 					 

    SFRPAGE = CONFIG_PAGE;
    SBRL0   = -(SYSCLK / Braud[Braud_Num] / 2);             	//���ò�����   SFRPAGE=0x0F     SBRL0 = -(SYSCLK / BAUDRATE / 2);
    SFRPAGE = SfrPage_;
}
//***********************************************************************/
// *��    �ܣ�Flash Update
// *��    �ڣ�
// *��    �ڣ�
// *��    ע�� ϵͳ�û����� ��Ӧ�˲���
// *�������ʣ�
//***********************************************************************/
void Flash_CalDataUpdate (void)
{
    Ram_CalData2Buf(1);
    FlashUpdate(FLASH_SYS,&DispBuf[0],FLASH_BUF_NUM);          //��ϵͳ������Flash
    Ram_CalData2Buf(2);                                        //���û�������Flash
    FlashUpdate(FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);
    FlashUpdate(FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);
}
void Flash_UserDataUpdate(void)
{

    Flash_CalDataUpdate_User (2,FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);  
    Flash_CalDataUpdate_User (2,FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);  

}
//***********************************************************************/
void FlashUpdate (int16u addr,int8u* buf,int16u n)
{
    bit  ea_temp = EA;
    EA    = 0;

    Flash_PageErase(addr);
    delay_ms(250);
    Flash_BufWr(addr,buf,n);

    EA = ea_temp;
}

//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע�� Update Flash User/Temp
// *�������ʣ�
//***********************************************************************/
void  Flash_CalDataUpdate_User (int8u page,int16u addr,int8u* buf,int16u n)
{

    int8u sum;
    char code * data pread;                                 // FLASH read pointer
    pread = (char code *)(addr+1);

    sum = *pread;                                           // ȡ��FLASH �е�CheckSum
    Ram_CalData2Buf(page);                                  // ���������浽������

    if(sum == DispBuf[1])return;                            // �ж�ǰ���CheckSum �Ƿ����
    FlashUpdate(addr,buf,n);
}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Ram_CalDataInit (void)
{
    AngCalDataInit();          								//�Ƕ�У׼���ݳ�ʼ��
}
//************************��У׼���ݴ浽������***********************************************/
static void Ram_CalData2Buf (int8u page)
{   int8u i;
	int16u j;
    int16u k;
    if(page==1)                                               
    {
        for(i=0;i<ANG_CAL_NUM;i++)
        {

            *((int32s*)&DispBuf[i*12+2]) = GetAngXCalAngVal(i);
            *((int32s*)&DispBuf[i*12+6]) = GetAngYCalAngVal(i);
            *((int16u*)&DispBuf[i*12+10]) = GetAngXCalAdcVal(i);
            *((int16u*)&DispBuf[i*12+12]) = GetAngYCalAdcVal(i);    			 

        }
		j=ANG_CAL_NUM*12+FLASH_CK_NUM;
        for(i=0;i<SystemVerNum;i++)
        {
          *((int8u*)&DispBuf[j+i]) = SystemVer[i];
        }

        j=j+SystemVerNum;

        *((int8u*)&DispBuf[j+0])  = GetAngXCalCnt();   //�Ƕ�У׼��������   
       
        *((int16u*)&DispBuf[j+1])  = AngRange;


        *((int32s*)&DispBuf[j+3])  = GetAngXSysZero();   //X��Բ����Ƕ�(�ڲ����㣩
        *((int32s*)&DispBuf[j+7])  = GetAngYSysZero();   //y��Բ����Ƕ�(�ڲ����㣩

        k=j+11;
        for(;k<FLASH_BUF_NUM;k++)
        {
            DispBuf[k]=0;
        }                                                                       //ʣ��� ����0

        DispBuf[0] = FLASH_DATA_CHK;
        DispBuf[1] = Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2));   //����CheckSum 
    }
    else if(2==page)                                                            
    {
        *((int8u*)&DispBuf[2]) = SendActPosi;                                   //��������ORӦ����
        *((int8u*)&DispBuf[3]) = SendSpeedNum;                                  //���������ٶ�
        *((int8u*)&DispBuf[4])  = Braud_Num;         							//������
        *((int8u*)&DispBuf[5])  = GetFilterCnt();    							//�˲�ϵ��

        j = 6;

        *((int32s*)&DispBuf[j+0])  = GetAngXUsrZero() ;                         //X��Բ����Ƕ�(�û����㣩
        *((int32s*)&DispBuf[j+4]) = GetAngYUsrZero() ;                          //Y��Բ����Ƕ�(�û����㣩   

        *((int32s*)&DispBuf[j+8]) = GetAngXInst() ;                             //X��װ�Ƕ�
        *((int32s*)&DispBuf[j+12]) = GetAngYInst();                             //Y��װ�Ƕ�


		*((int8u*)&DispBuf[j+16]) = Addr;

		k=j+17+32;					 											//	Ԥ��32���ռ�׷���µı���
        for(;k<FLASH_BUF_NUM;k++)
        {
            DispBuf[k]=0;
        }


        DispBuf[0] = FLASH_DATA_CHK;
        DispBuf[1] = Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2));       
    }
}


//**************************************************************************/
//**************************�������������ݴ浽�ڴ�DispBuf to Ram ************/
//**************************************************************************/
static void Ram_Buf2CalData (int8u page)
{   int8u i;
	int16u j;
    if(1==page)
    {
        for(i=0;i<ANG_CAL_NUM;i++)                                              
        {
            SetAngXCalAngVal(*((int32s*)&DispBuf[i*12+2]),i);
            SetAngYCalAngVal(*((int32s*)&DispBuf[i*12+6]),i);
            SetAngXCalAdcVal(*((int16u*)&DispBuf[i*12+10]),i);
            SetAngYCalAdcVal(*((int16u*)&DispBuf[i*12+12]),i);                  //

        }
		j=ANG_CAL_NUM*12+FLASH_CK_NUM;
        for(i=0;i<SystemVerNum;i++)
        {
          SystemVer[i] = *((int8u*)&DispBuf[j+i]);
        }


        j=j+SystemVerNum;                                                                    

        SetAngCalCnt(*((int8u*)&DispBuf[j+0]));    //�Ƕ�У׼��������   

        AngRange = *((int16u*)&DispBuf[j+2]);		//������Χ

    }
    if(2==page)
    {
        SendActPosi         = *((int8u*)&DispBuf[2]);
        SendSpeedNum        = *((int8u*)&DispBuf[3]);
        Braud_Num 			= *((int8u*)&DispBuf[4]);      									//������
        SetFilterCnt(*((int8u*)&DispBuf[5]));    											//�˲�������������

        j = 6;

        SetAngXUsrZero(*((int32s*)&DispBuf[j+0]));
        SetAngYUsrZero(*((int32s*)&DispBuf[j+4]));

        SetAngXInst(*((int32s*)&DispBuf[j+8]));
        SetAngYInst(*((int32s*)&DispBuf[j+12]));                                             //
		Addr = *((int8u*)&DispBuf[j+0]) ;
	}
}


//************************* CHECKSUM *************************************/
static int8u Flash_CheckSum (int8u *addr,int16u n)
{   int8u * data pread;
    int16u i;
    int8u  sum=0;
    pread = addr;
    for(i=0;i<n;i++)
    {
      sum += *(pread++);
    }
    return(sum);
}

//***********************************************************************/
// *��    �ܣ�ҳ����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Flash_PageErase (int16u addr)
{   char xdata * data pwrite;               // FLASH write pointer
    bit  ea_temp = EA;                      // Preserve EA


    pwrite = (char xdata *) addr;
    EA = 0;                                 // Disable interrupts
    VDM0CN = 0xA0;                          // Enable VDD monitor and high threshold
    RSTSRC = 0x02;                          // Enable VDD monitor as a reset source
    FLKEY  = 0xA5;                          // Key Sequence 1
    FLKEY  = 0xF1;                          // Key Sequence 2
    PSCTL |= 0x03;                          // PSWE = 1; PSEE = 1
    VDM0CN = 0xA0;                          // Enable VDD monitor and high threshold
    RSTSRC = 0x02;                          // Enable VDD monitor as a reset source
    *pwrite = 0;                            // Initiate page erase
    PSCTL  &= ~0x03;                        // PSWE = 0; PSEE = 0
    EA = ea_temp;                           // Restore interrupts
}
//***********************************************************************/
// *��    �ܣ�Flash Write
// *��    �ڣ�addr - valid range is from 0x0000 to 0x7DFE for 32K devices
//            valid range is from 0x0000 to 0x3FFE for 16K devices
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Flash_BufWr (int16u addr,char* buf,int16u n)
{   char xdata * data pwrite;               // FLASH write pointer
    bit  ea_temp = EA;                      // Preserve EA
    int16u i;


    pwrite = (char xdata *) addr;
    EA = 0;                                 // Disable interrupts
    for(i=0;i<n;i++)
    {   VDM0CN = 0xA0;                      // Enable VDD monitor and high threshold
        RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
        FLKEY  = 0xA5;                      // Key Sequence 1
        FLKEY  = 0xF1;                      // Key Sequence 2
        PSCTL |= 0x01;                      // PSWE = 1 which enables writes
        VDM0CN = 0xA0;                      // Enable VDD monitor and high threshold
        RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
        *pwrite++=*buf++;
        PSCTL &= ~0x01;                     // PSWE = 0 which disable writes
    }
    EA = ea_temp;                           // Restore interrupts
}
//***********************************************************************/
// *��    �ܣ�Flash Read
// *��    �ڣ�addr - valid range is from 0x0000 to 0x7DFE for 32K devices
//            valid range is from 0x0000 to 0x3FFE for 16K devices
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Flash_BufRd (int16u addr,char* buf,int16u n)
{   char code * data pread;                 // FLASH read pointer
    bit  ea_temp = EA;                      // Preserve EA
    int16u i;

    pread = (char code *) addr;
    EA    = 0;                              // Disable interrupts
    for(i=0;i<n;i++)
    {
      *buf++ = *pread++;
    }
    EA = ea_temp;                           // Restore interrupts
}

//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static int8u FlashChk (void)
{   int8u temp;;
    temp = DispBuf[0];
    if( temp == FLASH_DATA_CHK)return OK;
    else
    return ERR;
}

//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void FlashArea_User (void)
{
    Flash_BufRd(FLASH_USER,DispBuf,FLASH_BUF_NUM);                
    if((OK != FlashChk())||(DispBuf[1] != Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2))))
    {
      Ram_CalDataInit_User();                           //USER��CHK��CheckSum ������ ���ʼ�� TEMP OR USER ���������
      Flash_CalDataUpdate_User (2,FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);
      Flash_CalDataUpdate_User (2,FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);
    }
    else
    {
       Ram_Buf2CalData(2);                              //��USER��������ݴ浽�ڴ�
       Flash_CalDataUpdate_User (2,FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);  
       													//USER��CHK��CheckSum ��û������ ��USER ������UPDATE ��TEMP ����
    }

}


//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/

static  void  Ram_CalDataInit_User(void)
{

    SetAngXUsrZero(0);
    SetAngYUsrZero(0);	
    Init_SystemPara();

}



