#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIST_LEN 1000

void bubble_sort(int *array, int len);
int get_total_distance(int *list_a, int *list_b, int len);
int part_1(int *list_a, int *list_b, int len);
int part_2(int *list_a, int *list_b, int len);

int main(void) {
	int fd_a = open("input1", O_RDONLY);
	int fd_b = open("input2", O_RDONLY);

	long bytes_read_a;
	long bytes_read_b;

	char *line_from_a = malloc(6);
	char *line_from_b = malloc(6);
	int list_a[LIST_LEN];
	memset(list_a, 0, LIST_LEN * sizeof(int));
	int list_b[LIST_LEN];
	memset(list_b, 0, LIST_LEN * sizeof(int));

	int i = 0;
	while ((bytes_read_a = read(fd_a, line_from_a, 6)) && (bytes_read_b = read(fd_b, line_from_b, 6))) {
		list_a[i] = atoi(line_from_a);
		list_b[i] = atoi(line_from_b);
		i++;
	}
	if (bytes_read_a <= -1 || bytes_read_b <= -1) {
		return (1);
	}

	// printf("UNSORTED\n");
	// for (int i = 0; i < LIST_LEN; i++) {
	// 	printf("%d   %d\n", list_a[i], list_b[i]);
	// }

	int answer_1 = part_1(list_a, list_b, LIST_LEN);
	printf("Part 1 answer:\n%d\n", answer_1);
	int answer_2 = part_2(list_a, list_b, LIST_LEN);
	printf("Part 2 answer:\n%d\n", answer_2);
}

// Order both lists.
// Compare line by line.
int part_1(int *list_a, int *list_b, int len) {
	bubble_sort(list_a, len);
	bubble_sort(list_b, len);

	// printf("SORTED\n");
	// for (int i = 0; i < LIST_LEN; i++) {
	// 	printf("%d   %d\n", list_a[i], list_b[i]);
	// }
	
	int total_distance = get_total_distance(list_a, list_b, LIST_LEN);
	return (total_distance);
}

void bubble_sort(int *array, int len) {
	bool is_ordered;	
	do {
		is_ordered = 1;	
		for (int i = 0; i < len - 1; i++) {
			if (array[i + 1] < array[i]) {
				is_ordered = 0;
				int temp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = temp;
			}
		}
	} while (!is_ordered);
}

int get_total_distance(int *list_a, int *list_b, int len) {
	int entry_distance = 0, total_distance = 0;
	for (int i = 0; i < len; i++) {
		entry_distance = abs(list_a[i] - list_b[i]);
		total_distance += entry_distance;
	}
	return (total_distance);
}

// For every number in the left list, multiply said number by the number of times the number appears on the right list.
// Add all the multiplied numbers.
int part_2(int *list_a, int *list_b, int len) {
	long total_similarity_score = 0;
	for (int i = 0; i < len; i++) {
		int number_of_matches_for_entry = 0;
		for (int j = 0; j < len; j++) {
	  		if (list_b[j] == list_a[i]) {
				number_of_matches_for_entry += 1;
			}
		}
		long entry_similarity_score = list_a[i] * number_of_matches_for_entry;
		total_similarity_score += entry_similarity_score;
	}
	return(total_similarity_score);
}
