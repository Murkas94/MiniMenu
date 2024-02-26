#ifndef BOOL_CHECKBOX_MENU_ENTRY_H
#define BOOL_CHECKBOX_MENU_ENTRY_H

#include "./TypedEntryBase.h"
#include <type_traits>
#include <string>

namespace MiniMenu
{
class BoolCheckboxMenuEntry : public TypedMenuEntryBase<BoolCheckboxMenuEntry, bool>
{
public:    
    BoolCheckboxMenuEntry(Menu& menu) : TypedMenuEntryBase<BoolCheckboxMenuEntry, bool>(menu) {}
    BoolCheckboxMenuEntry(Menu& menu, bool& target) : TypedMenuEntryBase<BoolCheckboxMenuEntry, bool>(menu, target) {}
    BoolCheckboxMenuEntry(Menu& menu, const bool& valueGetter, bool& valueSetter) : TypedMenuEntryBase<BoolCheckboxMenuEntry, bool>(menu, valueGetter, valueSetter) { }
    BoolCheckboxMenuEntry(Menu& menu, typename FunctionValueContainer::getterFunctionType getterFunction, typename FunctionValueSetter::setterFunctionType setterFunction) : TypedMenuEntryBase<BoolCheckboxMenuEntry, bool>(menu, getterFunction, setterFunction) { }
    ~BoolCheckboxMenuEntry();

    void Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected) override;
    uint16_t GetRequiredHeight() override;
    bool CheckValueChange(MiniMenu::Input& input, bool& value) override;

private:
    struct BoolText
    {
        std::string EnabledText;
        std::string DisabledText;
    };
    BoolText* Texts = nullptr;

public:
    BoolCheckboxMenuEntry& SetTexts(const char* EnabledText, const char* DisabledText)
    {
        if(!Texts){Texts = new BoolText();}
        Texts->EnabledText = EnabledText;
        Texts->DisabledText = DisabledText;

        return *this;
    }

    enum Style
    {
        Checkbox,
        HorizontalSelectable,
        VerticalSelectable
    };

private:
    Style style = Style::Checkbox;

public:
    BoolCheckboxMenuEntry& SetStyle(Style newStyle)
    {
        style = newStyle;
        return *this;
    }
};
}

#endif
