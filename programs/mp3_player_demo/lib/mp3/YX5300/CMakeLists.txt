# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

target_sources(app PRIVATE
    "YX5300.c"
    "YX5300.h"
)

target_include_directories(app PUBLIC "./")


