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
 * @brief       Device driver for Atlas Scientific pH OEM Sensor with SMBus/I2C interface
 *
 * The Atlas Scientific pH OEM sensor can be used with multiple configurations,
 * for example with interrupt and enable pin, only interrupt or enable pin,
 * or the default configuration with none of these pins. Per default both these
 * pins are mapped to @ref GPIO_UNDEF if not otherwise defined.
 *
 * The enable pin is a custom PCB feature and not included in the factory
 * default sensor.
 *
 * The Sensor has no integrated temperature sensor and for the highest possible
 * precision it requires another device to provide the temperature for the compensation.
 *
 * Once the pH OEM is powered on it will be ready to receive commands and take
 * readings after 1ms. Communication is done using SMBus/I2C protocol at speeds
 * of 10-100 kHz.
 *
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 */

#ifndef PH_OEM_H
#define PH_OEM_H

#ifdef __cplusplus
extern "C"
{
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
typedef enum {
    PH_OEM_OK                   = 0,        /**< Everything was fine */
    PH_OEM_NOI2C                = -1,       /**< I2C communication failed */
    PH_OEM_NODEV                = -2,       /**< No PH_OEM device found on the bus */
    PH_OEM_NODATA               = -3,       /**< No data available */
    PH_OEM_NOT_PH               = -4,       /**< Not an Atlas Scientific pH OEM device */
    PH_OEM_INTERRUPT_GPIO_UNDEF = -5,       /**< Interrupt pin is @ref GPIO_UNDEF */
    PH_OEM_ENABLE_GPIO_UNDEF    = -6        /**< Enable pin is @ref GPIO_UNDEF */
} ph_oem_named_returns_t;

/**
 * @brief   LED state values
 */
typedef enum {
    PH_OEM_LED_ON   = true,                 /**< LED on state */
    PH_OEM_LED_OFF  = false,                /**< LED off state */
} ph_oem_led_state_t;

/**
 * @brief   Device state values
 */
typedef enum {
    PH_OEM_ACTIVE       = 0x01,             /**< Device active state */
    PH_OEM_HIBERNATE    = 0x00,             /**< Device hibernate state */
} ph_oem_device_state_t;
/**
 * @brief   Interrupt pin option values
 */
typedef enum {
    PH_OEM_IRQ_DISABLED = 0x00,             /**< Interrupt pin option disabled */
    PH_OEM_IRQ_RISING   = 0x02,             /**< Pin high on new reading (manually reset) */
    PH_OEM_IRQ_FALLING  = 0x04,             /**< Pin low on new reading (manually reset) */
    PH_OEM_IRQ_BOTH     = 0x08,             /**< Invert state on new reading (automatically reset) */
} ph_oem_irq_option_t;

/**
 * @brief   Calibration option values
 */
typedef enum {
    PH_OEM_CALIBRATE_LOW_POINT  = 0x02,     /**< Low point calibration option */
    PH_OEM_CALIBRATE_MID_POINT  = 0x03,     /**< Mid point calibration option */
    PH_OEM_CALIBRATE_HIGH_POINT = 0x04,     /**< High point calibration option */
} ph_oem_calibration_option_t;

/**
 * @brief   pH OEM sensor params
 */
typedef struct ph_oem_params {
    i2c_t i2c;              /**< I2C device the sensor is connected to */
    uint8_t addr;           /**< the slave address of the sensor on the I2C bus */
    gpio_t interrupt_pin;   /**< interrupt pin (@ref GPIO_UNDEF if not defined/connected) */
    gpio_t enable_pin;      /**< enable pin (@ref GPIO_UNDEF if not defined/connected) */
} ph_oem_params_t;

/**
 * @brief   pH OEM interrupt pin callback
 */
typedef void (*ph_oem_interrupt_pin_cb_t)(void *);

/**
 * @brief   pH OEM device descriptor
 */
typedef struct ph_oem {
    ph_oem_params_t params;         /**< device driver configuration */
    ph_oem_interrupt_pin_cb_t cb;   /**< interrupt pin callback */
    void *arg;                      /**< interrupt pin callback param */
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
 * @param[in]  dev               device descriptor
 * @param[out] firmware_version  read value
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_read_firmware_version(const ph_oem_t *dev,
                                 int16_t *firmware_version);

/**
 * @brief   Sets a new address for the pH OEM device by unlocking the
 *          @ref PH_OEM_REG_UNLOCK register and  writing a new address to
 *          the @ref PH_OEM_REG_ADDRESS register
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
 * @brief   Enable the interrupt if @ref ph_oem_params_t.interrupt_pin is
 *          defined
 *
 * @param[in] dev   device descriptor
 * @param[in] cb    callback called when the IRQ fires
 * @param[in] arg   callback argument
 *
 * @return zero on successful write, non zero on error
 */
int ph_oem_enable_interrupt(const ph_oem_t *dev, ph_oem_interrupt_pin_cb_t cb,
                            void *arg);

/**
 * @brief   Set the behavior of interrupt pin 7 of the pH OEM by setting the
 *          @ref PH_OEM_REG_INTERRUPT register.
 *          Pin is disabled by default.
 *
 *          The pin will not auto reset on option @ref PH_OEM_IRQ_RISING
 *          and @ref PH_OEM_IRQ_FALLING after IRQ occurs,
 *          so call this function again to reset, providing the same option
 *          (@ref PH_OEM_IRQ_RISING or @ref PH_OEM_IRQ_FALLING).
 *
 *          Settings are not retained if the power is cut, so you have to call
 *          this function again after powering on the device.
 *
 * @param[in] dev    device descriptor
 * @param[in] option @ref ph_oem_irq_option_t
 *
 * @return zero on successful write, non zero on error
 */
int ph_oem_set_interrupt(const ph_oem_t *dev, ph_oem_irq_option_t option);

/**
 * @brief   Set the LED state of the pH OEM sensor by writing to the
 *          @ref PH_OEM_REG_LED register
 *
 * @param[in] dev   device descriptor
 * @param[in] state @ref PH_OEM_LED_ON  = On and blinking each time a reading is taken<br>
 *                  @ref PH_OEM_LED_OFF = LED off
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_set_led_state(const ph_oem_t *dev, ph_oem_led_state_t state);

/**
 * @brief   Sets the device state (active/hibernate) of the pH OEM sensor by
 *          writing to the @ref PH_OEM_REG_HIBERNATE register.
 *          Once the device has been woken up it will continuously take readings
 *          every 420ms. Waking the device is the only way to take a reading.
 *          Hibernating the device is the only way to stop taking readings.
 *
 * @param[in] dev   device descriptor
 * @param[in] state @ref ph_oem_device_state_t
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_set_device_state(const ph_oem_t *dev, ph_oem_device_state_t state);

/**
 * @brief   Reads the @ref PH_OEM_REG_NEW_READING register.
 *          This function is for applications where the interrupt output pin
 *          cannot be used and continuously polling the device would be the
 *          preferred method of identifying when a new reading is available.
 *
 *          When the device is powered on, the @ref PH_OEM_REG_NEW_READING
 *          register will equal 0. Once the device is placed into active mode
 *          and a reading has been taken, the @ref PH_OEM_REG_NEW_READING register
 *          will move from 0 to 1.
 *
 *          This register will never automatically reset itself to 0.
 *          The master must reset the register back to 0 each time.
 *
 * @param[in] dev   device descriptor
 *
 * @return zero when new reading available, -1 when no new reading available,
 *  non zero or -1 on error
 */
int ph_oem_new_reading_available(const ph_oem_t *dev);

/**
 * @brief   Clears all calibrations previously done
 *
 * @param[in] dev   device descriptor
 *
 * @return zero on successful read, non zero on error
 */
int ph_oem_clear_calibration(const ph_oem_t *dev);

/**
 * @brief   Sets the @ref PH_OEM_REG_CALIBRATION_BASE register to the
 *          calibration_value which the pH OEM sensor will be
 *          calibrated to. Multiply the floating point calibration value of your
 *          solution by 1000 e.g. pH calibration solution = 7.002 * 1000 = 7002 = 0x00001B5A <br>
 *          The calibration value will be saved based on the given
 *          @ref ph_oem_calibration_option_t and retained after the power is cut.
 *
 * @param[in] dev                 device descriptor
 * @param[in] calibration_value   pH value multiplied by 1000 e.g 7.002 * 1000 = 7002
 * @param[in] option              @ref ph_oem_calibration_option_t
 *
 * @return zero if successfully cleared, non zero on error
 */
int ph_oem_set_calibration(const ph_oem_t *dev, uint16_t calibration_value,
                           ph_oem_calibration_option_t option);

/**
 * @brief   Read the @ref PH_OEM_REG_CALIBRATION_CONFIRM register.
 *          After a calibration event has been successfully carried out, the
 *          calibration confirmation register will reflect what calibration has
 *          been done, by setting bits 0 - 2.
 *
 *
 * @param[in]  dev                 device descriptor
 * @param[out] calibration_state   calibration state reflected by bits 0 - 2 <br>
 *                                 (0 = low, 1 = mid, 2 = high)
 *
 * @return zero if successfully cleared, non zero on error
 */
int ph_oem_read_calibration_reg(const ph_oem_t *dev, int16_t *calibration_state);

/**
 * @brief   Sets the @ref PH_OEM_REG_TEMP_COMPENSATION_BASE register to the
 *          temperature_compensation value which the pH OEM sensor will use
 *          to compensate the reading error.
 *          Multiply the floating point temperature value by 100
 *          e.g. temperature in degree Celsius = 34.26 * 100 = 3426 <br>
 *
 *          The temperature compensation will not be retained if the power is cut
 *
 *
 * @param[in] dev                        device descriptor
 * @param[in] temperature_compensation   temperature value in Celsius multiplied <br>
 *                                       by 100 e.g 34.26 * 100 = 3426
 *
 * @return zero if successfully cleared, non zero on error
 */
int ph_oem_set_compensation(const ph_oem_t *dev,
                            uint16_t temperature_compensation);

/**
 * @brief   Reads the @ref PH_OEM_REG_TEMP_CONFIRMATION_BASE register to verify
 *          the temperature compensation value that was used to take the pH
 *          reading is set to the correct temperature.
 *
 *          Divide the read temperature value by 100 to get the floating point
 *          e.g. temperature raw = 3426 / 100 = 34.26 *
 *
 * @param[in]  dev                       device descriptor
 * @param[out] temperature_compensation  raw temperature compensation value <br>
 *                                       Devide by 100 for floating point <br>
 *                                       e.g 3426 / 100 = 34.26
 *
 * @return zero if successfully cleared, non zero on error
 */
int ph_oem_read_compensation(const ph_oem_t *dev,
                             int16_t *temperature_compensation);

/**
 * @brief   Reads the @ref PH_OEM_REG_PH_READING_BASE register to get the current
 * 			pH reading.
 *
 *          Divide the raw pH value by 1000 to get the floating point
 *          e.g. pH raw = 8347 / 1000 = 8.347
 *
 *          The temperature compensation will not be retained if the power is cut
 *
 *
 * @param[in]  dev        device descriptor
 * @param[out] ph_value   raw pH value <br>
 *                        Devide by 1000 for floating point <br>
 *                        e.g 8347 / 1000 = 8.347
 *
 * @return zero if successfully cleared, non zero on error
 */
int ph_oem_read_ph(const ph_oem_t *dev,
                             int16_t *ph_value);



#ifdef __cplusplus
}
#endif

#endif /* PH_OEM_H */
/** @} */
