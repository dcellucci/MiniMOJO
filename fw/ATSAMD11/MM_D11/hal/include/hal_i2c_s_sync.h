/**
 * \file
 *
 * \brief Sync I2C Slave Hardware Abstraction Layer(HAL) declaration.
 *
 * Copyright (C) 2015-2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef _HAL_I2C_S_SYNC_H_INCLUDED
#define _HAL_I2C_S_SYNC_H_INCLUDED

#include <hal_io.h>
#include <hpl_i2c_s_sync.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_i2c_slave_sync
 *
 * @{
 */

/**
 * \brief i2c slave descriptor structure
 */
struct i2c_s_sync_descriptor {
	struct _i2c_s_sync_device device;
	struct io_descriptor      io;
};

/**
 * \brief Initialize synchronous i2c slave interface
 *
 * This function initializes the given i2c descriptor to be used as synchronous
 * I2C slave interface descriptor. It checks if the given hardware is not
 * initialized and if the given hardware is permitted to be initialized.
 *
 * \param[in] descr An i2c slave descriptor which is used to communicate through
 *                I2C
 * \param[in] hw The pointer to hardware instance
 *
 * \return Initialization status.
 */
int32_t i2c_s_sync_init(struct i2c_s_sync_descriptor *const descr, void *hw);

/**
 * \brief De-initialize synchronous i2c slave interface
 *
 * This function de-initializes the given synchronous I2C slave descriptor.
 * It checks if the given hardware is initialized and if the given hardware is
 * permitted to be de-initialized.
 *
 * \param[in] descr An i2c slave descriptor which is used to communicate through
 *                I2C
 *
 * \return De-initialization status.
 */
int32_t i2c_s_sync_deinit(struct i2c_s_sync_descriptor *const descr);

/**
 * \brief Set the device address
 *
 * This function sets the i2c slave device address.
 *
 * \param[in] descr An i2c slave descriptor which is used to communicate  through
 *                I2C
 * \param[in] address An address
 *
 * \return Status of address setting.
 */
int32_t i2c_s_sync_set_addr(struct i2c_s_sync_descriptor *const descr, const uint16_t address);

/**
 * \brief Enable I2C slave communication
 *
 * This function enables the i2c slave device
 *
 * \param[in] descr An i2c slave descriptor which is used to communicate through
 *                I2C
 *
 * \return Enabling status.
 */
int32_t i2c_s_sync_enable(struct i2c_s_sync_descriptor *const descr);

/**
 * \brief Disable I2C slave communication
 *
 * This function disables the i2c slave device
 *
 * \param[in] descr An i2c slave descriptor which is used to communicate through
 *                I2C
 *
 * \return Disabling status.
 */
int32_t i2c_s_sync_disable(struct i2c_s_sync_descriptor *const descr);

/**
 * \brief Retrieve IO descriptor
 *
 * This function returns a IO instance for the given I2C slave driver instance
 *
 * \param[in] descr An i2c slave descriptor which is used to communicate through
 *                I2C
 * \param[in] io A pointer to an IO descriptor pointer type
 *
 * \return IO retrieving status.
 */
int32_t i2c_s_sync_get_io_descriptor(struct i2c_s_sync_descriptor *const descr, struct io_descriptor **io);

/**
 * \brief Retrieve the current interface status
 *
 * \param[in]  descr An i2c descriptor which is used to communicate via USART
 * \param[out] status The state of I2C slave
 *
 * \return The status of I2C status retrieving.
 */
int32_t i2c_s_sync_get_status(const struct i2c_s_sync_descriptor *const descr, i2c_s_status_t *const status);

/**
 * \brief Retrieve the current driver version
 *
 * \return Current driver version
 */
uint32_t i2c_s_sync_get_version(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* _HAL_I2C_S_SYNC_H_INCLUDED */
