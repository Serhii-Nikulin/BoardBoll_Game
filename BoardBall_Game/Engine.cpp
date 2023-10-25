﻿#include "Engine.h"

//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine():
   Hwnd(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{//настройка игры при старте

   Hwnd = hwnd;

   AActive_Brick::Setup_Colors();

   Ball.Init();
   Level.Init();
   Platform.Init();
   Border.Init();

   Platform.Redraw_Platform(hwnd);

   SetTimer(Hwnd, Timer_ID, 1000 / AsConfig::FPS, NULL);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{//отрисовка экрана игры

   Ball.Draw(hdc, paint_area);
   Level.Draw(Hwnd, hdc, paint_area);//length = 12, height = 14
   Platform.Draw(hdc, paint_area);//lxb = 28x7, R=7, inner = 21
   Border.Draw(hdc);

 /*  for (int i = 0; i < 16; i++)
   {
   Level.Draw_Brick_Letter(hdc, 20 + i * AsConfig::Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
   Level.Draw_Brick_Letter(hdc, 20 + i * Level.Cell_Width * Global_Scale, 150, EBT_Red,  ELT_O, i);
   }*/
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{//Обработка нажатия клавиш

   if (Platform.Platform_State == EPS_Meltdown)
      return 0;

   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;

      if (AsConfig::Border_X_Offset >= Platform.X_Pos)
         Platform.X_Pos = AsConfig::Border_X_Offset;

      Platform.Redraw_Platform(Hwnd);
      break;

   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;

      if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width)
         Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width;

      Platform.Redraw_Platform(Hwnd);
      break;

   case EKT_Space:

      break;
   }
   return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{//отработка событий таймера

   ++AsConfig::Current_Timer_Tick;

   Ball.Move(Hwnd, Platform.X_Pos, Platform.Width, &Level);
   Level.Active_Brick.Act(Hwnd);
   Platform.Act(Hwnd);

   return 0;
}
//------------------------------------------------------------------------------------------------------------

