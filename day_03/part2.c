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
		
		do_start = input;
		dont_start = input;
		while ((do_start && do_start < mul_start) || (dont_start && dont_start < mul_start)) {
			// printf("mul_start:  %.20s\n", mul_start);
			// printf("do_start:   %.20s\n", do_start);
			// printf("do_end:     %.20s\n", do_end);
			// printf("dont_start: %.20s\n", dont_start);
			// printf("dont_end:   %.20s\n", dont_end);
			if (do_end != NULL) {
				do_start = strstr(do_end, "do()");
			}
			if (dont_end != NULL) {
				dont_start = strstr(dont_end, "don't()");
			}
			if (do_start) {
				do_end = &do_start[4];
			}
			else {
				do_end = NULL;
			}
			if (dont_start) {
				dont_end = &dont_start[7];
			}
			else {
				dont_end = NULL;
			}
			// printf("mul_start:  %.20s\n", mul_start);
			// printf("do_start:   %.20s\n", do_start);
			// printf("dont_start: %.20s\n\n", dont_start);
			int mul_order = 1;
			int do_order = 1;
			int dont_order = 1;
			if (mul_start > do_start) {
				mul_order++;
			}
			else {
				do_order++;
			}
			if (mul_start > dont_start) {
				mul_order++;
			}
			else {
				dont_order++;
			}
			if (do_start > dont_start) {
				do_order++;
			}
			else {
				dont_order++;
			}
			printf("mul_start:  %d  %16ld\n", mul_order, (long)mul_start);
			printf("do_start:   %d  %16ld\n", do_order, (long)do_start);
			printf("dont_start: %d  %16ld\n\n", dont_order, (long)dont_start);
			if (do_start && do_start < mul_start && do_start > dont_start) {
				last_valid_do = do_start;
				is_do = true;
				printf("is_do set");
			}
			else {
				first_invalid_do = do_start;
			}

			if (dont_start && dont_start < mul_start && dont_start > do_start) {
				last_valid_dont = dont_start;
				is_do = false;
				printf("is_do unset");
			}
			else {
				first_invalid_dont = dont_start;
			}
		}

		if (!is_do) {
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
