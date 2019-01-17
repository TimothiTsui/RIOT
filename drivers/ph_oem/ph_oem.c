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
 * @brief       pH OEM device driver
 *
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com> *
 * @}
 */

//#include "stdint.h"
//#include "fmt.h"
//#include "bitfield.h"
//#include "byteorder.h"
#include "assert.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#include "ph_oem.h"
#include "ph_oem_params.h"
#include "ph_oem_regs.h"

#define ENABLE_DEBUG    (0)
#include "debug.h"

#define I2C (dev->params.i2c)
#define ADDR (dev->params.addr)

//static int _ph_oem_init_test(i2c_t i2c, uint8_t addr);
//


int ph_oem_init(ph_oem_t *dev, const ph_oem_params_t *params)
{
    assert(dev && params);

    dev->params = *params;

    //return _ph_oem_init_test(I2C, ADDR);
    return PH_OEM_OK;
}

//static int _ph_oem_init_test(i2c_t i2c, uint8_t addr)
//{
//    uint8_t regs[2];
//
//    i2c_acquire(i2c);
//
//    /* Register read test */
//    if (i2c_read_regs(i2c, addr, ADS101X_CONF_ADDR, &regs, 2, 0x0) < 0) {
//        DEBUG("[ads101x] init - error: unable to read reg %x\n",
//              ADS101X_CONF_ADDR);
//        i2c_release(i2c);
//        return ADS101X_NODEV;
//    }
//
//    regs[1] = (regs[1] & ~ADS101X_DATAR_MASK) | ADS101X_DATAR_3300;
//
//    /* Register write test */
//    if (i2c_write_regs(i2c, addr, ADS101X_CONF_ADDR, &regs, 2, 0x0) < 0) {
//        DEBUG("[ads101x] init - error: unable to write reg %x\n",
//              ADS101X_CONF_ADDR);
//        i2c_release(i2c);
//        return ADS101X_NODEV;
//    }
//
//    i2c_read_regs(i2c, addr, ADS101X_CONF_ADDR, &regs, 2, 0x0);
//
//    i2c_release(i2c);
//
//    /* Write should have actually written the register */
//    if ((regs[1] & ADS101X_DATAR_MASK) != ADS101X_DATAR_3300) {
//        DEBUG("[ads101x] init - error: unable to set reg (reg=%x)\n", regs[1]);
//        return ADS101X_NODEV;
//    }

//    return PH_OEM_OK;
//}

//int ph_oem_read_device_type(const ph_oem_t *dev, int16_t *device_type)
//{
//
//    return PH_OEM_OK;
//}
