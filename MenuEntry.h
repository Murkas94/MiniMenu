#ifndef MINI_MENU_ENTRY_H
#define MINI_MENU_ENTRY_H

#include <Adafruit_GFX.h>
#include <cstdint>
#include "Geometry.h"
#include "DrawParameters.h"
#include <WString.h>

namespace MiniMenu
{
class Menu;

class MenuEntry
{
public:
    enum class HorizontalPosition
    {
        Left,
        Center,
        Right
    };

    String title;
    HorizontalPosition titlePosition = HorizontalPosition::Left;
    bool isSelectable = true;

protected:
    Menu& menu;

    static void DrawTextLeft(Adafruit_GFX& gfx, const Rect& area, uint16_t color, const char* text);
    static void DrawTextCenter(Adafruit_GFX& gfx, const Rect& area, uint16_t color, const char* text);
    static void DrawTextRight(Adafruit_GFX& gfx, const Rect& area, uint16_t color, const char* text);

    void DrawTextLeft(Adafruit_GFX& gfx, const Rect& area, bool isSelected, const char* text);
    void DrawTextCenter(Adafruit_GFX& gfx, const Rect& area, bool isSelected, const char* text);
    void DrawTextRight(Adafruit_GFX& gfx, const Rect& area, bool isSelected, const char* text);

    void EnterEditMode();
    void ExitEditMode();
    bool IsEditMode();

public:
    MenuEntry(Menu& menu):menu(menu){}
    virtual ~MenuEntry(){}

    MenuEntry& SetTitle(const String& title){ this->title = title; return *this; }
    MenuEntry& SetTitlePosition(HorizontalPosition pos){ this->titlePosition = pos; return *this; }
    MenuEntry& SetIsSelectable(bool selectable){ this->isSelectable = selectable; return *this; }

    // Handles the menu entry. Must return true, if the menu must be redrawn, otherwise false.
    virtual bool Handle(bool isSelected){return false;}
    virtual void Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected);

    //virtual bool GetIsSelectable() = 0;
    virtual uint16_t GetRequiredHeight();
};

template<typename T>
class InheritedMenuEntry : public MenuEntry
{
    T& GetDerived(){return static_cast<T&>(*this);}
public:
    InheritedMenuEntry(Menu& menu) : MenuEntry(menu){}
    T& SetTitle(const String& title){MenuEntry::SetTitle(title); return GetDerived(); }
    T& SetTitlePosition(HorizontalPosition pos){ this->titlePosition = pos; return GetDerived(); }
    T& SetIsSelectable(bool selectable){ this->isSelectable = selectable; return GetDerived(); }
};
}

#endif
