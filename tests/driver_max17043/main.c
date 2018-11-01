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

#ifndef TEST_MAX17043_I2C
#error "TEST_MAX17043_I2C not defined"
#endif
#ifndef TEST_MAX17043_ADDR
#error "TEST_MAX17043_ADDR not defined"
#endif

#include <fmt.h>

#include "xtimer.h"
#include <max17043.h>

#define SLEEP    (100 * 10000U)

int main(void){
    max17043_t dev;

    uint8_t percent;
    uint8_t percent_decimal;
    uint16_t voltage;

    puts("MAX17043 sensor driver test application\n");
    printf("Initializing I2C_%i... ", TEST_MAX17043_I2C);
    if(i2c_init_master(TEST_MAX17043_I2C, I2C_SPEED_FAST) < 0) {
        return -1;
    }

    printf("Initializing MAX17043 sensor at I2C_%i, address 0x%02x... ",
    TEST_MAX17043_I2C, TEST_MAX17043_ADDR);
    if(max17043_init(&dev, TEST_MAX17043_I2C, TEST_MAX17043_ADDR) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return 1;
    }

//    /* Power-On reset */
//    int ret_val = max17043_reset(&dev);
//
//    if(ret_val == -1) {
//        puts("write failed");
//    }
//    else {
//        printf("write success: %d\n", ret_val);
//    }
//
//    xtimer_usleep(SLEEP);
//
//    /* Making a quick start */
//    ret_val = max17043_set_quick_start(&dev);
//
//    if(ret_val == -1) {
//        puts("write failed");
//    }
//    else {
//        printf("write success: %d\n", ret_val);
//    }

    while(1) {
        /* Read state of charge. MSB is percent,
         * LSB is percent_decimal. LSB 1/256 = 0.003906 precision */
        max17043_read_soc(&dev, &percent, &percent_decimal);
        puts("State of charge:");
        print_float(percent + (percent_decimal * 0.003906), 3);
        puts("% \n");

        /* Read cell voltage, in millivolts */
        max17043_read_cell_voltage(&dev, &voltage);
        printf("Voltage:\n");
        printf("%dmV \n\n", voltage);
        xtimer_usleep(SLEEP);
    }

    //max17043_set_sleep(&dev);

    return 0;
}
