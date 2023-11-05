#pragma once

#include "Config.h"
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
enum EBall_State
{
	EBS_On_Platform,
	EBS_Normal,
	EBS_Lost
};
//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();
	void Init();
	void Draw(HDC hdc,  RECT &paint_area);
	void Move(int platform_x_pos, int platform_y_pos, ALevel *level);
	void Redraw_Ball();
	EBall_State Get_State();
	void Set_State(EBall_State new_state, int x_pos);

private:
	RECT Ball_Rect, Prev_Ball_Rect;
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;

	EBall_State Ball_State;

	double Ball_Direction;
	double Ball_Speed;
	double Ball_X_Pos;
	double Ball_Y_Pos;

	static const int Start_Ball_Y_Pos = AsConfig::Platform_Y_Pos + 1 - AsConfig::Ball_Size;
};
//------------------------------------------------------------------------------------------------------------