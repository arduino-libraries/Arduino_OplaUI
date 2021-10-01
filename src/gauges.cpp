#include "gauges.h"
#include "utils.h"
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

// ------------------------------------------------
// Pre-computed sin table
constexpr short MAXSIN = 255;
const uint8_t sinTab[91] PROGMEM = {
    0, 4, 8, 13, 17, 22, 26, 31, 35, 39, 44, 48, 53, 57, 61, 65, 70, 74, 78, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119, 123,
    127, 131, 135, 138, 142, 146, 149, 153, 156, 160, 163, 167, 170, 173, 177, 180, 183, 186, 189, 192, 195, 198, 200, 203, 206, 208, 211, 213, 216, 218,
    220, 223, 225, 227, 229, 231, 232, 234, 236, 238, 239, 241, 242, 243, 245, 246, 247, 248, 249, 250, 251, 251, 252, 253, 253, 254, 254, 254, 254, 254,
    255};

int fastSin(int i)
{
  while (i < 0)
    i += 360;
  while (i >= 360)
    i -= 360;
  if (i < 90)
    return (pgm_read_byte(&sinTab[i]));
  else if (i < 180)
    return (pgm_read_byte(&sinTab[180 - i]));
  else if (i < 270)
    return (-pgm_read_byte(&sinTab[i - 180]));
  else
    return (-pgm_read_byte(&sinTab[360 - i]));
}

int fastCos(int i)
{
  return fastSin(i + 90);
}
// ------------------------------------------------

#define RGBto565(r, g, b) ((((r)&0xF8) << 8) | (((g)&0xFC) << 3) | ((b) >> 3))

// ------------------------------------------------
// Input a value 0 to 511 (85*6) to get a color value.
// The colours are a transition R - Y - G - C - B - M - R.
static uint16_t rgbWheel(int idx)
{
  uint8_t r, g, b;

  idx &= 0x1ff;
  if (idx < 85)
  { // R->Y
    r = 255;
    g = idx * 3;
    b = 0;
  }
  else if (idx < 85 * 2)
  { // Y->G
    idx -= 85 * 1;
    r = 255 - idx * 3;
    g = 255;
    b = 0;
  }
  else if (idx < 85 * 3)
  { // G->C
    idx -= 85 * 2;
    r = 0;
    g = 255;
    b = idx * 3;
  }
  else if (idx < 85 * 4)
  { // C->B
    idx -= 85 * 3;
    r = 0;
    g = 255 - idx * 3;
    b = 255;
  }
  else if (idx < 85 * 5)
  { // B->M
    idx -= 85 * 4;
    r = idx * 3;
    g = 0;
    b = 255;
  }
  else
  { // M->R
    idx -= 85 * 5;
    r = 255;
    g = 0;
    b = 255 - idx * 3;
  }

  return RGBto565(r, g, b);
}

static Adafruit_ST7789 *lcd;

void OplaGauge::begin(Adafruit_ST7789 &d)
{
  this->display = &d;
  d.setRotation(0);
  w = d.width();
  h = d.height();
}

void OplaGauge::clear()
{
  this->display->fillScreen(RGBto565(_bgR, _bgG, _bgB));
}

void OplaGauge1::draw(float level)
{
  lcd = this->display;

  // Center gauge around this point
  const int cx = w / 2;
  const int cy = h / 2;

  // Radius of the inner circle (black area)
  const int rx0 = 70, ry0 = 70;

  // Radius of the outer circle
  const int rx1 = cx, ry1 = cy;

  // Angle range of the steps
  const int mina = -75;
  const int maxa = 180 + 75;
  const int gapa = 5;
  const int stepa = (maxa + gapa - mina) / steps;
  const int sectora = stepa - gapa;

  for (int i = 0; i < steps; i++)
  {
    const int a = mina + stepa * i;
    int sx = fastCos(a - 180);
    int sy = fastSin(a - 180);
    int xs0 = cx + sx * rx0 / MAXSIN;
    int ys0 = cy + sy * ry0 / MAXSIN;
    int xe0 = cx + sx * rx1 / MAXSIN;
    int ye0 = cy + sy * ry1 / MAXSIN;
    sx = fastCos(a + sectora - 180);
    sy = fastSin(a + sectora - 180);
    int xs1 = cx + sx * rx0 / MAXSIN;
    int ys1 = cy + sy * ry0 / MAXSIN;
    int xe1 = cx + sx * rx1 / MAXSIN;
    int ye1 = cy + sy * ry1 / MAXSIN;
    float l = min + (max - min) * 100 / steps * i / 100;
    if (l < level)
    {
      lcd->fillTriangle(xs0, ys0, xe0, ye0, xe1, ye1, rgbWheel(512L * l / 100));
      lcd->fillTriangle(xs1, ys1, xe1, ye1, xs0, ys0, rgbWheel(512L * l / 100));
    }
    else
    {
      lcd->fillTriangle(xs0, ys0, xe0, ye0, xe1, ye1, RGBto565(60, 60, 60));
      lcd->fillTriangle(xs1, ys1, xe1, ye1, xs0, ys0, RGBto565(60, 60, 60));
    }
  }

  // Print value
  lcd->setFont(&FreeSansBold12pt7b);
  lcd->setTextSize(1);
  lcd->setTextColor(ST77XX_WHITE);
  {
    String buf = String(level, digits) + suffix;
    drawStringCenter(*lcd, const_cast<char*>(buf.c_str()), lcd->width()/2, h/2+25, ST77XX_BLACK);
  }

  // Print title
  lcd->setFont(&FreeSans9pt7b);
  lcd->setTextSize(1);
  lcd->setTextColor(ST77XX_WHITE);
  drawStringCenter(*lcd, const_cast<char*>(title.c_str()), lcd->width()/2, h/2-6, ST77XX_BLACK);
}

