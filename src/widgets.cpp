#include "widgets.h"
#include "rre_arialb_16.h"
#include "rre_tahoma_65v.h"
#include "rre_6x8.h"

constexpr unsigned long REDRAW_INTERVAL = 100;

void Widget::show() {
    this->draw(true);

    _carrier->leds.setPixelColor(1, 0, 0, 0);
    _carrier->leds.setPixelColor(2, 0, 0, 0);
    _carrier->leds.setPixelColor(3, 0, 0, 0);
    _carrier->leds.setPixelColor(4, 0, 0, 0);
    _carrier->leds.show();
}

void GaugeWidget::begin(MKRIoTCarrier &carrier) {
    Widget::begin(carrier);
    _gauge->begin(carrier.display);
}

void GaugeWidget::show() {
    this->draw(true);
    
    _carrier->leds.setPixelColor(1, 0, 0, 0);
    _carrier->leds.setPixelColor(2, 0, 0, 0);
    if (_readonly) {
        _carrier->leds.setPixelColor(3, 0, 0, 0);
        _carrier->leds.setPixelColor(4, 0, 0, 0);
    } else {
        _carrier->leds.setPixelColor(3, 20, 0, 0);
        _carrier->leds.setPixelColor(4, 0, 20, 0);
    }
    _carrier->leds.show();
}

void GaugeWidget::draw(bool clear) {
    if (clear) _gauge->clear();
    _gauge->draw(this->getFloatValue());
}

void GaugeWidget::onButtonDown(short i) {
    if (_readonly) return;
    if (i == 3) {
        beep(*_carrier);
        this->incrementValue(1);
    } else if (i == 4) {
        beep(*_carrier);
        this->incrementValue(-1);
    }
}

static Adafruit_ST7789* lcd;

void String_Widget::draw(bool clear) {
    if (clear || *_value != _last_value) {
        _carrier->display.fillScreen(ST77XX_BLACK);
    
        RREFont font;
        lcd = &_carrier->display;
        font.init([](int x, int y, int w, int h, int c) {
            lcd->fillRect(x, y, w, h, c);
        }, _carrier->display.width(), _carrier->display.height());

        // Print title
        font.setFont(&rre_6x8);
        font.setColor(ST77XX_WHITE, ST77XX_BLACK);
        font.printStr(ALIGN_CENTER, _carrier->display.height()/2-30, const_cast<char*>(_title.c_str()));
        
        // Print value
        font.setFont(&rre_arialb_16);
        font.setColor(ST77XX_WHITE, ST77XX_BLACK);
        font.printStr(ALIGN_CENTER, _carrier->display.height()/2, const_cast<char*>(_value->c_str()));
        
        _last_value = *_value;
    }
}

void Bool_Widget::draw(bool clear) {
    if (clear || *_value != _last_value) {
        _carrier->display.fillScreen(ST77XX_BLACK);
    
        RREFont font;
        lcd = &_carrier->display;
        font.init([](int x, int y, int w, int h, int c) {
            lcd->fillRect(x, y, w, h, c);
        }, _carrier->display.width(), _carrier->display.height());

        // Print title
        font.setFont(&rre_6x8);
        font.setColor(ST77XX_WHITE, ST77XX_BLACK);
        font.printStr(ALIGN_CENTER, _carrier->display.height()/2-30, const_cast<char*>(_title.c_str()));
        
        // Print value
        font.setFont(&rre_tahoma_65v);
        font.setColor(ST77XX_WHITE, ST77XX_BLACK);
        font.printStr(ALIGN_CENTER, _carrier->display.height()/2, const_cast<char*>(*_value ? "ON" : "OFF"));
        
        _last_value = *_value;
        
        // Update LEDs
        _carrier->leds.setPixelColor(1, 0, 0, 0);
        _carrier->leds.setPixelColor(2, 0, 0, 0);
        _carrier->leds.setPixelColor(4, 0, 0, 0);
        if (_readonly) {
            _carrier->leds.setPixelColor(3, 0, 0, 0);
        } else if (*_value) {
            _carrier->leds.setPixelColor(3, 20, 0, 0);
        } else {
            _carrier->leds.setPixelColor(3, 0, 20, 0);
        }
        _carrier->leds.show();
    }
}

void Bool_Widget::onButtonDown(short i) {
    if (_readonly) return;
    if (i == 3) {
        beep(*_carrier);
        *_value = !*_value;
    }
}

void WidgetsApp::begin(MKRIoTCarrier &carrier) {
    _carrier = &carrier;
    _carrier->begin();

    // Perform a nice LED animation to signal the app is running
    for (short i = 0; i <= 4; i++) {
      _carrier->leds.setPixelColor(i, 20, 20, 20);
      _carrier->leds.show();
      delay(60);
      _carrier->leds.setPixelColor(i, 0);
      _carrier->leds.show();
    }
}

void WidgetsApp::loop() {
    // detect buttons
    _carrier->Buttons.update();
    if (_carrier->Button0.onTouchDown()) {
        this->onButtonDown(0);
    } else if (_carrier->Button1.onTouchDown()) {
        this->onButtonDown(1);
    } else if (_carrier->Button2.onTouchDown()) {
        this->onButtonDown(2);
    } else if (_carrier->Button3.onTouchDown()) {
        this->onButtonDown(3);
    } else if (_carrier->Button4.onTouchDown()) {
        this->onButtonDown(4);
    } else {
        this->redrawCurrentWidget();
    }
}

void WidgetsApp::redrawCurrentWidget() {
    if (_current_widget > -1 && (millis()-_last_draw) >= REDRAW_INTERVAL) {
        this->getCurrentWidget()->draw(false);
        _last_draw = millis();
    }
}
