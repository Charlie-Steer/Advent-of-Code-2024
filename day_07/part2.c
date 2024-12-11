#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#define NLINES 850
#define NUM_ARR_LEN 20

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

int get_num_len_in_base(int n, int base) {
	int i;
	for (i = 1; (n /= base) != 0; i++) {
		;
	}
	return (i);
}

char *ultoa(unsigned long number) {
	char *str = calloc(21, 1);
	int num_len = get_num_len_in_base(number, 10);
	while (number) {
		str[num_len--] = (number % 10) + '0';
		number /= 10;
	}

	return (str);
}

unsigned long get_operation_result(int *operands, char *operators, int n_operands) {
	unsigned long result = 0;
	for (int i = 0; i < n_operands; i++) {
		if (i == 0) {
			result = operands[0];
			continue;
		}
		unsigned int operator = operators[i - 1];
		printf("sequence: %s, operator: %c\n", operators, operator);
		if (operator == '0') {
			result += operands[i];
		}
		else if (operator == '1') {
			result *= operands[i];
		}
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
		}
		else {
			assert(0);
		}
	}
	return (result);
}

char *int_to_operators(int number, int n_operands) {
	int num_len = get_num_len_in_base(number, 3);
	printf("num_len: %d\n", num_len);

	char *str = malloc(n_operands);
	memset(str, '0', n_operands - 1);
	str[n_operands - 1] = '\0';
	printf("%d\n", number);

	for (int i = 0; i < num_len; i++) {
		str[i] = (number % 3) + '0';
		number /= 3;
	}
	printf("%s\n", str);

	return (str);
}

//! UNTESTED
char *get_next_operator_list(char *operators, int n_operands) {
	printf("operators: %s\n", operators);
	int n;
	if (operators == NULL) {
		n = strtoul("0", NULL, 3); //! What happens if operators is NULL?
	}
	else {
		n = strtoul(operators, NULL, 3); //! What happens if operators is NULL?
		free(operators);
	}
	printf("n: %d\n", n);
	n += 1;
	printf("n: %d\n", n);
	operators = int_to_operators(n, n_operands);
	printf("*operators: %s\n", operators);
	return (operators);
}

bool check_if_solution_matches_any_operator_combo(unsigned long solution, int *operands, int n_operands) {
	unsigned long number_of_combinations = pow(3, n_operands);
	char *operators = NULL;
	for (int j = 0; j < number_of_combinations; j++) {
		operators = get_next_operator_list(operators, n_operands);
		unsigned long result = get_operation_result(operands, operators, n_operands);
		// printf("solution: %lu, result: %lu\n", equation.solution, result);
		if (solution == result) {
			// printf("match: %lu\n", result);
			return (true);
			break;
		}
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
