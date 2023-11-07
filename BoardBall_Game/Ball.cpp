﻿#include "Ball.h"
#include "Level.h"
#include "Platform.h"

//ABall
const double ABall::Radius = 2.0;
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
   :Ball_State(EBS_Normal), Rest_Distance(0.0), Ball_Pen(0), Ball_Brush(0), Center_X_Pos(0.0),
   Center_Y_Pos(0.0), Ball_Direction(0.0), Ball_Rect{}, Prev_Ball_Rect{}, Ball_Speed(0.0)
{
   Set_State(EBS_Normal, 0);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
   AsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   //Зарисовуем прошлое положение мяча фоном
   if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect) )
   {
      SelectObject(hdc, AsConfig::BG_Pen);
      SelectObject(hdc, AsConfig::BG_Brush);
      Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
   }

   //Отрисовуем новое положение мяча
   if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect) )
   {
      SelectObject(hdc, Ball_Pen);
      SelectObject(hdc, Ball_Brush);
      Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
   }
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move(Hit_Checker* level_hit_checker, Hit_Checker* border_hit_checker, Hit_Checker* platform_hit_checker)
{//перерасчёт позиция мяча и запрос на его перерисовку

   bool got_hit;
   double next_x_pos, next_y_pos;
   double max_x_pos = AsConfig::Max_X_Pos;
	double max_y_pos = AsConfig::Max_Y_Pos;
   double platform_y_pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Size;
   double step_size = 1.0 / AsConfig::Global_Scale;
   Rest_Distance += Ball_Speed;

   if (Ball_State != EBS_Normal)
      return;

   Prev_Ball_Rect = Ball_Rect;

   while (Rest_Distance >= step_size)
   {
      got_hit = false;
      next_x_pos = Center_X_Pos + step_size * cos(Ball_Direction);
      next_y_pos = Center_Y_Pos - step_size * sin(Ball_Direction);
   
      got_hit |= border_hit_checker->Check_Hit(next_x_pos, next_y_pos, this);
      got_hit |= level_hit_checker->Check_Hit(next_x_pos, next_y_pos, this);
      got_hit |= platform_hit_checker->Check_Hit(next_x_pos, next_y_pos, this);
      
      if (!got_hit)
      {
         Center_X_Pos = next_x_pos;
         Center_Y_Pos = next_y_pos;
         Rest_Distance -= step_size;
      }
   }

   Redraw_Ball();
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Ball()
{
   Ball_Rect.left = (Center_X_Pos - Radius) * AsConfig::Global_Scale;
   Ball_Rect.top = (Center_Y_Pos - Radius) * AsConfig::Global_Scale;
   Ball_Rect.right = (Center_X_Pos + Radius) * AsConfig::Global_Scale;
   Ball_Rect.bottom = (Center_Y_Pos + Radius) * AsConfig::Global_Scale;

   InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
EBall_State ABall::Get_State()
{
   return Ball_State;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state, double x_pos)
{
   switch (new_state)
   {
   case EBS_Lost:
      Ball_Speed = 0.0;
      break;

   case EBS_Normal:
      Ball_State = EBS_Normal;
      Center_X_Pos = x_pos;
      Center_Y_Pos = Start_Ball_Y_Pos;
      Ball_Speed = 3.0;
      Ball_Direction = M_PI - M_PI_4;

      Redraw_Ball();

      break;

   case EBS_On_Platform:
      Ball_State = EBS_Normal;
      Center_X_Pos = x_pos;
      Center_Y_Pos = Start_Ball_Y_Pos;
      Ball_Speed = 0.0;
      Ball_Direction = M_PI - M_PI_4;

      Redraw_Ball();

      break;
   }

   Ball_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
