#ifndef _OPLA_COLOR_MENU_H
#define _OPLA_COLOR_MENU_H

#include <vector>

class OplaColorMenu {
  public:
  typedef void(*menuItemHandler)(void);
  
  uint16_t bgColor = 0x0000;
  uint16_t labelColor = 0xFFFF;

  OplaColorMenu(MKRIoTCarrier &carrier) {
    _carrier = &carrier;
  };
  void reset() {
    _items.clear();
  };
  void add(String label, uint16_t color, menuItemHandler handler) {
    _items.push_back({label, color, handler});
  };
  void getSize(uint16_t* mw, uint16_t* mh) const {
    _carrier->display.setTextSize(2);
    
    // Get the width of the largest menu item (label + square).
    // We'll use this to center the menu horizontally
    uint16_t maxWidth = 0;
    for (auto item : _items) {
      int16_t  x1, y1;
      uint16_t w, h;
      _carrier->display.getTextBounds(item.label, 0, 0, &x1, &y1, &w, &h);
      maxWidth = std::max(maxWidth, w);
    }
    maxWidth += _line_height*1.5;

    // Get the minimum y coordinate of the bottom-left corner of the menu.
    // The wider it is, the higher it needs to go.
    uint16_t y = 120 + sqrt(sq(120) - sq(maxWidth/2));
    
    // Get the y coordinate of the top-left corner of the menu
    y -= (_items.size() * (_line_height + _line_spacing)) + 3;

    *mw = maxWidth;
    *mh = 240 - y;
  };
  uint16_t height() const {
    uint16_t mw, mh;
    getSize(&mw, &mh);
    return mh;
  };
  void draw() {
    // Compute the size of the menu
    uint16_t mw, mh;
    getSize(&mw, &mh);

    // Compute the coordinates of the menu
    uint16_t x = (240 - mw)/2;
    uint16_t y = 240 - mh;

    // Clear the background
    _carrier->display.fillRect(x, y, _line_height, _line_height, bgColor);

    // Draw menu items
    _carrier->display.setTextColor(labelColor);
    _carrier->display.setTextSize(2);
    for (auto item : _items) {
      _carrier->display.fillRect(x, y, _line_height, _line_height, item.color);
      _carrier->display.setCursor(x + _line_height*1.5, y);
      _carrier->display.print(item.label);
      y += _line_height + _line_spacing;
    }

    // Apply colors to LEDs
    _carrier->leds.clear();
    _carrier->leds.setBrightness(12);
    for (size_t i = 0; i < _items.size(); ++i) {
      // Convert color from 565 encoding
      uint8_t r = ((((_items[i].color >> 11) & 0x1F) * 527) + 23) >> 6;
      uint8_t g = ((((_items[i].color >> 5) & 0x3F) * 259) + 33) >> 6;
      uint8_t b = (((_items[i].color & 0x1F) * 527) + 23) >> 6;
      _carrier->leds.setPixelColor(getButtonIdx(i), r, g, b);
    }
    _carrier->leds.show();
  };
  void handleButtons() {
    _carrier->Buttons.update();
    for (int i = 0; i <= 4; ++i) {
      if (_carrier->Buttons.onTouchDown((touchButtons)i)) {
        // A button was touched. Now find its menu item
        for (int j = 0; j < _items.size(); ++j) {
          if (getButtonIdx(j) == i) {
            _carrier->Buzzer.beep();

            // Clear all LEDs except the one pressed
            for (int k = 0; k <= 4; ++k) {
              if (k == i) continue;
              _carrier->leds.setPixelColor(k, 0, 0, 0);              
            }
            _carrier->leds.setBrightness(128);
            _carrier->leds.show();

            // Run the handler
            _items[j].handler();

            return;
          }          
        }
      }      
    }
  };

  private:
  static constexpr int _line_height = 14;
  static constexpr int _line_spacing = 4;
  
  struct menuItem {
    String label;
    uint16_t color;
    menuItemHandler handler;
  };
  std::vector<menuItem> _items;
  MKRIoTCarrier* _carrier = nullptr;

  // This method distributes buttons in a way that is slightly
  // better for UX
  int getButtonIdx(int i) {
    if ((_items.size() == 3 || _items.size() == 4) && i >= 2) {
      return i + 1;
    } else if (i == 0) {
      return 1;
    } else if (i == 1) {
      return 0;
    } else {
      return i;
    }
  };
};
#endif // _OPLA_COLOR_MENU_H
