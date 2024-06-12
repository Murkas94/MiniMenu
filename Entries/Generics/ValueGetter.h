#ifndef GENERIC_VALUES_H
#define GENERIC_VALUES_H

#include "ValueTypes.h"
#include <WString.h>
#include <string>
#include <stdint.h>

namespace MiniMenu
{
namespace Generics
{

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


// Specialization for c-string, since it already returns a pointer
template <>
class FunctionValueContainer<const char*> : public ValueContainer
{
    const char* valueBuffer;
public:
    typedef const char* (*getterFunctionType)();
    getterFunctionType getterFunction;
    FunctionValueContainer(getterFunctionType function) : getterFunction(function) {}
    const void* GetData() override { valueBuffer = getterFunction(); return valueBuffer; }
};

}
}

#endif
