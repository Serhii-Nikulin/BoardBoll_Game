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
//------------------------------------------------------------------------------------------------------------
class ALevel;
class AsEngine;
class AsPlatform;
class ABall
{
public:
	ABall();
	void Init();
	void Draw(HDC hdc,  RECT &paint_area, AsEngine* engine);
	void Move(AsEngine *engine, ALevel *level, AsPlatform *platform);

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;

	double Ball_Direction;

	static const int Ball_Size = 4;

private:
	RECT Ball_Rect, Prev_Ball_Rect;

	int Ball_X_Pos;
	int Ball_Y_Pos;
	double Ball_Speed;
};
//------------------------------------------------------------------------------------------------------------
class ALevel
{
public:
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
class AsPlatform
{
public:
	AsPlatform();
	void Init();
	void Redraw_Platform(AsEngine *engine);
	void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);

	int Width;
	int X_Step;
	int X_Pos ;
	static const int Y_Pos = 185;

private:
	RECT Platform_Rect, Prev_Platform_Rect;
	HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

	int Inner_Width;
	
	static const int Circle_Size = 7;
	static const int Height = 7;
};
//------------------------------------------------------------------------------------------------------------
class AsBorder
{
public:
	AsBorder();

	void Init();
	void Draw(HDC hdc, AsEngine *engine);

	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, AsEngine *engine);

	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;
};
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();
	static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);

	//Config
	HWND Hwnd;
	HPEN BG_Pen;
	HBRUSH BG_Brush;
	static const int Global_Scale = 3;

	//Level
	static const int Max_X_Pos = ALevel::Level_X_Offset + ALevel::Level_Width * ALevel::Cell_Width + 1;
	static const int Max_Y_Pos = 199;

private:
	ABall Ball;
	ALevel Level;
	AsPlatform Platform;
	AsBorder Border;
	
};
//------------------------------------------------------------------------------------------------------------