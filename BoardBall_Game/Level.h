#pragma once

#include "Config.h"
#include "Level.h"
#include "Active_Brick.h"

//------------------------------------------------------------------------------------------------------------
enum ELetter_Type{
	ELT_None,
	ELT_O
};
//------------------------------------------------------------------------------------------------------------
class ALevel
{
public:
	ALevel();

	void Init();
	void Check_Level_Brick_Hit(double &next_y_pos, double &ball_direction);
	void Draw(HWND hwnd, HDC hdc, RECT &paint_area);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

	bool Has_Floor;
	AActive_Brick Active_Brick;

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

	static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];
};
//------------------------------------------------------------------------------------------------------------