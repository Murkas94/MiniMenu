#include "MenuTab.h"
#include "MiniMenu.h"

#include "HardwareSerial.h"

using namespace MiniMenu;

MenuTab::MenuTab(Menu& _menu)
    : menu(_menu)
{

}

MenuTab::~MenuTab()
{
    for(auto it = entries.begin(); it != entries.end(); ++it)
    {
        delete *it;
    }
    entries.clear();
}

bool MenuTab::Handle(const Rect& area)
{
    if(entries.empty()){return false;}
    if(!menu.valueInput && selectedEntryIndex != -1)
    {
        int16_t lastValidSelectedEntry = selectedEntryIndex;
        if(menu.input.up.IsTriggered() || (menu.input.up.IsPressed() && menu.input.up.MillisSinceLastChange() > 750 && millis() - lastUpDownChangeTimestamp > 350))
        {
            lastUpDownChangeTimestamp = millis();
            do
            {
                --selectedEntryIndex;
                if(selectedEntryIndex < 0)
                {
                    selectedEntryIndex = lastValidSelectedEntry;
                    break;
                }
            }while(entries[selectedEntryIndex]->isSelectable == false);
        }
        if(menu.input.down.IsTriggered() || (menu.input.down.IsPressed() && menu.input.down.MillisSinceLastChange() > 750 && millis() - lastUpDownChangeTimestamp > 350)){
            lastUpDownChangeTimestamp = millis();
            selectedEntryIndex = getNextSelectableEntry(selectedEntryIndex);
        }
    }
    bool redraw = VisibilityCheck(area);
    for(int16_t i = 0; i < entries.size(); ++i)
    {
        if(entries[i]->Handle(i == selectedEntryIndex)){ redraw = true; }
    }
    return redraw;
}

int16_t MenuTab::getNextSelectableEntry(int16_t pos)
{
    int16_t lastValidSelectedEntry = pos;
    do
    {
        ++pos;
        if(pos >= entries.size())
        {
            return lastValidSelectedEntry;
        }
    } while (entries[pos]->isSelectable == false);
    return pos;
}

int MenuTab::GetSelectedItemPosition()
{
    if(selectedEntryIndex < fixedEntryCount) { return 0; }
    if(selectedEntryIndex < firstVisibleEntryIndex){return -1;}
    int16_t lastVisibleEntryIndex = firstVisibleEntryIndex + visibleEntryCount - 1;
    if(selectedEntryIndex <= lastVisibleEntryIndex) { return 0;}
    return 1;
}

void MenuTab::CalculateVisibleEntryCount(uint16_t height)
{
    if(height < menu.parameters.framesize){ visibleEntryCount = 0; return; }
    height -= menu.parameters.framesize;
    for(EntryVector::iterator it = entries.begin(), itend = min<EntryVector::iterator>(entries.begin() + fixedEntryCount, entries.end()); it != itend; ++it)
    {
        const uint16_t requiredHeight = (*it)->GetRequiredHeight();
        if(height < requiredHeight){ visibleEntryCount = 0; return; }
        height -= requiredHeight;   // pixels for content
        if(height < menu.parameters.framesize){ visibleEntryCount = 0; return; }
        height -= menu.parameters.framesize;
    }
    uint16_t count = 0;
    for(auto it = min<EntryVector::iterator>(entries.begin() + firstVisibleEntryIndex, entries.end()); it != entries.end(); ++it)
    {
        const uint16_t requiredHeight = (*it)->GetRequiredHeight();
        if(height < requiredHeight){ break; }
        height -= requiredHeight;   // pixels for content
        if(height < menu.parameters.framesize){ break; }
        height -= menu.parameters.framesize;
        ++count;
    }
    visibleEntryCount = count;
}

bool MenuTab::VisibilityCheck(const Rect& area)
{
    if(entries.empty()){return false;}
    bool valueChanged = false;
    if(selectedEntryIndex == -1){
        selectedEntryIndex = getNextSelectableEntry(-1);
        if(selectedEntryIndex == -1){ return false; }
        valueChanged = true;
    }
    if(firstVisibleEntryIndex == -1){firstVisibleEntryIndex = fixedEntryCount; valueChanged = true;}
    if(visibleEntryCount == -1){CalculateVisibleEntryCount(area.size.y); valueChanged = true;}
    int selectedDir = GetSelectedItemPosition();
    if(selectedDir < 0)
    {
        // selected element is above. set selected element as first visible
        firstVisibleEntryIndex = selectedEntryIndex;
        CalculateVisibleEntryCount(area.size.y);
        return true;
    }else if(selectedDir > 0)
    {
        while(selectedDir > 0)
        {
            // iterate visible items down until selected item is visible
            ++firstVisibleEntryIndex;
            CalculateVisibleEntryCount(area.size.y);
            selectedDir = GetSelectedItemPosition();
        }
        return true;
    }
    return valueChanged;
}

void MenuTab::Draw(Adafruit_GFX& gfx, const Rect& area)
{
    uint16_t y = area.pos.y;
    for(uint8_t i = 0;i<menu.parameters.framesize; ++i)
        gfx.drawFastHLine(area.pos.x, y++, area.size.x, menu.parameters.frameColor);
    for(uint16_t i = 0; i< fixedEntryCount; ++i)
    {
        int16_t entryIndex = i;
        auto& entry = *entries[entryIndex];

        const uint16_t requiredHeight = entry.GetRequiredHeight();
        const Rect entryArea (area.pos.x, y, area.size.x, requiredHeight);
        entry.Draw(gfx, entryArea, entryIndex == selectedEntryIndex);
        y += requiredHeight;
        for(uint8_t i = 0;i<menu.parameters.framesize; ++i)
            gfx.drawFastHLine(area.pos.x, y++, area.size.x, menu.parameters.frameColor);
    }
    for(uint16_t i = 0; i< visibleEntryCount; ++i)
    {
        int16_t entryIndex = i + firstVisibleEntryIndex;
        auto& entry = *entries[entryIndex];

        const uint16_t requiredHeight = entry.GetRequiredHeight();
        const Rect entryArea (area.pos.x, y, area.size.x, requiredHeight);
        entry.Draw(gfx, entryArea, entryIndex == selectedEntryIndex);
        y += requiredHeight;
        for(uint8_t i = 0;i<menu.parameters.framesize; ++i)
            gfx.drawFastHLine(area.pos.x, y++, area.size.x, menu.parameters.frameColor);
    }
}
