/*
 * This file is part of the OpenInsider project.
 *
 * Copyright (C) 2014 Frantisek Burian <BuFran@seznam.cz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RINGBUF_H_INCLUDED
#define RINGBUF_H_INCLUDED

/** @file ringbuf.h
 *
 * @defgroup primitives_ringbuf Ring Buffer
 * @ingroup primitives
 *
 * @brief Basic implementation of the Ring buffer for caching of serial data.
 *
 * @author 2014 (c) Frantisek Burian <BuFran@seznam.cz>
 *
 * @bug None of the accessor is atomic!
 *
 *@{*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct ringbuf {
	uint8_t *buffer;
	uint32_t mask;

	int32_t	rptr;
	int32_t wptr;
};

/*---------------------------------------------------------------------------*/
/** @brief Initialize the ring buffer structure.
 *
 * @param[in] rb      Pointer to the ring buffer
 * @param[in] buffer  The buffer to store the data
 * @param[in] bufsize The size of the buffer to store the data
 */
static inline
void ringbuf_init(struct ringbuf *rb, uint8_t *buffer, size_t bufsize)
{
	// TODO check if bufsize is 2^N, if not, select nearest less 2^N buffer
	rb->buffer = buffer;
	rb->mask = bufsize - 1;

	rb->rptr = rb->wptr = 0;

	/* Clear the buffer */
	memset(buffer, 0, bufsize);
}

/*---------------------------------------------------------------------------*/
/** @brief Get available bytes for writing.
 *
 * @param[in] rb      Pointer to the ring buffer
 * @returns number of bytes that is available for writing.
 */
static inline
size_t ringbuf_write_available(struct ringbuf *rb)
{
	return (rb->rptr + rb->mask - rb->wptr) & rb->mask;
}

/*---------------------------------------------------------------------------*/
/** @brief Write single byte to the ring buffer.
 *
 * @param[in] rb      Pointer to the ring buffer
 * @param[in] data    Byte to write
 * @returns true if success
 */
static inline
bool ringbuf_write_byte(struct ringbuf *rb,uint8_t data)
{
	/* Fail, if there is no space for writing */
	if (((rb->wptr + 1 - rb->rptr) & rb->mask) == 0)
		return false;

	rb->buffer[rb->wptr] = data;
	rb->wptr = (rb->wptr + 1) & rb->mask;

	return true;
}

/*---------------------------------------------------------------------------*/
/** @brief Write buffer to the ring buffer.
 *
 * @param[in] rb      Pointer to the ring buffer
 * @param[in] data    Data buffer
 * @param[in] bufsiz  Buffer size
 * @returns true if success
 */
static inline
bool ringbuf_write_buffer(struct ringbuf *rb, const uint8_t *data, size_t siz)
{
	if (siz == 0)
		return true;

	if (ringbuf_write_available(rb) < siz)
		return false;

	while (siz-- > 0) {
		rb->buffer[rb->wptr] = *data++;
		rb->wptr = (rb->wptr + 1) & rb->mask;
	}

	return true;
}

/*---------------------------------------------------------------------------*/
/** @brief Try to write the buffer to the ring, and returns count of bytes
 * successfully written.
 *
 * @param[in] rb      Pointer to the ring buffer
 * @param[in] data    Data buffer
 * @param[in] bufsiz  Buffer size
 * @returns count of bytes successfully written to the ring
 */
static inline
size_t ringbuf_write_buffer_partial(struct ringbuf *rb, const uint8_t *data,
	size_t siz)
{
	if (siz == 0)
		return 0;

	int cnt = 0;

	/* ToDo use two memcpy calls */
	while (((rb->wptr + 1 - rb->rptr) & rb->mask) > 0) {
		rb->buffer[rb->wptr] = *data++;
		rb->wptr = (rb->wptr + 1) & rb->mask;
		cnt++;
		if (--siz == 0)
			break;
	}

	return cnt;
}

/*---------------------------------------------------------------------------*/
/** @brief Get available bytes for reading
 *
 * @param[in] rb      Pointer to the ring buffer
 * @returns number of bytes that is available for read.
 */
static inline
size_t ringbuf_read_available(struct ringbuf *rb)
{
	return (rb->wptr + rb->mask + 1 - rb->rptr) & rb->mask;
}

