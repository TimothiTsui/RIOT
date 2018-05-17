/*
 * Copyright (C) 2015 Eistec AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_max17043
 * @{
 *
 * @file
 * @brief       Register definitions for Texas Instruments max17043 High or Low
 *              Side, Bi-Directional CURRENT/POWER MONITOR with Two-Wire
 *              Interface
 *
 * @author      Dhruv Verma <dhruv2scs@gmail.com>
 */

#ifndef max17043_REGS_H
#define max17043_REGS_H


#ifdef __cplusplus
 extern "C" {
#endif


/**
 * @brief max17043 register addresses
 *
 * All registers in the max17043 are 16 bit wide and transmitted MSB first.
 */
typedef enum max17043_reg {
    MAX17043_REG_WRITE         = 0x6C,
    MAX17043_REG_READ          = 0x6D,
    MAX17043_REG_VCELL         = 0x02, /**< reports measurement of battery voltage (read only) */
    MAX17043_REG_SOC           = 0x04, /**< reports battery voltage (read only) */
    MAX17043_REG_MODE          = 0x06, /**< sends special commands to IC (write only) */
    MAX17043_REG_VERSION       = 0x08, /**< returns IC version (read only) */
    MAX17043_REG_CONFIG        = 0x0C, /**< adjusts IC performance based on application condition (read/write) */
    MAX17043_REG_COMMAND       = 0xFE, /**< sends special commands to IC (write) */
} max17043_reg_t;


#ifdef __cplusplus
}
#endif

#endif /* max17043_REGS_H */
/** @} */
