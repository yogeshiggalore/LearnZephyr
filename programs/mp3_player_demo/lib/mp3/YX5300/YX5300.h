/*******************************************************************************
* File Name: YX5300.h
*
* Version: 1.0
*
* Description:
* This is header file for Catalex serial MP3 player YX5300. 
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
#ifndef __YX5300_H
#define __YX5300_H

#include <zephyr.h>
#include <drivers/uart.h>

typedef enum {
    none=0,
    next_song=1,
    previous_song=2,
    play_index=3,
    volume_up=4,
    volume_down=5,
    volume_set=6,
    single_cycle=8,
    select_device=9,
    sleep_mode=10,
    wake_up=11,
    reset=12,
    play=13,
    pause=14,
    play_fldr_fil=15,
    stop=0x16,
    play_cycle_fldr=0x17,
    set_dac=0x1A,
    play_with_vol=0x22,
    song_completed=0x3D,
    data_recv_err=0x40,
    data_recv_ok=0x41,
    current_state=0x42,
    current_volume=0x43,
    song_querry=0x48,
    current_song=0x4C
}YX5300_CMD;

struct yx5300_config {
    char uart_port[7];
    struct uart_config cfg_uart; 
	uint16_t current_song_index;
};

int yx5300_configure(const struct yx5300_config *cfg);
int yx5300_write_cmd(YX5300_CMD cmd);
#endif