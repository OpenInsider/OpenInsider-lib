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

#ifndef INSIDER_PROTOCOL_H_INCLUDED
#define INSIDER_PROTOCOL_H_INCLUDED

/******************************************************************************
* Packet protocol constants 
*******************************************************************************/

#define INSIDER_PACKET_START			'+'
#define INSIDER_PACKET_HASLEN(cmd)		(((cmd) & (3 << 6)) == (3 << 6))
#define INSIDER_PACKET_LEN(cmd)			(((cmd) >> 3) & 6)

/******************************************************************************
* Packet protocol Queries 
*******************************************************************************/

#define INSIDER_CMD_MEMRD			0x01
#define INSIDER_CMD_MEMWR			0x02
#define INSIDER_CMD_MEMWRMASK			0x03
#define INSIDER_CMD_MEMRD_EX			0x04
#define INSIDER_CMD_MEMWR_EX			0x05
#define INSIDER_CMD_MEMWRMASK_EX		0x06
#define INSIDER_CMD_SCOPE_SETUP			0x08
#define INSIDER_CMD_RECORDER_SETUP		0x09
#define INSIDER_CMD_SCOPE_SETUP_EX		0x0A
#define INSIDER_CMD_RECORDER_SETUP_EX		0x0B
/* codes 0C - 0F Reserved for future use */
#define INSIDER_CMD_USER			0x10
/* codes 11 - BF Reserved for future use */
#define INSIDER_CMD_GET_INFO			0xC0
#define INSIDER_CMD_RECORDER_START		0xC1
#define INSIDER_CMD_RECORDER_STOP		0xC2
#define INSIDER_CMD_RECORDER_STATUS		0xC3
#define INSIDER_CMD_RECORDER_GETBUF		0xC4
#define INSIDER_CMD_SCOPE_READ			0xC5
#define INSIDER_CMD_USER_STATUS			0xC6
/* code C7 Reserved for future use */
#define INSIDER_CMD_GET_INFO_BRIEF		0xC8
#define INSIDER_CMD_RECORDER_GETBUF_EX		0xC9
/* codes CA - CF Reserved for future use */
#define INSIDER_CMD_GET8			0xD0
#define INSIDER_CMD_GET16			0xD1
#define INSIDER_CMD_GET32			0xD2
/* codes D3 - DF Reserved for future use */
#define INSIDER_CMD_GET8_EX			0xE0
#define INSIDER_CMD_GET16_EX			0xE1
#define INSIDER_CMD_GET32_EX			0xE2
#define INSIDER_CMD_SET8			0xE3
#define INSIDER_CMD_SET16			0xE4
#define INSIDER_CMD_SET8MASK			0xE5
/* codes E6 - EF Reserved for future use */
#define INSIDER_CMD_SET32			0xF0
#define INSIDER_CMD_SET16MASK			0xF1
/* codes F2 - FF Reserved for future use */

/******************************************************************************
* Packet protocol Responses
*******************************************************************************/

#define INSIDER_RSP_ERROR			(1 << 7)

/* confirmation codes */
#define INSIDER_RSP_SUCCESS			0x00
#define INSIDER_RSP_RECORDER_RUN		0x01
#define INSIDER_RSP_RECORDER_DONE		0x02

/* error codes */
#define INSIDER_RSP_ERR_INVALID_CMD		(INSIDER_RSP_ERROR | 1)
#define INSIDER_RSP_ERR_CHECKSUM		(INSIDER_RSP_ERROR | 2)
#define INSIDER_RSP_ERR_OVERFLOW_IN		(INSIDER_RSP_ERROR | 3)
#define INSIDER_RSP_ERR_OVERFLOW_OUT		(INSIDER_RSP_ERROR | 4)
#define INSIDER_RSP_ERR_INVALID_BUFF		(INSIDER_RSP_ERROR | 5)
#define INSIDER_RSP_ERR_INVALID_SIZE		(INSIDER_RSP_ERROR | 6)
#define INSIDER_RSP_ERR_BUSY			(INSIDER_RSP_ERROR | 7)
#define INSIDER_RSP_ERR_UNINITIALIZED		(INSIDER_RSP_ERROR | 8)

/******************************************************************************
* The API
*******************************************************************************/

void insider_protocol_parse(void);
void insider_init(void);

#endif /* INSIDER_PROTOCOL_H_INCLUDED */