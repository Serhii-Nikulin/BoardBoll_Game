﻿#pragma once

#include <Windows.h>

#include "Border.h"
#include "Ball.h"//#include "Level.h"
#include "Platform.h"

//------------------------------------------------------------------------------------------------------------
enum EKey_Type{
	EKT_Left, 
	EKT_Right,
	EKT_Space
};
//------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
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

private:
	AsPlatform Platform;
	ABall Ball;
	ALevel Level;
	AsBorder Border;
	
};
//------------------------------------------------------------------------------------------------------------