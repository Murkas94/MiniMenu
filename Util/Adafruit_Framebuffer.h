#ifndef FrameBuffer_H
#define FrameBuffer_H

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>

class FrameBuffer : public Adafruit_GFX
{
private:
    Adafruit_SPITFT* baseDevice;
    uint16_t* buffer = nullptr;

public:
    FrameBuffer()
        : Adafruit_GFX(0,0)
        , baseDevice(nullptr)
    {}

    FrameBuffer(Adafruit_SPITFT& _baseDevice);

    ~FrameBuffer();

    FrameBuffer& operator=(const FrameBuffer& other) = delete;
    FrameBuffer& operator=(FrameBuffer&& other);

    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void pushToDisplay();
};

#endif
