#include "Engine.h"

enum EBrick_Type{
   EBT_None,
   EBT_Red,
   EBT_Blue
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

//------------------------------------------------------------------------------------------------------------
void Init()
{//настройка игры при старте

   Brick_Blue_Brush = CreateSolidBrush(RGB(62, 71, 202) );
   Brick_Blue_Pen = CreatePen(PS_SOLID, 0, RGB(62, 71, 202) );

   Brick_Red_Brush = CreateSolidBrush(RGB(235, 28, 35) );
   Brick_Red_Pen = CreatePen(PS_SOLID, 0, RGB(235, 28, 35) );
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
void Draw_Frame(HDC hdc)
{//отрисовка экрана игры

   Draw_Level(hdc);
}
//------------------------------------------------------------------------------------------------------------