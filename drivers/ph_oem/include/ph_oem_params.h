/*
 * Copyright (C) 2017 OTA keys S.A.
 *               2018 Acutam Automation, LLC
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_ads101x
 * @{
 *
 * @file
 * @brief       Default configuration for ADS101x/111x devices
 *
 * @author      Vincent Dupont <vincent@otakeys.com>
 * @author      Matthew Blue <matthew.blue.neuro@gmail.com>
 */

#ifndef PH_OEM_PARAMS_H
#define PH_OEM_PARAMS_H

#include "board.h"
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
#define PH_OEM_PARAM_I2C        (I2C_DEV(0))
#endif
#ifndef PH_OEM_PARAM_ADDR
#define PH_OEM_PARAM_ADDR       (PH_OEM_I2C_ADDRESS)
#endif
#ifndef PH_OEM_PARAM_INTERRUPT_PIN
#define PH_OEM_PARAM_INTERRUPT_PIN  (GPIO_UNDEF)
#endif
#ifndef PH_OEM_PARAM_EN_PIN
#define PH_OEM_PARAM_EN_PIN  (GPIO_UNDEF)
#endif

#ifndef ADS101X_PARAMS
#define ADS101X_PARAMS          { .i2c        = ADS101X_PARAM_I2C,        \
                                  .addr       = ADS101X_PARAM_ADDR }
#endif

#ifndef ADS101X_ALERT_PARAMS
#define ADS101X_ALERT_PARAMS    { .i2c        = ADS101X_PARAM_I2C,        \
                                  .addr       = ADS101X_PARAM_ADDR,       \
                                  .alert_pin  = ADS101X_PARAM_ALERT_PIN,  \
                                  .low_limit  = ADS101X_PARAM_LOW_LIMIT,  \
                                  .high_limit = ADS101X_PARAM_HIGH_LIMIT }
#endif
//#ifndef ADS101X_SAUL_INFO
//#define ADS101X_SAUL_INFO       { .name = "ads101x" }
//#endif
/** @} */

/**
 * @brief   ADS101X/111x defaults if not defined for a board or application
 */
static const ads101x_params_t ads101x_params[] = {
ADS101X_PARAMS };
static const ads101x_alert_params_t ads101x_alert_params[] = {
ADS101X_ALERT_PARAMS };

///**
// * @brief   Additional meta information to keep in the SAUL registry
// */
//static const saul_reg_info_t ads101x_saul_info[] = {
//ADS101X_SAUL_INFO };

#ifdef __cplusplus
}
#endif

#endif /* PH_OEM_PARAMS_H */
/** @} */
