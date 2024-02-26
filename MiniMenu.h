#ifndef MINI_MENU_H
#define MINI_MENU_H

#include <Adafruit_GFX.h>
#include "MenuTab.h"
#include <vector>
#include "Geometry.h"
#include "DrawParameters.h"
#include "Input.h"

namespace MiniMenu
{
class Menu
{
private:
    Adafruit_GFX& gfx;
    std::vector<MenuTab*> tabs;
    int16_t activeTabIndex = -1;

public:
    Rect drawArea;
    DrawParameters parameters;
    Input input;
    // Defines if the input is used for setting a value in an entry (true) or for menu-movement (false)
    bool valueInput = false;

    Menu(Adafruit_GFX& gfx);
    ~Menu();

    void Handle(bool forceDraw);
    void Draw();

    MenuTab& AddTab(MenuTab* tab)
    {
        tabs.push_back(tab);
        return *tab;
    }
    MenuTab& AddTab(){return AddTab(new MenuTab(*this));}
    MenuTab* GetTab(int16_t index){ if(index < tabs.size()){return tabs[index];} return nullptr;}
};
}

#endif
