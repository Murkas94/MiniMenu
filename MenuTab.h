#ifndef MINI_MENU_TAB_H
#define MINI_MENU_TAB_H

#include <Adafruit_GFX.h>
#include "MenuEntry.h"
#include <vector>
#include <type_traits>

namespace MiniMenu
{
class MenuTab
{
protected:
    Menu& menu;
private:
    using EntryVector = std::vector<MenuEntry*>;
    EntryVector entries;
    int16_t selectedEntryIndex = -1;
    int16_t firstVisibleEntryIndex = -1;
    int16_t visibleEntryCount = -1;
    uint32_t lastUpDownChangeTimestamp = 0;
    uint8_t fixedEntryCount = 0;

    bool VisibilityCheck(const Rect& area);
    int GetSelectedItemPosition();
    void CalculateVisibleEntryCount(uint16_t height);

    int16_t getNextSelectableEntry(int16_t pos);

public:
    MenuTab(Menu& menu);
    ~MenuTab();

    bool Handle(const Rect& area);
    void Draw(Adafruit_GFX& gfx, const Rect& area);

    inline const int16_t GetSelectedEntryIndex(){ return selectedEntryIndex; }
    inline void SetSelectedEntryIndex(int16_t index){ selectedEntryIndex = index; }
    inline MenuTab& SetFixedEntryCount(uint8_t count){ fixedEntryCount = count; return *this; }

    template<typename SpecificMenuEntry, typename std::enable_if<std::is_base_of<MenuEntry, SpecificMenuEntry>::value, bool>::type = true>
    SpecificMenuEntry& AddEntry(SpecificMenuEntry* entry)
    {
        entries.push_back(entry);
        return *entry;
    }
    inline MenuEntry* GetEntry(int16_t index){ if(index < entries.size()){return entries[index];} return nullptr;}

    void RemoveEntry(int16_t index);
    void ClearEntries();
};
}

#endif
