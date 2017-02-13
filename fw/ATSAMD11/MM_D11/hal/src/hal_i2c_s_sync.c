/**
 * \file
 *
 * \brief I2C Sync Slave related functionality implementation.
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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

#include <hal_i2c_s_sync.h>
#include <utils_assert.h>
#include <utils.h>

/**
 * \brief Driver version
 */
#define DRIVER_VERSION 0x00000001u

static int32_t i2c_s_sync_write(struct io_descriptor *const io, const uint8_t *const buf, const uint16_t length);
static int32_t i2c_s_sync_read(struct io_descriptor *const io, uint8_t *const buf, const uint16_t length);

/**
 * \brief Initialize synchronous i2c slave interface
 */
int32_t i2c_s_sync_init(struct i2c_s_sync_descriptor *const descr, void *const hw)
{
	int32_t init_status;

	ASSERT(descr && hw);

	init_status = _i2c_s_sync_init(&descr->device, hw);
	if (init_status) {
		return init_status;
	}

	descr->io.read  = i2c_s_sync_read;
	descr->io.write = i2c_s_sync_write;

	return ERR_NONE;
}

/**
 * \brief De-initialize synchronous i2c slave interface
 */
int32_t i2c_s_sync_deinit(struct i2c_s_sync_descriptor *const descr)
{
	ASSERT(descr);

	return _i2c_s_sync_deinit(&descr->device);
}

/**
 * \brief Set the device address
 */
int32_t i2c_s_sync_set_addr(struct i2c_s_sync_descriptor *const descr, const uint16_t address)
{
	ASSERT(descr);

	if (!_i2c_s_sync_is_10bit_addressing_on(&descr->device)) {
		return _i2c_s_sync_set_address(&descr->device, address & 0x7F);
	}

	return _i2c_s_sync_set_address(&descr->device, address);
}

/**
 * \brief Enable I2C slave communication
 */
int32_t i2c_s_sync_enable(struct i2c_s_sync_descriptor *const descr)
{
	ASSERT(descr);

	return _i2c_s_sync_enable(&descr->device);
}

/**
 * \brief Disable I2C slave communication
 */
int32_t i2c_s_sync_disable(struct i2c_s_sync_descriptor *descr)
{
	ASSERT(descr);

	return _i2c_s_sync_disable(&descr->device);
}

/**
 * \brief Retrieve IO descriptor
 */
int32_t i2c_s_sync_get_io_descriptor(struct i2c_s_sync_descriptor *const descr, struct io_descriptor **io)
{
	ASSERT(descr && io);

	*io = &descr->io;

	return ERR_NONE;
}

/**
 * \brief Retrieve the current interface status
 */
int32_t i2c_s_sync_get_status(const struct i2c_s_sync_descriptor *const descr, i2c_s_status_t *const status)
{
	ASSERT(descr && status);

	*status = _i2c_s_sync_get_status(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Retrieve the current driver version
 */
uint32_t i2c_s_sync_get_version(void)
{
	return DRIVER_VERSION;
}

/*
 * \internal Read data from i2cs interface
 *
 * \param[in] descr The pointer to an io descriptor
 * \param[in] buf A buffer to read data to
 * \param[in] length The size of a buffer
 *
 * \return The number of bytes read.
 */
static int32_t i2c_s_sync_read(struct io_descriptor *const io, uint8_t *const buf, const uint16_t length)
{
	uint16_t                      offset = 0;
	struct i2c_s_sync_descriptor *descr  = CONTAINER_OF(io, struct i2c_s_sync_descriptor, io);

	ASSERT(io && buf && length);

	do {
		while (!_i2c_s_sync_is_byte_received(&descr->device))
			;
		buf[offset] = _i2c_s_sync_read_byte(&descr->device);
	} while (++offset < length);

	return (int32_t)offset;
}

/*
 * \internal Write the given data to i2c interface
 *
 * \param[in] descr The pointer to an io descriptor
 * \param[in] buf Data to write to usart
 * \param[in] length The number of bytes to write
 *
 * \return The number of bytes written or -1 if another write operation is in
 *         progress.
 */
static int32_t i2c_s_sync_write(struct io_descriptor *const io, const uint8_t *const buf, const uint16_t length)
{
	uint16_t                      offset = 0;
	struct i2c_s_sync_descriptor *descr  = CONTAINER_OF(io, struct i2c_s_sync_descriptor, io);

	ASSERT(io && buf && length);

	while (!_i2c_s_sync_is_byte_sent(&descr->device))
		;
	do {
		_i2c_s_sync_write_byte(&descr->device, buf[offset]);
		while (!_i2c_s_sync_is_byte_sent(&descr->device))
			;
	} while (++offset < length);

	return (int32_t)offset;
}
