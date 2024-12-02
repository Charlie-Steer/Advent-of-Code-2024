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

enum order {
	ASCENDING,
	DESCENDING,
	EQUAL
};

int main() {
	int table[LINES][COLUMNS] = {0};

	set_table(table, "input");

	int number_of_safe_reports = get_number_of_safe_reports(table);
	// TEST_print_table(table);
	printf("number_of_safe_reports:\n%d\n", number_of_safe_reports);
	int number_of_safe_reports_with_problem_dampener = get_number_of_safe_reports_with_problem_dampener(table);
	printf("number_of_safe_reports_with_problem_dampener:\n%d\n", number_of_safe_reports_with_problem_dampener);

	return (0);
}

int get_number_of_safe_reports_with_problem_dampener(int table[LINES][COLUMNS]) {

	int number_of_safe_reports = 0;

	for (int i = 0; i < LINES; i++) {
		bool is_report_safe = true;
		bool is_difference_within_requirements = true;
		bool is_sequence_ordered = true;
		bool breaks_report_order = false;
		enum order report_order;

		for (int j = 1; table[i][j] != 0; j++) {
			int prev_value = table[i][j - 1];
			int current_value = table[i][j];

			int difference = abs(prev_value - current_value);
			if (!(difference >= MIN_DIFFERENCE && difference <= MAX_DIFFERENCE)) {
				is_difference_within_requirements = false;
				break;
			}

			if (j == 1) {
				if (prev_value < current_value) {
					report_order = ASCENDING;
					continue;
				}
				else if (prev_value > current_value) {
					report_order = DESCENDING;
					continue;
				}
				else {
					report_order = EQUAL;
					break;
				}
			}

			if ((prev_value < current_value && report_order == DESCENDING)
				|| prev_value > current_value && report_order == ASCENDING) {
				breaks_report_order = true;
				break;
			}
		}

		if (report_order == EQUAL || breaks_report_order) {
			is_sequence_ordered = false;
		}
		if (is_difference_within_requirements && is_sequence_ordered) {
			number_of_safe_reports += 1;
		}
		else {
			// get length of line.
			int row_length = 0;
			for (int j = 0; table[i][j] != 0; j++) {
				row_length++;
			}

			for (int index_to_ignore = 0; index_to_ignore < row_length; index_to_ignore++) {
				for (int j = 1; table[i][j] != 0; j++) {
					is_report_safe = false;
					is_difference_within_requirements = true;
					is_sequence_ordered = true;
					breaks_report_order = false;
					int current_value = table[i][j];
					int prev_value;
					if ((index_to_ignore == 0 && j == 1) || j == index_to_ignore) {
						continue;
					}
					else if (j - 1 == index_to_ignore) {
						if (j >= 2) {
							prev_value = table[i][j - 2];
						}
						else {
							continue;
						}
					}
					else {
						prev_value = table[i][j - 1];
					}

					int difference = abs(prev_value - current_value);
					if (!(difference >= MIN_DIFFERENCE && difference <= MAX_DIFFERENCE)) {
						is_difference_within_requirements = false;
						break;
					}

					if (index_to_ignore != 0 && j == 1
					|| (index_to_ignore == 0 && j == 2)) {
						if (prev_value < current_value) {
							report_order = ASCENDING;
							continue;
						}
						else if (prev_value > current_value) {
							report_order = DESCENDING;
							continue;
						}
						else {
							report_order = EQUAL;
							break;
						}
					}

					if ((prev_value < current_value && report_order == DESCENDING)
					|| prev_value > current_value && report_order == ASCENDING) {
						breaks_report_order = true;
						break;
					}
					if (is_difference_within_requirements && is_sequence_ordered) {
						is_report_safe = true;
						break;
					}
				}
				if (is_report_safe) {
					break;
				}
			}
			if (report_order == EQUAL || breaks_report_order) {
				is_sequence_ordered = false;
			}
			if (is_report_safe) {
				number_of_safe_reports += 1;
			}
		}
	}
	return (number_of_safe_reports);
}

int get_number_of_safe_reports(int table[LINES][COLUMNS]) {

	int number_of_safe_reports = 0;

	for (int i = 0; i < LINES; i++) {
		bool is_difference_within_requirements = true;
		bool is_sequence_ordered = true;
		bool breaks_report_order = false;
		enum order report_order;
		for (int j = 1; table[i][j] != 0; j++) {
			int prev_value = table[i][j - 1];
			int current_value = table[i][j];

			int difference = abs(prev_value - current_value);
			if (!(difference >= MIN_DIFFERENCE && difference <= MAX_DIFFERENCE)) {
				is_difference_within_requirements = false;
				break;
			}

			if (j == 1) {
				if (prev_value < current_value) {
					report_order = ASCENDING;
					continue;
				}
				else if (prev_value > current_value) {
					report_order = DESCENDING;
					continue;
				}
				else {
					report_order = EQUAL;
					break;
				}
			}

			if ((prev_value < current_value && report_order == DESCENDING)
			|| prev_value > current_value && report_order == ASCENDING) {
				breaks_report_order = true;
				break;
			}
		}

		if (report_order == EQUAL || breaks_report_order) {
			is_sequence_ordered = false;
		}
		if (is_difference_within_requirements && is_sequence_ordered) {
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
		for (int j = 0; token != NULL; j++) {
			table[i][j] = atoi(token);
			token = strtok(NULL, " \n");
		}
	}

	fclose(input);
}

void TEST_print_table(int table[LINES][COLUMNS]) {
	for (int i = 0; i < LINES; i++) {
		for (int j = 0; table[i][j] != '\0'; j++) {
			printf("%d ", table[i][j]);
		}
		printf("\n");
	}
}
