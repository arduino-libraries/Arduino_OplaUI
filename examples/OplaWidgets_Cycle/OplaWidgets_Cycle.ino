#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
CycleWidgetsApp app;
Gauge1_Widget w0;
Gauge2_Widget w1;
Gauge2_Widget w2;
String_Widget w3;
Bool_Widget w4;
Gauge1_Widget w5;

float humidity, temperature, pressure, counter;
String message;
bool light;

void setup() {
  w0.setTitle("HUMIDITY");
  w0.setSuffix("%");
  w0.setDigits(1);
  w0.attachValue(humidity);
  w0.setReadOnly(true);

  w1.setTitle("TEMPERATURE");
  w1.setSuffix(" C");
  w1.setDigits(1);
  w1.setRange(0, 50);
  w1.setSteps(10);
  w1.attachValue(temperature);
  w1.setReadOnly(true);

  w2.setTitle("PRESSURE");
  w2.setSuffix(" hPa");
  w2.setDigits(1);
  w2.setRange(700, 1200);
  w2.setSteps(10);
  w2.attachValue(pressure);
  w2.setReadOnly(true);
  
  w3.setTitle("MESSAGE");
  w3.attachValue(message);

  w4.setTitle("LIGHT");
  w4.attachValue(light);

  w5.setTitle("COUNTER");
  w5.setDigits(0);
  w5.setRange(0, 22);
  w5.setSteps(22);
  w5.attachValue(counter);
  
  carrier.begin();
  app.begin(carrier);
  app.addWidget(w0);
  app.addWidget(w1);
  app.addWidget(w2);
  app.addWidget(w3);
  app.addWidget(w4);
  app.addWidget(w5);

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
