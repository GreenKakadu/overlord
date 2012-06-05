#include "CustomStyles.h"
void setCustomStyle(QWidget * widget, CustomStyle style)
{
    switch(style)
    {
    case GRAY_COLOR:
        widget->setStyleSheet("QLabel {    border: 1px solid #8E8E8E; \
                border-radius: 5px; padding-left: 5px; padding-right: 5px;\
                background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
                stop: 0 #FFFFFF, stop: 0.4 #E5E5E5, \
                stop: 0.5 #E5E5E5, stop: 1.0 #BDBDBD); \
                color:#FFFFFF;  }");
    break;

    case BLUE_COLOR:
        widget->setStyleSheet("QLabel {    border: 1px solid rgb(22,63,116); \
                      border-radius: 5px;    \
                      padding-left: 5px;\
                      padding-right: 5px;\
                background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
                stop: 0 rgb(94,204,239), stop: 0.4 rgb(12,89,156), \
                stop: 0.5 rgb(12,89,156), stop: 1.0 rgb(67,136,184));\
                color:#FFFFFF;   }\
        QLabel:hover{\
                background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                stop: 0 rgb(84,194,229), stop: 0.4 rgb(22,99,166),\
                stop: 0.5 rgb(22,99,166), stop: 1.0 rgb(107,176,224));}");
    break;

    case RED_COLOR:
        widget->setStyleSheet("QLabel {    border: 1px solid #931E07; \
                border-radius: 5px; padding-left: 5px; padding-right: 5px;\
                background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
                stop: 0 #E6A497, stop: 0.4 #BF503B, \
                stop: 0.5 #B2290E, stop: 1.0 #EC4829); \
                color:blue;font: bold;  }\
         QLabel:hover{\
         widget->background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                stop: 0 #E7A497, stop: 0.4 #D83E1F,\
                stop: 0.5 #C82F11, stop: 1.0 #F99260);}");

break;
case LIGHT_GREEN_COLOR: // light green
    widget->setStyleSheet("QLabel {    border: 1px solid #63B431; \
            border-radius: 5px; padding-left: 5px; padding-right: 5px;\
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
            stop: 0 #FFFFFF, stop: 0.4 #ADDE8C, \
            stop: 0.5 #ADDE8C, stop: 1.0 #59A12D); \
            color:#FFFFFF;  }");
break;
case YELLOW_COLOR:
      widget->setStyleSheet("QLabel {    border: 1px solid #C48215; \
                        border-radius: 5px; padding-left: 5px; padding-right: 5px;\
                        background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
                        stop: 0 #FFFFFF, stop: 0.4 #F6D79D, \
                        stop: 0.5 #F6D79D, stop: 1.0 #F0BD63); \
                        color:#FFFFFF;  }");
            break;
 case LIGHT_RED_COLOR:
      widget->setStyleSheet("QLabel {    border: 1px solid #E38B77; \
                  border-radius: 5px; padding-left: 5px; padding-right: 5px;\
                  background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
                        stop: 0 #FFFFFF, stop: 0.4 #F5D4CC, \
                        stop: 0.5 #F5D4CC, stop: 1.0 #F5D5CE); \
                        color:blue; font:bold; }");
break;
case LIGHT_BLUE_COLOR:
      widget->setStyleSheet("QLabel {    border: 1px solid #658EED; \
             border-radius: 5px; padding-left: 5px; padding-right: 5px;\
             background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
             stop: 0 #FFFFFF, stop: 0.4 #CEDCFA, \
             stop: 0.5 #CEDCFA, stop: 1.0 #B4C9F7); \
             color:#FFFFFF;  }");
break;
case  INFO_WINDOW:
      widget->setStyleSheet("InfoWindow { border: 2px solid #CC8952;\
              border-radius: 20px;\
              background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
              stop: 0 #FFE8C5, stop: 0.4 #F7BE00,\
              stop: 0.5 #F7BE00, stop: 1.0 #ECE451); }");
break;
                             //    setStyleSheet("InfoWindow {\
                             //        background-image:url(:/new/prefix1/images/scroll-pattern.png) ;\
                             //        border: 2px solid yellow;\
                             //        border-radius: 20px;\
                             //        background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                             //        stop: 0 #F9CC2A, stop: 0.4 #FFCC54,\
                             //        stop: 0.5 #FFC965, stop: 1.0 #FFB355);\
                             //        color:blue;\
                             //        }");

case  POP_UP_MENU:
                  widget->setStyleSheet("PopupMenuWindow {    border: 2px solid #F7BE00; \
                  border-radius: 20px; \
                  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
                  stop: 0 #FFFFFF, stop: 0.4 #E5E5E5, \
                  stop: 0.5 #E5E5E5, stop: 1.0 #BDBDBD);  }");
break;

case  ORDER_WINDOW:
                                    widget->setStyleSheet("OrderWindow {    border: 2px solid #658EED; \
                                    border-radius: 20px; \
                                    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
                                            stop: 0 #FFFFFF, stop: 0.4 #CEDCFA, \
                                            stop: 0.5 #CEDCFA, stop: 1.0 #B4C9F7);  }");
break;
case  ORDER_COMBO:
                 widget->setStyleSheet("QComboBox {    border: 2px solid #658EED; \
                                             border-radius: 8px; \
                                    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                                    stop: 0 #FFE8C5, stop: 0.4 #F7BE00,\
                                    stop: 0.5 #F7BE00, stop: 1.0 #ECE451); }");
break;

case ORDER_TEXT:
                   widget->setStyleSheet("QLabel {color:blue; font:bold; font-size: 24px; }");
break;
    default:
        {
        }
    }
}

