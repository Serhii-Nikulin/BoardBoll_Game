#include "Border.h"

//AsBprder
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
   :Border_Blue_Pen(0), Border_White_Pen(0), Border_Blue_Brush(0), Border_White_Brush(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Init()
{
   AsConfig::Create_Pen_Brush(57, 210, 190, Border_Blue_Pen, Border_Blue_Brush);
   AsConfig::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc)
{
   for (int i = 0; i < 50; i++)
      Draw_Element(hdc, 2, 1 + 4 * i, false, AsConfig::BG_Pen, AsConfig::BG_Brush);

   for (int i = 0; i < 50; i++)
      Draw_Element(hdc, 201/*AsConfig::Max_X_Pos*/, 1 + 4 * i, false, AsConfig::BG_Pen, AsConfig::BG_Brush);

   for (int i = 0; i < 50; i++)
      Draw_Element(hdc, 3 + i * 4, 0, true, AsConfig::BG_Pen, AsConfig::BG_Brush);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
   bool got_hit = false;

   if (AsConfig::Border_X_Offset > next_x_pos - ball->Radius)
   {
      got_hit = true;
      ball->Ball_Direction = M_PI - ball->Ball_Direction;
   }

   if (AsConfig::Border_Y_Offset > next_y_pos - ball->Radius)
   {
      got_hit = true;
      ball->Ball_Direction = -ball->Ball_Direction;
   }

   if (next_x_pos + ball->Radius > AsConfig::Max_X_Pos)
   {
      got_hit = true;
      ball->Ball_Direction = M_PI - ball->Ball_Direction;
   }

   if (next_y_pos + ball->Radius > AsConfig::Max_Y_Pos)
   {
      if (AsConfig::Level_Has_Floor)
      {
         got_hit = true;
         ball->Ball_Direction = -ball->Ball_Direction;
      }
      else
      {
         if (next_y_pos > AsConfig::Max_Y_Pos + 2 * ball->Radius)//позиция при которой мяч покидает пределы окна
            ball->Set_State(EBS_Lost, next_x_pos);
      }
   }

   return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN bg_pen, HBRUSH bg_brush)
{
   SelectObject(hdc, Border_Blue_Brush);
   SelectObject(hdc, Border_Blue_Pen);
   if (top_border)
      Rectangle(hdc, x * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);
   else
      Rectangle(hdc, (x + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

   SelectObject(hdc, Border_White_Brush);
   SelectObject(hdc, Border_White_Pen);
   if (top_border)
      Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale);
   else
      Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

   SelectObject(hdc, bg_pen);
   SelectObject(hdc, bg_brush);
   if (top_border)
      Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale, (y + 3) * AsConfig::Global_Scale);
   else
      Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
