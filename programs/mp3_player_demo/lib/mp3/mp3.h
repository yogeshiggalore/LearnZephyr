/*******************************************************************************
* File Name: mp3.h
*
* Version: 1.0
*
* Description:
* This is header file for MP3 player. 
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
#ifndef __MP3_H
#define __MP3_H

#include <zephyr.h>

void mp3_play_song(uint16_t ui16Index);

#endif