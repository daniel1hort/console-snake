#include "utils.h"

float sampled_random() {
	return ((float)(rand() % RAND_PRECISION) / RAND_PRECIZIONf);
}

void set_rect(PSMALL_RECT psrect, SHORT left, SHORT top, SHORT right, SHORT bottom) {
	psrect->Left = left;
	psrect->Top = top;
	psrect->Right = right;
	psrect->Bottom = bottom;
}

void set_coord(PCOORD pcoord, SHORT x, SHORT y) {
	pcoord->X = x;
	pcoord->Y = y;
}

BOOL draw_rect(CHAR_INFO* buffer, COORD size, SMALL_RECT rect, CHAR_INFO style) {
	if (rect.Left < 0 || rect.Top < 0 || rect.Right >= size.X || rect.Bottom >= size.Y)
		return FALSE;

	for (int i = rect.Left; i <= rect.Right; i++) {
		buffer[size.X * rect.Top + i].Char.AsciiChar = style.Char.AsciiChar;
		buffer[size.X * rect.Top + i].Attributes = style.Attributes;
		buffer[size.X * rect.Bottom + i].Char.AsciiChar = style.Char.AsciiChar;
		buffer[size.X * rect.Bottom + i].Attributes = style.Attributes;
	}

	for (int i = rect.Top; i <= rect.Bottom; i++) {
		buffer[size.X * i + rect.Left].Char.AsciiChar = style.Char.AsciiChar;
		buffer[size.X * i + rect.Left].Attributes = style.Attributes;
		buffer[size.X * i + rect.Right].Char.AsciiChar = style.Char.AsciiChar;
		buffer[size.X * i + rect.Right].Attributes = style.Attributes;
	}

	return TRUE;
}

BOOL fill_rect(CHAR_INFO* buffer, COORD size, SMALL_RECT rect, CHAR_INFO style) {
	if (rect.Left < 0 || rect.Top < 0 || rect.Right >= size.X || rect.Bottom >= size.Y)
		return FALSE;

	for (int i = rect.Top; i <= rect.Bottom; i++) {
		for (int j = rect.Left; j <= rect.Right; j++) {
			buffer[size.X * i + j].Char.AsciiChar = style.Char.AsciiChar;
			buffer[size.X * i + j].Attributes = style.Attributes;
		}
	}

	return TRUE;
}

void SetConsoleFont(HANDLE handle, SHORT x, SHORT y) {
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(handle, 0, &fontInfo);

	fontInfo.dwFontSize.X = x;
	fontInfo.dwFontSize.Y = y;
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;
	wcscpy_s(fontInfo.FaceName, 32, L"Consolas");
	SetCurrentConsoleFontEx(handle, 0, &fontInfo);
}

void SetConsoleSize(HANDLE handle, SHORT width, SHORT height, PCHAR_INFO chars) {
	SMALL_RECT srect;
	set_rect(&srect, 0, 0, 1, 1);
	SetConsoleWindowInfo(handle, TRUE, &srect);
	SetConsoleScreenBufferSize(handle, coord(width, height));
	set_rect(&srect, 0, 0, width - 1, height - 1);
	SetConsoleWindowInfo(handle, TRUE, &srect);
}

void DisableResize() {
	HANDLE window, menu;
	window = GetConsoleWindow();
	menu = GetSystemMenu(window, FALSE);
	DeleteMenu(menu, SC_MAXIMIZE, MF_BYCOMMAND);
	DeleteMenu(menu, SC_SIZE, MF_BYCOMMAND);
}

void EnableMouseInput(HANDLE handle) {
	DWORD consoleMode;
	GetConsoleMode(handle, &consoleMode);
	SetConsoleMode(handle, consoleMode | ENABLE_MOUSE_INPUT);
}

BOOL ReadInput(HANDLE handle, PCHAR result) {
	INPUT_RECORD record;
	DWORD events_count;

	GetNumberOfConsoleInputEvents(handle, &events_count);

	if (events_count > 0) {
		ReadConsoleInputA(handle, &record, 1, &events_count);
		if (record.EventType == KEY_EVENT) {
			if (record.Event.KeyEvent.bKeyDown) {
				*result = record.Event.KeyEvent.uChar.AsciiChar;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL Timer(clock_t* start, clock_t interval) {
	clock_t end = clock();
	if (end - *start > interval) {
		*start = end;
		return TRUE;
	}
	return FALSE;
}