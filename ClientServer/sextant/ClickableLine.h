#ifndef CLICKABLELINE_H
#define CLICKABLELINE_H
#include <vector>
#include <string>
#include <QFrame>
class AbstractData;
class QHBoxLayout;
class QMouseEvent;
class DataView;
class ClickableLabel;
using namespace std;
class ClickableLine: public QFrame
{
    Q_OBJECT
    public:
    ClickableLine(QWidget * parent);
    ClickableLine(std::string  data, bool isIndented = true,
                  QWidget * parent =0);
    ClickableLine(const char * s, bool isIndented = true,
                  QWidget * parent =0);
    ClickableLine(AbstractData *  data, bool isIndented = true,
                   QWidget * parent =0);
     ClickableLine(std::vector<AbstractData * > data, bool isIndented = true,
                   QWidget * parent =0);
   ClickableLine(bool isIndented = true, QWidget * parent =0);
    void addLabel(ClickableLabel * label);
    void addItems(vector<AbstractData * > data);
    void addString(const char * s);
    void addString(string s);
    void addString(QString s);
    void addInteger(int num);
    void addItem(AbstractData *  item);

    int getWidth();
    void makeOffset(int offset);
    signals:
    void clickedSignal(AbstractData * data, QMouseEvent * event);
    void leftClickedSignal(AbstractData * data);
    void doubleClickedSignal(AbstractData * data);
    void rightClickedSignal(AbstractData * data);
    void mouseOverSignal(AbstractData * data);
public slots:
    void clickedSlot(AbstractData * data, QMouseEvent * event);
    void leftClickedSlot(AbstractData * data);
    void doubleClickedSlot(AbstractData * data);
    void rightClickedSlot(AbstractData * data);
    void mouseOverSlot(AbstractData * data);


    protected:
    void setup(bool isIndented, QWidget * parent);
    QHBoxLayout * layout;
    int index;
};

#endif // CLICKABLELINE_H
