# name of your application
APPLICATION = Olivers_Thesis

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

CFLAGS = -c -Wall -I$(CURDIR)/include

USEMODULE += lsm6dsl
USEMODULE += xtimer

# Example for providing settings through CFLAGS. Change them based on your setup
LSM6DSL_PARAM_ACC_ODR  ?= LSM6DSL_DATA_RATE_6_66KHZ

# export parameters. Uncomment the one you want to use
CFLAGS += -DLSM6DSL_PARAM_ACC_ODR=$(LSM6DSL_PARAM_ACC_ODR)



include $(RIOTBASE)/Makefile.include