void OplaGauge2::draw(float level)
{
  lcd = this->display;

  const int cx = w / 2;
  const int cy = h / 2;

  const int rx0 = w / 2 - 4;
  int rx1 = rx0 - 8;
  const int rx2 = rx1 - 15;

  const int mina = 15;
  const int maxa = 180 - 15;
  const int stepa = (maxa - mina) / steps;
  
  int lastx, lasty;
  static int xs0Old = 0, ys0Old = 0, xs1Old = 0, ys1Old = 0, xe0Old = 0, ye0Old = 0, xe1Old = 0, ye1Old = 0;
  for (int a = mina; a <= maxa; a += stepa)
  {
    int sx = fastCos(a - 180);
    int sy = fastSin(a - 180);
    int xs0 = cx + sx * rx0 / MAXSIN;
    int ys0 = cy + sy * rx0 / MAXSIN;
    int xe0 = cx + sx * rx1 / MAXSIN;
    int ye0 = cy + sy * rx1 / MAXSIN;
    lcd->drawLine(xs0, ys0, xe0, ye0, ST77XX_BLACK);
    if (a > mina)
      lcd->drawLine(xs0, ys0, lastx, lasty, ST77XX_BLACK);
    lastx = xs0;
    lasty = ys0;
    xs0 = cx + sx * rx2 / MAXSIN;
    ys0 = cy + sy * rx2 / MAXSIN;

    int i = (a - mina) / stepa;  // number of step
    int v = min + (max - min) / steps * i;  // value of step
    lcd->setFont(&FreeSans9pt7b);
    lcd->setTextSize(1);
    lcd->setTextColor(RGBto565(150, 150, 150));
    {
      String buf(v);
      drawStringCenter(*lcd, const_cast<char*>(buf.c_str()), xs0, ys0+4, ST77XX_WHITE);
    }
  }

  int a = map(level, min, max, mina, maxa);
  rx1 = rx0 - 12;
  int sx = fastCos(a - 180);
  int sy = fastSin(a - 180);
  int xs0 = cx + (sx * rx1 + MAXSIN / 2) / MAXSIN;
  int ys0 = cy + (sy * rx1 + MAXSIN / 2) / MAXSIN;
  rx1 = rx0 - 42;
  sx = fastCos(a - 180);
  sy = fastSin(a - 180);
  int xs1 = cx + (sx * rx1 + MAXSIN / 2) / MAXSIN;
  int ys1 = cy + (sy * rx1 + MAXSIN / 2) / MAXSIN;
  rx1 = rx0 - 42;
  sx = fastCos(a - 180 - 8);
  sy = fastSin(a - 180 - 8);
  int xe0 = cx + (sx * rx1 + MAXSIN / 2) / MAXSIN;
  int ye0 = cy + (sy * rx1 + MAXSIN / 2) / MAXSIN;
  sx = fastCos(a - 180 + 8);
  sy = fastSin(a - 180 + 8);
  int xe1 = cx + (sx * rx1 + MAXSIN / 2) / MAXSIN;
  int ye1 = cy + (sy * rx1 + MAXSIN / 2) / MAXSIN;

  lcd->drawLine(xs1Old, ys1Old, cx, cy, ST77XX_WHITE);
  lcd->fillTriangle(xs0Old, ys0Old, xe0Old, ye0Old, xe1Old, ye1Old, ST77XX_WHITE);
  lcd->drawLine(xs1, ys1, cx, cy, ST77XX_BLACK);
  lcd->fillTriangle(xs0, ys0, xe0, ye0, xe1, ye1, ST77XX_RED);
  lcd->fillCircle(cx, cy, 2, ST77XX_BLACK);
  xs0Old = xs0;
  ys0Old = ys0;
  xs1Old = xs1;
  ys1Old = ys1;
  xe0Old = xe0;
  ye0Old = ye0;
  xe1Old = xe1;
  ye1Old = ye1;

  // Print value
  lcd->setFont(&FreeSansBold12pt7b);
  lcd->setTextSize(1);
  lcd->setTextColor(ST77XX_BLACK);
  {
    String buf = String(level, digits) + suffix;
    drawStringCenter(*lcd, const_cast<char*>(buf.c_str()), lcd->width()/2, h/2+38, ST77XX_WHITE);
  }

  // Print title
  lcd->setFont(&FreeSans9pt7b);
  lcd->setTextSize(1);
  lcd->setTextColor(ST77XX_BLACK);
  drawStringCenter(*lcd, const_cast<char*>(title.c_str()), lcd->width()/2, h/2+65, ST77XX_WHITE);
}

