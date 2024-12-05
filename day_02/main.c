#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINES 1000
#define COLUMNS 128
#define MIN_DIFFERENCE 1
#define MAX_DIFFERENCE 3

void set_table(int table[LINES][COLUMNS], char *file_name);
void TEST_print_table(int table[LINES][COLUMNS]);
int get_number_of_safe_reports(int table[LINES][COLUMNS]);
int get_number_of_safe_reports_with_problem_dampener(int table[LINES][COLUMNS]);
int get_number_of_safe_reports_with_dampener(int table[LINES][COLUMNS]);

enum order {
	ASCENDING,
	DESCENDING,
	EQUAL
};

int main() {
	int table[LINES][COLUMNS] = {0};

	set_table(table, "input");
	// TEST_print_table(table);

	int number_of_safe_reports = get_number_of_safe_reports(table);
	printf("part 1:\n%d\n\n", number_of_safe_reports);
	int number_of_safe_reports_with_dampener = get_number_of_safe_reports_with_dampener(table);
	printf("part 2:\n%d\n", number_of_safe_reports_with_dampener);

	return (0);
}

bool determine_if_pair_difference_correct(int a, int b) {
	int difference = abs(a - b);
	if (difference >= MIN_DIFFERENCE && difference <= MAX_DIFFERENCE) {
		return (true);
	}
	else {
		return (false);
	}
}

bool determine_if_report_safe(int report[COLUMNS]) {
	enum order starting_order;
	for (int i = 1; report[i] != -1; i++) {
		bool is_pair_difference_correct = determine_if_pair_difference_correct(report[i], report[i - 1]);
		if (!is_pair_difference_correct) {
			return (false);
		}
		
		// determine_initial_ordering
		if (i == 1) {
			if (report[i - 1] < report[i]) {
				starting_order = ASCENDING;
			}
			else if (report[i - 1] > report[i]) {
				starting_order = DESCENDING;
			}
			else {
				return (false);
			}
		}
		// is_initial_ordering_respected
		else {
			if (report[i - 1] < report[i] && starting_order == ASCENDING) {
				continue;
			}
			else if (report[i - 1] > report[i] && starting_order == DESCENDING) {
				continue;
			}
			else {
				return (false);
			}
		}
	}
	return (true);
}

int get_report_len(int report[COLUMNS]) {
	int i;
	for (i = 0; report[i] != -1; i++)
		;
	return (i);
}

enum order get_pair_order(int first_number, int second_number) {
	if (first_number < second_number) {
		return (ASCENDING);
	}
	else if (first_number > second_number) {
		return (DESCENDING);
	}
	else {
		return (EQUAL);
	}
}

void DEBUG_report_status(int report[COLUMNS], int report_len, int index, int skip_index, int index_a, int index_b, enum order start_order, char *error_message);

bool determine_if_report_safe_with_index_mask(int report[COLUMNS], int skip_index) {
	enum order starting_order;
	bool is_report_safe = true;
	int first_iteration = true;
	for (int i = 1; report[i] != -1; i++) {
		// set_indeces
		int index_a;
		int index_b;
		if (i - 1 == skip_index) {
			continue;
		}
		if (i == skip_index) {
			if (report[i + 1] == -1) {
				break;
			}
			else {
				index_a = i - 1;
				index_b = i + 1;
			}
		}
		else {
			index_a = i - 1;
			index_b = i;
		}

		// determine_initial_ordering
		if (first_iteration) {
			first_iteration = false;
			starting_order = get_pair_order(report[index_a], report[index_b]);
			if (starting_order == EQUAL) {
				// printf("Starting order incorrect.\n");
				is_report_safe = false;
				break;
			}
		}
		// is_initial_ordering_respected
		else {
			if (get_pair_order(report[index_a], report[index_b]) != starting_order) {
				// printf("Inconsistent order.\n");
				is_report_safe = false;
				break;
			}
		}

		bool is_pair_difference_correct = determine_if_pair_difference_correct(report[index_a], report[index_b]);
		if (!is_pair_difference_correct) {
			// printf("Pair difference incorrect.\n");
			is_report_safe = false;
			break;
		}
	}

	if (is_report_safe) {
		return (true);
	}
	else {
		return (false);
	}
}

int determine_if_report_safe_with_dampener(int report[COLUMNS]) {
	// test_report_with_dampener
	int report_len = get_report_len(report);
	int i, skip_index, index_a, index_b;
	for (skip_index = 0; skip_index < report_len; skip_index++) {
		int is_report_safe = determine_if_report_safe_with_index_mask(report, skip_index);
		if (is_report_safe) {
			// DEBUG_report_status(report, report_len, i, skip_index, index_a, index_b, starting_order, "SAFE");
			return (true);
		}
		else {
			continue;
		}
	}
	// DEBUG_report_status(report, report_len, i, skip_index, index_a, index_b, starting_order, "UNSAFE");
	return (false);
}

int get_number_of_safe_reports_with_dampener(int table[LINES][COLUMNS]) {
	int number_of_safe_reports = 0;
	for (int i = 0; i < LINES; i++) {
		if (determine_if_report_safe(table[i])) {
			number_of_safe_reports += 1;
		}
		else if (determine_if_report_safe_with_dampener(table[i])) {
			number_of_safe_reports += 1;
		}
	}
	return (number_of_safe_reports);
}

int get_number_of_safe_reports(int table[LINES][COLUMNS]) {
	int number_of_safe_reports = 0;

	for (int i = 0; i < LINES; i++) {
		if (determine_if_report_safe(table[i])) {
			number_of_safe_reports += 1;
		}
	}
	return (number_of_safe_reports);
}

void set_table(int table[LINES][COLUMNS], char *file_name) {
	FILE *input = fopen(file_name, "r");
	if (!input) {
		perror("Error opening file.");
		exit (1);
	}

	char buffer[COLUMNS];
	for (int i = 0; fgets(buffer, sizeof(buffer), input) != NULL; i++) {
		char *token;
		token = strtok(buffer, " \n");
		int j;
		for (j = 0; token != NULL; j++) {
			table[i][j] = atoi(token);
			token = strtok(NULL, " \n");
		}
		table[i][j] = -1;
	}

	fclose(input);
}

void TEST_print_table(int table[LINES][COLUMNS]) {
	for (int i = 0; i < LINES; i++) {
		for (int j = 0; table[i][j] != -1; j++) {
			printf("%d ", table[i][j]);
		}
		printf("\n");
	}
}

void DEBUG_report_status(int report[COLUMNS], int report_len, int index, int skip_index, int index_a, int index_b, enum order start_order, char *error_message) {
	printf("original_report:\n");
	for (int i = 0; i < report_len; i++) {
		printf("%d ", report[i]);
	}
	printf("\n");

	printf("error: %s\n", error_message);
	printf("report_len: %d\n", report_len);
	printf("i:          %d\n", index);
	printf("skip_index: %d\n", skip_index);
	printf("index_a:    %d\n", index_a);
	printf("index_b:    %d\n", index_b);
	if (start_order == ASCENDING)
		printf("start_order:    %s\n", "ASCENDING");
	else if (start_order == DESCENDING)
		printf("start_order:    %s\n", "DESCENDING");
	else if (start_order == EQUAL)
		printf("start_order:    %s\n", "EQUAL");
	else
		printf("ERROR: START ORDER IS INVALID.");
	printf("\n");
}
