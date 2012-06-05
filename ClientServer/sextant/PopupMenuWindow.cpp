#include "PopupMenuWindow.h"
#include "AbstractData.h"
#include <QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>
#include "ClickableLine.h"
#include "ClickableLabel.h"
#include <QBitmap>

PopupMenuWindow::PopupMenuWindow(QWidget *parent,Qt::WindowFlags flag) :
    QFrame(parent,flag)
{

    mainLayout = new QVBoxLayout;
    //currentLine = 0;
    //maxWidth = 800;//default
    isKeepingOpen = false;

    ::setCustomStyle(this,POP_UP_MENU);
    setAttribute(Qt::WA_Hover,true);
}

void PopupMenuWindow::close()
{
    if(!isKeepingOpen)
    {
        QFrame::close();
    }
}

void PopupMenuWindow::doClose()
{
    //cout<<"PopupMenuWindow::doClose()"<<endl;
        QFrame::close();
}


void PopupMenuWindow::resizeEvent(QResizeEvent * /* event */)
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
    setMask(r);
}

void PopupMenuWindow::setMaxWidth(int width)
{
    maxWidth = width;
}

void PopupMenuWindow::addWidget(ClickableLabel * item, bool isNewLine)
{
//    if(isNewLine || (currentLine ==0) || currentLine->getWidth() >maxWidth )
//    {
//        currentLine = new ClickableLine(this);
//        mainLayout->addWidget(currentLine);
//    }
    currentLine->addLabel(item);
    items.push_back(item);
}
ClickableLabel * PopupMenuWindow::addMenuItem(QString  menuName)
{
    ClickableLabel * menuItem = new ClickableLabel(menuName);
    mainLayout->addWidget(menuItem);
    return menuItem;
}

void PopupMenuWindow::clean()
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

void  PopupMenuWindow::setMyLayout()
{
    setLayout(mainLayout);
}



bool PopupMenuWindow::event ( QEvent * event )
{
    if(event->type() == QEvent::HoverEnter)
    {
    isKeepingOpen = true;
        return true;
    }
    if(event->type() == QEvent::HoverLeave)
    {
        isKeepingOpen = false;
        QFrame::close();
        return true;
    }
return QWidget::event(event);
}


void PopupMenuWindow::leftClickedSlot(AbstractData * data)
{
    emit leftClickedSignal(data);
}


void PopupMenuWindow::doubleClickedSlot(AbstractData * data)
{
    emit doubleClickedSignal(data);
}



void PopupMenuWindow::rightClickedSlot(AbstractData * data)
{
    if(data)
    {
        emit rightClickedSignal(data);
    }
}

void PopupMenuWindow::clickedSlot(AbstractData * data, QMouseEvent * event)
{
    if(data)
    {
        emit clickedSignal(data,event);
    }
}

void PopupMenuWindow::mouseOverSlot(AbstractData * data)
{
    emit mouseOverSignal(data);
}

