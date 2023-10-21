#pragma once

#include <Windows.h>

#include "Border.h"
#include "Level.h"

enum EKey_Type{
	EKT_Left, 
	EKT_Right,
	EKT_Space
};

const int Timer_ID = WM_USER + 1;
//------------------------------------------------------------------------------------------------------------
class AsEngine;
class ALevel;
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
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	//Config
	HWND Hwnd;
	HPEN BG_Pen;
	HBRUSH BG_Brush;

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