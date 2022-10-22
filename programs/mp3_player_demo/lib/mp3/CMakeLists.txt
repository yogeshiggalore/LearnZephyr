# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

target_sources(app PRIVATE
    "mp3.c"
    "mp3.h"
)

add_subdirectory("YX5300")

target_include_directories(app PUBLIC "./")


