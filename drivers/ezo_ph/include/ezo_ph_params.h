/*
 * Copyright (C) 2017 Igor Knippenberg <igor.knippenberg@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_ezo_ph
 * @{
 *
 * @file
 * @brief       Default configuration for Atlas Scientific EZO pH circuits
 *
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 */

#ifndef EZO_PH_PARAMS_H
#define EZO_PH_PARAMS_H

#include "board.h"
#include "saul_reg.h"
#include "ezo_ph.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters for the ADCXX1C driver
 * @{
 */
#ifndef EZO_PH_PARAM_I2C
#define EZO_PH_PARAM_I2C        (I2C_DEV(0))
#endif
#ifndef EZO_PH_PARAM_ADDR
#define EZO_PH_PARAM_ADDR       (EZO_PH_I2C_ADDRESS )
#endif

#ifndef EZO_PH_PARAMS
#define EZO_PH_PARAMS  { .i2c        = ADCXX1C_PARAM_I2C, \
                                  .addr       = ADCXX1C_PARAM_ADDR}
#endif
/** @} */

/**
 * @brief   EZO_PH configuration
 */
static const ezo_ph_params_t ezo_ph_params[] =
{
#ifdef EZO_PH_PARAMS_BOARD
    ADCXX1C_PARAMS_BOARD,
#else
    ADCXX1C_PARAMS_DEFAULT, // @suppress("Symbol is not resolved")
#endif
};


/**
 * @brief   Additional meta information to keep in the SAUL registry
 */
static const saul_reg_info_t adcxx1c_saul_info[] =
{
    {
        .name = "adcxx1c",
    },
};

#ifdef __cplusplus
}
#endif

#endif /* ADCXX1C_PARAMS_H */
/** @} */
