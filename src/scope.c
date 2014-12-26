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

#include <stdint.h>
#include "insider/insider.h"
#include "insider/packet.h"
#include "insider/protocol.h"
#include "insider/scope.h"

struct {
	uint8_t sz;
	uint8_t *ptr;
} scope_vars[8];

static size_t scope_var_count = 0;

void insider_scope_setup(size_t asiz)
{
	scope_var_count = 0;

	uint8_t cnt = 0;
	uint32_t pos = 3;

	ringbuf_peek_buffer(&insider_rx_ring, pos++, &cnt, 1);

	for (size_t i=0; i< cnt; i++) {
		ringbuf_peek_buffer(&insider_rx_ring, pos+0, &scope_vars[i].sz, 1);
		ringbuf_peek_buffer(&insider_rx_ring, pos+1, (uint8_t *)&scope_vars[i].ptr, asiz);
		pos += asiz + 1;
	}

	scope_var_count = cnt;

	insider_packet_reply(INSIDER_RSP_SUCCESS, 0, 0);
}

static void _local_memcpy(void *dst, const void * src, size_t sz)
{
	uint8_t *d = dst;
	const uint8_t *s = src;
	
	for (size_t i = 0; i < sz; i++)	{
		*d++ = *s++;
	}
}

void insider_scope_read(void)
{
	if (scope_var_count == 0) {
		insider_packet_reply(INSIDER_RSP_ERR_UNINITIALIZED, 0, 0);
		return;
	}

	uint8_t reply[32], *ptr = reply;
	uint8_t cnt = 0;

	for (size_t i = 0; i < scope_var_count; i++) {
		_local_memcpy(ptr, scope_vars[i].ptr, scope_vars[i].sz);
		ptr += scope_vars[i].sz;
		cnt += scope_vars[i].sz;
	}

	insider_packet_reply(INSIDER_RSP_SUCCESS, reply, cnt);
}

