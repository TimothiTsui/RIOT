include ../Makefile.tests_common

FEATURES_REQUIRED = periph_i2c

USEMODULE += max17043
USEMODULE += xtimer
USEMODULE += fmt
USEMODULE += gnrc_netdev_default

# set default device parameters in case they are undefined
TEST_MAX17043_I2C  ?= I2C_DEV\(0\)
TEST_MAX17043_ADDR ?= 0x36

# export parameters
CFLAGS += -DTEST_MAX17043_I2C=$(TEST_MAX17043_I2C)
CFLAGS += -DTEST_MAX17043_ADDR=$(TEST_MAX17043_ADDR)

include $(RIOTBASE)/Makefile.include
