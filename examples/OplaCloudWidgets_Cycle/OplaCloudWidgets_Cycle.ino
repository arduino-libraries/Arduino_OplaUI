#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

#include "arduino_secrets.h"
#include "thingProperties.h"
 
MKRIoTCarrier carrier;
CycleWidgetsApp app;
Gauge1_Widget w0;
Gauge2_Widget w1;
String_Widget w2;
Bool_Widget w3;

void setup() {
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  w0.setTitle("SPEED");
  w0.setSuffix(" km/h");
  w0.attachValue(speed);

  w1.setTitle("TEMPERATURE");
  w1.setSuffix(" C");
  w1.attachValue(temperature);

  w2.setTitle("MESSAGE");
  w2.attachValue(message);

  w3.setTitle("LIGHT");
  w3.attachValue(light);

  carrier.begin();
  app.begin(carrier);
  app.addWidget(w0);
  app.addWidget(w1);
  app.addWidget(w2);
  app.addWidget(w3);
}

void loop() {
  ArduinoCloud.update();
  app.loop();
}

void onSpeedChange() {};
void onTemperatureChange() {};
void onMessageChange() {};
void onLightChange() {};


