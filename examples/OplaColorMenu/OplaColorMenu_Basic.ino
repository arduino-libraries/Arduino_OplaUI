#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaColorMenu menu(carrier);

void setup() {
  carrier.noCase();
  carrier.begin();

  carrier.display.fillScreen(0x0000);
  carrier.display.setRotation(0);

  draw_page1();
}

void loop() {
  menu.handleButtons();
}

void draw_page1() {
  carrier.display.fillScreen(0x0000);
  menu.reset();
  menu.add("< BACK", 0x001F, draw_page1);
  menu.add("ENABLE", 0xFFE0, draw_page2);
  menu.add("SET DAYS", 0xF800, draw_page1);
  menu.add("SET TIME", 0x07E0, draw_page1);
  menu.draw();
}

void draw_page2() {
  carrier.display.fillScreen(0x0000);
  menu.reset();
  menu.add("< BACK", 0xF800, draw_page1);
  menu.add("BARBARBAR", 0x07E0, draw_page2);
  menu.add("BAZ", 0x001F, draw_page2);
  menu.draw();
}
