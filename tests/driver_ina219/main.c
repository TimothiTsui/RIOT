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
 * @brief       Test application for the INA219 sensor driver
 *
 * @author      Joakim Nohlgård <joakim.nohlgard@eistec.se>
 *
 * @}
 */

#ifndef TEST_INA219_I2C
#error "TEST_INA219_I2C not defined"
#endif
#ifndef TEST_INA219_ADDR
#error "TEST_INA219_ADDR not defined"
#endif

#include <stdio.h>

#include "xtimer.h"
#include "ina219.h"

/* Use the following configuration:
 *
 *  - Continuous measurements, both shunt and bus voltage
 *  - +/- 320 mV Vshunt range
 *  - 32 V maximum bus voltage
 *  - 12 bit ADC resolution, no hardware averaging
 */
#define CONFIG   (INA219_MODE_CONTINUOUS_SHUNT_BUS | INA219_RANGE_320MV | \
                  INA219_BRNG_16V_FSR | INA219_SADC_12BIT | INA219_BADC_12BIT)
#define CALIBRATION (10240)
#define SLEEP    (100 * 20000U)

int main(void){
    ina219_t dev;
    int16_t val;

    puts("INA219 sensor driver test application\n");
    printf("Initializing I2C_%i... ", TEST_INA219_I2C);
    if(i2c_init_master(TEST_INA219_I2C, I2C_SPEED_FAST) < 0) {
        return -1;
    }

    printf("Initializing INA219 sensor at I2C_%i, address 0x%02x... ",
            TEST_INA219_I2C, TEST_INA219_ADDR);
    if(ina219_init(&dev, TEST_INA219_I2C, TEST_INA219_ADDR) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return 1;
    }
    puts("Set configuration register");
    if(ina219_set_config(&dev, CONFIG) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return 1;
    }

    puts("Set calibration register");
    if(ina219_set_calibration(&dev, CALIBRATION) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return 1;
    }

    while(1) {
        /* Read shunt resistor voltage, in millivolts */
        ina219_read_shunt(&dev, &val);
        printf("shunt: %6d mV", (val));

        /* Read VBUS voltage, in volts */
        ina219_read_bus(&dev, &val);
        /* The bus voltage is found in the topmost 13 bits of the bus voltage
         * register */
        val = (val >> INA219_BUS_VOLTAGE_SHIFT) * 4;
        printf("\tbus: %6d V", (val));


        /* Read current register milliampere, the scale depends on the value of the
         * calibration register */
        ina219_read_current(&dev, &val);
        printf("\tcurrent: %6d mA", (val / 25));

        /* Read power register in watts, the scale depends on the value of the
         * calibration register */
        ina219_read_power(&dev, &val);
        printf("\tpower: %6d mW\n", (val));

        xtimer_usleep(SLEEP);
    }

    return 0;
}
