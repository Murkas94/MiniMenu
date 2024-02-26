#include "GenericValueMenuEntry.h"
#include "../MiniMenu.h"

using namespace MiniMenu;

uint16_t GenericValueMenuEntry::GetRequiredHeight()
{
    return MenuEntry::GetRequiredHeight() + menu.parameters.fontsize + 2;
}

void GenericValueMenuEntry::Draw(Adafruit_GFX& gfx, const Rect& area, bool isSelected)
{
    MenuEntry::Draw(gfx, area, isSelected);
    if(getter == nullptr){return;}
    if(formatter == nullptr) { SetDefaultFormatter();}
    ValueContainer* data = getter->GetValue();
    if(data == nullptr){return;}
    String text = formatter->Format(data->GetData(), getter->GetValueType());
    if(getter->ReturnsNewInstances()){ delete data; }
    Rect valueArea = area;
    valueArea.size.x -= 10;
    valueArea.pos.x += 10;
    uint16_t baseHeight = MenuEntry::GetRequiredHeight();
    valueArea.pos.y += baseHeight;
    valueArea.size.y -= baseHeight;
    DrawTextRight(gfx, valueArea, isSelected, text.c_str());
}

String GenericValueMenuEntry::DefaultFormatter::Format(const void* data, ValueType valueType)
{
    String text = Prefix;
    switch (valueType)
    {
    case ValueType::boolean:
        text += (*(const bool*)data) ? "Yes" : "No";
        break;
    case ValueType::cstring:
        text += String((const char*)data);
        break;
    case ValueType::stdstring:
        text += String(((std::string*)data)->c_str());
        break;
    case ValueType::wstring:
        text += *((String*)data);
        break;
    case ValueType::character:
        text += *(const char*)data;
        break;
    case ValueType::int64:
        text += *(const int64_t*)data;
        break;
    case ValueType::int32:
        text += *(const int32_t*)data;
        break;
    case ValueType::int16:
        text += *(const int16_t*)data;
        break;
    case ValueType::int8:
        text += *(const int8_t*)data;
        break;
    case ValueType::uint64:
        text += *(const uint64_t*)data;
        break;
    case ValueType::uint32:
        text += *(const uint32_t*)data;
        break;
    case ValueType::uint16:
        text += *(const uint16_t*)data;
        break;
    case ValueType::uint8:
        text += *(const uint8_t*)data;
        break;
    case ValueType::vt_float:
        text += *(const float*)data;
        break;
    case ValueType::vt_double:
        text += *(const double*)data;
        break;
    default:
        break;
    }
    text += Postfix;
    return text;
}

String GenericValueMenuEntry::PercentFormatter::Format(const void* data, ValueType valueType)
{
    switch (valueType)
    {
    case ValueType::int64:
        return String(*(const int64_t*)data) + "%";
    case ValueType::int32:
        return String(*(const int32_t*)data) + "%";
    case ValueType::int16:
        return String(*(const int16_t*)data) + "%";
    case ValueType::int8:
        return String(*(const int8_t*)data) + "%";
    case ValueType::uint64:
        return String(*(const uint64_t*)data) + "%";
    case ValueType::uint32:
        return String(*(const uint32_t*)data) + "%";
    case ValueType::uint16:
        return String(*(const uint16_t*)data) + "%";
    case ValueType::uint8:
        return String(*(const uint8_t*)data) + "%";
    case ValueType::vt_float:
        return String(100*(*(const float*)data)) + "%";
    case ValueType::vt_double:
        return String(100*(*(const double*)data)) + "%";
    case ValueType::boolean:
    case ValueType::cstring:
    case ValueType::character:
    default:
        return "-%";
    }
}

String GenericValueMenuEntry::TimeFromSecondsFormatter::Format(const void* data, ValueType valueType)
{
    switch (valueType)
    {
    case ValueType::int64:
        return GetTimeString(*(const int64_t*)data);
    case ValueType::int32:
        return GetTimeString(*(const int32_t*)data);
    case ValueType::int16:
        return GetTimeString(*(const int16_t*)data);
    case ValueType::int8:
        return GetTimeString(*(const int8_t*)data);
    case ValueType::uint64:
        return GetTimeString(*(const uint64_t*)data);
    case ValueType::uint32:
        return GetTimeString(*(const uint32_t*)data);
    case ValueType::uint16:
        return GetTimeString(*(const uint16_t*)data);
    case ValueType::uint8:
        return GetTimeString(*(const uint8_t*)data);
    case ValueType::vt_float:
        return GetTimeString((*(const float*)data) + 0.5f);
    case ValueType::vt_double:
        return GetTimeString((*(const double*)data) + 0.5f);
    case ValueType::boolean:
    case ValueType::cstring:
    case ValueType::character:
    default:
        return "-";
    }
}

String GenericValueMenuEntry::TimeFromMillisecondsFormatter::Format(const void* data, ValueType valueType)
{
    switch (valueType)
    {
    case ValueType::int64:
        return GetTimeString((*(const int64_t*)data) / 1000);
    case ValueType::int32:
        return GetTimeString((*(const int32_t*)data) / 1000);
    case ValueType::uint64:
        return GetTimeString((*(const uint64_t*)data) / 1000);
    case ValueType::uint32:
        return GetTimeString((*(const uint32_t*)data) / 1000);
    case ValueType::vt_float:
        return GetTimeString(((*(const float*)data) * 0.001f) + 0.5f);
    case ValueType::vt_double:
        return GetTimeString(((*(const double*)data) * 0.001) + 0.5);
    case ValueType::int16:
    case ValueType::int8:
    case ValueType::uint16:
    case ValueType::uint8:
    case ValueType::boolean:
    case ValueType::cstring:
    case ValueType::character:
    default:
        return "-";
    }
}

String GenericValueMenuEntry::TimeFromSecondsFormatter::GetTimeString(int64_t seconds)
{
    String result;
    if(seconds < 0){seconds = - seconds; result = "-"; }
    uint16_t days = seconds / 86400;
    uint8_t hours = (seconds % 86400) / 3600;
    uint8_t minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;
    bool show = days > 0;
    if(show){ result.concat(days); result.concat('d'); }
    bool higherUnit = show;
    show |= hours > 0;
    if(show){ if(higherUnit && hours < 10){ result.concat('0');} result.concat(hours); result.concat('h'); }
    higherUnit = show;
    show |= minutes > 0;
    if(show){ if(higherUnit && minutes < 10){ result.concat('0');} result.concat(minutes); result.concat('m'); }
    higherUnit = show;
    if(higherUnit && seconds < 10){ result.concat('0');}
    result.concat(seconds); result.concat('s');
    return result;
}
