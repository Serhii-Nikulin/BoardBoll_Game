#pragma once

#include "Ball.h";

enum EPlatform_State
{
	EPS_Missing,
	EPS_Ready,
	EPS_Normal,
	EPS_Meltdown,
	EPS_Roll_In,
	EPS_Expand_Roll_In
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform: public Hit_Checker
{
public:
	AsPlatform();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);
	void Init(int x_pos);
	void Act();
	EPlatform_State Get_State();
	void Set_State(EPlatform_State platform_state);
	void Redraw_Platform();
	void Draw(HDC hdc, RECT &paint_area);

	EPlatform_State Platform_State;
	int Width;
	int X_Step;
	int X_Pos ;
	int Meltdown_Y_Pos;

private:
	void Draw_Meltdown_State(HDC hdc, RECT &paint_area);
	void Draw_Normal_State(HDC hdc, RECT &paint_area);
	void Draw_Roll_In_State(HDC hdc, RECT &paint_area);
	void Draw_Expanding_Roll_In_State(HDC hdc, RECT &paint_area);
	void Draw_Circle_Highlight(HDC hdc, int x, int y);
	void Clear_BG(HDC hdc);

	RECT Platform_Rect, Prev_Platform_Rect;
	HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

	static const int Normal_Width = 28;
	int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];
	int Inner_Width;
	int Rolling_Step;

	static const int Circle_Size = 7;
	static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size;
	static const int Height = 7;
	static const int Meltdown_Speed = 4;
	static const int Max_Rolling_Step = 16;
	static const int Roll_In_Platform_End_X_Pos = AsConfig::Border_X_Offset + (AsConfig::Max_X_Pos - AsConfig::Border_X_Offset - Circle_Size) / 2 - 1;
	static const int Rolling_Platform_Speed = 3;
};
