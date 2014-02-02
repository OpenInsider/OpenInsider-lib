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

struct ringbuf insider_rx_ring;
struct ringbuf insider_tx_ring;

static uint8_t rx_buffer[256];
static uint8_t tx_buffer[256];

void insider_packet_init(void)
{
	ringbuf_init(&insider_rx_ring, rx_buffer, 256);
	ringbuf_init(&insider_tx_ring, tx_buffer, 256);
}

bool insider_packet_parse(void)
{
	uint8_t ch;

resynchronize:
	/* Synchronize to the packet start */
	while (ringbuf_peek_byte(&insider_rx_ring, 0, &ch) &&
	       (ch != INSIDER_PACKET_START))
		ringbuf_skip(&insider_rx_ring, 1);

	/* If we didn't found the packet starter, we can try
	 * to parse the next chunk of data, or return to the caller */
	if (ch != INSIDER_PACKET_START)
		return false;

	/* Try to determine the length of the packet */
	uint8_t pklen;
	if (!ringbuf_peek_byte(&insider_rx_ring, 1, &pklen))
		return false;

	if (INSIDER_PACKET_HASLEN(pklen))
		pklen = INSIDER_PACKET_LEN(pklen) - 1;
	else if (!ringbuf_peek_byte(&insider_rx_ring, 2, &pklen))
		return false;


	pklen += 4;
	if (ringbuf_read_available(&insider_rx_ring) < pklen)
		return false;

	/* We detected that we have all needed data in the buffer. */
	/* Check the CRC, skip the SOM */
	uint8_t cs = 0;
	for (size_t i = 1; i < pklen; i++) {
		ringbuf_peek_byte(&insider_rx_ring, i, &ch);
		cs += ch;
	}

	if (cs != 0) {
		/* Bad CRC. Try to resynchronize with next byte */
		ringbuf_skip(&insider_rx_ring, 1);
		goto resynchronize;
	}

	/* Parse the found message by higher layer of protocol */
	insider_protocol_parse();

	/* Unbuffer the packet */
	ringbuf_skip(&insider_rx_ring, pklen);
	return true;
}

void insider_packet_reply(uint8_t code, const uint8_t *data, size_t length)
{
	ringbuf_write_byte(&insider_tx_ring, INSIDER_PACKET_START);
	ringbuf_write_byte(&insider_tx_ring, code);
	ringbuf_write_buffer(&insider_tx_ring, data, length);

	uint8_t cs = code;
	for (size_t i = 0; i < length; i++)
		cs += *data++;

	ringbuf_write_byte(&insider_tx_ring, (uint8_t)(0x100 - cs));
}

