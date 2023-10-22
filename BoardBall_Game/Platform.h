#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
	AsPlatform();
	void Init();
	void Redraw_Platform(HWND hwnd);
	void Draw(HDC hdc, RECT &paint_area, HPEN bg_pen, HBRUSH bg_brush);

	int Width;
	int X_Step;
	int X_Pos ;

private:
	RECT Platform_Rect, Prev_Platform_Rect;
	HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

	int Inner_Width;

	static const int Circle_Size = 7;
	static const int Height = 7;
};
