#include "BoolCheckboxEntry.h"
#include "../MiniMenu.h"

using namespace MiniMenu;

BoolCheckboxMenuEntry::~BoolCheckboxMenuEntry()
{
    if(Texts)
    {
        delete Texts;
        Texts = nullptr;
    }
}

uint16_t BoolCheckboxMenuEntry::GetRequiredHeight()
{
    switch(style)
    {
        case Style::Checkbox:
        case Style::HorizontalSelectable:
            return MenuEntry::GetRequiredHeight() + menu.parameters.fontsize + 2;
        case Style::VerticalSelectable:
            return MenuEntry::GetRequiredHeight() + 2 * menu.parameters.fontsize + 4;
    }
}

void BoolCheckboxMenuEntry::Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected)
{
    MenuEntry::Draw(gfx, area, isSelected);
    const uint16_t baseHeight = MenuEntry::GetRequiredHeight();
    switch(style)
    {
        case Style::Checkbox:
        {
            // draw checkbox
            Rect valueArea = area;
            valueArea.pos.x = valueArea.pos.x + valueArea.size.x - menu.parameters.fontsize - 1;
            valueArea.size.x = menu.parameters.fontsize;
            valueArea.pos.y += baseHeight + 1;
            valueArea.size.y = menu.parameters.fontsize;
            gfx.drawRect(valueArea.pos.x, valueArea.pos.y, valueArea.size.x, valueArea.size.y, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor);
            if(currentValue)
            {
                gfx.fillRect(valueArea.pos.x+2, valueArea.pos.y+2, valueArea.size.x-4, valueArea.size.y-4, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor);
            }
            // draw text
            if(Texts)
            {
                valueArea = area;
                valueArea.size.x -= 12 + menu.parameters.fontsize; // 10 spacing left + 2 spacing right + size of checkbox (=fontsize)
                valueArea.pos.x += 10;
                valueArea.pos.y += baseHeight;
                valueArea.size.y -= baseHeight;
                DrawTextRight(gfx, valueArea, isSelected, (currentValue ? Texts->EnabledText : Texts->DisabledText).c_str());
            }
            return;
        }
        case Style::HorizontalSelectable:
        {

            Rect valueArea = area;
            valueArea.size.x -= 10;
            valueArea.pos.x += 10;
            valueArea.pos.y += baseHeight;
            valueArea.size.y -= baseHeight;
            if(!Texts){ DrawTextCenter(gfx, valueArea, isSelected, "Text missing"); return;}

            valueArea.size.x /= 2;

            // Draw left (disabled)
            if(!currentValue)
                gfx.drawRect(valueArea.pos.x, valueArea.pos.y, valueArea.size.x, valueArea.size.y, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor);
            DrawTextCenter(gfx, valueArea, isSelected, Texts->DisabledText.c_str());
            
            // Draw right (enabled)
            valueArea.pos.x += valueArea.size.x;
            if(currentValue)
                gfx.drawRect(valueArea.pos.x, valueArea.pos.y, valueArea.size.x, valueArea.size.y, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor);
            DrawTextCenter(gfx, valueArea, isSelected, Texts->EnabledText.c_str());
            return;
        }
        case Style::VerticalSelectable:
        {

            Rect valueArea = area;
            valueArea.size.x -= 10;
            valueArea.pos.x += 10;
            valueArea.pos.y += baseHeight;
            valueArea.size.y -= baseHeight + 2 + menu.parameters.fontsize;
            if(!Texts){ DrawTextCenter(gfx, valueArea, isSelected, "Text missing"); return;}

            // Draw up(enabled)
            if(currentValue)
                gfx.drawRect(valueArea.pos.x, valueArea.pos.y, valueArea.size.x, valueArea.size.y, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor);
            DrawTextCenter(gfx, valueArea, isSelected, Texts->EnabledText.c_str());
            
            // Draw down/disabled
            valueArea.pos.y += 2 + menu.parameters.fontsize;
            if(!currentValue)
                gfx.drawRect(valueArea.pos.x, valueArea.pos.y, valueArea.size.x, valueArea.size.y, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor);
            DrawTextCenter(gfx, valueArea, isSelected, Texts->DisabledText.c_str());
            return;
        }
    }
}

bool BoolCheckboxMenuEntry::CheckValueChange(MiniMenu::Input& input, bool& value)
{
    if(input.enter.IsTriggered())
    {
        value = !value;
        return true;
    }
    return false;
}