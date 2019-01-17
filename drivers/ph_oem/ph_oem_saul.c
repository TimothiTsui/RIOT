/*
 * Copyright (C) 2019 Igor Knippenberg
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
 * @brief       pH OEM sensor adaption to the sensor/actuator abstraction layer
 *
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 *
 * @}
 */

#include <string.h>
#include <stdio.h>

#include "saul.h"
#include "ph_oem.h"
#include "include/ph_oem_regs.h"


//static int read_ph(const void *dev, phydat_t *res)
//{
//    const ads101x_t *mydev = dev;
//
//    /* Change the mux channel */
//    ads101x_set_mux_gain(mydev, mydev->params.mux_gain);
//
//    /* Read raw value */
//    if (ads101x_read_raw(mydev, res->val) < 0) {
//        return ECANCELED;
//    }
//
//    /* Special case for 2.048V */
//    /* (this is the fixed FSR of ADS1013 and ADS1113) */
//    if ((mydev->params.mux_gain & ADS101X_PGA_MASK)
//        == ADS101X_PGA_FSR_2V048) {
//
//        /* LSB == 62.5uV to LSB == 100uV */
//        *(res->val) = (int16_t)((CONV_TO_B10 * (int32_t)*(res->val)) >> 16);
//
//        /* 100uV == 2^-4 V */
//        res->unit = UNIT_V;
//        res->scale = -4;
//    }
//    else {
//        /* Otherwise let the user deal with it */
//        res->unit = UNIT_NONE;
//        res->scale = 0;
//    }

//    return 1;
//}

//const saul_driver_t ph_oem_saul_driver = {
//    .read = read_ph,
//    .write = saul_notsup,
//    .type = SAUL_SENSE_PH,
//};
