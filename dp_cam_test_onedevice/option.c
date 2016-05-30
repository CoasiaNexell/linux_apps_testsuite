#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "option.h"

int handle_option(int argc, char **argv, uint32_t *w,
		  uint32_t *h, uint32_t *c, char *d)
{
	int opt;

	while ((opt = getopt(argc, argv, "w:h:c:d:")) != -1) {
		switch (opt) {
		case 'w':
			*w = atoi(optarg);
			break;
		case 'h':
			*h = atoi(optarg);
			break;
		case 'c':
			*c = atoi(optarg);
			break;
		case 'd':
			strcpy(d, optarg);
			break;
		}
	}

	return 0;
}
