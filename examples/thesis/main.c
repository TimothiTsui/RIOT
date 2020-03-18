/*
 * Copyright (C) 2017 OTA keys S.A.
 *               2017 HAW Hamburg
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
 * @brief       Test application for the LSM6DSL accelerometer/gyroscope driver.
 *
 * @author      Vincent Dupont <vincent@otakeys.com>
 * @author      Sebastian Meiling <s@mlng.net>
 *
 * @}
 */

//std
#include <stdio.h>
#include <stdlib.h>

//Driver
#include "xtimer.h"
#include "lsm6dsl.h"
#include "lsm6dsl_params.h"
#include "lsm6dsl_internal.h"

//Project
#include "lsm6dsl_utility.h"
#include "integration.h"
#include "calculate_distance.h"

#define SLEEP_USEC  (500UL * US_PER_MS)

//Count of the samples used
#define SAMPLE_LIMIT 20
#define WATERMARK_VALUE (SAMPLE_LIMIT * 2 * 3)

//Postition of watermark bit
#define WATERMARK_BIT 0b10000000

/**
 * order in arry [0, 1, 2, 3] is
 * LSM6DSL_ACC_FS_2G, LSM6DSL_ACC_FS_16G, LSM6DSL_ACC_FS_4G, LSM6DSL_ACC_FS_8G
 */
static const int16_t range_acc[] = { 2000, 16000, 4000, 8000 };
/**
 * order in arry [0, 1, 2, 3] is
 * LSM6DSL_GYRO_FS_245DPS, LSM6DSL_GYRO_FS_500DPS,
 * LSM6DSL_GYRO_FS_1000DPS, LSM6DSL_GYRO_FS_2000DPS
 */
static const int16_t range_gyro[] = { 2450, 5000, 10000, 20000 };

#define LSM6DSL_FIFO_CTRL5_BYPASS_MODE                  0b000
#define LSM6DSL_FIFO_CTRL5_FIFO_MODE                    0b001
#define LSM6DSL_FIFO_CTRL5_CONTINUOUS_TO_FIFO_MODE      0b011
#define LSM6DSL_FIFO_CTRL5_BYPASS_TO_CONTINUOUS_MODE    0b100

int lsm6dsl_set_config(lsm6dsl_t *dev)
{

    int res = 0;
    uint8_t tmp;

    i2c_acquire(dev->params.i2c);

    /* Set BDU */
    i2c_read_reg(BUS, ADDR, LSM6DSL_REG_CTRL3_C, &tmp, 0);

    //BDU, Block Data Update until read
    tmp |= 0b1000000;
    i2c_write_reg(BUS, ADDR, LSM6DSL_REG_CTRL3_C, tmp, 0);


    /* Set Filter */
    //LPF2_XL_EN, Low pass path
    tmp = 0b10000000;

    //INPUT_COMPOSITE, ODR/2 : ODR/4
    //tmp |= 0b1000;
    //HP_SLOPE_XL_EN, High pass path
    tmp |= 0b100;
    //HPCF_XL[1:0]
    tmp |=  0b11 << 5;
    //tmp = 0;
    res += i2c_write_reg(dev->params.i2c, dev->params.addr,
                         LSM6DSL_REG_CTRL8_XL, tmp, 0);

    tmp = (dev->params.acc_odr << LSM6DSL_CTRL_ODR_SHIFT) |
          (dev->params.acc_fs << LSM6DSL_CTRL_FS_SHIFT);
    //LPF1_BW_SEL, ODR/2 : ODR/4
    tmp |= 0b10;
    tmp = 0;
    //res += i2c_write_reg(dev->params.i2c, dev->params.addr, LSM6DSL_REG_CTRL1_XL, tmp, 0);


    /* Set FIFO Watermark */
    tmp = (uint8_t)WATERMARK_VALUE;
    res += i2c_write_reg(BUS, ADDR, LSM6DSL_REG_FIFO_CTRL1, tmp, 0);
    tmp = WATERMARK_VALUE >> 8;
    res += i2c_write_reg(BUS, ADDR, LSM6DSL_REG_FIFO_CTRL2, tmp, 0);

    //fifo default odr:         00100110
    //fifo default decimation:  00001001
    /* Set FIFO mode */
    uint8_t fifo_odr = MAX(dev->params.acc_odr, dev->params.gyro_odr);
    //tmp = (fifo_odr << LSM6DSL_FIFO_CTRL5_FIFO_ODR_SHIFT) | LSM6DSL_FIFO_CTRL5_CONTINUOUS_MODE;

    tmp = (fifo_odr << LSM6DSL_FIFO_CTRL5_FIFO_ODR_SHIFT) |
          LSM6DSL_FIFO_CTRL5_BYPASS_MODE;
    res += i2c_write_reg(BUS, ADDR, LSM6DSL_REG_FIFO_CTRL5, tmp, 0);

    tmp = (fifo_odr << LSM6DSL_FIFO_CTRL5_FIFO_ODR_SHIFT) |
          LSM6DSL_FIFO_CTRL5_FIFO_MODE;
    res += i2c_write_reg(BUS, ADDR, LSM6DSL_REG_FIFO_CTRL5, tmp, 0);

    /* Set decimation */
    //tmp = (dev->params.gyro_decimation << LSM6DSL_FIFO_CTRL3_GYRO_DEC_SHIFT) | dev->params.acc_decimation;
    //res += i2c_write_reg(BUS, ADDR, LSM6DSL_REG_FIFO_CTRL3, tmp, 0);

    i2c_release(dev->params.i2c);

    if (res < 0) {
        puts("ERROR: set config");
        return -1;
    }
    else {
        return 0;
    }
}

