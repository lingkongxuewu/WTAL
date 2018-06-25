/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  ModBusRtu.c
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
#define   __MODBUSRTU_C__
#include  "_Include.h"
//***********************************************************************/
#if MODBUS_CHECK_ENB==1
static void ModRtu_RlyError (int8u* buf,int8u excode);
static void ModRtu_WrReg (int8u* buf,int8u n);
static void ModRtu_RdReg (int8u* buf);
static int8u xdata udflashtag=0;

//***********************************************************************/
static  int16u  FloatToInt( float ang, int16u highlow )
{
    int32u  temp ;
    int16u  vol ;

    ang /= 1000.0 ;
    temp = *( int32u* )&ang ;

    if( highlow == 'L' )
    {
        vol  = ( ( temp << 24 ) >> 24 ) << 8 ;
        vol += ( ( temp << 16 ) >> 24 ) << 0 ;
    }
    else
    {
        vol  = ( ( temp <<  8 ) >> 24 ) << 8 ;
        vol += ( ( temp <<  0 ) >> 24 ) << 0 ;
    }
    return( vol ) ;
}
//***********************************************************************/
// *��    �ܣ�����У����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u ModRtu_ChkCrc (int8u *buf,int8u len)
{
    if(len<3)
    {   
		return(FALSE);
    }
    if( ModbusCrcCnt(buf, len) == 0 )		//���ݺ�CRC��֤������CRC�����Ϊ0 
    {   
		return(TRUE);
    }
    else
    {   
		return(FALSE);
    }
}
//***********************************************************************/
// *��    �ܣ��洢У����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void ModRtu_WrCrc (int8u* buf,int16u crc)
{
    buf[0] = crc>>8;				//HIGH 8BIT 
    buf[1] = crc;					//LOW 8BIT
}
//***********************************************************************/
// *��    �ܣ�����ת��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int16u  ModRtu_8to16 (int8u* buf)
{
    int16u temp;

    temp = buf[0];
    temp <<= 8;
    temp |= buf[1];
    return(temp);			 //�û��ӿڲ����Ǵ��ģʽ
}
//***********************************************************************/
// *��    �ܣ��������
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void ModRtu_SetZero (int8u* buf)
{   int16u temp;
    temp = ModRtu_8to16(buf);

    if(1 == temp)
    {
        SetAngXUsrZero(GetAngXRelSys());   //  ����X���û��������(��ȡX����ԽǶ�ֵ�ڲ�)
        SetAngYUsrZero(GetAngYRelSys());    
    }
    else if(0 == temp)
    {
        SetAngXUsrZero(0);
        SetAngYUsrZero(0);                      
    }
}
//***********************************************************************/
// *��    �ܣ��޸ļĴ�����ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ����ģʽ�� ָ���ݵĸ�λ���������ڴ�ĵ͵�ַ
//***********************************************************************/
int32s ModRtu_Wr16u (int8u* buf)//int32s* dat)
{
    int16s temp;
    int32s dat;                

    temp = buf[0];
    temp <<= 8;
    temp |= buf[1];
    dat = (int32s)temp*10;
    return dat;               
}
//***********************************************************************/
// *��    �ܣ��޸ı�����ַ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u ModRtu_WrAddr (int8u* buf)
{
    int16u temp;

    temp = buf[0];
    temp <<= 8;
    temp |= buf[1];
    if((temp < 1) || (temp > 247)) return (FALSE);
    Addr = temp;
    return(TRUE);
}
//***********************************************************************/
// *��    �ܣ��޸Ĳ�����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u ModRtu_WrBaud (int8u* buf)
{
    int16u temp;

    temp = buf[0];
    temp <<= 8;
    temp |= buf[1];
    if((temp < 0) || (temp > 8)) return (FALSE);//
    Braud_Num = temp;
    return(TRUE);
}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
//int8u ModRtu_WrVal (int16u addr, int8u* buf)
int8u ModRtu_WrVal (int16u addr, int8u* buf,int8u* excode)
{
    static  xdata int32u  vol32u ;
    static  xdata int8u  vol8u ;
    float   xdata volf ;
    int16u  xdata pc_data ;
    int16s  xdata vol16s ;

    pc_data = buf[1] * 256 + buf[0] ;  
    switch(addr)
    {
        case ANGX_OFFSET_ADDR:      SetAngXInst(ModRtu_Wr16u(buf));             //д��X�ᰲװ�Ƕ�
								    udflashtag = 1;
                                    break;
        case ANGY_OFFSET_ADDR:      SetAngYInst(ModRtu_Wr16u(buf));             //д��Y�ᰲװ�Ƕ�
									udflashtag = 1;
                                    break;        //
        case ZERO_ADDR:             ModRtu_SetZero(buf);                        //�������ֵ
									udflashtag = 1;
                                    break;
        case SELF_ADDR:             
									udflashtag = 1;
									return (ModRtu_WrAddr(buf));				//�޸��ն˻���ַ
        case BAUD_ADDR:             
									udflashtag = 1;
									return (ModRtu_WrBaud(buf));				//�޸Ĳ�����
 //***********************************************
{
    {
    
        case 0x011F:
        {
            vol32u += pc_data * 65536 ;
            if( vol32u == 0xAC35AC35 )
            {
                SetAngXUsrZero(GetAngXRelSys());   
            }
            else if( vol32u == 0x03050305 )
            {
                SetAngXUsrZero(0);
            }
        }break ;
        case 0x0121:
        {
            vol32u += pc_data * 65536 ;
            if( vol32u == 0xAC35AC35 )
            {
                SetAngYUsrZero(GetAngYRelSys());   
            }
            else if( vol32u == 0x03050305 )
            {
                SetAngYUsrZero(0);
            }
        }break ;
        case 0x0123:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngXInst(volf * 1000);
        }break ;
        case 0x0125:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngYInst(volf * 1000);

        }break ;
        case 0x0128:SetFilterCnt(buf[1]); break ;
        case 0x0129:Addr = buf[1] ; break ;
		case 0x012B:
        {	
            if( buf[1] == 0xAC )
            {
                Flash_UserDataUpdate();
            }
        }break ;

        //***********************************************************
        case 0xA800:
        case 0xA801:
        case 0xA802:
        case 0xA803:
        case 0xA804:
        case 0xA805:
        case 0xA806:
        case 0xA807:
        {
            SystemVer[(addr-0xA800)*2+0] = buf[0] ;
            SystemVer[(addr-0xA800)*2+1] = buf[1] ;
        }break ;
        case 0xA808:
        {
            vol8u = buf[0] ; // 'X' 'Y'
            pc_data = buf[1] ;
            AngRange = pc_data ;                         //������Χ
            switch( pc_data )
            {
                case   5: SetAngCalCnt(3) ;  break ;
                case  10: SetAngCalCnt(5) ;  break ;
                case  15: SetAngCalCnt(7) ;  break ;
                case  30: SetAngCalCnt(7) ;  break ;
                case  45: SetAngCalCnt(7) ;  break ;
                case  60: SetAngCalCnt(9) ;  break ;
                case  90: SetAngCalCnt(13);  break ;
 //               default : SetAngCalCnt(3) ;  break ;
				default : *excode=3;  return(FALSE);
            }
        }break ;
        //***********************************************
        case 0xA80A:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            if( volf > 0 )
            {
                vol16s = (int16s)volf + 2 ;
            }
            else
            {
                vol16s = (int16s)volf - 2 ;
            }
            if( AngRange < 30 )
            {
                vol16s = ( int16s )( vol16s / 5 ) ;
                pc_data = vol16s + (GetAngXCalCnt() / 2) ;   //�@ȡ�Ƕ�У���c����  
            }
            else if( AngRange == 30 )
            {
                vol16s = ( int16s )( vol16s / 5 ) * 5 ;
                switch( vol16s )
                {
                    case -30 : pc_data = 0 ; break ;
                    case -15 : pc_data = 1 ; break ;
                    case  -5 : pc_data = 2 ; break ;
                    case   0 : pc_data = 3 ; break ;
                    case   5 : pc_data = 4 ; break ;
                    case  15 : pc_data = 5 ; break ;
                    case  30 : pc_data = 6 ; break ;
					default	 : *excode=3;  return(FALSE);
                }
            }
            else 
            {
                vol16s = ( int16s )( vol16s / 15 ) ;
                pc_data = vol16s + (GetAngXCalCnt() / 2) ;   // �@ȡ�Ƕ�У���c����  

            }
            if( vol8u == 'X' )
            {
              SetAngXCalAngVal(volf * 1000,pc_data);
              SetAngXCalAdcVal(GetXFtVal(),pc_data);              
            }
            else
            {
                SetAngYCalAngVal(volf * 1000,pc_data );           
                SetAngYCalAdcVal(GetYFtVal(),pc_data);            

            }
        }break ;
        case 0xA80B:
        {
            pc_data = buf[0] * 256 + buf[1] ;
            if( pc_data == 0x0305 )
            {
                Flash_CalDataUpdate() ;
            }
        }break ;
        case 0xA80E:
        {
            pc_data = buf[0] * 256 + buf[1] ;
            switch( pc_data )
            {
                case 0xA501:SetAngXSysZero(GetXAng());
                            SetAngYSysZero(GetYAng());
                            break ;                       
                case 0xA502:SetAngXSysZero(0);
                            SetAngYSysZero(0);
                            break ;                       
                case 0xA503:SetAngXUsrZero(GetAngXRelSys());
                            SetAngYUsrZero(GetAngYRelSys());
                            break ;
                case 0xA504:SetAngXUsrZero(0) ;
                            SetAngYUsrZero(0) ;
                            break ;
            }
        }break ;
        case 0xA810:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngXInst(volf * 1000);
        }break ;
        case 0xA812:
        {
            vol32u += pc_data * 65536 ;
            volf = *(float*)&vol32u ;
            SetAngYInst(volf * 1000);         
        }break ;
        case 0xA81B:
        {
            pc_data = buf[0] * 256 + buf[1] ;
            SendSpeedNum = pc_data ;
        }break ;
        case 0xA81C:
        {
            pc_data = buf[0] * 256 + buf[1] ;
            SetFilterCnt(pc_data);
        }break ;        
        
    }
}
 //       default:return(FALSE);
        default:	*excode=02;
					return(FALSE);
    }
    return(TRUE);
}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void ModRtu_Rd16u (int8u* buf,int16s dat)
{
    buf[0] = MR_READ_REGH(dat);
    buf[1] = MR_READ_REGL(dat);
}
//***********************************************************************/
// *��    �ܣ����Ĵ���
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u ModRtu_RdVal (int16u addr,int8u* buf,int8u* excode)
{

    int16u  xdata pc_data ;
    int16s  xdata vol16s ;
    switch(addr)
    {

        case ANGX_ADDR:         vol16s = (GetXAngAllSetVal()/10) ; pc_data = *(int16u*)&vol16s ; break ;    //��ȡ���յĽǶ�
        case ANGY_ADDR:         vol16s = (GetYAngAllSetVal()/10) ; pc_data = *(int16u*)&vol16s ; break ;    //HXL 12/8/27 ���ϸ���

        case ANGP_ADDR:         vol16s = (GetAngP()/10)           ; pc_data = *(int16u*)&vol16s ; break ;    //HXL 12/8/27 ���ϸ���

        case ANGX_OFFSET_ADDR:  vol16s = (GetAngXInst()/10)       ; pc_data = *(int16u*)&vol16s ; break ;
        case ANGY_OFFSET_ADDR:  vol16s = (GetAngYInst()/10)       ; pc_data = *(int16u*)&vol16s ; break ;  //HXL 12/8/26 ���ϸ���
        case SELF_ADDR:         vol16s = Addr                     ; pc_data = *(int16u*)&vol16s ; break ;
        case BAUD_ADDR:         vol16s = Braud_Num                ; pc_data = *(int16u*)&vol16s ; break ;
 //**************************************
{
    {
        case 0x0100 : pc_data = SystemVer[ 0] *256 + SystemVer[ 1]       ; break ;
        case 0x0101 : pc_data = SystemVer[ 2] *256 + SystemVer[ 3]       ; break ;
        case 0x0102 : pc_data = SystemVer[ 4] *256 + SystemVer[ 5]       ; break ;
        case 0x0103 : pc_data = SystemVer[ 6] *256 + SystemVer[ 7]       ; break ;
        case 0x0104 : pc_data = SystemVer[ 8] *256 + SystemVer[ 9]       ; break ;
        case 0x0105 : pc_data = SystemVer[10] *256 + SystemVer[11]       ; break ;
        case 0x0106 : pc_data = SystemVer[12] *256 + SystemVer[13]       ; break ;
        case 0x0107 : pc_data = SystemVer[14] *256 + SystemVer[15]       ; break ;
        case 0x0108 : pc_data = FloatToInt( GetXAngAllSetVal() ,'L' )    ; break ;
        case 0x0109 : pc_data = FloatToInt( GetXAngAllSetVal() ,'H' )    ; break ;
        case 0x010A : pc_data = FloatToInt( GetYAngAllSetVal() ,'L' )    ; break ;
        case 0x010B : pc_data = FloatToInt( GetYAngAllSetVal() ,'H' )    ; break ;
        case 0x010C : pc_data = FloatToInt( GetAngP(), 'L' )             ; break ;
        case 0x010D : pc_data = FloatToInt( GetAngP(), 'H' )             ; break ;
        case 0x010E : pc_data = 0                                        ; break ;
        case 0x010F : pc_data = 0                                        ; break ;
        case 0x0110 : pc_data = 0                                        ; break ;
        case 0x011E : pc_data = FloatToInt( GetAngXUsrZero()    , 'L' )  ; break ;
        case 0x011F : pc_data = FloatToInt( GetAngXUsrZero()    , 'H' )  ; break ;
        case 0x0120 : pc_data = FloatToInt( GetAngYUsrZero()    , 'L' )  ; break ;
        case 0x0121 : pc_data = FloatToInt( GetAngYUsrZero()    , 'H' )  ; break ;
        case 0x0122 : pc_data = FloatToInt( GetAngXInst(), 'L' )         ; break ;
        case 0x0123 : pc_data = FloatToInt( GetAngXInst(), 'H' )         ; break ;
        case 0x0124 : pc_data = FloatToInt( GetAngYInst(), 'L' )         ; break ;
        case 0x0125 : pc_data = FloatToInt( GetAngYInst(), 'H' )         ; break ;
        case 0x0128 : pc_data = (GetFilterCnt())                         ; break ;
        case 0x0129 : pc_data = Addr                                     ; break ;

        //*************************************************************
        case 0xA000 : pc_data = 0xAC00                                   ; break ;
        case 0xA001 : pc_data = Get_McuTemp()                                 ; break ;
        //******************************************************
        case 0xA100 : pc_data = 0xAC01 ;                                 ; break ;
        case 0xA101 : pc_data = GetXFtVal()                             ; break ;    //HXL 12/8/27
        case 0xA102 : pc_data = FloatToInt(GetXAngAllSetVal(),'L')       ; break ;
        case 0xA103 : pc_data = FloatToInt(GetXAngAllSetVal(),'H')       ; break ;
        case 0xA11C : pc_data = GetAngXCalAdcVal(addr-0xA104)              ; break ;
        case 0xA14D : pc_data = FloatToInt(GetAngXCalAngVal((addr-0xA11D)/2),'L'); break ; //HXL 12/8/27���ϸ���
        case 0xA14E : pc_data = FloatToInt(GetAngXCalAngVal((addr-0xA11E)/2),'H'); break ; 
        //******************************************************
        case 0xA200 : pc_data = 0xAC02 ;                                 ; break ;
        case 0xA202 : pc_data = FloatToInt(GetYAngAllSetVal(),'L')       ; break ;
        case 0xA203 : pc_data = FloatToInt(GetYAngAllSetVal(),'H')       ; break ;           
        case 0xA21C : pc_data = GetAngYCalAdcVal(addr-0xA204)              ; break ;  
      case 0xA24D : pc_data = FloatToInt(GetAngYCalAngVal((addr-0xA21D)/2),'L'); break ;   
        case 0xA24E : pc_data = FloatToInt(GetAngYCalAngVal((addr-0xA21E)/2),'H'); break ;     
        //*************************************************************
        case 0xA300 : pc_data = 0xAC03                                      ; break ;
        case 0xA301 : pc_data = SystemVer[ 0] *256 + SystemVer[ 1]          ; break ;
        case 0xA302 : pc_data = SystemVer[ 2] *256 + SystemVer[ 3]          ; break ;
        case 0xA303 : pc_data = SystemVer[ 4] *256 + SystemVer[ 5]          ; break ;
        case 0xA304 : pc_data = SystemVer[ 6] *256 + SystemVer[ 7]          ; break ;
        case 0xA305 : pc_data = SystemVer[ 8] *256 + SystemVer[ 9]          ; break ;
        case 0xA306 : pc_data = SystemVer[10] *256 + SystemVer[11]          ; break ;
        case 0xA307 : pc_data = SystemVer[12] *256 + SystemVer[13]          ; break ;
        case 0xA308 : pc_data = SystemVer[14] *256 + SystemVer[15]          ; break ;
        case 0xA309 : pc_data = GetXFtVal()                                 ; break ;
        case 0xA30A : pc_data = GetYFtVal()                                 ; break ;
        case 0xA30D : pc_data = FloatToInt( GetXAng()         ,'L' )        ; break ;
        case 0xA30E : pc_data = FloatToInt( GetXAng()         ,'H' )        ; break ;
        case 0xA30F : pc_data = FloatToInt( GetYAng()         ,'L' )        ; break ;
        case 0xA310 : pc_data = FloatToInt( GetYAng()         ,'H' )        ; break ;
        case 0xA311 : pc_data = FloatToInt( GetAngXRelSys()  ,'L' )        ; break ;
        case 0xA312 : pc_data = FloatToInt( GetAngXRelSys()  ,'H' )        ; break ;
        case 0xA313 : pc_data = FloatToInt( GetAngYRelSys()  ,'L' )        ; break ;
        case 0xA314 : pc_data = FloatToInt( GetAngYRelSys()  ,'H' )        ; break ;
        case 0xA315 : pc_data = FloatToInt( GetAngXZeroUsr()     ,'L' )     ; break ;
        case 0xA316 : pc_data = FloatToInt( GetAngXZeroUsr()     ,'H' )     ; break ;
        case 0xA317 : pc_data = FloatToInt( GetAngYZeroUsr()     ,'L' )     ; break ;
        case 0xA318 : pc_data = FloatToInt( GetAngYZeroUsr()     ,'H' )     ; break ;
        case 0xA319 : pc_data = FloatToInt( GetXAngAllSetVal() ,'L' )       ; break ;
        case 0xA31A : pc_data = FloatToInt( GetXAngAllSetVal() ,'H' )       ; break ;
        case 0xA31B : pc_data = FloatToInt( GetYAngAllSetVal() ,'L' )       ; break ;
        case 0xA31C : pc_data = FloatToInt( GetYAngAllSetVal() ,'H' )       ; break ;
        case 0xA31D : pc_data = FloatToInt( GetAngP(), 'L' )                ; break ;
        case 0xA31E : pc_data = FloatToInt( GetAngP(), 'H' )                ; break ;
        case 0xA327 : pc_data = FloatToInt( GetAngXSysZero()   , 'L' )      ; break ;
        case 0xA328 : pc_data = FloatToInt( GetAngXSysZero()   , 'H' )      ; break ;
        case 0xA329 : pc_data = FloatToInt( GetAngYSysZero()   , 'L' )      ; break ;
        case 0xA32A : pc_data = FloatToInt( GetAngYSysZero()   , 'H' )      ; break ;
        case 0xA32B : pc_data = FloatToInt( GetAngXUsrZero()        , 'L' ) ; break ;
        case 0xA32C : pc_data = FloatToInt( GetAngXUsrZero()        , 'H' ) ; break ;
        case 0xA32D : pc_data = FloatToInt( GetAngYUsrZero()        , 'L' ) ; break ;
        case 0xA32E : pc_data = FloatToInt( GetAngYUsrZero()        , 'H' ) ; break ;
        case 0xA32F : pc_data = FloatToInt( GetAngXInst()    , 'L' )        ; break ;
        case 0xA330 : pc_data = FloatToInt( GetAngXInst()    , 'H' )        ; break ;
        case 0xA331 : pc_data = FloatToInt( GetAngYInst()    , 'L' )        ; break ;
        case 0xA332 : pc_data = FloatToInt( GetAngYInst()    , 'H' )        ; break ;

        case 0xA342 :pc_data = 0 ; break ;// pc_data = FloatToInt( Get_AngHigh()    , 'H' )     ; break ; //��ȡDA�����Ӧ�����Ƕ�

        case 0xA343 : pc_data = '1'*256+'8'                                 ; break ;
        case 0xA344 : pc_data = '6'*256+'B'                                 ; break ;
        case 0xA345 : pc_data = 2018                              		   ; break ;
        case 0xA346 : pc_data = 316                              		   ; break ;
        case 0xA347 : pc_data = 0                                           ; break ;
        case 0xA348 : //pc_data = (GetAngXCalCnt() *256) + GetTempXCalCnt()   ; break ;
        case 0xA349 : pc_data = AngRange                                    ; break ;
        case 0xA34A : pc_data = SendActPosi                                 ; break ;
        case 0xA34B : pc_data = SendSpeedNum                                ; break ;
        case 0xA34C : pc_data = GetFilterCnt()                              ; break ;
        case 0xA34D : pc_data = 0                                           ; break ;
        case 0xA34E : pc_data = 97                                         ; break ;
    }
}
        default:    *excode=2;
					return(FALSE);
 //       default:     return(FALSE);
    }
    ModRtu_Rd16u( buf, pc_data ) ;
    return(TRUE);
}
//***********************************************************************/
// *��    �ܣ����Ĵ�����ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�	
//�����������[�豸��ַ]+[�����]+[��ʼ�Ĵ�����ַ��8λ][��8λ]
//  			 +[��ȡ�ļĴ�������8λ][��8λ]+[CRCУ��ĵ�8λ][CRCУ��ĸ�8λ]
//�豸��Ӧ��         
// 	[�豸��ַ] [�����] [���ص��ֽڸ���][����1][����2]...[����n][CRCУ��ĵ�8λ] [CRCУ��ĸ�8λ]

