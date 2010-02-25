/****************************************************************************
                         Led.cpp

                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#include "Led.h"
#include <QLabel>

Led::Led(QWidget * )
{

    setPixmap(QPixmap(QString::fromUtf8
                      (":/new/prefix1/icons/Gray1.png")).scaled(24,24));
    blinkTimer = new QTimer(this);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blink()));
    isLit_ = false;
    size_ = 24;
    period_ = 500;

}



void Led::setColor(QColor color, bool isBlinking)
{
    color_ = color;
    
        isLit_ = true;
        if(color == Qt::green)
        {
            setPixmap(QPixmap(QString::fromUtf8
                     (":/new/prefix1/icons/Green1.png")).scaled(size_,size_));
        }
        if(color == QColor(Qt::red))
        {
            setPixmap(QPixmap(QString::fromUtf8
                     (":/new/prefix1/icons/Red1.png")).scaled(size_,size_));
        }
        if(color == QColor(Qt::yellow))
        {
            setPixmap(QPixmap(QString::fromUtf8
                     (":/new/prefix1/icons/Yellow1.png")).scaled(size_,size_));
        }

        if(color == QColor(Qt::gray))
        {
            setPixmap(QPixmap(QString::fromUtf8
                     (":/new/prefix1/icons/Gray1.png")).scaled(size_,size_));
        }

    if(isBlinking)
    {
        blinkTimer->start(period_);
    }
    else
    {
        blinkTimer->stop();
    }
      repaint();
}



void Led::setOff()
{
    setPixmap(QPixmap(QString::fromUtf8
                      (":/new/prefix1/icons/Gray1.png")).scaled(size_,size_));
    color_ =Qt::gray;
    repaint();
}



void Led::blink()
{
   if(!isLit_)
    {
        isLit_ = true;
        if(color_ == Qt::green)
        {
          setPixmap(QPixmap(QString::fromUtf8
                   (":/new/prefix1/icons/Green1.png")).scaled(size_,size_));
        }
        if(color_ == Qt::red)
        {
          setPixmap(QPixmap(QString::fromUtf8
                   (":/new/prefix1/icons/Red1.png")).scaled(size_,size_));
        }
        if(color_ == Qt::yellow)
        {
            setPixmap(QPixmap(QString::fromUtf8
                   (":/new/prefix1/icons/Yellow1.png")).scaled(size_,size_));
        }
    }
    else
    {
          setPixmap(QPixmap(QString::fromUtf8
                   (":/new/prefix1/icons/Gray1.png")).scaled(size_,size_));
         isLit_ = false;
   }

}
