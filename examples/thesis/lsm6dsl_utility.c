#include "lsm6dsl_utility.h"

void print_binary(int value, int length){
	char buffer[33];
	buffer[length] = '\0';

	for(int i = length - 1; i >= 0; i--)
	{
		if(value & 1){
			buffer[i] = '1';
		}else{
			buffer[i] = '0';
		}
		value >>= 1;
	}

	printf("%s", buffer);
}

void lsm6dsl_print_acc(lsm6dsl_t *dev){
	//avg out: Accelerometer x: -36 y: -32 z: 1005
	lsm6dsl_3d_data_t acc_value;
	if (lsm6dsl_read_acc(dev, &acc_value) == LSM6DSL_OK) {
		printf("Accelerometer x: %i y: %i z: %i\n", acc_value.x, acc_value.y, acc_value.z);
	}else {
		puts("[ERROR] reading accelerometer!\n");
	}
}

void lsm6dsl_print_gyro(lsm6dsl_t *dev){
	//avg out: Gyroscope x: 25 y: -33 z: -29
	lsm6dsl_3d_data_t mag_value;
	if (lsm6dsl_read_gyro(dev, &mag_value) == LSM6DSL_OK) {
		printf("Gyroscope x: %i y: %i z: %i\n", mag_value.x, mag_value.y, mag_value.z);
	}else {
		puts("[ERROR] reading gyroscope!\n");
	}
}

void lsm6dsl_print_temp(lsm6dsl_t *dev){
	 //avg out: Temperature [in °C x 100]: 2452
	int16_t temp_value;
	if (lsm6dsl_read_temp(dev, &temp_value) == LSM6DSL_OK) {
		printf("Temperature [in °C x 100]: %i \n", temp_value);
	}else {
		puts("[ERROR] reading temperature!\n");
	}
}

//LSM6DSL_REG_CTRL8_XL
int lsm6dsl_write_reg(lsm6dsl_t *dev, uint16_t addr, uint8_t value){
    i2c_acquire(dev->params.i2c);

    if(i2c_write_reg(dev->params.i2c, dev->params.addr, addr, value, 0) < 0){
    	i2c_release(dev->params.i2c);
    	puts("ERROR: write reg\n");
    	return -1;
    }else{
    	i2c_release(dev->params.i2c);
    	return 0;
    }
}

int lsm6dsl_read_reg(lsm6dsl_t *dev, uint16_t addr, uint8_t *var){
    i2c_acquire(dev->params.i2c);

    if (i2c_read_reg(dev->params.i2c, dev->params.addr, addr, var, 0) < 0) {
        i2c_release(dev->params.i2c);
        puts("ERROR: read reg\n");
        return -1;
    }else{
//    	print_binary(*var, 8);
    	//puts("");

		i2c_release(dev->params.i2c);

		return 0;
    }
}
