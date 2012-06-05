#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QWidget>
#include <QFrame>//
class AbstractData;
class QVBoxLayout;
class QPushButton;
class ClickableLine;
class ClickableLabel;

class InfoWindow : public QFrame//QWidget
{
    Q_OBJECT
public:
    explicit InfoWindow(QWidget *parent = 0,Qt::WindowFlags flag =0);
    void addWidget(ClickableLabel * item, bool isNewLine = false);
    void clean();
    void  setMyLayout();
    void setMaxWidth(int width);
    void forceClose();

signals:
    void leftClickedSignal(AbstractData * data);
    void doubleClickedSignal(AbstractData * data);
    void rightClickedSignal(AbstractData * data);
    void clickedSignal(AbstractData * data, QMouseEvent * event);
    void mouseOverSignal(AbstractData * data);
public slots:
    void close();
    void leftClickedSlot(AbstractData * data);
    void doubleClickedSlot(AbstractData * data);
    void rightClickedSlot(AbstractData * data);
    void clickedSlot(AbstractData * data, QMouseEvent * event);
    void mouseOverSlot(AbstractData * data);
protected:
    bool event ( QEvent * event );
    void resizeEvent(QResizeEvent * /* event */);
    //QWidget * view;
//    QVBoxLayout *topLayout;
    QVBoxLayout *mainLayout;
 //   QVBoxLayout *buttonLayout;
 //   QPushButton *okButton;
    ClickableLine * currentLine;

    std::vector<QWidget *> items;
    int maxWidth;
    bool isKeepingOpen;
    //int w;

};

#endif // INFOWINDOW_H
