﻿#include "Level.h"

//Level
//------------------------------------------------------------------------------------------------------------
char Level_01[AsConfig::Level_Height][AsConfig::Level_Width] = {
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
//------------------------------------------------------------------------------------------------------------
ALevel::ALevel()
   :Level_Rect{}, Letter_Pen{}, Brick_Blue_Pen(0), Brick_Red_Pen(0),Brick_Blue_Brush(0), Brick_Red_Brush(0)
{
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
   Letter_Pen = CreatePen(PS_SOLID, AsConfig::Global_Scale, RGB(255, 255, 255) );
   AsConfig::Create_Pen_Brush(62, 71, 202, Brick_Blue_Pen, Brick_Blue_Brush);
   AsConfig::Create_Pen_Brush(235, 28, 35, Brick_Red_Pen, Brick_Red_Brush);

   Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
   Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
   Level_Rect.right = Level_Rect.left + AsConfig::Level_Width * AsConfig::Cell_Width * AsConfig::Global_Scale;
   Level_Rect.bottom = Level_Rect.top + AsConfig::Level_Height * AsConfig::Cell_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction)
{//проверка поподания по кирпичу

   int i, j;
   int brick_y_pos = AsConfig::Level_Y_Offset + (AsConfig::Level_Height - 1) * AsConfig::Cell_Height + AsConfig::Brick_Height;

   for (i = AsConfig::Level_Height - 1; i >= 0; i--)
   {
      for (j = 0; j < AsConfig::Level_Width; j++)
      {
         if (Level_01[i][j] == 0)
            continue;

         if(next_y_pos < brick_y_pos)
         {
            next_y_pos = brick_y_pos + (brick_y_pos - next_y_pos);
            ball_direction = -ball_direction;
         }
      }
      brick_y_pos -= AsConfig::Cell_Height;
   }
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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
   RoundRect(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Brick_Width) * AsConfig::Global_Scale, (y + AsConfig::Brick_Height) * AsConfig::Global_Scale,
      2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT &paint_area)
{//отрисовка кирпичей уровня

   int i, j;
   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Level_Rect) )
      return;

   for (i = 0; i < AsConfig::Level_Height; i++)
      for (j = 0; j < AsConfig::Level_Width; j++)
         Draw_Brick(hdc, AsConfig::Level_X_Offset + AsConfig::Cell_Width * j, AsConfig::Level_Y_Offset + AsConfig::Cell_Height * i, EBrick_Type(Level_01[i][j]));
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
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
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
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
      Rectangle(hdc, x, y - AsConfig::Global_Scale, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y - 1);

      //выводим передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      Rectangle(hdc, x, y, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + AsConfig::Global_Scale - 1);
   }
   else
   {
      XFORM xForm, prev_xForm;
      SetGraphicsMode(hdc, GM_ADVANCED);
      //сохраняем текущее пространство преобразования в матрицу
      GetWorldTransform(hdc, &prev_xForm);

      brick_half_height = AsConfig::Brick_Height * int(AsConfig::Global_Scale / 2.0);

      xForm.eM11 = 1.0f; xForm.eM12 = 0.0f;
      xForm.eM21 = 0.0f; xForm.eM22 = float(cos(rotation_angle) );

      //смещение начала новой системы координат относительно старой
      xForm.eDx = float(x);    xForm.eDy = float(y);

      //устанавливаем новое пространство преобразования
      SetWorldTransform(hdc, &xForm);

      offset = 3.0 * (1.0 - fabs(xForm.eM22) ) * AsConfig::Global_Scale;

      //0.0, 0.68, 2.63, 5.55, 8.99, 5.55, 2.63, 0.68, 0.0, 0.68, 2.63, 5.55, 8.99, 5.55, 2.63, 0.68

      back_part_offset = round(offset);

      //выводим фон
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, 0, -brick_half_height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height - back_part_offset);

      //выводим передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, -brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height);

      //вывод буквы на кирпиче
      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, -5 * AsConfig::Global_Scale / 2, 0 + 10 * AsConfig::Global_Scale, 5 * AsConfig::Global_Scale / 2);
         }
      }
      //устанавливаем исходное пространство преобразования
      SetWorldTransform(hdc, &prev_xForm);
   }

}
//------------------------------------------------------------------------------------------------------------