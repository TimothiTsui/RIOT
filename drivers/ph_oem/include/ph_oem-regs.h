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
 * @brief       Register definitions for Atlas Scientific pH Sensor.
 *              For detailed description of the register view the datasheet
 *
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 */

#ifndef PH_OEM_REGS_H
#define PH_OEM_REGS_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Atlas Scientific pH OEM register addresses
     *
     * All registers in the pH OEM are 8 bit wide and transmitted MSB first.
     */
    typedef enum ph_oem_reg
    {
        /**< Device Information */
        PH_OEM_REG_DEVICE_TYPE = 0x00, /**< contains a number indicating what type of the OEM device it is */
        PH_OEM_REG_FIRMWARE_VERSION = 0x01, /**< contains a number indicating what firmware version of the OEM device it is */
        /**< Device address */
        PH_OEM_REG_UNLOCK = 0x02, /**< unlock register to be able to change the I²C address */
        PH_OEM_REG_ADDRESS = 0x03, /**< Bus address of the device. Can be changed from 0x01-0x7F when unlocked */
        /**< Control */
        PH_OEM_REG_INTERRUPT = 0x04, /**< The Interrupt control register adjusts the function of pin 7 (the interrupt output pin) */
        PH_OEM_REG_LED = 0x05, /**< Register to turn the LED on or off. Not retained after power is cut*/
        PH_OEM_REG_HIBERNATE = 0x06, /**< Register is used to activate, or hibernate the sensing subsystem */
        PH_OEM_REG_NEW_READING = 0x07, /**< Indicates if a new reading is available (=1). Needs to be set to 0 manually  */
        /**< Calibration */
        PH_OEM_REG_CALIBRATION = 0x08 /**< Calibration Register. 0x08 MSB, 0x09 high byte, 0x0A low byte, 0x0B LSB  */

    } ph_oem_reg_t;

#ifdef __cplusplus
}
#endif

#endif /* PH_OEM_REGS_H */
/** @} */
