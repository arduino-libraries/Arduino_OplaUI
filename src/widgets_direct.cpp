#include "widgets_direct.h"

void DirectWidgetsApp::onButtonDown(short i) {
    this->showWidget(i);
}

void DirectWidgetsApp::setWidget(size_t i, Widget &w) {
    _widgets[i] = &w;
    w.setReadOnly(true);
    w.begin(*_carrier);
}

void DirectWidgetsApp::showWidget(short i) {
    if (i > -1 && _widgets[i] == nullptr) return;
    if (i == _current_widget) return;
    _current_widget = i;

    // Update LEDs
    _carrier->leds.clear();
    if (i > -1)
      _carrier->leds.setPixelColor(i, 20, 20, 20);
    _carrier->leds.show();

    // Provide acustic feedback
    if (i > -1) {
      beep(*_carrier);
    }

    // Update display
    if (i == -1) {
      _carrier->display.fillScreen(ST77XX_BLACK);
    } else {
      _widgets[i]->show();
      _last_draw = millis();
    }
}

