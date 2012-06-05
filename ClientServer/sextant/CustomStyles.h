#ifndef CUSTOMSTYLES_H
#define CUSTOMSTYLES_H
#include <QWidget>
enum CustomStyle {
    GRAY_COLOR=0,
    BLUE_COLOR =1,
    RED_COLOR=2,
    LIGHT_GREEN_COLOR=3,
    YELLOW_COLOR=4,
    LIGHT_BLUE_COLOR=5,
    LIGHT_RED_COLOR=6,
    GREEN_COLOR=7,
    INFO_WINDOW = 8,
    POP_UP_MENU = 9,
    ORDER_WINDOW = 10,
    ORDER_TEXT = 11,
    ORDER_COMBO = 12,
    LAST_COLOR= 99
};

void setCustomStyle(QWidget * widget, CustomStyle style);

#endif // CUSTOMSTYLES_H

