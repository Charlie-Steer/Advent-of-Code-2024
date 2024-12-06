#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char **get_input_block(char *input_path);
int get_line_len(char *text);
int get_number_of_lines(char *text);
void DEBUG_print_block(char **block);

//M.S
//.A.
//M.S
int main(void) {
	char **block = get_input_block("input");
	// DEBUG_print_block(block);
	
	long x_mas_total = 0;
	for (int i = 0; block[i]; i++) {
		for (int j = 0; block[j]; j++) {
			if (block[i][j] == 'A') {
				int does_A_respect_bounds = false;
				int is_diagonal_a_valid = false;
				int is_diagonal_b_valid = false;

				if ((i > 0 && i < 139) && (j > 0 && j < 139)) {
					does_A_respect_bounds = true;
				}

				if (does_A_respect_bounds) {
					if (((block[i - 1][j - 1] == 'M' && block[i + 1][j + 1] == 'S')
					|| (block[i - 1][j - 1] == 'S' && block[i + 1][j + 1] == 'M'))) {
						is_diagonal_a_valid = true;
					}

					if ((block[i - 1][j + 1] == 'M' && block[i + 1][j - 1] == 'S')
					|| (block[i - 1][j + 1] == 'S' && block[i + 1][j - 1] == 'M')) {
						is_diagonal_b_valid = true;
					}

					if (is_diagonal_a_valid && is_diagonal_b_valid) {
						x_mas_total += 1;
					}
				}
			}
		}
	}
	printf("%ld\n", x_mas_total);

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
