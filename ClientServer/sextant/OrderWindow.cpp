#include "OrderWindow.h"
#include "AbstractData.h"
#include <QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>
#include "ClickableLine.h"
#include "ClickableLabel.h"
#include <QBitmap>
#include "OrderDialog.h"

OrderWindow::OrderWindow(QWidget *parent,Qt::WindowFlags flag) :
    QFrame(parent,flag)//QWidget(parent,flag)
{
    topLayout = new QVBoxLayout;
    mainLayout = new QHBoxLayout;
    buttonLayout = new QHBoxLayout;
    currentLine = 0;
::setCustomStyle(this,ORDER_WINDOW);
//  setAttribute(Qt::WA_Hover,true);
}
OrderWindow * OrderWindow::editOrderWindow(TokenEntity * token, 
        OrderLine * order, ViewManager * view, 
        ExtendedCommand extendedCommandFlag)
{
 OrderDialog * dialog = OrderDialog::findOrderDialog(order);
 if(dialog ==0)
 {
     cerr<< "Dialog is not defined for order "
             <<order->getOrderPrototype()->getKeyword()<<endl;
     return 0;
 }
// cout <<"OrderWindow::editOrderWindow ";
// if(order)
// {
//   cout <<"for order " <<order->getOrderPrototype()->getKeyword();  
// }
//  cout <<endl;
  return dialog->show(token,order,view,extendedCommandFlag);    
}




OrderWindow * OrderWindow::showOrderWindow(TokenEntity * token, 
        OrderLine * order, ViewManager * view, 
        ExtendedCommand extendedCommandFlag)
{
 OrderDialog * dialog = OrderDialog::findOrderDialog(order);
 if(dialog ==0)
 {
     cerr<< "Dialog is not defined for order "
             <<order->getOrderPrototype()->getKeyword()<<endl;
     return 0;
 }
 order->save(cout);
  return dialog->show(token,order,view,extendedCommandFlag);    
}



OrderWindow * OrderWindow::showOrderWindow(TokenEntity * token, string keyword,
        ViewManager * view, EXTENDED_COMMAND extendedCommandFlag)
{

 OrderDialog * dialog = OrderDialog::findOrderDialogByKeyword(keyword);
 if(dialog ==0)
 {
     cerr<< "Dialog is not defined for order "<<keyword<<endl;
     return 0;
 }
 return dialog->show(token,0,view,extendedCommandFlag);     
}




// Button OK pressed
void OrderWindow::closeWin()
{
    cout<<"OrderWindow::close: ";

    
    OrderLine * order = dialog_->getOrderLine();
    if(order)
    {
        //order->printOrderLine(cout);
       order->save(cout);
    }
    cout<<endl;
    emit orderLineChanged();
        QFrame::close();

}



// Button Cancel pressed
void OrderWindow::cancelChanges()
{
    cout<<"OrderWindow::cancel: ";
     cout<<endl;
        QFrame::close();   
}

//void OrderWindow::addWidget(ClickableLabel * item)
//{
//  if(currentLine==0)
//  {
//        currentLine = new ClickableLine(this);
//        mainLayout->addWidget(currentLine);
//  }
//    currentLine->addLabel(item);
//    items.push_back(item);
//}

void OrderWindow::addWidget(QWidget * item)
{

    mainLayout->addWidget(item);
    items.push_back(item);
}

QWidget * OrderWindow::getWidget(int itemNum) 
{
    if (itemNum < items.size()) 
    {
        return items[itemNum];
    } 
    else 
    {
        return 0;
    }
}

void OrderWindow::clean()
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

    mainLayout =  new QHBoxLayout;
}

void  OrderWindow::setMyLayout()
{
    okButton = new QPushButton(this);
    okButton->setObjectName(QString::fromUtf8("okButton"));
    okButton->setMaximumSize(QSize(80, 200));
    okButton->setLayoutDirection(Qt::RightToLeft);
    okButton->setText(QApplication::translate("InfoWindow", "OK", 0, QApplication::UnicodeUTF8));
    buttonLayout->addWidget(okButton);
    cancelButton = new QPushButton(this);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
    cancelButton->setMaximumSize(QSize(80, 200));
    cancelButton->setLayoutDirection(Qt::RightToLeft);
    cancelButton->setText(QApplication::translate("InfoWindow", "Cancel", 0, QApplication::UnicodeUTF8));
    buttonLayout->addWidget(cancelButton);
    topLayout->addLayout(mainLayout);
    topLayout->addLayout(buttonLayout);
    setLayout(topLayout);
//    setLayout(mainLayout);
    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(closeWin()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelChanges()));
}

void OrderWindow::resizeEvent(QResizeEvent * /* event */)
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

void OrderWindow::leftClickedSlot(AbstractData * data)
{
     emit leftClickedSignal(data);
}

void OrderWindow::doubleClickedSlot(AbstractData * data)
{
     emit doubleClickedSignal(data);
}



void OrderWindow::rightClickedSlot(AbstractData * data)
{
     if(data)
    {
    emit rightClickedSignal(data);
}
}

void OrderWindow::clickedSlot(AbstractData * data, QMouseEvent * event)
{
     if(data)
    {
    emit clickedSignal(data,event);
}
}

void OrderWindow::mouseOverSlot(AbstractData * data)
{
    //emit mouseOverSignal(data);
}

QComboBox * OrderWindow::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    //comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    //cBoxes.push_back(comboBox);
    return comboBox;
}

QSpinBox * OrderWindow::createSpinBox(int value, int min, int max)
{
    QSpinBox * box = new QSpinBox;
    box->setMinimum(min);
    box->setMaximum(max);
    box->setValue(value);
    return box;
}
