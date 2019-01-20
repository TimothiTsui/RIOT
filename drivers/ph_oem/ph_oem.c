/*
 * Copyright (C) 2019 University of Applied Sciences Emden / Leer
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
#include "xtimer.h"
#include "assert.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#include "ph_oem.h"
#include "ph_oem_params.h"
#include "ph_oem_regs.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

#define I2C (dev->params.i2c)
#define ADDR (dev->params.addr)

static int _ph_oem_init_test(i2c_t i2c, uint8_t addr);

int ph_oem_init(ph_oem_t *dev, const ph_oem_params_t *params)
{
    assert(dev && params);

    dev->params = *params;

    return _ph_oem_init_test(I2C, ADDR);
}

static int _ph_oem_init_test(i2c_t i2c, uint8_t addr)
{
    uint8_t reg_data;

    i2c_acquire(i2c);

    /* Register read test */
    if (i2c_read_regs(i2c, addr, PH_OEM_REG_DEVICE_TYPE, &reg_data, 1, 0x0) < 0) {
        DEBUG("[ph_oem] init - error: unable to read reg %x\n",
              PH_OEM_REG_DEVICE_TYPE);
        i2c_release(i2c);
        return PH_OEM_NODEV;
    }

    /* Test if the device ID of the attached pH OEM sensor equals the
     * value of the @ref PH_OEM_REG_DEVICE_TYPE register
     * */
    if (reg_data != PH_OEM_DEVICE_TYPE_ID) {
        DEBUG("[ph_oem] init - error: the attached device is not a pH OEM "
              "Sensor. Read Device Type ID: %i\n", reg_data);
        i2c_release(i2c);
        return PH_OEM_NOT_PH;
    }

    i2c_release(i2c);

    return PH_OEM_OK;
}

int ph_oem_read_firmware_version(const ph_oem_t *dev, int16_t *firmware_version)
{
    uint8_t reg_value;

    i2c_acquire(I2C);

    if (i2c_read_regs(I2C, ADDR, PH_OEM_REG_FIRMWARE_VERSION, &reg_value, 1,
                      0x0) < 0) {
        i2c_release(I2C);
        return PH_OEM_NODATA;
    }

    i2c_release(I2C);

    *firmware_version = (int16_t) reg_value;

    return PH_OEM_OK;
}


//int ph_oem_set_i2c_address(const ph_oem_t *dev, uint8_t addr)
//{
//
//
//    return PH_OEM_OK;
//}
//
//int ph_oem_enable_interrupt(const ph_oem_t *dev, ph_oem_interrupt_pin_cb_t cb,
//                            void *arg){
//	return PH_OEM_OK;
//}
//
//int ph_oem_set_interrupt(const ph_oem_t *dev, ph_oem_irq_option_t option)
//{
//
//    return PH_OEM_OK;
//}

int ph_oem_set_led_state(const ph_oem_t *dev, ph_oem_led_state_t state)
{

    i2c_acquire(I2C);
    i2c_write_regs(I2C, ADDR, PH_OEM_REG_LED, &state, 1, 0x0);

    i2c_release(I2C);

    return PH_OEM_OK;
}
//
//int ph_oem_enable_device(const ph_oem_t *dev, bool enabled){
//
//	return PH_OEM_OK;
//}

//int ph_oem_set_device_state(const ph_oem_t *dev, ph_oem_device_state_t state) {
//
//	return PH_OEM_OK;
//}
//
//int ph_oem_new_reading_available(const ph_oem_t *dev) {
//
//	return PH_OEM_OK;
//}
//
///**
// * @brief   Sets the @ref PH_OEM_REG_CALIBRATION_BASE register to the value
// *            which the pH OEM sensor will be calibrated to. Multiply the
// *            floating point calibration value of your solution by 1000
// *            e.g. pH calibration solution = 7.002 * 1000 = 7002 = 0x00001B5A
// *
// * @param[in] dev   device descriptor
// * @param[in] calibration_value   Calibration value
// *
// * @return zero on successful read, non zero on error
// */
//static int ph_oem_set_calibration_value(const ph_oem_t *dev, uint16_t calibration_value) {
//
//	return PH_OEM_OK;
//}
//
//int ph_oem_clear_calibration(const ph_oem_t *dev){
//
//	return PH_OEM_OK;
//}
//
//int ph_oem_set_calibration(const ph_oem_t *dev, uint16_t calibration_value,
//                     ph_oem_calibration_option_t option){
//	return PH_OEM_OK;
//}
//
//int ph_oem_read_calibration_reg(const ph_oem_t *dev, int16_t *calibration_state){
//
//	return PH_OEM_OK;
//}
//
//int ph_oem_set_compensation(const ph_oem_t *dev,
//                            uint16_t temperature_compensation){
//
//	return PH_OEM_OK;
//}
//
//int ph_oem_read_compensation(const ph_oem_t *dev,
//                             int16_t *temperature_compensation){
//
//	return PH_OEM_OK;
//}

int ph_oem_read_ph(const ph_oem_t *dev,
                   int16_t *ph_value)
{
    uint8_t reg_value[4];

    i2c_acquire(I2C);
    DEBUG("[ph_oem] ACTIVE\n");
    i2c_write_reg(I2C, ADDR, PH_OEM_REG_HIBERNATE, PH_OEM_ACTIVE, 0x0);

    xtimer_usleep(500 * US_PER_MS);

    if (i2c_read_regs(I2C, ADDR, PH_OEM_REG_PH_READING_BASE, &reg_value, 4,
                      0) < 0) {
        DEBUG("[ph_oem] HIBERNATE\n");
        i2c_write_reg(I2C, ADDR, PH_OEM_REG_HIBERNATE, PH_OEM_HIBERNATE, 0x0);
        i2c_release(I2C);
        return PH_OEM_NODATA;
    }

    *ph_value = (int16_t)(reg_value[2] << 8) | (int16_t)(reg_value[3]);
    DEBUG("[ph_oem] pH value raw %d\n", *ph_value);

    DEBUG("[ph_oem] HIBERNATE\n\n");
    i2c_write_reg(I2C, ADDR, PH_OEM_REG_HIBERNATE, PH_OEM_HIBERNATE, 0x0);

    i2c_release(I2C);

    return PH_OEM_OK;
}
