/****************************************************************************
                         Led.h
                         Led with blinking
                         -------------------
     begin                : Tue Jul  7 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#ifndef LED_H
#define LED_H
#include <QColor>
#include <QLabel>
#include <QTimer>

class Led : public QLabel
{
    Q_OBJECT
public:
    Led(QWidget * parent =0);
public slots:
    void blink();
public:
    void setColor(QColor color, bool isBlinking = false);
    void setOff();
    void setIconSize(int size){size_ = size;}
    void setBlinkingPeriod(int time){period_ = time;}
 protected:
    QTimer * blinkTimer;
    QColor color_;
    bool isLit_;
    int size_;
    int period_;
};

#endif // LED_H
