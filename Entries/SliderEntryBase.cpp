#include "SliderEntryBase.h"
#include "../MiniMenu.h"

using namespace MiniMenu;

uint16_t SliderMenuEntryGraphics::GetSliderGraphicsRequiredHeight(MenuEntry& menuEntry, Menu& menu)
{
    return GetBaseRequiredHeight() + menu.parameters.fontsize + 3;
}

void SliderMenuEntryGraphics::DrawSliderGraphics(MenuEntry& menuEntry, Menu& menu, Adafruit_GFX& gfx, const Rect& area, bool isSelected, float value)
{
    value = min(1.f, max(0.f, value));
    DrawBase(gfx, area, isSelected);
    Rect valueFrameArea = area;
    valueFrameArea.pos.x += 2;
    valueFrameArea.size.x -= 4;
    uint16_t baseHeight = GetBaseRequiredHeight();
    valueFrameArea.pos.y += baseHeight + 1;
    valueFrameArea.size.y = menu.parameters.fontsize + 1;
    gfx.drawRoundRect(valueFrameArea.pos.x, valueFrameArea.pos.y, valueFrameArea.size.x, valueFrameArea.size.y, 2, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor);
    uint16_t sliderWidth = 14;
    uint16_t freeFrameWidth = valueFrameArea.size.x - sliderWidth;
    Rect sliderArea = valueFrameArea;
    sliderArea.size.x = sliderWidth;
    sliderArea.pos.x += static_cast<int16_t>(value * freeFrameWidth + 0.5f);
    gfx.fillRoundRect(sliderArea.pos.x, sliderArea.pos.y, sliderArea.size.x, sliderArea.size.y, 2, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor);
    if(menu.valueInput && isSelected)
    {
        gfx.setTextColor(menu.parameters.selectedBackgroundColor);
        gfx.setCursor(sliderArea.pos.x, sliderArea.pos.y + 1);
        gfx.print('<');
        gfx.setCursor(sliderArea.pos.x+1, sliderArea.pos.y + 1);
        gfx.print('<');
        gfx.setCursor(sliderArea.pos.x + sliderArea.size.x - 6, sliderArea.pos.y + 1);
        gfx.print('>');
        gfx.setCursor(sliderArea.pos.x + sliderArea.size.x - 7, sliderArea.pos.y + 1);
        gfx.print('>');
    }
}
