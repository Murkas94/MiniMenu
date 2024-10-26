#include "MultiGenericValueMenuEntry.h"
#include "../MiniMenu.h"
#include "../Util/DrawUtils.h"

using namespace MiniMenu;

void MultiGenericValueMenuEntryBase::ValueField::Draw(Menu& menu, Adafruit_GFX& gfx, const Rect& area, bool isSelected)
{
    MiniMenu::Utils::DrawTextLeft(gfx, area, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor, caption.c_str());
    if(getter == nullptr){ return; }
    if(formatter == nullptr) { SetDefaultFormatter(); }
    Generics::ValueContainer* data = getter->GetValue();
    if(data == nullptr){return;}
    String text = formatter->Format(data->GetData(), getter->GetValueType());
    if(getter->ReturnsNewInstances()){ delete data; }
    MiniMenu::Utils::DrawTextRight(gfx, area, isSelected ? menu.parameters.selectedTextColor : menu.parameters.textColor, text.c_str());
}