void lsm6dsl_read_fifo_status(const lsm6dsl_t *dev)
{

    uint8_t tmp;
    int res = 0;

    //# fifo status 1: 0
    //# fifo status 2: 01100000
    //# fifo status 3: 00000010
    //# fifo status 4: 00000000
    const char str_fifo[] = "\nfifo status %i: ";

    res = i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_CTRL5, &tmp, 0);
    printf("\nfifo control5: ");
    print_binary(tmp, 8);
    printf("\n");

    //s1
    res += i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_STATUS1, &tmp, 0);
    printf(str_fifo, 1);

    //s2
    int status1 = tmp;
    res += i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_STATUS2, &tmp, 0);
    print_binary(tmp, 3);
    printf(" ");
    print_binary(status1, 8);
    printf(" ; %i", status1);
    status1 |= (((tmp & 0b111) << 8));
    printf(" ; %i", status1);
    printf("\nstatus 1 new:   ");
    print_binary(status1, 11);
    printf(str_fifo, 2);
    print_binary(tmp, 8);

    //s3
    res += i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_STATUS3, &tmp, 0);
    printf(str_fifo, 3);
    print_binary(tmp, 8);

    //s4
    res += i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_STATUS4, &tmp, 0);
    printf(str_fifo, 4);
    print_binary(tmp, 8);
    puts("\n");

    i2c_release(BUS);

    if (res < 0) {
        puts("ERROR: read fifo status\n");
    }
}

int lsm6dsl_read_fifo(const lsm6dsl_t *dev, lsm6dsl_3d_data_t acc_buffer[],
                      lsm6dsl_3d_data_t mag_buffer[], int level)
{

    uint8_t tmp;
    int res = 0;

    int16_t x_tmp;
    int16_t y_tmp;
    int16_t z_tmp;

    i2c_acquire(BUS);

    for (int c = 0, i = 0; c < level; c++) {
        res = i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_DATA_OUT_L, &tmp, 0);
        x_tmp = tmp;
        res += i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_DATA_OUT_H, &tmp, 0);
        x_tmp |= tmp << 8;

        res += i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_DATA_OUT_L, &tmp, 0);
        y_tmp = tmp;
        res += i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_DATA_OUT_H, &tmp, 0);
        y_tmp |= tmp << 8;

        res += i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_DATA_OUT_L, &tmp, 0);
        z_tmp = tmp;
        res += i2c_read_reg(BUS, ADDR, LSM6DSL_REG_FIFO_DATA_OUT_H, &tmp, 0);
        z_tmp |= tmp << 8;

        if (c % 2) {
            acc_buffer[i].x = ((int32_t)x_tmp * range_acc[dev->params.acc_fs]) /
                              INT16_MAX;
            acc_buffer[i].y = ((int32_t)y_tmp * range_acc[dev->params.acc_fs]) /
                              INT16_MAX;
            acc_buffer[i].z = ((int32_t)z_tmp * range_acc[dev->params.acc_fs]) /
                              INT16_MAX;


            //printf("%i: Accelerometer x: %i y: %i z: %i\n", i, acc_buffer[i].x, acc_buffer[i].y, acc_buffer[i].z);

            i++;

        }
        else {
            mag_buffer[i].x =
                ((int32_t)x_tmp * range_gyro[dev->params.gyro_fs]) / INT16_MAX;
            mag_buffer[i].y =
                ((int32_t)y_tmp * range_gyro[dev->params.gyro_fs]) / INT16_MAX;
            mag_buffer[i].z =
                ((int32_t)z_tmp * range_gyro[dev->params.gyro_fs]) / INT16_MAX;

            //printf("%i: Gyroscope x: %i y: %i z: %i\n", i, mag_buffer[i].x, mag_buffer[i].y, mag_buffer[i].z);
        }

    }

    /* Reset FIFO */
    uint8_t fifo_odr = MAX(dev->params.acc_odr, dev->params.gyro_odr);

    tmp = (fifo_odr << LSM6DSL_FIFO_CTRL5_FIFO_ODR_SHIFT) |
          LSM6DSL_FIFO_CTRL5_BYPASS_MODE;
    res += i2c_write_reg(BUS, ADDR, LSM6DSL_REG_FIFO_CTRL5, tmp, 0);
    tmp = (fifo_odr << LSM6DSL_FIFO_CTRL5_FIFO_ODR_SHIFT) |
          LSM6DSL_FIFO_CTRL5_FIFO_MODE;
    res += i2c_write_reg(BUS, ADDR, LSM6DSL_REG_FIFO_CTRL5, tmp, 0);

    i2c_release(BUS);

    if (res < 0) {
        puts("ERROR: read fifo\n");
        return -1;
    }

    return 0;
}

