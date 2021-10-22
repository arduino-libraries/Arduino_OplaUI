# Arduino_OplaUI

This Arduino library provides user interface helpers and widgets for the [Arduino Oplà IoT](https://store.arduino.cc/opla-iot-kit) Kit and the [MKR IoT Carrier](http://store.arduino.cc/mkr-iot-carrier).With this library you can use the features of the device (round display, colored LEDs, buzzer and touch buttons) to create an interactive user interface.

The library is centered on the concept of "widget", which is used to represent data on the display and provide interaction. With this library you can add multiple widgets to your Oplà, which will be displayed one at time as if they were pages. This way you can use your Oplà for multiple things at the same time, acting like a control panel: for instance, if you have multiple values (temperature, humidity, light...) you can add a widget for each one.

This library works seamlessly with Arduino Cloud, so you can use it also to visualize and manipulate data that may be related to other devices in your network, using the device-to-device communication feature of Arduino Cloud. This allows you to show temperature from multiple rooms in your house, or control multiple lights.

## How to use the library

### Widgets

There are two ways you can access your widgets:

* The **DirectWidgetsApp** class assigns each widget to a button: in this case you're limited to 5 widgets and you can't use the buttons for interaction
* The **CycleWidgetsApp** class lets you cycle through the widgets using a single button: in this case you can have as many widgets as the board memory allows and you can use the remaining four buttons for interaction

See the examples to learn the syntax.

### Gauges

The library provides also lower level classes to render gauges to display values in different styles. Such classes are automatically called when you create widgets, so you don't need to use them directly unless you want to build a custom UI which does not use the widgets system provided by this library.
