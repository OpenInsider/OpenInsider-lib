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
#include "insider/packet.h"
#include "insider/protocol.h"
#include "insider/memory.h"

void insider_memory_read(uint32_t apos, size_t alen, size_t dlen)
{
	uint8_t reply[dlen];
	uint8_t *address = 0;

	ringbuf_peek_buffer(&insider_rx_ring, apos,(uint8_t *)&address, alen);

	for (size_t i = 0; i < dlen; i++) {
		reply[i] = *address++;
	}

	insider_packet_reply(INSIDER_RSP_SUCCESS, reply, dlen);
}

void insider_memory_write(uint32_t apos, size_t alen, size_t dlen)
{
	uint8_t *address = 0;

	ringbuf_peek_buffer(&insider_rx_ring, apos,(uint8_t *)&address, alen);
	ringbuf_peek_buffer(&insider_rx_ring, apos + alen, address, dlen);

	insider_packet_reply(INSIDER_RSP_SUCCESS, 0, 0);
}

void insider_memory_write_mask(uint32_t apos, size_t alen, size_t dlen)
{
	uint8_t *address = 0;

	ringbuf_peek_buffer(&insider_rx_ring, apos,(uint8_t *)&address, alen);

	uint32_t valpos = apos + alen;
	uint32_t mskpos = apos + alen + dlen;

	for (size_t i = 0; i < dlen; i++, address++, valpos++, mskpos++) {
		uint8_t val = 0, msk = 0;
		ringbuf_peek_byte(&insider_rx_ring, valpos, &val);
		ringbuf_peek_byte(&insider_rx_ring, mskpos, &msk);

		*address = (*address & ~msk) | (val & msk);
	}

	insider_packet_reply(INSIDER_RSP_SUCCESS, 0, 0);
}
