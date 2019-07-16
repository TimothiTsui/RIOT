/*
 * Copyright (C) 2019 University of Applied Sciences Emden / Leer
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_ph_oem
 * @{
 *
 * @file
 * @brief       Default configuration for Atlas Scientific pH OEM sensors
 *
 * @author      Ting Xu <timtsui@outlook.com>
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 */

#ifndef EC_OEM_PARAMS_H
#define EC_OEM_PARAMS_H

#include "board.h" /* THIS INCLUDE IS MANDATORY */
#include "saul_reg.h"
#include "ph_oem.h"
#include "ph_oem_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters for the Atlas Scientific pH OEM driver
 * @{
 */
#ifndef EC_OEM_PARAM_I2C
#define EC_OEM_PARAM_I2C                  (I2C_DEV(0))
#endif
#ifndef EC_OEM_PARAM_ADDR
#define EC_OEM_PARAM_ADDR                 (0x65)
#endif
#ifndef EC_OEM_PARAM_INTERRUPT_PIN
#define EC_OEM_PARAM_INTERRUPT_PIN        (GPIO_UNDEF)
#endif
#ifndef EC_OEM_PARAM_INTERRUPT_OPTION
#define EC_OEM_PARAM_INTERRUPT_OPTION     (PH_OEM_IRQ_BOTH)
#endif
#ifndef EC_OEM_PARAM_INTERRUPT_GPIO_MODE
#define EC_OEM_PARAM_INTERRUPT_GPIO_MODE  (GPIO_IN_PD)
#endif

#ifndef EC_OEM_PARAMS
#define EC_OEM_PARAMS       { .i2c = EC_OEM_PARAM_I2C,        \
                              .addr = EC_OEM_PARAM_ADDR,       \
                              .interrupt_pin = EC_OEM_PARAM_INTERRUPT_PIN, \
                              .gpio_mode = EC_OEM_PARAM_INTERRUPT_GPIO_MODE, \
                              .irq_option = EC_OEM_PARAM_INTERRUPT_OPTION }
#endif
#ifndef EC_OEM_SAUL_INFO
#define EC_OEM_SAUL_INFO       { .name = "EC OEM sensor" }
#endif
/** @} */
/**
 * @brief   pH OEM defaults if not defined for a board or application
 */
static const ph_oem_params_t ph_oem_params[] =
{
    EC_OEM_PARAMS
};

/**
 * @brief   Additional meta information to keep in the SAUL registry
 */
static const saul_reg_info_t ph_oem_saul_info[] =
{
    PH_OEM_SAUL_INFO
};

#ifdef __cplusplus
}
#endif

#endif /* EC_OEM_PARAMS_H */
/** @} */
