#ifndef _OPLA_WIDGETS_CYCLE_H
#define _OPLA_WIDGETS_CYCLE_H

#include <Arduino_MKRIoTCarrier.h>
#include "widgets.h"
#include <vector>

class CycleWidgetsApp : public WidgetsApp {
    public:
    void addWidget(Widget &w);
    void begin(MKRIoTCarrier &carrier);
    void onButtonDown(short i);
    void showWidget(short i);
    Widget* getCurrentWidget() {
        return (_current_widget > -1) ? _widgets[_current_widget] : nullptr;
    };

    protected:
    std::vector<Widget*> _widgets;
};

#endif
