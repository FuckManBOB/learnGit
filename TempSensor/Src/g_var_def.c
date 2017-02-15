#include "includes.h"


u8  CurMode   = 0;                        //��ǰģʽ��0-����ģʽ 1-͸��ģʽ

u8  RfMsg     = 1;                        //��Ƶ˯��������Ϣ������������Ϣֵ

u8  SoftWatchDogOverTime;                 //������Ź������־

char CmdModeBuffer[CMD_MODE_BUFFER_LENGTH];//����ģʽ����
u8   CmdModeBufferIndex; 

char RfBuf[RF_BUFFER_LENGTH];             //͸��ģʽ����
s16  RfIndex;

/* Config ���ȫ�ֱ��� */
char ConfigBuf[CONFIG_BUFFER_LENGTH];	//���ڽ���������Ϣ
s16  ConfigBufIndex; 		        //���ڽ���������Ϣ����


/*
*********************************************************************************************************
*                                            ��������
*********************************************************************************************************
*/
volatile char  Paramter_SampleTime;                        //�ɼ����
volatile char  Paramter_SendTime;                          //���ͼ��
volatile char  Paramter_WarningUpperLimit;                 //��������
volatile char  Paramter_WarningLowerLimit;                 //��������
volatile u16   Paramter_Year;                              //��
volatile char  Paramter_Month;                             //��
volatile char  Paramter_Day;                               //��
volatile char  Paramter_Hour;                              //ʱ
volatile char  Paramter_Min;                               //��
volatile char  Paramter_Sec;                               //��

u8  Paramter_Sn[6];                                      //SN��

DATA_FORMAT     SubFram         = {1, 1, 1, 0, 12, 0, 0, 0};
      

STD_FRAM        StdFram         = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF,
                                    0xFF, 0xFF}, 0x68, 0x00, 0x00, {0x00}, 0xFF, 0x16};

VSN_FRAM        VsnFram         = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0x80, 0x02, 0, 0xFF, 0x16};

PARAMTER_FRAM   ParamterFram    = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0, 0x0F, {0}, 0xFF, 0x16};

SN_FRAM         SnFram          = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0, 0x06, {0}, 0xFF, 0x16};

TIME_FRAM       TimeFram        = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0, 0x07, {0}, 0xFF, 0x16};

WARN_FRAM       WarnFram        = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0, 0x02, {0}, 0xFF, 0x16};

SSI_FRAM        SsiFram         = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0, 0x02, {0}, 0xFF, 0x16};


CMD_OPERATE CmdOp[12] = {ReadVersion,
  ParamterDownLoad,
  ReadParamter,
  SampleTData,
  RtuResponse,
  CmdReserve,
  ReadSnNum,
  WriteSnNum,
  CurrentTimeCheck,
  ReadCurTime,
  WarningSet,
  SampleSendSet};


