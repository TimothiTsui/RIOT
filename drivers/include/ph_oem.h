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
 * The Atlas Scientific pH OEM sensor can be used with multiple configurations,
 * for example with interrupt and enable pin, only interrupt or enable pin,
 * or the default configuration with none of these pins. Per default both these
 * pins are mapped to GPIO_UNDEF if not otherwise defined.
 *
 * The enable pin is a custom PCB functionality and not included in the factory
 * default sensor.
 *
 * The Sensor has no integrated temperature sensor and requires another device
 * to provide the temperature for compensation.
 *
 *
 *
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 */

#ifndef PH_OEM_H
#define PH_OEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "periph/i2c.h"
#include "periph/gpio.h"

/**
 * @brief    pH OEM default SMBus/I2C address
 *
 */
#ifndef PH_OEM_I2C_ADDRESS
#define PH_OEM_I2C_ADDRESS    (0x65)
#endif

/**
 * @brief   Named return values
 */
enum {
    PH_OEM_OK       = 0,    /**< everything was fine */
    PH_OEM_NOI2C    = -1,   /**< I2C communication failed */
    PH_OEM_NODEV    = -2,   /**< no PH_OEM device found on the bus */
    PH_OEM_NODATA   = -3,   /**< no data available */
    PH_OEM_NOT_PH   = -4    /**< not an Atlas Scientific pH OEM device */
};

/**
 * @brief   pH OEM sensor params
 */
typedef struct ph_oem_params {
    i2c_t i2c;              /**< I2C device the sensor is connected to */
    uint8_t addr;           /**< the slave address of the sensor on the I2C bus */
    gpio_t interrupt_pin;   /**< interrupt pin (GPIO_UNDEF if not connected) */
    gpio_t enable_pin;      /**< enable pin (GPIO_UNDEF if not connected) */
} ph_oem_params_t;

/**
 * @brief   pH OEM device descriptor
 */
typedef struct ph_oem {
    ph_oem_params_t params;     /**< device driver configuration */
} ph_oem_t;

/**
 * @brief   Initialize an pH OEM sensor
 *
 * @param[in,out] dev  device descriptor
 * @param[in] params   device configuration
 *
 * @return zero on successful initialization, non zero on error
 */
int ph_oem_init(ph_oem_t *dev, const ph_oem_params_t *params);

/**
 * @brief   Read the firmware version register value
 *
 * @param[in] dev   device descriptor
 * @param[out] raw  read value
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_read_firmware_version(const ph_oem_t *dev, int16_t *firmware_version);

/**
 * @brief   Sets the unlock register to be able to change the I2C address
 *
 * @param[in] dev   device descriptor
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_set_unlock(const ph_oem_t *dev);

/**
 * @brief   Sets a new address for the pH OEM device
 *
 * @param[in] dev   device descriptor
 * @param[in] addr  new address for the device. Range: 0x01 - 0x7f
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_set_i2c_address(const ph_oem_t *dev, uint8_t addr);

/**
 * @brief   Sets the led on if state true, or off if state false.
 *
 * @param[in] dev   device descriptor
 * @param[in] state True  = Blinking each time a reading is taken
 *                  False = off
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_set_led_on(const ph_oem_t *dev, uint8_t state);



#ifdef __cplusplus
}
#endif

#endif /* PH_OEM_H */
/** @} */