//***********************************************************************/
void ModRtu_RdReg (int8u* buf)
{   int16u addr;
    int16u i,j;
    int16u crc=0;
    int8u err_code = 0,excode;

    if(buf[0] == 0)return;              //��֧�ֹ㲥��ʽ
    addr = ModRtu_8to16(&buf[2]);		//��Ҫ��ȡ�Ĵ�������ʼ��ַ h=buf[2],l=buf[3]
    j = ModRtu_8to16(&buf[4]);			//��ȡ�ļĴ����� h=buf[4],l=buf[5]
    for(i = 0; i < j; i++)
    {
        if(FALSE == ModRtu_RdVal(addr,&buf[3+i*2],&excode))
        {
            err_code = 1;
            break;
        }
        addr++;
    }
    if(err_code == 1)
    {
        ModRtu_RlyError(buf,excode);
        return;
    }
    buf[2] = j*2;       			//���ݳ���
    i = j*2+3;						//CRC�ļĴ�������=�豸��+�����+��������+������
    crc = ModbusCrcCnt(buf,i);		//����CRC
    ModRtu_WrCrc(&buf[i],crc);      //д��CRCУ����
    Comm_PutStr(buf,i+2);			//i+2 ��2��ָ��CRC�Ľ��
    Comm_SendSta();
}
//***********************************************************************/
// *��    �ܣ�д�Ĵ�����ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void ModRtu_WrReg (int8u* buf,int8u n)
{   int16u addr;
    int16u i,j;
    int16u crc;
    int8u err_code = 0,excode;

    j = ModRtu_8to16(&buf[4]);
    if((n != (j*2+9))||(buf[6]!= (j*2)))    return;	//���ݳ����ж�
    addr = ModRtu_8to16(&buf[2]);					//��ַ
    for(i = 0; i < j; i++)
    {
        if(FALSE == ModRtu_WrVal(addr,&buf[7+i*2],&excode))
        {
            err_code = 1;
            break;
        }
        addr++;
    }
    if(buf[0] == 0) return;         //�㲥��ʽ�޷���
    if(err_code == 1)
    {
        ModRtu_RlyError(buf,excode);
        return;
    }
    crc = ModbusCrcCnt(buf,6);
    ModRtu_WrCrc(&buf[6],crc);
    Comm_PutStr(buf,8);
    Comm_SendSta();
    if(  1 == udflashtag )
    {
        udflashtag=0;
		Flash_UserDataUpdate();
    }
}
//***********************************************************************/
// *��    �ܣ�ͨѶ�쳣����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
/*void ModRtu_RlyError (int8u* buf)
{   int16u crc;

    buf[1] |= 0x80;                 //���������λ����Ϊ1
    buf[2] = 0xff;
    crc = ModbusCrcCnt(buf,6);
    buf[3] = MR_READ_REGL(crc);
    buf[4] = MR_READ_REGH(crc);
    Comm_PutStr(buf,5);
    Comm_SendSta();
}*/
void ModRtu_RlyError (int8u* buf,int8u excode)
{   int16u crc;

    buf[1] |= 0x80;                 //���������λ����Ϊ1
    buf[2] = excode;
    crc = ModbusCrcCnt(buf,3);      //liuliu ���ĳ���,ԭΪ6,error
    buf[3] = MR_READ_REGH(crc);     //liuliu 2013-04-01
    buf[4] = MR_READ_REGL(crc);		//liuliu 2013-04-01
    Comm_PutStr(buf,5);
    Comm_SendSta();
}
//***********************************************************************/
// *��    �ܣ���������֡����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void ModRtu_RxProc (int8u* buf, int8u n)
{
    if(FALSE == ModRtu_ChkCrc(buf,n)) return;                   //����У����

    switch(buf[1])
    {
        case READ:          ModRtu_RdReg(buf);  				// ��ȡ�Ĵ���
                            break;
        case WRITE:         ModRtu_WrReg(buf,n);				// д�Ĵ���
                            break;
        default:            ModRtu_RlyError(buf,1);
                            break;
    }
}

#endif