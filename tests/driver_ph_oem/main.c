/*
 * Copyright (C) 2019 University of Applied Sciences Emden / Leer
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       Test application for the Atlas Scientific pH OEM sensor driver
 *
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 *
 * @}
 */

#include <fmt.h>
#include "xtimer.h"
#include "timex.h"

#include "ph_oem.h"
#include "ph_oem_params.h"
#include "ph_oem_regs.h"

static ph_oem_t dev;

//static void interrupt_pin_cb(void *arg)
//{
//    (void)arg;
//    puts("\n[New Reading!]");
//}

int main(void)
{
    int16_t data;

    puts("Atlas Scientific pH OEM sensor driver test application\n");

    printf("Initializing pH OEM sensor at I2C_%i, address 0x%02x...",
           PH_OEM_PARAM_I2C, PH_OEM_PARAM_ADDR);
    xtimer_sleep(2);

    if (ph_oem_init(&dev, ph_oem_params) == PH_OEM_OK) {
        puts("[OK]");
    }
    else {
        puts("[Failed]");
        return -1;
    }

    /* Read Firmware Version. */
    if (ph_oem_read_firmware_version(&dev, &data) == PH_OEM_OK) {
        printf("pH OEM firmware version: %d\n", data);
    }

    /* Test LED state by turning LED off and on again */
    if (ph_oem_set_led_state(&dev, PH_OEM_LED_OFF) == PH_OEM_OK) {
        puts("pH OEM LED turned off");
        xtimer_sleep(2);
    }
    if (ph_oem_set_led_state(&dev, PH_OEM_LED_ON) == PH_OEM_OK) {
        puts("pH OEM LED turned on");
    }

    /* Test changing pH OEM I2C address to 0x66 and back to 0x65 in the sensor as
     * well as ph_oem_t dev
     */
    if (ph_oem_set_i2c_address(&dev, 0x66) == PH_OEM_OK) {
        printf("pH OEM I2C address changed to %x\n", dev.params.addr);
        xtimer_sleep(1);
    }
    if (ph_oem_set_i2c_address(&dev, 0x65) == PH_OEM_OK) {
        printf("pH OEM I2C address changed to %x\n", dev.params.addr);
        xtimer_sleep(2);
    }

    /* Test calibration and if they are correctly applied in the pH OEM */
//    ph_oem_clear_calibration(&dev);
    ph_oem_read_calibration_state(&dev, &data);
    printf("Calibration state value is: %d\n", data);
//
//    ph_oem_set_calibration(&dev, 7002, PH_OEM_CALIBRATE_MID_POINT);
//    ph_oem_read_calibration_state(&dev, &data);
//    printf("Calibration state value is: %d\n", data);
//
//    ph_oem_set_calibration(&dev, 3000, PH_OEM_CALIBRATE_LOW_POINT);
//    ph_oem_read_calibration_state(&dev, &data);
//    printf("Calibration state value is: %d\n", data);
//
//    ph_oem_set_calibration(&dev, 10000, PH_OEM_CALIBRATE_HIGH_POINT);
//    ph_oem_read_calibration_state(&dev, &data);
//    printf("Calibration state value is: %d\n", data);
//
//    ph_oem_set_calibration(&dev, 7002, PH_OEM_CALIBRATE_MID_POINT);
//    ph_oem_read_calibration_state(&dev, &data);
//    printf("Calibration state value is: %d\n", data);
//
//    ph_oem_clear_calibration(&dev);
//	ph_oem_read_calibration_state(&dev, &data);
//	printf("Calibration state value is: %d\n", data);
//

    while (1) {
        /* After powering on the device, it takes a while for the pH circuit
         * to stabilize. In my tests the value stabilized after around 10 minutes.
         * Its probably only a good idea to turn the device off, if you need
         * a reading every couple hour or so. Best would be to leave the sensor
         * enabled and in hibernate mode.
         */
//        if (ph_oem_enable_device(&dev, true) == PH_OEM_OK) {
//            puts("pH OEM device turned on");
//            puts("Waiting 80 seconds for pH reading to stabilize...");
//            xtimer_sleep(120);
//        }

        float avg;
        for (int i = 0; i < 20; i++) {
            ph_oem_set_compensation(&dev, 2100);
            ph_oem_start_new_reading(&dev);
            ph_oem_read_ph(&dev, &data);
            printf("i = %d pH value raw: %d\n", i, data);
            avg += data;

            ph_oem_read_compensation(&dev, &data);
            printf("Reading was taken at a temperature of %d\n", data);

        }
        avg = (avg / 20) / 1000;

        printf("pH value avg float:\n");
        print_float(avg, 3);
        printf("\n");

//        if (ph_oem_enable_device(&dev, false) == PH_OEM_OK) {
//            puts("pH OEM device turned off");
//        }


        xtimer_sleep(240);
    }

    return 0;
}
