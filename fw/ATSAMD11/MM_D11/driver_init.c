/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_irq.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

extern struct _irq_descriptor *_irq_table[PERIPH_COUNT_IRQn];
extern void                    Default_Handler(void);

struct i2c_s_sync_descriptor I2C_0;

struct i2c_m_sync_desc I2C_1;

struct pwm_descriptor PWM_0;

void I2C_0_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA14,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA14, PINMUX_PA14C_SERCOM0_PAD0);

	gpio_set_pin_pull_mode(PA15,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA15, PINMUX_PA15C_SERCOM0_PAD1);
}

void I2C_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
	_gclk_enable_channel(SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();
	i2c_s_sync_init(&I2C_0, SERCOM0);
	I2C_0_PORT_init();
}

void I2C_1_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA22,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA22, PINMUX_PA22C_SERCOM1_PAD0);

	gpio_set_pin_pull_mode(PA23,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA23, PINMUX_PA23C_SERCOM1_PAD1);
}

void I2C_1_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM1);
	_gclk_enable_channel(SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC);
	_gclk_enable_channel(SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC);
}

void I2C_1_init(void)
{
	I2C_1_CLOCK_init();
	i2c_m_sync_init(&I2C_1, SERCOM1);
	I2C_1_PORT_init();
}

void PWM_0_PORT_init(void)
{

	gpio_set_pin_function(PA04, PINMUX_PA04F_TCC0_WO0);

	gpio_set_pin_function(PA05, PINMUX_PA05F_TCC0_WO1);

	gpio_set_pin_function(PA06, PINMUX_PA06F_TCC0_WO2);
}

void PWM_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, TCC0);
	_gclk_enable_channel(TCC0_GCLK_ID, CONF_GCLK_TCC0_SRC);
}

void PWM_0_init(void)
{
	PWM_0_CLOCK_init();
	PWM_0_PORT_init();
	pwm_init(&PWM_0, TCC0, _tcc_get_pwm());
}

void PM_Handler(void)
{
	if (_irq_table[PM_IRQn + 0]) {
		_irq_table[PM_IRQn + 0]->handler(_irq_table[PM_IRQn + 0]->parameter);
	} else {
		Default_Handler();
	}
}

void SERCOM0_Handler(void)
{
	if (_irq_table[SERCOM0_IRQn + 0]) {
		_irq_table[SERCOM0_IRQn + 0]->handler(_irq_table[SERCOM0_IRQn + 0]->parameter);
	} else {
		Default_Handler();
	}
}

void SERCOM1_Handler(void)
{
	if (_irq_table[SERCOM0_IRQn + 1]) {
		_irq_table[SERCOM0_IRQn + 1]->handler(_irq_table[SERCOM0_IRQn + 1]->parameter);
	} else {
		Default_Handler();
	}
}

void DMAC_Handler(void)
{
	if (_irq_table[+0]) {
		_irq_table[+0]->handler(_irq_table[+0]->parameter);
	} else {
		Default_Handler();
	}
}

void SYSCTRL_Handler(void)
{
	if (_irq_table[SYSCTRL_IRQn + 0]) {
		_irq_table[SYSCTRL_IRQn + 0]->handler(_irq_table[SYSCTRL_IRQn + 0]->parameter);
	} else {
		Default_Handler();
	}
}

void TCC0_Handler(void)
{
	if (_irq_table[TCC0_IRQn + 0]) {
		_irq_table[TCC0_IRQn + 0]->handler(_irq_table[TCC0_IRQn + 0]->parameter);
	} else {
		Default_Handler();
	}
}

void system_init(void)
{
	init_mcu();

	I2C_0_init();

	I2C_1_init();

	PWM_0_init();
}
