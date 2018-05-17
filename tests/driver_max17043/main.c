/*
 * Copyright (C) 2015 Eistec AB
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
 * @brief       Test application for the max17043 sensor driver
 *
 * @author      Dhruv Verma <dhruv2scs@gmail.com>
 *
 * @}
 */

#ifndef TEST_max17043_I2C
#error "TEST_max17043_I2C not defined"
#endif
#ifndef TEST_max17043_ADDR
#error "TEST_max17043_ADDR not defined"
#endif

#include <stdio.h>

#include "xtimer.h"
#include "max17043.h"

#define CONFIG   (MAX17043_CONFIG_DEFAULT)

#define SLEEP    (100 * 20000U)

int main(void){
    max17043_t dev;
    int16_t val;

    puts("max17043 sensor driver test application\n");
    printf("Initializing I2C_%i... ", TEST_max17043_I2C);
    if(i2c_init_master(TEST_max17043_I2C, I2C_SPEED_FAST) < 0) {
        return -1;
    }

    printf("Initializing max17043 sensor at I2C_%i, address 0x%02x... ",
            TEST_max17043_I2C, TEST_max17043_ADDR);
    if(max17043_init(&dev, TEST_max17043_I2C, TEST_max17043_ADDR) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return 1;
    }
    puts("Set configuration register");
    if(max17043_set_config(&dev, CONFIG) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return 1;
    }

//    puts("Set calibration register");
//    if(max17043_set_calibration(&dev, CALIBRATION) == 0) {
//        puts("[OK]\n");
//    }
//    else {
//        puts("[Failed]");
//        return 1;
//    }

    while(1) {
        printf("Hallo!");
        /* Read shunt resistor voltage, in millivolts */
        max17043_read_vcell(&dev, &val);
        val = (val >> 4);
        printf("Vcell: %6d mV", (val));
//
//        /* Read VBUS voltage, in volts */
////        max17043_read_soc(&dev, &val);
////        printf("\tsoc: %6d", (val));
//        /* The bus voltage is found in the topmost 13 bits of the bus voltage
//         * register */
//
//
//
//        /* Read current register milliampere, the scale depends on the value of the
//         * calibration register */
////        max17043_read_current(&dev, &val);
////        printf("\tcurrent: %6d mA", (val / 25));
////
////        /* Read power register in watts, the scale depends on the value of the
////         * calibration register */
////        max17043_read_power(&dev, &val);
////        printf("\tpower: %6d mW\n", (val));
//
       xtimer_usleep(SLEEP);
    }

    return 0;
}
