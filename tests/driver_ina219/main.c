/*
 * Copyright (C) 2018 University of Applied Science Emden/Leer
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
 * @brief       Test application for the INA219 sensor driver. Basically just small modifications of the INA220 implementation of Joakim Nohlgård
 *
 * @author      Dhruv Verma <dhruv2scs@gmail.com>
 *
 * @}
 */

#ifndef TEST_INA219_I2C
#error "TEST_INA219_I2C not defined"
#endif
#ifndef TEST_INA219_ADDR
#error "TEST_INA219_ADDR not defined"
#endif

#include <fmt.h>

#include "xtimer.h"
#include "ina219.h"

/*  By default we use a pretty huge range for the input voltage,
 *   which probably isn't the most appropriate choice for system
 *   that don't use a lot of power.  But all of the calculations
 *   are shown below if you want to change the settings.  You will
 *   also need to change any relevant register settings, such as
 *   setting the VBUS_MAX to 16V instead of 32V, etc.
 *
 *   Example calculations:
 *
 *   VBUS_MAX = 32V            (Assumes 32V, can also be set to 16V)
 *   VSHUNT_MAX = 0.32         (Assumes Gain 8, 320mV, can also be 0.16, 0.08, 0.04)
 *   RSHUNT = 0.1              (Resistor value in ohms)

 *   1. Determine max possible current
 *   MaxPossible_I = VSHUNT_MAX / RSHUNT
 *   MaxPossible_I = 3.2A
 *
 *   2. Determine max expected current
 *   MaxExpected_I = 2.0A
 *
 *   3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
 *   MinimumLSB = MaxExpected_I/32767
 *   MinimumLSB = 0.000061              (61uA per bit)
 *   MaximumLSB = MaxExpected_I/4096
 *   MaximumLSB = 0,000488              (488uA per bit)
 *
 *   4. Choose an LSB between the min and max values
 *   (Preferrably a roundish number close to MinLSB)
 *   CurrentLSB = 0.0001 (100uA per bit)
 *
 *   5. Compute the calibration register
 *   Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
 *   Cal = 4096 (0x1000)
 *
 *   CALIBRATION = 4096;
 *
 *   6. Calculate the power LSB
 *   PowerLSB = 20 * CurrentLSB
 *   PowerLSB = 0.002 (2mW per bit)
 *
 *   7. Compute the maximum current and shunt voltage values before overflow
 *
 *   Max_Current = Current_LSB * 32767
 *   Max_Current = 3.2767A before overflow
 *
 *   If Max_Current > Max_Possible_I then
 *   Max_Current_Before_Overflow = MaxPossible_I
 *   Else
 *   Max_Current_Before_Overflow = Max_Current
 *   End If
 *
 *   Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
 *   Max_ShuntVoltage = 0.32V
 *
 *   If Max_ShuntVoltage >= VSHUNT_MAX
 *      Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
 *   Else
 *      Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
 *   End If
 *
 *   8. Compute the Maximum Power
 *   MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
 *   MaximumPower = 3.2 * 32V
 *   MaximumPower = 102.4W
 *
 *   Set multipliers to convert raw current/power values
 *   CURRENT_DEVIDER_mA = 10;  // Current LSB = 100uA per bit (1000/100 = 10)
 *   POWER_MULTIPLIER_mW = 2;     // Power LSB = 1mW per bit (2/1)
 *
 *
 */

#define CONFIG        (INA219_MODE_CONTINUOUS_SHUNT_BUS | INA219_RANGE_40MV | \
                      INA219_BRNG_16V_FSR | INA219_BADC_AVG_128_SAMPLES | \
                      INA219_BADC_AVG_128_SAMPLES)
#define CALIBRATION   (10240)
#define CURRENT_DEVIDER_MA (25)
#define POWER_MULTIPLIER_MW (1)
#define SLEEP    (100 * 10000U)

int main(void){

    ina219_t dev;
    int16_t val;

    puts("INA219 sensor driver test application\n");
    printf("Initializing I2C_%i... ", TEST_INA219_I2C);

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
        puts("shunt: ");
        print_float((val * 0.001), 2);
        puts(" mV ");

        /* Read VBUS voltage, in volts */
        ina219_read_bus(&dev, &val);
        /* The bus voltage is found in the topmost 13 bits of the bus voltage
         * register */
        val = (val >> INA219_BUS_VOLTAGE_SHIFT) * 4;
        puts("bus: ");
        print_float((val * 0.001), 2);
        puts(" V");

        /* Read current register milliampere, the scale depends on the value of the
         * calibration register */
        ina219_read_current(&dev, &val);
        puts("current: ");
        print_float(((val / CURRENT_DEVIDER_MA) ), 5);
        puts(" mA");

        /* Read power register in watts, the scale depends on the value of the
         * calibration register */
        ina219_read_power(&dev, &val);
        puts("power: ");
        print_float(((val) * POWER_MULTIPLIER_MW), 6);
        puts(" mW\n");

        xtimer_usleep(SLEEP);
        puts(" test\n");
    }

    return 0;
}
