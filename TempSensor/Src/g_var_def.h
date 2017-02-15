#ifndef __G_VAR_DEF__H
#define __G_VAR_DEF__H

//#define PROGRAM_PARAMETER_DOWNLOAD                     //����ú���ʹ�ó���ֱ�ӽ�����д��FLASH������ʹ�ó���ֱ��д��FLASH(ͨ�����ߴ���) 
#define SN_DOWNLOAD                     0              //�궨�忪��        1-ʹ��    0-��ֹ
#define BASIC_DOWNLOAD                  0              //�궨�忪��        1-ʹ��    0-��ֹ
#define LED_DEBUG                       0              //ledϵͳ����ָʾ��

#define CPU_F           ((double)8000000)
#define Delay_Us(x)     __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define Delay_Ms(x)     __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define Delay_S(x)      __delay_cycles((long)(CPU_F*(double)x/1.0))

typedef unsigned char      u8;
typedef unsigned short int u16;
typedef short int          s16;
typedef unsigned long      u32;
typedef long               s32;

typedef volatile u32 vu32;
typedef volatile u16 vu16;
typedef volatile u8 vu8;



/*
*********************************************************************************************************
*                                            ���������
*********************************************************************************************************
*/
#define D_RTU_RESPONSE                0              //�ش���Ӧ            1:�򿪹��� 0:�رչ���
#define D_READ_VERSION                0              //���汾��            1:�򿪹��� 0:�رչ���
#define D_PARAMTER_DOWNLOAD           0              //д����              1:�򿪹��� 0:�رչ���
#define D_READ_PARAMTER               0              //������              1:�򿪹��� 0:�رչ���
#define D_SAMPLE_TDATA                0              //��վ�ɼ���������    1:�򿪹��� 0:�رչ���(��ʱ��ʹ�øù��ܣ�������)
#define D_READ_SN_NUM                 0              //��SN��              1:�򿪹��� 0:�رչ���
#define D_WRITE_SN_NUM                0              //дSN��              1:�򿪹��� 0:�رչ���
#define D_TIME_CHECK                  1              //ʱ��У׼            1:�򿪹��� 0:�رչ���
#define D_READ_CUR_TIME               0              //����ǰʱ��          1:�򿪹��� 0:�رչ���
#define D_WARNING_SET                 0              //�����ޱ�������      1:�򿪹��� 0:�رչ���
#define D_SSI_SET                     0              //�ɼ�������ͼ������1:�򿪹��� 0:�رչ���



/* ���ڲ�׽F8913�źŴ���ģʽ�л�����Ӧ���ȶ�Ӱ��������Ҽ����ź��� �رոù���,������ʾ��ͬ*/
#define RSSI_TEST                       0
#define BATTERY_TEST                    1
#define RETRANSMISSION_TIMES            3                       //�ɼ������ش�����
#define RETRANSMISSION_OVER_TIME        64*3                    //��ʱʱ��(64*tick = 1024����)(��ǰtickΪ16ms)
/*********************************************************************************/
#define DEVICE_TYPE             0x01

#define DEBUG   1
#define	FAIL	0		        //����ֵʧ��
#define SECCESS !FAIL	                //����ֵ�ɹ�

#define APP_ERR 0                       //����
#define APP_OK  1                       //��ȷ

#define CONFIG_BUFFER_LENGTH    100
#define RF_BUFFER_LENGTH        250
#define CMD_MODE_BUFFER_LENGTH  50

#define DECIMALS                10      //LCD��ʾ������ȥ��С����X*10

#define CMD_MODE        0
#define TC_MODE         1
#define CMD_MODE_ON     CurMode = CMD_MODE
#define TC_MODE_ON      CurMode = TC_MODE


/* Global typedef -----------------------------------------------------------*/
/* RF ���ȫ�ֱ��� */
extern char RfBuf[RF_BUFFER_LENGTH];
extern s16  RfIndex;

/* Config ���ȫ�ֱ��� */
extern char ConfigBuf[CONFIG_BUFFER_LENGTH];	//���ڽ���������Ϣ
extern s16  ConfigBufIndex; 		        //���ڽ���������Ϣ����

extern char CmdModeBuffer[50];
extern u8   CmdModeBufferIndex; 

extern OS_EVENT *CMDMbox;              //����������������汾�š�����������SN��
extern OS_EVENT *ResponseMbox;         //�������䣺��Ӧ֡����

/*
*********************************************************************************************************
*                                            ��������
*********************************************************************************************************
*/
extern volatile char  Paramter_SampleTime;                        //�ɼ����
extern volatile char  Paramter_SendTime;                          //���ͼ��
extern volatile char  Paramter_WarningUpperLimit;                 //��������
extern volatile char  Paramter_WarningLowerLimit;                 //��������
extern volatile u16   Paramter_Year;                              //��
extern volatile char  Paramter_Month;                             //��
extern volatile char  Paramter_Day;                               //��
extern volatile char  Paramter_Hour;                              //ʱ
extern volatile char  Paramter_Min;                               //��
extern volatile char  Paramter_Sec;                               //��

