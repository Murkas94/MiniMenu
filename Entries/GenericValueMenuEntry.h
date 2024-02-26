#ifndef GENERIC_VALUE_MENU_ENTRY_H
#define GENERIC_VALUE_MENU_ENTRY_H

#include "../MenuEntry.h"
#include <WString.h>
#include <type_traits>
#include <string>

namespace MiniMenu
{
class GenericValueMenuEntry : public InheritedMenuEntry<GenericValueMenuEntry>
{
public:
    enum class ValueType
    {
        boolean,
        cstring,
        stdstring,
        wstring,
        character,
        uint8,
        uint16,
        uint32,
        uint64,
        int8,
        int16,
        int32,
        int64,
        vt_float,
        vt_double
    };

    class Formatter
    {
    public:
        virtual ~Formatter(){}
        virtual String Format(const void* data, ValueType valueType) = 0;
    };

    class DefaultFormatter : public Formatter
    {
    public:
        String Format(const void* data, ValueType valueType) override;

        String Postfix;
        String Prefix;

        inline DefaultFormatter& SetPostfix(const String& postfix) { Postfix = postfix; return *this;}
        inline DefaultFormatter& SetPrefix(const String& prefix) { Prefix = prefix; return *this;}
    };

    class ValueContainer
    {
    public:
        virtual ~ValueContainer(){}
        virtual const void* GetData() = 0;
    };

    class StaticReferenceValueContainer : public ValueContainer
    {
        const void* data;
    public:
        StaticReferenceValueContainer(const void* data) : data(data){}
        const void* GetData() override { return data; }
    };

    class ValueGetter
    {
    public:
        virtual ~ValueGetter(){}
        virtual ValueContainer* GetValue() = 0;
        virtual ValueType GetValueType() = 0;
        virtual bool ReturnsNewInstances() = 0;
    };

    class StaticReferenceValueGetter : public ValueGetter
    {
        const ValueType valueType;
        StaticReferenceValueContainer data;
    public:
        ValueContainer* GetValue() override
        {
            return &data;
        }
        ValueType GetValueType() override { return valueType; }
        bool ReturnsNewInstances() override {return false;}

        StaticReferenceValueGetter(const bool& target) : valueType(ValueType::boolean), data(&target){}
        StaticReferenceValueGetter(const char& target) : valueType(ValueType::character), data(&target){}
        StaticReferenceValueGetter(const char* target) : valueType(ValueType::cstring), data(target){}
        StaticReferenceValueGetter(const std::string& target) : valueType(ValueType::stdstring), data(&target){}
        StaticReferenceValueGetter(const String& target) : valueType(ValueType::wstring), data(&target){}
        StaticReferenceValueGetter(const uint8_t& target) : valueType(ValueType::uint8), data(&target){}
        StaticReferenceValueGetter(const uint16_t& target) : valueType(ValueType::uint16), data(&target){}
        StaticReferenceValueGetter(const uint32_t& target) : valueType(ValueType::uint32), data(&target){}
        StaticReferenceValueGetter(const uint64_t& target) : valueType(ValueType::uint64), data(&target){}
        StaticReferenceValueGetter(const int8_t& target) : valueType(ValueType::int8), data(&target){}
        StaticReferenceValueGetter(const int16_t& target) : valueType(ValueType::int16), data(&target){}
        StaticReferenceValueGetter(const int32_t& target) : valueType(ValueType::int32), data(&target){}
        StaticReferenceValueGetter(const int64_t& target) : valueType(ValueType::int64), data(&target){}
        StaticReferenceValueGetter(const float& target) : valueType(ValueType::vt_float), data(&target){}
        StaticReferenceValueGetter(const double& target) : valueType(ValueType::vt_double), data(&target){}
    };


    ValueGetter* getter = nullptr;
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
    StaticReferenceValueGetter& SetDefaultGetter(const T& target){
        return SetGetter(new StaticReferenceValueGetter(target));
    }
    
    GenericValueMenuEntry(Menu& menu) : InheritedMenuEntry(menu) {}
    template <typename T>
    GenericValueMenuEntry(Menu& menu, const T& target) : InheritedMenuEntry(menu) { SetDefaultGetter(target);}

    ~GenericValueMenuEntry(){ SetFormatter<Formatter>(nullptr); SetGetter<ValueGetter>(nullptr); }

    void Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected) override;
    uint16_t GetRequiredHeight() override;

    class PercentFormatter : public Formatter
    {
    public:
        String Format(const void* data, ValueType valueType) override;
    };

    class TimeFromSecondsFormatter : public Formatter
    {        
    public:
        static String GetTimeString(int64_t seconds);
        String Format(const void* data, ValueType valueType) override;
    };
    class TimeFromMillisecondsFormatter : public TimeFromSecondsFormatter
    {
    public:
        String Format(const void* data, ValueType valueType) override;
    };

    template <typename T>
    class FunctionValueContainer : public ValueContainer
    {
        T valueBuffer;
    public:
        typedef T (*getterFunctionType)();
        getterFunctionType getterFunction;
        FunctionValueContainer(getterFunctionType function) : getterFunction(function) {}
        const void* GetData() override { valueBuffer = getterFunction(); return &valueBuffer; }
    };

    template <ValueType valueType, typename variableType>
    class FunctionValueGetter : public ValueGetter
    {
        FunctionValueContainer<variableType> data;
    public:
        FunctionValueGetter(typename FunctionValueContainer<variableType>::getterFunctionType function)
            : data(function){}
        ValueContainer* GetValue() override { return &data; }
        ValueType GetValueType() override {return valueType;}
        bool ReturnsNewInstances() override {return false;}
    };
};
}

// Specialization for c-string, since it already returns a pointer
template <>
class MiniMenu::GenericValueMenuEntry::FunctionValueContainer<const char*> : public ValueContainer
{
    const char* valueBuffer;
public:
    typedef const char* (*getterFunctionType)();
    getterFunctionType getterFunction;
    FunctionValueContainer(getterFunctionType function) : getterFunction(function) {}
    const void* GetData() override { valueBuffer = getterFunction(); return valueBuffer; }
};

#endif
