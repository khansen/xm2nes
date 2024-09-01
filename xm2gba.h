/*
    This file is part of xm2gba.

    xm2gba is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    xm2gba is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with xm2gba.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XM2GBA_H
#define XM2GBA_H

#include "xm.h"
#include "instrmap.h"

struct xm2gba_options {
    int channels;
    const struct instr_mapping *instr_map;
    const char *label_prefix;
    int order_start_offset;
    int order_end_offset;
};

void convert_xm_to_gba(const struct xm *,
                       const struct xm2gba_options *,
                       FILE *);

#endif
