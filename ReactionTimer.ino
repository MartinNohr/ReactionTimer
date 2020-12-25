/*
 Name:		ReactionTimer.ino
 Created:	12/24/2020 8:45:46 PM
 Author:	Martin
*/
/*
    Martin Nohr 10/10/2020
*/
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "font.h"

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_GREY 0x5AEB
#define TFT_ENABLE 4

//#include <Fonts/GFXFF/gfxfont.h>

void setup()
{
    pinMode(TFT_ENABLE, OUTPUT);
    digitalWrite(TFT_ENABLE, 1);
    Serial.begin(115200);
    while (!Serial)
        continue;
    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set the font colour AND the background colour
    tft.setCursor(0, 0);
    tft.setFreeFont(&Dialog_bold_16);
    tft.setRotation(1);
	tft.setTextWrap(false, false);
    tft.setCursor(0, tft.fontHeight());
    tft.println("");
    tft.println("  Press and hold button");
    tft.println("  Release when circle");
    tft.println("      turns green");
    delay(1000);
}

void loop()
{
    static int state = 0;
    static bool lastBtn0 = true;
    bool btn0 = digitalRead(0);
	static unsigned long start = 0;
    static int randomtime = 0;
    switch (state) {
    case 0: // wait for btn0
        if (btn0 == 0) {
            state = 1;
        }
        break;
    case 1: // draw the red button
        tft.fillScreen(TFT_BLACK);
        tft.fillCircle(100, 50, 20, TFT_RED);
        state = 2;
        start = millis();
        randomtime = random(1000, 5000);
        break;
    case 2: // wait for random time
        if (millis() > start + randomtime) {
            tft.fillCircle(100, 50, 20, TFT_GREEN);
            start = millis();
            state = 3;
        }
        else {
            if (btn0) {
                state = 4;
            }
        }
        break;
    case 3: // wait for button release
        if (btn0) {
            unsigned long final = millis();
            tft.fillScreen(TFT_BLACK);
            state = 0;
            //tft.setRotation(0);
			tft.setCursor(0, tft.fontHeight());
            tft.println("");
            tft.println("     TIME: " + String(final - start) + " mSec");
        }
        break;
    case 4: // cheat
        tft.fillScreen(TFT_RED);
        state = 0;
        tft.setCursor(0, tft.fontHeight());
        tft.println("");
        tft.println("   Too Early!");
        break;
    default:
        state = 0;
        break;
    }
}
