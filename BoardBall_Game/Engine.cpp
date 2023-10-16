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

const int Circle_Size = 7;
int Inner_Width = 21;

HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;
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
void Draw_Frame(HDC hdc)
{//отрисовка экрана игры

   Draw_Level(hdc);//lenth = 12, height = 14
   Draw_Platform(hdc, 110, 150);//lxb = 28x7, R=7, inner = 21
}
//------------------------------------------------------------------------------------------------------------