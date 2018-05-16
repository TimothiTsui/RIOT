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
 * @author      Joakim Nohlgård <joakim.nohlgard@eistec.se>
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
#include "max17043.h"

#define SLEEP    (100 * 60000U)

int main(void){
    max17043_t dev;
    uint8_t val;

    puts("max17043 sensor driver test application\n");
    printf("Initializing I2C_%i... ", TEST_MAX17043_I2C);
    if(i2c_init_master(TEST_MAX17043_I2C, I2C_SPEED_FAST) < 0) {
        return -1;
    }

    printf("Initializing max17043 sensor at I2C_%i, address 0x%02x... ",
            TEST_MAX17043_I2C, TEST_MAX17043_ADDR);
    if(max17043_init(&dev, TEST_MAX17043_I2C, TEST_MAX17043_I2C) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return 1;
    }

    while(1) {

        /* Read cell voltage from vcel register */
        if (max17043_read_vcell(&dev, &val) == -1)
        {
            puts("read failed");
        }
        else
        {
            //val = (val >> 4);
            printf("Vcell: %6d V", (val));
        }

        xtimer_usleep(SLEEP);
    }

    return 0;
}
