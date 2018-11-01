include ../Makefile.tests_common

FEATURES_REQUIRED = periph_i2c

USEMODULE += ina219
USEMODULE += xtimer
USEMODULE += fmt

# set default device parameters in case they are undefined
TEST_INA219_I2C  ?= I2C_DEV\(0\)
TEST_INA219_ADDR ?= 0x40

# export parameters
CFLAGS += -DTEST_INA219_I2C=$(TEST_INA219_I2C)
CFLAGS += -DTEST_INA219_ADDR=$(TEST_INA219_ADDR)
CFLAGS += -ggdb -g3 -Og

include $(RIOTBASE)/Makefile.include
