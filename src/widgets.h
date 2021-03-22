#ifndef _OPLA_WIDGETS_H
#define _OPLA_WIDGETS_H

#include <Arduino_MKRIoTCarrier.h>
#include "gauges.h"

class Widget {
  public:
  virtual void begin(MKRIoTCarrier &carrier) {
    _carrier = &carrier;
  };
  virtual void show() {
    this->draw(true);
  };
  virtual void draw(bool clear = true) = 0;
  virtual void onButtonDown(short i) {};
  void setReadOnly(bool v) {
    _readonly = v;
  };

  protected:
  MKRIoTCarrier* _carrier = nullptr;
  bool _readonly = false;
};

class GaugeWidget : public Widget {
  public:
  void attachValue(float &val) {
    float_value = &val;
  };
  void attachValue(int &val) {
    int_value = &val;
  };
  void setTitle(char* v) {
    _gauge->title = v;
  };
  void setSuffix(char* v) {
    _gauge->suffix = v;
  };
  void setMin(int v) {
    _gauge->min = v;
  };
  void setMax(int v) {
    _gauge->max = v;
  };
  void setSteps(int v) {
    _gauge->steps = v;
  };
  void setDigits(int v) {
    _gauge->digits = v;
  };
  void begin(MKRIoTCarrier &carrier);
  void show();
  void draw(bool clear = true);
  void onButtonDown(short i);
  
  protected:
  OplaGauge* _gauge = nullptr;
  float* float_value = nullptr;
  int* int_value = nullptr;
  float getFloatValue() {
    if (float_value != nullptr) {
      return *float_value;
    } else if (int_value != nullptr) {
      return (float)*int_value;
    } else {
      return 0;
    }
  };
  void incrementValue(float v) {
    if (float_value != nullptr) {
      *float_value += v;
    } else if (int_value != nullptr) {
      *int_value += v;
    }
  };
};

class Gauge1_Widget : public GaugeWidget {
  public:
  Gauge1_Widget() {
    _gauge = new OplaGauge1 ();  // TODO: destroy this
  };
};

class Gauge2_Widget : public GaugeWidget {
  public:
  Gauge2_Widget() {
    _gauge = new OplaGauge2 ();  // TODO: destroy this
  };
};

class String_Widget : public Widget {
  public:
  void attachValue(const String &val) {
    _value = &val;
  };
  void setTitle(char* v) {
    _title = v;
  };
  void draw(bool clear = true);
  
  protected:
  const String* _value = nullptr;
  String _last_value;
  String _title;
};

class Bool_Widget : public Widget {
  public:
  void attachValue(bool &val) {
    _value = &val;
  };
  void setTitle(char* v) {
    _title = v;
  };
  void show() {
    this->draw(true);
  }
  void draw(bool clear = true);
  void onButtonDown(short i);
  
  protected:
  bool* _value = nullptr;
  bool _last_value;
  String _title;
};

class WidgetsApp {
  public:
  void begin(MKRIoTCarrier &carrier);
  virtual void loop();
  virtual void onButtonDown(short i) = 0;
  virtual Widget* getCurrentWidget() = 0;
  virtual void redrawCurrentWidget();
  
  protected:
  MKRIoTCarrier* _carrier = nullptr;
  short _current_widget = -1;
  unsigned long _last_draw = 0;
};

static void beep(MKRIoTCarrier &carrier) {
    carrier.Buzzer.sound(800);
    delay(20);
    carrier.Buzzer.noSound();
}

#endif
