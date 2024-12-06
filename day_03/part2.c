#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

char *get_input_buffer(char *file_path);

// "do()" and "don't()" enable or disable the following mults.
// only the last "do()" or "don't()" is in  effect.
int main(void) {
	char *input = get_input_buffer("input");
	char *input_start = input;
	// printf("%s", input);
	
	int input_len = strlen(input);

	long total = 0;
	char *mul_start;
	char *last_valid_do = NULL;
	char *last_valid_dont = NULL;
	char *first_invalid_do = NULL;
	char *first_invalid_dont = NULL;
	char *do_start = input;
	char *dont_start = input;
	char *do_end = input;
	char *dont_end = input;
	bool is_do = true;
	while ((mul_start = strstr(input, "mul("))) {
		printf("mul_start: %.10s\n", mul_start);
		
		do_start = input;
		dont_start = input;
		do_end = input;
		dont_end = input;
		while ((do_start && do_start < mul_start) || (dont_start && dont_start < mul_start)) {
			if (do_end) {
				do_start = strstr(do_end, "do()");
			}
			if (do_start) {
				do_end = &do_start[4];
			}
			if (dont_end) {
				dont_start = strstr(dont_end, "don't()");
			}
			if (dont_start) {
				dont_end = &dont_start[7];
			}

			if (do_start && do_start < mul_start) {
				printf("do found\n");
				last_valid_do = do_start;
			}
			if (dont_start && dont_start < mul_start) {
				printf("don't found\n");
				last_valid_dont = dont_start;
			}
		}
		if (last_valid_do && last_valid_do > last_valid_dont) {
			printf("do\n");
			is_do = true;
		}
		else if (last_valid_dont && last_valid_dont > last_valid_do) {
			printf("don't\n");
			is_do = false;
		}

		if (!is_do) {
			printf("continuing...\n");
			input = &mul_start[4];
			continue;
		}

		// printf("mul_start: %.5s\n", mul_start);
		char *start_of_a = &mul_start[4];
		char *start_of_b;
		char *end_of_a;
		char *end_of_b;
		int a;
		int b;

		a = strtol(start_of_a, &end_of_a, 10);
		if (a == 0 && end_of_a == start_of_a) {
			input = start_of_a;
			continue;
		}
		// printf("a: %d\nend_of_a: %.5s\n", a, end_of_a);

		if (end_of_a[0] == ',') {
			b = strtol(&end_of_a[1], &end_of_b, 10);
			// printf("b: %d\nend_of_b: %.5s\n", b, end_of_b);
		}
		else {
			input = end_of_a;
			continue;
		}
		if (end_of_b[0] == ')') {
			// printf("%.12s\n", mul_start);
			// printf("%d * %d\n\n", a, b);
			input = &end_of_b[1];
			total += a * b;
		}
		else {
			input = end_of_b;
		}
	}
	printf("%ld\n", total);
	free(input_start);
	return (0);
}


char *get_input_buffer(char *file_path) {
	FILE *input = fopen(file_path, "r");
	if (!input) {
		perror("Failed to open input");
		fclose(input);
		exit(1);
	}

	if (fseek(input, 0, SEEK_END)) {
		perror("Failed to set file position.");
		fclose(input);
		exit(1);
	}

	long file_len = ftell(input);
	if (file_len == -1) {
		perror("Error getting file length.");
		fclose(input);
		exit(1);
	}

	if (fseek(input, 0, SEEK_SET)) {
		perror("Failed to reset file position.");
		fclose(input);
		exit(1);
	}

	char *input_buffer = malloc(file_len + 1);
	if (!input_buffer) {
		perror("Failed to allocate memory.");
		exit(1);
	}

	// printf("%ld\n", file_len);
	int bytes_read = fread(input_buffer, 1, file_len, input);
	fclose(input);
	if (bytes_read != file_len) {
		fprintf(stderr, "Failed to read entire buffer. Bytes read: %d.\n", bytes_read);
		exit(1);
	}

	input_buffer[file_len] = '\0';

	// printf("%s", input_buffer);

	return (input_buffer);
}