int main(void)
{
    uint8_t check_watermark = 0;
    lsm6dsl_t dev;

    lsm6dsl_3d_data_t acc_buffer[SAMPLE_LIMIT];
    lsm6dsl_3d_data_t mag_buffer[SAMPLE_LIMIT];

    int displacement = 0;

    printf("Initializing LSM6DSL sensor at I2C_%i... ", lsm6dsl_params->i2c);

    if (lsm6dsl_init(&dev, lsm6dsl_params) != LSM6DSL_OK) {
        puts("[ERROR]");
        return 1;
    }

    //Set own individual config
    if (lsm6dsl_set_config(&dev) != LSM6DSL_OK) {
        puts("[ERROR]");
        return 1;
    }

    puts("[SUCCESS]\n");

    puts("Powering down LSM6DSL sensor...");
    if (lsm6dsl_acc_power_down(&dev) != LSM6DSL_OK) {
        puts("[ERROR]");
        return 1;
    }
    if (lsm6dsl_gyro_power_down(&dev) != LSM6DSL_OK) {
        puts("[ERROR]");
        return 1;
    }
    puts("[SUCCESS]\n");
    xtimer_sleep(1);

    puts("Powering up LSM6DSL sensor...");
    if (lsm6dsl_acc_power_up(&dev) != LSM6DSL_OK) {
        puts("[ERROR]");
        return 1;
    }
    if (lsm6dsl_gyro_power_up(&dev) != LSM6DSL_OK) {
        puts("[ERROR]");
        return 1;
    }
    puts("[SUCCESS]\n");

    xtimer_sleep(3);
    puts("start");


    while (1) {

        int res = 0;

        lsm6dsl_print_acc(&dev);
        //lsm6dsl_print_gyro(&dev);

        //Check if FIFO is full
        if (lsm6dsl_read_reg(&dev, LSM6DSL_REG_FIFO_STATUS2,
                             &check_watermark) < 0) {
            printf("Error");
        }

        //Read FIFO
        if (check_watermark & WATERMARK_BIT) {
            res = lsm6dsl_read_fifo(&dev, acc_buffer, mag_buffer, SAMPLE_LIMIT);

            int dataArr[SAMPLE_LIMIT];
            for (int i = 0; i < SAMPLE_LIMIT; i++) {
                dataArr[i] = acc_buffer[i].z - 1005;
                if (dataArr[i] < 10 && dataArr[i] > -10) {
                    dataArr[i] = 0;
                }
            }

            double sum = trapezoidal_twice_array_integer(SAMPLE_LIMIT - 1, dataArr);
            displacement += sum;


            //printf("sum: %i         ; dis:\n", (int)sum);


        }

        //xtimer_sleep(1);

        if (res == LSM6DSL_OK) {}
        else {
            puts("[ERROR]\n");
        }

        //puts("");
        xtimer_usleep(SLEEP_USEC);
    }

    return 0;
}
