#include "MenuEntry.h"
#include "MiniMenu.h"
#include "Util/DrawUtils.h"

using namespace MiniMenu;

void MenuEntry::DrawTextLeft(Adafruit_GFX& gfx, const Rect& area, uint16_t color, const char* text)
{
    MiniMenu::Utils::DrawTextLeft(gfx, area, color, text);
}

void MenuEntry::DrawTextRight(Adafruit_GFX& gfx, const Rect& area, uint16_t color, const char* text)
{
    MiniMenu::Utils::DrawTextRight(gfx, area, color, text);
}

void MenuEntry::DrawTextCenter(Adafruit_GFX& gfx, const Rect& area, uint16_t color, const char* text)
{
    MiniMenu::Utils::DrawTextCenter(gfx, area, color, text);
}

void MenuEntry::DrawTextLeft(Adafruit_GFX& gfx, const Rect& area, bool isSelected, const char* text)
{
    DrawTextLeft(gfx, area, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor, text);
}
void MenuEntry::DrawTextCenter(Adafruit_GFX& gfx, const Rect& area, bool isSelected, const char* text)
{
    DrawTextCenter(gfx, area, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor, text);
}
void MenuEntry::DrawTextRight(Adafruit_GFX& gfx, const Rect& area, bool isSelected, const char* text)
{
    DrawTextRight(gfx, area, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor, text);
}

void MenuEntry::EnterEditMode()
{
    menu.valueInput = true;
}
void MenuEntry::ExitEditMode()
{
    menu.valueInput = false;
}
bool MenuEntry::IsEditMode()
{
    return menu.valueInput;
}

void MenuEntry::Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected)
{
    if(isSelected)
    {
        gfx.fillRect(area.pos.x, area.pos.y, area.size.x, area.size.y, menu.parameters.selectedBackgroundColor);
    }
    if(title.isEmpty() == false)
    {
        switch(titlePosition)
        {
            case HorizontalPosition::Left:
                DrawTextLeft(gfx, area, isSelected, title.c_str());
                break;
            case HorizontalPosition::Center:
                DrawTextCenter(gfx, area, isSelected, title.c_str());
                break;
            case HorizontalPosition::Right:
                DrawTextRight(gfx, area, isSelected, title.c_str());
                break;
            default:
                break;
        }
    }
}

uint16_t MenuEntry::GetRequiredHeight()
{
    if(!title.isEmpty())
        return menu.parameters.fontsize + 2;
    return 0;
}
