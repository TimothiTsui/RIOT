include ../Makefile.tests_common

FEATURES_REQUIRED = periph_i2c

USEMODULE += ph_oem
USEMODULE += xtimer
USEMODULE += fmt

# set default device parameters in case they are undefined
TEST_PH_OEM_I2C  ?= I2C_DEV\(0\)
TEST_PH_OEM_ADDR ?= 0x65

# export parameters
CFLAGS += -DTEST_PH_OEM_I2C=$(TEST_PH_OEM_I2C)
CFLAGS += -DTEST_PH_OEM_ADDR=$(TEST_PH_OEM_ADDR)

include $(RIOTBASE)/Makefile.include
