// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>


const char THING_ID[] = "xxx";

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

void onSpeedChange();
void onTemperatureChange();
void onMessageChange();
void onLightChange();

CloudFloat speed;
CloudFloat temperature;
String message;
bool light;

void initProperties(){

  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(speed, READWRITE, ON_CHANGE, onSpeedChange, 1);
  ArduinoCloud.addProperty(temperature, READWRITE, ON_CHANGE, onTemperatureChange, 1);
  ArduinoCloud.addProperty(message, READWRITE, ON_CHANGE, onMessageChange, 1);
  ArduinoCloud.addProperty(light, READWRITE, ON_CHANGE, onLightChange, 1);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);