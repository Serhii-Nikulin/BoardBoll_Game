#include "Active_Brick.h"

//AActiveBrick
AActive_Brick::AActive_Brick()
   :Brick_Rect{}, Fade_Step(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Act(HWND hwnd)
{
   if (Fade_Step < Max_Fade_Step)
   {
      ++Fade_Step;
      InvalidateRect(hwnd, &Brick_Rect, FALSE);
   }
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersect_rect;

   HPEN pen;
   HBRUSH brush;

   AsConfig::Create_Pen_Brush(62 - Fade_Step * 62 / Max_Fade_Step, 71 - Fade_Step * 71 / Max_Fade_Step, 202 - Fade_Step * 202 / Max_Fade_Step, pen, brush);
   ++Fade_Step;

   Brick_Rect.left = (AsConfig::Level_X_Offset + AsConfig::Cell_Width) * AsConfig::Global_Scale;
   Brick_Rect.top = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height) * AsConfig::Global_Scale;
   Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
   Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}