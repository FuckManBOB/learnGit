/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-1999, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                           MASTER INCLUDE FILE
*********************************************************************************************************
*/

#ifndef     __INCLUDES_H
#define     __INCLUDES_H
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <in430.h>
#include    <msp430f1611.h>


#include "os_cpu.h"
#include "os_cfg.h"
#include "ucos_ii.h"

#include "app_cfg.h"
#include "g_var_def.h"
#include "hal_board.h"
#include "hal_lcd_tm1621.h"
#include "hal_rf.h"
#include "hal_battery.h"
#include "hal_flash.h"
#include "hal_ds18b20.h"
#include "hal_time.h"
#include "cmd_func.h"
#include "app_wdt.h"
#include "app_wd_handle.h"


#endif
