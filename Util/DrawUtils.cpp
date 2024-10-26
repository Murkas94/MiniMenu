#include "DrawUtils.h"

using namespace MiniMenu;
using namespace MiniMenu::Utils;

void MiniMenu::Utils::DrawTextLeft(Adafruit_GFX& gfx, const MiniMenu::Rect& area, uint16_t color, const char* text)
{
    gfx.setCursor(area.pos.x + 1, area.pos.y + 1);
    gfx.setTextWrap(false);
    gfx.setTextColor(color);
    gfx.setTextSize(1);
    gfx.print(text);
}

void MiniMenu::Utils::DrawTextRight(Adafruit_GFX& gfx, const MiniMenu::Rect& area, uint16_t color, const char* text)
{
    gfx.setTextWrap(false);
    gfx.setTextSize(1);
    Rect bounds;
    gfx.getTextBounds(text, area.pos.x, area.pos.y, &bounds.pos.x, &bounds.pos.y, reinterpret_cast<uint16_t*>(&bounds.size.x), reinterpret_cast<uint16_t*>(&bounds.size.y));
    Rect leftArea = area;
    leftArea.pos.x += leftArea.size.x - bounds.size.x;
    //leftArea.size.x = bounds.size.x - 1;
    DrawTextLeft(gfx, leftArea, color, text);
}

void MiniMenu::Utils::DrawTextCenter(Adafruit_GFX& gfx, const MiniMenu::Rect& area, uint16_t color, const char* text)
{
    gfx.setTextWrap(false);
    gfx.setTextSize(1);
    Rect bounds;
    gfx.getTextBounds(text, area.pos.x, area.pos.y, &bounds.pos.x, &bounds.pos.y, reinterpret_cast<uint16_t*>(&bounds.size.x), reinterpret_cast<uint16_t*>(&bounds.size.y));
    Rect leftArea = area;
    leftArea.pos.x += (leftArea.size.x - bounds.size.x) / 2;
    //leftArea.size.x = bounds.size.x - 1;
    DrawTextLeft(gfx, leftArea, color, text);
}
