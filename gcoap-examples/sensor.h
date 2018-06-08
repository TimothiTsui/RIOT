/*
 * sensor.h
 *
 *  Created on: 8 Jun 2018
 *      Author: riot
 */

#ifndef GCOAP_EXAMPLES_SENSOR_H_
#define GCOAP_EXAMPLES_SENSOR_H_

#include <stdint.h>

#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/nanocoap.h"
#include "xtimer.h"

uint16_t sensor_get_refresh(void);

uint16_t sensor_get_accuracy(void);

uint16_t sensor_get_temp(void);

void sensor_set_refresh(uint16_t refresh);

void sensor_set_accuracy(uint16_t accuracy);

int sensor_init(void);

void set_pid(kernel_pid_t pid);



#endif /* GCOAP_EXAMPLES_SENSOR_H_ */
