/*
 * Copyright (C) 2017 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_hd44780
 *
 * @{
 * @file
 * @brief       Pinout config for the HD44780 display
 *
 * @author      Sebastian Meiling <s@mlng.net>
 */
#ifndef HD44780_PARAMS_H
#define HD44780_PARAMS_H

#include "board.h"
#include "periph/gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef HD44780_PARAM_COLS
#define HD44780_PARAM_COLS              (16U)
#endif
#ifndef HD44780_PARAM_ROWS
#define HD44780_PARAM_ROWS              (2U)
#endif
#ifndef HD44780_PARAM_PIN_RS
#define HD44780_PARAM_PIN_RS            ARDUINO_PIN_2
#endif
#ifndef HD44780_PARAM_PIN_RW
#define HD44780_PARAM_PIN_RW            GPIO_UNDEF
#endif
#ifndef HD44780_PARAM_PIN_ENABLE
#define HD44780_PARAM_PIN_ENABLE        ARDUINO_PIN_3
#endif
#ifndef HD44780_PARAM_PINS_DATA
#define HD44780_PARAM_PINS_DATA         {   ARDUINO_PIN_4,  \
                                            ARDUINO_PIN_5,  \
                                            ARDUINO_PIN_6,  \
                                            ARDUINO_PIN_7,  \
                                            GPIO_UNDEF,     \
                                            GPIO_UNDEF,     \
                                            GPIO_UNDEF,     \
                                            GPIO_UNDEF }
#endif

#ifndef HD44780_PARAMS
#define HD44780_PARAMS               {  .cols   = HD44780_PARAM_COLS,       \
                                        .rows   = HD44780_PARAM_ROWS,       \
                                        .rs     = HD44780_PARAM_PIN_RS,     \
                                        .rw     = HD44780_PARAM_PIN_RW,     \
                                        .enable = HD44780_PARAM_PIN_ENABLE, \
                                        .data   = HD44780_PARAM_PINS_DATA }
#endif

/**
 * @brief   LCM1602C configuration
 */
static const hd44780_params_t hd44780_params[] =
{
    HD44780_PARAMS,
};

#ifdef __cplusplus
}
#endif

#endif /* HD44780_PARAMS_H */
/** @} */
