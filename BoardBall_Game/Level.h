#pragma once

#include "Config.h"

#define _USE_MATH_DEFINES
#include <math.h>

enum EBrick_Type{
	EBT_None,
	EBT_Red,
	EBT_Blue
};

enum ELetter_Type{
	ELT_None,
	ELT_O
};

class ALevel
{
public:
	ALevel();

	void Init();
	void Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction);
	void Draw(HDC hdc, RECT &paint_area);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

	//Brick
	static const int Cell_Width = 16;
	static const int Cell_Height = 8;

	//Level
	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;
	static const int Level_Height = 14;
	static const int Level_Width = 12;

private:
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush);

	//Level
	RECT Level_Rect;

	//Letter
	HPEN Letter_Pen;

	//Brick
	HPEN Brick_Blue_Pen, Brick_Red_Pen;
	HBRUSH Brick_Blue_Brush, Brick_Red_Brush;

	//Brick
	static const int Brick_Width = 15;
	static const int Brick_Height = 7;
};
//------------------------------------------------------------------------------------------------------------