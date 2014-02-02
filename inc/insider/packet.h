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

#ifndef INSIDER_PACKET_H_INCLUDED
#define INSIDER_PACKET_H_INCLUDED

#include "insider/ringbuf.h"
#include "insider/buffer.h"


/******************************************************************************
* The API
*******************************************************************************/


extern struct ringbuf insider_rx_ring;
extern struct ringbuf insider_tx_ring;

void insider_packet_init(void);
bool insider_packet_parse(void);
void insider_packet_reply(uint8_t code, const uint8_t *data, size_t length);


#endif /* INSIDER_PACKET_H_INCLUDED */