/*---------------------------------------------------------------------------*/
/** @brief Test, if the buffer is at least 75% full
 *
 * @param[in] rb      Pointer to the ring buffer
 * @returns true, if the read buffer is at 75% or more full
 */
static inline
bool ringbuf_read_almost_full(struct ringbuf *rb)
{
	size_t avail = (rb->wptr + rb->mask + 1 - rb->rptr) & rb->mask;
	return avail > (rb->mask - (rb->mask >> 2));
}

/*---------------------------------------------------------------------------*/
/** @brief Read single byte from the ring
 *
 * @param[in] rb      Pointer to the ring buffer
 * @param[out] data   Buffer to hold the result
 * @returns true, if success
 */
static inline
bool ringbuf_read_byte(struct ringbuf *rb,uint8_t *data)
{
	/* Fail, if there is no data for reading */
	if (rb->wptr == rb->rptr)
		return false;

	*data = rb->buffer[rb->rptr];
	rb->rptr = (rb->rptr + 1) & rb->mask;

	return true;
}

/*---------------------------------------------------------------------------*/
/** @brief Read buffer from the ring
 *
 * @param[in] rb      Pointer to the ring buffer
 * @param[out] data   Data buffer to hold the result
 * @param[in] bufsiz  Buffer size
 * @returns true, if success
 */
static inline
bool ringbuf_read_buffer(struct ringbuf *rb, uint8_t *data, size_t bufsiz)
{
	if (rb->wptr == rb->rptr)
		return false;

	if (ringbuf_read_available(rb) < bufsiz)
		return false;

	/* ToDo use two memcpy calls */
	while (bufsiz-- > 0) {
		*data++ = rb->buffer[rb->rptr];
		rb->rptr = (rb->rptr + 1) & rb->mask;
	}

	return true;
}

static inline
size_t ringbuf_read_buffer_partial(struct ringbuf *rb, uint8_t *data, size_t bufsiz)
{
	if (rb->wptr == rb->rptr)
		return 0;

	size_t cnt = ringbuf_read_available(rb);

	if (cnt > bufsiz)
		cnt = bufsiz;
	if (cnt < bufsiz)
		bufsiz = cnt;

	/* ToDo use two memcpy calls */
	while (bufsiz-- > 0) {
		*data++ = rb->buffer[rb->rptr];
		rb->rptr = (rb->rptr + 1) & rb->mask;
	}

	return cnt;
}

/*---------------------------------------------------------------------------*/
/** @brief Skip data inside the ring
 *
 * @param[in] rb      Pointer to the ring buffer
 * @param[in] len     Skip length
 * @returns true, if success
 */
static inline
bool ringbuf_skip(struct ringbuf *rb, size_t len)
{
	if (rb->wptr == rb->rptr)
		return false;

	if (ringbuf_read_available(rb) < len)
		return false;

	rb->rptr = (rb->rptr + len) & rb->mask;
	return true;
}

/*---------------------------------------------------------------------------*/
/** @brief Peek the data byte from ring buffer at offset
 *
 * @param[in] rb      Pointer to the ring buffer
 * @param[in] offset  Offset from current reading position
 * @param[out] data   Pointer to data byte
 * @returns true if success
 */
static inline
bool ringbuf_peek_byte(struct ringbuf *rb, uint32_t offset, uint8_t *data)
{
	if (rb->wptr == rb->rptr)
		return false;

	if (ringbuf_read_available(rb) < offset)
		return false;

	*data = rb->buffer[(rb->rptr + offset) & rb->mask];
	return true;
}

/**
 * @brief Peek the data buffer from ring buffer at offset
 *
 * @param[in] rb      Pointer to the ring buffer
 * @param[in] offset  Offset from current reading position to start of buffer
 * @param[out] data   Pointer to data buffer
 * @param[in] bufsiz  The readed buffer size
 * @returns true if success
 */
static inline
bool ringbuf_peek_buffer(struct ringbuf *rb, uint32_t offset, uint8_t *data,
	size_t bufsiz)
{
	if (rb->wptr == rb->rptr)
		return false;

	if (ringbuf_read_available(rb) < (offset + bufsiz))
		return false;

	while (bufsiz-- > 0)
		*data++ = rb->buffer[(rb->rptr + offset++) & rb->mask];

	return true;
}

/**@}*/

#endif /* RINGBUF_H_INCLUDED */