extern u8  Paramter_Sn[6];                                        //SN��

extern u32 CurTime;                                               //��ǰʱ��(��)
extern u8  CurMode;                        //��ǰģʽ��0-����ģʽ 1-͸��ģʽ
extern u8  RfMsg;                          //��Ƶ˯��������Ϣ������������Ϣֵ
extern u8  SoftWatchDogOverTime;           //������Ź������־
/*
*********************************************************************************************************
*                                            ��վ����֡
*********************************************************************************************************
*/
#define   CMD_WORD                        0x1F             //���������ֺ�5λ
#define   READ_VERSION                    0x00             //���汾��
#define   PARAMTER_DOWNLOAD               0x01             //������������
#define   READ_PARAMTER                   0x02             //������
#define   SAMPLE_TDATA                    0x03             //��վ�ɼ����ݷ���
#define   RTU_RESPONSE                    0x04             //RTU�Ļ�Ӧ֡
#define   READ_SN_NUM                     0x06             //��SN��
#define   WRITE_SN_NUM                    0x07             //дSN��
#define   TIME_CHECK                      0x08             //ʱ��У��
#define   READ_CUR_TIME                   0x0a             //��ȡ��ǰʱ��
#define   WARNING_SET                     0x0b             //�¶�/ѹ����������
#define   SSI_SET                         0x0c             //�ɼ������ͼ������
/*
*********************************************************************************************************
*                                            ��վ����/�쳣Ӧ��֡
*********************************************************************************************************
*/
#define  S_PARAMTER_DOWNLOAD_OK           0x81             //������������_����_��Ӧ
#define  S_PARAMTER_DOWNLOAD_ERR          0xC1             //������������_�쳣_��Ӧ

#define  S_READ_VERSION                   0x80             //���汾��_��Ӧ
#define  S_READ_PARAMTER                  0x82             //������_��Ӧ
#define  S_SAMPLE_TDATA                   0x83             //��վ�ɼ����ݷ���_��Ӧ

#define  S_READ_SN_NUM_OK                 0x86             //��SN��_����_��Ӧ

#define  S_WRITE_SN_NUM_OK                0x87             //д�豸��ַ_����_��Ӧ
#define  S_WRITE_SN_NUM_ERR               0xC7             //д�豸��ַ_�쳣_��Ӧ

#define  S_TIME_CHECK_OK                  0x88             //ʱ��У��_����_��Ӧ
#define  S_TIME_CHECK_ERR                 0xC8             //ʱ��У��_�쳣_��Ӧ

#define  S_WARNING                        0x89             //���ޱ���

#define  CMD_WORD_TS                      0x85             //��ʱ��������������(������)

#define  S_WARNING_SET_OK                 0x8B             //�¶�/ѹ����������_����_��Ӧ
#define  S_WARNING_SET_ERR                0xCB             //�¶�/ѹ����������_�쳣_��Ӧ

#define  S_SSI_SET_OK                     0x8C             //�ɼ�/���ͼ������_����_��Ӧ
#define  S_SSI_SET_ERR                    0xCC             //�ɼ�/���ͼ������_�쳣_��Ӧ

#define  S_READ_CUR_TIME                  0x8A             //��ȡ��ǰʱ��_��Ӧ
/*
*********************************************************************************************************
*                                            ������
*********************************************************************************************************
*/

typedef enum{
  ReadVersion,          //0
  ParamterDownLoad,     //1
  ReadParamter,         //2
  SampleTData,          //3
  RtuResponse,          //4
  CmdReserve,           //5
  ReadSnNum,            //6
  WriteSnNum,           //7
  CurrentTimeCheck,     //8
  ReadCurTime,          //9
  WarningSet,           //10
  SampleSendSet,        //11
}CMD_OPERATE;  

/*
*********************************************************************************************************
*                                            ��Ӧ��FLASH_A
*********************************************************************************************************
*/
#define  FA_SN_ADDR_START       (char  *)0x1080
#define  FA_RTU_NUM             *((u16 *)0x1080)



/*
*********************************************************************************************************
*                                            ��Ӧ��FLASH_B
*********************************************************************************************************
*/
#define   FB_PARAMTER_ADDR                (char *)0x1000
#define   FB_SAMPLE_TIME               *((char  *)0x1000)
#define   FB_SEND_TIME                 *((char  *)0x1001)
#define   FB_WARNING_ULIMIT   *((unsigned char  *)0x1002)
#define   FB_WARNING_LLIMIT   *((unsigned char  *)0x1003)
#define   FB_YEAR                      *((u16   *)0x1004)
#define   FB_MONTH                     *((char  *)0x1006)
#define   FB_DAY                       *((char  *)0x1007)
#define   FB_HOUT                      *((char  *)0x1008)
#define   FB_MINITES                   *((char  *)0x1009)
#define   FB_SECONDS                   *((char  *)0x100a)
#define   FB_VERSION                   *((u16   *)0x100b)
#define   FB_DATE_ADDR                    (char *)0x1004
#define   FB_WARN_SET                     (char *)0x1002
#define   FB_SSI_SET                    FB_PARAMTER_ADDR


