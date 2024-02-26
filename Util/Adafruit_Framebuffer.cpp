#include "Adafruit_Framebuffer.h"

FrameBuffer::FrameBuffer(Adafruit_SPITFT& _baseDevice)
    : Adafruit_GFX(_baseDevice.width(), _baseDevice.height())
    , baseDevice(&_baseDevice)
    , buffer(new uint16_t[_baseDevice.width()*_baseDevice.height()])
{
    memset(&buffer[0], 0, WIDTH*HEIGHT*2);
}

FrameBuffer::~FrameBuffer()
{
    if(buffer != nullptr)
    {
        delete[] buffer;
    }
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other)
{
    Adafruit_GFX::operator=(std::move(other));
    baseDevice = other.baseDevice;
    if(buffer != nullptr){delete[] buffer;}
    buffer = other.buffer;
    other.buffer = nullptr;
    return *this;
}

void FrameBuffer::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if(x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT)
    {
        buffer[x + y * WIDTH] = color;
    }
}

void FrameBuffer::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    if(y >= 0 && y < HEIGHT)
    {
        int16_t x_min = max<int16_t>(0, min(x, WIDTH));
        int16_t x_max = max<int16_t>(0, min<int16_t>(x + w, WIDTH));
        int32_t yOffset = static_cast<int32_t>(y) * WIDTH;
        int32_t i_start = yOffset + x_min;
        int32_t i_end = yOffset + x_max;
        for(int32_t i = i_start; i<i_end; ++i)
        {
            buffer[i] = color;
        }
    }
}

void FrameBuffer::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    int16_t x_min = max<int16_t>(0, min(x, WIDTH));
    int16_t x_max = max<int16_t>(0, min<int16_t>(x + w, WIDTH));
    int16_t y_min = max<int16_t>(0, min(y, HEIGHT));
    int16_t y_max = max<int16_t>(0, min<int16_t>(y + h, HEIGHT));
    int32_t yOffset = static_cast<int32_t>(y_min) * WIDTH;
    for(int16_t v = y_min; v < y_max; ++v, yOffset += WIDTH)
    {
        int32_t i_start = yOffset + x_min;
        int32_t i_end = yOffset + x_max;
        for(int32_t i = i_start; i<i_end; ++i)
        {
            buffer[i] = color;
        }
    }
}

void FrameBuffer::pushToDisplay()
{
    baseDevice->drawRGBBitmap(0,0, &buffer[0], WIDTH, HEIGHT);
}