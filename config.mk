# Name: Makefile_STM
# Author: Daniel Nery Silva de Oliveira
# ThundeRatz Robotics Team
# 06/2019

# Cube file name without .ioc extension
PROJECT_NAME = Teste_VL53L0X

DEVICE_FAMILY  := STM32G0xx
DEVICE_TYPE    := STM32G071xx
DEVICE_DEF     := STM32G071xx
DEVICE         := STM32G071RB

# Linker script file without .ld extension
# Find it on cube folder after code generation
DEVICE_LD_FILE := STM32G071RBTx_FLASH

# Lib dir
LIB_DIR  := lib

# Cube Directory
CUBE_DIR := cube

# Default values, can be set on the command line or here
DEBUG   ?= 1
VERBOSE ?= 0
