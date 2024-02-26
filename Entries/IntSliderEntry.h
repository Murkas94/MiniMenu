#ifndef INT_SLIDER_MENU_ENTRY_H
#define INT_SLIDER_MENU_ENTRY_H

#include "./SliderEntryBase.h"
#include "../MiniMenu.h"

namespace MiniMenu
{

template<typename IntDataType>
class IntSliderMenuEntry : public SliderMenuEntry<IntSliderMenuEntry<IntDataType>, IntDataType>
{
private:
    unsigned long lastContiniousChangeTime = 0;
public:
    using FunctionValueContainer = typename TypedMenuEntryStaticBase<IntDataType>::FunctionValueContainer;
    using FunctionValueSetter = typename TypedMenuEntryStaticBase<IntDataType>::FunctionValueSetter;

    IntDataType minValue = 0;
    IntDataType maxValue = 100;
    int diffChangeTimeDiffMs = 50;

    IntSliderMenuEntry(Menu& menu) : SliderMenuEntry<IntSliderMenuEntry<IntDataType>, IntDataType>(menu) {}
    IntSliderMenuEntry(Menu& menu, IntDataType& target) : SliderMenuEntry<IntSliderMenuEntry<IntDataType>, IntDataType>(menu, target) {}
    IntSliderMenuEntry(Menu& menu, const IntDataType& valueGetter, IntDataType& valueSetter) : SliderMenuEntry<IntSliderMenuEntry<IntDataType>, IntDataType>(menu, valueGetter, valueSetter) { }
    IntSliderMenuEntry(Menu& menu, typename FunctionValueContainer::getterFunctionType getterFunction, typename FunctionValueSetter::setterFunctionType setterFunction) : SliderMenuEntry<IntSliderMenuEntry<IntDataType>, IntDataType>(menu, getterFunction, setterFunction) { }

    bool CheckValueChange(MiniMenu::Input& input, IntDataType& value) override;
    float GetDrawPercentage(const IntDataType& value) override;

    IntSliderMenuEntry<IntDataType>& SetMinMax(const IntDataType& min, const IntDataType& max);
    IntSliderMenuEntry<IntDataType>& SetChangeRate(const IntDataType& changePerSecond);
};

template<typename IntDataType>
bool IntSliderMenuEntry<IntDataType>::CheckValueChange(MiniMenu::Input& input, IntDataType& value)
{
    if(MenuEntry::IsEditMode())
    {
        if(input.enter.IsTriggered()){ MenuEntry::ExitEditMode(); return true;}
        if(value > minValue && (input.left.IsTriggered() || (input.left.IsPressed() && input.left.MillisSinceLastChange() > 750 && millis() - lastContiniousChangeTime > diffChangeTimeDiffMs)))
        {
            --value;
            lastContiniousChangeTime = millis();
            return true;
        }
        if(value < maxValue && (input.right.IsTriggered() || (input.right.IsPressed() && input.right.MillisSinceLastChange() > 750 && millis() - lastContiniousChangeTime > diffChangeTimeDiffMs)))
        {
            ++value;
            lastContiniousChangeTime = millis();
            return true;
        }
    }else if(input.enter.IsTriggered())
    {
        MenuEntry::EnterEditMode();
        return true;
    }
    return false;
}

template<typename IntDataType>
float IntSliderMenuEntry<IntDataType>::GetDrawPercentage(const IntDataType& value)
{
    return static_cast<float>(value - minValue) / static_cast<float>(maxValue - minValue);
}

template<typename IntDataType>
IntSliderMenuEntry<IntDataType>& IntSliderMenuEntry<IntDataType>::SetMinMax(const IntDataType& min, const IntDataType& max)
{
    minValue = min;
    maxValue = max;
    return *this;
}

template<typename IntDataType>
IntSliderMenuEntry<IntDataType>& IntSliderMenuEntry<IntDataType>::SetChangeRate(const IntDataType& changePerSecond)
{
    diffChangeTimeDiffMs = 1000 / changePerSecond;
    return *this;
}

}

#endif
