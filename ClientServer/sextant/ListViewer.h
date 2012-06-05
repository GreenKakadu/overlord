#ifndef LISTVIEWER_H
#define LISTVIEWER_H
#include <vector>
#include <QWidget>
class AbstractData;
class QHBoxLayout;
class QVBoxLayout;
class DataView;
class ClickableLine;
class ClickableLabel;
using namespace std;





class ListViewer : public QWidget
{
    Q_OBJECT
public:
    ListViewer(){}
    ListViewer(ClickableLine * header, DataView * parent=0, bool isActiveHeader =false);
    ListViewer(const char * header, DataView * parent=0, bool isActiveHeader =false);
    ~ListViewer(){}
    void addItem(ClickableLine * item);
    void addList(ListViewer * list);
    signals:
    void leftClickedSignal(AbstractData * data);
    void doubleClickedSignal(AbstractData * data);
    void rightClickedSignal(AbstractData * data);
    void mouseOverSignal(AbstractData * data);
public slots:
    void leftClickedListSlot(AbstractData * data);
    void doubleClickedListSlot(AbstractData * data);
    void rightClickedListSlot(AbstractData * data);
    void headerClickedSlot();
    void mouseOverListSlot(AbstractData * data);
   protected:
    void makeList(ClickableLine * header, DataView * parent, bool isActiveHeader);
    std::vector<QWidget *> items;
    ClickableLine * header_;
    QVBoxLayout * layout;
    bool isExpanded;
    int index;
};

;
#endif // LISTVIEWER_H
