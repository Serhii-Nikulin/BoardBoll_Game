#pragma once

#include "Config.h"
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();
	void Init();
	void Draw(HDC hdc,  RECT &paint_area, HPEN bg_pen, HBRUSH bg_brush);
	void Move(HWND hwnd, int platform_x_pos, int platform_y_pos, ALevel *level);

private:
	RECT Ball_Rect, Prev_Ball_Rect;
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;

	double Ball_Direction;
	int Ball_X_Pos;
	int Ball_Y_Pos;
	double Ball_Speed;
};
//------------------------------------------------------------------------------------------------------------