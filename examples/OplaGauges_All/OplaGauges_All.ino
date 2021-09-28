#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaGauge1 gauge1;
OplaGauge2 gauge2;
OplaGauge3 gauge3;
OplaGauge4 gauge4;

void setup()
{
  carrier.begin();
  gauge1.begin(carrier.display);
  gauge1.setSuffix("%");

  gauge2.begin(carrier.display);
  gauge3.begin(carrier.display);
  gauge4.begin(carrier.display);
}

void loop()
{
  unsigned long ms;
  constexpr unsigned long tm = 5000;

  carrier.display.fillScreen(ST77XX_BLACK);
  ms = millis();
  while (millis() - ms < tm)
  {
    for (int i = gauge1.getMin(); i <= gauge1.getMax(); i += 5)
    {
      gauge1.draw(i);
      delay(10);
    }
    delay(250);
    for (int i = gauge1.getMax(); i >= gauge1.getMin(); i -= 5)
    {
      gauge1.draw(i);
      delay(10);
    }
    delay(250);
  }

  carrier.display.fillScreen(ST77XX_WHITE);
  ms = millis();
  while (millis() - ms < tm)
  {
    for (int i = 0; i <= 100; i += 2)
    {
      gauge2.draw(i);
      delay(10);
    }
    delay(450);
    for (int i = 100; i >= 0; i -= 2)
    {
      gauge2.draw(i);
      delay(10);
    }
    delay(450);
  }

  carrier.display.fillScreen(ST77XX_BLACK);
  ms = millis();
  while (millis() - ms < tm)
  {
    for (int i = 0; i <= 100; i += 2)
    {
      gauge3.draw(i);
      delay(40);
    }
    delay(450);
    for (int i = 100; i >= 0; i -= 2)
    {
      gauge3.draw(i);
      delay(40);
    }
    delay(450);
  }

  carrier.display.fillScreen(ST77XX_BLACK);
  ms = millis();
  while (millis() - ms < tm)
  {
    for (int i = 0; i <= 90; i += 5)
    {
      gauge4.draw(i);
      delay(40);
    }
    delay(450);
    for (int i = 90; i >= 0; i -= 5)
    {
      gauge4.draw(i);
      delay(40);
    }
    delay(450);
  }
}
