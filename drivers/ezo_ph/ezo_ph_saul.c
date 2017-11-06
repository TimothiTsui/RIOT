/*
 * Copyright (C) 2017 OTA keys S.A.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_ezo_ph
 * @{
 *
 * @file
 * @brief       Atlas Scientific EZO pH adaption to the RIOT actuator/sensor interface
 *
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 *
 * @}
 */

#include <string.h>
#include <stdio.h>

#include "saul.h"
#include "ezo_ph.h"

static int read_ph(const void *dev, phydat_t *res)
{
//    adcxx1c_read_raw((const adcxx1c_t *)dev, res->val);
//
//    res->unit = UNIT_NONE;
//    res->scale = 0;

    return 1;
}

const saul_driver_t ezo_ph_saul_driver = {
    .read = read_ph,
    .write = saul_notsup,
    .type = SAUL_SENSE_PH,
};
