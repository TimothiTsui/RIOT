/*
 * Copyright (C) 2018 University of Applied Science Emden/Leer
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_ina219
 * @{
 *
 * @file
 * @brief       Register definitions for Texas Instruments INA219 Zer�-Drift High
 *              Side, Bi-Directional CURRENT/POWER MONITOR with Two-Wire
 *              Interface
 *
 * @author      Dhruv Verma <dhruv2scs@gmail.com>
 */

#ifndef INA219_REGS_H
#define INA219_REGS_H


#ifdef __cplusplus
 extern "C" {
#endif


/**
 * @brief INA219 register addresses
 *
 * All registers in the INA219 are 16 bit wide and transmitted MSB first.
 */
typedef enum ina219_reg {
    INA219_REG_CONFIGURATION = 0x00, /**< Configuration register (read/write) */
    INA219_REG_SHUNT_VOLTAGE = 0x01, /**< Shunt voltage register (read only) */
    INA219_REG_BUS_VOLTAGE   = 0x02, /**< Bus voltage register (read only) */
    INA219_REG_POWER         = 0x03, /**< Power register (read only) */
    INA219_REG_CURRENT       = 0x04, /**< Current register (read only) */
    INA219_REG_CALIBRATION   = 0x05, /**< Calibration register (read/write) */
} ina219_reg_t;


#ifdef __cplusplus
}
#endif

#endif /* INA219_REGS_H */
/** @} */
