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

#ifndef INSIDER_MEMORY_H_INCLUDED
#define INSIDER_MEMORY_H_INCLUDED

/******************************************************************************
* The API
*******************************************************************************/

void insider_memory_read(uint32_t apos, size_t alen, size_t dlen);
void insider_memory_write(uint32_t apos, size_t alen, size_t dlen);
void insider_memory_write_mask(uint32_t apos, size_t alen, size_t dlen);


#endif /* INSIDER_PACKET_H_INCLUDED */