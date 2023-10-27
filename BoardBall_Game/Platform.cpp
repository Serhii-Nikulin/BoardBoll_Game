#include "Platform.h"


//AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
   : Meltdown_Y_Pos(0), Platform_State(EPS_Normal), X_Pos(0), Width(Normal_Width),  Inner_Width(21), Rolling_Step(15), X_Step(AsConfig::Global_Scale * 2), Platform_Rect{}, Prev_Platform_Rect{}, Platform_Circle_Pen(0), Platform_Inner_Pen(0), Highlight_Pen(0), Platform_Circle_Brush(0), Platform_Inner_Brush(0)
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
void AsPlatform::Act(HWND hwnd)
{
   switch (Platform_State)
   {
   case EPS_Roll_In:
   case EPS_Expand_Roll_In:
   case EPS_Normal:
      Redraw_Platform();
      break;
   }
   
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State new_state)
{
   int i, len;

   if (Platform_State == new_state)
      return;

   switch (new_state)
   {
   case EPS_Meltdown:
      Platform_State = EPS_Meltdown;
      len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

      for (i = 0; i < len; i++)
         Meltdown_Platform_Y_Pos[i] += Platform_Rect.bottom;

      break;

   case EPS_Roll_In:
      X_Pos = AsConfig::Max_X_Pos - 2;
      break;

   }
 

   Platform_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform()
{
   int platform_width;
   Prev_Platform_Rect = Platform_Rect;

   if (Platform_State == EPS_Roll_In)
      platform_width = Circle_Size;
   else
      platform_width = Width;
   
   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = Platform_Rect.left + platform_width * AsConfig::Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Circle_Size * AsConfig::Global_Scale;

   if (Platform_State == EPS_Meltdown)
      Prev_Platform_Rect.bottom = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

   InvalidateRect(AsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{//отрисовка платформы

   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
      return;

   switch (Platform_State)
   {
   case EPS_Normal:
      Draw_Normal_State(hdc, paint_area);
      break;

   case EPS_Meltdown:
      Draw_Meltdown_State(hdc, paint_area);
      break;

   case EPS_Roll_In:
      Draw_Roll_In_State(hdc, paint_area);
      break;

   case EPS_Expand_Roll_In:
      Draw_Expanding_Roll_In_State(hdc, paint_area);
      break;
   }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area)
{
   int x = X_Pos;
   int y = AsConfig::Platform_Y_Pos;

   //0.Зарисовка предыдущего положения платформы
   Clear_BG(hdc);

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
   Draw_Circle_Highlight(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT &paint_area)
{
   int i, j;
   int x, y;
   int area_width, area_height;
   int y_offset = 0;
   COLORREF pixel, bg_pixel;
   
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
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT &paint_area)
{
   int x = X_Pos * AsConfig::Global_Scale;
   int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   int roller_size = Circle_Size * AsConfig::Global_Scale;
   double angle;
   XFORM xform, old_xform;

   //0. Зарисовка фона предыдущего положения платформы
   Clear_BG(hdc);

   //1.отрисовка шариков платформы
   SelectObject(hdc, Platform_Circle_Brush);
   SelectObject(hdc, Platform_Circle_Pen);

   Ellipse(hdc, x, y, x + roller_size, y + roller_size);

   //2.Отрисовка разделительной линии
   SetGraphicsMode(hdc, GM_ADVANCED);
   GetWorldTransform(hdc, &old_xform);

   angle = 2 * M_PI * Rolling_Step / Max_Rolling_Step;
   
   xform.eM11 = (float)cos(angle); xform.eM12 = float(-sin(angle) );
   xform.eM21 = (float)sin(angle); xform.eM22 = (float)cos(angle);

   xform.eDx = x + roller_size / 2;
   xform.eDy = y + roller_size / 2;

   SetWorldTransform(hdc, &xform);
   SelectObject(hdc, AsConfig::BG_Brush);
   SelectObject(hdc, AsConfig::BG_Pen);

   Rectangle(hdc, -AsConfig::Global_Scale / 2, -roller_size / 2, AsConfig::Global_Scale / 2, roller_size / 2);
   SetWorldTransform(hdc, &old_xform);

   //3.Отрисовка блика
   Draw_Circle_Highlight(hdc, x, y);

   //4.Others
   ++Rolling_Step;

   if (Rolling_Step > Max_Rolling_Step)
      Rolling_Step -= Max_Rolling_Step;

   X_Pos -= Rolling_Platform_Speed;

   if (X_Pos <= Roll_In_Platform_End_X_Pos)
   {
      X_Pos =  Roll_In_Platform_End_X_Pos;
      Inner_Width = 1;
      Platform_State = EPS_Expand_Roll_In;
   }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Expanding_Roll_In_State(HDC hdc, RECT &paint_area)
{
   Draw_Normal_State(hdc, paint_area);
	Inner_Width += 2;
   X_Pos -= 1;

   if (Inner_Width == Normal_Platform_Inner_Width)
   {
		Platform_State = EPS_Normal;
      Redraw_Platform();
   }   
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle_Highlight(HDC hdc, int x, int y)
{
   SelectObject(hdc, Highlight_Pen);

   Arc(hdc, x + 1 * AsConfig::Global_Scale, y + 1 * AsConfig::Global_Scale,
      x + (Circle_Size - 1) * AsConfig::Global_Scale, y + (Circle_Size - 1) * AsConfig::Global_Scale,
      x + 2 * AsConfig::Global_Scale, y + 1 * AsConfig::Global_Scale,
      x + 1 * AsConfig::Global_Scale, y + 3 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Clear_BG(HDC hdc)
{
   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
}
//------------------------------------------------------------------------------------------------------------