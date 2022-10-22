/*******************************************************************************
* File Name: YX5300.c
*
* Version: 1.0
*
* Description:
* This is source file for Catalex serial MP3 player YX5300. 
* all the API are implemented in this file.
* 
*
* Written:
* Yogesh M Iggalore
*
* Code Tested With:
* Zephyr and nrf52832
*
********************************************************************************
* Copyright (2021-22) , LearnZephyr
********************************************************************************/
#include <zephyr.h>
#include <YX5300.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(yx5300, LOG_LEVEL_NONE);

struct yx5300_config cfg_yx5300;

int yx5300_configure(struct yx5300_config *cfg)
{   
    /* setting up uart port */
    
}

int yx5300_write_cmd(YX5300_CMD cmd)
{
    cmd=0;
}