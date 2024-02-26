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
        ButtonState value = ButtonState_Up;

        bool IsPressed() const {return value == ButtonState_Push || value == ButtonState_Down;}
        bool IsTriggered() const {return value == ButtonState_Push;}
        uint32_t MillisSinceLastChange(){ return millis() - lastButtonChange; }

        // Sets the current value (Up/Push/Down/Release)
        void Set(ButtonState value){
            if(this->value != value)
            {
                lastButtonChange = millis();
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
