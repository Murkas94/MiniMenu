#include "GenericValueMenuEntry.h"
#include "../MiniMenu.h"

using namespace MiniMenu;

uint16_t GenericValueMenuEntry::GetRequiredHeight()
{
    return MenuEntry::GetRequiredHeight() + menu.parameters.fontsize + 2;
}

void GenericValueMenuEntry::Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected)
{
    MenuEntry::Draw(gfx, area, isSelected);
    if(getter == nullptr){return;}
    if(formatter == nullptr) { SetDefaultFormatter();}
    Generics::ValueContainer* data = getter->GetValue();
    if(data == nullptr){return;}
    String text = formatter->Format(data->GetData(), getter->GetValueType());
    if(getter->ReturnsNewInstances()){ delete data; }
    Rect valueArea = area;
    valueArea.size.x -= 10;
    valueArea.pos.x += 10;
    uint16_t baseHeight = MenuEntry::GetRequiredHeight();
    valueArea.pos.y += baseHeight;
    valueArea.size.y -= baseHeight;
    DrawTextRight(gfx, valueArea, isSelected, text.c_str());
}
