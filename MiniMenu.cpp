#include "MiniMenu.h"

using namespace MiniMenu;

Menu::Menu(Adafruit_GFX& _gfx)
    : gfx(_gfx)
{

}

Menu::~Menu()
{
    for(auto it = tabs.begin(); it != tabs.end(); ++it)
    {
        delete *it;
    }
    tabs.clear();
}

void Menu::Handle(bool forceDraw)
{
    if(tabs.empty()){return;}
    if(activeTabIndex == -1)
    {
        activeTabIndex = 0;
    }
    if(!valueInput)
    {
        if(activeTabIndex > 0 && input.left.IsTriggered()){ --activeTabIndex; }
        if(activeTabIndex < tabs.size() - 1 && input.right.IsTriggered()){ ++activeTabIndex; }
    }
    if(tabs[activeTabIndex]->Handle(drawArea) || forceDraw)
    {
        Draw();
    }
}

void Menu::Draw()
{
    gfx.fillRect(drawArea.pos.x, drawArea.pos.y, drawArea.size.x, drawArea.size.y, 0);
    if(activeTabIndex == -1){return;}
    tabs[activeTabIndex]->Draw(gfx, drawArea);
}
