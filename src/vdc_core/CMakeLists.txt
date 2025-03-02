cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)

project(vdc_core VERSION 0.1.0)

add_library(vdc_core STATIC vdc_core.c)