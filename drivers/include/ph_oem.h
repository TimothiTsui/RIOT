/*
 * Copyright (C) 2019 Igor Knippenberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_ph_oem pH OEM sensor device driver
 * @ingroup     drivers_sensors
 * @brief       Device driver for Atlas Scientific pH OEM Sensor with SMBus/I2C interface
 *
 * @{
 *
 * @file
 * @brief       pH OEM device driver
 * The sensor can be used with multiple configurations, for example with
 * interrupt and enable pin, only interrupt or enable pin, or the default
 * configuration with none of these pins.
 * The configurations are seperated into 4 devices.The enable pin is a custom
 * PCB functionality and not included in the factory default sensor.
 *
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 */

#ifndef PH_OEM_H
#define PH_OEM_H

#include <stdint.h>

#include "periph/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Atlas Scientific pH OEM default SMBus/I2C address
 *
 */
#ifndef PH_OEM_I2C_ADDRESS
#define PH_OEM_I2C_ADDRESS    (0x65)
#endif

/**
 * @brief   Named return values
 */
enum {
    PH_OEM_OK      = 0,    /**< everything was fine */
    PH_OEM_NOI2C   = -1,   /**< I2C communication failed */
    PH_OEM_NODEV   = -2,   /**< no PH_OEM device found on the bus */
    PH_OEM_NODATA  = -3    /**< no data available */
};

/**
 * @brief  Atlas Scientfic pH OEM sensor interrupt + enable params
 */
typedef struct ph_oem_params {
    i2c_t i2c;              /**< I2C device the sensor is connected to */
    uint8_t addr;           /**< the slave address of the sensor on the I2C bus */
    gpio_t interrupt_pin;   /**< interrupt pin (GPIO_UNDEF if not connected) */
    gpio_t enable_pin;      /**< enable pin (GPIO_UNDEF if not connected) */
} ph_oem_params_t;

/**
 * @brief   pH_OEM device descriptor
 */
typedef struct ph_oem {
    ph_oem_params_t params;     /**< device driver configuration */
} ph_oem_t;

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
int ina219_init(ina219_t *dev, i2c_t i2c, uint8_t address);

/**
 * @brief   Write to calibration register
 *
 * @param[in]  dev          device descriptor of sensor to configure
 * @param[in]  calibration  calibration register settings, see data sheet
 *
 * @return                  0 on success
 * @return                  <0 on error
 */
int ina219_set_calibration(const ina219_t *dev, uint16_t calibration);

/**
 * @brief   Write to configuration register
 *
 * @param[in]  dev          device descriptor of sensor to configure
 * @param[in]  config       configuration register settings, see data sheet
 *
 * @return                  0 on success
 * @return                  <0 on error
 */
int ina219_set_config(const ina219_t *dev, uint16_t config);

/**
 * @brief   Read shunt voltage
 *
 * @param[in]  dev          device descriptor of sensor
 * @param[out] voltage      measured voltage across shunt resistor
 *
 * @return                  0 on success
 * @return                  <0 on error
 */
int ina219_read_shunt(const ina219_t *dev, int16_t *voltage);

/**
 * @brief   Read bus voltage register
 *
 * The bus voltage can be found in the most significant bits of the bus voltage
 * register, the lower three bits are flags/reserved.
 *
 * See the device data sheet for details.
 *
 * @param[in]  dev          device descriptor of sensor
 * @param[out] voltage      measured bus voltage
 *
 * @return                  0 on success
 * @return                  <0 on error
 */
int ina219_read_bus(const ina219_t *dev, int16_t *voltage);

/**
 * @brief   Read shunt current
 *
 * @param[in]  dev          device descriptor of sensor
 * @param[out] current      measured current through shunt resistor
 *
 * @return                  0 on success
 * @return                  <0 on error
 */
int ina219_read_current(const ina219_t *dev, int16_t *current);

/**
 * @brief   Read power consumption
 *
 * @param[in]  dev          device descriptor of sensor
 * @param[out] power        measured power consumption
 *
 * @return                  0 on success
 * @return                  <0 on error
 */
int ina219_read_power(const ina219_t *dev, int16_t *power);

#ifdef __cplusplus
}
#endif

#endif /* INA219_H */
/** @} */
