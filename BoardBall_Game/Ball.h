#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum EBall_State
{
	EBS_On_Platform,
	EBS_Normal,
	EBS_Lost
};
//------------------------------------------------------------------------------------------------------------
class ABall;
class Hit_Checker
{
public:
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball) = 0;
};
//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();
	void Init();
	void Draw(HDC hdc,  RECT &paint_area);
	void Move(Hit_Checker* level_hit_checker, Hit_Checker* border_hit_checker, Hit_Checker* platform_hit_checker);
	void Redraw_Ball();
	EBall_State Get_State();
	void Set_State(EBall_State new_state, double x_pos);

	double Ball_Direction;

	static const double Radius;

private:
	RECT Ball_Rect, Prev_Ball_Rect;
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;

	EBall_State Ball_State;

	double Ball_Speed;
	double Center_X_Pos;
	double Center_Y_Pos;
	double Rest_Distance;

	static const int Start_Ball_Y_Pos = AsConfig::Platform_Y_Pos + 1 - AsConfig::Ball_Size;
};
//------------------------------------------------------------------------------------------------------------