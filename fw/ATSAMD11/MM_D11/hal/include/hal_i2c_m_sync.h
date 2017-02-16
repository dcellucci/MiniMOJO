/**
 * \file
 *
 * \brief Sync I2C Hardware Abstraction Layer(HAL) declaration.
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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

#ifndef _HAL_I2C_M_SYNC_H_INCLUDED
#define _HAL_I2C_M_SYNC_H_INCLUDED

#include <hpl_i2c_m_sync.h>
#include <hal_io.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_i2c_master_sync
 *
 * @{
 */

#define I2C_M_MAX_RETRY 1

/**
 * \brief i2c descriptor structure, embed i2c_device & i2c_interface
 */
struct i2c_m_sync_desc {
	struct _i2c_m_sync_device device;
	struct io_descriptor      io;
	uint16_t                  slave_addr;
};

/**
 * \brief Initialize synchronous i2c interface
 *
 * This function initializes the given io descriptor to be used as synchronous I2C interface descriptor.
 * It checks if the given hardware is not initialized and if the given hardware is permitted to be initialized.
 *
 * \param[in] i2c An i2c descriptor which is used to communicate through I2C
 * \param[in] hw The pointer to hardware instance
 *
 * \return Initialization status.
 * \retval -1 Passed parameters were invalid or the interface is already initialized
 * \retval 0 The initialization is completed successfully
 */
int32_t i2c_m_sync_init(struct i2c_m_sync_desc *i2c, void *hw);

/**
 * \brief De-initialize i2c interface
 *
 * This function de-initializes the given IO descriptor.
 * It checks if the given hardware is initialized and if the given hardware is permitted to be de-initialized.
 *
 * \param[in] i2c An i2c descriptor which is used to communicate through I2C
 *
 * \return Uninitialization status.
 * \retval -1 Passed parameters were invalid or the interface is already de-initialized
 * \retval 0 The de-initialization is completed successfully
 */
int32_t i2c_m_sync_deinit(struct i2c_m_sync_desc *i2c);

/**
 * \brief Set the slave device address
 *
 * This function sets the next transfer target slave i2c device address.
 * It takes no effect to already started access.
 *
 * \param[in] i2c An i2c descriptor which is used to communicate through I2C
 * \param[in] addr The slave address to access
 * \param[in] addr_len The slave address length, can be I2C_M_TEN or I2C_M_SEVEN
 *
 * \return Masked slave address, the mask is maximum 10-bit address, and 10th
 *         bit set if 10-bit is used
 */
int32_t i2c_m_sync_set_slaveaddr(struct i2c_m_sync_desc *i2c, int16_t addr, int32_t addr_len);

/**
 * \brief Set baudrate
 *
 * This function set the i2c device to specified baudrate, only takes effect when hardware is disabled
 *
 * \param[in] i2c An i2c descriptor which is used to communicate through I2C
 * \param[in] clkrate Unused parameter, should always be 0
 * \param[in] baudrate The baud rate value set to master
 *
 * \return Whether successfully set the baudrate
 * \retval -1 Passed parameters were invalid or device is already enabled
 * \retval 0 The baudrate set is completed successfully
 */
int32_t i2c_m_sync_set_baudrate(struct i2c_m_sync_desc *i2c, uint32_t clkrate, uint32_t baudrate);

/**
 * \brief Sync version of enable hardware
 *
 * This function enables the i2c device then waits for this enabling operation is done
 *
 * \param[in] i2c An i2c descriptor which is used to communicate through I2C
 *
 * \return Whether successfully enable the device
 * \retval -1 Passed parameters were invalid or device enable failed
 * \retval 0 The hardware enabling is completed successfully
 */
int32_t i2c_m_sync_enable(struct i2c_m_sync_desc *i2c);

/**
 * \brief Sync version of disable hardware
 *
 * This function disables the i2c device then waits for this disabling operation is done
 *
 * \param[in] i2c An i2c descriptor which is used to communicate through I2C
 *
 * \return Whether successfully disable the device
 * \retval -1 Passed parameters were invalid or device disable failed
 * \retval 0 The hardware disabling is completed successfully
 */
int32_t i2c_m_sync_disable(struct i2c_m_sync_desc *i2c);

/**
 * \brief Sync version of write command to i2c slave
 *
 * This function will write the value to specified reg in i2c slave device then waits for this operation is done
 * The sequence of this routine is
 * sta->address(write)->ack->reg address->ack->resta->address(write)->ack->reg value->nack->stt
 *
 * \param[in] i2c An i2c descriptor which is used to communicate through I2C
 * \param[in] reg The internal address/register of i2c slave device
 * \param[in] value The value write to i2c slave device
 *
 * \return Whether successfully write to the device
 * \retval <0 Passed parameters were invalid or write fail
 * \retval 0 Writing to register is completed successfully
 */
int32_t i2c_m_sync_cmd_write(struct i2c_m_sync_desc *i2c, uint8_t reg, uint8_t value);

/**
 * \brief Sync version of read register value from i2c slave
 *
 * This function will read a byte value from specified reg in i2c slave device then waits for this operation is done
 * The sequence of this routine is
 * sta->address(write)->ack->reg address->ack->resta->address(read)->ack->reg value->nack->stt
 *
 * \param[in] i2c An i2c descriptor which is used to communicate through I2C
 * \param[in] reg The internal address/register of i2c slave device
 * \param[in] value The value write to i2c slave device
 *
 * \return Whether successfully read from the device
 * \retval <0 Passed parameters were invalid or read fail
 * \retval 0 Reading from register is completed successfully
 */
int32_t i2c_m_sync_cmd_read(struct i2c_m_sync_desc *i2c, uint8_t reg, uint8_t *value);

/**
 * \brief Sync version of transfer message to/from i2c slave
 *
 * This function will transfer a message between the i2c slave and the master. This function will wait for the operation
 * to be done.
 *
 * \param[in] i2c An i2c descriptor which is used to communicate through I2C
 * \param[in] msg  An i2c_m_msg struct
 *
 * \return The status of the operation
 * \retval 0 Operation completed successfully
 * \retval <0 Operation failed
 */
int32_t i2c_m_sync_transfer(struct i2c_m_sync_desc *const i2c, struct _i2c_m_msg *msg);

/**
 * \brief Sync version of send stop condition on the i2c bus
 *
 * This function will create a stop condition on the i2c bus to release the bus
 *
 * \param[in] i2c An i2c descriptor which is used to communicate through I2C
 *
 * \return The status of the operation
 * \retval 0 Operation completed successfully
 * \retval <0 Operation failed
 */
int32_t i2c_m_sync_send_stop(struct i2c_m_sync_desc *const i2c);

/**
 * \brief Return IO descriptor for this I2C instance
 *
 * This function will return a IO instance for this I2C driver instance
 *
 * \param[in] i2c_m_sync_desc An i2c descriptor which is used to communicate through I2C
 * \param[in] io_descriptor A pointer to an IO descriptor pointer type
 *
 * \return Error code
 * \retval 0 No error detected
 * \retval <0 Error code
 */
int32_t i2c_m_sync_get_io_descriptor(struct i2c_m_sync_desc *const i2c, struct io_descriptor **io);

/**
 * \brief Retrieve the current driver version
 *
 * \return Current driver version
 */
uint32_t i2c_m_sync_get_version(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif