/*
 * Copyright (C) 2015 Eistec AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_max17043 max17043 current/power monitor
 * @ingroup     drivers_sensors
 * @brief       Device driver for Texas Instruments max17043 High or Low Side,
 *              Bi-Directional CURRENT/POWER MONITOR with Two-Wire Interface
 * @{
 *
 * @file
 *
 *
 * @author      Dhruv Verma <dhruv2scs@gmail.com>
 */

#ifndef max17043_H
#define max17043_H

#include <stdint.h>

#include "periph/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Device descriptor for max17043 sensors
 */
typedef struct {
    i2c_t i2c;              /**< I2C device the sensor is connected to */
    uint8_t addr;           /**< the slave address of the sensor on the I2C bus */
} max17043_t;

/**
 * @brief   max17043 possible mode settings
 */

typedef enum max17043_mode {
    MAX17043_MODE_QUICK_START = 0x4000,
} max17043_mode_t;

typedef enum max17043_config {
    MAX17043_CONFIG_DEFAULT = 0x971C,
    MAX17043_CONFIG_SLEEP = 0x0080,
    MAX17043_CONFIG_NOT_SLEEP = 0x0000,
} max17043_config_t;

///** max17043 reset command bit (in configuration register) */
//#define max17043_RESET_BIT (0x8000)


/**
 * @brief   Initialize a current sensor
 *
 * @param[out] dev          device descriptor of sensor to initialize
 * @param[in]  i2c          I2C bus the sensor is connected to
 * @param[in]  address      I2C slave address of the sensor
 *
 * @return                  0 on success
 * @return                  <0 on error
 */
int max17043_init(max17043_t *dev, i2c_t i2c, uint8_t address);


/**
 * @brief   Read v voltage
 *
 * @param[in]  dev          device descriptor of sensor
 * @param[out] voltage      measured voltage across shunt resistor
 *
 * @return                  0 on success
 * @return                  <0 on error
 */

int max17043_read_soc(const max17043_t *dev, uint16_t *temp);

int max17043_read_vcell(const max17043_t *dev, uint16_t *temp);

/*int max17043_read_vcell(const max17043_t *dev, int16_t *voltage);*/

/*int max17043_read_soc(const max17043_t *dev, int16_t *battery);*/



#ifdef __cplusplus
}
#endif

#endif /* max17043_H */
/** @} */
