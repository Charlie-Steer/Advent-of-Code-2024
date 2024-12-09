#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define MAP_SIDE 130
#define TIMEOUT 20000

enum direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

typedef struct guard_info {
	int x;
	int y;
	enum direction direction;
} guard_info;

void print_map(char **map);
char **get_map(char *input_path);
guard_info get_guard_info(char **map);
bool check_if_bound_ahead(guard_info guard, char **map);
bool check_if_clear_ahead(guard_info guard, char **map);
guard_info move_forward(guard_info guard, char **map);
guard_info rotate_right(guard_info guard, char **map);
int count_path_locations(char **map);
bool place_new_obstacle(char **map);
void copy_map(char **map_src, char **map_dst);

int main(void) {
	char **map = get_map("input");
	char **map_backup = get_map("input");

	long n_loops = 0;
	while (place_new_obstacle(map)) {
		guard_info guard = get_guard_info(map);
		int iters;
		for (iters = 0; !check_if_bound_ahead(guard, map) && iters < TIMEOUT; iters++) {
			bool is_clear_ahead = check_if_clear_ahead(guard, map);
			if (is_clear_ahead) {
				guard = move_forward(guard, map);
			}
			else {
				guard = rotate_right(guard, map);
			}
		}
		// printf("%d\n", iters);
		if (iters == TIMEOUT) {
			n_loops += 1;
		}
		// print_map(map);
		copy_map(map_backup, map);
	}

	printf("\nanswer:\n%ld\n", n_loops);
	
	return (0);
}

bool place_new_obstacle(char **map) {
	static int y = 0;
	static int x = 0;

	// printf("y: %d, x: %d\n", y, x);
	for (; y < MAP_SIDE; y++) {
		for (; x < MAP_SIDE;) {
			if (map[y][x] != '#' && map[y][x] != '^') {
				map[y][x] = '#';
				x++;
				return (true);
			}
			else {
				x++;
			}
		}
		x = 0;
	}
	return (false);
}

void copy_map(char **map_src, char **map_dst) {
	for (int y = 0; y < MAP_SIDE; y++) {
		for (int x = 0; x < MAP_SIDE; x++) {
			map_dst[y][x] = map_src[y][x];
		}
	}
}

bool check_if_bound_ahead(guard_info guard, char **map) {
	if (guard.direction == UP && guard.y == 0) {
		return (true);
	}
	else if (guard.direction == DOWN && guard.y == MAP_SIDE - 1) {
		return (true);
	}
	else if (guard.direction == LEFT && guard.x == 0) {
		return (true);
	}
	else if (guard.direction == RIGHT && guard.x == MAP_SIDE - 1) {
		return (true);
	}
	else {
		return (false);
	}
}

bool check_if_clear_ahead(guard_info guard, char **map) {
	if (guard.direction == UP && map[guard.y - 1][guard.x] == '#') {
		return (false);
	}
	else if (guard.direction == DOWN && map[guard.y + 1][guard.x] == '#') {
		return (false);
	}
	else if (guard.direction == LEFT && map[guard.y][guard.x - 1] == '#') {
		return (false);
	}
	else if (guard.direction == RIGHT && map[guard.y][guard.x + 1] == '#') {
		return (false);
	}
	else {
		return (true);
	}
}

guard_info move_forward(guard_info guard, char **map) {
	// printf("forward\n");
	if (guard.direction == UP) {
		map[guard.y][guard.x] = 'X';
		guard.y = guard.y - 1;
		map[guard.y][guard.x] = '^';
	}
	else if (guard.direction == DOWN) {
		map[guard.y][guard.x] = 'X';
		guard.y = guard.y + 1;
		map[guard.y][guard.x] = 'v';
	}
	else if (guard.direction == LEFT) {
		map[guard.y][guard.x] = 'X';
		guard.x = guard.x - 1;
		map[guard.y][guard.x] = '<';
	}
	else if (guard.direction == RIGHT) {
		map[guard.y][guard.x] = 'X';
		guard.x = guard.x + 1;
		map[guard.y][guard.x] = '>';
	}
	return (guard);
}

guard_info rotate_right(guard_info guard, char **map) {
	// printf("rotate\n");
	if (guard.direction == UP) {
		guard.direction = RIGHT;
		map[guard.y][guard.x] = '>';
	}
	else if (guard.direction == RIGHT) {
		guard.direction = DOWN;
		map[guard.y][guard.x] = 'v';
	}
	else if (guard.direction == DOWN) {
		guard.direction = LEFT;
		map[guard.y][guard.x] = '<';
	}
	else if (guard.direction == LEFT) {
		guard.direction = UP;
		map[guard.y][guard.x] = '^';
	}
	return (guard);
}

int count_path_locations(char **map) {
	int path_locations = 0;
	for (int i = 0; i < MAP_SIDE; i++) {
		for (int j = 0; j < MAP_SIDE; j++) {
			if (map[i][j] == 'X') {
				path_locations++;
			}
		}
	}
	return (path_locations);
}

char **get_map(char *input_path) {
	FILE *file = fopen(input_path, "r");

	char **map = malloc(MAP_SIDE * sizeof(char *));

	for (int i = 0; i < MAP_SIDE; i++) {
		map[i] = malloc(MAP_SIDE + 2);
		fgets(map[i], MAP_SIDE + 2, file);
	}

	// print_map(map);
	return (map);
}

void print_map(char **map) {
	for (int i = 0; i < MAP_SIDE; i++) {
		printf("%s", map[i]);
	}
}

guard_info get_guard_info(char **map) {
	guard_info guard_info;
	for (int i = 0; i < MAP_SIDE; i++) {
		for (int j = 0; j < MAP_SIDE; j++) {
			// printf("i: %d, j: %d\n", i, j);
			if (map[i][j] == '^' || map[i][j] == 'v' || map[i][j] == '<' || map[i][j] == '>') {
				guard_info.y = i;
				guard_info.x = j;
				if (map[i][j] == '^') {
					guard_info.direction = UP;
				}
				else if (map[i][j] == 'v') {
					guard_info.direction = DOWN;
				}
				else if (map[i][j] == '<') {
					guard_info.direction = LEFT;
				}
				else if (map[i][j] == '>') {
					guard_info.direction = RIGHT;
				}
			}
		}
	}
	return (guard_info);
}