#define   RESERVE_WORD        0xFFFFFFFF               //������Ĭ��Ϊ0xFF
/*
*      ���跢�͵����ݰ���ʽ
*/
#pragma pack(1)
typedef struct{
    u32                 CycleNumber;            //���ڱ��
    u8                  PackageCnt;             //������
    u8                  PackageNumber;          //����
    u32                 Time;                   //�ɼ�ʱ��
    u8                  DataLength;             //���ݳ���
    float               SampleData;             //�ɼ���������
    float               BatteryCharge;          //��ص���
    u32                 Reserve;                //����
}DATA_FORMAT;
#pragma pack()

/*
*      ��׼֡��ʽ
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //ǰ����
    u8            StartSymbol_1;          //��ʼ��
    u8            SlaveAddr[6];           //��վ��ַ
    u8            StartSymbol_2;          //��ʼ��
    u8            CmdWord;                //������
    u8            DataLength;             //���ݳ���
    DATA_FORMAT   Data;                   //��׼֡�ܳ���12+26+3
    u8            CheckSum;               //У����
    u8            EndSymbol;              //������
}STD_FRAM;
#pragma pack()

/*
*      �汾��֡
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //ǰ����
    u8            StartSymbol_1;          //��ʼ��
    u8            SlaveAddr[6];           //��վ��ַ
    u8            StartSymbol_2;          //��ʼ��
    u8            CmdWord;                //������
    u8            DataLength;             //���ݳ���
    u16           Version;                //�汾��
    u8            CheckSum;               //У����
    u8            EndSymbol;              //������
}VSN_FRAM;
#pragma pack()

/*
*      ��������֡��Ӧ
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //ǰ����
    u8            StartSymbol_1;          //��ʼ��
    u8            SlaveAddr[6];           //��վ��ַ
    u8            StartSymbol_2;          //��ʼ��
    u8            CmdWord;                //������
    u8            DataLength;             //���ݳ���
    u8            Paramter[15];           //������Ϣ
    u8            CheckSum;               //У����
    u8            EndSymbol;              //������
}PARAMTER_FRAM;
#pragma pack()

/*
*      SN��֡��Ӧ
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //ǰ����
    u8            StartSymbol_1;          //��ʼ��
    u8            SlaveAddr[6];           //��վ��ַ
    u8            StartSymbol_2;          //��ʼ��
    u8            CmdWord;                //������
    u8            DataLength;             //���ݳ���
    u8            SnNumber[6];            //SN����Ϣ
    u8            CheckSum;               //У����
    u8            EndSymbol;              //������
}SN_FRAM;
#pragma pack()

/*
*      ʱ��֡��Ӧ
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //ǰ����
    u8            StartSymbol_1;          //��ʼ��
    u8            SlaveAddr[6];           //��վ��ַ
    u8            StartSymbol_2;          //��ʼ��
    u8            CmdWord;                //������
    u8            DataLength;             //���ݳ���
    u8            CurTime[7];             //ʱ����Ϣ:������ʱ����
    u8            CheckSum;               //У����
    u8            EndSymbol;              //������
}TIME_FRAM;
#pragma pack()

/*
*      Ԥ��֡��Ӧ
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //ǰ����
    u8            StartSymbol_1;          //��ʼ��
    u8            SlaveAddr[6];           //��վ��ַ
    u8            StartSymbol_2;          //��ʼ��
    u8            CmdWord;                //������
    u8            DataLength;             //���ݳ���
    u8            WarnSet[2];             //Ԥ����Ϣ:������
    u8            CheckSum;               //У����
    u8            EndSymbol;              //������
}WARN_FRAM;
#pragma pack()

/*
*      ���͡��ɼ��������֡��Ӧ
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //ǰ����
    u8            StartSymbol_1;          //��ʼ��
    u8            SlaveAddr[6];           //��վ��ַ
    u8            StartSymbol_2;          //��ʼ��
    u8            CmdWord;                //������
    u8            DataLength;             //���ݳ���
    u8            SSISet[2];              //�ɼ������ͼ����Ϣ
    u8            CheckSum;               //У����
    u8            EndSymbol;              //������
}SSI_FRAM;
#pragma pack()


/*
*      SN�Žṹ��
*/
#pragma pack(1)
typedef struct
{
    u16           SN_DevId;          //�豸��
    u8            SN_Batch;          //����
    u16           SN_Year;           //��
    u8            SN_DevType;        //�豸����
}SN_STRUCT;
#pragma pack()


extern STD_FRAM        StdFram;
extern DATA_FORMAT     SubFram;
extern VSN_FRAM        VsnFram;
extern PARAMTER_FRAM   ParamterFram;
extern SN_FRAM         SnFram;
extern TIME_FRAM       TimeFram;
extern WARN_FRAM       WarnFram;
extern SSI_FRAM        SsiFram;
extern SN_STRUCT       Sn_Struct; 
extern CMD_OPERATE CmdOp[12];
extern volatile u32 LocalTime;
#endif