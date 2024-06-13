#ifndef MULTI_GENERIC_VALUE_MENU_ENTRY_H
#define MULTI_GENERIC_VALUE_MENU_ENTRY_H

#include "../MenuEntry.h"
#include "Generics/ValueGetter.h"
#include "Generics/ValueFormatter.h"
#include <array>

namespace MiniMenu
{

class MultiGenericValueMenuEntryBase
{
public:
    struct ValueField
    {
        using Formatter = Generics::Formatter;
        using DefaultFormatter = Generics::DefaultFormatter;
        using ValueGetter = Generics::ValueGetter;
        
        Generics::ValueGetter* getter = nullptr;
        Formatter* formatter = nullptr;
        std::string caption;

        template<typename SpecificFormatter, typename std::enable_if<std::is_base_of<Formatter, SpecificFormatter>::value, bool>::type = true>
        SpecificFormatter& SetFormatter(SpecificFormatter* newFormatter)
        {
            if(formatter != nullptr){delete formatter;}
            formatter = newFormatter;
            return *newFormatter;
        }
        DefaultFormatter& SetDefaultFormatter(){
            return SetFormatter(new DefaultFormatter());
        }

        template<typename SpecificGetter, typename std::enable_if<std::is_base_of<ValueGetter, SpecificGetter>::value, bool>::type = true>
        SpecificGetter& SetGetter(SpecificGetter* newGetter)
        {
            if(getter != nullptr){delete getter;}
            getter = newGetter;
            return *newGetter;
        }
        template <typename T>
        Generics::StaticReferenceValueGetter& SetDefaultGetter(const T& target){
            return SetGetter(new Generics::StaticReferenceValueGetter(target));
        }

        void Draw(Menu& menu, Adafruit_GFX& gfx, const Rect& area, bool isSelected);

        ValueField(){}
        template <typename T>
        ValueField(const T& target){ SetDefaultGetter(target);}

        ~ValueField(){ SetFormatter<Formatter>(nullptr); SetGetter<ValueGetter>(nullptr); }
    };
    //static void DrawGraphics(MenuEntry& menuEntry, Menu& menu, Adafruit_GFX& gfx, const Rect& area, bool isSelected, float value);
    //static uint16_t GetGraphicsRequiredHeight(MenuEntry& menuEntry, Menu& menu);
};

template <size_t size_x, size_t size_y>
class MultiGenericValueMenuEntry : public InheritedMenuEntry<MultiGenericValueMenuEntry<size_x, size_y>>, private MultiGenericValueMenuEntryBase
{
public:
    
    MultiGenericValueMenuEntry(Menu& menu) : InheritedMenuEntry<MultiGenericValueMenuEntry<size_x, size_y>>(menu) {}
    ~MultiGenericValueMenuEntry(){}

    std::array<std::array<ValueField, size_y>, size_x> fields;

    Vec2 spacing = Vec2(1, 1);

    void Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected) override;
    uint16_t GetRequiredHeight() override;
};

template <size_t size_x, size_t size_y>
void MultiGenericValueMenuEntry<size_x,size_y>::Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected)
{
    MenuEntry::Draw(gfx, area, isSelected);
    const uint16_t baseHeight = MenuEntry::GetRequiredHeight();
    const Vec2 fieldSize = Vec2((area.size.x - ((size_x - 1) * spacing.x) ) / size_x, this->menu.parameters.fontsize);
    for(size_t x = 0; x < size_x; ++x)
    {
        for(size_t y = 0; y < size_y; ++y)
        {
            Vec2 fieldPos = Vec2(x * (fieldSize.x + spacing.x), baseHeight + y * (fieldSize.y + spacing.y)) + area.pos;
            fields[x][y].Draw(this->menu, gfx, Rect(fieldPos, fieldSize), isSelected);
        }
    }
}

template <size_t size_x, size_t size_y>
uint16_t MultiGenericValueMenuEntry<size_x,size_y>::GetRequiredHeight()
{
    return MenuEntry::GetRequiredHeight() + 2 + size_y * (this->menu.parameters.fontsize + spacing.y);
}

}


#endif
