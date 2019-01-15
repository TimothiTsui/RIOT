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
#include <fmt.h>
#include <bitfield.h>
#include "max17043.h"
#include "periph/i2c.h"
#include "byteorder.h"
#include "include/max17043-regs.h"

#define ENABLE_DEBUG    (0)
#include "debug.h"

/** @brief Read one 16 bit register from a max17043 device and swap byte order, if necessary. */

int max17043_init(max17043_t *dev, i2c_t i2c, uint8_t address){
    /* write device descriptor */
    dev->i2c = i2c;
    dev->addr = address;

    return 0;
}

/** @brief Read one 16 bit register from a INA219 device and swap byte order, if necessary.*/
static int max17043_read_reg(const max17043_t *dev, uint8_t reg, uint16_t *out)
{
    union {
        uint8_t c[2];
        uint16_t u16;
    } tmp = { .u16 = 0 };
    int status = 0;

    i2c_acquire(dev->i2c);
    status = i2c_read_regs(dev->i2c, dev->addr, reg, &tmp.c[0], 2, 0);
    i2c_release(dev->i2c);

    if (status != 2) {
        return -1;
    }

    *out = ntohs(tmp.u16);

    return 0;
}

/** @brief Write one 16 bit register to a INA219 device and swap byte order, if necessary. */
static int max17043_write_reg(const max17043_t *dev, uint8_t reg, uint16_t in)
{
    union {
        uint8_t c[2];
        uint16_t u16;
    } tmp = { .u16 = 0 };
    int status = 0;

    tmp.u16 = htons(in);

    i2c_acquire(dev->i2c);
    status = i2c_write_regs(dev->i2c, dev->addr, reg, &tmp.c[0], 2, 0);
    i2c_release(dev->i2c);

    if (status != 2) {
        return -1;
    }

    return 0;
}

int max17043_read_soc(const max17043_t *dev, uint8_t *soc, uint8_t *soc_decimal){
    uint16_t result;
    int status = max17043_read_reg(dev, MAX17043_REG_SOC, &result);
    printf("soc raw 16bit: %x\n", result);
    *soc = (result & 0xFF00) >> 8;
    *soc_decimal = (result & 0x00FF);
    return status;
}

int max17043_read_cell_voltage(const max17043_t *dev, uint16_t *vcell){
    int status = max17043_read_reg(dev, MAX17043_REG_VCELL, vcell);

    *vcell = (*vcell >> 4) * 1.25;

    return status;
}

int max17043_set_quick_start(const max17043_t *dev){
    return max17043_write_reg(dev, MAX17043_REG_MODE, MAX17043_MODE_QUICK_START);
}

int max17043_reset(const max17043_t *dev)
{
    return max17043_write_reg(dev, MAX17043_REG_COMMAND, 0x5400);

}

int max17043_set_sleep(const max17043_t *dev){
    uint16_t config;
    max17043_read_reg(dev, MAX17043_REG_CONFIG, &config);
    printf("Config value: %x\n", config);

    config ^= 1 << 7;

    if(max17043_write_reg(dev, MAX17043_REG_CONFIG, config) == -1){
        printf("Write error config");
    }
    else{
        max17043_read_reg(dev, MAX17043_REG_CONFIG, &config);
        printf("Write successful: %x\n", config);
    }
    return 0;
}



