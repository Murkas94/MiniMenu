#ifndef MINI_MENU_DRAWUTILS_H
#define MINI_MENU_DRAWUTILS_H

#include <stdint.h>
#include "../Geometry.h"
#include <Adafruit_GFX.h>

namespace MiniMenu
{
namespace Utils
{
void DrawTextLeft(Adafruit_GFX& gfx, const MiniMenu::Rect& area, uint16_t color, const char* text);
void DrawTextRight(Adafruit_GFX& gfx, const MiniMenu::Rect& area, uint16_t color, const char* text);
void DrawTextCenter(Adafruit_GFX& gfx, const MiniMenu::Rect& area, uint16_t color, const char* text);
}
}

#endif
