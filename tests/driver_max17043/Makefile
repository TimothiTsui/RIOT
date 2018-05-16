include ../Makefile.tests_common

FEATURES_REQUIRED = periph_i2c

USEMODULE += max17043
USEMODULE += xtimer

# set default device parameters in case they are undefined
TEST_max17043_I2C  ?= I2C_DEV\(0\)
TEST_max17043_ADDR ?= 0x6C

# export parameters
CFLAGS += -DTEST_max17043_I2C=$(TEST_max17043_I2C)
CFLAGS += -DTEST_max17043_ADDR=$(TEST_max17043_ADDR)

include $(RIOTBASE)/Makefile.include
