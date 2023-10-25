#include "Platform.h"


//AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
   : Meltdown_Y_Pos(0), Platform_State(EPS_Normal), X_Pos(0), Width(28),  Inner_Width(21), X_Step(AsConfig::Global_Scale * 2), Platform_Rect{}, Prev_Platform_Rect{}, Platform_Circle_Pen(0), Platform_Inner_Pen(0), Highlight_Pen(0), Platform_Circle_Brush(0), Platform_Inner_Brush(0)
{
   X_Pos = (AsConfig::Max_X_Pos - Width) / 2;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
   Highlight_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255) );
   AsConfig::Create_Pen_Brush(166, 81, 167, Platform_Circle_Pen, Platform_Circle_Brush);
   AsConfig::Create_Pen_Brush(32, 174, 73, Platform_Inner_Pen, Platform_Inner_Brush);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act(HWND hwnd)
{
   int i, len;

   if (Platform_State != EPS_Meltdown)
   {
      Platform_State = EPS_Meltdown;
      len = sizeof(Meltdown_Platform_Y_Pos)/ sizeof(Meltdown_Platform_Y_Pos[0]);

      for (i = 0; i < len; i++)
         Meltdown_Platform_Y_Pos[i] += Platform_Rect.bottom;
   }

   if (Platform_State == EPS_Meltdown)
      Redraw_Platform(hwnd);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(HWND hwnd)
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

   if (Platform_State == EPS_Meltdown)
      Prev_Platform_Rect.bottom = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{//отрисовка платформы

   switch (Platform_State)
   {
   case EPS_Normal:
      Draw_Normal_State(hdc, paint_area);
      break;

   case EPS_Meltdown:
      Draw_Meltdown_State(hdc, paint_area);
      break;
   }
  
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;
   int x = X_Pos;
   int y = AsConfig::Platform_Y_Pos;

   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
      return;

   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);

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
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;
   int i, j;
   int x, y;
   int area_width, area_height;
   int y_offset = 0;
   COLORREF pixel, bg_pixel;

   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
      return;
   
   area_width = Width * AsConfig::Global_Scale;
   area_height = Height * AsConfig::Global_Scale;
   bg_pixel = RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);

   for (i = 0; i < area_width; i++)
   {
      x = Platform_Rect.left + i;
      y_offset = AsConfig::Rand(Meltdown_Speed) + 1;

      for (j = 0; j < area_height; j++)
      {
         y = Meltdown_Platform_Y_Pos[i] - 1 - j;
         pixel = GetPixel(hdc, x, y);
         SetPixel(hdc, x, y + y_offset, pixel);
      }

      for (j = 0; j < y_offset; j++)
      {
         y = Meltdown_Platform_Y_Pos[i] - area_height + j;
         SetPixel(hdc, x, y, bg_pixel);
      }

      Meltdown_Platform_Y_Pos[i] += y_offset;
   }
}
//------------------------------------------------------------------------------------------------------------
