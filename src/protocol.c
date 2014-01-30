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

static const uint8_t reply_err_invalid_cmd[] = { INSIDER_RSP_ERR_INVALID_CMD };


void insider_protocol_parse(void)
{
	uint8_t cmd;
	
	ringbuf_peek_byte(&insider_rx_ring, 0, &cmd);
	
	switch (cmd) {
	
	default:
		insider_packet_reply(reply_err_invalid_cmd, 1);
		break;
	}
}


void insider_init(void)
{
	insider_packet_init();
}