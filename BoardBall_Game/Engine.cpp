#include "Engine.h"

//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
   :Game_State(EGS_Play_Level)
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{//настройка игры при старте
   int platform_x_pos, ball_x_pos;
   AsConfig::Hwnd = hwnd;

   AActive_Brick::Setup_Colors();

   platform_x_pos = AsConfig::Border_X_Offset + (AsConfig::Max_X_Pos - AsConfig::Border_X_Offset) / 2;
   Platform.Init(platform_x_pos);
   ball_x_pos = Platform.X_Pos + Platform.Width / 2 - AsConfig::Ball_Size / 2;
   Ball.Init();
   Ball.Set_State(EBS_Normal, ball_x_pos);
   Level.Init();
   Border.Init();

   Platform.Set_State(EPS_Normal);
   Platform.Redraw_Platform();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, NULL);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{//отрисовка экрана игры

   Ball.Draw(hdc, paint_area);
   Level.Draw(hdc, paint_area);//length = 12, height = 14
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
   int ball_x_pos;

   if (! (Platform.Platform_State == EPS_Normal || Platform.Platform_State == EPS_Ready) )
      return 0;

   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;

      if (AsConfig::Border_X_Offset >= Platform.X_Pos)
         Platform.X_Pos = AsConfig::Border_X_Offset;

      Platform.Redraw_Platform();
      break;

   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;

      if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width)
         Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width;

      Platform.Redraw_Platform();
      break;

   case EKT_Space:

      if (Platform.Get_State() == EPS_Ready)
      {
         ball_x_pos = Platform.X_Pos + Platform.Width / 2 - AsConfig::Ball_Size / 2;
         Ball.Set_State(EBS_Normal, ball_x_pos);
         Platform.Set_State(EPS_Normal);
      }
      break;
   }
   return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{//отработка событий таймера

   ++AsConfig::Current_Timer_Tick;
   int ball_x_pos;

   switch (Game_State)
   {
   case EGS_Play_Level:
      Ball.Move(&Level, &Border, &Platform);

      if (Ball.Get_State() == EBS_Lost)
      {
         Game_State = EGS_Lost_Ball;
         Platform.Set_State(EPS_Meltdown);
      }

      break;
   
   case EGS_Lost_Ball:

      if (Platform.Get_State() == EPS_Missing)
      {
         Game_State = EGS_Restart_Level;
         Platform.Set_State(EPS_Roll_In);
      }

      break;

   case EGS_Restart_Level:

      if (Platform.Get_State() == EPS_Ready)
      {
         ball_x_pos = Platform.X_Pos + Platform.Width / 2 - AsConfig::Ball_Size / 2;
         Game_State = EGS_Play_Level;
         Ball.Set_State(EBS_On_Platform, ball_x_pos);
      }

      break;

   }
   //Level.Active_Brick.Act(AsConfig::Hwnd);
   Platform.Act();

   return 0;
}
//------------------------------------------------------------------------------------------------------------

