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
 * @brief       Device driver implementation for Texas Instruments max17043 High
 *              or Low Side, Bi-Directional CURRENT/POWER MONITOR with Two-Wire
 *              Interface
 *
 * @author      Joakim Nohlgård <joakim.nohlgard@eistec.se>
 *
 * @}
 */

#include <stdint.h>

#include "max17043.h"
#include "periph/i2c.h"
#include "byteorder.h"
#include "include/max17043-regs.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

/** @brief Read one 16 bit register from a max17043 device and swap byte order, if necessary. */
static int max17043_read_reg(const max17043_t *dev, uint8_t reg, uint8_t *out)
{
//    union {
//        uint8_t c[2];
//        uint16_t u16;
//    } tmp = { .u16 = 0 };
    int status = 0;

    uint8_t test = 0;

    //status = i2c_read_regs(dev->i2c, dev->addr, reg, &tmp.c[0], 2);
    status = i2c_read_reg(dev->i2c, dev->addr, reg, &test);

    DEBUG("status %d , tmp: %d\n", status, test);

    if (status != 2) {
        return -1;
    }

    *out = test;
    return 0;
}

/** @brief Write one 16 bit register to a max17043 device and swap byte order, if necessary. */
//static int max17043_write_reg(const max17043_t *dev, uint8_t reg, uint16_t in)
//{
//    union {
//        uint8_t c[2];
//        uint16_t u16;
//    } tmp = { .u16 = 0 };
//    int status = 0;
//
//    tmp.u16 = htons(in);
//
//    status = i2c_write_regs(dev->i2c, dev->addr, reg, &tmp.c[0], 2);
//
//    if (status != 2) {
//        return -1;
//    }
//
//    return 0;
//}


int max17043_init(max17043_t *dev, i2c_t i2c, uint8_t address)
{
    /* write device descriptor */
    dev->i2c = i2c;
    dev->addr = address;

    return 0;
}

int max17043_read_vcell(const max17043_t *dev, uint8_t *vcell)
{
    return max17043_read_reg(dev, MAX17043_REG_VCELL, (uint8_t *)vcell);
}

/*int max17043_read_soc(const max17043_t *dev, uint16_t config)
{
    return max17043_read_reg(dev, MAX17043_REG_SOC , config);
}

int max17043_set_mode(const max17043_t *dev, uint16_t config)
{
    return max17043_write_reg(dev, MAX17043_REG_MODE, config);
}

int max17043_set_config(const max17043_t *dev, uint16_t config)
{
    return max17043_write_reg(dev, MAX17043_REG_CONFIG, config);
}*/




