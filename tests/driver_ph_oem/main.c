/*
 * Copyright (C) 2019 Igor Knippenberg
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

//#include <fmt.h>

#include "xtimer.h"
#include "timex.h"
#include "ph_oem.h"

#include "ph_oem_params.h"
#include "ph_oem_regs.h"

#define SLEEP    (100 * US_PER_MS)

static ph_oem_t dev;

int main(void){

//    int16_t data;

    puts("Atlas Scientific pH OEM sensor driver test application\n");
    printf("Initializing I2C_%i... ", PH_OEM_PARAM_I2C);

    printf("Initializing pH OEM sensor at I2C_%i, address 0x%02x... ",
    PH_OEM_PARAM_I2C, PH_OEM_PARAM_ADDR);

    if(ph_oem_init(&dev, ph_oem_params) == PH_OEM_OK) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return -1;
    }

//    while(1) {
        /* Read state of charge. MSB is percent,
         * LSB is percent_decimal. LSB 1/256 = 0.003906 precision */
//        max17043_read_soc(&dev, &percent, &percent_decimal);
//        puts("State of charge:");
//        print_float(percent + (percent_decimal * 0.003906), 3);
//        puts("% \n");
//
//        /* Read cell voltage, in millivolts */
////        max17043_read_cell_voltage(&dev, &voltage);
////        printf("Voltage:\n");
////        printf("%dmV \n\n", voltage);
//        xtimer_usleep(SLEEP);
//    }

    //max17043_set_sleep(&dev);

    return 0;
}
