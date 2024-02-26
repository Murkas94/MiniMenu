#include "PushCallbackEntry.h"
#include "../MiniMenu.h"

using namespace MiniMenu;

bool PushCallbackMenuEntry::Handle(bool isSelected)
{
    if(isSelected && menu.input.enter.IsTriggered())
    {
        callback();
    }
    return false;
}
void PushCallbackMenuEntry::Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected)
{
    MenuEntry::Draw(gfx, area, isSelected);
}
uint16_t PushCallbackMenuEntry::GetRequiredHeight()
{
    return MenuEntry::GetRequiredHeight();
}
