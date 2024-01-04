#ifndef __UTILS_H__
#define __UTILS_H__

#include "logging.h"
#include <time.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RAND_PRECISION 32767 // the bigger the merrier
#define RAND_PRECIZIONf 32767.0f

extern float sampled_random();
#define sampled_randomN() (sampled_random() * 2.0f - 1.0f)

#define max(a, b) (((a) > (b))?(a):(b))
#define min(a, b) (((a) > (b))?(b):(a))
#define clamp(a, b, c) min(max(a, b), c)

#define no_rng() srand(0)
#define rng() srand(time(NULL));

#define rect(l, t, r, b) ((SMALL_RECT){ .Left = (l), .Top = (t), .Right = (r), .Bottom = (b) })
#define coord(x, y) ((COORD){ .X = (x), .Y = (y) })

typedef struct {
	COORD size;
	CHAR* data;
} SPRITE;

extern void set_rect(PSMALL_RECT psrect, SHORT left, SHORT top, SHORT right, SHORT bottom);
extern void set_coord(PCOORD pcoord, SHORT x, SHORT y);

extern BOOL rect_inside_boundaries(SMALL_RECT rect, COORD size);
extern BOOL coord_inside_boundaries(COORD pos, COORD size);
extern BOOL draw_rect(CHAR_INFO* buffer, COORD size, SMALL_RECT rect, CHAR_INFO style);
extern BOOL fill_rect(CHAR_INFO* buffer, COORD size, SMALL_RECT rect, CHAR_INFO style);
extern BOOL set_pixel(CHAR_INFO* buffer, COORD size, COORD pos, CHAR_INFO style);
extern BOOL draw_sprite(CHAR_INFO* buffer, COORD size, COORD pos, CHAR_INFO style, SPRITE* sprite);

extern void SetConsoleFont(HANDLE handle, SHORT x, SHORT y);
extern void SetConsoleSize(HANDLE handle, SHORT width, SHORT height, PCHAR_INFO chars);
extern void DisableResize();
extern void EnableMouseInput(HANDLE handle);

extern BOOL ReadInput(HANDLE handle, PCHAR result);
extern BOOL Timer(clock_t* start, clock_t interval);

#ifdef __cplusplus
}
#endif

#endif