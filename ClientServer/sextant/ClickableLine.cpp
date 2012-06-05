#include <iostream>//
#include "ClickableLine.h"
#include "ClickableLabel.h"
#include <QHBoxLayout>
#include "DataView.h"

// QWidget * parent parameter should be used in ClickableLine constructors 
// when  ClickableLine is used not in the list.
// in this case signals from ClickableLine would be delivered directly to parent
// Use of parent parameter in ClickableLine that are in the list will cause 
// duplication of signals





ClickableLine::ClickableLine(QWidget * parent)
{
    setup(false,parent);
}

ClickableLine::ClickableLine(bool isIndented,QWidget * parent)
{
    setup(isIndented,parent);
}

ClickableLine::ClickableLine(vector<AbstractData * > data, bool isIndented,
                             QWidget * parent)
{
    setup(isIndented,parent);
    for(vector<AbstractData * >::iterator iter = data.begin();
    iter != data.end(); ++iter)
    {
        addItem(*iter);
    }
}

ClickableLine::ClickableLine(const char * s, bool isIndented,
                             QWidget * parent)
{
    setup(isIndented,parent);
    addString(s);
}

ClickableLine::ClickableLine(AbstractData *  data, bool isIndented,
                             QWidget * parent)
{
    setup(isIndented,parent);
    addItem(data);
}

ClickableLine::ClickableLine(string   data, bool isIndented,
                             QWidget * parent)
{
    setup(isIndented,parent);
    addString(data);
}

void ClickableLine::setup(bool isIndented, QWidget * parent)
{
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    //layout->setStretchFactor(0,0);
    if(isIndented)
    {
        QLabel * label0 = new QLabel("        ");
        layout->addWidget(label0);
        index = 1;
    }
    else
    {
        index = 0;
    }
    layout->addStretch();
    setLayout(layout);
    if(parent)
    {
        connect(this,SIGNAL(leftClickedSignal(AbstractData*)),
                parent,SLOT(leftClickedSlot(AbstractData*)));
        connect(this,SIGNAL(doubleClickedSignal(AbstractData*)),
                parent,SLOT(doubleClickedSlot(AbstractData*)));
        connect(this,SIGNAL(rightClickedSignal(AbstractData*)),
                parent,SLOT(rightClickedSlot(AbstractData*)));
        connect(this,SIGNAL(clickedSignal(AbstractData*, QMouseEvent * )),
                parent,SLOT(clickedSlot(AbstractData*, QMouseEvent * )));
        connect(this,SIGNAL(mouseOverSignal(AbstractData*)),
                parent,SLOT(mouseOverSlot(AbstractData*)));
    }
    //this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    //this->setMinimumSize(this->minimumSizeHint());
    //setStyleSheet("QFrame {background: yellow; margin: 1px; padding: 0px; spacing 0; border-style: dotted; border-width: 1px; border-color: darkblue;}");

}



void ClickableLine::makeOffset(int offset)
{
  // make string of length  offset
  string s(offset,' ');
  addString(s);
}


void ClickableLine::addLabel(ClickableLabel * label)
{
            layout->insertWidget(index,label,0,0);
            index++;
            connect(label,SIGNAL(leftClicked(AbstractData*)),
                    this,SLOT(leftClickedSlot(AbstractData*)));
            connect(label,SIGNAL(doubleClicked(AbstractData*)),
                    this,SLOT(doubleClickedSlot(AbstractData*)));
            connect(label,SIGNAL(rightClicked(AbstractData*)),
                    this,SLOT(rightClickedSlot(AbstractData*)));
            connect(label,SIGNAL(clicked(AbstractData*,QMouseEvent * )),
                    this,SLOT(clickedSlot(AbstractData*,QMouseEvent * )));
            connect(label,SIGNAL(mouseOver(AbstractData*)),
                    this,SLOT(mouseOverSlot(AbstractData*)));
}



void ClickableLine::addString(const char * str)
{
  addLabel(new ClickableLabel(str));
}

void ClickableLine::addString(string str)
{
  addLabel(new ClickableLabel(str));
}

void ClickableLine::addString(QString str)
{
  addLabel(new ClickableLabel(str));
}

void ClickableLine::addInteger(int num)
{
  addLabel(new ClickableLabel(num));
}

void ClickableLine::addItem(AbstractData *  item)
{
  addLabel(new ClickableLabel(item));
}



void ClickableLine::addItems(vector<AbstractData * > data)
{
    for(vector<AbstractData * >::iterator iter = data.begin();
    iter != data.end(); ++iter)
    {
        addLabel(new ClickableLabel(*iter));
    }
}



int ClickableLine::getWidth()
{
    QSize ws =	minimumSizeHint ();
    //cout<<"InfoWindow: " <<ws.width()<<endl;
    return ws.width();
}



void ClickableLine::clickedSlot(AbstractData * data, QMouseEvent * event)
{
    //std::cout<<"ClickableLine::clickedSlot "<<endl;
    emit clickedSignal(data,event);
}



void ClickableLine::leftClickedSlot(AbstractData * data)
{
    std::cout<<"ClickableLine::leftClickedSlot "<<endl;
    emit leftClickedSignal(data);
}

void ClickableLine::doubleClickedSlot(AbstractData * data)
{
    //std::cout<<"ClickableLine::doubleClickedSlot "<<endl;
    emit doubleClickedSignal(data);
}


void ClickableLine::rightClickedSlot(AbstractData * data)
{
    //std::cout<<"ClickableLine::rightClickedSlot "<<endl;
    emit rightClickedSignal(data);
}

 void ClickableLine::mouseOverSlot(AbstractData * data)
 {
    // cout<<"ClickableLine::mouseOverSignal invoked"<<endl;
     emit mouseOverSignal(data);
 }
