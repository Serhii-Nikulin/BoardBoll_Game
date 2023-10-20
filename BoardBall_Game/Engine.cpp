#include "Engine.h"

//Config
HWND Hwnd;
HPEN BG_Pen;
HBRUSH BG_Brush;

//Level
RECT Level_Rect;
char Level_01[AsEngine::Level_Height][AsEngine::Level_Width] = {
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//Brick
HPEN Brick_Blue_Pen, Brick_Red_Pen;
HBRUSH Brick_Blue_Brush, Brick_Red_Brush;

//Platform
HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;
RECT Platform_Rect, Prev_Platform_Rect;

//Letter
HPEN Letter_Pen;

//Ball
RECT Ball_Rect, Prev_Ball_Rect;
HPEN Ball_Pen;
HBRUSH Ball_Brush;

//Bounds
HPEN Border_Blue_Pen, Border_White_Pen, Border_Perfaration_Pen;
HBRUSH Border_Blue_Brush, Border_White_Brush, Border_Perfaration_Brush;
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine():
   Inner_Width(21), Platform_Width(28), Platform_X_Pos(50),
   Ball_X_Pos(Platform_X_Pos + (Platform_Width - Ball_Size) / 2),
   Ball_Y_Pos(Platform_Y_Pos + 1 - Ball_Size), Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4)
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{//настройка игры при старте

   Hwnd = hwnd;

   Redraw_Platform();

   Create_Pen_Brush(0, 0, 0, BG_Pen, BG_Brush);

   Create_Pen_Brush(62, 71, 202, Brick_Blue_Pen, Brick_Blue_Brush);
   Create_Pen_Brush(235, 28, 35, Brick_Red_Pen, Brick_Red_Brush);

   Create_Pen_Brush(1661, 81, 167, Platform_Circle_Pen, Platform_Circle_Brush);
   Create_Pen_Brush(32, 174, 73, Platform_Inner_Pen, Platform_Inner_Brush);
   Highlight_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255) );
   Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(255, 255, 255) );

   Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);

   Level_Rect.left = Level_X_Offset * Global_Scale;
   Level_Rect.top = Level_Y_Offset * Global_Scale;
   Level_Rect.right = Level_Rect.left + Level_Width * Cell_Width * Global_Scale;
   Level_Rect.bottom = Level_Rect.top + Level_Height * Cell_Height * Global_Scale;

   Create_Pen_Brush(57, 210, 190, Border_Blue_Pen, Border_Blue_Brush);
   Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
   Create_Pen_Brush(0, 0, 0, Border_Perfaration_Pen, Border_Perfaration_Brush);

   SetTimer(Hwnd, Timer_ID, 50, NULL);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{//отрисовка экрана игры

   RECT intersection_rect;//прямоугольник для нахождения пересечения областей

   //Запрос на отрисовку уровня
   if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect) )
      Draw_Level(hdc);//length = 12, height = 14

   //Запрос на отрисовку платформы
   if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
      Draw_Platform(hdc, Platform_X_Pos, Platform_Y_Pos);//lxb = 28x7, R=7, inner = 21

   //Запрос на отрисовку мяча
   if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      Draw_Ball(hdc);

   Draw_Bounds(hdc);

   /*for (int i = 0; i < 16; i++)
   {
   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 150, EBT_Red,  ELT_O, i);
   }*/
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{//Обработка нажатия клавиш

   switch (key_type)
   {
   case EKT_Left:
      Platform_X_Pos -= Platform_X_Step;

      if (Border_X_Offset >= Platform_X_Pos)
         Platform_X_Pos = Border_X_Offset;

      Redraw_Platform();
      break;

   case EKT_Right:
      Platform_X_Pos += Platform_X_Step;

      if (Platform_X_Pos >= Max_X_Pos - Platform_Width)
         Platform_X_Pos = Max_X_Pos - Platform_Width;

      Redraw_Platform();
      break;

   case EKT_Space:

      break;
   }
   return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{//отработка событий таймера

   Move_Ball();
   return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b) );
   brush = CreateSolidBrush(RGB(r, g, b) );
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Redraw_Platform()
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = Platform_X_Pos * Global_Scale;
   Platform_Rect.top = Platform_Y_Pos * Global_Scale;
   Platform_Rect.right = Platform_Rect.left + Platform_Width * Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Global_Scale;

   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{//отрисовка кирпича

   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_Blue:
      brush = Brick_Blue_Brush;
      pen = Brick_Blue_Pen;
      break;

   case EBT_Red:
      brush = Brick_Red_Brush;
      pen = Brick_Red_Pen;
      break;

   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);
   RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale,
      2 * Global_Scale, 2 * Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Level(HDC hdc)
{//отрисовка кирпичей уровня

   int i, j;

   for (i = 0; i < Level_Height; i++)
      for (j = 0; j < Level_Width; j++)
         Draw_Brick(hdc, Level_X_Offset + Cell_Width * j, Level_Y_Offset + Cell_Height * i, EBrick_Type(Level_01[i][j]));
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Platform(HDC hdc, int x, int y)
{//отрисовка платформы
   
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   //1.отрисовка шариков платформы
   SelectObject(hdc, Platform_Circle_Brush);
   SelectObject(hdc, Platform_Circle_Pen);
  
   Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Inner_Width + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);

   //2.Отрисовка внутренней части
   SelectObject(hdc, Platform_Inner_Brush);
   SelectObject(hdc, Platform_Inner_Pen);

   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + (Inner_Width - 1)) * Global_Scale, (y + 1 + 5) * Global_Scale,
      5 * Global_Scale, 5 * Global_Scale);

   //3.Отрисовка дуги
   SelectObject(hdc, Highlight_Pen);

   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale,
            (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale,
            (x + 2) * Global_Scale, (y + 1) * Global_Scale,
            x * Global_Scale, (y + 3) * Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
{
   if (is_switch_color)//если перевернутый синий или неперевенутый красный
   {
      front_pen = Brick_Red_Pen;
      front_brush = Brick_Red_Brush;

      back_pen = Brick_Blue_Pen;
      back_brush = Brick_Red_Brush;
   }
   else//если перевернутый красный или неперевернутый синий
   {
      front_pen = Brick_Blue_Pen;
      front_brush = Brick_Blue_Brush;

      back_pen = Brick_Red_Pen;
      back_brush = Brick_Red_Brush;
   }
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{//вывод падающей буквы
   
   bool switch_color;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;

   double rotation_angle;
   double offset;
   int brick_half_height;
   double back_part_offset;

   if (brick_type != EBT_Blue && brick_type != EBT_Red)
      return;

   rotation_step = rotation_step % 16;

   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16.0 * rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16.0 * (rotation_step - 8);


	if (rotation_step > 4 && rotation_step <= 12)
	{
		if (brick_type == EBT_Blue)
			switch_color = true;
		else
			switch_color = false;
	}
	else
	{
		if (brick_type == EBT_Blue)
			switch_color = false;
		else
			switch_color = true;
	}

	Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

   if (rotation_step == 4 || rotation_step == 12)//вывод киприча в горизонтальном положении
   {
      //выводим фон
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);
      Rectangle(hdc, x, y - Global_Scale, x + Brick_Width * Global_Scale, y - 1);

      //выводим передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      Rectangle(hdc, x, y, x + Brick_Width * Global_Scale, y + Global_Scale - 1);
   }
   else
   {
      XFORM xForm, prev_xForm;
      SetGraphicsMode(hdc, GM_ADVANCED);
      //сохраняем текущее пространство преобразования в матрицу
      GetWorldTransform(hdc, &prev_xForm);

      brick_half_height = Brick_Height * Global_Scale / 2.0;

      xForm.eM11 = 1.0; xForm.eM12 = 0.0;
      xForm.eM21 = 0.0; xForm.eM22 = cos(rotation_angle);

      //смещение начала новой системы координат относительно старой
      xForm.eDx = x;    xForm.eDy = y;

      //устанавливаем новое пространство преобразования
      SetWorldTransform(hdc, &xForm);

      offset = 3.0 * (1.0 - fabs(xForm.eM22) ) * Global_Scale;

      //0.0, 0.68, 2.63, 5.55, 8.99, 5.55, 2.63, 0.68, 0.0, 0.68, 2.63, 5.55, 8.99, 5.55, 2.63, 0.68

      back_part_offset = round(offset);

      //выводим фон
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * Global_Scale, brick_half_height - back_part_offset);

      //выводим передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, -brick_half_height, Brick_Width * Global_Scale, brick_half_height);

      //вывод буквы на кирпиче
      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * Global_Scale, -5 * Global_Scale / 2, 0 + 10 * Global_Scale, 5 * Global_Scale / 2);
         }
      }
      //устанавливаем исходное пространство преобразования
      SetWorldTransform(hdc, &prev_xForm);
   }
   
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Ball(HDC hdc)
{
   //Зарисовуем прошлое положение мяча фоном
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);
   Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

   //Отрисовуем новое положение мяча
   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);
   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Border(HDC hdc, int x, int y, bool top_border)
{
   SelectObject(hdc, Border_Blue_Brush);
   SelectObject(hdc, Border_Blue_Pen);
   if (top_border)
      Rectangle(hdc, x * Global_Scale, (y + 1) * Global_Scale, (x + 4) * Global_Scale, (y + 4) * Global_Scale);
   else
      Rectangle(hdc, (x + 1) * Global_Scale, y * Global_Scale, (x + 4) * Global_Scale, (y + 4) * Global_Scale);

   SelectObject(hdc, Border_White_Brush);
   SelectObject(hdc, Border_White_Pen);
   if (top_border)
      Rectangle(hdc, x * Global_Scale, y * Global_Scale, (x + 4) * Global_Scale, (y + 1) * Global_Scale);
   else
      Rectangle(hdc, x * Global_Scale, y * Global_Scale, (x + 1) * Global_Scale, (y + 4) * Global_Scale);

   SelectObject(hdc, Border_Perfaration_Brush);
   SelectObject(hdc, Border_Perfaration_Pen);
   if (top_border)
      Rectangle(hdc, (x + 2) * Global_Scale, (y + 2) * Global_Scale, (x + 3) * Global_Scale, (y + 3) * Global_Scale);
   else
      Rectangle(hdc, (x + 2) * Global_Scale, (y + 1) * Global_Scale, (x + 3) * Global_Scale, (y + 2) * Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Bounds(HDC hdc)
{
   for (int i = 0; i < 50; i++)
      Draw_Border(hdc, 2, 1 + 4 * i, false);

   for (int i = 0; i < 50; i++)
      Draw_Border(hdc, Max_X_Pos, 1 + 4 * i, false);

   for (int i = 0; i < 50; i++)
      Draw_Border(hdc, 3 + i * 4, 0, true);
  
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Check_Level_Brick_Hit(int &next_y_pos)
{//проверка поподания по кирпичу
 
   int i, j;
   int brick_y_pos = Level_Y_Offset + (Level_Height - 1) * Cell_Height + Brick_Height;

   for (i = Level_Height - 1; i >= 0; i--)
   {
      for (j = 0; j < Level_Width; j++)
      {
         if (Level_01[i][j] == 0)
            continue;

         if(next_y_pos < brick_y_pos)
         {
            next_y_pos = brick_y_pos + (brick_y_pos - next_y_pos);
            Ball_Direction = -Ball_Direction;
         }
      }
      brick_y_pos -= Cell_Height;
   }
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Move_Ball()
{//перерасчёт позиция мяча и запрос на его перерисовку

   int next_x_pos, next_y_pos;
   int max_x_pos = Max_X_Pos - Ball_Size;
   int max_y_pos = Max_Y_Pos - Ball_Size;
   int platform_y_pos = Platform_Y_Pos - Ball_Size;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction) );
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction) );

   if (Border_X_Offset > next_x_pos)
   {
      next_x_pos = Border_X_Offset + (Border_X_Offset - next_x_pos);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (Border_Y_Offset > next_y_pos)
   {
      next_y_pos = Border_Y_Offset + (Border_Y_Offset - next_y_pos);
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
      if(next_x_pos >= Platform_X_Pos && next_x_pos + Ball_Size <= Platform_X_Pos + Platform_Width)
      {
         next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
         Ball_Direction = -Ball_Direction;
      }
   }

   Check_Level_Brick_Hit(next_y_pos);

   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Ball_Rect.left = Ball_X_Pos * Global_Scale;
   Ball_Rect.top = Ball_Y_Pos * Global_Scale;
   Ball_Rect.right = Ball_Rect.left + Ball_Size * Global_Scale;
   Ball_Rect.bottom = Ball_Rect.top  + Ball_Size * Global_Scale;

   InvalidateRect(Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------