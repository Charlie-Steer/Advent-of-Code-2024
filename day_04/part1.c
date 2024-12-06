#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **get_input_block(char *input_path);
int get_line_len(char *text);
int get_number_of_lines(char *text);
void DEBUG_print_block(char **block);

int main(void) {
	char **block = get_input_block("input");
	// DEBUG_print_block(block);
	
	long xmas_total = 0;
	for (int i = 0; block[i]; i++) {
		for (int j = 0; block[j]; j++) {
			if (block[i][j] == 'X') {
				if (j <= 136 && strncmp(&block[i][j], "XMAS", 4) == 0) {
					xmas_total += 1;
				}
				if (j >= 3 && strncmp(&block[i][j - 3], "SAMX", 4) == 0) {
					xmas_total += 1;
				}
				// if (j >= 3 && block[i][j - 1] == 'M' && block[i][j - 2] == 'A' && block[i][j - 3] == 'S') {
				// 	xmas_total += 1;
				// }
				//! Add boundaries protection.
				if (i <= 136 && block[i + 1][j] == 'M' && block[i + 2][j] == 'A' && block[i + 3][j] == 'S') {
					xmas_total += 1;
				}
				if (i >= 3 && block[i - 1][j] == 'M' && block[i - 2][j] == 'A' && block[i - 3][j] == 'S') {
					xmas_total += 1;
				}

				// Diagonals
				if (i <= 136 && j <= 136 && block[i + 1][j + 1] == 'M' && block[i + 2][j + 2] == 'A' && block[i + 3][j + 3] == 'S') {
					xmas_total += 1;
				}
				if (i >= 3 && j >= 3 && block[i - 1][j - 1] == 'M' && block[i - 2][j - 2] == 'A' && block[i - 3][j - 3] == 'S') {
					xmas_total += 1;
				}

				if (i <= 136 && j >= 3 && block[i + 1][j - 1] == 'M' && block[i + 2][j - 2] == 'A' && block[i + 3][j - 3] == 'S') {
					xmas_total += 1;
				}
				if (i >= 3 && j <= 136 && block[i - 1][j + 1] == 'M' && block[i - 2][j + 2] == 'A' && block[i - 3][j + 3] == 'S') {
					xmas_total += 1;
				}
			}
		}
	}
	printf("%ld\n", xmas_total);

	return (0);
}

char **get_input_block(char *input_path) {
	FILE *file = fopen("input", "r");

	fseek(file, 0, SEEK_END);
	int nbytes = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = malloc(nbytes + 1);

	fread(buffer, 1, nbytes, file);

	buffer[nbytes] = 0;

	// Read into malloc'd 2d array.
	
	int line_len = get_line_len(buffer);
	int n_lines = get_number_of_lines(buffer);

	char **block = malloc((n_lines + 1) * sizeof(char *));
	if (!block) {
		perror("Failed to allocate memory.");
		exit(1);
	}

	for (int i = 0; i < n_lines; i++) {
		block[i] = malloc(line_len + 1);
		if (!block) {
			perror("Failed to allocate memory.");
			exit(1);
		}
	}

	for (int i = 0; i < n_lines; i++) {
		for (int j = 0; j <= line_len; j++) {
			if (buffer[(i * (line_len + 1)) + j] == '\n') {
				block[i][j] = '\0';
			}
			else {
				block[i][j] = buffer[(i * (line_len + 1)) + j];
			}
		}
	}
	block[n_lines] = NULL;

	return (block);
}

int get_line_len(char *text) {
	int i;
	for (i = 0; text[i] != '\n'; i++) {
		;
	}
	return (i);
}

int get_number_of_lines(char *text) {
	int newline_number = 0;

	for (int i = 0; text[i] != 0; i++) {
		if (text[i] == '\n') {
			newline_number++;
		}
	}

	return (newline_number);
}

void DEBUG_print_block(char **block) {
	for (int i = 0; block[i]; i++) {
		for (int j = 0; block[i][j]; j++) {
			printf("%c", block[i][j]);
		}
		printf("\n");
	}
}
