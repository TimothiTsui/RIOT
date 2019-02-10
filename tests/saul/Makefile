include ../Makefile.tests_common

# include and auto-initialize all available sensors
USEMODULE += saul_default

USEMODULE += ph_oem
USEMODULE += xtimer

CFLAGS += -DLOG_LEVEL=LOG_ALL

# Too little flash:
BOARD_INSUFFICIENT_MEMORY := arduino-duemilanove arduino-uno

include $(RIOTBASE)/Makefile.include
