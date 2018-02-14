/*
 * Copyright (C) 2017 Igor Knippenberg <igor.knippenberg@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup   drivers_ezo_ph Atlas Scientific EZO pH circuit device driver
 * @ingroup    drivers_sensors
 * @brief      I2C Atlas Scientific EZO pH circuit device driver
 *
 *             This driver works with Atlas Scientific EZO pH firmware ....
 * @{
 *
 * @file
 * @brief      EZO pH circuit device driver
 *
 * @author     Igor Knippenberg <igor.knippenberg@gmail.com>
 */

#ifndef EZO_PH_H
#define EZO_PH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "periph/i2c.h"
#include "periph/gpio.h"

/**
 * @brief  EZO pH I2C default address
 */
#ifndef EZO_PH_I2C_ADDRESS
#define EZO_PH_I2C_ADDRESS  (0x68)
#endif



/**
 * @brief   Named return values
 */
enum {
    EZO_PH__NODEV      = -2,    /**< no EZO_PH device found on the bus */
    EZO_PH_NOI2C       = -1,    /**< I2C communication failed */
    EZO_PH_OK          = 1,     /**< everything was fine */
    EZO_PH_ERROR       = 2,     /**< EZO_PH returned "error" */
    EZO_PH_NOTREADY    = 254,   /**< EZO_PH is still processing, not ready yet */
    EZO_PH_NODATA      = 255    /**< no data to send */
};

/**
 * @brief   EZO_PH params
 */
typedef struct ezo_ph_params {
    i2c_t i2c;            /**< i2c device */
    uint8_t addr;         /**< i2c address */
    uint8_t bits;         /**< resolution */
    uint8_t cycle;        /**< conversion interval */
    gpio_t alert_pin;     /**< alert pin (GPIO_UNDEF if not connected) */
    int16_t low_limit;    /**< alert low value */
    int16_t high_limit;   /**< alert high value */
    int16_t hysteresis;   /**< alert hysteresis */
} ezo_ph_params_t;

/**
 * @brief   ADCxx1C alert callback
 */
typedef void (*adcxx1c_cb_t)(void *);

/**
 * @brief   ADCxx1C device descriptor
 */
typedef struct adcxx1c {
    adcxx1c_params_t params; /**< device driver configuration */
    adcxx1c_cb_t cb;         /**< alert callback */
    void *arg;               /**< alert callback param */
} adcxx1c_t;

/**
 * @brief   Initialize an ADCxx1C ADC device
 *
 * @param[in,out] dev  device descriptor
 * @param[in] params   device configuration
 *
 * @return zero on successful initialization, non zero on error
 */
int adcxx1c_init(adcxx1c_t *dev, const adcxx1c_params_t *params);

/**
 * @brief   Read a raw ADC value
 *
 * @param[in] dev   device descriptor
 * @param[out] raw  read value
 *
 * @return zero on successful read, non zero on error
 */
int adcxx1c_read_raw(const adcxx1c_t *dev, int16_t *raw);

/**
 * @brief   Enable alert interrupt
 *
 * @param[in] dev   device descriptor
 * @param[in] cb    callback called when the alert fires
 * @param[in] arg   callback argument
 *
 * @return zero on success, non zero on error
 */
int adcxx1c_enable_alert(adcxx1c_t *dev, adcxx1c_cb_t cb, void *arg);

/**
 * @brief   Set the alert parameters
 *
 * @param[in,out] dev      device descriptor
 * @param[in] low_limit    alert low limit
 * @param[in] high_limit   alert high limit
 * @param[in] hysteresis   alert hysteresis
 *
 * @return zero on success, non zero on error
 */
int adcxx1c_set_alert_parameters(const adcxx1c_t *dev, int16_t low_limit,
                                 int16_t high_limit, int16_t hysteresis);

#ifdef __cplusplus
}
#endif

#endif /* ADCXX1C_H */
/** @} */
