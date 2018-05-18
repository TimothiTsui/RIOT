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
//static int max17043_read_reg(const max17043_t *dev, uint8_t reg, uint16_t *out)
//{
//    union {
//        uint8_t c[2];
//        uint16_t u16;
//    } tmp = { .u16 = 0 };
//    int status = 0;
//
//    status = i2c_read_regs(dev->i2c, dev->addr, reg, &tmp.c[0], 2);
//
//    if (status != 2) {
//        return -1;
//    }
//
//    *out = ntohs(tmp.u16);
//    return 0;
//}
int max17043_read_soc(const max17043_t *dev, uint16_t *temp){

    uint8_t msb = 0;
    uint8_t lsb = 0;

    i2c_acquire(dev->i2c);
    i2c_write_byte(dev->i2c, dev->addr, MAX17043_REG_SOC);

    i2c_read_byte(dev->i2c, dev->addr, &msb);
    i2c_read_byte(dev->i2c, dev->addr, &lsb);

    i2c_release(dev->i2c);

//    uint16_t voltage = ((lsb|(msb << 8)) >> 4);
//    printf("msb: %x\n", voltage);
//    *voltage = (uint16_t)msb;
    *temp = ((uint16_t)msb * 1000) + lsb;


    return 0;
}

int max17043_read_vcell(const max17043_t *dev, uint16_t *temp){
    uint8_t msb = 0;
    uint8_t lsb = 0;
    //printf("address: %x\n", dev->addr);

    i2c_acquire(dev->i2c);
    i2c_write_byte(dev->i2c, dev->addr, MAX17043_REG_VCELL);

    i2c_read_byte(dev->i2c, dev->addr, &msb);
    i2c_read_byte(dev->i2c, dev->addr, &lsb);

    i2c_release(dev->i2c);

    //    uint16_t voltage = ((lsb|(msb << 8)) >> 4);
    //    printf("msb: %x\n", voltage);
    //    *voltage = (uint16_t)msb;
//    temp = ((uint16_t)msb << 8) | lsb;

    *temp = ((lsb | ((uint16_t)msb << 8)) >> 4);

    return 0;
}

//int max17043_read_vcell(const max17043_t *dev, int16_t *voltage)
//{
//    return max17043_read_reg(dev, MAX17043_REG_VCELL, (uint16_t *)voltage);
//}

