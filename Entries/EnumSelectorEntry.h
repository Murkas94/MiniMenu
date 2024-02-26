#ifndef ENUM_SELECTOR_MENU_ENTRY_H
#define ENUM_SELECTOR_MENU_ENTRY_H

#include "./TypedEntryBase.h"
#include <type_traits>
#include <vector>

namespace MiniMenu
{

template <typename EnumType>
class EnumSelectorMenuEntry : public TypedMenuEntryBase<EnumSelectorMenuEntry<EnumType>, EnumType>
{
public:    
    EnumSelectorMenuEntry(Menu& menu) : TypedMenuEntryBase<EnumSelectorMenuEntry<EnumType>, EnumType>(menu) {}
    EnumSelectorMenuEntry(Menu& menu, EnumType& target) : TypedMenuEntryBase<EnumSelectorMenuEntry<EnumType>, EnumType>(menu, target) {}
    EnumSelectorMenuEntry(Menu& menu, const EnumType& valueGetter, EnumType& valueSetter) : TypedMenuEntryBase<EnumSelectorMenuEntry<EnumType>, EnumType>(menu, valueGetter, valueSetter) { }
    EnumSelectorMenuEntry(Menu& menu, typename TypedMenuEntryStaticBase<EnumType>::FunctionValueContainer::getterFunctionType getterFunction, typename TypedMenuEntryStaticBase<EnumType>::FunctionValueSetter::setterFunctionType setterFunction) : TypedMenuEntryBase<EnumSelectorMenuEntry<EnumType>, EnumType>(menu, getterFunction, setterFunction) { }
    ~EnumSelectorMenuEntry(){}

    void Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected) override;
    uint16_t GetRequiredHeight() override;
    bool CheckValueChange(MiniMenu::Input& input, EnumType& value) override;

private:
    struct EnumEntry
    {
        std::string Text;
        EnumType Value;

        EnumEntry(const char* _Text, EnumType _Value)
            : Text(_Text)
            , Value(_Value)
        {}
    };
    std::vector<EnumEntry> entries;
    int getSelectedIndex(EnumType Value)
    {
        for(int i = 0;i<entries.size(); ++i)
        {
            if(entries[i].Value == Value){return i;}
        }
        return -1;
    }

public:
    EnumSelectorMenuEntry& AddEntry(EnumType Value, const char* Text)
    {
        entries.push_back(EnumEntry(Text, Value));
        return *this;
    }
};

template <typename EnumType>
uint16_t EnumSelectorMenuEntry<EnumType>::GetRequiredHeight()
{
    return MenuEntry::GetRequiredHeight() + entries.size() * (this->menu.parameters.fontsize + 2);
}

template <typename EnumType>
void EnumSelectorMenuEntry<EnumType>::Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected)
{
    MenuEntry::Draw(gfx, area, isSelected);
    const uint16_t baseHeight = MenuEntry::GetRequiredHeight();
    
    Rect valueArea = area;
    valueArea.size.x -= 10;
    valueArea.pos.x += 10;
    valueArea.pos.y += baseHeight;
    valueArea.size.y = 2 + this->menu.parameters.fontsize;
    if(entries.empty()){ this->DrawTextCenter(gfx, valueArea, isSelected, "Entries missing"); return;}

    int currentIndex = getSelectedIndex(this->currentValue);
    for(int i = 0; i < entries.size(); ++i)
    {
        // Draw background
        if(i == currentIndex)
        {
            gfx.drawRect(valueArea.pos.x, valueArea.pos.y, valueArea.size.x, valueArea.size.y, isSelected ? this->menu.parameters.selectedTextColor : this->menu.parameters.textColor);
        }
        // Draw text
        this->DrawTextCenter(gfx, valueArea, isSelected, entries[i].Text.c_str());

        // prepare position of next entry
        valueArea.pos.y += 2 + this->menu.parameters.fontsize;
    }
}

template <typename EnumType>
bool EnumSelectorMenuEntry<EnumType>::CheckValueChange(MiniMenu::Input& input, EnumType& value)
{
    if(this->IsEditMode())
    {
        if(input.enter.IsTriggered()){ this->ExitEditMode(); return true;}
        int currentIndex = getSelectedIndex(value);
        if(input.up.IsTriggered() && currentIndex > 0)
        {
            value = entries[currentIndex - 1].Value;
            return true;
        }
        if(input.down.IsTriggered() && currentIndex + 1 < entries.size())
        {
            value = entries[currentIndex + 1].Value;
            return true;
        }
    }
    else if(input.enter.IsTriggered())
    {
        this->EnterEditMode();
        return true;
    }
    return false;
}

}

#endif
