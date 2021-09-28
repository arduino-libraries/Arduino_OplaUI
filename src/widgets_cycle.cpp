#include "widgets_cycle.h"

void CycleWidgetsApp::addWidget(Widget &w) {
    _widgets.push_back(&w);
    w.begin(*_carrier);
}

void CycleWidgetsApp::begin(MKRIoTCarrier &carrier) {
    WidgetsApp::begin(carrier);
    
    _carrier->leds.setPixelColor(0, 10, 10, 10);
    _carrier->leds.show();
}

void CycleWidgetsApp::onButtonDown(short i) {
    if (i == 0) {
        size_t j = _current_widget + 1;
        if (j >= _widgets.size()) j = 0;
        this->showWidget(j);
    } else if (Widget* w = this->getCurrentWidget()) {
        w->onButtonDown(i);
    }
}

void CycleWidgetsApp::showWidget(short i) {
    if (i > -1 && _widgets[i] == nullptr) return;
    if (i == _current_widget) return;

    if (Widget* w = this->getCurrentWidget()) {
      w->hide();
    }

    _current_widget = i;

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
