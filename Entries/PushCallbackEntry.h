#ifndef PUSH_CALLBACK_MENU_ENTRY_H
#define PUSH_CALLBACK_MENU_ENTRY_H

#include <type_traits>
#include "../MenuEntry.h"
#include <functional>

namespace MiniMenu
{
class PushCallbackMenuEntry : public InheritedMenuEntry<PushCallbackMenuEntry>
{
private:
    std::function<void()> callback;

public:    
    PushCallbackMenuEntry(Menu& menu) : InheritedMenuEntry<PushCallbackMenuEntry>(menu) {}
    PushCallbackMenuEntry(Menu& menu, std::function<void()> target) : InheritedMenuEntry<PushCallbackMenuEntry>(menu), callback(target) {}
    PushCallbackMenuEntry(Menu& menu, void (*target)()) : InheritedMenuEntry<PushCallbackMenuEntry>(menu), callback(target) {}

    bool Handle(bool isSelected) override;
    void Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected) override;
    uint16_t GetRequiredHeight() override;
};
}

#endif
