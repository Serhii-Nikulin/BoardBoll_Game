#include "Engine.h"

enum EBrick_Type{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

enum ELetter_Type{
   ELT_None,
   ELT_O
};

char Level_01[14][12] = {
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

const int Global_Scale = 3;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;

HPEN Brick_Blue_Pen, Brick_Red_Pen;
HBRUSH Brick_Blue_Brush, Brick_Red_Brush;

const int Circle_Size = 7;
int Inner_Width = 21;

HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

HPEN Letter_Pen;
//------------------------------------------------------------------------------------------------------------
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b) );
   brush = CreateSolidBrush(RGB(r, g, b) );
}
//------------------------------------------------------------------------------------------------------------
void Init()
{//настройка игры при старте

   Create_Pen_Brush(62, 71, 202, Brick_Blue_Pen, Brick_Blue_Brush);
   Create_Pen_Brush(235, 28, 35, Brick_Red_Pen, Brick_Red_Brush);

   Create_Pen_Brush(1661, 81, 167, Platform_Circle_Pen, Platform_Circle_Brush);
   Create_Pen_Brush(32, 174, 73, Platform_Inner_Pen, Platform_Inner_Brush);
   Highlight_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255) );
   Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(255, 255, 255) );
}
//------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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
void Draw_Level(HDC hdc)
{//отрисовка кирпичей уровня

   int i, j;

   for (i = 0; i < 14; i++)
      for (j = 0; j < 12; j++)
         Draw_Brick(hdc, Level_X_Offset + Cell_Width * j, Level_Y_Offset + Cell_Height * i, EBrick_Type(Level_01[i][j]));
}
//------------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y)
{//отрисовка платформы
   
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
void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
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
void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
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
void Draw_Frame(HDC hdc)
{//отрисовка экрана игры

   //Draw_Level(hdc);//length = 12, height = 14
   //Draw_Platform(hdc, 110, 150);//lxb = 28x7, R=7, inner = 21

   for(int i = 0; i < 16; i++)
   {
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 150, EBT_Red,  ELT_O, i);
   }

}
//------------------------------------------------------------------------------------------------------------