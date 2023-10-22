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
void AsBorder::Draw(HDC hdc, HPEN bg_pen, HBRUSH bg_brush)
{
   for (int i = 0; i < 50; i++)
      Draw_Element(hdc, 2, 1 + 4 * i, false, bg_pen, bg_brush);

   for (int i = 0; i < 50; i++)
      Draw_Element(hdc, 201/*AsConfig::Max_X_Pos*/, 1 + 4 * i, false, bg_pen, bg_brush);

   for (int i = 0; i < 50; i++)
      Draw_Element(hdc, 3 + i * 4, 0, true, bg_pen, bg_brush);
}
//------------------------------------------------------------------------------------------------------------