include ../Makefile.tests_common

USEMODULE += ph_oem
USEMODULE += fmt

## set default device parameters in case they are undefined
#PH_OEM_PARAM_I2C  ?= I2C_DEV\(0\)
#PH_OEM_PARAM_ADDR ?= 0x65
PH_OEM_PARAM_EN_PIN ?= GPIO_PIN\(3,1\)
#
## export parameters
#CFLAGS += -DPH_OEM_PARAM_I2C=$(PH_OEM_PARAM_I2C)
#CFLAGS += -DPH_OEM_PARAM_ADDR=$(PH_OEM_PARAM_ADDR)
CFLAGS += -DPH_OEM_PARAM_EN_PIN=$(PH_OEM_PARAM_EN_PIN)

include $(RIOTBASE)/Makefile.include
