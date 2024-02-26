#ifndef SLIDER_MENU_ENTRY_BASE_H
#define SLIDER_MENU_ENTRY_BASE_H

#include "./TypedEntryBase.h"
#include <type_traits>

namespace MiniMenu
{

class SliderMenuEntryGraphics
{
public:
    void DrawSliderGraphics(MenuEntry& menuEntry, Menu& menu, Adafruit_GFX& gfx, const Rect& area, bool isSelected, float value);
    uint16_t GetSliderGraphicsRequiredHeight(MenuEntry& menuEntry, Menu& menu);

protected:
    virtual void DrawBase(Adafruit_GFX& gfx, const Rect& area, bool isSelected) = 0;
    virtual uint16_t GetBaseRequiredHeight() = 0;
};

template<typename T, typename DataType>
class SliderMenuEntry : public TypedMenuEntryBase<T, DataType>, public SliderMenuEntryGraphics
{
public:
    using FunctionValueContainer = typename TypedMenuEntryStaticBase<DataType>::FunctionValueContainer;
    using FunctionValueSetter = typename TypedMenuEntryStaticBase<DataType>::FunctionValueSetter;

    SliderMenuEntry(Menu& menu) : TypedMenuEntryBase<T, DataType>(menu) {}
    SliderMenuEntry(Menu& menu, DataType& target) : TypedMenuEntryBase<T, DataType>(menu, target) {}
    SliderMenuEntry(Menu& menu, const DataType& valueGetter, DataType& valueSetter) : TypedMenuEntryBase<T, DataType>(menu, valueGetter, valueSetter) { }
    SliderMenuEntry(Menu& menu, typename FunctionValueContainer::getterFunctionType getterFunction, typename FunctionValueSetter::setterFunctionType setterFunction) : TypedMenuEntryBase<T, DataType>(menu, getterFunction, setterFunction) { }

    virtual float GetDrawPercentage(const DataType& value) = 0;
    uint16_t GetRequiredHeight() override { return SliderMenuEntryGraphics::GetSliderGraphicsRequiredHeight(*this, MenuEntry::menu); }
    void Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected) override { SliderMenuEntryGraphics::DrawSliderGraphics(*this, MenuEntry::menu, gfx, area, isSelected, GetDrawPercentage(TypedMenuEntryStaticBase<DataType>::currentValue));}

protected:
    uint16_t GetBaseRequiredHeight(){ return MenuEntry::GetRequiredHeight(); }
    void DrawBase(Adafruit_GFX& gfx, const Rect& area, bool isSelected) override { MenuEntry::Draw(gfx, area, isSelected); }
};
}

#endif
