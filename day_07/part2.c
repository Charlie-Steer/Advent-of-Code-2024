#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#define NLINES 850
#define NUM_ARR_LEN 20
#define OP_BUFF_LEN 20

typedef struct equation {
	long solution;
	int *operands;
} equation;

equation *get_equations(const char *file_path);

int get_number_of_operands(int *operands) {
	int i;
	for (i = 0; operands[i] != -1; i++) {
		;
	}
	return (i);
}

int get_num_len_in_base(unsigned long n, int base) {
	int i;
	for (i = 1; (n /= base) != 0; i++) {
		;
	}
	return (i);
}

char *ultoa(unsigned long number) {
	char *str = calloc(21, 1);
	int num_len = get_num_len_in_base(number, 10);
	// printf("num_len: %d\n", num_len);
	for (int i = num_len - 1; number; i--) {
		// printf("mod: %c\n", (int)(number % 10) + '0');
		str[i] = (number % 10) + '0';
		// printf("i: %d, str[i]: %c\n", i, str[i]);
		number /= 10;
	}
	str[num_len] = '\0';

	return (str);
}

unsigned long get_operation_result(int *operands, char *operators, int n_operands) {
	unsigned long result = 0;
	for (int i = 0; i < n_operands; i++) {
		if (i == 0) {
			result = operands[0];
			continue;
		}
		unsigned int operator = operators[OP_BUFF_LEN - i];
		if (operator == '0') {
			result += operands[i];
		}
		else if (operator == '1') {
			result *= operands[i];
		}
		//! Concat not working.
		else if (operator == '2') {
			char *ascii_result = ultoa(result);
			char *ascii_operand = ultoa(operands[i]);
			if (strlen(ascii_result) + strlen(ascii_operand) > 20) {
				printf("Number would go out of bounds for a long integer on concatenation.");
				printf("res: %s\n", ascii_result);
				printf("ope: %s\n", ascii_operand);
				exit(1);
			}
			result = strtoul(strncat(ascii_result, ascii_operand, 20), NULL, 10);
			free(ascii_result);
			free(ascii_operand);
		}
		else {
			assert(0);
		}
		// printf("result: %lu\n", result);
	}
	return (result);
}

char *int_to_operators(int number, int n_operands) {
	static char *str = NULL;
	if (str == NULL) {
		str = malloc(OP_BUFF_LEN + 1);
	}

	if (number == 0) {
		memset(str, '0', OP_BUFF_LEN);
		str[OP_BUFF_LEN] = '\0';
	}
	int num_len = get_num_len_in_base(number, 3);

	for (int i = 0; i < num_len; i++) {
		str[OP_BUFF_LEN - 1 - i] = (number % 3) + '0';
		number /= 3;
	}

	return (str);
}

//! UNTESTED
char *get_next_operator_list(char *operators, int n_operands) {
	int n;
	if (operators == NULL) {
		// n = strtoul("0", NULL, 3);
		operators = int_to_operators(0, n_operands);
		return (operators);
	}
	else {
		n = strtoul(operators, NULL, 3);
		// free(operators);
	}
	n += 1;
	operators = int_to_operators(n, n_operands);
	return (operators);
}

bool check_if_solution_matches_any_operator_combo(unsigned long solution, int *operands, int n_operands) {
	unsigned long number_of_combinations = pow(3, n_operands);
	char *operators = NULL;
	printf("%lu:", solution);
	for (int i = 0; operands[i] != -1; i++) {
		printf(" %d", operands[i]);
	}
	printf("\n");
	for (int j = 0; j < number_of_combinations; j++) {
		operators = get_next_operator_list(operators, n_operands);
		// printf("operators: %s\n", operators);
		unsigned long result = get_operation_result(operands, operators, n_operands);
		// printf("solution: %lu, result: %lu\n", equation.solution, result);
		if (solution == result) {
			printf("MATCH: %lu\n", result);
			printf("operators: %s\n\n", operators);
			return (true);
			break;
		}

		// if (j == 10) {
		// 	assert(0);
		// }
	}
	return (false);
}

int main(void) {
	equation *equations = get_equations("input");

	unsigned long answer = 0;
	for (int i = 0; i < NLINES; i++) {
		equation equation = equations[i];
		int *operands = equation.operands;

		int n_operands = get_number_of_operands(operands);
		bool does_result_match = check_if_solution_matches_any_operator_combo(equation.solution, operands, n_operands);
		if (does_result_match) {
			answer += equation.solution;
		}
		// assert(0);
	}

	printf("answer:\n%lu\n", answer);

	// equation *equations = get_equations("input");
	return (0);
}

equation *get_equations(const char *file_path) {
	FILE *file = fopen(file_path, "r");

	equation *equations = malloc(NLINES * sizeof(equation));
	if (equations == NULL) {
		perror("malloc error.");
		exit (1);
	}

	char *line = NULL;
	size_t line_size = 0;

	for (int i = 0; i < NLINES; i++) {
		memset(&equations[i], 0, sizeof(equation));
		int line_len = getline(&line, &line_size, file);
		if (line_len < 0) {
			perror("Couldn't read line");
			exit (1);
		}

		equations[i].solution = strtol(strtok(line, ": "), NULL, 10);
		// printf("%ld:", equations[i].solution);
		equations[i].operands = malloc(NUM_ARR_LEN * sizeof(int));
		memset(equations[i].operands, -1, NUM_ARR_LEN * sizeof(int));
		char *operand;
		for (int j = 0; (operand = strtok(NULL, " ")) != NULL; j++) {
			equations[i].operands[j] = strtol(operand, NULL, 10);
			// printf(" %d", equations[i].operands[j]);
		}
		// printf("\n");
	}

	// for (int i = 0; i < NLINES; i++) {
	// 	for (int j = 0; j < NUM_ARR_LEN; j++) {
	// 		printf("%d ", equations[i].operands[j]);
	// 	}
	// 	printf("\n");
	// }

	fclose(file);
	
	return (equations);
}
