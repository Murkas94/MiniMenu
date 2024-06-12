#ifndef GENERIC_VALUE_MENU_ENTRY_H
#define GENERIC_VALUE_MENU_ENTRY_H

#include "../MenuEntry.h"
#include "Generics/ValueGetter.h"
#include "Generics/ValueFormatter.h"
#include <WString.h>
#include <type_traits>
#include <string>

namespace MiniMenu
{
class GenericValueMenuEntry : public InheritedMenuEntry<GenericValueMenuEntry>
{
public:
    using Formatter = Generics::Formatter;
    using DefaultFormatter = Generics::DefaultFormatter;
    using ValueGetter = Generics::ValueGetter;
    
    Generics::ValueGetter* getter = nullptr;
    Formatter* formatter = nullptr;

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
    
    GenericValueMenuEntry(Menu& menu) : InheritedMenuEntry(menu) {}
    template <typename T>
    GenericValueMenuEntry(Menu& menu, const T& target) : InheritedMenuEntry(menu) { SetDefaultGetter(target);}

    ~GenericValueMenuEntry(){ SetFormatter<Formatter>(nullptr); SetGetter<ValueGetter>(nullptr); }

    void Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected) override;
    uint16_t GetRequiredHeight() override;
};
}


#endif
