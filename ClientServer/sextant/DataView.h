#ifndef DATAVIEW_H
#define DATAVIEW_H
#include <vector>
#include <QWidget>
#include <QScrollArea>
class AbstractData;
class UnitEntity;
class ListViewer;
class QVBoxLayout;
class QFrame;

class DataView : public QWidget
{
    Q_OBJECT
public:
    DataView();
    void addWidget(QWidget * item);
    void clean();
   void  setScrolledLayout();
signals:
    void leftClickedSignal(AbstractData * data);
    void doubleClickedSignal(AbstractData * data);
    void rightClickedSignal(AbstractData * data);
    void clickedSignal(AbstractData * data, QMouseEvent * event);
    void mouseOverSignal(AbstractData * data);
public slots:
    void leftClickedSlot(AbstractData * data);
    void doubleClickedSlot(AbstractData * data);
    void rightClickedSlot(AbstractData * data);
    void clickedSlot(AbstractData * data, QMouseEvent * event);
    void mouseOverSlot(AbstractData * data);
protected:
    void recursiveShowStack(UnitEntity *unit, ListViewer * list);
    QScrollArea * area;
    QWidget * view;
    QVBoxLayout *topLayout;
    QVBoxLayout *mainLayout;
    std::vector<QWidget *> items;
    int offset;
    int w;
    int index;
};

#endif // DATAVIEW_H
