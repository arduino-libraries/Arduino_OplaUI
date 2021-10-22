# OplaGauge

`OplaGauge` is an abstract class which contains all the common methods that you can use on any type of gauge. When instantiating the object, you should use the subclasses directly:

* `OplaGauge1`
* `OplaGauge2`
* `OplaGauge3`
* `OplaGauge4`

## Methods

### `begin()`

Initializes the gauge.

#### Syntax

```
gauge.begin(display);
```

#### Parameters

* _display_: a variable of type `Adafruit_ST7789` (such as `carrier.display` when using the [`MKRIoTCarrier`](https://www.arduino.cc/reference/en/libraries/arduino_mkriotcarrier/) library)

#### Example

```
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaGauge1 gauge1;

void setup() {
  carrier.begin();
  gauge1.begin(carrier.display);
}
```


### `setTitle()`

Sets the title of the gauge if you want to display a label.

#### Syntax

```
gauge.setTitle(title);
```

#### Parameters

* _title_: a variable of type `String` containing the title

#### Example

```
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaGauge1 gauge1;

void setup() {
  carrier.begin();
  gauge1.begin(carrier.display);
  gauge1.setTitle("TEMPERATURE");
}
```

#### See also

* [getTitle()](#getTitle)


### `getTitle()`

Returns the current title of the gauge.

#### Syntax

```
title = gauge.getTitle();
```

#### See also

* [setTitle()](#setTitle)


### `setSuffix()`

Sets the suffix to be displayed after the value. You can use this for units of measurement.

#### Syntax

```
gauge.setSuffix(suffix);
```

#### Parameters

* _suffix_: a variable of type `String`

#### Example

```
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaGauge1 gauge1;

void setup() {
  carrier.begin();
  gauge1.begin(carrier.display);
  gauge1.setSuffix(" km/h");
}
```

#### See also

* [getSuffix()](#getSuffix)


### `getSuffix()`

Returns the current suffix of the gauge.

#### Syntax

```
suffix = gauge.getSuffix();
```

#### See also

* [setSuffix()](#setSuffix)



### `setRange()`

Sets the minimum and the maximum values displayed by the gauge.

#### Syntax

```
gauge.setRange(min, max);
```

#### Parameters

* _min_: a variable of type `int`
* _max_: a variable of type `int`

#### Example

```
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaGauge1 gauge1;

void setup() {
  carrier.begin();
  gauge1.begin(carrier.display);
  gauge1.setRange(-40, 40);
}
```

#### See also

* [getMin()](#getMin)
* [getMax()](#getMax)


### `getMin()`

Returns the current minimum value of the gauge.

#### Syntax

```
min = gauge.getMin();
```

#### See also

* [setRange()](#setRange)
* [getMax()](#getMax)


### `setSteps()`

Sets the number of steps to draw between minimum and maximum. This only affects how many segments are drawn, but it does not prevent you from assigning any value within the range

#### Syntax

```
gauge.setSteps(steps);
```

#### Parameters

* _steps_: a variable of type `int`

#### Example

```
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaGauge1 gauge1;

void setup() {
  carrier.begin();
  gauge1.begin(carrier.display);
  gauge1.setRange(-40, 40);
  gauge1.setSteps(8);
}
```

#### See also

* [getSteps()](#getSteps)


### `getSteps()`

Returns the current number of steps between minimum and maximum.

#### Syntax

```
steps = gauge.getSteps();
```

#### See also

* [setSteps()](#setSteps)



### `setDigits()`

Sets the precision of the value expressed in number of decimal digits to show.

#### Syntax

```
gauge.setDigits(digits);
```

#### Parameters

* _digits_: a variable of type `int`

#### Example

```
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaGauge1 gauge1;

void setup() {
  carrier.begin();
  gauge1.begin(carrier.display);
  gauge1.setDigits(1);
}
```

#### See also

* [getDigits()](#getDigits)


### `getDigits()`

Returns the current number of decimal digits to show.

#### Syntax

```
digits = gauge.getDigits();
```

#### See also

* [setDigits()](#setDigits)




### `getMax()`

Returns the current maximum value of the gauge.

#### Syntax

```
max = gauge.getMin();
```

#### See also

* [setRange()](#setRange)
* [getMin()](#getMin)



### `setBackgroundColor()`

Sets the background color.

#### Syntax

```
gauge.setBackgroundColor(r, g, b);
```

#### Parameters

* _r_, _g_, _b_ (`uint8_t`): the color expressed in RGB form

#### Example

```
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaGauge1 gauge1;

void setup() {
  carrier.begin();
  gauge1.begin(carrier.display);
  gauge1.setBackgroundColor(0, 0, 0);
}
```

### `clear()`

Fills the area with the background color, making the gauge disappear.

#### Syntax

```
gauge.clear(); 
```

#### Example

```
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaGauge1 gauge1;

void setup() {
  carrier.begin();
  gauge1.begin(carrier.display);
  gauge1.setBackgroundColor(0, 0, 0);
  gauge1.clear();
}
```

### `draw()`

Redraws the gauge. Use this method in your `loop()` function or whenever you need to update the gauge when you change its configuration or value.

#### Syntax

```
gauge.draw(); 
```

#### Example

```
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>

MKRIoTCarrier carrier;
OplaGauge1 gauge1;

void setup() {
  carrier.begin();
  gauge1.begin(carrier.display);
  gauge1.setBackgroundColor(0, 0, 0);
}

void loop(void) {
  gauge1.draw();
  delay(250);
}
```

