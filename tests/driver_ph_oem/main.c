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

#define SLEEP    (5000 * US_PER_MS)

static ph_oem_t dev;

int main(void)
{
    int16_t data;
    uint8_t state = 0x00;
    uint8_t count = 0;

    puts("Atlas Scientific pH OEM sensor driver test application");

    printf("Initializing pH OEM sensor at I2C_%i, address 0x%02x... \n",
           PH_OEM_PARAM_I2C, PH_OEM_PARAM_ADDR);

    if (ph_oem_init(&dev, ph_oem_params) == PH_OEM_OK) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return -1;
    }

    /* Read Firmware Version. */
    ph_oem_read_firmware_version(&dev, &data);
    printf("pH OEM firmware version: %d\n", data);

    while (1) {
        if (count < 5) {
            if (state == 0x00) {
                if (ph_oem_set_led_on(&dev, 0x01) == PH_OEM_OK) {
                    puts("pH OEM LED turned on\n");
                    state = 0x01;
                }
            }
            else if (state == 0x01) {
                if (ph_oem_set_led_on(&dev, 0x00) == PH_OEM_OK) {
                    puts("pH OEM LED turned off\n");
                    state = 0x00;
                }
            }
            count++;
        }
        xtimer_usleep(SLEEP);
    }

    return 0;
}
