#pragma once
#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

enum EKey_Type{
	EKT_Left, 
	EKT_Right,
	EKT_Space
};

enum EBrick_Type{
	EBT_None,
	EBT_Red,
	EBT_Blue
};

enum ELetter_Type{
	ELT_None,
	ELT_O
};

const int Timer_ID = WM_USER + 1;

class AsEngine
{
public:
	AsEngine();
	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	//Level
	static const int Level_Height = 14;
	static const int Level_Width = 12;

private:
	void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);
	void Redraw_Platform();
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
	void Draw_Level(HDC hdc);
	void Draw_Platform(HDC hdc, int x, int y);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
	void Draw_Ball(HDC hdc);
	void Draw_Border(HDC hdc, int x, int y, bool top_border);
	void Draw_Bounds(HDC hdc);
	void Check_Level_Brick_Hit(int &next_y_pos);
	void Move_Ball();

	//Config
	static const int Global_Scale = 3;

	//Level
	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;

	//Brick
	static const int Brick_Width = 15;
	static const int Brick_Height = 7;
	static const int Cell_Width = 16;
	static const int Cell_Height = 8;

	//Platform
	static const int Circle_Size = 7;
	static const int Platform_Height = 7;
	static const int Platform_X_Step = Global_Scale * 2;
	static const int Platform_Y_Pos = 185;
	int Platform_X_Pos ;
	int Inner_Width;
	int Platform_Width;

	//Ball
	static const int Ball_Size = 4;
	static const int Max_X_Pos = Level_X_Offset + Level_Width * Cell_Width + 1;
	static const int Max_Y_Pos = 199;
	int Ball_X_Pos;
	int Ball_Y_Pos;
	double Ball_Speed;
	double Ball_Direction;

	//Bounds
	const int Border_X_Offset = 6;
	const int Border_Y_Offset = 4;

};