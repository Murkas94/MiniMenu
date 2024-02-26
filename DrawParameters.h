#ifndef MINIMENU_DRAWPARAMETERS
#define MINIMENU_DRAWPARAMETERS

namespace MiniMenu
{
struct DrawParameters{
    uint16_t backgroundColor = 0;
    uint16_t frameColor = 0xFFFF;
    uint16_t selectedBackgroundColor = 0xAAAA;
    uint16_t textColor = 0xFFFF;
    uint16_t selectedTextColor = 0xFFFF;

    uint8_t framesize = 1;
    const uint8_t fontsize = 8;
};
}

#endif
