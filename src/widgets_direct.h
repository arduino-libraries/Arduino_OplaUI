#ifndef _OPLA_WIDGETS_DIRECT_H
#define _OPLA_WIDGETS_DIRECT_H

#include "widgets.h"

class DirectWidgetsApp : public WidgetsApp {
    public:
    void onButtonDown(short i);
    void setWidget(size_t i, Widget &w);
    void showWidget(short i);
    Widget* getCurrentWidget() {
        return (_current_widget > -1) ? _widgets[_current_widget] : nullptr;
    };

    protected:
    Widget* _widgets[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
};

#endif
