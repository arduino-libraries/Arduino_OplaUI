#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
CycleWidgetsApp app;
Gauge1_Widget w0;
Gauge2_Widget w1;
Gauge2_Widget w2;
String_Widget w3;

float humidity, temperature, pressure;
String message;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  w0.setTitle("HUMIDITY");
  w0.setSuffix("%");
  w0.setValue(humidity);
  w0.setReadOnly(true);

  w1.setTitle("TEMPERATURE");
  w1.setSuffix(" C");
  w1.setMin(0);
  w1.setMax(50);
  w1.setSteps(10);
  w1.setValue(temperature);
  w1.setReadOnly(true);

  w2.setTitle("PRESSURE");
  w2.setSuffix(" hPa");
  w2.setDigits(4);
  w2.setMin(700);
  w2.setMax(1200);
  w2.setSteps(10);
  w2.setValue(pressure);
  w2.setReadOnly(true);
  
  w3.setTitle("MESSAGE");
  w3.setValue(message);

  app.begin(carrier);
  app.addWidget(w0);
  app.addWidget(w1);
  app.addWidget(w2);
  app.addWidget(w3);
}

void loop() {
  app.loop();

  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();
  pressure = carrier.Pressure.readPressure()*10;
  message = "Hello world!";
  delay(100);
}
