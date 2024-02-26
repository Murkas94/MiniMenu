#include "Input.h"

using namespace MiniMenu;

void Input::ButtonValue::UpdateContinuos(bool pressed)
{
    if(pressed)
    {
        switch(value)
        {
            case ButtonState_Up:
                if(millis() - lastButtonChange > 100)
                {
                    value = ButtonState_Push;
                    lastButtonChange = millis();
                }
                break;
            case ButtonState_Push:
                value = ButtonState_Down;
                break;
            case ButtonState_Down:
                break;
            case ButtonState_Release:
                value = ButtonState_Up;
                break;
        }
    }else{
        switch(value)
        {
            case ButtonState_Up:
                break;
            case ButtonState_Push:
                value = ButtonState_Down;
                break;
            case ButtonState_Down:
                if(millis() - lastButtonChange > 100)
                {
                    value = ButtonState_Release;
                    lastButtonChange = millis();
                }
                break;
            case ButtonState_Release:
                value = ButtonState_Up;
                break;
        }
    }
}