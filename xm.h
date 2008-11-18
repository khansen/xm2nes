/*
    This file is part of xm2nes.

    xm2nes is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    xm2nes is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with xm2nes.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XM_H
#define XM_H

#include <stdio.h>

struct xm_header {
    char id_text[17];
    char module_name[20];
    char pad1a;
    char tracker_name[20];
    unsigned short version;
    unsigned int header_size;
    unsigned short song_length;
    unsigned short restart_position;
    unsigned short channel_count;
    unsigned short pattern_count;
    unsigned short instrument_count;
    unsigned short flags;
    unsigned short default_tempo;
    unsigned short default_bpm;
    unsigned char pattern_order_table[256];
};

struct xm_pattern_slot {
    unsigned char note;
    unsigned char instrument;
    unsigned char volume;
    unsigned char effect_type;
    unsigned char effect_param;
};

struct xm_pattern {
    int row_count;
    struct xm_pattern_slot *data;
};

struct xm {
    struct xm_header header;
    struct xm_pattern *patterns;
};

#define XM_NO_ERROR 0
#define XM_FORMAT_ERROR 1
#define XM_VERSION_ERROR 2
#define XM_HEADER_SIZE_ERROR 3
#define XM_PREMATURE_END_OF_FILE_ERROR 4

int xm_read(FILE *, struct xm *);
void xm_print_header(const struct xm_header *, FILE *);
void xm_print_pattern(const struct xm *, int, FILE *);
void xm_destroy(struct xm *);

#endif
