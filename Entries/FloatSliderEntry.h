#ifndef FLOAT_SLIDER_MENU_ENTRY_H
#define FLOAT_SLIDER_MENU_ENTRY_H

#include "./SliderEntryBase.h"

namespace MiniMenu
{
class FloatSliderMenuEntry : public SliderMenuEntry<FloatSliderMenuEntry, float>
{
private:
    unsigned long lastContiniousChangeTime = 0;
public:
    using FunctionValueContainer = typename TypedMenuEntryStaticBase<float>::FunctionValueContainer;
    using FunctionValueSetter = typename TypedMenuEntryStaticBase<float>::FunctionValueSetter;

    float minValue = 0;
    float maxValue = 1;
    float diffValuePerSecond = 0.2;

    FloatSliderMenuEntry(Menu& menu) : SliderMenuEntry(menu) {}
    FloatSliderMenuEntry(Menu& menu, float& target) : SliderMenuEntry<FloatSliderMenuEntry, float>(menu, target) {}
    FloatSliderMenuEntry(Menu& menu, const float& valueGetter, float& valueSetter) : SliderMenuEntry<FloatSliderMenuEntry, float>(menu, valueGetter, valueSetter) { }
    FloatSliderMenuEntry(Menu& menu, typename FunctionValueContainer::getterFunctionType getterFunction, typename FunctionValueSetter::setterFunctionType setterFunction) : SliderMenuEntry<FloatSliderMenuEntry, float>(menu, getterFunction, setterFunction) { }

    bool CheckValueChange(MiniMenu::Input& input, float& value) override;
    float GetDrawPercentage(const float& value) override;

    FloatSliderMenuEntry& SetMinMax(const float& min, const float& max);
    FloatSliderMenuEntry& SetChangeRate(const float& changePerSecond);
};
}

#endif
