# name of your application
APPLICATION = saul_example

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

BOARD_INSUFFICIENT_MEMORY := arduino-duemilanove arduino-uno

# we want to use SAUL:
USEMODULE += saul_default
# include the shell:
USEMODULE += shell
USEMODULE += shell_commands
# additional modules for debugging:
USEMODULE += ps
USEMODULE += ph_oem

#Example for providing device settings thru CFLAGS 
PH_OEM_PARAM_I2C  ?= I2C_DEV\(0\)
PH_OEM_PARAM_ADDR ?= 0x65
PH_OEM_PARAM_EN_PIN ?= GPIO_PIN\(3,1\)
PH_OEM_PARAM_INTERRUPT_PIN ?= GPIO_PIN\(2,6\)

# export parameters
#CFLAGS += -DPH_OEM_PARAM_I2C=$(PH_OEM_PARAM_I2C)
#CFLAGS += -DPH_OEM_PARAM_ADDR=$(PH_OEM_PARAM_ADDR)
CFLAGS += -DPH_OEM_PARAM_EN_PIN=$(PH_OEM_PARAM_EN_PIN)
CFLAGS += -DPH_OEM_PARAM_INTERRUPT_PIN=$(PH_OEM_PARAM_INTERRUPT_PIN)


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
