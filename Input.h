#ifndef MINIMENU_INPUT_H
#define MINIMENU_INPUT_H

#include "Arduino.h"

namespace MiniMenu
{
struct Input
{
    enum ButtonState
    {
        ButtonState_Up,     // Not pressed
        ButtonState_Push,   // Just pressed
        ButtonState_Down,   // Stay pressed
        ButtonState_Release // Press released
    };
    struct ButtonValue{
        uint32_t lastButtonChange = 0;
        uint32_t lastButtonDownTime = 0;
        uint32_t lastButtonPushedTime = 0;
        ButtonState value = ButtonState_Up;

        bool IsPressed() const { return value == ButtonState_Push || value == ButtonState_Down; }
        bool IsTriggered() const { return value == ButtonState_Push; }
        bool IsReleased() const { return value == ButtonState_Release; }
        uint32_t MillisSinceLastChange() const { return millis() - lastButtonChange; }
        const uint32_t& MillisOfLastPushed() const { return lastButtonPushedTime; }

        // Sets the current value (Up/Push/Down/Release)
        void Set(ButtonState value){
            if(this->value != value)
            {
                lastButtonChange = millis();
                if((value == ButtonState_Down || value == ButtonState_Push) && this->value != ButtonState_Down && this->value != ButtonState_Push)
                {
                    lastButtonDownTime = millis();
                }
                if((value == ButtonState_Up || value == ButtonState_Release) && this->value != ButtonState_Up && this->value != ButtonState_Release)
                {
                    lastButtonPushedTime = millis() - lastButtonDownTime;
                }
                this->value = value;
            }
        }
        // Sets a raw value from a button as input
        void UpdateContinuos(bool pressed);
        // Sets the already debounced value (1 "true" per button-press, no continuos input possible)
        void UpdateTriggered(bool pressed)
        {
            ButtonState newValue = pressed ? ButtonState_Push : ButtonState_Up;
            Set(value);
        }
    };

    ButtonValue up;
    ButtonValue down;
    ButtonValue left;
    ButtonValue right;
    ButtonValue enter;
};
}

#endif
