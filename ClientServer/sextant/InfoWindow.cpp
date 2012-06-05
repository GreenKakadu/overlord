#include "InfoWindow.h"
#include "AbstractData.h"
#include <QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>
#include "ClickableLine.h"
#include "ClickableLabel.h"
#include <QBitmap>

InfoWindow::InfoWindow(QWidget *parent,Qt::WindowFlags flag) :
    QFrame(parent,flag)//QWidget(parent,flag)
{
    //    topLayout = new QVBoxLayout;
    mainLayout = new QVBoxLayout;
    //   buttonLayout = new QVBoxLayout;
    currentLine = 0;
    maxWidth = 800;//default
    isKeepingOpen = false;

::setCustomStyle(this,INFO_WINDOW);
  setAttribute(Qt::WA_Hover,true);
}

void InfoWindow::close()
{
    if(!isKeepingOpen)
    {
        QFrame::close();
    }
}


void InfoWindow::forceClose()
 {
    isKeepingOpen = false;
    QFrame::close();
 }


void InfoWindow::resizeEvent(QResizeEvent * /* event */)
{

    int curve = 20;
    QRegion r1(curve,0,width()-2*curve,height());
    QRegion r2(0,curve,width(),height()-2*curve);
    QRegion rr1(0,0,curve*2,curve*2,QRegion::Ellipse);
    QRegion rr2(0,height()-curve*2,curve*2,curve*2,QRegion::Ellipse);
    QRegion rr3(width()-curve*2,0,curve*2,curve*2,QRegion::Ellipse);
    QRegion rr4(width()-curve*2,height()-curve*2,curve*2,curve*2,QRegion::Ellipse);
    QRegion r = r1.united(r2);
    r=r.united(rr1);
    r=r.united(rr2);
    r=r.united(rr3);
    r=r.united(rr4);
 //   QPixmap pix("comics_cloud.png");
 //   setMask(pix.mask());
    setMask(r);
}

void InfoWindow::setMaxWidth(int width)
{
   maxWidth = width;
}

void InfoWindow::addWidget(ClickableLabel * item, bool isNewLine)
{
//    QFont  font = this->font();
//    font.setPointSize(8);
//    setFont(font);

    if(isNewLine || (currentLine ==0) || currentLine->getWidth() >maxWidth )
    {
        currentLine = new ClickableLine(this);
        mainLayout->addWidget(currentLine);
    }
    currentLine->addLabel(item);
    items.push_back(item);
}


void InfoWindow::clean()
{
    for(vector<QWidget *>::iterator iter=items.begin(); iter != items.end();
    ++iter)
    {
        mainLayout->removeWidget(*iter);
        delete *iter;
    }
    for(int i = 0; i < mainLayout->count(); ++i)
    {
        QLayoutItem * a = mainLayout->itemAt(i);
        if(a)
        {
            mainLayout->removeItem(a);
        }
    }
    items.clear();
    delete mainLayout;

    mainLayout =  new QVBoxLayout;
}

void  InfoWindow::setMyLayout()
{
//    okButton = new QPushButton(this);
//    okButton->setObjectName(QString::fromUtf8("okButton"));
//    okButton->setMaximumSize(QSize(80, 16777215));
//    okButton->setLayoutDirection(Qt::RightToLeft);
//    okButton->setText(QApplication::translate("InfoWindow", "OK", 0, QApplication::UnicodeUTF8));

//    buttonLayout->addWidget(okButton);

//    topLayout->addLayout(mainLayout);
//    topLayout->addLayout(buttonLayout);
//    setLayout(topLayout);
    setLayout(mainLayout);
//    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
}



bool InfoWindow::event ( QEvent * event )
{
        if(event->type() == QEvent::HoverEnter)
        {
        isKeepingOpen = true;
            return true;
        }
        if(event->type() == QEvent::HoverLeave)
        {
            forceClose();
            return true;
        }



    return QWidget::event(event);
}

void InfoWindow::leftClickedSlot(AbstractData * data)
{
     emit leftClickedSignal(data);
}

void InfoWindow::doubleClickedSlot(AbstractData * data)
{
     emit doubleClickedSignal(data);
}



void InfoWindow::rightClickedSlot(AbstractData * data)
{
     if(data)
    {
    emit rightClickedSignal(data);
}
}

void InfoWindow::clickedSlot(AbstractData * data, QMouseEvent * event)
{
     if(data)
    {
    emit clickedSignal(data,event);
}
}

void InfoWindow::mouseOverSlot(AbstractData * data)
{
    emit mouseOverSignal(data);
}

