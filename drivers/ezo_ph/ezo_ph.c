/*
 * Copyright (C) 2017 Igor Knippenberg <igor.knippenberg@gmail.com>
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
 * @brief       Atlas Scientific EZO pH circuit device driver
 *
 * @author      Igor Knippenberg <igor.knippenberg@gmail.com>
 *
 * @}
 */

#include "ezo_ph.h"
#include "ezo_ph_params.h"

#include "periph/i2c.h"
#include "periph/gpio.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

#define I2C_SPEED I2C_SPEED_FAST

#define I2C (dev->params.i2c)
#define ADDR (dev->params.addr)


