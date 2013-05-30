#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <string>
#include <QWidget>
#include <QFrame>//
#include <QComboBox>
#include <QSpinBox>
class AbstractData;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class ClickableLine;
class ClickableLabel;
class OrderDialog;
class ViewManager; //?
class TokenEntity;
class OrderLine;

enum class ExtendedCommand {
  DEFAULT       = 0,
  BUY           = 1,
  SELL          = 2,
  MARCH         = 3,
  GIVE          = 4,
  ACCEPT        = 5,
  EQUIP         = 6,
  UNEQUIP       = 7,
  EJECT         = 8,
  STACK         = 9,
  PROMOTE       = 10,
  EXIT          = 11,
  FORGET     = 12,
  EXTENDED_COMMAND_END      = 99
};
typedef enum ExtendedCommand EXTENDED_COMMAND;


class OrderWindow : public QFrame//QWidget
{
    Q_OBJECT
public:
    explicit OrderWindow(QWidget *parent = 0,Qt::WindowFlags flag =0);
//    void addWidget(ClickableLabel * item);
    void addWidget(QWidget * item);
    QWidget * getWidget(int itemNum);
    void clean();
    void  setMyLayout();
 QComboBox *createComboBox(const QString &text = QString());
 QSpinBox *createSpinBox(int value, int min = 0, int max = 9999);

 
    static OrderWindow * editOrderWindow(TokenEntity * token, OrderLine * order,
        ViewManager * view, ExtendedCommand extendedCommandFlag= ExtendedCommand::DEFAULT);
    static OrderWindow * showOrderWindow(TokenEntity * token, OrderLine * order, 
        ViewManager * view, ExtendedCommand extendedCommandFlag=ExtendedCommand::DEFAULT);
    static OrderWindow * showOrderWindow(TokenEntity * token, std::string order,
        ViewManager * view, ExtendedCommand extendedCommandFlag=ExtendedCommand::DEFAULT);
    
signals:
    void leftClickedSignal(AbstractData * data);
    void doubleClickedSignal(AbstractData * data);
    void rightClickedSignal(AbstractData * data);
    void clickedSignal(AbstractData * data, QMouseEvent * event);
//    void mouseOverSignal(AbstractData * data);
    void orderLineChanged();
public slots:
    void closeWin();
    void cancelChanges();
    void leftClickedSlot(AbstractData * data);
    void doubleClickedSlot(AbstractData * data);
    void rightClickedSlot(AbstractData * data);
    void clickedSlot(AbstractData * data, QMouseEvent * event);
    void mouseOverSlot(AbstractData * data);
    OrderDialog * getDialog(){return dialog_;}
    void setDialog(OrderDialog * dialog){dialog_ =dialog;}
protected:

    void resizeEvent(QResizeEvent * /* event */);
    QVBoxLayout *topLayout;
    QHBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;
    ClickableLine * currentLine;
    std::vector<QWidget *> items;
    OrderDialog * dialog_;

};

#endif // ORDERWINDOW_H
