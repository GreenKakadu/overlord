#include <iostream>//
//#include "AbstractData.h"//
#include "ListViewer.h"
#include "DataView.h"
#include "ClickableLabel.h"
#include "ClickableLine.h"
#include <QVBoxLayout>
//#include <iostream>//
ListViewer::ListViewer(ClickableLine * header, DataView * parent, bool isActiveHeader)
{
        makeList(header,parent,isActiveHeader);
 //     setStyleSheet("QWidget {background: yellow}");
}

ListViewer::ListViewer(const char * header, DataView * parent, bool isActiveHeader)
{
    makeList(new ClickableLine(header,false,parent),parent,isActiveHeader);

}


void ListViewer::makeList(ClickableLine * header, DataView * parent, bool isActiveHeader)
{
    isExpanded = true;
    header_ = header;
    layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0,0,0,0);
::setCustomStyle(header_,LIGHT_RED_COLOR);




    layout->addWidget(header_);
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    setLayout(layout);
    index = 1;
    connect(header_,SIGNAL(leftClickedSignal(AbstractData*)),
            this,SLOT(headerClickedSlot()));
    if(isActiveHeader)
    {
    connect(header_,SIGNAL(leftClickedSignal(AbstractData*)),
            this,SLOT(leftClickedListSlot(AbstractData*)));
    connect(header_,SIGNAL(doubleClickedSignal(AbstractData*)),
            this,SLOT(doubleClickedListSlot(AbstractData*)));
    connect(header_,SIGNAL(rightClickedSignal(AbstractData*)),
            this,SLOT(rightClickedListSlot(AbstractData*)));
    connect(header_,SIGNAL(mouseOverSignal(AbstractData*)),
            this,SLOT(mouseOverListSlot(AbstractData*)));
    }
   if(parent)
    {
        connect(this,SIGNAL(leftClickedSignal(AbstractData*)),
                parent,SLOT(leftClickedSlot(AbstractData*)));
        connect(this,SIGNAL(doubleClickedSignal(AbstractData*)),
                parent,SLOT(doubleClickedSlot(AbstractData*)));
        connect(this,SIGNAL(rightClickedSignal(AbstractData*)),
                parent,SLOT(rightClickedSlot(AbstractData*)));
        connect(this,SIGNAL(mouseOverSignal(AbstractData*)),
                parent,SLOT(mouseOverSlot(AbstractData*)));
    }
}

void ListViewer::addItem(ClickableLine * item)
{
    items.push_back(item);
    layout->insertWidget(index,item,0,0);
    index++;
//    layout->addWidget(item);
    item->show();
            connect(item,SIGNAL(leftClickedSignal(AbstractData*)),
                    this,SLOT(leftClickedListSlot(AbstractData*)));
            connect(item,SIGNAL(doubleClickedSignal(AbstractData*)),
                    this,SLOT(doubleClickedListSlot(AbstractData*)));
            connect(item,SIGNAL(rightClickedSignal(AbstractData*)),
                    this,SLOT(rightClickedListSlot(AbstractData*)));
            connect(item,SIGNAL(mouseOverSignal(AbstractData*)),
                    this,SLOT(mouseOverListSlot(AbstractData*)));
}


void ListViewer::addList(ListViewer * list)
{
    items.push_back(list);
    layout->insertWidget(index,list,0,0);
    index++;
//    layout->addWidget(list);
    list->show();
// Signals from list elements will be intercepted by list header
// and thus will not be delivered to ViewManager.
// So a copy of signals is generated here
            connect(list,SIGNAL(leftClickedSignal(AbstractData*)),
                    this,SLOT(leftClickedListSlot(AbstractData*)));
            connect(list,SIGNAL(doubleClickedSignal(AbstractData*)),
                    this,SLOT(doubleClickedListSlot(AbstractData*)));
            connect(list,SIGNAL(rightClickedSignal(AbstractData*)),
                    this,SLOT(rightClickedListSlot(AbstractData*)));
            connect(list,SIGNAL(mouseOverSignal(AbstractData*)),
                    this,SLOT(mouseOverListSlot(AbstractData*)));
}



void ListViewer::headerClickedSlot()
{
    //std::cout<<"ListViewer::headerClickedSlot "<<endl;
    if(isExpanded)
    {
        isExpanded = false;
//        for(std::vector<ClickableLine *>::iterator iter =items.begin();
        for(std::vector<QWidget *>::iterator iter =items.begin();
        iter !=items.end(); ++iter)
        {
            (*iter)->hide();
        }
    }
    else
    {
        isExpanded = true;
//        for(std::vector<ClickableLine *>::iterator iter =items.begin();
        for(std::vector<QWidget *>::iterator iter =items.begin();
        iter !=items.end(); ++iter)
        {
            (*iter)->show();
        }
   }
}
void ListViewer::leftClickedListSlot(AbstractData * data)
{
   std::cout<<"ListViewer::leftClickedSlot "<<endl;
    if(data)
    {
        emit leftClickedSignal(data);
    }

}


void ListViewer::doubleClickedListSlot(AbstractData * data)
{
   // std::cout<<"ListViewer::doubleClickedSlot "<<endl;
    if(data)
    {
        emit doubleClickedSignal(data);
    }

}



void ListViewer::rightClickedListSlot(AbstractData * data)
{
  // cout<<"ListViewer:rightClicked event "<<endl;
  if(data)
    {
        //cout<<"ListViewer:rightClicked event with data"<<endl;
        emit rightClickedSignal(data);
    }
}



void ListViewer::mouseOverListSlot(AbstractData * data)
{
    //cout<<"ListViewer::mouseOverSignal invoked"<<endl;
        emit mouseOverSignal(data);
}
