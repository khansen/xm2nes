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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "xm.h"

extern void convert_xm_to_nes(const struct xm *, int, const char *, FILE *);

static char program_version[] = "xm2nes 1.0";

/* Prints usage message and exits. */
static void usage()
{
    printf(
        "Usage: xm2nes [--output=FILE] [--channels=CHANNELS]\n"
        "              [--verbose]\n"
        "              [--help] [--usage] [--version]\n"
        "              FILE\n");
    exit(0);
}

/* Prints help message and exits. */
static void help()
{
    printf("Usage: xm2nes [OPTION...] FILE\n"
           "xm2nes converts Fasttracker ][ eXtended Module (XM) files to Kent's NES music format.\n\n"
           "Options:\n\n"
           "  --output=FILE                   Store output in FILE\n"
           "  --channels=CHANNELS             Process only CHANNELS (0,1,2,3,4)\n"
           "  --verbose                       Print progress information to standard output\n"  
           "  --help                          Give this help list\n"
           "  --usage                         Give a short usage message\n"
           "  --version                       Print program version\n");
    exit(0);
}

/* Prints version and exits. */
static void version()
{
    printf("%s\n", program_version);
    exit(0);
}

/**
  Program entrypoint.
*/
int main(int argc, char *argv[])
{
    int verbose = 0;
    const char *input_filename = 0;
    const char *output_filename = 0;
    int channels = 0x1F;
    /* Process arguments. */
    {
        char *p;
        while ((p = *(++argv))) {
            if (!strncmp("--", p, 2)) {
                const char *opt = &p[2];
                if (!strncmp("output=", opt, 7)) {
                    output_filename = &opt[7];
                } else if (!strncmp("channels=", opt, 9)) {
                    const char *p = &opt[9];
                    channels = 0;
                    if (*p) {
                        channels |= 1 << (*p - '0');
                        while (*(++p)) {
                            if (*(p++) != ',')
                                break;
                            if (*p)
                                channels |= 1 << (*p - '0');
			}
		    }
                    channels &= 0x1F;
                } else if (!strcmp("verbose", opt)) {
                    verbose = 1;
                } else if (!strcmp("help", opt)) {
                    help();
                } else if (!strcmp("usage", opt)) {
                    usage();
                } else if (!strcmp("version", opt)) {
                    version();
                } else {
                    fprintf(stderr, "xm2nes: unrecognized option `%s'\n"
			    "Try `xm2nes --help' or `xm2nes --usage' for more information.\n", p);
                    return(-1);
                }
            } else {
                input_filename = p;
            }
        }
    }

    if (!input_filename) {
        fprintf(stderr, "xm2nes: no filename given\n"
                        "Try `xm2nes --help' or `xm2nes --usage' for more information.\n");
        return(-1);
    }

    if (!channels) {
        fprintf(stderr, "xm2nes: --channels argument needs to include at least one channel\n");
        return(-1);
    }

    {
        struct xm xm;
        FILE *out;
        if (!output_filename)
            out = stdout;
        else {
            out = fopen(output_filename, "wt");
            if (!out) {
                fprintf(stderr, "xm2nes: failed to open `%s' for writing\n", output_filename);
                return(-1);
            }
        }

        {
            FILE *in;
            in = fopen(input_filename, "rb");
            if (!in) {
                fprintf(stderr, "xm2nes: failed to open `%s' for reading\n", input_filename);
                return(-1);
            }
            if (verbose)
                fprintf(stdout, "Reading `%s'...\n", input_filename);
            xm_read(in, &xm);
            if (verbose)
                fprintf(stdout, "OK.\n");
        }

        if (verbose)
            xm_print_header(&xm.header, stdout);

        if (verbose)
            fprintf(stdout, "Converting...\n");

        {
            char *prefix;
            int len;
            char *last_dot = strrchr(input_filename, '.');
            if (!last_dot)
                len = strlen(input_filename);
            else
                len = last_dot - input_filename;
            prefix = (char *)malloc(len + 2);
            prefix[len] = '_';
            prefix[len+1] = '\0';
            strncpy(prefix, input_filename, len);

            convert_xm_to_nes(&xm, channels, prefix, out);

            free(prefix);
        }

        if (verbose)
            fprintf(stdout, "Done.\n");

        xm_destroy(&xm);
    }
    return 0;
}
