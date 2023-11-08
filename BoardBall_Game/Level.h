#pragma once

#include "Active_Brick.h"
#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
enum ELetter_Type{
	ELT_None,
	ELT_O
};
//------------------------------------------------------------------------------------------------------------
class ALevel: public Hit_Checker
{
public:
	ALevel();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);
	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

	//bool Has_Floor;
	AActive_Brick Active_Brick;

private:
	bool Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius);
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