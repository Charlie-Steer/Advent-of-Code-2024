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

unsigned long get_operation_result(int *operands, unsigned int operators, int n_operands) {
	unsigned long result = 0;
	for (int i = 0; i < n_operands; i++) {
		unsigned int operator = operators & 1;
		printf("sequence: %d, operator: %d\n", operators, operator);
		operators >>= 1;
		if (i == 0) {
			result = operands[0];
			continue;
		}
		if (operator == 0) {
			result += operands[i];
		}
		else if (operator == 1) {
			result *= operands[i];
		}
		else {
			assert(0);
		}
	}
	return (result);
}

bool check_if_solution_matches_any_operator_combo(unsigned long solution, int *operands, int n_operands) {
	unsigned long number_of_combinations = pow(2, n_operands);
	for (int j = 0; j < number_of_combinations; j++) {
		int operators = j;	// byte sequence corresponding to a comibation of additions (0) and multiplications (1).
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

//! Bits standing for operators?
// 56246509072 too low.
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
		printf("%ld:", equations[i].solution);
		equations[i].operands = malloc(NUM_ARR_LEN * sizeof(int));
		memset(equations[i].operands, -1, NUM_ARR_LEN * sizeof(int));
		char *operand;
		for (int j = 0; (operand = strtok(NULL, " ")) != NULL; j++) {
			equations[i].operands[j] = strtol(operand, NULL, 10);
			printf(" %d", equations[i].operands[j]);
		}
		printf("\n");
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
