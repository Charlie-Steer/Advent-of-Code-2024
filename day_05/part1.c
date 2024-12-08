#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ORD_ROWS 1176
#define ORD_COLS 2
#define UPD_ROWS 193
#define UPD_COLS 30
#define BUF_SIZE 100

void set_input_ordering(int input_ordering[ORD_ROWS][ORD_COLS]);
void set_input_updates(int input_updates[UPD_ROWS][UPD_COLS]);

bool check_if_pair_respects_ordering(int main_number, int main_index, int other_number, int other_index, int order_list[ORD_ROWS][ORD_COLS]) {
	bool is_pair_valid;
	if (other_number == main_number) {
		// printf("MAIN == OTHER\n");
		return (true);
	}
	for (int l = 0; l < ORD_ROWS; l++) {
		int a = order_list[l][0];
		int b = order_list[l][1];
		// printf("%d|%d\n", a, b);
		if (a == other_number && b == main_number) {
			// printf("%d is before %d per rule %d|%d\n", other_number, main_number, a, b);
			if (other_index < main_index) {
				// printf("VALID: %d|%d\n", a, b);
				return (true);
			}
			else {
				// printf("INVALID: %d|%d\n", a, b);
				return (false);
			}
		}
		else if (a == main_number && b == other_number) {
			// printf("%d is after %d per rule %d|%d\n", main_number, other_number, a, b);
			if (main_index < other_index) {
				// printf("VALID: %d|%d\n", a, b);
				return (true);
			}
			else {
				// printf("INVALID: %d|%d\n", a, b);
				return (false);
			}
		}
	}
	printf("PAIR NOT FOUND IN LIST!\nmain: %d, other: %d\n", main_number, other_number);
	assert(0);
}

bool check_if_update_valid(int update[UPD_COLS], int updates_list[UPD_ROWS][UPD_COLS], int order_list[ORD_ROWS][ORD_COLS]) {
	bool update_valid = true;

	for (int j = 0; update[j] != 0; j++) {
		printf("%d,", update[j]);
	}
	printf("\n");

	int main_number;
	int other_number;
	for (int j = 0; (main_number = update[j]) != 0; j++) {
		// printf("i: %d, j: %d\n", i, j);
		for (int k = 0; (other_number = update[k]) != 0; k++) {
			// printf("main: %d, other: %d\n", main_number, other_number);
			bool does_pair_respect_ordering = check_if_pair_respects_ordering(main_number, j, other_number, k, order_list);
			if (!does_pair_respect_ordering) {
				printf("NO\n");
				return (false);
			}
		}
	}
	printf("YES\n");
	return (true);
}

int get_middle_number(int update[UPD_COLS]) {
	int i;
	for (i = 0; update[i] != 0; i++) {
		;
	}
	printf("middle number: %d\n", update[i/2]);
	return (update[i/2]);
}

int get_sum_of_middle_numbers_of_valid_updates(int updates[UPD_ROWS][UPD_COLS], int orders[ORD_ROWS][ORD_COLS]) {
	int sum_of_middle_numbers_of_valid_updates = 0;
	for (int i = 0; i < UPD_ROWS; i++) {
		int *update = updates[i];
		bool is_update_valid = check_if_update_valid(update, updates, orders);
		if (is_update_valid) {
			sum_of_middle_numbers_of_valid_updates += get_middle_number(update);
		}
		printf("\n");
	}
	return (sum_of_middle_numbers_of_valid_updates);
}


int main(void) {
	int orders[ORD_ROWS][ORD_COLS];
	memset(orders, 0, ORD_ROWS * ORD_COLS);
	int updates[UPD_ROWS][UPD_COLS];
	memset(updates, 0, UPD_ROWS * UPD_COLS);

	set_input_ordering(orders);
	set_input_updates(updates);

	int sum = get_sum_of_middle_numbers_of_valid_updates(updates, orders);
	printf("answer a:\n%d\n", sum);

	return (0);
}

void set_input_ordering(int input_ordering[ORD_ROWS][ORD_COLS]) {
	FILE *file = fopen("input_ordering", "r");

	fseek(file, 0, SEEK_END);
	int nbytes = ftell(file);
	fseek(file, 0, SEEK_SET);

	char buffer[nbytes + 1];

	fread(buffer, 1, nbytes, file);
	buffer[nbytes] = 0;

	// printf("%s", buffer);
	
	for (int i = 0; i < ORD_ROWS; i++) {
		input_ordering[i][0] = strtol(&buffer[i * 6], NULL, 10);
		input_ordering[i][1] = strtol(&buffer[(i * 6) + 3], NULL, 10);
	}

	// for (int i = 0; i < ORD_ROWS; i++) {
	// 	printf("%d|%d\n", input_ordering[i][0], input_ordering[i][1]);
	// }
}

void set_input_updates(int input_updates[UPD_ROWS][UPD_COLS]) {
	FILE *file = fopen("input_updates", "r");

	fseek(file, 0, SEEK_END);
	int nbytes = ftell(file);
	fseek(file, 0, SEEK_SET);

	char buffer[BUF_SIZE];
	memset(buffer, 0, BUF_SIZE);

	char *fgets_return;
	int n_lines = 1;
	for (int i = 0; (fgets_return = fgets(buffer, BUF_SIZE, file)) != NULL; i++) {
		// printf("%p\n", fgets_return);
		n_lines++;
		if (fgets_return == NULL || fgets_return[0] == 0) {
			break;
		}
		else {
			char *token;
			token = strtok(buffer, ",\n");
			for (int j = 0; token != NULL; j++) {
				// printf("i: %d, j: %d\n", i, j);
				input_updates[i][j] = strtol(token, NULL, 10);
				token = strtok(NULL, ",\n");
				// printf("%d,", input_updates[i][j]);
			}
			memset(buffer, 0, BUF_SIZE);
		}
	}

	// for (int i = 0; i < n_lines; i++) {
	// 	for (int j = 0; input_updates[i][j] != 0; j++) {
	// 		char tail = ',';
	// 		if (input_updates[i][j + 1] == 0) {
	// 			tail = '\n';
	// 		}
	// 		printf("%d%c", input_updates[i][j], tail);
	// 	}
	// }
}
