/*
 * lsm6dsl_utility.h
 *
 *  Created on: 13.03.2020
 *      Author: riot
 */

#ifndef EXAMPLES_THESIS_LSM6DSL_UTILITY_H_
#define EXAMPLES_THESIS_LSM6DSL_UTILITY_H_

#include "lsm6dsl_params.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define BUS         (dev->params.i2c)
#define ADDR        (dev->params.addr)

void print_binary(int value, int length);

void lsm6dsl_print_acc(lsm6dsl_t *dev);

void lsm6dsl_print_gyro(lsm6dsl_t *dev);

void lsm6dsl_print_temp(lsm6dsl_t *dev);

int lsm6dsl_write_reg(lsm6dsl_t *dev, uint16_t addr, uint8_t value);

int lsm6dsl_read_reg(lsm6dsl_t *dev, uint16_t addr, uint8_t *var);


#endif /* EXAMPLES_THESIS_LSM6DSL_UTILITY_H_ */
