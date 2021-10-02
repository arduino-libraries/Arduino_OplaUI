#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
DirectWidgetsApp app;
Gauge1_Widget w0;
Gauge2_Widget w1;
Gauge2_Widget w2;
String_Widget w3;
Bool_Widget w4;

float humidity, temperature, pressure;
String message;
bool light;

void setup() {
  w0.setTitle("HUMIDITY");
  w0.setSuffix("%");
  w0.setDigits(1);
  w0.attachValue(humidity);

  w1.setTitle("TEMPERATURE");
  w1.setSuffix(" C");
  w1.setDigits(1);
  w1.setRange(0, 50);
  w1.setSteps(10);
  w1.attachValue(temperature);

  w2.setTitle("PRESSURE");
  w2.setSuffix(" hPa");
  w2.setDigits(1);
  w2.setRange(700, 1200);
  w2.setSteps(8);
  w2.attachValue(pressure);
  
  w3.setTitle("MESSAGE");
  w3.attachValue(message);

  w4.setTitle("LIGHT");
  w4.attachValue(light);

  carrier.begin();
  app.begin(carrier);
  app.setWidget(0, w0);
  app.setWidget(1, w1);
  app.setWidget(2, w2);
  app.setWidget(3, w3);
  app.setWidget(4, w4);

  message = "Hello world!";
  light = false;
}

void loop() {
  app.loop();

  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();
  pressure = carrier.Pressure.readPressure()*10;
  delay(100);
}
