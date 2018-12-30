///////////////////////////////////////////////////////////////////////////////
//`File: utils.cpp
// Desc: Implementation file for some helpful functions.
//
//
// Author: Irfan Hossain
///////////////////////////////////////////////////////////////////////////////
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TCS34725.h>
#include <Arduino.h>
#include <Math.h>
#include "utils.h"

extern Adafruit_TCS34725 tcs;
extern Adafruit_NeoPixel pixels;
extern uint8_t gammatable[256];
/////////////////////////////////////////
// Thanks to adafruit and PhilB for the
// gammatable! The table converts the TCS
// rgb data to colors human eyes can see.
void initGammaTable()
{
  for (int i=0; i<256; i++)
  {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    gammatable[i] = x;
  }
}

/////////////////////////////////////////
// This function is based on example code
// from the colorview example in the TCS
// library.
uint8_t convertToGamma(uint16_t clr, uint16_t rawData)
{
  float color;
  uint8_t idx;

  color = rawData;
  color /= clr;
  color *= 256;

  idx = (uint8_t)color;

  return gammatable[idx];
}

/////////////////////////////////////////
//
uint8_t mapColors256(uint16_t rawData, uint16_t max)
{
  uint8_t neoData;
  neoData = map(rawData, 0, max, 0, 255);
  return neoData;
}

/////////////////////////////////////////
// Custom get data function to get data
// in one shot.
void getData(uint16_t *red, uint16_t *blue, uint16_t *green, uint16_t *clr)
{
  digitalWrite(TCS_LED_PIN, HIGH);
  delay(TCS_DELAY);
  tcs.getRawData(red, blue, green, clr);
  delay(TCS_DELAY);
  digitalWrite(TCS_LED_PIN, LOW);
  delay(TCS_DELAY);
}

/////////////////////////////////////////
//
void calibrate(uint16_t *red, uint16_t *blue, uint16_t *green, uint16_t *clr)
{
  uint16_t dummy1, dummy2;

  updateAllPixels(NEOPIXEL_ON, 0, 0, NUMPIXELS, CALIBRATE_DELAY);
  getData(red, &dummy1, &dummy2, clr);
  updateAllPixels(0, 0, 0, NUMPIXELS, 0);

  updateAllPixels(0, NEOPIXEL_ON, 0, NUMPIXELS, CALIBRATE_DELAY);
  getData(&dummy1, green, &dummy2, clr);
  updateAllPixels(0, 0, 0, NUMPIXELS, 0);

  updateAllPixels(0, 0, NEOPIXEL_ON, NUMPIXELS, CALIBRATE_DELAY);
  getData(&dummy1, &dummy2, blue, clr);
  updateAllPixels(0, 0, 0, NUMPIXELS, 0);
}

/////////////////////////////////////////
// Update all the neopixels.
void updateAllPixels(uint8_t red, uint8_t blue, uint8_t green, int num, int delayTime)
{
  for(int i=0;i<num;i++)
  {
    pixels.setPixelColor(i, red, green, blue);
    pixels.show();
    delay(delayTime);
  }
}

/////////////////////////////////////////
//
void printValues(uint8_t red, uint8_t green, uint8_t blue)
{
  Serial.print("RED: "); Serial.print("\t"); Serial.println(red);
  Serial.print("GRN: "); Serial.print("\t"); Serial.println(green);
  Serial.print("BLU: "); Serial.print("\t"); Serial.println(blue);
  Serial.println(" ");
}
