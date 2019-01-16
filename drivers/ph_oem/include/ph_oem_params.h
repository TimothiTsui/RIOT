/*
 * Copyright (C) 2019 Igor Knippenberg
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
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 */

#ifndef PH_OEM_PARAMS_H
#define PH_OEM_PARAMS_H

//#include "board.h"
//#include "saul_reg.h"
#include "ph_oem.h"
#include "ph_oem_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters for the Atlas Scientific pH OEM driver
 * @{
 */
#ifndef PH_OEM_PARAM_I2C
#define PH_OEM_PARAM_I2C            (I2C_DEV(0))
#endif
#ifndef PH_OEM_PARAM_ADDR
#define PH_OEM_PARAM_ADDR           (PH_OEM_I2C_ADDRESS)
#endif
#ifndef PH_OEM_PARAM_INTERRUPT_PIN
#define PH_OEM_PARAM_INTERRUPT_PIN  (GPIO_UNDEF)
#endif
#ifndef PH_OEM_PARAM_EN_PIN
#define PH_OEM_PARAM_EN_PIN         (GPIO_UNDEF)
#endif

#ifndef PH_OEM_PARAMS
#define PH_OEM_PARAMS       { .i2c = PH_OEM_PARAM_I2C,        \
                              .addr = PH_OEM_PARAM_ADDR,       \
                              .interrupt_pin = PH_OEM_PARAM_INTERRUPT_PIN,  \
                              .enable_pin = PH_OEM_PARAM_EN_PIN }
#endif

/**
 * @brief   pH OEM defaults if not defined for a board or application
 */
static const ph_oem_params_t ph_oem_params[] =
{
    PH_OEM_PARAMS
};

#ifdef __cplusplus
}
#endif

#endif /* PH_OEM_PARAMS_H */
/** @} */
