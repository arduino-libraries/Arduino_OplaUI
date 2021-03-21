#ifndef _OPLA_GAUGES_H
#define _OPLA_GAUGES_H

#include <Adafruit_ST7789.h>
#include <RREFont.h>

class OplaGauge
{
public:
  String title;
  String suffix;
  int min = 0;
  int max = 100;
  int steps = 22;
  int digits = 3;
  void begin(Adafruit_ST7789 &display);
  void setBackgroundColor(uint8_t r, uint8_t g, uint8_t b) {
    _bgR = r; _bgG = g; _bgB = b;
  };
  void clear();
  virtual void draw(float level) = 0;

protected:
  int w, h;
  Adafruit_ST7789 *display;
  RREFont font;
  uint8_t _bgR, _bgG, _bgB = 0;
};

class OplaGauge1 : public OplaGauge
{
public:
  void draw(float level);
};

class OplaGauge2 : public OplaGauge
{
public:
  OplaGauge2() {
    min = 0;
    max = 10;
    steps = 10;
    setBackgroundColor(255, 255, 255);
  };
  void draw(float level);
};

class OplaGauge3 : public OplaGauge
{
public:
  void draw(float level);
};

class OplaGauge4 : public OplaGauge
{
public:
  void draw(float level);
};

#endif // _OPLA_GAUGES_H
