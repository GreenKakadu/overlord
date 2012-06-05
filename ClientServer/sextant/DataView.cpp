#include "DataView.h"
#include "AbstractData.h"
#include <QVBoxLayout>
#include <QLabel>
#include "UnitEntity.h"
#include "ClickableLabel.h"
#include "ClickableLine.h"
#include "ListViewer.h"
const static int MAX_STACK_DEPTH = 32;
DataView::DataView()
{
    topLayout = new QVBoxLayout;
    area = new QScrollArea();
    view = new QWidget();
    w=0;
    index = 0;
    offset = 0;
}
void DataView::leftClickedSlot(AbstractData * data)
{
    cout << "DataView::leftClickedSlot"<<endl;
     emit leftClickedSignal(data);
}

void DataView::doubleClickedSlot(AbstractData * data)
{
     emit doubleClickedSignal(data);
}



void DataView::rightClickedSlot(AbstractData * data)
{
     if(data)
    {
     //    cout<<"DataView:rightClicked event "<<data->print()<<endl;
    emit rightClickedSignal(data);
}
}
void DataView::clickedSlot(AbstractData * data, QMouseEvent * event)
{
     if(data)
    {
    emit clickedSignal(data,event);
}
}

void DataView::mouseOverSlot(AbstractData * data)
{

    //cout<<"DataView::mouseOverSignal invoked"<<endl;
    emit mouseOverSignal(data);
}



void DataView::addWidget(QWidget * item)
{
    //QSize size = item->size/*Hint*/();
   // w+=size.width();
//    cout << "Size: " /*<< size.width()<< " Total: "
//            <<w<< "("*/<<view->size().width()
//            <<","<<view->sizeHint().width()
//            <<"|"<< sizeHint().width()
//            <<","<<this->size().width() <<")"<<endl;
    mainLayout->addWidget(item);
//    mainLayout->insertWidget(index,item,0,0);
//   index++;
    //view->resize(mainLayout->sizeHint());
    items.push_back(item);
}


void DataView::clean()
{
    for(vector<QWidget *>::iterator iter=items.begin(); iter != items.end();
    ++iter)
    {
        mainLayout->removeWidget(*iter);
        delete *iter;
    }
    //cout << "Count : "<<mainLayout->count()<<endl;
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
    delete view;
    delete area;
    mainLayout =  new QVBoxLayout;
    area = new QScrollArea();
    view = new QWidget();
    //w=0;
    index =0;
}


void  DataView::setScrolledLayout()
{
    mainLayout->addStretch();
    //mainLayout->setSpacing(0);
//    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
//    mainLayout->addItem(verticalSpacer);

//    QLabel *label = new QLabel(view);
//    label->setObjectName(QString::fromUtf8("----------------->.<----------------------"));
//    mainLayout->addWidget(label);
//    QLabel *label2 = new QLabel(view);
//    label2->settext(QString::fromUtf8("----------------->.<----------------------"));
//    mainLayout->addWidget(label2);

    view->setLayout(mainLayout);
    area->setWidget(view);
    view->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    topLayout->addWidget(area, 0,0);
    setLayout(topLayout);
}

void DataView::recursiveShowStack(UnitEntity *unit, ListViewer * list)
{
// If unit is leading - create list and call recursiveShowStack for each follower. add list
// If unit is not leading - create line and add it
    offset++;
    if(offset > MAX_STACK_DEPTH)
    {
        ClickableLine * unitLine = new ClickableLine();
        unitLine->addString("  ...and some more units.");
        list ->addItem(unitLine );
        return;
    }
    if(unit->hasFollowers())
    {
        ClickableLine * unitLine = new ClickableLine();
        if(offset >1)
        {
        unitLine->makeOffset((offset -1) *5);
        }
        unitLine->addLabel(new ClickableLabel(QPixmap(":/new/prefix1/icons/redo.png"),16,16));
        unitLine->addItem(unit);
        ListViewer * newList = new ListViewer(unitLine,0,true);
        vector<UnitEntity *> followers = unit->getAllFollowers();
      for (StackIterator iter = followers.begin(); iter != followers.end(); iter++)
        {
          //newList->addItem(new ClickableLine(*iter));
          recursiveShowStack(*iter, newList);
      }
      list->addList(newList );
    }
    else
    {
        ClickableLine * unitLine = new ClickableLine();
        unitLine->makeOffset(offset *5);
        //unitLine->addItem(new ClickableLabel("      "));
        unitLine->addItem(unit);
        list ->addItem(unitLine );
    }
    offset--;
    //=====
    //addWidget(lv3);

}
