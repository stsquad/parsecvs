/*
 *  Copyright © 2006 Keith Packard <keithp@keithp.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#include "cvs.h"
#include <stdint.h>

typedef uint32_t	crc32_t;

static crc32_t crc32_table[256];

static void
generate_crc32_table(void)
{
    crc32_t	c, p;
    int		n, m;

    p = 0xedb88320;
    for (n = 0; n < 256; n++)
    {
	c = n;
	for (m = 0; m < 8; m++)
	    c = (c >> 1) ^ ((c & 1) ? p : 0);
	crc32_table[n] = c;
    }
}

static crc32_t
crc32 (char *string)
{
    crc32_t		crc32 = ~0;
    unsigned char	c;

    if (crc32_table[1] == 0) generate_crc32_table ();
    while ((c = (unsigned char) *string++))
	crc32 = (crc32 >> 8) ^ crc32_table[(crc32 ^ c) & 0xff];
    return ~crc32;
}

#define HASH_SIZE	9013

typedef struct _hash_bucket {
    struct _hash_bucket	*next;
    crc32_t		crc;
    char		string[0];
} hash_bucket_t;

static hash_bucket_t	*buckets[HASH_SIZE];

char *
atom (char *string)
{
    crc32_t		crc = crc32 (string);
    hash_bucket_t	**head = &buckets[crc % HASH_SIZE];
    hash_bucket_t	*b;

    while ((b = *head)) {
	if (b->crc == crc && !strcmp (string, b->string))
	    return b->string;
	head = &(b->next);
    }
    b = malloc (sizeof (hash_bucket_t) + strlen (string) + 1);
    b->next = 0;
    b->crc = crc;
    strcpy (b->string, string);
    *head = b;
    return b->string;
}
