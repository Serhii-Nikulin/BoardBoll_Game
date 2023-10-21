#pragma once

#include "Config.h"

class AsBorder
{
public:
	AsBorder();

	void Init();
	void Draw(HDC hdc, HPEN bg_pen, HBRUSH bg_brush);

	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN bg_pen, HBRUSH bg_brush);

	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;
};
//------------------------------------------------------------------------------------------------------------