void OplaGauge3::draw(float level)
{
  lcd = this->display;

  int cx = w / 2;
  int cy = h / 2;
  int rx0 = w / 2 - 4;
  int mina = 0;
  int maxa = 270;
  int lastx, lasty;
  static int xs0Old = 0, ys0Old = 0;
  for (int i = mina; i <= maxa; i += 3)
  {
    int sx = fastCos(i - 270);
    int sy = fastSin(i - 270);
    int xs0 = cx + sx * rx0 / MAXSIN;
    int ys0 = cy + sy * rx0 / MAXSIN;
    if (i > mina)
      lcd->drawLine(xs0, ys0, lastx, lasty, ST77XX_WHITE);
    lastx = xs0;
    lasty = ys0;
  }
  for (int i = mina; i <= maxa; i += 9)
  {
    int sx = fastCos(i - 270);
    int sy = fastSin(i - 270);
    int xs0 = cx + sx * rx0 / MAXSIN;
    int ys0 = cy + sy * rx0 / MAXSIN;
    int rx1 = (i % 27 == 0) ? rx0 - 25 : rx0 - 23;
    int xe0 = cx + sx * rx1 / MAXSIN;
    int ye0 = cy + sy * rx1 / MAXSIN;
    lcd->drawLine(xs0, ys0, xe0, ye0, ST77XX_WHITE);
  }

  int a = map(level, 0, 100, mina, maxa);
  int rx1 = rx0 - 30;
  int sx = fastCos(a - 270);
  int sy = fastSin(a - 270);
  int xs0 = cx + sx * rx1 / MAXSIN;
  int ys0 = cy + sy * rx1 / MAXSIN;
  lcd->drawLine(xs0Old, ys0Old, cx, cy, ST77XX_BLACK);
  lcd->drawLine(xs0, ys0, cx, cy, ST77XX_RED);
  lcd->fillCircle(cx, cy, 2, ST77XX_RED);
  xs0Old = xs0;
  ys0Old = ys0;

  lcd->fillRect(w - 13 * 3 - 2, h - 20, 13 * 3 + 2, 20, ST77XX_BLACK);

  lcd->setFont(&FreeSansBold12pt7b);
  lcd->setTextSize(1);
  lcd->setTextColor(ST77XX_WHITE);
  {
    String buf(level);
    drawStringRight(*lcd, const_cast<char*>(buf.c_str()), lcd->width(), h - 20);
  }
}

void OplaGauge4::draw(float level)
{
  lcd = this->display;

  int cx = w / 2 - 1;
  int cy = h / 2 - 1;
  int rx0 = cx - 2, ry0 = cy - 2;
  int rx1 = rx0 - 20, ry1 = ry0 - 20;
  int rx2 = rx0 + 2, ry2 = ry0 + 2;
  int mina = 0;
  int maxa = 90;
  int xs1, ys1, xe1, ye1;
  for (int i = mina; i <= maxa; i += 5)
  {
    int sx = fastCos(i - 180);
    int sy = fastSin(i - 180);
    int xs0 = cx + sx * rx0 / MAXSIN;
    int ys0 = cy + sy * ry0 / MAXSIN;
    int xe0 = cx + sx * rx1 / MAXSIN;
    int ye0 = cy + sy * ry1 / MAXSIN;
    if (i > mina)
    {
      if (i <= level)
      {
        lcd->fillTriangle(xs0, ys0, xe0, ye0, xe1, ye1, rgbWheel(85 * 2 * (90 - i) / 90));
        lcd->fillTriangle(xs1, ys1, xe1, ye1, xs0, ys0, rgbWheel(85 * 2 * (90 - i) / 90));
      }
      else
      {
        lcd->fillTriangle(xs0, ys0, xe0, ye0, xe1, ye1, RGBto565(60, 60, 60));
        lcd->fillTriangle(xs1, ys1, xe1, ye1, xs0, ys0, RGBto565(60, 60, 60));
      }
    }
    xs1 = xs0;
    ys1 = ys0;
    xe1 = xe0;
    ye1 = ye0;
    xe0 = cx + sx * rx2 / MAXSIN;
    ye0 = cy + sy * ry2 / MAXSIN;
    lcd->drawLine(xs0, ys0, xe0, ye0, ST77XX_YELLOW);
  }
  lcd->fillRect(w - (13 * 3 + 2) * 1, h - 20, (13 * 3 + 2) * 1, 20, ST77XX_BLACK);

  lcd->setFont(&FreeSansBold12pt7b);
  lcd->setTextSize(1);
  lcd->setTextColor(ST77XX_WHITE);
  {
    String buf(level);
    drawStringRight(*lcd, const_cast<char*>(buf.c_str()), lcd->width(), h - 20);
  }
}
