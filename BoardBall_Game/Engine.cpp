#include "Engine.h"

//ABall
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
   :Ball_Pen(0), Ball_Brush(0), Ball_X_Pos(50 + (28 - Ball_Size) / 2),
   Ball_Y_Pos(185 + 1 - Ball_Size), Ball_Direction(M_PI - M_PI_4), Ball_Rect{}, Prev_Ball_Rect{}, Ball_Speed(3.0)
{
   /*Ball_X_Pos = X_Pos + (Width - Ball_Size) / 2),
   Ball_Y_Pos(Y_Pos + 1 - Ball_Size);*/
}
//------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
   AsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)//CRUTCH
{
   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      return;

   //Зарисовуем прошлое положение мяча фоном
   SelectObject(hdc, engine->BG_Pen);
   SelectObject(hdc, engine->BG_Brush);
   Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

   //Отрисовуем новое положение мяча
   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);
   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move(AsEngine *engine, ALevel *level, AsPlatform *platform)
{//перерасчёт позиция мяча и запрос на его перерисовку

   int next_x_pos, next_y_pos;
   int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
   int max_y_pos = AsEngine::Max_Y_Pos - Ball_Size;
   int platform_y_pos = AsPlatform::Y_Pos - Ball_Size;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction) );
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction) );

   if (AsBorder::Border_X_Offset > next_x_pos)
   {
      next_x_pos = AsBorder::Border_X_Offset + (AsBorder::Border_X_Offset - next_x_pos);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (AsBorder::Border_Y_Offset > next_y_pos)
   {
      next_y_pos = AsBorder::Border_Y_Offset + (AsBorder::Border_Y_Offset - next_y_pos);
      Ball_Direction = -Ball_Direction;
   }

   if (next_x_pos > max_x_pos)
   {
      next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (next_y_pos > max_y_pos)
   {
      next_y_pos = max_y_pos - (next_y_pos - max_y_pos);
      Ball_Direction = -Ball_Direction;
   }

   if (next_y_pos > platform_y_pos)
   {
      if(next_x_pos >= platform->X_Pos && next_x_pos + Ball_Size <= platform->X_Pos + platform->Width)
      {
         next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
         Ball_Direction = -Ball_Direction;
      }
   }

   level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Ball_Rect.left = Ball_X_Pos * AsConfig::Global_Scale;
   Ball_Rect.top = Ball_Y_Pos * AsConfig::Global_Scale;
   Ball_Rect.right = Ball_Rect.left + Ball_Size * AsConfig::Global_Scale;
   Ball_Rect.bottom = Ball_Rect.top  + Ball_Size * AsConfig::Global_Scale;

   InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);
}
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
void AsPlatform::Redraw_Platform(AsEngine *engine)
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

   InvalidateRect(engine->Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(engine->Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area, AsEngine* engine)
{//отрисовка платформы

   RECT intersection_rect;
   int x = X_Pos;
   int y = Y_Pos;

   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
      return;

   SelectObject(hdc, engine->BG_Pen);
   SelectObject(hdc, engine->BG_Brush);

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


//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine():
   Hwnd(0), BG_Pen(0), BG_Brush()
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{//настройка игры при старте

   Hwnd = hwnd;

   AsConfig::Create_Pen_Brush(0, 0, 0, BG_Pen, BG_Brush);
  
   Ball.Init();
   Level.Init();
   Platform.Init();
   Border.Init();

   Platform.Redraw_Platform(this);

   SetTimer(Hwnd, Timer_ID, 50, NULL);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{//отрисовка экрана игры

   RECT intersection_rect;//прямоугольник для нахождения пересечения областей

   Ball.Draw(hdc, paint_area, this);
   Level.Draw(hdc, paint_area);//length = 12, height = 14
   Platform.Draw(hdc, paint_area, this);//lxb = 28x7, R=7, inner = 21
   Border.Draw(hdc, BG_Pen, BG_Brush);

 /*  for (int i = 0; i < 16; i++)
   {
   Level.Draw_Brick_Letter(hdc, 20 + i * ALevel::Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
   Level.Draw_Brick_Letter(hdc, 20 + i * Level.Cell_Width * Global_Scale, 150, EBT_Red,  ELT_O, i);
   }*/
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{//Обработка нажатия клавиш

   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;

      if (AsBorder::Border_X_Offset >= Platform.X_Pos)
         Platform.X_Pos = AsBorder::Border_X_Offset;

      Platform.Redraw_Platform(this);
      break;

   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;

      if (Platform.X_Pos >= Max_X_Pos - Platform.Width)
         Platform.X_Pos = Max_X_Pos - Platform.Width;

      Platform.Redraw_Platform(this);
      break;

   case EKT_Space:

      break;
   }
   return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{//отработка событий таймера

   Ball.Move(this, &Level, &Platform);
   return 0;
}
//------------------------------------------------------------------------------------------------------------

