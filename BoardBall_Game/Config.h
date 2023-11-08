#pragma once

#include "Windows.h"

#define _USE_MATH_DEFINES
#include <math.h>

//AColor
//------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	AColor(unsigned char r, unsigned char g, unsigned char b);

	unsigned char R, G, B;
};
//------------------------------------------------------------------------------------------------------------



//AsConfig
//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);
	static void Create_Pen_Brush(const AColor &color, HPEN &pen, HBRUSH &brush);
	static void Setup_Colors();
	static int Rand(int range);

	static const AColor Blue_Brick_Color;
	static const AColor Red_Brick_Color;
	static const AColor BG_Color;

	static HPEN BG_Pen;
	static HBRUSH BG_Brush;

	static int Current_Timer_Tick;

	//Config
	static HWND Hwnd;
	static const bool Level_Has_Floor = false;
	static const int Global_Scale = 3;
	static const int FPS = 20;

	//Brick
	static const int Brick_Width = 15;
	static const int Brick_Height = 7;
	static const int Cell_Width = 16;
	static const int Cell_Height = 8;

	//Level
	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;
	static const int Level_Height = 14;
	static const int Level_Width = 12;
	static const int Max_X_Pos = Level_X_Offset + Level_Width * Cell_Width + 1;
	static const int Max_Y_Pos = 199;

	//Platform
	static const int Platform_Y_Pos = 185;

	//Border
	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;
};
//------------------------------------------------------------------------------------------------------------
