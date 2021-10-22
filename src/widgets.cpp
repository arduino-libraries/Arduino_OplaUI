#include "widgets.h"
#include "utils.h"
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

constexpr unsigned long REDRAW_INTERVAL = 100;

void GaugeWidget::begin(MKRIoTCarrier &carrier) {
    Widget::begin(carrier);
    _gauge->begin(carrier.display);
}

void GaugeWidget::show() {
    this->draw(true);
    
    if (!_readonly) {
        _carrier->leds.setPixelColor(3, 20, 0, 0);
        _carrier->leds.setPixelColor(4, 0, 20, 0);
        _carrier->leds.show();
    }
}

void GaugeWidget::hide() {
    if (!_readonly) {
        _carrier->leds.setPixelColor(3, 0, 0, 0);
        _carrier->leds.setPixelColor(4, 0, 0, 0);
        _carrier->leds.show();
    }
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
    
        lcd = &_carrier->display;

        // Print title
        lcd->setFont(&FreeSans9pt7b);
        lcd->setTextSize(1);
        lcd->setTextColor(ST77XX_WHITE);
        drawStringCenter(*lcd, const_cast<char*>(_title.c_str()), lcd->width()/2, _carrier->display.height()/2-30, ST77XX_BLACK);
        
        // Print value
        lcd->setFont(&FreeSans12pt7b);
        lcd->setTextSize(1);
        lcd->setTextColor(ST77XX_WHITE);
        drawStringCenter(*lcd, const_cast<char*>(_value->c_str()), lcd->width()/2, _carrier->display.height()/2+30, ST77XX_BLACK);
        
        _last_value = *_value;
    }
}

void Bool_Widget::draw(bool clear) {
    if (clear || *_value != _last_value) {
        _carrier->display.fillScreen(ST77XX_BLACK);
    
        lcd = &_carrier->display;

        // Print title
        lcd->setFont(&FreeSans9pt7b);
        lcd->setTextSize(1);
        lcd->setTextColor(ST77XX_WHITE);
        drawStringCenter(*lcd, const_cast<char*>(_title.c_str()), lcd->width()/2, _carrier->display.height()/2-30, ST77XX_BLACK);
        
        // Print value
        lcd->setFont(&FreeSansBold24pt7b);
        lcd->setTextSize(1);
        lcd->setTextColor(*_value ? ST77XX_GREEN : ST77XX_RED);
        drawStringCenter(*lcd, const_cast<char*>(*_value ? "ON" : "OFF"), lcd->width()/2, _carrier->display.height()/2+50, ST77XX_BLACK);
        
        _last_value = *_value;
        
        // Update LEDs
        if (!_readonly) {
            if (*_value) {
                _carrier->leds.setPixelColor(3, 20, 0, 0);
            } else {
                _carrier->leds.setPixelColor(3, 0, 20, 0);
            }
            _carrier->leds.show();
        }
    }
}

void Bool_Widget::hide() {
    if (!_readonly) {
        _carrier->leds.setPixelColor(3, 0, 0, 0);
        _carrier->leds.show();
    }
}

void Bool_Widget::onButtonDown(short i) {
    if (_readonly) return;
    if (i == 3) {
        beep(*_carrier);
        *_value = !*_value;
        if (_on_value_change != nullptr) _on_value_change();
    }
}

void WidgetsApp::begin(MKRIoTCarrier &carrier) {
    _carrier = &carrier;

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
