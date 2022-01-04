#include "game.h"

COORD COORD_ORIGIN = { .X = 0, .Y = 0 };
COORD GAME_SIZE = { .X = GAME_WIDTH, .Y = GAME_HEIGHT };
COORD SCREEN_SIZE = { .X = SCREEN_WIDTH, .Y = SCREEN_HEIGHT };
SMALL_RECT SCREEN_RECT = {
	.Left = 0,
	.Top = 0,
	.Right = SCREEN_WIDTH - 1,
	.Bottom = SCREEN_HEIGHT - 1
};
CHAR_INFO SCREEN_BUFFER[SCREEN_WIDTH * SCREEN_HEIGHT] = { 0 };
CHAR_INFO DEFAULT_STYLE = {
	.Attributes = BACKGROUND_GREEN,
	.Char.AsciiChar = ' ',
};

void init_snake(SNAKE* snake) {
	snake->segments[0] = coord(5, 5);
	snake->segments[1] = coord(4, 5);
	snake->segments[2] = coord(3, 5);
	snake->segments[3] = coord(2, 5);
	snake->size = 4;
	snake->direction = DIRECTION_RIGHT;
	snake->is_alive = TRUE;
	snake->has_eaten = FALSE;
}

void update_snake(SNAKE* snake, COORD size, COORD food) {
	COORD new_head;
	INT16 size_to_check = snake->size - (snake->has_eaten ? 0 : 1);
	new_head.X = (snake->segments[0].X + snake->direction.X + size.X) % size.X;
	new_head.Y = (snake->segments[0].Y + snake->direction.Y + size.Y) % size.Y;

	for (int i = 0; i < size_to_check; i++)
		if (new_head.X == snake->segments[i].X && new_head.Y == snake->segments[i].Y)
		{
			snake->is_alive = FALSE;
			return;
		}

	for (int i = snake->size; i > 0; i--)
		snake->segments[i] = snake->segments[i - 1];
	snake->segments[0] = new_head;
	snake->has_eaten = FALSE;

	if (food.X == new_head.X && food.Y == new_head.Y) {
		snake->size++;
		snake->has_eaten = TRUE;
	}
}

BOOL draw_snake(CHAR_INFO* buffer, COORD size, SNAKE* snake, CHAR_INFO style) {
	INT16 minx = MAXINT16, miny = MAXINT16, maxx = 0, maxy = 0;
	for (int i = 0; i < snake->size; i++) {
		minx = min(minx, 5 + 4 * snake->segments[i].X);
		miny = min(miny, 5 + 4 * snake->segments[i].Y);
		maxx = max(maxx, 7 + 4 * snake->segments[i].X);
		maxy = max(maxy, 7 + 4 * snake->segments[i].Y);
	}
	if (minx < 0 || miny < 0 || maxx >= size.X || maxy >= size.Y)
		return FALSE;

	for (int i = 0; i < snake->size; i++) {
		draw_rect(buffer, size, rect(
			5 + 4 * snake->segments[i].X,
			5 + 4 * snake->segments[i].Y,
			7 + 4 * snake->segments[i].X,
			7 + 4 * snake->segments[i].Y
		), style);
	}
	return TRUE;
}

void change_snake_direction(SNAKE* snake, CHAR input) {
	if ((input == 'a' || input == 'A') &&
		snake->direction.X != DIRECTION_RIGHT.X &&
		snake->direction.Y != DIRECTION_RIGHT.Y)
		snake->direction = DIRECTION_LEFT;
	if ((input == 'w' || input == 'W') &&
		snake->direction.X != DIRECTION_DOWN.X &&
		snake->direction.Y != DIRECTION_DOWN.Y)
		snake->direction = DIRECTION_UP;
	if ((input == 'd' || input == 'D') &&
		snake->direction.X != DIRECTION_LEFT.X &&
		snake->direction.Y != DIRECTION_LEFT.Y)
		snake->direction = DIRECTION_RIGHT;
	if ((input == 's' || input == 'S') &&
		snake->direction.X != DIRECTION_UP.X &&
		snake->direction.Y != DIRECTION_UP.Y)
		snake->direction = DIRECTION_DOWN;
}

BOOL update_food(COORD* food, COORD size, SNAKE* snake) {
	INT16 max = size.X * size.Y;
	INT16 index = rand() % max;
	COORD pos = { 0 };
	BOOL is_valid = FALSE;

	if (snake->size >= max)
		return FALSE;

	while (!is_valid) {
		is_valid = TRUE;
		pos = coord(index % size.X, index / size.X);
		for (int i = 0; i < snake->size; i++)
			if (pos.X == snake->segments[i].X && pos.Y == snake->segments[i].Y) {
				index = (index + 1) % max;
				is_valid = FALSE;
				break;
			}
	}

	set_coord(food, pos.X, pos.Y);
	return TRUE;
}

BOOL draw_food(CHAR_INFO* buffer, COORD size, COORD pos, CHAR_INFO style) {
	COORD corner = coord(5 + 4 * pos.X, 5 + 4 * pos.Y);
	if (corner.X < 0 || corner.Y < 0 || corner.X + 2 >= size.X || corner.Y + 2 >= size.Y)
		return FALSE;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if ((i + j) % 2 == 1) {
				draw_rect(buffer, size, rect(
					corner.X + i,
					corner.Y + j,
					corner.X + i,
					corner.Y + j
				), style);
			}
	return TRUE;
}

void start_game() {
	HANDLE hStdout, hStdin;
	CHAR input = 0;
	SNAKE snake = { 0 };
	COORD food = { 0 };
	clock_t gameTimer = clock();
	INT16 score = 0;
	CHAR buff[256] = { 0 };

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleFont(hStdout, 8, 8);
	SetConsoleSize(hStdout, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BUFFER);
	DisableResize();
	init_snake(&snake);
	rng();
	update_food(&food, GAME_SIZE, &snake);

	while (input != 0x1B && snake.is_alive) {
		ReadInput(hStdin, &input);
		change_snake_direction(&snake, input);

		if (Timer(&gameTimer, CLOCKS_PER_SEC / 4)) {
			update_snake(&snake, GAME_SIZE, food);
			if (snake.has_eaten) {
				score++;
				update_food(&food, GAME_SIZE, &snake);
			}
		}

		memset(SCREEN_BUFFER, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(CHAR_INFO));
		draw_rect(SCREEN_BUFFER, SCREEN_SIZE, rect(1, 1, SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2), DEFAULT_STYLE);
		draw_rect(SCREEN_BUFFER, SCREEN_SIZE, rect(3, 3, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4), DEFAULT_STYLE);
		draw_food(SCREEN_BUFFER, SCREEN_SIZE, food, DEFAULT_STYLE);
		draw_snake(SCREEN_BUFFER, SCREEN_SIZE, &snake, DEFAULT_STYLE);
		WriteConsoleOutputW(hStdout, SCREEN_BUFFER, SCREEN_SIZE, COORD_ORIGIN, &SCREEN_RECT);
		sprintf_s(buff, 256, "Score: %05d", score);
		SetConsoleTitleA(buff);
	}

	CloseHandle(hStdin);
	CloseHandle(hStdout);
}