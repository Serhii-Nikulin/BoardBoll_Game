#include "Platform.h"

//AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
   : X_Pos(50), Width(28),  Inner_Width(21), X_Step(AsConfig::Global_Scale * 2), Platform_Rect{}, Prev_Platform_Rect{}, Platform_Circle_Pen(0), Platform_Inner_Pen(0), Highlight_Pen(0), Platform_Circle_Brush(0), Platform_Inner_Brush(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
   Highlight_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255) );
   AsConfig::Create_Pen_Brush(166, 81, 167, Platform_Circle_Pen, Platform_Circle_Brush);
   AsConfig::Create_Pen_Brush(32, 174, 73, Platform_Inner_Pen, Platform_Inner_Brush);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(HWND hwnd)
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = AsConfig::Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area, HPEN bg_pen, HBRUSH bg_brush)
{//отрисовка платформы

   RECT intersection_rect;
   int x = X_Pos;
   int y = AsConfig::Y_Pos;

   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
      return;

   SelectObject(hdc, bg_pen);
   SelectObject(hdc, bg_brush);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   //1.отрисовка шариков платформы
   SelectObject(hdc, Platform_Circle_Brush);
   SelectObject(hdc, Platform_Circle_Pen);

   Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Inner_Width + Circle_Size) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);

   //2.Отрисовка внутренней части
   SelectObject(hdc, Platform_Inner_Brush);
   SelectObject(hdc, Platform_Inner_Pen);

   RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4 + (Inner_Width - 1)) * AsConfig::Global_Scale, (y + 1 + 5) * AsConfig::Global_Scale,
      5 * AsConfig::Global_Scale, 5 * AsConfig::Global_Scale);

   //3.Отрисовка дуги
   SelectObject(hdc, Highlight_Pen);

   Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale,
      (x + Circle_Size - 1) * AsConfig::Global_Scale, (y + Circle_Size - 1) * AsConfig::Global_Scale,
      (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale,
      x * AsConfig::Global_Scale, (y + 3) * AsConfig::Global_Scale);
}
