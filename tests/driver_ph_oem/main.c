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

//#include <fmt.h>
#include "xtimer.h"
#include "timex.h"
#include "ph_oem.h"

#include "ph_oem_params.h"
#include "ph_oem_regs.h"

#define SLEEP    (1000 * US_PER_MS)

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

    if (ph_oem_init(&dev, ph_oem_params) == PH_OEM_OK) {
        puts("[OK]");
    }
    else {
        puts("[Failed]");
        return -1;
    }

    /* Read Firmware Version. */
    ph_oem_read_firmware_version(&dev, &data);
    printf("pH OEM firmware version: %d\n", data);

    /* Test LED state by turning LED of and on again */
    if (ph_oem_set_led_state(&dev, PH_OEM_LED_OFF) == PH_OEM_OK) {
        puts("pH OEM LED turned off");
    }
    xtimer_sleep(2);
    if (ph_oem_set_led_state(&dev, PH_OEM_LED_ON) == PH_OEM_OK) {
        puts("pH OEM LED turned on");
    }
    xtimer_sleep(1);

    while (1) {
        ph_oem_start_new_reading(&dev);
        ph_oem_read_ph(&dev, &data);
        printf("pH value raw: %d\n", data);
        xtimer_sleep(5);
    }

    return 0;
}
