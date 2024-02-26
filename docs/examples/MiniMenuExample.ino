#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "src/MiniMenu//Util/Adafruit_Framebuffer.h"
#include "src/MiniMenu/MiniMenu.h"

// Set these up like your hardware needs it
#define PORT_DC 1
#define PORT_INPUT_UP 10
#define PORT_INPUT_DOWN 11
#define PORT_INPUT_LEFT 12
#define PORT_INPUT_RIGHT 13
#define PORT_INPUT_ENTER 14

void buildMenu();
void initDisplay();
Adafruit_ST7735 tft(-1, PORT_DC, -1);
FrameBuffer gfx;
MiniMenu::Menu* menu;

uint8_t myUint8Var;
uint32_t myUint32Var;
float myFloatVar = 0.5f;
float myVoltageVar = 5.8f; // Means 5.8V
float myBattPercVar = 0.8f; // Means 80%
void myCallback(){Serial.println("Callback!");}
bool myBool;

enum class MyEnum
{
    ABC = 1,
    DEF = 2,
    GHI = 3
} myEnum;

void setup()
{
    initDisplay();
    buildMenu();
}

void loop()
{
    // Fill with black
    gfx.fillScreen(ST77XX_BLACK);

    // Update menu input
    menu->input.up.UpdateContinuos(digitalRead(PORT_INPUT_UP) == 0);
    menu->input.down.UpdateContinuos(digitalRead(PORT_INPUT_DOWN) == 0);
    menu->input.left.UpdateContinuos(digitalRead(PORT_INPUT_LEFT) == 0);
    menu->input.right.UpdateContinuos(digitalRead(PORT_INPUT_RIGHT) == 0);
    menu->input.enter.UpdateContinuos(digitalRead(PORT_INPUT_ENTER) == 0);

    // Handle menu
    menu->Handle(true);

    // draw on real display
    gfx.pushToDisplay();
}

void initDisplay()
{
    // Setup display. This highly depends on your local hardware.
    tft.initR(INITR_MINI160x80_PLUGIN);
    tft.setRotation(ST7735_MADCTL_BGR);
    tft.invertDisplay(false);

    delay(500);

    // Create framebuffer, fill with black and push to display
    gfx = FrameBuffer(tft);
    gfx.fillScreen(ST77XX_BLACK);
    gfx.pushToDisplay();
}

void buildMenu()
{
    // Create the menu-instance, which will draw on the framebuffer
    menu = new MiniMenu::Menu(gfx);

    // define area, where should be drawn
    menu->drawArea = MiniMenu::Rect(0, 0, 80, 160);

    // define colors (orange)
    menu->parameters.selectedBackgroundColor = tft.color565(250,156,30);
    menu->parameters.frameColor = tft.color565(250,156,30);

    // Add a tab
    auto& tab1 = menu->AddTab();
    // Add a title to the title
    tab1.SetFixedEntryCount(1);
    tab1.AddEntry(new MiniMenu::MenuEntry(*menu)).SetTitle("Tab 1").SetTitlePosition(MiniMenu::MenuEntry::HorizontalPosition::Center).SetIsSelectable(false);
    // Add a text-entry, which will show voltage
    tab1.AddEntry(new MiniMenu::GenericValueMenuEntry(*menu, myVoltageVar)).SetTitle("Voltage").SetDefaultFormatter().SetPostfix("V");
    // Add a text-entry, which will show battery percentage
    tab1.AddEntry(new MiniMenu::GenericValueMenuEntry(*menu, myBattPercVar)).SetTitle("Battery").SetFormatter(new MiniMenu::GenericValueMenuEntry::PercentFormatter());
    // Add a enum-selectable entry
    auto& enumEntry = tab1.AddEntry(new MiniMenu::EnumSelectorMenuEntry<MyEnum>(*menu, myEnum));
    enumEntry.SetTitle("Enum Entry");
    enumEntry.AddEntry(MyEnum::ABC, "ABC!");
    enumEntry.AddEntry(MyEnum::DEF, "DEF?");
    enumEntry.AddEntry(MyEnum::GHI, "GHI.");

    // Add another tab
    auto& tab2 = menu->AddTab();
    tab2.SetFixedEntryCount(1);
    tab2.AddEntry(new MiniMenu::MenuEntry(*menu)).SetTitle("Tab 2").SetTitlePosition(MiniMenu::MenuEntry::HorizontalPosition::Center).SetIsSelectable(false);
    // Add a float-slider by referencing a simple variable
    tab2.AddEntry(new MiniMenu::FloatSliderMenuEntry(*menu, myFloatVar)).SetMinMax(0.1f, 2.f).SetChangeRate(0.8f).SetTitle("My Float");
    // Add a uint8-slider with callbacks for reading/writing
    tab2.AddEntry(new MiniMenu::IntSliderMenuEntry<uint8_t>(*menu,[]()->uint8_t{return myUint8Var;}, [](const uint8_t& value){myUint8Var = value;})).SetMinMax(0, 249).SetTitle("My UInt8").SetChangeRate(30);
    // Add a uint32-slider by referencing a simple variable
    tab2.AddEntry(new MiniMenu::IntSliderMenuEntry<uint32_t>(*menu, myUint32Var)).SetMinMax(100, 1000).SetTitle("My UInt32").SetChangeRate(100);
    // Add a simple checkbox
    tab2.AddEntry(new MiniMenu::BoolCheckboxMenuEntry(*menu, myBool)).SetTitle("Boolean");
    // Add a simple callback entry
    tab2.AddEntry(new MiniMenu::PushCallbackMenuEntry(*menu, [](){myCallback();})).SetTitle("Do callback!");
}
