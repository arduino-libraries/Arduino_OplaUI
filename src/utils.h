#ifndef _OPLAUI_UTILS_H
#define _OPLAUI_UTILS_H

static void drawStringLeft(Adafruit_ST7789 &display, char *buf, int x, int y) {
    display.setCursor(x, y);
    display.print(buf);
}

static void drawStringCenter(Adafruit_ST7789 &display, char *buf, int x, int y, uint16_t bgColor) {
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
    display.fillRect(x-w/2 + x1, y + y1, w+6, h, bgColor);
    display.setCursor(x-w/2, y);
    display.print(buf);
}

static void drawStringRight(Adafruit_ST7789 &display, char *buf, int x, int y) {
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, x, y, &x1, &y1, &w, &h);
    display.setCursor(x - w, y);
    display.print(buf);
}

#endif
