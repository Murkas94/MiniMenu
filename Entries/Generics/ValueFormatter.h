#ifndef GENERIC_VALUES_FORMATTERS_H
#define GENERIC_VALUES_FORMATTERS_H

#include "ValueTypes.h"
#include <WString.h>
#include <stdint.h>

namespace MiniMenu
{
namespace Generics
{

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

}
}

#endif
