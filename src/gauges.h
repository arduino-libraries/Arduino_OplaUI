#ifndef _OPLA_GAUGES_H
#define _OPLA_GAUGES_H

#include <Adafruit_ST7789.h>

class OplaGauge
{
public:
  String getTitle() const { return title; };
  void setTitle(String t) { title = t; };
  String getSuffix() const { return suffix; };
  void setSuffix(String s) { suffix = s; };
  int getMin() const { return min; };
  int getMax() const { return max; };
  void setRange(int i, int j) { min = i; max = j; };
  int getSteps() const { return steps; };
  void setSteps(int i) { steps = i; };
  int getDigits() const { return digits; };
  void setDigits(int i) { digits = i; };
  void begin(Adafruit_ST7789 &display);
  void setBackgroundColor(uint8_t r, uint8_t g, uint8_t b) {
    _bgR = r; _bgG = g; _bgB = b;
  };
  void clear();
  virtual void draw(float level) = 0;

protected:
  String title;
  String suffix;
  int min = 0;
  int max = 100;
  int steps = 22;
  int digits = 3;
  int w, h;
  Adafruit_ST7789 *display;
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
