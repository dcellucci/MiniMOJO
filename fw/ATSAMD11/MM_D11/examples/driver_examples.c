/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

void I2C_0_example(void)
{
	struct io_descriptor *io;
	uint8_t               c;

	i2c_s_sync_get_io_descriptor(&I2C_0, &io);
	i2c_s_sync_set_addr(&I2C_0, 1);
	i2c_s_sync_enable(&I2C_0);

	io_read(io, &c, 1);
}

void I2C_1_example(void)
{
	struct io_descriptor *I2C_1_io;

	i2c_m_sync_get_io_descriptor(&I2C_1, &I2C_1_io);
	i2c_m_sync_enable(&I2C_1);
	i2c_m_sync_set_slaveaddr(&I2C_1, 0x12, I2C_M_SEVEN);
	io_write(I2C_1_io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using PWM_0.
 */
void PWM_0_example(void)
{
	pwm_set_parameters(&PWM_0, 10000, 5000);
	pwm_enable(&PWM_0);
}
