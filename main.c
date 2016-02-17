/*
 * randfreq - byte frequency count
 *
 * Copyright (C) 2016  xdevelnet (xdevelnet at xdevelnet dot org)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <signal.h>

#define DEFAULT_BLOCK_DEV "/dev/urandom"
#define CHARS_COUNT 256 // pow(2, sizeof(char)*8);
#define CHARS_COUNT_4_INTEGER 64

char done = 0;

void term_handler(int i) {
	done = 1;
}

void flush_arr_count (unsigned long int *ptr) {
	int position = 0;
	while (position < CHARS_COUNT_4_INTEGER) {
		printf("|%4u | %20lu |%4u | %20lu |%4u | %20lu |%4u | %20lu\n",
			   position, ptr[position],
			   position+CHARS_COUNT_4_INTEGER, ptr[position+CHARS_COUNT_4_INTEGER],
			   position+CHARS_COUNT_4_INTEGER*2, ptr[position+CHARS_COUNT_4_INTEGER*2],
			   position+CHARS_COUNT_4_INTEGER*3, ptr[position+CHARS_COUNT_4_INTEGER*3]
		);
		position++;
	}
}

int main(int argc, char **argv) {
	signal(SIGTERM, term_handler);
	signal(SIGINT, term_handler);

	FILE *input_stream = NULL;
	if ( argc == 2 ) {
		input_stream = fopen(argv[1], "rb");
		if (input_stream == NULL) {
			perror(argv[1]);
			fprintf(stderr, "%s %s\n", "Using", DEFAULT_BLOCK_DEV);
		}
	}
	if ( input_stream == NULL ) {
		input_stream = fopen(DEFAULT_BLOCK_DEV, "rb");
		if (input_stream == NULL) {
			perror(DEFAULT_BLOCK_DEV);
			exit(EXIT_FAILURE);
		}
	}
	//setbuf(input_stream, NULL);
	printf("%c[?25l%c[2J", 0x1b, 0x1b);
	int c;
	unsigned long int *numbers = malloc(sizeof(unsigned long int)*CHARS_COUNT);
	unsigned long int all_chars_count = 0;
	if (system("stty -ctlecho") == -1) {
		fprintf(stderr, "Can't disable Ctrl+C indication.\n");
	}
	while ((c = getc(input_stream)) != EOF) {
		numbers[c]++;
		all_chars_count++;
		printf("%c%s", 0x1b, "[2;0H");
		flush_arr_count(numbers);
		printf("\nCount:%lu\n", all_chars_count);
		if (done == 1) break;
	}
	printf("%c[?25h", 0x1b);
	if (system("stty sane") == -1) {
		fprintf(stderr, "Can't restore tty settings.\n");
	}
	return EXIT_SUCCESS;
}
