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
#include <string.h>
#include "insider/packet.h"
#include "insider/protocol.h"
#include "insider/memory.h"

#define INSIDER_FIRMWARE_MAJOR		1
#define INSIDER_FIRMWARE_MINOR		0
#define INSIDER_BUFFERSIZE		255

static const char* const board_name = "OpenInsider board driver";

static const uint8_t reply_err_invalid_cmd[] = { INSIDER_RSP_ERR_INVALID_CMD };

static void insider_protocol_getinfo(bool full)
{
	uint8_t reply[36];
	size_t sz = 6;

	reply[0] = 3;
	reply[1] = 0;
	reply[2] = 1;
	reply[3] = INSIDER_FIRMWARE_MAJOR;
	reply[4] = INSIDER_FIRMWARE_MINOR;
	reply[5] = INSIDER_BUFFERSIZE;
	
	if (full) {
		reply[6] = 0;	// recorder buffer length
		reply[7] = 0;
		reply[8] = 0;	// recorder time base
		reply[9] = 0;
		strcpy((char *)&reply[10], board_name);
		sz += 29;
	}
	insider_packet_reply(reply, sz);
	
}

void insider_protocol_parse(void)
{
	uint8_t cmd = 0;
	size_t alen = 2;
	uint8_t dlen = 0;
	
	ringbuf_peek_byte(&insider_rx_ring, 0, &cmd);
	
	switch (cmd) {
	/**********************************************************************/
	/* Information gathering operations */
	
	case INSIDER_CMD_GET_INFO:
		insider_protocol_getinfo(true);
		break;

	case INSIDER_CMD_GET_INFO_BRIEF:
		insider_protocol_getinfo(false);
		break;

	/**********************************************************************/
	/* Memory operations */

	case INSIDER_CMD_GET8_EX:
		alen = 4;
		/* No break */

	case INSIDER_CMD_GET8:
		insider_memory_read(2, alen, 1);
		break;

	case INSIDER_CMD_GET16_EX:
		alen = 4;
		/* No break */

	case INSIDER_CMD_GET16:
		insider_memory_read(2, alen, 2);
		break;

	case INSIDER_CMD_GET32_EX:
		alen = 4;
		/* No break */

	case INSIDER_CMD_GET32:
		insider_memory_read(2, alen, 4);
		break;

	case INSIDER_CMD_MEMRD_EX:
		alen = 4;
		/* No break */

	case INSIDER_CMD_MEMRD:
		ringbuf_peek_buffer(&insider_rx_ring, 3,(uint8_t *)&dlen, 1);
		insider_memory_read(4, alen, dlen);
		break;

	case INSIDER_CMD_SET8:
		insider_memory_write(2, alen, 1);
		break;

	case INSIDER_CMD_SET16:
		insider_memory_write(2, alen, 2);
		break;

	case INSIDER_CMD_SET32:
		insider_memory_write(2, alen, 4);
		break;

	case INSIDER_CMD_MEMWR_EX:
		alen = 4;
		/* No break */

	case INSIDER_CMD_MEMWR:
		ringbuf_peek_buffer(&insider_rx_ring, 3,(uint8_t *)&dlen, 1);
		insider_memory_write(4, alen, dlen);
		break;

	case INSIDER_CMD_SET8MASK:
		insider_memory_write_mask(2, alen, 1);
		break;

	case INSIDER_CMD_SET16MASK:
		insider_memory_write_mask(2, alen, 2);
		break;

	case INSIDER_CMD_MEMWRMASK_EX:
		alen = 4;
		/* No break */

	case INSIDER_CMD_MEMWRMASK:
		ringbuf_peek_buffer(&insider_rx_ring, 3,(uint8_t *)&dlen, 1);
		insider_memory_write_mask(4, alen, dlen);
		break;

	/**********************************************************************/
	/* Default */
	default:
		insider_packet_reply(reply_err_invalid_cmd, 1);
		break;
	}
}


void insider_init(void)
{
	insider_packet_init();
}
