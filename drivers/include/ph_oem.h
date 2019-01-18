/*
 * Copyright (C) 2019 Igor Knippenberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_ph_oem PH OEM sensor device driver
 * @ingroup     drivers_sensors
 * @ingroup     drivers_saul
 * @brief       Device driver for Atlas Scientific pH OEM Sensor with SMBus/I2C interface
 *
 * @{
 *
 * @file
 * @brief       PH OEM sensor device driver
 *
 * The Atlas Scientific pH OEM sensor can be used with multiple configurations,
 * for example with interrupt and enable pin, only interrupt or enable pin,
 * or the default configuration with none of these pins. Per default both these
 * pins are mapped to GPIO_UNDEF if not otherwise defined.
 *
 * The enable pin is a custom PCB functionality and not included in the factory
 * default sensor.
 *
 * The Sensor has no integrated temperature sensor and for the highest possible
 * precision it requires another device to provide the temperature for the compensation.
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
 * @brief   Named argument values
 */
enum {
    PH_OEM_LED_ON   = true,    /**< LED on state */
    PH_OEM_LED_OFF	= false,	   /**< LED off state */
	PH_OEM_LED_OFF	= false
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
 * @brief   Read the @ref PH_OEM_REG_FIRMWARE_VERSION register value
 *
 * @param[in]  dev   			 device descriptor
 * @param[out] firmware_version  read value
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_read_firmware_version(const ph_oem_t *dev, int16_t *firmware_version);

/**
 * @brief   Sets a new address for the pH OEM device by unlocking the
 * 			@ref PH_OEM_REG_UNLOCK register and  writing a new address to
 * 			the @ref PH_OEM_REG_ADDRESS register
 *
 *			Settings are retained if the power is cut.
 *
 * @param[in] dev   device descriptor
 * @param[in] addr  new address for the device. Range: 0x01 - 0x7f
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_set_i2c_address(const ph_oem_t *dev, uint8_t addr);

/**
 * @brief   Adjusts the function of pin 7 (interrupt output pin) by setting
 * 			the @ref PH_OEM_REG_INTERRUPT register.
 *
 * 			Settings are not retained if the power is cut!
 *
 * @param[in] dev    device descriptor
 * @param[in] option 0 = disabled
 *                   2 = pin high on new reading (manually reset)
 *                   4 = pin low on new reading (manually reset)
 *                   8 = invert state on new reading (automatically reset)
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_set_interrupt_behaviour(const ph_oem_t *dev, uint8_t option);

/**
 * @brief   Sets the LED state of the pH OEM sensor by writing to the
 * 			@ref PH_OEM_REG_LED register
 *
 * @param[in] dev   device descriptor
 * @param[in] state True  = On and blinking each time a reading is taken
 *                  False = Off
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_set_led_state(const ph_oem_t *dev, bool state);

/**
 * @brief   Sets the device state (active/hibernate) of the pH OEM sensor by
 * 			writing to the @ref PH_OEM_REG_HIBERNATE register.
 *
 * @param[in] dev   device descriptor
 * @param[in] state True  = Blinking each time a reading is taken
 *                  False = off
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_set_device_state(const ph_oem_t *dev, bool state);


#ifdef __cplusplus
}
#endif

#endif /* PH_OEM_H */
/** @} */
