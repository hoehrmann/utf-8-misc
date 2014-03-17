/*-
 * Copyright (c) 2014 Taylor R Campbell
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>

#define	UTF8_ACCEPT	0
#define	UTF8_REJECT	0xf

static const uint32_t utf8_classtab[0x10] = {
	0x88888888UL,0x88888888UL,0x99999999UL,0x99999999UL,
	0xaaaaaaaaUL,0xaaaaaaaaUL,0xaaaaaaaaUL,0xaaaaaaaaUL,
	0x222222ffUL,0x22222222UL,0x22222222UL,0x22222222UL,
	0x3333333bUL,0x33433333UL,0xfff5666cUL,0xffffffffUL,
};

static const uint32_t utf8_statetab[0x10] = {
	0xfffffff0UL,0xffffffffUL,0xfffffff1UL,0xfffffff3UL,
	0xfffffff4UL,0xfffffff7UL,0xfffffff6UL,0xffffffffUL,
	0x33f11f0fUL,0xf3311f0fUL,0xf33f110fUL,0xfffffff2UL,
	0xfffffff5UL,0xffffffffUL,0xffffffffUL,0xffffffffUL,
};

static inline uint8_t
utf8_decode_step(uint8_t state, uint8_t octet, uint32_t *cpp)
{
	const uint8_t reject = (state >> 3), nonascii = (octet >> 7);
	const uint8_t class = (0xf & (nonascii | -nonascii) &
	    (utf8_classtab[(octet >> 3) & 0xf] >> (4 * (octet & 7))));
	const uint32_t trailer_p = (state | -state);

	*cpp = ((trailer_p & ((octet & 0x3fU) | (*cpp << 6)))
	     | (~trailer_p & (octet & (0xffU >> class))));

	return (0xf & (reject | -reject |
		(utf8_statetab[class] >> (4 * (state & 7)))));
}
