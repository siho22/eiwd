/*
 *
 *  Wireless daemon for Linux
 *
 *  Copyright (C) 2015  Intel Corporation. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdbool.h>

#include <ell/ell.h>

#include "wsc.h"

void wsc_attr_iter_init(struct wsc_attr_iter *iter, const unsigned char *pdu,
			unsigned int len)
{
	iter->pdu = pdu;
	iter->max = len;
	iter->pos = 0;
}

bool wsc_attr_iter_next(struct wsc_attr_iter *iter)
{
	const unsigned char *start = iter->pdu + iter->pos;
	const unsigned char *end = iter->pdu + iter->max;
	unsigned short type;
	unsigned short len;

	/* Make sure we have at least type + len fields */
	if (iter->pos + 4 >= iter->max)
		return false;

	type = l_get_be16(start);
	start += 2;

	len = l_get_be16(start);
	start += 2;

	if (start + len > end)
		return false;

	iter->type = type;
	iter->len = len;
	iter->data = start;

	iter->pos = start + len - iter->pdu;

	return true;
}
