#ifndef __CMD_FUNC_H
#define __CMD_FUNC_H
#include "includes.h"


void    CmdOp_RVN                  (void                                     );
void    CmdOp_WPTR                 (void                                     );
void    CmdOp_RPTR                 (void                                     );
void    CmdOp_SSD                  (void                                     );
void    CmdOp_RRF                  (void                                     );
void    CmdOp_RSN                  (void                                     );
void    CmdOp_WSN                  (void                                     );
u8      CmdOp_CTC                  (void                                     );
void    CmdOp_RCT                  (void                                     );
void    CmdOp_WS                   (void                                     );
void    CmdOp_SSI                  (void                                     );
void    GetBasicParameter          (void                                     );
u8      BasicParameterCheck        (char                                 *Ptr);
u8      SnCheck                    (char                               *SnPtr);
u8      WarnSetCheck               (char                              *wscPtr);
u8      CurTimeCheck               (char                              *ctcPtr);
u8      SSICheck                   (char                              *ssiPtr);
void    FramHandle                 (u32 CurTime_Sec, float SData, float BCData, u8 RespCmdWord, u32 Reserve);
void    ParameterHandle            (u8                            RespCmdWord);
void    SnNumberHandle             (u8                            RespCmdWord);
void    TimerHandle                (u8                            RespCmdWord);
void    ReadCurTimeAndSendTimeFram (u8                            RespCmdWord);
void    WarnSetHandle              (u8                            RespCmdWord);
void    SSISetHandle               (u8                            RespCmdWord);
#endif
