#ifndef __GAME_H__
#define __GAME_H__

#include <windows.h>
#include <time.h>
#include "utils.h"

#define SCREEN_WIDTH 101
#define SCREEN_HEIGHT 61
#define GAME_WIDTH 23
#define GAME_HEIGHT 13

#define DIRECTION_LEFT coord(-1, 0)
#define DIRECTION_UP coord(0, -1)
#define DIRECTION_RIGHT coord(1, 0)
#define DIRECTION_DOWN coord(0, 1)

typedef struct {
	COORD segments[GAME_WIDTH * GAME_HEIGHT];
	COORD direction;
	INT16 size;
	BOOL is_alive;
	BOOL has_eaten;
} SNAKE;

extern COORD COORD_ORIGIN;
extern COORD GAME_SIZE;
extern COORD SCREEN_SIZE;
extern SMALL_RECT SCREEN_RECT;
extern CHAR_INFO SCREEN_BUFFER[SCREEN_WIDTH * SCREEN_HEIGHT];
extern CHAR_INFO DEFAULT_STYLE;

extern void init_snake(SNAKE* snake);
extern void update_snake(SNAKE* snake, COORD size, COORD food);
extern BOOL draw_snake(CHAR_INFO* buffer, COORD size, SNAKE* snake, CHAR_INFO style);
extern void change_snake_direction(SNAKE* snake, CHAR input);
extern BOOL update_food(COORD* food, COORD size, SNAKE* snake);
extern BOOL draw_food(CHAR_INFO* buffer, COORD size, COORD pos, CHAR_INFO style);
extern void start_game();

#endif