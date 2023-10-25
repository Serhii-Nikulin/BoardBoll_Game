#include "Config.h"

//AColor
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
   : R(r), G(g), B(b)
{
}
//------------------------------------------------------------------------------------------------------------




//AsConfig
//------------------------------------------------------------------------------------------------------------
int AsConfig::Current_Timer_Tick = 0;
const AColor AsConfig::Blue_Brick_Color(62, 71, 202);
const AColor AsConfig::Red_Brick_Color(235, 28, 35);
const AColor AsConfig::BG_Color(0, 0, 0);
HPEN AsConfig::BG_Pen = 0;
HBRUSH AsConfig::BG_Brush = 0;
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b) );
   brush = CreateSolidBrush(RGB(r, g, b) );
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(const AColor &color, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B) );
   brush = CreateSolidBrush(RGB(color.R, color.G, color.B) );
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Setup_Colors()
{
   AsConfig::Create_Pen_Brush(AsConfig::BG_Color, AsConfig::BG_Pen, AsConfig::BG_Brush);
}
//------------------------------------------------------------------------------------------------------------
int AsConfig::Rand(int range)
{
   return rand() * range / RAND_MAX;
}
//------------------------------------------------------------------------------------------------------------
