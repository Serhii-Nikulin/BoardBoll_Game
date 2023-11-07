#pragma once

#include <Windows.h>

#include "Border.h"
#include "Ball.h"
#include "Level.h"
#include "Platform.h"


//------------------------------------------------------------------------------------------------------------
enum EGame_State
{
	EGS_Start_Level,
	EGS_Play_Level,//единственное состояние в котором перемещается мяч
	EGS_Lost_Ball,//отсановка анимации, расплавление монстров и платформы
	EGS_Restart_Level,//выкатывание платформы, её расширение и подгатовка к игре
};
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


private:
	EGame_State Game_State;

	AsPlatform Platform;
	ABall Ball;
	ALevel Level;
	AsBorder Border;
};
//------------------------------------------------------------------------------------------------------------