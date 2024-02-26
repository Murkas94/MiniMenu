#include "FloatSliderEntry.h"
#include "../MiniMenu.h"

using namespace MiniMenu;

bool FloatSliderMenuEntry::CheckValueChange(MiniMenu::Input& input, float& value)
{
    if(IsEditMode())
    {
        if(input.enter.IsTriggered()){ ExitEditMode(); return true;}
        if(value > minValue && (input.left.IsTriggered() || (input.left.IsPressed() && input.left.MillisSinceLastChange() > 750 && millis() - lastContiniousChangeTime > 250)))
        {
            value -= 0.25f * diffValuePerSecond;
            if(value < minValue){value = minValue;}
            lastContiniousChangeTime = millis();
            return true;
        }
        if(value < maxValue && (input.right.IsTriggered() || (input.right.IsPressed() && input.right.MillisSinceLastChange() > 750 && millis() - lastContiniousChangeTime > 250)))
        {
            value += 0.25f * diffValuePerSecond;
            if(value > maxValue){value = maxValue;}
            lastContiniousChangeTime = millis();
            return true;
        }
    }else if(input.enter.IsTriggered())
    {
        EnterEditMode();
        return true;
    }
    return false;
}

float FloatSliderMenuEntry::GetDrawPercentage(const float& value)
{
    return (value - minValue) / (maxValue - minValue);
}

FloatSliderMenuEntry& FloatSliderMenuEntry::SetMinMax(const float& min, const float& max)
{
    minValue = min;
    maxValue = max;
    return *this;
}

FloatSliderMenuEntry& FloatSliderMenuEntry::SetChangeRate(const float& changePerSecond)
{
    diffValuePerSecond = changePerSecond;
    return *this;
}
